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
  } 동안 (0)

struct pid_stat
{
  char state;
  long num_threads;
  unsigned long long starttime;
};

static inline bool
is_valid_id_char (char c)
{
  만약 ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    반환 true;
  반환 c == '_' || c == '+' || c == '.' || c == '-';
}

static 정수
validate_id (const char *id, libcrun_error_t *err)
{
  만약 (id == NULL)
    반환 0;

  만약 (id[0] == '\0')
    반환 crun_make_error (err, 0, "empty container ID");

  만약 (id[0] == '.')
    반환 crun_make_error (err, 0, "invalid container ID `%s`", id);

  위해 (size_t i = 0; id[i]; i++)
    {
      만약 (! is_valid_id_char (id[i]))
        반환 crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  반환 0;
}

정수
get_run_directory (char **out, const char *state_root, libcrun_error_t *err)
{
  정수 ret;
  cleanup_free char *root = NULL;

  만약 (state_root)
    root = xstrdup (state_root);
  만약 (root == NULL)
    {
      const char *runtime_dir = getenv ("XDG_RUNTIME_DIR");
      만약 (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&root, err, runtime_dir, "crun", NULL);
          만약 (UNLIKELY (ret < 0))
            반환 ret;
        }
    }
  만약 (root == NULL)
    root = xstrdup ("/run/crun");

  ret = crun_ensure_directory (root, 0700, false, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  STEAL_POINTER (out, root);

  반환 0;
}

정수
get_shared_empty_directory_path (char **out, const char *state_root, libcrun_error_t *err)
{
  cleanup_free char *run_dir = NULL;
  정수 ret;

  ret = get_run_directory (&run_dir, state_root, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = append_paths (out, err, run_dir, ".empty-directory", NULL);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*out, 0555, false, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  반환 0;
}

정수
libcrun_get_state_directory (char **out, const char *state_root, const char *id, libcrun_error_t *err)
{
  정수 ret;
  cleanup_free char *path = NULL;
  cleanup_free char *root = NULL;

  ret = validate_id (id, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = get_run_directory (&root, state_root, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = append_paths (&path, err, root, id, NULL);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  STEAL_POINTER (out, path);

  반환 0;
}

static 정수
get_state_directory_status_file (char **out, const char *state_root, const char *id, libcrun_error_t *err)
{
  cleanup_free char *dir = NULL;
  정수 ret;

  ret = libcrun_get_state_directory (&dir, state_root, id, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  반환 append_paths (out, err, dir, "status", NULL);
}

static 정수
read_pid_stat (pid_t pid, struct pid_stat *st, libcrun_error_t *err)
{
  cleanup_free char *buffer = NULL;
  cleanup_close 정수 fd = -1;
  char pid_stat_file[64];
  char *it, *s;
  정수 i, ret;

  ret = snprintf (pid_stat_file, sizeof (pid_stat_file), "/proc/%d/stat", pid);
  만약 (UNLIKELY (ret >= (정수) sizeof (pid_stat_file)))
    반환 crun_make_error (err, 0, "internal error: static buffer too small");

  fd = open (pid_stat_file, O_RDONLY | O_CLOEXEC);
  만약 (fd < 0)
    {
      /* The process already exited.  */
      만약 (errno == ENOENT || errno == ESRCH)
        {
          memset (st, 0, sizeof (*st));
          반환 0;
        }
      반환 crun_make_error (err, errno, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &buffer, NULL, err);
  만약 (ret < 0)
    {
      st->starttime = 0;
      st->num_threads = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      반환 0;
    }

  s = NULL;

  /* Skip the first two arguments.  */
  위해 (it = buffer; it; it = strchr (it + 1, ')'))
    s = it + 1;

  만약 (s)
    동안 (*s == ' ')
      s++;

  만약 (s == NULL || *s == '\0')
    반환 crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  위해 (it = s + 1, i = 0; i < 19; i++)
    {
      it = strchr (it, ' ');
      만약 (it == NULL)
        반환 crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      만약 (i == 16)
        st->num_threads = strtol (it, NULL, 10);
    }

  errno = 0;
  st->starttime = strtoull (it, NULL, 10);
  만약 (errno != 0)
    반환 crun_make_error (err, errno, "parse process start time");

  반환 0;
}

정수
libcrun_write_container_status (const char *state_root, const char *id, libcrun_container_status_t *status,
                                libcrun_error_t *err)
{
  정수 r, ret;
  cleanup_free char *file_tmp = NULL;
  cleanup_free char *file = NULL;
  size_t len;
  cleanup_close 정수 fd_write = -1;
  const char *buf = NULL;
  struct pid_stat st;
  const char *tmp;
  cleanup_json_gen json_gen_ctx *gen = NULL;

  ret = get_state_directory_status_file (&file, state_root, id, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = read_pid_stat (status->pid, &st, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  status->process_start_time = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = open (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  만약 (UNLIKELY (fd_write < 0))
    반환 crun_make_error (err, errno, "cannot open status file");

  만약 (! json_gen_init (&gen, NULL))
    반환 crun_make_error (err, 0, "json_gen_init failed");

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

  만약 (status->owner)
    {
      GEN_KEY (gen, "owner");
      GEN_STR (gen, status->owner);
    }

  GEN_KEY (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, status->detached));

  GEN_KEY (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, status->no_new_keyring));

  만약 (status->external_descriptors)
    {
      GEN_KEY (gen, "external_descriptors");
      GEN_STR (gen, status->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = safe_write (fd_write, "status file", buf, len, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  close_and_reset (&fd_write);

  만약 (UNLIKELY (rename (file_tmp, file) < 0))
    반환 crun_make_error (err, errno, "cannot rename status file");

  반환 0;

gen_error:
  반환 json_gen_error_to_crun_error (r, err);
}

정수
libcrun_read_container_status (libcrun_container_status_t *status, const char *state_root, const char *id,
                               libcrun_error_t *err)
{
  cleanup_free char *buffer = NULL;
  정수 ret;
  cleanup_free char *file = NULL;
  json_object *doc = NULL;
  json_object *tmp;
  const char *val;

  ret = get_state_directory_status_file (&file, state_root, id, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = read_all_file (file, &buffer, NULL, err);
  만약 (UNLIKELY (ret < 0))
    {

      만약 (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free char *statedir = NULL;
          libcrun_error_t tmp_err;
          정수 tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_root, id, &tmp_err);
          만약 (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          아니면
            {
              tmp_ret = crun_path_exists (statedir, &tmp_err);
              만약 (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              아니면 만약 (tmp_ret == 0)
                반환 crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      반환 ret;
    }

  ret = parse_json_file (&doc, buffer, NULL, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  {
    tmp = json_object_object_get (doc, "pid");
    만약 (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        goto exit;
      }
    만약 (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        goto exit;
      }
    status->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    만약 (UNLIKELY (tmp == NULL))
      status->process_start_time = 0; /* backwards compatibility */
    아니면 만약 (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        goto exit;
      }
    아니면
      status->process_start_time = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    만약 (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        goto exit;
      }
    val = json_object_get_string (tmp);
    만약 (UNLIKELY (val == NULL))
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
    만약 (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        goto exit;
      }
    val = json_object_get_string (tmp);
    만약 (UNLIKELY (val == NULL))
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
    만약 (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        goto exit;
      }
    val = json_object_get_string (tmp);
    만약 (UNLIKELY (val == NULL))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        goto exit;
      }
    status->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    만약 (UNLIKELY (tmp == NULL))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        goto exit;
      }
    val = json_object_get_string (tmp);
    만약 (UNLIKELY (val == NULL))
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
  반환 ret;
}

정수
libcrun_status_check_directories (const char *state_root, const char *id, libcrun_error_t *err)
{
  cleanup_free char *dir = NULL;
  정수 ret;

  ret = libcrun_get_state_directory (&dir, state_root, id, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = crun_path_exists (dir, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  만약 (ret)
    반환 crun_make_error (err, 0, "container `%s` already exists", id);

  만약 (UNLIKELY (mkdir (dir, 0700) < 0))
    반환 crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  반환 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#define RMDIRFD_MAX_MOVES (1U << 20)

static 정수
rmdirfd_move_to (DIR **d, const char *name, libcrun_error_t *err)
{
  cleanup_close 정수 fd = -1;
  DIR *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  만약 (UNLIKELY (fd < 0))
    반환 crun_make_error (err, errno, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  만약 (UNLIKELY (new_d == NULL))
    반환 crun_make_error (err, errno, "cannot open directory `%s`", name);

  fd = -1;

  closedir (*d);
  *d = new_d;
  반환 0;
}

static 정수
rmdirfd (const char *namedir, 정수 fd, libcrun_error_t *err)
{
  정수 ret;
  cleanup_dir DIR *d = NULL;
  struct dirent *de;
  struct stat root_st;
  bool is_root = true;
  unsigned 정수 budget = RMDIRFD_MAX_MOVES;
  __attribute__ ((unused)) cleanup_close 정수 fd_cleanup = fd;

  ret = fstat (fd, &root_st);
  만약 (UNLIKELY (ret < 0))
    반환 crun_make_error (err, errno, "stat `%s`", namedir);

  d = fdopendir (fd);
  만약 (d == NULL)
    반환 crun_make_error (err, errno, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  위해 (;;)
    {
      errno = 0;
      de = readdir (d);
      만약 (de == NULL)
        {
          struct stat st;

          만약 (UNLIKELY (errno != 0))
            반환 crun_make_error (err, errno, "cannot read directory");

          만약 (is_root)
            break;

          만약 (UNLIKELY (--budget == 0))
            반환 crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_to (&d, "..", err);
          만약 (UNLIKELY (ret < 0))
            반환 ret;

          ret = fstat (dirfd (d), &st);
          만약 (UNLIKELY (ret < 0))
            반환 crun_make_error (err, errno, "stat `..`");

          is_root = st.st_dev == root_st.st_dev && st.st_ino == root_st.st_ino;
          continue;
        }

      만약 ((strcmp (de->d_name, ".") == 0) || (strcmp (de->d_name, "..") == 0))
        continue;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      만약 (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          만약 (ret < 0 && errno == EBUSY)
            {
              cleanup_close 정수 tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_PATH | O_NOFOLLOW);
              만약 (tfd >= 0)
                {
                  proc_fd_path_t procpath;

                  get_proc_self_fd_path (procpath, tfd);
                  만약 (umount2 (procpath, MNT_DETACH) == 0)
                    goto retry_unlink;
                }
              만약 (! is_root)
                반환 crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          아니면 만약 (ret < 0 && errno == ENOTEMPTY)
            {
              만약 (UNLIKELY (--budget == 0))
                반환 crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_to (&d, de->d_name, err);
              만약 (UNLIKELY (ret < 0))
                반환 ret;

              is_root = false;
            }
          아니면 만약 (ret < 0 && errno != ENOENT && ! is_root)
            {
              반환 crun_make_error (err, errno, "cannot rm `%s`", de->d_name);
            }
        }
    }

  반환 0;
}

정수
libcrun_container_delete_status (const char *state_root, const char *id, libcrun_error_t *err)
{
  정수 ret;
  cleanup_close 정수 rundir_dfd = -1;
  cleanup_close 정수 dfd = -1;
  cleanup_free char *dir = NULL;

  ret = validate_id (id, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = get_run_directory (&dir, state_root, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  rundir_dfd = TEMP_FAILURE_RETRY (open (dir, O_DIRECTORY | O_PATH | O_CLOEXEC));
  만약 (UNLIKELY (rundir_dfd < 0))
    반환 crun_make_error (err, errno, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  만약 (UNLIKELY (dfd < 0))
    반환 crun_make_error (err, errno, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  만약 (UNLIKELY (ret < 0))
    반환 crun_make_error (err, errno, "cannot rm state directory `%s/%s`", dir, id);

  반환 0;
}

void
libcrun_free_container_status (libcrun_container_status_t *status)
{
  만약 (status == NULL)
    반환;
  free (status->cgroup_path);
  free (status->bundle);
  free (status->rootfs);
  free (status->external_descriptors);
  free (status->created);
  free (status->scope);
  free (status->owner);
}

정수
libcrun_get_containers_list (libcrun_container_list_t **out, const char *state_root, libcrun_error_t *err)
{
  struct dirent *next;
  cleanup_container_list libcrun_container_list_t *tmp = NULL;
  cleanup_free char *root = NULL;
  cleanup_dir DIR *dir = NULL;
  정수 ret;

  *out = NULL;

  ret = get_run_directory (&root, state_root, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  dir = opendir (root);
  만약 (UNLIKELY (dir == NULL))
    반환 crun_make_error (err, errno, "cannot opendir `%s`", root);

  위해 (next = readdir (dir); next; next = readdir (dir))
    {
      정수 exists;
      cleanup_free char *status_file = NULL;

      libcrun_container_list_t *next_container;

      만약 (next->d_name[0] == '.')
        continue;

      ret = append_paths (&status_file, err, root, next->d_name, "status", NULL);
      만약 (UNLIKELY (ret < 0))
        반환 ret;

      exists = crun_path_exists (status_file, err);
      만약 (exists < 0)
        {
          반환 exists;
        }

      만약 (! exists)
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

  반환 0;
}

void
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *next;
  동안 (list)
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
정수
libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err)
{
  struct pid_stat st;
  정수 ret;

  /* For backwards compatibility, check start time only if available. */
  만약 (! status->process_start_time)
    반환 1;

  ret = read_pid_stat (status->pid, &st, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  만약 (status->process_start_time != st.starttime || st.state == 'X')
    반환 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  만약 (st.state == 'Z' && st.num_threads <= 1)
    반환 0; /* stopped */

  반환 1; /* running, created, or paused */
}

정수
libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err)
{
  정수 ret;

  ret = kill (status->pid, 0);
  만약 (UNLIKELY (ret < 0) && errno != ESRCH)
    반환 crun_make_error (err, errno, "kill");

  만약 (ret == 0)
    반환 libcrun_check_pid_valid (status, err);

  반환 0; /* stopped */
}

static 정수
get_exec_fifo_path (char **out, const char *state_root, const char *id, libcrun_error_t *err)
{
  cleanup_free char *state_dir = NULL;
  정수 ret;

  ret = libcrun_get_state_directory (&state_dir, state_root, id, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  반환 append_paths (out, err, state_dir, "exec.fifo", NULL);
}

정수
libcrun_status_create_exec_fifo (const char *state_root, const char *id, libcrun_error_t *err)
{
  cleanup_free char *fifo_path = NULL;
  정수 ret, fd = -1;

  ret = get_exec_fifo_path (&fifo_path, state_root, id, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_path);
  ret = mkfifo (fifo_path, 0600);
  만약 (UNLIKELY (ret < 0))
    반환 crun_make_error (err, errno, "mkfifo");

  fd = open (fifo_path, O_NONBLOCK | O_CLOEXEC);
  만약 (UNLIKELY (fd < 0))
    반환 crun_make_error (err, errno, "cannot open pipe `%s`", fifo_path);

  반환 fd;
}

정수
libcrun_status_write_exec_fifo (const char *state_root, const char *id, libcrun_error_t *err)
{
  cleanup_free char *fifo_path = NULL;
  char buffer[1] = {
    0,
  };
  cleanup_close 정수 fd = -1;
  정수 ret;

  ret = get_exec_fifo_path (&fifo_path, state_root, id, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  fd = open (fifo_path, O_WRONLY | O_CLOEXEC);
  만약 (UNLIKELY (fd < 0))
    반환 crun_make_error (err, errno, "cannot open `%s`", fifo_path);

  ret = unlink (fifo_path);
  만약 (UNLIKELY (ret < 0))
    반환 crun_make_error (err, errno, "unlink `%s`", fifo_path);

  ret = TEMP_FAILURE_RETRY (write (fd, buffer, 1));
  만약 (UNLIKELY (ret < 0))
    반환 crun_make_error (err, errno, "write to exec.fifo");

  반환 0;
}

정수
libcrun_status_has_read_exec_fifo (const char *state_root, const char *id, libcrun_error_t *err)
{
  cleanup_free char *fifo_path = NULL;
  정수 ret;

  ret = get_exec_fifo_path (&fifo_path, state_root, id, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  반환 crun_path_exists (fifo_path, err);
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
  정수 running;
};

정수
libcrun_container_status_load (libcrun_context_t *context, const char *id, libcrun_status_t **out, libcrun_error_t *err)
{
  libcrun_status_t *st;
  정수 ret;

  st = xmalloc0 (sizeof (*st));

  ret = libcrun_read_container_status (&st->status, context->state_root, id, err);
  만약 (UNLIKELY (ret < 0))
    {
      free (st);
      반환 ret;
    }

  st->id = xstrdup (id);
  st->state_root = xstrdup (context->state_root);

  *out = st;
  반환 0;
}

void
libcrun_container_status_free (libcrun_status_t *st)
{
  만약 (st == NULL)
    반환;
  libcrun_free_container_status (&st->status);
  free (st->state_root);
  free (st->id);
  free (st);
}

정수
libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, 정수 *running, libcrun_error_t *err)
{
  만약 (! st->state_resolved)
    {
      const char *state_string = NULL;
      정수 ret;

      ret = libcrun_get_container_state_string (st->id, &st->status, st->state_root, &state_string, &st->running, err);
      만약 (UNLIKELY (ret < 0))
        반환 ret;

      만약 (state_string == NULL)
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;
      아니면 만약 (strcmp (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATING;
      아니면 만약 (strcmp (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATED;
      아니면 만약 (strcmp (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_RUNNING;
      아니면 만약 (strcmp (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_PAUSED;
      아니면
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;

      st->state_resolved = true;
    }

  만약 (state)
    *state = st->state;
  만약 (running)
    *running = st->running;
  반환 0;
}

const char *
libcrun_container_state_to_string (libcrun_container_state_t state)
{
  switch (state)
    {
    case LIBCRUN_CONTAINER_STATUS_CREATING:
      반환 "creating";
    case LIBCRUN_CONTAINER_STATUS_CREATED:
      반환 "created";
    case LIBCRUN_CONTAINER_STATUS_RUNNING:
      반환 "running";
    case LIBCRUN_CONTAINER_STATUS_PAUSED:
      반환 "paused";
    case LIBCRUN_CONTAINER_STATUS_STOPPED:
      반환 "stopped";
    }
  반환 "unknown";
}

pid_t
libcrun_status_get_pid (libcrun_status_t *st)
{
  반환 st->status.pid;
}

const char *
libcrun_status_get_bundle (libcrun_status_t *st)
{
  반환 st->status.bundle;
}

const char *
libcrun_status_get_rootfs (libcrun_status_t *st)
{
  반환 st->status.rootfs;
}

const char *
libcrun_status_get_created (libcrun_status_t *st)
{
  반환 st->status.created;
}

const char *
libcrun_status_get_owner (libcrun_status_t *st)
{
  반환 st->status.owner;
}

const char *
libcrun_status_get_scope (libcrun_status_t *st)
{
  반환 st->status.scope;
}

const char *
libcrun_status_get_external_descriptors (libcrun_status_t *st)
{
  반환 st->status.external_descriptors;
}

정수
libcrun_container_get_state_string (libcrun_context_t *context, const char *id, const char **out, 정수 *running,
                                    libcrun_error_t *err)
{
  cleanup_container_status libcrun_container_status_t status = {};
  정수 local_running;
  정수 ret;

  ret = libcrun_read_container_status (&status, context->state_root, id, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = libcrun_get_container_state_string (id, &status, context->state_root, out, &local_running, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  만약 (running)
    *running = local_running;
  반환 0;
}

정수
libcrun_get_state_dir (char **out, const char *state_root, const char *id, libcrun_error_t *err)
{
  반환 libcrun_get_state_directory (out, state_root, id, err);
}

정수
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out, libcrun_error_t *err)
{
  반환 libcrun_get_containers_list (out, context->state_root, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (sizeof (*it));
  it->cur = l;
  반환 it;
}

bool
libcrun_container_iter_next (libcrun_container_iter_t *it, const char **id)
{
  만약 (it == NULL || it->cur == NULL)
    반환 false;

  만약 (id)
    *id = it->cur->name;
  it->cur = it->cur->next;
  반환 true;
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
