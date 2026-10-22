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
#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "status.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "json_gen_utils.h"
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <json-c/json.h>
#ಅನ್ತರ್ಭಾವ <ocispec/json_common.h>
#ಅನ್ತರ್ಭಾವ <sys/mount.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <dirent.h>
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ <limits.h>

#ವ್ಯಾಖ್ಯಾನಿಸು STEAL_ಸೂಚಕ(x, y) \
  ಮಾಡು                        \
    {                       \
      *x = y;               \
      y = ಖಾಲಿ;             \
  } ಆಗ (0)

ರಚನೆ pid_stat
{
  ಅಕ್ಷರ state;
  ದೀರ್ಘ ಅಂಕೆ_ಎಳೆಗಳು;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ starttime;
};

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_valid_id_char (ಅಕ್ಷರ c)
{
  ಆದರೆ ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    ಹಿಂತಿರುಗಿ true;
  ಹಿಂತಿರುಗಿ c == '_' || c == '+' || c == '.' || c == '-';
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
validate_id (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err)
{
  ಆದರೆ (id == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (id[0] == '\0')
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "empty container ID");

  ಆದರೆ (id[0] == '.')
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid container ID `%s`", id);

  ಗಾಗಿ (size_t i = 0; id[i]; i++)
    {
      ಆದರೆ (! is_valid_id_char (id[i]))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
get_ಸತತ_directory (ಅಕ್ಷರ **ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  cleanup_free ಅಕ್ಷರ *ಬೇರು = ಖಾಲಿ;

  ಆದರೆ (state_ಬೇರು)
    ಬೇರು = xstrdup (state_ಬೇರು);
  ಆದರೆ (ಬೇರು == ಖಾಲಿ)
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *runtime_dir = getenv ("XDG_RUNTIME_DIR");
      ಆದರೆ (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&ಬೇರು, err, runtime_dir, "crun", ಖಾಲಿ);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
    }
  ಆದರೆ (ಬೇರು == ಖಾಲಿ)
    ಬೇರು = xstrdup ("/run/crun");

  ret = crun_ensure_directory (ಬೇರು, 0700, false, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  STEAL_ಸೂಚಕ (ಹೊರಹೋಗು, ಬೇರು);

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
get_shared_ಖಾಲಿಗಳು_directory_ಮಾರ್ಗ (ಅಕ್ಷರ **ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *ಸತತ_dir = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  ret = get_ಸತತ_directory (&ಸತತ_dir, state_ಬೇರು, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = append_paths (ಹೊರಹೋಗು, err, ಸತತ_dir, ".empty-directory", ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*ಹೊರಹೋಗು, 0555, false, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_get_state_directory (ಅಕ್ಷರ **ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  cleanup_free ಅಕ್ಷರ *ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *ಬೇರು = ಖಾಲಿ;

  ret = validate_id (id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = get_ಸತತ_directory (&ಬೇರು, state_ಬೇರು, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = append_paths (&ಮಾರ್ಗ, err, ಬೇರು, id, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  STEAL_ಸೂಚಕ (ಹೊರಹೋಗು, ಮಾರ್ಗ);

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_state_directory_ಸ್ಥಿತಿ_file (ಅಕ್ಷರ **ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *dir = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  ret = libcrun_get_state_directory (&dir, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ append_paths (ಹೊರಹೋಗು, err, dir, "status", ಖಾಲಿ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
read_pid_stat (pid_t pid, ರಚನೆ pid_stat *st, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *ಬಫರ್ = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  ಅಕ್ಷರ pid_stat_file[64];
  ಅಕ್ಷರ *it, *s;
  ಪೂರ್ಣಾಂಕ i, ret;

  ret = snprintf (pid_stat_file, ಗಾತ್ರ (pid_stat_file), "/proc/%d/stat", pid);
  ಆದರೆ (UNLIKELY (ret >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (pid_stat_file)))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

  fd = open (pid_stat_file, O_RDONLY | O_CLOEXEC);
  ಆದರೆ (fd < 0)
    {
      /* The process already exited.  */
      ಆದರೆ (errno == ENOENT || errno == ESRCH)
        {
          memset (st, 0, ಗಾತ್ರ (*st));
          ಹಿಂತಿರುಗಿ 0;
        }
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &ಬಫರ್, ಖಾಲಿ, err);
  ಆದರೆ (ret < 0)
    {
      st->starttime = 0;
      st->ಅಂಕೆ_ಎಳೆಗಳು = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      ಹಿಂತಿರುಗಿ 0;
    }

  s = ಖಾಲಿ;

  /* Skip the first two arguments.  */
  ಗಾಗಿ (it = ಬಫರ್; it; it = strchr (it + 1, ')'))
    s = it + 1;

  ಆದರೆ (s)
    ಆಗ (*s == ' ')
      s++;

  ಆದರೆ (s == ಖಾಲಿ || *s == '\0')
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  ಗಾಗಿ (it = s + 1, i = 0; i < 19; i++)
    {
      it = strchr (it, ' ');
      ಆದರೆ (it == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      ಆದರೆ (i == 16)
        st->ಅಂಕೆ_ಎಳೆಗಳು = strtol (it, ಖಾಲಿ, 10);
    }

  errno = 0;
  st->starttime = strtoull (it, ಖಾಲಿ, 10);
  ಆದರೆ (errno != 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "parse process start time");

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_write_container_ಸ್ಥಿತಿ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2,
                                libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ r, ret;
  cleanup_free ಅಕ್ಷರ *file_tmp = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *file = ಖಾಲಿ;
  size_t len;
  cleanup_close ಪೂರ್ಣಾಂಕ fd_write = -1;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *buf = ಖಾಲಿ;
  ರಚನೆ pid_stat st;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *tmp;
  cleanup_json_gen json_gen_ctx *gen = ಖಾಲಿ;

  ret = get_state_directory_ಸ್ಥಿತಿ_file (&file, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = read_pid_stat (ಸ್ಥಿತಿ_2->pid, &st, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  ಸ್ಥಿತಿ_2->process_ಆರಂಭ_ಸಮಯ = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = open (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  ಆದರೆ (UNLIKELY (fd_write < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot open status file");

  ಆದರೆ (! json_gen_init (&gen, ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ಕೀಲಿ (gen, "pid");
  GEN_OR_FAIL (map_int (gen, ಸ್ಥಿತಿ_2->pid));

  GEN_ಕೀಲಿ (gen, "process-start-time");
  GEN_OR_FAIL (map_uint (gen, ಸ್ಥಿತಿ_2->process_ಆರಂಭ_ಸಮಯ));

  GEN_ಕೀಲಿ (gen, "cgroup-path");
  tmp = ಸ್ಥಿತಿ_2->cgroup_ಮಾರ್ಗ ? ಸ್ಥಿತಿ_2->cgroup_ಮಾರ್ಗ : "";
  GEN_STR (gen, tmp);

  GEN_ಕೀಲಿ (gen, "scope");
  tmp = ಸ್ಥಿತಿ_2->scope ? ಸ್ಥಿತಿ_2->scope : "";
  GEN_STR (gen, tmp);

  GEN_ಕೀಲಿ (gen, "rootfs");
  GEN_STR (gen, ಸ್ಥಿತಿ_2->rootfs);

  GEN_ಕೀಲಿ (gen, "systemd-cgroup");
  GEN_OR_FAIL (json_gen_bool (gen, ಸ್ಥಿತಿ_2->systemd_cgroup));

  GEN_ಕೀಲಿ (gen, "bundle");
  GEN_STR (gen, ಸ್ಥಿತಿ_2->bundle);

  GEN_ಕೀಲಿ (gen, "created");
  GEN_STR (gen, ಸ್ಥಿತಿ_2->created);

  ಆದರೆ (ಸ್ಥಿತಿ_2->owner)
    {
      GEN_ಕೀಲಿ (gen, "owner");
      GEN_STR (gen, ಸ್ಥಿತಿ_2->owner);
    }

  GEN_ಕೀಲಿ (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, ಸ್ಥಿತಿ_2->detached));

  GEN_ಕೀಲಿ (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, ಸ್ಥಿತಿ_2->no_new_keyring));

  ಆದರೆ (ಸ್ಥಿತಿ_2->external_descriptors)
    {
      GEN_ಕೀಲಿ (gen, "external_descriptors");
      GEN_STR (gen, ಸ್ಥಿತಿ_2->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = ಸುರಕ್ಷಿತ_write (fd_write, "status file", buf, len, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  close_and_reset (&fd_write);

  ಆದರೆ (UNLIKELY (rename (file_tmp, file) < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot rename status file");

  ಹಿಂತಿರುಗಿ 0;

gen_error:
  ಹಿಂತಿರುಗಿ json_gen_error_ಗಮ್ಯ_crun_error (r, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_read_container_ಸ್ಥಿತಿ (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id,
                               libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *ಬಫರ್ = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;
  cleanup_free ಅಕ್ಷರ *file = ಖಾಲಿ;
  json_object *doc = ಖಾಲಿ;
  json_object *tmp;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *val;

  ret = get_state_directory_ಸ್ಥಿತಿ_file (&file, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = read_all_file (file, &ಬಫರ್, ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {

      ಆದರೆ (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free ಅಕ್ಷರ *statedir = ಖಾಲಿ;
          libcrun_error_t tmp_err;
          ಪೂರ್ಣಾಂಕ tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_ಬೇರು, id, &tmp_err);
          ಆದರೆ (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          ಇಲ್ಲದಿದ್ದರೆ
            {
              tmp_ret = crun_ಮಾರ್ಗ_exists (statedir, &tmp_err);
              ಆದರೆ (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (tmp_ret == 0)
                ಹಿಂತಿರುಗಿ crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      ಹಿಂತಿರುಗಿ ret;
    }

  ret = parse_json_file (&doc, ಬಫರ್, ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  {
    tmp = json_object_object_get (doc, "pid");
    ಆದರೆ (UNLIKELY (tmp == ಖಾಲಿ))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        ಹೋಗು ನಿರ್ಗಮಿಸು;
      }
    ಆದರೆ (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        ಹೋಗು ನಿರ್ಗಮಿಸು;
      }
    ಸ್ಥಿತಿ_2->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    ಆದರೆ (UNLIKELY (tmp == ಖಾಲಿ))
      ಸ್ಥಿತಿ_2->process_ಆರಂಭ_ಸಮಯ = 0; /* backwards compatibility */
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        ಹೋಗು ನಿರ್ಗಮಿಸು;
      }
    ಇಲ್ಲದಿದ್ದರೆ
      ಸ್ಥಿತಿ_2->process_ಆರಂಭ_ಸಮಯ = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    ಆದರೆ (UNLIKELY (tmp == ಖಾಲಿ))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        ಹೋಗು ನಿರ್ಗಮಿಸು;
      }
    val = json_object_get_string (tmp);
    ಆದರೆ (UNLIKELY (val == ಖಾಲಿ))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` is not a string in `%s`", file);
        ಹೋಗು ನಿರ್ಗಮಿಸು;
      }
    ಸ್ಥಿತಿ_2->cgroup_ಮಾರ್ಗ = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "scope");
    val = tmp ? json_object_get_string (tmp) : ಖಾಲಿ;
    ಸ್ಥಿತಿ_2->scope = val ? xstrdup (val) : ಖಾಲಿ;
  }
  {
    tmp = json_object_object_get (doc, "rootfs");
    ಆದರೆ (UNLIKELY (tmp == ಖಾಲಿ))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        ಹೋಗು ನಿರ್ಗಮಿಸು;
      }
    val = json_object_get_string (tmp);
    ಆದರೆ (UNLIKELY (val == ಖಾಲಿ))
      {
        ret = crun_make_error (err, 0, "`rootfs` is not a string in `%s`", file);
        ಹೋಗು ನಿರ್ಗಮಿಸು;
      }
    ಸ್ಥಿತಿ_2->rootfs = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "systemd-cgroup");
    ಸ್ಥಿತಿ_2->systemd_cgroup = tmp != ಖಾಲಿ && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "bundle");
    ಆದರೆ (UNLIKELY (tmp == ಖಾಲಿ))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        ಹೋಗು ನಿರ್ಗಮಿಸು;
      }
    val = json_object_get_string (tmp);
    ಆದರೆ (UNLIKELY (val == ಖಾಲಿ))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        ಹೋಗು ನಿರ್ಗಮಿಸು;
      }
    ಸ್ಥಿತಿ_2->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    ಆದರೆ (UNLIKELY (tmp == ಖಾಲಿ))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        ಹೋಗು ನಿರ್ಗಮಿಸು;
      }
    val = json_object_get_string (tmp);
    ಆದರೆ (UNLIKELY (val == ಖಾಲಿ))
      {
        ret = crun_make_error (err, 0, "`created` is not a string in `%s`", file);
        ಹೋಗು ನಿರ್ಗಮಿಸು;
      }
    ಸ್ಥಿತಿ_2->created = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "owner");
    val = tmp ? json_object_get_string (tmp) : ಖಾಲಿ;
    ಸ್ಥಿತಿ_2->owner = val ? xstrdup (val) : ಖಾಲಿ;
  }
  {
    tmp = json_object_object_get (doc, "detached");
    ಸ್ಥಿತಿ_2->detached = tmp != ಖಾಲಿ && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "no-new-keyring");
    ಸ್ಥಿತಿ_2->no_new_keyring = tmp != ಖಾಲಿ && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "external_descriptors");
    val = tmp ? json_object_get_string (tmp) : ಖಾಲಿ;
    ಸ್ಥಿತಿ_2->external_descriptors = val ? xstrdup (val) : ಖಾಲಿ;
  }
  ret = 0;

ನಿರ್ಗಮಿಸು:
  json_object_put (doc);
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
libcrun_ಸ್ಥಿತಿ_check_directories (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *dir = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  ret = libcrun_get_state_directory (&dir, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = crun_ಮಾರ್ಗ_exists (dir, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (ret)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "container `%s` already exists", id);

  ಆದರೆ (UNLIKELY (mkdir (dir, 0700) < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  ಹಿಂತಿರುಗಿ 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#ವ್ಯಾಖ್ಯಾನಿಸು RMDIRFD_MAX_ಚಲನೆಗಳು (1U << 20)

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
rmdirfd_move_ಗಮ್ಯ (DIR **d, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  DIR *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  ಆದರೆ (UNLIKELY (new_d == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot open directory `%s`", name);

  fd = -1;

  closedir (*d);
  *d = new_d;
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
rmdirfd (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *namedir, ಪೂರ್ಣಾಂಕ fd, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  cleanup_dir DIR *d = ಖಾಲಿ;
  ರಚನೆ dirent *de;
  ರಚನೆ stat ಬೇರು_st;
  bool is_ಬೇರು = true;
  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ budget = RMDIRFD_MAX_ಚಲನೆಗಳು;
  __attribute__ ((ಬಳಸದ)) cleanup_close ಪೂರ್ಣಾಂಕ fd_cleanup = fd;

  ret = fstat (fd, &ಬೇರು_st);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "stat `%s`", namedir);

  d = fdopendir (fd);
  ಆದರೆ (d == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  ಗಾಗಿ (;;)
    {
      errno = 0;
      de = readdir (d);
      ಆದರೆ (de == ಖಾಲಿ)
        {
          ರಚನೆ stat st;

          ಆದರೆ (UNLIKELY (errno != 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot read directory");

          ಆದರೆ (is_ಬೇರು)
            ನಿಲ್ಲಿಸು;

          ಆದರೆ (UNLIKELY (--budget == 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_ಗಮ್ಯ (&d, "..", err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;

          ret = fstat (dirfd (d), &st);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "stat `..`");

          is_ಬೇರು = st.st_dev == ಬೇರು_st.st_dev && st.st_ino == ಬೇರು_st.st_ino;
          ಮುಂದುವರಿ;
        }

      ಆದರೆ ((strcmp (de->d_name, ".") == 0) || (strcmp (de->d_name, "..") == 0))
        ಮುಂದುವರಿ;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      ಆದರೆ (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          ಆದರೆ (ret < 0 && errno == EBUSY)
            {
              cleanup_close ಪೂರ್ಣಾಂಕ tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_ಮಾರ್ಗ | O_NOFOLLOW);
              ಆದರೆ (tfd >= 0)
                {
                  proc_fd_ಮಾರ್ಗ_t procpath;

                  get_proc_self_fd_ಮಾರ್ಗ (procpath, tfd);
                  ಆದರೆ (umount2 (procpath, MNT_DETACH) == 0)
                    ಹೋಗು retry_unlink;
                }
              ಆದರೆ (! is_ಬೇರು)
                ಹಿಂತಿರುಗಿ crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ret < 0 && errno == ENOTEMPTY)
            {
              ಆದರೆ (UNLIKELY (--budget == 0))
                ಹಿಂತಿರುಗಿ crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_ಗಮ್ಯ (&d, de->d_name, err);
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ ret;

              is_ಬೇರು = false;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ret < 0 && errno != ENOENT && ! is_ಬೇರು)
            {
              ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot rm `%s`", de->d_name);
            }
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_container_delete_ಸ್ಥಿತಿ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  cleanup_close ಪೂರ್ಣಾಂಕ rundir_dfd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ dfd = -1;
  cleanup_free ಅಕ್ಷರ *dir = ಖಾಲಿ;

  ret = validate_id (id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = get_ಸತತ_directory (&dir, state_ಬೇರು, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  rundir_dfd = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (open (dir, O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC));
  ಆದರೆ (UNLIKELY (rundir_dfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (dfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot rm state directory `%s/%s`", dir, id);

  ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ
libcrun_free_container_ಸ್ಥಿತಿ (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;
  ಬಿಡುಗಡೆ (ಸ್ಥಿತಿ_2->cgroup_ಮಾರ್ಗ);
  ಬಿಡುಗಡೆ (ಸ್ಥಿತಿ_2->bundle);
  ಬಿಡುಗಡೆ (ಸ್ಥಿತಿ_2->rootfs);
  ಬಿಡುಗಡೆ (ಸ್ಥಿತಿ_2->external_descriptors);
  ಬಿಡುಗಡೆ (ಸ್ಥಿತಿ_2->created);
  ಬಿಡುಗಡೆ (ಸ್ಥಿತಿ_2->scope);
  ಬಿಡುಗಡೆ (ಸ್ಥಿತಿ_2->owner);
}

ಪೂರ್ಣಾಂಕ
libcrun_get_containers_list (libcrun_container_list_t **ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, libcrun_error_t *err)
{
  ರಚನೆ dirent *ಮುಂದಿನ;
  cleanup_container_list libcrun_container_list_t *tmp = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *ಬೇರು = ಖಾಲಿ;
  cleanup_dir DIR *dir = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  *ಹೊರಹೋಗು = ಖಾಲಿ;

  ret = get_ಸತತ_directory (&ಬೇರು, state_ಬೇರು, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  dir = opendir (ಬೇರು);
  ಆದರೆ (UNLIKELY (dir == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot opendir `%s`", ಬೇರು);

  ಗಾಗಿ (ಮುಂದಿನ = readdir (dir); ಮುಂದಿನ; ಮುಂದಿನ = readdir (dir))
    {
      ಪೂರ್ಣಾಂಕ exists;
      cleanup_free ಅಕ್ಷರ *ಸ್ಥಿತಿ_file = ಖಾಲಿ;

      libcrun_container_list_t *ಮುಂದಿನ_container;

      ಆದರೆ (ಮುಂದಿನ->d_name[0] == '.')
        ಮುಂದುವರಿ;

      ret = append_paths (&ಸ್ಥಿತಿ_file, err, ಬೇರು, ಮುಂದಿನ->d_name, "status", ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      exists = crun_ಮಾರ್ಗ_exists (ಸ್ಥಿತಿ_file, err);
      ಆದರೆ (exists < 0)
        {
          ಹಿಂತಿರುಗಿ exists;
        }

      ಆದರೆ (! exists)
        {
          libcrun_error (errno, "error opening file `%s`", ಸ್ಥಿತಿ_file);
          ಮುಂದುವರಿ;
        }

      ಮುಂದಿನ_container = xmalloc (ಗಾತ್ರ (libcrun_container_list_t));
      ಮುಂದಿನ_container->name = xstrdup (ಮುಂದಿನ->d_name);
      ಮುಂದಿನ_container->ಮುಂದಿನ = tmp;
      tmp = ಮುಂದಿನ_container;
    }

  STEAL_ಸೂಚಕ (ಹೊರಹೋಗು, tmp);

  ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *ಮುಂದಿನ;
  ಆಗ (list)
    {
      ಮುಂದಿನ = list->ಮುಂದಿನ;
      ಬಿಡುಗಡೆ (list->name);
      ಬಿಡುಗಡೆ (list);
      list = ಮುಂದಿನ;
    }
}

/* check if the container pid is still valid.
   Returns:
   -1: on errors
    0: pid not valid
    1: pid valid and container in the running/created/paused state
*/
ಪೂರ್ಣಾಂಕ
libcrun_check_pid_valid (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, libcrun_error_t *err)
{
  ರಚನೆ pid_stat st;
  ಪೂರ್ಣಾಂಕ ret;

  /* For backwards compatibility, check start time only if available. */
  ಆದರೆ (! ಸ್ಥಿತಿ_2->process_ಆರಂಭ_ಸಮಯ)
    ಹಿಂತಿರುಗಿ 1;

  ret = read_pid_stat (ಸ್ಥಿತಿ_2->pid, &st, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (ಸ್ಥಿತಿ_2->process_ಆರಂಭ_ಸಮಯ != st.starttime || st.state == 'X')
    ಹಿಂತಿರುಗಿ 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  ಆದರೆ (st.state == 'Z' && st.ಅಂಕೆ_ಎಳೆಗಳು <= 1)
    ಹಿಂತಿರುಗಿ 0; /* stopped */

  ಹಿಂತಿರುಗಿ 1; /* running, created, or paused */
}

ಪೂರ್ಣಾಂಕ
libcrun_is_container_running (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ret = kill (ಸ್ಥಿತಿ_2->pid, 0);
  ಆದರೆ (UNLIKELY (ret < 0) && errno != ESRCH)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "kill");

  ಆದರೆ (ret == 0)
    ಹಿಂತಿರುಗಿ libcrun_check_pid_valid (ಸ್ಥಿತಿ_2, err);

  ಹಿಂತಿರುಗಿ 0; /* stopped */
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_exec_fifo_ಮಾರ್ಗ (ಅಕ್ಷರ **ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *state_dir = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  ret = libcrun_get_state_directory (&state_dir, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ append_paths (ಹೊರಹೋಗು, err, state_dir, "exec.fifo", ಖಾಲಿ);
}

ಪೂರ್ಣಾಂಕ
libcrun_ಸ್ಥಿತಿ_create_exec_fifo (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *fifo_ಮಾರ್ಗ = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret, fd = -1;

  ret = get_exec_fifo_ಮಾರ್ಗ (&fifo_ಮಾರ್ಗ, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_ಮಾರ್ಗ);
  ret = mkfifo (fifo_ಮಾರ್ಗ, 0600);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "mkfifo");

  fd = open (fifo_ಮಾರ್ಗ, O_NONBLOCK | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot open pipe `%s`", fifo_ಮಾರ್ಗ);

  ಹಿಂತಿರುಗಿ fd;
}

ಪೂರ್ಣಾಂಕ
libcrun_ಸ್ಥಿತಿ_write_exec_fifo (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *fifo_ಮಾರ್ಗ = ಖಾಲಿ;
  ಅಕ್ಷರ ಬಫರ್[1] = {
    0,
  };
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  ಪೂರ್ಣಾಂಕ ret;

  ret = get_exec_fifo_ಮಾರ್ಗ (&fifo_ಮಾರ್ಗ, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  fd = open (fifo_ಮಾರ್ಗ, O_WRONLY | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot open `%s`", fifo_ಮಾರ್ಗ);

  ret = unlink (fifo_ಮಾರ್ಗ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "unlink `%s`", fifo_ಮಾರ್ಗ);

  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (fd, ಬಫರ್, 1));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write to exec.fifo");

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_ಸ್ಥಿತಿ_has_read_exec_fifo (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *fifo_ಮಾರ್ಗ = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  ret = get_exec_fifo_ಮಾರ್ಗ (&fifo_ಮಾರ್ಗ, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ crun_ಮಾರ್ಗ_exists (fifo_ಮಾರ್ಗ, err);
}

ರಚನೆ libcrun_ಸ್ಥಿತಿ_s
{
  libcrun_container_ಸ್ಥಿತಿ_t ಸ್ಥಿತಿ_2;
  ಅಕ್ಷರ *id;
  ಅಕ್ಷರ *state_ಬೇರು;

  /* The live state requires probing the container (and its cgroup), so it is
     resolved on demand and cached here.  */
  bool state_resolved;
  libcrun_container_state_t state;
  ಪೂರ್ಣಾಂಕ running;
};

ಪೂರ್ಣಾಂಕ
libcrun_container_ಸ್ಥಿತಿ_ತುಂಬು (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_ಸ್ಥಿತಿ_t **ಹೊರಹೋಗು, libcrun_error_t *err)
{
  libcrun_ಸ್ಥಿತಿ_t *st;
  ಪೂರ್ಣಾಂಕ ret;

  st = xmalloc0 (ಗಾತ್ರ (*st));

  ret = libcrun_read_container_ಸ್ಥಿತಿ (&st->ಸ್ಥಿತಿ_2, context->state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಬಿಡುಗಡೆ (st);
      ಹಿಂತಿರುಗಿ ret;
    }

  st->id = xstrdup (id);
  st->state_ಬೇರು = xstrdup (context->state_ಬೇರು);

  *ಹೊರಹೋಗು = st;
  ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ
libcrun_container_ಸ್ಥಿತಿ_free (libcrun_ಸ್ಥಿತಿ_t *st)
{
  ಆದರೆ (st == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;
  libcrun_free_container_ಸ್ಥಿತಿ (&st->ಸ್ಥಿತಿ_2);
  ಬಿಡುಗಡೆ (st->state_ಬೇರು);
  ಬಿಡುಗಡೆ (st->id);
  ಬಿಡುಗಡೆ (st);
}

ಪೂರ್ಣಾಂಕ
libcrun_ಸ್ಥಿತಿ_get_state (libcrun_ಸ್ಥಿತಿ_t *st, libcrun_container_state_t *state, ಪೂರ್ಣಾಂಕ *running, libcrun_error_t *err)
{
  ಆದರೆ (! st->state_resolved)
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_string = ಖಾಲಿ;
      ಪೂರ್ಣಾಂಕ ret;

      ret = libcrun_get_container_state_string (st->id, &st->ಸ್ಥಿತಿ_2, st->state_ಬೇರು, &state_string, &st->running, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ಆದರೆ (state_string == ಖಾಲಿ)
        st->state = LIBCRUN_CONTAINER_ಸ್ಥಿತಿ_STOPPED;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_ಸ್ಥಿತಿ_CREATING;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_ಸ್ಥಿತಿ_CREATED;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_ಸ್ಥಿತಿ_RUNNING;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_ಸ್ಥಿತಿ_PAUSED;
      ಇಲ್ಲದಿದ್ದರೆ
        st->state = LIBCRUN_CONTAINER_ಸ್ಥಿತಿ_STOPPED;

      st->state_resolved = true;
    }

  ಆದರೆ (state)
    *state = st->state;
  ಆದರೆ (running)
    *running = st->running;
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
libcrun_container_state_ಗಮ್ಯ_string (libcrun_container_state_t state)
{
  ಆಯ್ಕೆ (state)
    {
    ಸ್ಥಿತಿ LIBCRUN_CONTAINER_ಸ್ಥಿತಿ_CREATING:
      ಹಿಂತಿರುಗಿ "creating";
    ಸ್ಥಿತಿ LIBCRUN_CONTAINER_ಸ್ಥಿತಿ_CREATED:
      ಹಿಂತಿರುಗಿ "created";
    ಸ್ಥಿತಿ LIBCRUN_CONTAINER_ಸ್ಥಿತಿ_RUNNING:
      ಹಿಂತಿರುಗಿ "running";
    ಸ್ಥಿತಿ LIBCRUN_CONTAINER_ಸ್ಥಿತಿ_PAUSED:
      ಹಿಂತಿರುಗಿ "paused";
    ಸ್ಥಿತಿ LIBCRUN_CONTAINER_ಸ್ಥಿತಿ_STOPPED:
      ಹಿಂತಿರುಗಿ "stopped";
    }
  ಹಿಂತಿರುಗಿ "unknown";
}

pid_t
libcrun_ಸ್ಥಿತಿ_get_pid (libcrun_ಸ್ಥಿತಿ_t *st)
{
  ಹಿಂತಿರುಗಿ st->ಸ್ಥಿತಿ_2.pid;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
libcrun_ಸ್ಥಿತಿ_get_bundle (libcrun_ಸ್ಥಿತಿ_t *st)
{
  ಹಿಂತಿರುಗಿ st->ಸ್ಥಿತಿ_2.bundle;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
libcrun_ಸ್ಥಿತಿ_get_rootfs (libcrun_ಸ್ಥಿತಿ_t *st)
{
  ಹಿಂತಿರುಗಿ st->ಸ್ಥಿತಿ_2.rootfs;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
libcrun_ಸ್ಥಿತಿ_get_created (libcrun_ಸ್ಥಿತಿ_t *st)
{
  ಹಿಂತಿರುಗಿ st->ಸ್ಥಿತಿ_2.created;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
libcrun_ಸ್ಥಿತಿ_get_owner (libcrun_ಸ್ಥಿತಿ_t *st)
{
  ಹಿಂತಿರುಗಿ st->ಸ್ಥಿತಿ_2.owner;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
libcrun_ಸ್ಥಿತಿ_get_scope (libcrun_ಸ್ಥಿತಿ_t *st)
{
  ಹಿಂತಿರುಗಿ st->ಸ್ಥಿತಿ_2.scope;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
libcrun_ಸ್ಥಿತಿ_get_external_descriptors (libcrun_ಸ್ಥಿತಿ_t *st)
{
  ಹಿಂತಿರುಗಿ st->ಸ್ಥಿತಿ_2.external_descriptors;
}

ಪೂರ್ಣಾಂಕ
libcrun_container_get_state_string (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ *running,
                                    libcrun_error_t *err)
{
  cleanup_container_ಸ್ಥಿತಿ libcrun_container_ಸ್ಥಿತಿ_t ಸ್ಥಿತಿ_2 = {};
  ಪೂರ್ಣಾಂಕ local_running;
  ಪೂರ್ಣಾಂಕ ret;

  ret = libcrun_read_container_ಸ್ಥಿತಿ (&ಸ್ಥಿತಿ_2, context->state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_get_container_state_string (id, &ಸ್ಥಿತಿ_2, context->state_ಬೇರು, ಹೊರಹೋಗು, &local_running, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (running)
    *running = local_running;
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_get_state_dir (ಅಕ್ಷರ **ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ libcrun_get_state_directory (ಹೊರಹೋಗು, state_ಬೇರು, id, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **ಹೊರಹೋಗು, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ libcrun_get_containers_list (ಹೊರಹೋಗು, context->state_ಬೇರು, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (ಗಾತ್ರ (*it));
  it->cur = l;
  ಹಿಂತಿರುಗಿ it;
}

bool
libcrun_container_iter_ಮುಂದಿನ (libcrun_container_iter_t *it, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **id)
{
  ಆದರೆ (it == ಖಾಲಿ || it->cur == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ false;

  ಆದರೆ (id)
    *id = it->cur->name;
  it->cur = it->cur->ಮುಂದಿನ;
  ಹಿಂತಿರುಗಿ true;
}

ಶೂನ್ಯ
libcrun_container_iter_free (libcrun_container_iter_t *it)
{
  ಬಿಡುಗಡೆ (it);
}

ಶೂನ್ಯ
libcrun_container_list_free (libcrun_container_list_t *l)
{
  libcrun_free_containers_list (l);
}
