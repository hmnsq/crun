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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "status.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "json_gen_utils.h"
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <json-c/json.h>
#ଅନ୍ତର୍ଭାଵ <ocispec/json_common.h>
#ଅନ୍ତର୍ଭାଵ <sys/mount.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <dirent.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>
#ଅନ୍ତର୍ଭାଵ <limits.h>

#define STEAL_POINTER(x, y) \
  କର                        \
    {                       \
      *x = y;               \
      y = ଖାଲି;             \
  } ଯେତେବେଳେ (0)

ସଂରଚନା pid_stat
{
  ଅକ୍ଷର state;
  long ଅଙ୍କ_threads;
  unsigned long long starttime;
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
is_valid_id_char (ଅକ୍ଷର c)
{
  ଯଦି ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    ଫେରନ୍ତୁ true;
  ଫେରନ୍ତୁ c == '_' || c == '+' || c == '.' || c == '-';
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
validate_id (ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err)
{
  ଯଦି (id == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  ଯଦି (id[0] == '\0')
    ଫେରନ୍ତୁ crun_make_error (err, 0, "empty container ID");

  ଯଦି (id[0] == '.')
    ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid container ID `%s`", id);

  ପାଇଁ (size_t i = 0; id[i]; i++)
    {
      ଯଦି (! is_valid_id_char (id[i]))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_run_directory (ଅକ୍ଷର **out, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  cleanup_free ଅକ୍ଷର *ମୂଳ = ଖାଲି;

  ଯଦି (state_ମୂଳ)
    ମୂଳ = xstrdup (state_ମୂଳ);
  ଯଦି (ମୂଳ == ଖାଲି)
    {
      ଅଚଳ ଅକ୍ଷର *runtime_dir = getenv ("XDG_RUNTIME_DIR");
      ଯଦି (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&ମୂଳ, err, runtime_dir, "crun", ଖାଲି);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
    }
  ଯଦି (ମୂଳ == ଖାଲି)
    ମୂଳ = xstrdup ("/run/crun");

  ret = crun_ensure_directory (ମୂଳ, 0700, false, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  STEAL_POINTER (out, ମୂଳ);

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_shared_empty_directory_path (ଅକ୍ଷର **out, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *run_dir = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = get_run_directory (&run_dir, state_ମୂଳ, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = append_paths (out, err, run_dir, ".empty-directory", ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*out, 0555, false, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_get_state_directory (ଅକ୍ଷର **out, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  cleanup_free ଅକ୍ଷର *path = ଖାଲି;
  cleanup_free ଅକ୍ଷର *ମୂଳ = ଖାଲି;

  ret = validate_id (id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = get_run_directory (&ମୂଳ, state_ମୂଳ, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = append_paths (&path, err, ମୂଳ, id, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  STEAL_POINTER (out, path);

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_state_directory_status_file (ଅକ୍ଷର **out, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *dir = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = libcrun_get_state_directory (&dir, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ append_paths (out, err, dir, "status", ଖାଲି);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
read_pid_stat (pid_t pid, ସଂରଚନା pid_stat *st, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *buffer = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  ଅକ୍ଷର pid_stat_file[64];
  ଅକ୍ଷର *it, *s;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ret;

  ret = snprintf (pid_stat_file, ଆକାର (pid_stat_file), "/proc/%d/stat", pid);
  ଯଦି (UNLIKELY (ret >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (pid_stat_file)))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

  fd = open (pid_stat_file, O_RDONLY | O_CLOEXEC);
  ଯଦି (fd < 0)
    {
      /* The process already exited.  */
      ଯଦି (errno == ENOENT || errno == ESRCH)
        {
          memset (st, 0, ଆକାର (*st));
          ଫେରନ୍ତୁ 0;
        }
      ଫେରନ୍ତୁ crun_make_error (err, errno, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &buffer, ଖାଲି, err);
  ଯଦି (ret < 0)
    {
      st->starttime = 0;
      st->ଅଙ୍କ_threads = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      ଫେରନ୍ତୁ 0;
    }

  s = ଖାଲି;

  /* Skip the first two arguments.  */
  ପାଇଁ (it = buffer; it; it = strchr (it + 1, ')'))
    s = it + 1;

  ଯଦି (s)
    ଯେତେବେଳେ (*s == ' ')
      s++;

  ଯଦି (s == ଖାଲି || *s == '\0')
    ଫେରନ୍ତୁ crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  ପାଇଁ (it = s + 1, i = 0; i < 19; i++)
    {
      it = strchr (it, ' ');
      ଯଦି (it == ଖାଲି)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      ଯଦି (i == 16)
        st->ଅଙ୍କ_threads = strtol (it, ଖାଲି, 10);
    }

  errno = 0;
  st->starttime = strtoull (it, ଖାଲି, 10);
  ଯଦି (errno != 0)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "parse process start time");

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_write_container_status (ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର *id, libcrun_container_status_t *status,
                                libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା r, ret;
  cleanup_free ଅକ୍ଷର *file_tmp = ଖାଲି;
  cleanup_free ଅକ୍ଷର *file = ଖାଲି;
  size_t len;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd_write = -1;
  ଅଚଳ ଅକ୍ଷର *buf = ଖାଲି;
  ସଂରଚନା pid_stat st;
  ଅଚଳ ଅକ୍ଷର *tmp;
  cleanup_json_gen json_gen_ctx *gen = ଖାଲି;

  ret = get_state_directory_status_file (&file, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = read_pid_stat (status->pid, &st, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  status->process_start_time = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = open (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  ଯଦି (UNLIKELY (fd_write < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot open status file");

  ଯଦି (! json_gen_init (&gen, ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ଚାବି (gen, "pid");
  GEN_OR_FAIL (map_int (gen, status->pid));

  GEN_ଚାବି (gen, "process-start-time");
  GEN_OR_FAIL (map_uint (gen, status->process_start_time));

  GEN_ଚାବି (gen, "cgroup-path");
  tmp = status->cgroup_path ? status->cgroup_path : "";
  GEN_STR (gen, tmp);

  GEN_ଚାବି (gen, "scope");
  tmp = status->scope ? status->scope : "";
  GEN_STR (gen, tmp);

  GEN_ଚାବି (gen, "rootfs");
  GEN_STR (gen, status->rootfs);

  GEN_ଚାବି (gen, "systemd-cgroup");
  GEN_OR_FAIL (json_gen_bool (gen, status->systemd_cgroup));

  GEN_ଚାବି (gen, "bundle");
  GEN_STR (gen, status->bundle);

  GEN_ଚାବି (gen, "created");
  GEN_STR (gen, status->created);

  ଯଦି (status->owner)
    {
      GEN_ଚାବି (gen, "owner");
      GEN_STR (gen, status->owner);
    }

  GEN_ଚାବି (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, status->detached));

  GEN_ଚାବି (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, status->no_new_keyring));

  ଯଦି (status->external_descriptors)
    {
      GEN_ଚାବି (gen, "external_descriptors");
      GEN_STR (gen, status->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = safe_write (fd_write, "status file", buf, len, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  close_and_reset (&fd_write);

  ଯଦି (UNLIKELY (rename (file_tmp, file) < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot rename status file");

  ଫେରନ୍ତୁ 0;

gen_error:
  ଫେରନ୍ତୁ json_gen_error_to_crun_error (r, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_read_container_status (libcrun_container_status_t *status, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର *id,
                               libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *buffer = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  cleanup_free ଅକ୍ଷର *file = ଖାଲି;
  json_object *doc = ଖାଲି;
  json_object *tmp;
  ଅଚଳ ଅକ୍ଷର *val;

  ret = get_state_directory_status_file (&file, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = read_all_file (file, &buffer, ଖାଲି, err);
  ଯଦି (UNLIKELY (ret < 0))
    {

      ଯଦି (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free ଅକ୍ଷର *statedir = ଖାଲି;
          libcrun_error_t tmp_err;
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_ମୂଳ, id, &tmp_err);
          ଯଦି (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          ଅନ୍ୟଥା
            {
              tmp_ret = crun_path_exists (statedir, &tmp_err);
              ଯଦି (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              ଅନ୍ୟଥା ଯଦି (tmp_ret == 0)
                ଫେରନ୍ତୁ crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      ଫେରନ୍ତୁ ret;
    }

  ret = parse_json_file (&doc, buffer, ଖାଲି, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  {
    tmp = json_object_object_get (doc, "pid");
    ଯଦି (UNLIKELY (tmp == ଖାଲି))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        ଯାଅ exit;
      }
    ଯଦି (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        ଯାଅ exit;
      }
    status->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    ଯଦି (UNLIKELY (tmp == ଖାଲି))
      status->process_start_time = 0; /* backwards compatibility */
    ଅନ୍ୟଥା ଯଦି (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        ଯାଅ exit;
      }
    ଅନ୍ୟଥା
      status->process_start_time = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    ଯଦି (UNLIKELY (tmp == ଖାଲି))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        ଯାଅ exit;
      }
    val = json_object_get_string (tmp);
    ଯଦି (UNLIKELY (val == ଖାଲି))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` is not a string in `%s`", file);
        ଯାଅ exit;
      }
    status->cgroup_path = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "scope");
    val = tmp ? json_object_get_string (tmp) : ଖାଲି;
    status->scope = val ? xstrdup (val) : ଖାଲି;
  }
  {
    tmp = json_object_object_get (doc, "rootfs");
    ଯଦି (UNLIKELY (tmp == ଖାଲି))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        ଯାଅ exit;
      }
    val = json_object_get_string (tmp);
    ଯଦି (UNLIKELY (val == ଖାଲି))
      {
        ret = crun_make_error (err, 0, "`rootfs` is not a string in `%s`", file);
        ଯାଅ exit;
      }
    status->rootfs = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "systemd-cgroup");
    status->systemd_cgroup = tmp != ଖାଲି && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "bundle");
    ଯଦି (UNLIKELY (tmp == ଖାଲି))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        ଯାଅ exit;
      }
    val = json_object_get_string (tmp);
    ଯଦି (UNLIKELY (val == ଖାଲି))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        ଯାଅ exit;
      }
    status->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    ଯଦି (UNLIKELY (tmp == ଖାଲି))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        ଯାଅ exit;
      }
    val = json_object_get_string (tmp);
    ଯଦି (UNLIKELY (val == ଖାଲି))
      {
        ret = crun_make_error (err, 0, "`created` is not a string in `%s`", file);
        ଯାଅ exit;
      }
    status->created = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "owner");
    val = tmp ? json_object_get_string (tmp) : ଖାଲି;
    status->owner = val ? xstrdup (val) : ଖାଲି;
  }
  {
    tmp = json_object_object_get (doc, "detached");
    status->detached = tmp != ଖାଲି && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "no-new-keyring");
    status->no_new_keyring = tmp != ଖାଲି && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "external_descriptors");
    val = tmp ? json_object_get_string (tmp) : ଖାଲି;
    status->external_descriptors = val ? xstrdup (val) : ଖାଲି;
  }
  ret = 0;

exit:
  json_object_put (doc);
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_status_check_directories (ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *dir = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = libcrun_get_state_directory (&dir, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = crun_path_exists (dir, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (ret)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "container `%s` already exists", id);

  ଯଦି (UNLIKELY (mkdir (dir, 0700) < 0))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  ଫେରନ୍ତୁ 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#define RMDIRFD_MAX_MOVES (1U << 20)

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
rmdirfd_move_to (DIR **d, ଅଚଳ ଅକ୍ଷର *name, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  DIR *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  ଯଦି (UNLIKELY (new_d == ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot open directory `%s`", name);

  fd = -1;

  closedir (*d);
  *d = new_d;
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
rmdirfd (ଅଚଳ ଅକ୍ଷର *namedir, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  cleanup_dir DIR *d = ଖାଲି;
  ସଂରଚନା dirent *de;
  ସଂରଚନା stat ମୂଳ_st;
  bool is_ମୂଳ = true;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା budget = RMDIRFD_MAX_MOVES;
  __attribute__ ((unused)) cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd_cleanup = fd;

  ret = fstat (fd, &ମୂଳ_st);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "stat `%s`", namedir);

  d = fdopendir (fd);
  ଯଦି (d == ଖାଲି)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  ପାଇଁ (;;)
    {
      errno = 0;
      de = readdir (d);
      ଯଦି (de == ଖାଲି)
        {
          ସଂରଚନା stat st;

          ଯଦି (UNLIKELY (errno != 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot read directory");

          ଯଦି (is_ମୂଳ)
            ଭାଙ୍ଗ;

          ଯଦି (UNLIKELY (--budget == 0))
            ଫେରନ୍ତୁ crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_to (&d, "..", err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;

          ret = fstat (dirfd (d), &st);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "stat `..`");

          is_ମୂଳ = st.st_dev == ମୂଳ_st.st_dev && st.st_ino == ମୂଳ_st.st_ino;
          ଜାରି;
        }

      ଯଦି ((strcmp (de->d_name, ".") == 0) || (strcmp (de->d_name, "..") == 0))
        ଜାରି;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      ଯଦି (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          ଯଦି (ret < 0 && errno == EBUSY)
            {
              cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_PATH | O_NOFOLLOW);
              ଯଦି (tfd >= 0)
                {
                  proc_fd_path_t procpath;

                  get_proc_self_fd_path (procpath, tfd);
                  ଯଦି (umount2 (procpath, MNT_DETACH) == 0)
                    ଯାଅ retry_unlink;
                }
              ଯଦି (! is_ମୂଳ)
                ଫେରନ୍ତୁ crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          ଅନ୍ୟଥା ଯଦି (ret < 0 && errno == ENOTEMPTY)
            {
              ଯଦି (UNLIKELY (--budget == 0))
                ଫେରନ୍ତୁ crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_to (&d, de->d_name, err);
              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ ret;

              is_ମୂଳ = false;
            }
          ଅନ୍ୟଥା ଯଦି (ret < 0 && errno != ENOENT && ! is_ମୂଳ)
            {
              ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot rm `%s`", de->d_name);
            }
        }
    }

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_delete_status (ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା rundir_dfd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dfd = -1;
  cleanup_free ଅକ୍ଷର *dir = ଖାଲି;

  ret = validate_id (id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = get_run_directory (&dir, state_ମୂଳ, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  rundir_dfd = TEMP_FAILURE_RETRY (open (dir, O_DIRECTORY | O_PATH | O_CLOEXEC));
  ଯଦି (UNLIKELY (rundir_dfd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  ଯଦି (UNLIKELY (dfd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot rm state directory `%s/%s`", dir, id);

  ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ
libcrun_free_container_status (libcrun_container_status_t *status)
{
  ଯଦି (status == ଖାଲି)
    ଫେରନ୍ତୁ;
  ମୁକ୍ତ (status->cgroup_path);
  ମୁକ୍ତ (status->bundle);
  ମୁକ୍ତ (status->rootfs);
  ମୁକ୍ତ (status->external_descriptors);
  ମୁକ୍ତ (status->created);
  ମୁକ୍ତ (status->scope);
  ମୁକ୍ତ (status->owner);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_get_containers_list (libcrun_container_list_t **out, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, libcrun_error_t *err)
{
  ସଂରଚନା dirent *next;
  cleanup_container_list libcrun_container_list_t *tmp = ଖାଲି;
  cleanup_free ଅକ୍ଷର *ମୂଳ = ଖାଲି;
  cleanup_dir DIR *dir = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  *out = ଖାଲି;

  ret = get_run_directory (&ମୂଳ, state_ମୂଳ, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  dir = opendir (ମୂଳ);
  ଯଦି (UNLIKELY (dir == ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot opendir `%s`", ମୂଳ);

  ପାଇଁ (next = readdir (dir); next; next = readdir (dir))
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା exists;
      cleanup_free ଅକ୍ଷର *status_file = ଖାଲି;

      libcrun_container_list_t *next_container;

      ଯଦି (next->d_name[0] == '.')
        ଜାରି;

      ret = append_paths (&status_file, err, ମୂଳ, next->d_name, "status", ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      exists = crun_path_exists (status_file, err);
      ଯଦି (exists < 0)
        {
          ଫେରନ୍ତୁ exists;
        }

      ଯଦି (! exists)
        {
          libcrun_error (errno, "error opening file `%s`", status_file);
          ଜାରି;
        }

      next_container = xmalloc (ଆକାର (libcrun_container_list_t));
      next_container->name = xstrdup (next->d_name);
      next_container->next = tmp;
      tmp = next_container;
    }

  STEAL_POINTER (out, tmp);

  ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *next;
  ଯେତେବେଳେ (list)
    {
      next = list->next;
      ମୁକ୍ତ (list->name);
      ମୁକ୍ତ (list);
      list = next;
    }
}

/* check if the container pid is still valid.
   Returns:
   -1: on errors
    0: pid not valid
    1: pid valid and container in the running/created/paused state
*/
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err)
{
  ସଂରଚନା pid_stat st;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  /* For backwards compatibility, check start time only if available. */
  ଯଦି (! status->process_start_time)
    ଫେରନ୍ତୁ 1;

  ret = read_pid_stat (status->pid, &st, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (status->process_start_time != st.starttime || st.state == 'X')
    ଫେରନ୍ତୁ 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  ଯଦି (st.state == 'Z' && st.ଅଙ୍କ_threads <= 1)
    ଫେରନ୍ତୁ 0; /* stopped */

  ଫେରନ୍ତୁ 1; /* running, created, or paused */
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = kill (status->pid, 0);
  ଯଦି (UNLIKELY (ret < 0) && errno != ESRCH)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "kill");

  ଯଦି (ret == 0)
    ଫେରନ୍ତୁ libcrun_check_pid_valid (status, err);

  ଫେରନ୍ତୁ 0; /* stopped */
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_exec_fifo_path (ଅକ୍ଷର **out, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *state_dir = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = libcrun_get_state_directory (&state_dir, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ append_paths (out, err, state_dir, "exec.fifo", ଖାଲି);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_status_create_exec_fifo (ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *fifo_path = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret, fd = -1;

  ret = get_exec_fifo_path (&fifo_path, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_path);
  ret = mkfifo (fifo_path, 0600);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "mkfifo");

  fd = open (fifo_path, O_NONBLOCK | O_CLOEXEC);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot open pipe `%s`", fifo_path);

  ଫେରନ୍ତୁ fd;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_status_write_exec_fifo (ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *fifo_path = ଖାଲି;
  ଅକ୍ଷର buffer[1] = {
    0,
  };
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = get_exec_fifo_path (&fifo_path, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  fd = open (fifo_path, O_WRONLY | O_CLOEXEC);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot open `%s`", fifo_path);

  ret = unlink (fifo_path);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "unlink `%s`", fifo_path);

  ret = TEMP_FAILURE_RETRY (ଲିଖ (fd, buffer, 1));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "write to exec.fifo");

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_status_has_read_exec_fifo (ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *fifo_path = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = get_exec_fifo_path (&fifo_path, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ crun_path_exists (fifo_path, err);
}

ସଂରଚନା libcrun_status_s
{
  libcrun_container_status_t status;
  ଅକ୍ଷର *id;
  ଅକ୍ଷର *state_ମୂଳ;

  /* The live state requires probing the container (and its cgroup), so it is
     resolved on demand and cached here.  */
  bool state_resolved;
  libcrun_container_state_t state;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା running;
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_status_load (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, libcrun_status_t **out, libcrun_error_t *err)
{
  libcrun_status_t *st;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  st = xmalloc0 (ଆକାର (*st));

  ret = libcrun_read_container_status (&st->status, context->state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ମୁକ୍ତ (st);
      ଫେରନ୍ତୁ ret;
    }

  st->id = xstrdup (id);
  st->state_ମୂଳ = xstrdup (context->state_ମୂଳ);

  *out = st;
  ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ
libcrun_container_status_free (libcrun_status_t *st)
{
  ଯଦି (st == ଖାଲି)
    ଫେରନ୍ତୁ;
  libcrun_free_container_status (&st->status);
  ମୁକ୍ତ (st->state_ମୂଳ);
  ମୁକ୍ତ (st->id);
  ମୁକ୍ତ (st);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *running, libcrun_error_t *err)
{
  ଯଦି (! st->state_resolved)
    {
      ଅଚଳ ଅକ୍ଷର *state_string = ଖାଲି;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      ret = libcrun_get_container_state_string (st->id, &st->status, st->state_ମୂଳ, &state_string, &st->running, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ଯଦି (state_string == ଖାଲି)
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;
      ଅନ୍ୟଥା ଯଦି (strcmp (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATING;
      ଅନ୍ୟଥା ଯଦି (strcmp (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_CREATED;
      ଅନ୍ୟଥା ଯଦି (strcmp (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_RUNNING;
      ଅନ୍ୟଥା ଯଦି (strcmp (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_STATUS_PAUSED;
      ଅନ୍ୟଥା
        st->state = LIBCRUN_CONTAINER_STATUS_STOPPED;

      st->state_resolved = true;
    }

  ଯଦି (state)
    *state = st->state;
  ଯଦି (running)
    *running = st->running;
  ଫେରନ୍ତୁ 0;
}

ଅଚଳ ଅକ୍ଷର *
libcrun_container_state_to_string (libcrun_container_state_t state)
{
  ବାଛ (state)
    {
    ସ୍ଥିତି LIBCRUN_CONTAINER_STATUS_CREATING:
      ଫେରନ୍ତୁ "creating";
    ସ୍ଥିତି LIBCRUN_CONTAINER_STATUS_CREATED:
      ଫେରନ୍ତୁ "created";
    ସ୍ଥିତି LIBCRUN_CONTAINER_STATUS_RUNNING:
      ଫେରନ୍ତୁ "running";
    ସ୍ଥିତି LIBCRUN_CONTAINER_STATUS_PAUSED:
      ଫେରନ୍ତୁ "paused";
    ସ୍ଥିତି LIBCRUN_CONTAINER_STATUS_STOPPED:
      ଫେରନ୍ତୁ "stopped";
    }
  ଫେରନ୍ତୁ "unknown";
}

pid_t
libcrun_status_get_pid (libcrun_status_t *st)
{
  ଫେରନ୍ତୁ st->status.pid;
}

ଅଚଳ ଅକ୍ଷର *
libcrun_status_get_bundle (libcrun_status_t *st)
{
  ଫେରନ୍ତୁ st->status.bundle;
}

ଅଚଳ ଅକ୍ଷର *
libcrun_status_get_rootfs (libcrun_status_t *st)
{
  ଫେରନ୍ତୁ st->status.rootfs;
}

ଅଚଳ ଅକ୍ଷର *
libcrun_status_get_created (libcrun_status_t *st)
{
  ଫେରନ୍ତୁ st->status.created;
}

ଅଚଳ ଅକ୍ଷର *
libcrun_status_get_owner (libcrun_status_t *st)
{
  ଫେରନ୍ତୁ st->status.owner;
}

ଅଚଳ ଅକ୍ଷର *
libcrun_status_get_scope (libcrun_status_t *st)
{
  ଫେରନ୍ତୁ st->status.scope;
}

ଅଚଳ ଅକ୍ଷର *
libcrun_status_get_external_descriptors (libcrun_status_t *st)
{
  ଫେରନ୍ତୁ st->status.external_descriptors;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_get_state_string (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର **out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *running,
                                    libcrun_error_t *err)
{
  cleanup_container_status libcrun_container_status_t status = {};
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା local_running;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = libcrun_read_container_status (&status, context->state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_get_container_state_string (id, &status, context->state_ମୂଳ, out, &local_running, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (running)
    *running = local_running;
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_get_state_dir (ଅକ୍ଷର **out, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ libcrun_get_state_directory (out, state_ମୂଳ, id, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ libcrun_get_containers_list (out, context->state_ମୂଳ, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (ଆକାର (*it));
  it->cur = l;
  ଫେରନ୍ତୁ it;
}

bool
libcrun_container_iter_next (libcrun_container_iter_t *it, ଅଚଳ ଅକ୍ଷର **id)
{
  ଯଦି (it == ଖାଲି || it->cur == ଖାଲି)
    ଫେରନ୍ତୁ false;

  ଯଦି (id)
    *id = it->cur->name;
  it->cur = it->cur->next;
  ଫେରନ୍ତୁ true;
}

ଶୂନ୍ୟ
libcrun_container_iter_free (libcrun_container_iter_t *it)
{
  ମୁକ୍ତ (it);
}

ଶୂନ୍ୟ
libcrun_container_list_free (libcrun_container_list_t *l)
{
  libcrun_free_containers_list (l);
}
