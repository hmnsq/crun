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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "json_gen_utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <json-c/json.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ocispec/json_common.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/mount.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <dirent.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STEAL_ਸੂਚਕ(x, y) \
  ਕਰੋ                        \
    {                       \
      *x = y;               \
      y = ਨਿਰਾ;             \
  } ਜਦੋਂ (0)

ਬਣਤਰ pid_stat
{
  ਅੱਖਰ state;
  ਲੰਮਾ ਅੰਕ_ਧਾਗੇ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ starttime;
};

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_valid_id_char (ਅੱਖਰ c)
{
  ਜੇ ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    ਵਾਪਸ true;
  ਵਾਪਸ c == '_' || c == '+' || c == '.' || c == '-';
}

ਸਥਿਰ ਪੂਰਨ
validate_id (ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err)
{
  ਜੇ (id == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ਜੇ (id[0] == '\0')
    ਵਾਪਸ crun_make_error (err, 0, "empty container ID");

  ਜੇ (id[0] == '.')
    ਵਾਪਸ crun_make_error (err, 0, "invalid container ID `%s`", id);

  ਲਈ (size_t i = 0; id[i]; i++)
    {
      ਜੇ (! is_valid_id_char (id[i]))
        ਵਾਪਸ crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  ਵਾਪਸ 0;
}

ਪੂਰਨ
get_ਲੜੀਵਾਰ_directory (ਅੱਖਰ **ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  cleanup_free ਅੱਖਰ *ਜੜ੍ਹ = ਨਿਰਾ;

  ਜੇ (state_ਜੜ੍ਹ)
    ਜੜ੍ਹ = xstrdup (state_ਜੜ੍ਹ);
  ਜੇ (ਜੜ੍ਹ == ਨਿਰਾ)
    {
      ਅਚਲ ਅੱਖਰ *runtime_dir = getenv ("XDG_RUNTIME_DIR");
      ਜੇ (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&ਜੜ੍ਹ, err, runtime_dir, "crun", ਨਿਰਾ);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
    }
  ਜੇ (ਜੜ੍ਹ == ਨਿਰਾ)
    ਜੜ੍ਹ = xstrdup ("/run/crun");

  ret = crun_ensure_directory (ਜੜ੍ਹ, 0700, false, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  STEAL_ਸੂਚਕ (ਬਾਹਰਲਾ, ਜੜ੍ਹ);

  ਵਾਪਸ 0;
}

ਪੂਰਨ
get_shared_ਖਾਲੀ_ਥਾਵਾਂ_directory_ਰਸਤਾ (ਅੱਖਰ **ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *ਲੜੀਵਾਰ_dir = ਨਿਰਾ;
  ਪੂਰਨ ret;

  ret = get_ਲੜੀਵਾਰ_directory (&ਲੜੀਵਾਰ_dir, state_ਜੜ੍ਹ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = append_paths (ਬਾਹਰਲਾ, err, ਲੜੀਵਾਰ_dir, ".empty-directory", ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*ਬਾਹਰਲਾ, 0555, false, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_get_state_directory (ਅੱਖਰ **ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  cleanup_free ਅੱਖਰ *ਰਸਤਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *ਜੜ੍ਹ = ਨਿਰਾ;

  ret = validate_id (id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = get_ਲੜੀਵਾਰ_directory (&ਜੜ੍ਹ, state_ਜੜ੍ਹ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = append_paths (&ਰਸਤਾ, err, ਜੜ੍ਹ, id, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  STEAL_ਸੂਚਕ (ਬਾਹਰਲਾ, ਰਸਤਾ);

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
get_state_directory_ਸਥਿਤੀ_file (ਅੱਖਰ **ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *dir = ਨਿਰਾ;
  ਪੂਰਨ ret;

  ret = libcrun_get_state_directory (&dir, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ append_paths (ਬਾਹਰਲਾ, err, dir, "status", ਨਿਰਾ);
}

ਸਥਿਰ ਪੂਰਨ
read_pid_stat (pid_t pid, ਬਣਤਰ pid_stat *st, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *ਬਫਰ = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ fd = -1;
  ਅੱਖਰ pid_stat_file[64];
  ਅੱਖਰ *it, *s;
  ਪੂਰਨ i, ret;

  ret = snprintf (pid_stat_file, ਆਕਾਰ (pid_stat_file), "/proc/%d/stat", pid);
  ਜੇ (UNLIKELY (ret >= (ਪੂਰਨ) ਆਕਾਰ (pid_stat_file)))
    ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

  fd = open (pid_stat_file, O_RDONLY | O_CLOEXEC);
  ਜੇ (fd < 0)
    {
      /* The process already exited.  */
      ਜੇ (errno == ENOENT || errno == ESRCH)
        {
          memset (st, 0, ਆਕਾਰ (*st));
          ਵਾਪਸ 0;
        }
      ਵਾਪਸ crun_make_error (err, errno, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &ਬਫਰ, ਨਿਰਾ, err);
  ਜੇ (ret < 0)
    {
      st->starttime = 0;
      st->ਅੰਕ_ਧਾਗੇ = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      ਵਾਪਸ 0;
    }

  s = ਨਿਰਾ;

  /* Skip the first two arguments.  */
  ਲਈ (it = ਬਫਰ; it; it = strchr (it + 1, ')'))
    s = it + 1;

  ਜੇ (s)
    ਜਦੋਂ (*s == ' ')
      s++;

  ਜੇ (s == ਨਿਰਾ || *s == '\0')
    ਵਾਪਸ crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  ਲਈ (it = s + 1, i = 0; i < 19; i++)
    {
      it = strchr (it, ' ');
      ਜੇ (it == ਨਿਰਾ)
        ਵਾਪਸ crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      ਜੇ (i == 16)
        st->ਅੰਕ_ਧਾਗੇ = strtol (it, ਨਿਰਾ, 10);
    }

  errno = 0;
  st->starttime = strtoull (it, ਨਿਰਾ, 10);
  ਜੇ (errno != 0)
    ਵਾਪਸ crun_make_error (err, errno, "parse process start time");

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_write_container_ਸਥਿਤੀ (ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id, libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ,
                                libcrun_error_t *err)
{
  ਪੂਰਨ r, ret;
  cleanup_free ਅੱਖਰ *file_tmp = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *file = ਨਿਰਾ;
  size_t len;
  cleanup_close ਪੂਰਨ fd_write = -1;
  ਅਚਲ ਅੱਖਰ *buf = ਨਿਰਾ;
  ਬਣਤਰ pid_stat st;
  ਅਚਲ ਅੱਖਰ *tmp;
  cleanup_json_gen json_gen_ctx *gen = ਨਿਰਾ;

  ret = get_state_directory_ਸਥਿਤੀ_file (&file, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = read_pid_stat (ਸਥਿਤੀ->pid, &st, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  ਸਥਿਤੀ->process_ਸ਼ੁਰੂ_ਸਮਾਂ = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = open (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  ਜੇ (UNLIKELY (fd_write < 0))
    ਵਾਪਸ crun_make_error (err, errno, "cannot open status file");

  ਜੇ (! json_gen_init (&gen, ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ਕੁੰਜੀ (gen, "pid");
  GEN_OR_FAIL (map_int (gen, ਸਥਿਤੀ->pid));

  GEN_ਕੁੰਜੀ (gen, "process-start-time");
  GEN_OR_FAIL (map_uint (gen, ਸਥਿਤੀ->process_ਸ਼ੁਰੂ_ਸਮਾਂ));

  GEN_ਕੁੰਜੀ (gen, "cgroup-path");
  tmp = ਸਥਿਤੀ->cgroup_ਰਸਤਾ ? ਸਥਿਤੀ->cgroup_ਰਸਤਾ : "";
  GEN_STR (gen, tmp);

  GEN_ਕੁੰਜੀ (gen, "scope");
  tmp = ਸਥਿਤੀ->scope ? ਸਥਿਤੀ->scope : "";
  GEN_STR (gen, tmp);

  GEN_ਕੁੰਜੀ (gen, "rootfs");
  GEN_STR (gen, ਸਥਿਤੀ->rootfs);

  GEN_ਕੁੰਜੀ (gen, "systemd-cgroup");
  GEN_OR_FAIL (json_gen_bool (gen, ਸਥਿਤੀ->systemd_cgroup));

  GEN_ਕੁੰਜੀ (gen, "bundle");
  GEN_STR (gen, ਸਥਿਤੀ->bundle);

  GEN_ਕੁੰਜੀ (gen, "created");
  GEN_STR (gen, ਸਥਿਤੀ->created);

  ਜੇ (ਸਥਿਤੀ->owner)
    {
      GEN_ਕੁੰਜੀ (gen, "owner");
      GEN_STR (gen, ਸਥਿਤੀ->owner);
    }

  GEN_ਕੁੰਜੀ (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, ਸਥਿਤੀ->detached));

  GEN_ਕੁੰਜੀ (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, ਸਥਿਤੀ->no_new_keyring));

  ਜੇ (ਸਥਿਤੀ->external_descriptors)
    {
      GEN_ਕੁੰਜੀ (gen, "external_descriptors");
      GEN_STR (gen, ਸਥਿਤੀ->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = ਸੁਰੱਖਿਅਤ_write (fd_write, "status file", buf, len, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  close_and_reset (&fd_write);

  ਜੇ (UNLIKELY (rename (file_tmp, file) < 0))
    ਵਾਪਸ crun_make_error (err, errno, "cannot rename status file");

  ਵਾਪਸ 0;

gen_error:
  ਵਾਪਸ json_gen_error_ਮੰਜ਼ਿਲ_crun_error (r, err);
}

ਪੂਰਨ
libcrun_read_container_ਸਥਿਤੀ (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id,
                               libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *ਬਫਰ = ਨਿਰਾ;
  ਪੂਰਨ ret;
  cleanup_free ਅੱਖਰ *file = ਨਿਰਾ;
  json_object *doc = ਨਿਰਾ;
  json_object *tmp;
  ਅਚਲ ਅੱਖਰ *val;

  ret = get_state_directory_ਸਥਿਤੀ_file (&file, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = read_all_file (file, &ਬਫਰ, ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    {

      ਜੇ (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free ਅੱਖਰ *statedir = ਨਿਰਾ;
          libcrun_error_t tmp_err;
          ਪੂਰਨ tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_ਜੜ੍ਹ, id, &tmp_err);
          ਜੇ (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          ਨਹੀਂਤਾਂ
            {
              tmp_ret = crun_ਰਸਤਾ_exists (statedir, &tmp_err);
              ਜੇ (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              ਨਹੀਂਤਾਂ ਜੇ (tmp_ret == 0)
                ਵਾਪਸ crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      ਵਾਪਸ ret;
    }

  ret = parse_json_file (&doc, ਬਫਰ, ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  {
    tmp = json_object_object_get (doc, "pid");
    ਜੇ (UNLIKELY (tmp == ਨਿਰਾ))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
      }
    ਜੇ (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
      }
    ਸਥਿਤੀ->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    ਜੇ (UNLIKELY (tmp == ਨਿਰਾ))
      ਸਥਿਤੀ->process_ਸ਼ੁਰੂ_ਸਮਾਂ = 0; /* backwards compatibility */
    ਨਹੀਂਤਾਂ ਜੇ (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
      }
    ਨਹੀਂਤਾਂ
      ਸਥਿਤੀ->process_ਸ਼ੁਰੂ_ਸਮਾਂ = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    ਜੇ (UNLIKELY (tmp == ਨਿਰਾ))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
      }
    val = json_object_get_string (tmp);
    ਜੇ (UNLIKELY (val == ਨਿਰਾ))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` is not a string in `%s`", file);
        ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
      }
    ਸਥਿਤੀ->cgroup_ਰਸਤਾ = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "scope");
    val = tmp ? json_object_get_string (tmp) : ਨਿਰਾ;
    ਸਥਿਤੀ->scope = val ? xstrdup (val) : ਨਿਰਾ;
  }
  {
    tmp = json_object_object_get (doc, "rootfs");
    ਜੇ (UNLIKELY (tmp == ਨਿਰਾ))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
      }
    val = json_object_get_string (tmp);
    ਜੇ (UNLIKELY (val == ਨਿਰਾ))
      {
        ret = crun_make_error (err, 0, "`rootfs` is not a string in `%s`", file);
        ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
      }
    ਸਥਿਤੀ->rootfs = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "systemd-cgroup");
    ਸਥਿਤੀ->systemd_cgroup = tmp != ਨਿਰਾ && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "bundle");
    ਜੇ (UNLIKELY (tmp == ਨਿਰਾ))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
      }
    val = json_object_get_string (tmp);
    ਜੇ (UNLIKELY (val == ਨਿਰਾ))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
      }
    ਸਥਿਤੀ->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    ਜੇ (UNLIKELY (tmp == ਨਿਰਾ))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
      }
    val = json_object_get_string (tmp);
    ਜੇ (UNLIKELY (val == ਨਿਰਾ))
      {
        ret = crun_make_error (err, 0, "`created` is not a string in `%s`", file);
        ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
      }
    ਸਥਿਤੀ->created = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "owner");
    val = tmp ? json_object_get_string (tmp) : ਨਿਰਾ;
    ਸਥਿਤੀ->owner = val ? xstrdup (val) : ਨਿਰਾ;
  }
  {
    tmp = json_object_object_get (doc, "detached");
    ਸਥਿਤੀ->detached = tmp != ਨਿਰਾ && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "no-new-keyring");
    ਸਥਿਤੀ->no_new_keyring = tmp != ਨਿਰਾ && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "external_descriptors");
    val = tmp ? json_object_get_string (tmp) : ਨਿਰਾ;
    ਸਥਿਤੀ->external_descriptors = val ? xstrdup (val) : ਨਿਰਾ;
  }
  ret = 0;

ਬਾਹਰ_ਨਿਕਲੋ:
  json_object_put (doc);
  ਵਾਪਸ ret;
}

ਪੂਰਨ
libcrun_ਸਥਿਤੀ_check_directories (ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *dir = ਨਿਰਾ;
  ਪੂਰਨ ret;

  ret = libcrun_get_state_directory (&dir, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = crun_ਰਸਤਾ_exists (dir, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (ret)
    ਵਾਪਸ crun_make_error (err, 0, "container `%s` already exists", id);

  ਜੇ (UNLIKELY (mkdir (dir, 0700) < 0))
    ਵਾਪਸ crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  ਵਾਪਸ 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RMDIRFD_MAX_ਚਾਲਾਂ (1U << 20)

ਸਥਿਰ ਪੂਰਨ
rmdirfd_move_ਮੰਜ਼ਿਲ (DIR **d, ਅਚਲ ਅੱਖਰ *name, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ fd = -1;
  DIR *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  ਜੇ (UNLIKELY (new_d == ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, errno, "cannot open directory `%s`", name);

  fd = -1;

  closedir (*d);
  *d = new_d;
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
rmdirfd (ਅਚਲ ਅੱਖਰ *namedir, ਪੂਰਨ fd, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  cleanup_dir DIR *d = ਨਿਰਾ;
  ਬਣਤਰ dirent *de;
  ਬਣਤਰ stat ਜੜ੍ਹ_st;
  bool is_ਜੜ੍ਹ = true;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ budget = RMDIRFD_MAX_ਚਾਲਾਂ;
  __attribute__ ((ਅਣਵਰਤਿਆ)) cleanup_close ਪੂਰਨ fd_cleanup = fd;

  ret = fstat (fd, &ਜੜ੍ਹ_st);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "stat `%s`", namedir);

  d = fdopendir (fd);
  ਜੇ (d == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, errno, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  ਲਈ (;;)
    {
      errno = 0;
      de = readdir (d);
      ਜੇ (de == ਨਿਰਾ)
        {
          ਬਣਤਰ stat st;

          ਜੇ (UNLIKELY (errno != 0))
            ਵਾਪਸ crun_make_error (err, errno, "cannot read directory");

          ਜੇ (is_ਜੜ੍ਹ)
            ਰੋਕੋ;

          ਜੇ (UNLIKELY (--budget == 0))
            ਵਾਪਸ crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_ਮੰਜ਼ਿਲ (&d, "..", err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;

          ret = fstat (dirfd (d), &st);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "stat `..`");

          is_ਜੜ੍ਹ = st.st_dev == ਜੜ੍ਹ_st.st_dev && st.st_ino == ਜੜ੍ਹ_st.st_ino;
          ਜਾਰੀ;
        }

      ਜੇ ((strcmp (de->d_name, ".") == 0) || (strcmp (de->d_name, "..") == 0))
        ਜਾਰੀ;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      ਜੇ (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          ਜੇ (ret < 0 && errno == EBUSY)
            {
              cleanup_close ਪੂਰਨ tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_ਰਸਤਾ | O_NOFOLLOW);
              ਜੇ (tfd >= 0)
                {
                  proc_fd_ਰਸਤਾ_t procpath;

                  get_proc_self_fd_ਰਸਤਾ (procpath, tfd);
                  ਜੇ (umount2 (procpath, MNT_DETACH) == 0)
                    ਜਾਓ retry_unlink;
                }
              ਜੇ (! is_ਜੜ੍ਹ)
                ਵਾਪਸ crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          ਨਹੀਂਤਾਂ ਜੇ (ret < 0 && errno == ENOTEMPTY)
            {
              ਜੇ (UNLIKELY (--budget == 0))
                ਵਾਪਸ crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_ਮੰਜ਼ਿਲ (&d, de->d_name, err);
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ ret;

              is_ਜੜ੍ਹ = false;
            }
          ਨਹੀਂਤਾਂ ਜੇ (ret < 0 && errno != ENOENT && ! is_ਜੜ੍ਹ)
            {
              ਵਾਪਸ crun_make_error (err, errno, "cannot rm `%s`", de->d_name);
            }
        }
    }

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_container_delete_ਸਥਿਤੀ (ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  cleanup_close ਪੂਰਨ rundir_dfd = -1;
  cleanup_close ਪੂਰਨ dfd = -1;
  cleanup_free ਅੱਖਰ *dir = ਨਿਰਾ;

  ret = validate_id (id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = get_ਲੜੀਵਾਰ_directory (&dir, state_ਜੜ੍ਹ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  rundir_dfd = ਆਰਜ਼ੀ_FAILURE_RETRY (open (dir, O_DIRECTORY | O_ਰਸਤਾ | O_CLOEXEC));
  ਜੇ (UNLIKELY (rundir_dfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  ਜੇ (UNLIKELY (dfd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "cannot rm state directory `%s/%s`", dir, id);

  ਵਾਪਸ 0;
}

ਖਾਲੀ
libcrun_free_container_ਸਥਿਤੀ (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ == ਨਿਰਾ)
    ਵਾਪਸ;
  ਮੁਕਤ (ਸਥਿਤੀ->cgroup_ਰਸਤਾ);
  ਮੁਕਤ (ਸਥਿਤੀ->bundle);
  ਮੁਕਤ (ਸਥਿਤੀ->rootfs);
  ਮੁਕਤ (ਸਥਿਤੀ->external_descriptors);
  ਮੁਕਤ (ਸਥਿਤੀ->created);
  ਮੁਕਤ (ਸਥਿਤੀ->scope);
  ਮੁਕਤ (ਸਥਿਤੀ->owner);
}

ਪੂਰਨ
libcrun_get_containers_list (libcrun_container_list_t **ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, libcrun_error_t *err)
{
  ਬਣਤਰ dirent *ਅਗਲਾ;
  cleanup_container_list libcrun_container_list_t *tmp = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *ਜੜ੍ਹ = ਨਿਰਾ;
  cleanup_dir DIR *dir = ਨਿਰਾ;
  ਪੂਰਨ ret;

  *ਬਾਹਰਲਾ = ਨਿਰਾ;

  ret = get_ਲੜੀਵਾਰ_directory (&ਜੜ੍ਹ, state_ਜੜ੍ਹ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  dir = opendir (ਜੜ੍ਹ);
  ਜੇ (UNLIKELY (dir == ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, errno, "cannot opendir `%s`", ਜੜ੍ਹ);

  ਲਈ (ਅਗਲਾ = readdir (dir); ਅਗਲਾ; ਅਗਲਾ = readdir (dir))
    {
      ਪੂਰਨ exists;
      cleanup_free ਅੱਖਰ *ਸਥਿਤੀ_file = ਨਿਰਾ;

      libcrun_container_list_t *ਅਗਲਾ_container;

      ਜੇ (ਅਗਲਾ->d_name[0] == '.')
        ਜਾਰੀ;

      ret = append_paths (&ਸਥਿਤੀ_file, err, ਜੜ੍ਹ, ਅਗਲਾ->d_name, "status", ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      exists = crun_ਰਸਤਾ_exists (ਸਥਿਤੀ_file, err);
      ਜੇ (exists < 0)
        {
          ਵਾਪਸ exists;
        }

      ਜੇ (! exists)
        {
          libcrun_error (errno, "error opening file `%s`", ਸਥਿਤੀ_file);
          ਜਾਰੀ;
        }

      ਅਗਲਾ_container = xmalloc (ਆਕਾਰ (libcrun_container_list_t));
      ਅਗਲਾ_container->name = xstrdup (ਅਗਲਾ->d_name);
      ਅਗਲਾ_container->ਅਗਲਾ = tmp;
      tmp = ਅਗਲਾ_container;
    }

  STEAL_ਸੂਚਕ (ਬਾਹਰਲਾ, tmp);

  ਵਾਪਸ 0;
}

ਖਾਲੀ
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *ਅਗਲਾ;
  ਜਦੋਂ (list)
    {
      ਅਗਲਾ = list->ਅਗਲਾ;
      ਮੁਕਤ (list->name);
      ਮੁਕਤ (list);
      list = ਅਗਲਾ;
    }
}

/* check if the container pid is still valid.
   Returns:
   -1: on errors
    0: pid not valid
    1: pid valid and container in the running/created/paused state
*/
ਪੂਰਨ
libcrun_check_pid_valid (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, libcrun_error_t *err)
{
  ਬਣਤਰ pid_stat st;
  ਪੂਰਨ ret;

  /* For backwards compatibility, check start time only if available. */
  ਜੇ (! ਸਥਿਤੀ->process_ਸ਼ੁਰੂ_ਸਮਾਂ)
    ਵਾਪਸ 1;

  ret = read_pid_stat (ਸਥਿਤੀ->pid, &st, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (ਸਥਿਤੀ->process_ਸ਼ੁਰੂ_ਸਮਾਂ != st.starttime || st.state == 'X')
    ਵਾਪਸ 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  ਜੇ (st.state == 'Z' && st.ਅੰਕ_ਧਾਗੇ <= 1)
    ਵਾਪਸ 0; /* stopped */

  ਵਾਪਸ 1; /* running, created, or paused */
}

ਪੂਰਨ
libcrun_is_container_running (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ret = kill (ਸਥਿਤੀ->pid, 0);
  ਜੇ (UNLIKELY (ret < 0) && errno != ESRCH)
    ਵਾਪਸ crun_make_error (err, errno, "kill");

  ਜੇ (ret == 0)
    ਵਾਪਸ libcrun_check_pid_valid (ਸਥਿਤੀ, err);

  ਵਾਪਸ 0; /* stopped */
}

ਸਥਿਰ ਪੂਰਨ
get_exec_fifo_ਰਸਤਾ (ਅੱਖਰ **ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *state_dir = ਨਿਰਾ;
  ਪੂਰਨ ret;

  ret = libcrun_get_state_directory (&state_dir, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ append_paths (ਬਾਹਰਲਾ, err, state_dir, "exec.fifo", ਨਿਰਾ);
}

ਪੂਰਨ
libcrun_ਸਥਿਤੀ_create_exec_fifo (ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *fifo_ਰਸਤਾ = ਨਿਰਾ;
  ਪੂਰਨ ret, fd = -1;

  ret = get_exec_fifo_ਰਸਤਾ (&fifo_ਰਸਤਾ, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_ਰਸਤਾ);
  ret = mkfifo (fifo_ਰਸਤਾ, 0600);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "mkfifo");

  fd = open (fifo_ਰਸਤਾ, O_NONBLOCK | O_CLOEXEC);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "cannot open pipe `%s`", fifo_ਰਸਤਾ);

  ਵਾਪਸ fd;
}

ਪੂਰਨ
libcrun_ਸਥਿਤੀ_write_exec_fifo (ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *fifo_ਰਸਤਾ = ਨਿਰਾ;
  ਅੱਖਰ ਬਫਰ[1] = {
    0,
  };
  cleanup_close ਪੂਰਨ fd = -1;
  ਪੂਰਨ ret;

  ret = get_exec_fifo_ਰਸਤਾ (&fifo_ਰਸਤਾ, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  fd = open (fifo_ਰਸਤਾ, O_WRONLY | O_CLOEXEC);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "cannot open `%s`", fifo_ਰਸਤਾ);

  ret = unlink (fifo_ਰਸਤਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "unlink `%s`", fifo_ਰਸਤਾ);

  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (fd, ਬਫਰ, 1));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "write to exec.fifo");

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_ਸਥਿਤੀ_has_read_exec_fifo (ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *fifo_ਰਸਤਾ = ਨਿਰਾ;
  ਪੂਰਨ ret;

  ret = get_exec_fifo_ਰਸਤਾ (&fifo_ਰਸਤਾ, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ crun_ਰਸਤਾ_exists (fifo_ਰਸਤਾ, err);
}

ਬਣਤਰ libcrun_ਸਥਿਤੀ_s
{
  libcrun_container_ਸਥਿਤੀ_t ਸਥਿਤੀ;
  ਅੱਖਰ *id;
  ਅੱਖਰ *state_ਜੜ੍ਹ;

  /* The live state requires probing the container (and its cgroup), so it is
     resolved on demand and cached here.  */
  bool state_resolved;
  libcrun_container_state_t state;
  ਪੂਰਨ running;
};

ਪੂਰਨ
libcrun_container_ਸਥਿਤੀ_ਲੱਦੋ (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, libcrun_ਸਥਿਤੀ_t **ਬਾਹਰਲਾ, libcrun_error_t *err)
{
  libcrun_ਸਥਿਤੀ_t *st;
  ਪੂਰਨ ret;

  st = xmalloc0 (ਆਕਾਰ (*st));

  ret = libcrun_read_container_ਸਥਿਤੀ (&st->ਸਥਿਤੀ, context->state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਮੁਕਤ (st);
      ਵਾਪਸ ret;
    }

  st->id = xstrdup (id);
  st->state_ਜੜ੍ਹ = xstrdup (context->state_ਜੜ੍ਹ);

  *ਬਾਹਰਲਾ = st;
  ਵਾਪਸ 0;
}

ਖਾਲੀ
libcrun_container_ਸਥਿਤੀ_free (libcrun_ਸਥਿਤੀ_t *st)
{
  ਜੇ (st == ਨਿਰਾ)
    ਵਾਪਸ;
  libcrun_free_container_ਸਥਿਤੀ (&st->ਸਥਿਤੀ);
  ਮੁਕਤ (st->state_ਜੜ੍ਹ);
  ਮੁਕਤ (st->id);
  ਮੁਕਤ (st);
}

ਪੂਰਨ
libcrun_ਸਥਿਤੀ_get_state (libcrun_ਸਥਿਤੀ_t *st, libcrun_container_state_t *state, ਪੂਰਨ *running, libcrun_error_t *err)
{
  ਜੇ (! st->state_resolved)
    {
      ਅਚਲ ਅੱਖਰ *state_string = ਨਿਰਾ;
      ਪੂਰਨ ret;

      ret = libcrun_get_container_state_string (st->id, &st->ਸਥਿਤੀ, st->state_ਜੜ੍ਹ, &state_string, &st->running, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ਜੇ (state_string == ਨਿਰਾ)
        st->state = LIBCRUN_CONTAINER_ਸਥਿਤੀ_STOPPED;
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_ਸਥਿਤੀ_CREATING;
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_ਸਥਿਤੀ_CREATED;
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_ਸਥਿਤੀ_RUNNING;
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_ਸਥਿਤੀ_PAUSED;
      ਨਹੀਂਤਾਂ
        st->state = LIBCRUN_CONTAINER_ਸਥਿਤੀ_STOPPED;

      st->state_resolved = true;
    }

  ਜੇ (state)
    *state = st->state;
  ਜੇ (running)
    *running = st->running;
  ਵਾਪਸ 0;
}

ਅਚਲ ਅੱਖਰ *
libcrun_container_state_ਮੰਜ਼ਿਲ_string (libcrun_container_state_t state)
{
  ਚੋਣ (state)
    {
    ਹਾਲਤ LIBCRUN_CONTAINER_ਸਥਿਤੀ_CREATING:
      ਵਾਪਸ "creating";
    ਹਾਲਤ LIBCRUN_CONTAINER_ਸਥਿਤੀ_CREATED:
      ਵਾਪਸ "created";
    ਹਾਲਤ LIBCRUN_CONTAINER_ਸਥਿਤੀ_RUNNING:
      ਵਾਪਸ "running";
    ਹਾਲਤ LIBCRUN_CONTAINER_ਸਥਿਤੀ_PAUSED:
      ਵਾਪਸ "paused";
    ਹਾਲਤ LIBCRUN_CONTAINER_ਸਥਿਤੀ_STOPPED:
      ਵਾਪਸ "stopped";
    }
  ਵਾਪਸ "unknown";
}

pid_t
libcrun_ਸਥਿਤੀ_get_pid (libcrun_ਸਥਿਤੀ_t *st)
{
  ਵਾਪਸ st->ਸਥਿਤੀ.pid;
}

ਅਚਲ ਅੱਖਰ *
libcrun_ਸਥਿਤੀ_get_bundle (libcrun_ਸਥਿਤੀ_t *st)
{
  ਵਾਪਸ st->ਸਥਿਤੀ.bundle;
}

ਅਚਲ ਅੱਖਰ *
libcrun_ਸਥਿਤੀ_get_rootfs (libcrun_ਸਥਿਤੀ_t *st)
{
  ਵਾਪਸ st->ਸਥਿਤੀ.rootfs;
}

ਅਚਲ ਅੱਖਰ *
libcrun_ਸਥਿਤੀ_get_created (libcrun_ਸਥਿਤੀ_t *st)
{
  ਵਾਪਸ st->ਸਥਿਤੀ.created;
}

ਅਚਲ ਅੱਖਰ *
libcrun_ਸਥਿਤੀ_get_owner (libcrun_ਸਥਿਤੀ_t *st)
{
  ਵਾਪਸ st->ਸਥਿਤੀ.owner;
}

ਅਚਲ ਅੱਖਰ *
libcrun_ਸਥਿਤੀ_get_scope (libcrun_ਸਥਿਤੀ_t *st)
{
  ਵਾਪਸ st->ਸਥਿਤੀ.scope;
}

ਅਚਲ ਅੱਖਰ *
libcrun_ਸਥਿਤੀ_get_external_descriptors (libcrun_ਸਥਿਤੀ_t *st)
{
  ਵਾਪਸ st->ਸਥਿਤੀ.external_descriptors;
}

ਪੂਰਨ
libcrun_container_get_state_string (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ **ਬਾਹਰਲਾ, ਪੂਰਨ *running,
                                    libcrun_error_t *err)
{
  cleanup_container_ਸਥਿਤੀ libcrun_container_ਸਥਿਤੀ_t ਸਥਿਤੀ = {};
  ਪੂਰਨ local_running;
  ਪੂਰਨ ret;

  ret = libcrun_read_container_ਸਥਿਤੀ (&ਸਥਿਤੀ, context->state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_get_container_state_string (id, &ਸਥਿਤੀ, context->state_ਜੜ੍ਹ, ਬਾਹਰਲਾ, &local_running, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (running)
    *running = local_running;
  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_get_state_dir (ਅੱਖਰ **ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err)
{
  ਵਾਪਸ libcrun_get_state_directory (ਬਾਹਰਲਾ, state_ਜੜ੍ਹ, id, err);
}

ਪੂਰਨ
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **ਬਾਹਰਲਾ, libcrun_error_t *err)
{
  ਵਾਪਸ libcrun_get_containers_list (ਬਾਹਰਲਾ, context->state_ਜੜ੍ਹ, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (ਆਕਾਰ (*it));
  it->cur = l;
  ਵਾਪਸ it;
}

bool
libcrun_container_iter_ਅਗਲਾ (libcrun_container_iter_t *it, ਅਚਲ ਅੱਖਰ **id)
{
  ਜੇ (it == ਨਿਰਾ || it->cur == ਨਿਰਾ)
    ਵਾਪਸ false;

  ਜੇ (id)
    *id = it->cur->name;
  it->cur = it->cur->ਅਗਲਾ;
  ਵਾਪਸ true;
}

ਖਾਲੀ
libcrun_container_iter_free (libcrun_container_iter_t *it)
{
  ਮੁਕਤ (it);
}

ਖਾਲੀ
libcrun_container_list_free (libcrun_container_list_t *l)
{
  libcrun_free_containers_list (l);
}
