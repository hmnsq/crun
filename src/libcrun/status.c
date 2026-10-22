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
  } поки (0)

struct pid_stat
{
  char state;
  long num_threads;
  unsigned long long starttime;
};

static inline bool
is_valid_id_char (char c)
{
  якщо ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    повернути true;
  повернути c == '_' || c == '+' || c == '.' || c == '-';
}

static ціле
validate_id (const char *id, libcrun_error_t *err)
{
  якщо (id == NULL)
    повернути 0;

  якщо (id[0] == '\0')
    повернути crun_make_error (err, 0, "empty container ID");

  якщо (id[0] == '.')
    повернути crun_make_error (err, 0, "invalid container ID `%s`", id);

  для (size_t i = 0; id[i]; i++)
    {
      якщо (! is_valid_id_char (id[i]))
        повернути crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  повернути 0;
}

ціле
get_run_directory (char **out, const char *state_root, libcrun_error_t *err)
{
  ціле ret;
  cleanup_free char *root = NULL;

  якщо (state_root)
    root = xstrdup (state_root);
  якщо (root == NULL)
    {
      const char *runtime_dir = getenv ("XDG_RUNTIME_DIR");
      якщо (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&root, err, runtime_dir, "crun", NULL);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
        }
    }
  якщо (root == NULL)
    root = xstrdup ("/run/crun");

  ret = crun_ensure_directory (root, 0700, false, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  STEAL_POINTER (out, root);

  повернути 0;
}

ціле
get_shared_empty_directory_path (char **out, const char *state_root, libcrun_error_t *err)
{
  cleanup_free char *run_dir = NULL;
  ціле ret;

  ret = get_run_directory (&run_dir, state_root, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = append_paths (out, err, run_dir, ".empty-directory", NULL);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*out, 0555, false, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  повернути 0;
}

ціле
libcrun_get_state_directory (char **out, const char *state_root, const char *id, libcrun_error_t *err)
{
  ціле ret;
  cleanup_free char *path = NULL;
  cleanup_free char *root = NULL;

  ret = validate_id (id, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = get_run_directory (&root, state_root, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = append_paths (&path, err, root, id, NULL);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  STEAL_POINTER (out, path);

  повернути 0;
}

static ціле
get_state_directory_status_file (char **out, const char *state_root, const char *id, libcrun_error_t *err)
{
  cleanup_free char *dir = NULL;
  ціле ret;

  ret = libcrun_get_state_directory (&dir, state_root, id, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  повернути append_paths (out, err, dir, "status", NULL);
}

static ціле
read_pid_stat (pid_t pid, struct pid_stat *st, libcrun_error_t *err)
{
  cleanup_free char *buffer = NULL;
  cleanup_close ціле fd = -1;
  char pid_stat_file[64];
  char *it, *s;
  ціле i, ret;

  ret = snprintf (pid_stat_file, sizeof (pid_stat_file), "/proc/%d/stat", pid);
  якщо (UNLIKELY (ret >= (ціле) sizeof (pid_stat_file)))
    повернути crun_make_error (err, 0, "internal error: static buffer too small");

  fd = open (pid_stat_file, O_RDONLY | O_CLOEXEC);
  якщо (fd < 0)
    {
      /* The process already exited.  */
      якщо (errno == ENOENT || errno == ESRCH)
        {
          memset (st, 0, sizeof (*st));
          повернути 0;
        }
      повернути crun_make_error (err, errno, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &buffer, NULL, err);
  якщо (ret < 0)
    {
      st->starttime = 0;
      st->num_threads = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      повернути 0;
    }

  s = NULL;

  /* Skip the first two arguments.  */
  для (it = buffer; it; it = strchr (it + 1, ')'))
    s = it + 1;

  якщо (s)
    поки (*s == ' ')
      s++;

  якщо (s == NULL || *s == '\0')
    повернути crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  для (it = s + 1, i = 0; i < 19; i++)
    {
      it = strchr (it, ' ');
      якщо (it == NULL)
        повернути crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      якщо (i == 16)
        st->num_threads = strtol (it, NULL, 10);
    }

  errno = 0;
  st->starttime = strtoull (it, NULL, 10);
  якщо (errno != 0)
    повернути crun_make_error (err, errno, "parse process start time");

  повернути 0;
}

ціле
libcrun_write_container_status (const char *state_root, const char *id, libcrun_container_status_t *status,
                                libcrun_error_t *err)
{
  ціле r, ret;
  cleanup_free char *file_tmp = NULL;
  cleanup_free char *file = NULL;
  size_t len;
  cleanup_close ціле fd_write = -1;
  const char *buf = NULL;
  struct pid_stat st;
  const char *tmp;
  cleanup_json_gen json_gen_ctx *gen = NULL;

  ret = get_state_directory_status_file (&file, state_root, id, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = read_pid_stat (status->pid, &st, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  status->process_start_time = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = open (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  якщо (UNLIKELY (fd_write < 0))
    повернути crun_make_error (err, errno, "cannot open status file");

  якщо (! json_gen_init (&gen, NULL))
    повернути crun_make_error (err, 0, "json_gen_init failed");

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

  якщо (status->owner)
    {
      GEN_KEY (gen, "owner");
      GEN_STR (gen, status->owner);
    }

  GEN_KEY (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, status->detached));

  GEN_KEY (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, status->no_new_keyring));

  якщо (status->external_descriptors)
    {
      GEN_KEY (gen, "external_descriptors");
      GEN_STR (gen, status->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = safe_write (fd_write, "status file", buf, len, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  close_and_reset (&fd_write);

  якщо (UNLIKELY (rename (file_tmp, file) < 0))
    повернути crun_make_error (err, errno, "cannot rename status file");

  повернути 0;

gen_error:
  повернути json_gen_error_to_crun_error (r, err);
}

ціле
libcrun_read_container_status (libcrun_container_status_t *status, const char *state_root, const char *id,
                               libcrun_error_t *err)
{
  cleanup_free char *buffer = NULL;
  ціле ret;
  cleanup_free char *file = NULL;
  json_object *doc = NULL;
  json_object *tmp;
  const char *val;

  ret = get_state_directory_status_file (&file, state_root, id, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = read_all_file (file, &buffer, NULL, err);
  якщо (UNLIKELY (ret < 0))
    {

      якщо (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free char *statedir = NULL;
          libcrun_error_t tmp_err;
          ціле tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_root, id, &tmp_err);
          якщо (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          інакше
            {
              tmp_ret = crun_path_exists (statedir, &tmp_err);
              якщо (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              інакше якщо (tmp_ret == 0)
                повернути crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      повернути ret;
    }

  ret = parse_json_file (&doc, buffer, NULL, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  {
    tmp = json_object_object_get (doc, "pid");
    якщо (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        goto exit;
      }
    якщо (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        goto exit;
      }
    status->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    якщо (UNLIKELY (tmp == NULL))
      status->process_start_time = 0; /* backwards compatibility */
    інакше якщо (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        goto exit;
      }
    інакше
      status->process_start_time = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    якщо (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        goto exit;
      }
    val = json_object_get_string (tmp);
    якщо (UNLIKELY (val == NULL))
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
    якщо (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        goto exit;
      }
    val = json_object_get_string (tmp);
    якщо (UNLIKELY (val == NULL))
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
    якщо (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        goto exit;
      }
    val = json_object_get_string (tmp);
    якщо (UNLIKELY (val == NULL))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        goto exit;
      }
    status->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    якщо (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        goto exit;
      }
    val = json_object_get_string (tmp);
    якщо (UNLIKELY (val == NULL))
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
  повернути ret;
}

ціле
libcrun_status_check_directories (const char *state_root, const char *id, libcrun_error_t *err)
{
  cleanup_free char *dir = NULL;
  ціле ret;

  ret = libcrun_get_state_directory (&dir, state_root, id, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = crun_path_exists (dir, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  якщо (ret)
    повернути crun_make_error (err, 0, "container `%s` already exists", id);

  якщо (UNLIKELY (mkdir (dir, 0700) < 0))
    повернути crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  повернути 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#define RMDIRFD_MAX_MOVES (1U << 20)

static ціле
rmdirfd_move_to (DIR **d, const char *name, libcrun_error_t *err)
{
  cleanup_close ціле fd = -1;
  DIR *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  якщо (UNLIKELY (fd < 0))
    повернути crun_make_error (err, errno, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  якщо (UNLIKELY (new_d == NULL))
    повернути crun_make_error (err, errno, "cannot open directory `%s`", name);

  fd = -1;

  closedir (*d);
  *d = new_d;
  повернути 0;
}

static ціле
rmdirfd (const char *namedir, ціле fd, libcrun_error_t *err)
{
  ціле ret;
  cleanup_dir DIR *d = NULL;
  struct dirent *de;
  struct stat root_st;
  bool is_root = true;
  unsigned ціле budget = RMDIRFD_MAX_MOVES;
  __attribute__ ((unused)) cleanup_close ціле fd_cleanup = fd;

  ret = fstat (fd, &root_st);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "stat `%s`", namedir);

  d = fdopendir (fd);
  якщо (d == NULL)
    повернути crun_make_error (err, errno, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  для (;;)
    {
      errno = 0;
      de = readdir (d);
      якщо (de == NULL)
        {
          struct stat st;

          якщо (UNLIKELY (errno != 0))
            повернути crun_make_error (err, errno, "cannot read directory");

          якщо (is_root)
            break;

          якщо (UNLIKELY (--budget == 0))
            повернути crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_to (&d, "..", err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;

          ret = fstat (dirfd (d), &st);
          якщо (UNLIKELY (ret < 0))
            повернути crun_make_error (err, errno, "stat `..`");

          is_root = st.st_dev == root_st.st_dev && st.st_ino == root_st.st_ino;
          continue;
        }

      якщо ((strcmp (de->d_name, ".") == 0) || (strcmp (de->d_name, "..") == 0))
        continue;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      якщо (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          якщо (ret < 0 && errno == EBUSY)
            {
              cleanup_close ціле tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_PATH | O_NOFOLLOW);
              якщо (tfd >= 0)
                {
                  proc_fd_path_t procpath;

                  get_proc_self_fd_path (procpath, tfd);
                  якщо (umount2 (procpath, MNT_DETACH) == 0)
                    goto retry_unlink;
                }
              якщо (! is_root)
                повернути crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          інакше якщо (ret < 0 && errno == ENOTEMPTY)
            {
              якщо (UNLIKELY (--budget == 0))
                повернути crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_to (&d, de->d_name, err);
              якщо (UNLIKELY (ret < 0))
                повернути ret;

              is_root = false;
            }
          інакше якщо (ret < 0 && errno != ENOENT && ! is_root)
            {
              повернути crun_make_error (err, errno, "cannot rm `%s`", de->d_name);
            }
        }
    }

  повернути 0;
}

ціле
libcrun_container_delete_status (const char *state_root, const char *id, libcrun_error_t *err)
{
  ціле ret;
  cleanup_close ціле rundir_dfd = -1;
  cleanup_close ціле dfd = -1;
  cleanup_free char *dir = NULL;

  ret = validate_id (id, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = get_run_directory (&dir, state_root, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  rundir_dfd = TEMP_FAILURE_RETRY (open (dir, O_DIRECTORY | O_PATH | O_CLOEXEC));
  якщо (UNLIKELY (rundir_dfd < 0))
    повернути crun_make_error (err, errno, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  якщо (UNLIKELY (dfd < 0))
    повернути crun_make_error (err, errno, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "cannot rm state directory `%s/%s`", dir, id);

  повернути 0;
}

void
libcrun_free_container_status (libcrun_container_status_t *status)
{
  якщо (status == NULL)
    повернути;
  free (status->cgroup_path);
  free (status->bundle);
  free (status->rootfs);
  free (status->external_descriptors);
  free (status->created);
  free (status->scope);
  free (status->owner);
}

ціле
libcrun_get_containers_list (libcrun_container_list_t **out, const char *state_root, libcrun_error_t *err)
{
  struct dirent *next;
  cleanup_container_list libcrun_container_list_t *tmp = NULL;
  cleanup_free char *root = NULL;
  cleanup_dir DIR *dir = NULL;
  ціле ret;

  *out = NULL;

  ret = get_run_directory (&root, state_root, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  dir = opendir (root);
  якщо (UNLIKELY (dir == NULL))
    повернути crun_make_error (err, errno, "cannot opendir `%s`", root);

  для (next = readdir (dir); next; next = readdir (dir))
    {
      ціле exists;
      cleanup_free char *status_file = NULL;

      libcrun_container_list_t *next_container;

      якщо (next->d_name[0] == '.')
        continue;

      ret = append_paths (&status_file, err, root, next->d_name, "status", NULL);
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      exists = crun_path_exists (status_file, err);
      якщо (exists < 0)
        {
          повернути exists;
        }

      якщо (! exists)
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

  повернути 0;
}

void
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *next;
  поки (list)
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
ціле
libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err)
{
  struct pid_stat st;
  ціле ret;

  /* For backwards compatibility, check start time only if available. */
  якщо (! status->process_start_time)
    повернути 1;

  ret = read_pid_stat (status->pid, &st, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  якщо (status->process_start_time != st.starttime || st.state == 'X')
    повернути 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  якщо (st.state == 'Z' && st.num_threads <= 1)
    повернути 0; /* stopped */

  повернути 1; /* running, created, or paused */
}

ціле
libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err)
{
  ціле ret;

  ret = kill (status->pid, 0);
  якщо (UNLIKELY (ret < 0) && errno != ESRCH)
    повернути crun_make_error (err, errno, "kill");

  якщо (ret == 0)
    повернути libcrun_check_pid_valid (status, err);

  повернути 0; /* stopped */
}

static ціле
get_exec_fifo_path (char **out, const char *state_root, const char *id, libcrun_error_t *err)
{
  cleanup_free char *state_dir = NULL;
  ціле ret;

  ret = libcrun_get_state_directory (&state_dir, state_root, id, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  повернути append_paths (out, err, state_dir, "exec.fifo", NULL);
}

ціле
libcrun_status_create_exec_fifo (const char *state_root, const char *id, libcrun_error_t *err)
{
  cleanup_free char *fifo_path = NULL;
  ціле ret, fd = -1;

  ret = get_exec_fifo_path (&fifo_path, state_root, id, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_path);
  ret = mkfifo (fifo_path, 0600);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "mkfifo");

  fd = open (fifo_path, O_NONBLOCK | O_CLOEXEC);
  якщо (UNLIKELY (fd < 0))
    повернути crun_make_error (err, errno, "cannot open pipe `%s`", fifo_path);

  повернути fd;
}

ціле
libcrun_status_write_exec_fifo (const char *state_root, const char *id, libcrun_error_t *err)
{
  cleanup_free char *fifo_path = NULL;
  char buffer[1] = {
    0,
  };
  cleanup_close ціле fd = -1;
  ціле ret;

  ret = get_exec_fifo_path (&fifo_path, state_root, id, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  fd = open (fifo_path, O_WRONLY | O_CLOEXEC);
  якщо (UNLIKELY (fd < 0))
    повернути crun_make_error (err, errno, "cannot open `%s`", fifo_path);

  ret = unlink (fifo_path);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "unlink `%s`", fifo_path);

  ret = TEMP_FAILURE_RETRY (write (fd, buffer, 1));
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "write to exec.fifo");

  повернути 0;
}

ціле
libcrun_status_has_read_exec_fifo (const char *state_root, const char *id, libcrun_error_t *err)
{
  cleanup_free char *fifo_path = NULL;
  ціле ret;

  ret = get_exec_fifo_path (&fifo_path, state_root, id, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  повернути crun_path_exists (fifo_path, err);
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
  ціле running;
};

ціле
libcrun_container_status_load (libcrun_context_t *context, const char *id, libcrun_status_t **out, libcrun_error_t *err)
{
  libcrun_status_t *st;
  ціле ret;

  st = xmalloc0 (sizeof (*st));

  ret = libcrun_read_container_status (&st->status, context->state_root, id, err);
  якщо (UNLIKELY (ret < 0))
    {
      free (st);
      повернути ret;
    }

  st->id = xstrdup (id);
  st->state_root = xstrdup (context->state_root);

  *out = st;
  повернути 0;
}

void
libcrun_container_status_free (libcrun_status_t *st)
{
  якщо (st == NULL)
    повернути;
  libcrun_free_container_status (&st->status);
  free (st->state_root);
  free (st->id);
  free (st);
}

ціле
libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, ціле *running, libcrun_error_t *err)
{
  якщо (! st->state_resolved)
    {
      const char *state_string = NULL;
      ціле ret;

      ret = libcrun_get_container_state_string (st->id, &st->status, st->state_root, &state_string, &st->running, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      якщо (state_string == NULL)
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;
      інакше якщо (strcmp (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATING;
      інакше якщо (strcmp (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATED;
      інакше якщо (strcmp (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_RUNNING;
      інакше якщо (strcmp (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_PAUSED;
      інакше
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;

      st->state_resolved = true;
    }

  якщо (state)
    *state = st->state;
  якщо (running)
    *running = st->running;
  повернути 0;
}

const char *
libcrun_container_state_to_string (libcrun_container_state_t state)
{
  switch (state)
    {
    case LIBCRUN_CONTAINER_STATUS_CREATING:
      повернути "creating";
    case LIBCRUN_CONTAINER_STATUS_CREATED:
      повернути "created";
    case LIBCRUN_CONTAINER_STATUS_RUNNING:
      повернути "running";
    case LIBCRUN_CONTAINER_STATUS_PAUSED:
      повернути "paused";
    case LIBCRUN_CONTAINER_STATUS_STOPPED:
      повернути "stopped";
    }
  повернути "unknown";
}

pid_t
libcrun_status_get_pid (libcrun_status_t *st)
{
  повернути st->status.pid;
}

const char *
libcrun_status_get_bundle (libcrun_status_t *st)
{
  повернути st->status.bundle;
}

const char *
libcrun_status_get_rootfs (libcrun_status_t *st)
{
  повернути st->status.rootfs;
}

const char *
libcrun_status_get_created (libcrun_status_t *st)
{
  повернути st->status.created;
}

const char *
libcrun_status_get_owner (libcrun_status_t *st)
{
  повернути st->status.owner;
}

const char *
libcrun_status_get_scope (libcrun_status_t *st)
{
  повернути st->status.scope;
}

const char *
libcrun_status_get_external_descriptors (libcrun_status_t *st)
{
  повернути st->status.external_descriptors;
}

ціле
libcrun_container_get_state_string (libcrun_context_t *context, const char *id, const char **out, ціле *running,
                                    libcrun_error_t *err)
{
  cleanup_container_status libcrun_container_status_t status = {};
  ціле local_running;
  ціле ret;

  ret = libcrun_read_container_status (&status, context->state_root, id, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = libcrun_get_container_state_string (id, &status, context->state_root, out, &local_running, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  якщо (running)
    *running = local_running;
  повернути 0;
}

ціле
libcrun_get_state_dir (char **out, const char *state_root, const char *id, libcrun_error_t *err)
{
  повернути libcrun_get_state_directory (out, state_root, id, err);
}

ціле
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out, libcrun_error_t *err)
{
  повернути libcrun_get_containers_list (out, context->state_root, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (sizeof (*it));
  it->cur = l;
  повернути it;
}

bool
libcrun_container_iter_next (libcrun_container_iter_t *it, const char **id)
{
  якщо (it == NULL || it->cur == NULL)
    повернути false;

  якщо (id)
    *id = it->cur->name;
  it->cur = it->cur->next;
  повернути true;
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
