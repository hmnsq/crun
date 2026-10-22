/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
 * crun is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * crun is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with crun.  If not, see <http://www.gnu.org/licenses/>.
 */
#define _GNU_SOURCE

#include <config.h>
#include "status.h"
#include "utils.h"
#include "json_gen_utils.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <json-c/json.h>
#include <ocispec/json_common.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <limits.h>

#define STEAL_POINTER(x, y) \
  do                        \
    {                       \
      *x = y;               \
      y = NULL;             \
  } докато (0)

struct pid_stat
{
  char state;
  long num_threads;
  unsigned long long starttime;
};

static inline bool
is_valid_id_char (char c)
{
  ако ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    върни true;
  върни c == '_' || c == '+' || c == '.' || c == '-';
}

static цяло
validate_id (const char *id, libcrun_error_t *err)
{
  ако (id == NULL)
    върни 0;

  ако (id[0] == '\0')
    върни crun_make_error (err, 0, "empty container ID");

  ако (id[0] == '.')
    върни crun_make_error (err, 0, "invalid container ID `%s`", id);

  за (size_t i = 0; id[i]; i++)
    {
      ако (! is_valid_id_char (id[i]))
        върни crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  върни 0;
}

цяло
get_run_directory (char **out, const char *state_root, libcrun_error_t *err)
{
  цяло ret;
  cleanup_free char *root = NULL;

  ако (state_root)
    root = xstrdup (state_root);
  ако (root == NULL)
    {
      const char *runtime_dir = getenv ("XDG_RUNTIME_DIR");
      ако (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&root, err, runtime_dir, "crun", NULL);
          ако (UNLIKELY (ret < 0))
            върни ret;
        }
    }
  ако (root == NULL)
    root = xstrdup ("/run/crun");

  ret = crun_ensure_directory (root, 0700, false, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  STEAL_POINTER (out, root);

  върни 0;
}

цяло
get_shared_empty_directory_path (char **out, const char *state_root, libcrun_error_t *err)
{
  cleanup_free char *run_dir = NULL;
  цяло ret;

  ret = get_run_directory (&run_dir, state_root, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = append_paths (out, err, run_dir, ".empty-directory", NULL);
  ако (UNLIKELY (ret < 0))
    върни ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*out, 0555, false, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  върни 0;
}

цяло
libcrun_get_state_directory (char **out, const char *state_root, const char *id, libcrun_error_t *err)
{
  цяло ret;
  cleanup_free char *path = NULL;
  cleanup_free char *root = NULL;

  ret = validate_id (id, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = get_run_directory (&root, state_root, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = append_paths (&path, err, root, id, NULL);
  ако (UNLIKELY (ret < 0))
    върни ret;

  STEAL_POINTER (out, path);

  върни 0;
}

static цяло
get_state_directory_status_file (char **out, const char *state_root, const char *id, libcrun_error_t *err)
{
  cleanup_free char *dir = NULL;
  цяло ret;

  ret = libcrun_get_state_directory (&dir, state_root, id, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  върни append_paths (out, err, dir, "status", NULL);
}

static цяло
read_pid_stat (pid_t pid, struct pid_stat *st, libcrun_error_t *err)
{
  cleanup_free char *buffer = NULL;
  cleanup_close цяло fd = -1;
  char pid_stat_file[64];
  char *it, *s;
  цяло i, ret;

  ret = snprintf (pid_stat_file, sizeof (pid_stat_file), "/proc/%d/stat", pid);
  ако (UNLIKELY (ret >= (цяло) sizeof (pid_stat_file)))
    върни crun_make_error (err, 0, "internal error: static buffer too small");

  fd = open (pid_stat_file, O_RDONLY | O_CLOEXEC);
  ако (fd < 0)
    {
      /* The process already exited.  */
      ако (errno == ENOENT || errno == ESRCH)
        {
          memset (st, 0, sizeof (*st));
          върни 0;
        }
      върни crun_make_error (err, errno, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &buffer, NULL, err);
  ако (ret < 0)
    {
      st->starttime = 0;
      st->num_threads = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      върни 0;
    }

  s = NULL;

  /* Skip the first two arguments.  */
  за (it = buffer; it; it = strchr (it + 1, ')'))
    s = it + 1;

  ако (s)
    докато (*s == ' ')
      s++;

  ако (s == NULL || *s == '\0')
    върни crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  за (it = s + 1, i = 0; i < 19; i++)
    {
      it = strchr (it, ' ');
      ако (it == NULL)
        върни crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      ако (i == 16)
        st->num_threads = strtol (it, NULL, 10);
    }

  errno = 0;
  st->starttime = strtoull (it, NULL, 10);
  ако (errno != 0)
    върни crun_make_error (err, errno, "parse process start time");

  върни 0;
}

цяло
libcrun_write_container_status (const char *state_root, const char *id, libcrun_container_status_t *status,
                                libcrun_error_t *err)
{
  цяло r, ret;
  cleanup_free char *file_tmp = NULL;
  cleanup_free char *file = NULL;
  size_t len;
  cleanup_close цяло fd_write = -1;
  const char *buf = NULL;
  struct pid_stat st;
  const char *tmp;
  cleanup_json_gen json_gen_ctx *gen = NULL;

  ret = get_state_directory_status_file (&file, state_root, id, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = read_pid_stat (status->pid, &st, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  status->process_start_time = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = open (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  ако (UNLIKELY (fd_write < 0))
    върни crun_make_error (err, errno, "cannot open status file");

  ако (! json_gen_init (&gen, NULL))
    върни crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_KEY (gen, "pid");
  GEN_OR_FAIL (map_int (gen, status->pid));

  GEN_KEY (gen, "process-start-time");
  GEN_OR_FAIL (map_uint (gen, status->process_start_time));

  GEN_KEY (gen, "cgroup-path");
  tmp = status->cgroup_path ? status->cgroup_path : "";
  GEN_STR (gen, tmp);

  GEN_KEY (gen, "scope");
  tmp = status->scope ? status->scope : "";
  GEN_STR (gen, tmp);

  GEN_KEY (gen, "rootfs");
  GEN_STR (gen, status->rootfs);

  GEN_KEY (gen, "systemd-cgroup");
  GEN_OR_FAIL (json_gen_bool (gen, status->systemd_cgroup));

  GEN_KEY (gen, "bundle");
  GEN_STR (gen, status->bundle);

  GEN_KEY (gen, "created");
  GEN_STR (gen, status->created);

  ако (status->owner)
    {
      GEN_KEY (gen, "owner");
      GEN_STR (gen, status->owner);
    }

  GEN_KEY (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, status->detached));

  GEN_KEY (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, status->no_new_keyring));

  ако (status->external_descriptors)
    {
      GEN_KEY (gen, "external_descriptors");
      GEN_STR (gen, status->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = safe_write (fd_write, "status file", buf, len, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  close_and_reset (&fd_write);

  ако (UNLIKELY (rename (file_tmp, file) < 0))
    върни crun_make_error (err, errno, "cannot rename status file");

  върни 0;

gen_error:
  върни json_gen_error_to_crun_error (r, err);
}

цяло
libcrun_read_container_status (libcrun_container_status_t *status, const char *state_root, const char *id,
                               libcrun_error_t *err)
{
  cleanup_free char *buffer = NULL;
  цяло ret;
  cleanup_free char *file = NULL;
  json_object *doc = NULL;
  json_object *tmp;
  const char *val;

  ret = get_state_directory_status_file (&file, state_root, id, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = read_all_file (file, &buffer, NULL, err);
  ако (UNLIKELY (ret < 0))
    {

      ако (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free char *statedir = NULL;
          libcrun_error_t tmp_err;
          цяло tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_root, id, &tmp_err);
          ако (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          иначе
            {
              tmp_ret = crun_path_exists (statedir, &tmp_err);
              ако (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              иначе ако (tmp_ret == 0)
                върни crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      върни ret;
    }

  ret = parse_json_file (&doc, buffer, NULL, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  {
    tmp = json_object_object_get (doc, "pid");
    ако (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        goto exit;
      }
    ако (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        goto exit;
      }
    status->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    ако (UNLIKELY (tmp == NULL))
      status->process_start_time = 0; /* backwards compatibility */
    иначе ако (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        goto exit;
      }
    иначе
      status->process_start_time = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    ако (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        goto exit;
      }
    val = json_object_get_string (tmp);
    ако (UNLIKELY (val == NULL))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` is not a string in `%s`", file);
        goto exit;
      }
    status->cgroup_path = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "scope");
    val = tmp ? json_object_get_string (tmp) : NULL;
    status->scope = val ? xstrdup (val) : NULL;
  }
  {
    tmp = json_object_object_get (doc, "rootfs");
    ако (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        goto exit;
      }
    val = json_object_get_string (tmp);
    ако (UNLIKELY (val == NULL))
      {
        ret = crun_make_error (err, 0, "`rootfs` is not a string in `%s`", file);
        goto exit;
      }
    status->rootfs = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "systemd-cgroup");
    status->systemd_cgroup = tmp != NULL && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "bundle");
    ако (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        goto exit;
      }
    val = json_object_get_string (tmp);
    ако (UNLIKELY (val == NULL))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        goto exit;
      }
    status->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    ако (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        goto exit;
      }
    val = json_object_get_string (tmp);
    ако (UNLIKELY (val == NULL))
      {
        ret = crun_make_error (err, 0, "`created` is not a string in `%s`", file);
        goto exit;
      }
    status->created = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "owner");
    val = tmp ? json_object_get_string (tmp) : NULL;
    status->owner = val ? xstrdup (val) : NULL;
  }
  {
    tmp = json_object_object_get (doc, "detached");
    status->detached = tmp != NULL && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "no-new-keyring");
    status->no_new_keyring = tmp != NULL && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "external_descriptors");
    val = tmp ? json_object_get_string (tmp) : NULL;
    status->external_descriptors = val ? xstrdup (val) : NULL;
  }
  ret = 0;

exit:
  json_object_put (doc);
  върни ret;
}

цяло
libcrun_status_check_directories (const char *state_root, const char *id, libcrun_error_t *err)
{
  cleanup_free char *dir = NULL;
  цяло ret;

  ret = libcrun_get_state_directory (&dir, state_root, id, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = crun_path_exists (dir, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ако (ret)
    върни crun_make_error (err, 0, "container `%s` already exists", id);

  ако (UNLIKELY (mkdir (dir, 0700) < 0))
    върни crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  върни 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#define RMDIRFD_MAX_MOVES (1U << 20)

static цяло
rmdirfd_move_to (DIR **d, const char *name, libcrun_error_t *err)
{
  cleanup_close цяло fd = -1;
  DIR *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  ако (UNLIKELY (fd < 0))
    върни crun_make_error (err, errno, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  ако (UNLIKELY (new_d == NULL))
    върни crun_make_error (err, errno, "cannot open directory `%s`", name);

  fd = -1;

  closedir (*d);
  *d = new_d;
  върни 0;
}

static цяло
rmdirfd (const char *namedir, цяло fd, libcrun_error_t *err)
{
  цяло ret;
  cleanup_dir DIR *d = NULL;
  struct dirent *de;
  struct stat root_st;
  bool is_root = true;
  unsigned цяло budget = RMDIRFD_MAX_MOVES;
  __attribute__ ((unused)) cleanup_close цяло fd_cleanup = fd;

  ret = fstat (fd, &root_st);
  ако (UNLIKELY (ret < 0))
    върни crun_make_error (err, errno, "stat `%s`", namedir);

  d = fdopendir (fd);
  ако (d == NULL)
    върни crun_make_error (err, errno, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  за (;;)
    {
      errno = 0;
      de = readdir (d);
      ако (de == NULL)
        {
          struct stat st;

          ако (UNLIKELY (errno != 0))
            върни crun_make_error (err, errno, "cannot read directory");

          ако (is_root)
            break;

          ако (UNLIKELY (--budget == 0))
            върни crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_to (&d, "..", err);
          ако (UNLIKELY (ret < 0))
            върни ret;

          ret = fstat (dirfd (d), &st);
          ако (UNLIKELY (ret < 0))
            върни crun_make_error (err, errno, "stat `..`");

          is_root = st.st_dev == root_st.st_dev && st.st_ino == root_st.st_ino;
          continue;
        }

      ако ((strcmp (de->d_name, ".") == 0) || (strcmp (de->d_name, "..") == 0))
        continue;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      ако (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          ако (ret < 0 && errno == EBUSY)
            {
              cleanup_close цяло tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_PATH | O_NOFOLLOW);
              ако (tfd >= 0)
                {
                  proc_fd_path_t procpath;

                  get_proc_self_fd_path (procpath, tfd);
                  ако (umount2 (procpath, MNT_DETACH) == 0)
                    goto retry_unlink;
                }
              ако (! is_root)
                върни crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          иначе ако (ret < 0 && errno == ENOTEMPTY)
            {
              ако (UNLIKELY (--budget == 0))
                върни crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_to (&d, de->d_name, err);
              ако (UNLIKELY (ret < 0))
                върни ret;

              is_root = false;
            }
          иначе ако (ret < 0 && errno != ENOENT && ! is_root)
            {
              върни crun_make_error (err, errno, "cannot rm `%s`", de->d_name);
            }
        }
    }

  върни 0;
}

цяло
libcrun_container_delete_status (const char *state_root, const char *id, libcrun_error_t *err)
{
  цяло ret;
  cleanup_close цяло rundir_dfd = -1;
  cleanup_close цяло dfd = -1;
  cleanup_free char *dir = NULL;

  ret = validate_id (id, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = get_run_directory (&dir, state_root, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  rundir_dfd = TEMP_FAILURE_RETRY (open (dir, O_DIRECTORY | O_PATH | O_CLOEXEC));
  ако (UNLIKELY (rundir_dfd < 0))
    върни crun_make_error (err, errno, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  ако (UNLIKELY (dfd < 0))
    върни crun_make_error (err, errno, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  ако (UNLIKELY (ret < 0))
    върни crun_make_error (err, errno, "cannot rm state directory `%s/%s`", dir, id);

  върни 0;
}

void
libcrun_free_container_status (libcrun_container_status_t *status)
{
  ако (status == NULL)
    върни;
  free (status->cgroup_path);
  free (status->bundle);
  free (status->rootfs);
  free (status->external_descriptors);
  free (status->created);
  free (status->scope);
  free (status->owner);
}

цяло
libcrun_get_containers_list (libcrun_container_list_t **out, const char *state_root, libcrun_error_t *err)
{
  struct dirent *next;
  cleanup_container_list libcrun_container_list_t *tmp = NULL;
  cleanup_free char *root = NULL;
  cleanup_dir DIR *dir = NULL;
  цяло ret;

  *out = NULL;

  ret = get_run_directory (&root, state_root, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  dir = opendir (root);
  ако (UNLIKELY (dir == NULL))
    върни crun_make_error (err, errno, "cannot opendir `%s`", root);

  за (next = readdir (dir); next; next = readdir (dir))
    {
      цяло exists;
      cleanup_free char *status_file = NULL;

      libcrun_container_list_t *next_container;

      ако (next->d_name[0] == '.')
        continue;

      ret = append_paths (&status_file, err, root, next->d_name, "status", NULL);
      ако (UNLIKELY (ret < 0))
        върни ret;

      exists = crun_path_exists (status_file, err);
      ако (exists < 0)
        {
          върни exists;
        }

      ако (! exists)
        {
          libcrun_error (errno, "error opening file `%s`", status_file);
          continue;
        }

      next_container = xmalloc (sizeof (libcrun_container_list_t));
      next_container->name = xstrdup (next->d_name);
      next_container->next = tmp;
      tmp = next_container;
    }

  STEAL_POINTER (out, tmp);

  върни 0;
}

void
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *next;
  докато (list)
    {
      next = list->next;
      free (list->name);
      free (list);
      list = next;
    }
}

/* check if the container pid is still valid.
   Returns:
   -1: on errors
    0: pid not valid
    1: pid valid and container in the running/created/paused state
*/
цяло
libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err)
{
  struct pid_stat st;
  цяло ret;

  /* For backwards compatibility, check start time only if available. */
  ако (! status->process_start_time)
    върни 1;

  ret = read_pid_stat (status->pid, &st, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ако (status->process_start_time != st.starttime || st.state == 'X')
    върни 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  ако (st.state == 'Z' && st.num_threads <= 1)
    върни 0; /* stopped */

  върни 1; /* running, created, or paused */
}

цяло
libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err)
{
  цяло ret;

  ret = kill (status->pid, 0);
  ако (UNLIKELY (ret < 0) && errno != ESRCH)
    върни crun_make_error (err, errno, "kill");

  ако (ret == 0)
    върни libcrun_check_pid_valid (status, err);

  върни 0; /* stopped */
}

static цяло
get_exec_fifo_path (char **out, const char *state_root, const char *id, libcrun_error_t *err)
{
  cleanup_free char *state_dir = NULL;
  цяло ret;

  ret = libcrun_get_state_directory (&state_dir, state_root, id, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  върни append_paths (out, err, state_dir, "exec.fifo", NULL);
}

цяло
libcrun_status_create_exec_fifo (const char *state_root, const char *id, libcrun_error_t *err)
{
  cleanup_free char *fifo_path = NULL;
  цяло ret, fd = -1;

  ret = get_exec_fifo_path (&fifo_path, state_root, id, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_path);
  ret = mkfifo (fifo_path, 0600);
  ако (UNLIKELY (ret < 0))
    върни crun_make_error (err, errno, "mkfifo");

  fd = open (fifo_path, O_NONBLOCK | O_CLOEXEC);
  ако (UNLIKELY (fd < 0))
    върни crun_make_error (err, errno, "cannot open pipe `%s`", fifo_path);

  върни fd;
}

цяло
libcrun_status_write_exec_fifo (const char *state_root, const char *id, libcrun_error_t *err)
{
  cleanup_free char *fifo_path = NULL;
  char buffer[1] = {
    0,
  };
  cleanup_close цяло fd = -1;
  цяло ret;

  ret = get_exec_fifo_path (&fifo_path, state_root, id, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  fd = open (fifo_path, O_WRONLY | O_CLOEXEC);
  ако (UNLIKELY (fd < 0))
    върни crun_make_error (err, errno, "cannot open `%s`", fifo_path);

  ret = unlink (fifo_path);
  ако (UNLIKELY (ret < 0))
    върни crun_make_error (err, errno, "unlink `%s`", fifo_path);

  ret = TEMP_FAILURE_RETRY (write (fd, buffer, 1));
  ако (UNLIKELY (ret < 0))
    върни crun_make_error (err, errno, "write to exec.fifo");

  върни 0;
}

цяло
libcrun_status_has_read_exec_fifo (const char *state_root, const char *id, libcrun_error_t *err)
{
  cleanup_free char *fifo_path = NULL;
  цяло ret;

  ret = get_exec_fifo_path (&fifo_path, state_root, id, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  върни crun_path_exists (fifo_path, err);
}

struct libcrun_status_s
{
  libcrun_container_status_t status;
  char *id;
  char *state_root;

  /* The live state requires probing the container (and its cgroup), so it is
     resolved on demand and cached here.  */
  bool state_resolved;
  libcrun_container_state_t state;
  цяло running;
};

цяло
libcrun_container_status_load (libcrun_context_t *context, const char *id, libcrun_status_t **out, libcrun_error_t *err)
{
  libcrun_status_t *st;
  цяло ret;

  st = xmalloc0 (sizeof (*st));

  ret = libcrun_read_container_status (&st->status, context->state_root, id, err);
  ако (UNLIKELY (ret < 0))
    {
      free (st);
      върни ret;
    }

  st->id = xstrdup (id);
  st->state_root = xstrdup (context->state_root);

  *out = st;
  върни 0;
}

void
libcrun_container_status_free (libcrun_status_t *st)
{
  ако (st == NULL)
    върни;
  libcrun_free_container_status (&st->status);
  free (st->state_root);
  free (st->id);
  free (st);
}

цяло
libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, цяло *running, libcrun_error_t *err)
{
  ако (! st->state_resolved)
    {
      const char *state_string = NULL;
      цяло ret;

      ret = libcrun_get_container_state_string (st->id, &st->status, st->state_root, &state_string, &st->running, err);
      ако (UNLIKELY (ret < 0))
        върни ret;

      ако (state_string == NULL)
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;
      иначе ако (strcmp (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATING;
      иначе ако (strcmp (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATED;
      иначе ако (strcmp (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_RUNNING;
      иначе ако (strcmp (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_PAUSED;
      иначе
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;

      st->state_resolved = true;
    }

  ако (state)
    *state = st->state;
  ако (running)
    *running = st->running;
  върни 0;
}

const char *
libcrun_container_state_to_string (libcrun_container_state_t state)
{
  switch (state)
    {
    case LIBCRUN_CONTAINER_STATUS_CREATING:
      върни "creating";
    case LIBCRUN_CONTAINER_STATUS_CREATED:
      върни "created";
    case LIBCRUN_CONTAINER_STATUS_RUNNING:
      върни "running";
    case LIBCRUN_CONTAINER_STATUS_PAUSED:
      върни "paused";
    case LIBCRUN_CONTAINER_STATUS_STOPPED:
      върни "stopped";
    }
  върни "unknown";
}

pid_t
libcrun_status_get_pid (libcrun_status_t *st)
{
  върни st->status.pid;
}

const char *
libcrun_status_get_bundle (libcrun_status_t *st)
{
  върни st->status.bundle;
}

const char *
libcrun_status_get_rootfs (libcrun_status_t *st)
{
  върни st->status.rootfs;
}

const char *
libcrun_status_get_created (libcrun_status_t *st)
{
  върни st->status.created;
}

const char *
libcrun_status_get_owner (libcrun_status_t *st)
{
  върни st->status.owner;
}

const char *
libcrun_status_get_scope (libcrun_status_t *st)
{
  върни st->status.scope;
}

const char *
libcrun_status_get_external_descriptors (libcrun_status_t *st)
{
  върни st->status.external_descriptors;
}

цяло
libcrun_container_get_state_string (libcrun_context_t *context, const char *id, const char **out, цяло *running,
                                    libcrun_error_t *err)
{
  cleanup_container_status libcrun_container_status_t status = {};
  цяло local_running;
  цяло ret;

  ret = libcrun_read_container_status (&status, context->state_root, id, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ret = libcrun_get_container_state_string (id, &status, context->state_root, out, &local_running, err);
  ако (UNLIKELY (ret < 0))
    върни ret;

  ако (running)
    *running = local_running;
  върни 0;
}

цяло
libcrun_get_state_dir (char **out, const char *state_root, const char *id, libcrun_error_t *err)
{
  върни libcrun_get_state_directory (out, state_root, id, err);
}

цяло
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out, libcrun_error_t *err)
{
  върни libcrun_get_containers_list (out, context->state_root, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (sizeof (*it));
  it->cur = l;
  върни it;
}

bool
libcrun_container_iter_next (libcrun_container_iter_t *it, const char **id)
{
  ако (it == NULL || it->cur == NULL)
    върни false;

  ако (id)
    *id = it->cur->name;
  it->cur = it->cur->next;
  върни true;
}

void
libcrun_container_iter_free (libcrun_container_iter_t *it)
{
  free (it);
}

void
libcrun_container_list_free (libcrun_container_list_t *l)
{
  libcrun_free_containers_list (l);
}
