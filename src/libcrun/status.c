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
#وصف_ڪريو GNU_ذريعو

#ضمن <config.h>
#ضمن "status.h"
#ضمن "utils.h"
#ضمن "json_gen_utils.h"
#ضمن <stdlib.h>
#ضمن <unistd.h>
#ضمن <fcntl.h>
#ضمن <json-c/json.h>
#ضمن <ocispec/json_common.h>
#ضمن <sys/mount.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#ضمن <dirent.h>
#ضمن <signal.h>
#ضمن <limits.h>

#وصف_ڪريو STEAL_اشارو(x, y) \
  افعل                        \
    {                       \
      *x = y;               \
      y = عدم;             \
  } طالما (0)

بنية pid_stat
{
  حرف state;
  ڊگهو انگ_ڌاڳا;
  بنانشان ڊگهو ڊگهو starttime;
};

ساكن مضمن bool
is_valid_id_char (حرف c)
{
  إذا ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
    أرجع true;
  أرجع c == '_' || c == '+' || c == '.' || c == '-';
}

ساكن صحيح
validate_id (ثابت حرف *id, libcrun_error_t *err)
{
  إذا (id == عدم)
    أرجع 0;

  إذا (id[0] == '\0')
    أرجع crun_make_error (err, 0, "empty container ID");

  إذا (id[0] == '.')
    أرجع crun_make_error (err, 0, "invalid container ID `%s`", id);

  لكل (size_t i = 0; id[i]; i++)
    {
      إذا (! is_valid_id_char (id[i]))
        أرجع crun_make_error (err, 0, "invalid character `%c` in the container ID `%s`", id[i], id);
    }

  أرجع 0;
}

صحيح
get_لاڳيتو_directory (حرف **ٻاهريون, ثابت حرف *state_پاڙ, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_free حرف *پاڙ = عدم;

  إذا (state_پاڙ)
    پاڙ = xstrdup (state_پاڙ);
  إذا (پاڙ == عدم)
    {
      ثابت حرف *runtime_dir = getenv ("XDG_RUNTIME_DIR");
      إذا (runtime_dir && runtime_dir[0] != '\0')
        {
          ret = append_paths (&پاڙ, err, runtime_dir, "crun", عدم);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
    }
  إذا (پاڙ == عدم)
    پاڙ = xstrdup ("/run/crun");

  ret = crun_ensure_directory (پاڙ, 0700, false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  STEAL_اشارو (ٻاهريون, پاڙ);

  أرجع 0;
}

صحيح
get_shared_خالي_جايون_directory_رستو (حرف **ٻاهريون, ثابت حرف *state_پاڙ, libcrun_error_t *err)
{
  cleanup_free حرف *لاڳيتو_dir = عدم;
  صحيح ret;

  ret = get_لاڳيتو_directory (&لاڳيتو_dir, state_پاڙ, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (ٻاهريون, err, لاڳيتو_dir, ".empty-directory", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* Ensure the empty directory exists */
  ret = crun_ensure_directory (*ٻاهريون, 0555, false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 0;
}

صحيح
libcrun_get_state_directory (حرف **ٻاهريون, ثابت حرف *state_پاڙ, ثابت حرف *id, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_free حرف *رستو = عدم;
  cleanup_free حرف *پاڙ = عدم;

  ret = validate_id (id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = get_لاڳيتو_directory (&پاڙ, state_پاڙ, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&رستو, err, پاڙ, id, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  STEAL_اشارو (ٻاهريون, رستو);

  أرجع 0;
}

ساكن صحيح
get_state_directory_حالت_file (حرف **ٻاهريون, ثابت حرف *state_پاڙ, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *dir = عدم;
  صحيح ret;

  ret = libcrun_get_state_directory (&dir, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع append_paths (ٻاهريون, err, dir, "status", عدم);
}

ساكن صحيح
read_pid_stat (pid_t pid, بنية pid_stat *st, libcrun_error_t *err)
{
  cleanup_free حرف *بفر = عدم;
  cleanup_close صحيح fd = -1;
  حرف pid_stat_file[64];
  حرف *it, *s;
  صحيح i, ret;

  ret = snprintf (pid_stat_file, حجم (pid_stat_file), "/proc/%d/stat", pid);
  إذا (UNLIKELY (ret >= (صحيح) حجم (pid_stat_file)))
    أرجع crun_make_error (err, 0, "internal error: static buffer too small");

  fd = open (pid_stat_file, O_RDONLY | O_CLOEXEC);
  إذا (fd < 0)
    {
      /* The process already exited.  */
      إذا (errno == ENOENT || errno == ESRCH)
        {
          memset (st, 0, حجم (*st));
          أرجع 0;
        }
      أرجع crun_make_error (err, errno, "open state file `%s`", pid_stat_file);
    }

  ret = read_all_fd (fd, pid_stat_file, &بفر, عدم, err);
  إذا (ret < 0)
    {
      st->starttime = 0;
      st->انگ_ڌاڳا = 0;
      st->state = 'X';
      /* The process already exited.  */
      libcrun_error_release (err);
      أرجع 0;
    }

  s = عدم;

  /* Skip the first two arguments.  */
  لكل (it = بفر; it; it = strchr (it + 1, ')'))
    s = it + 1;

  إذا (s)
    طالما (*s == ' ')
      s++;

  إذا (s == عدم || *s == '\0')
    أرجع crun_make_error (err, 0, "could not read process state");

  st->state = *s;

  /* Seek to the starttime argument.  */
  لكل (it = s + 1, i = 0; i < 19; i++)
    {
      it = strchr (it, ' ');
      إذا (it == عدم)
        أرجع crun_make_error (err, 0, "could not read process start time");
      it++;

      /* num_threads is the second argument before starttime.  */
      إذا (i == 16)
        st->انگ_ڌاڳا = strtol (it, عدم, 10);
    }

  errno = 0;
  st->starttime = strtoull (it, عدم, 10);
  إذا (errno != 0)
    أرجع crun_make_error (err, errno, "parse process start time");

  أرجع 0;
}

صحيح
libcrun_write_container_حالت (ثابت حرف *state_پاڙ, ثابت حرف *id, libcrun_container_حالت_t *حالت,
                                libcrun_error_t *err)
{
  صحيح r, ret;
  cleanup_free حرف *file_tmp = عدم;
  cleanup_free حرف *file = عدم;
  size_t len;
  cleanup_close صحيح fd_write = -1;
  ثابت حرف *buf = عدم;
  بنية pid_stat st;
  ثابت حرف *tmp;
  cleanup_json_gen json_gen_ctx *gen = عدم;

  ret = get_state_directory_حالت_file (&file, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = read_pid_stat (حالت->pid, &st, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* read_pid_stat returns a start time of 0 when the process already exited,
     e.g. a restored container that terminated before the status is written.
     Do not store the 0, libcrun_check_pid_valid treats it as "no start time
     available" and would accept any process that later reuses the same pid.
     Store a value that no process can have instead, so that the container is
     reported as stopped.  */
  حالت->process_شروعات_وقت = st.starttime ? st.starttime : ULLONG_MAX;

  xasprintf (&file_tmp, "%s.tmp", file);
  fd_write = open (file_tmp, O_CREAT | O_WRONLY | O_CLOEXEC, 0700);
  إذا (UNLIKELY (fd_write < 0))
    أرجع crun_make_error (err, errno, "cannot open status file");

  إذا (! json_gen_init (&gen, عدم))
    أرجع crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ڪنجي (gen, "pid");
  GEN_OR_FAIL (map_int (gen, حالت->pid));

  GEN_ڪنجي (gen, "process-start-time");
  GEN_OR_FAIL (map_uint (gen, حالت->process_شروعات_وقت));

  GEN_ڪنجي (gen, "cgroup-path");
  tmp = حالت->cgroup_رستو ? حالت->cgroup_رستو : "";
  GEN_STR (gen, tmp);

  GEN_ڪنجي (gen, "scope");
  tmp = حالت->scope ? حالت->scope : "";
  GEN_STR (gen, tmp);

  GEN_ڪنجي (gen, "rootfs");
  GEN_STR (gen, حالت->rootfs);

  GEN_ڪنجي (gen, "systemd-cgroup");
  GEN_OR_FAIL (json_gen_bool (gen, حالت->systemd_cgroup));

  GEN_ڪنجي (gen, "bundle");
  GEN_STR (gen, حالت->bundle);

  GEN_ڪنجي (gen, "created");
  GEN_STR (gen, حالت->created);

  إذا (حالت->owner)
    {
      GEN_ڪنجي (gen, "owner");
      GEN_STR (gen, حالت->owner);
    }

  GEN_ڪنجي (gen, "detached");
  GEN_OR_FAIL (json_gen_bool (gen, حالت->detached));

  GEN_ڪنجي (gen, "no-new-keyring");
  GEN_OR_FAIL (json_gen_bool (gen, حالت->no_new_keyring));

  إذا (حالت->external_descriptors)
    {
      GEN_ڪنجي (gen, "external_descriptors");
      GEN_STR (gen, حالت->external_descriptors);
    }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ret = محفوظ_write (fd_write, "status file", buf, len, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  close_and_reset (&fd_write);

  إذا (UNLIKELY (rename (file_tmp, file) < 0))
    أرجع crun_make_error (err, errno, "cannot rename status file");

  أرجع 0;

gen_error:
  أرجع json_gen_error_منزل_crun_error (r, err);
}

صحيح
libcrun_read_container_حالت (libcrun_container_حالت_t *حالت, ثابت حرف *state_پاڙ, ثابت حرف *id,
                               libcrun_error_t *err)
{
  cleanup_free حرف *بفر = عدم;
  صحيح ret;
  cleanup_free حرف *file = عدم;
  json_object *doc = عدم;
  json_object *tmp;
  ثابت حرف *val;

  ret = get_state_directory_حالت_file (&file, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = read_all_file (file, &بفر, عدم, err);
  إذا (UNLIKELY (ret < 0))
    {

      إذا (crun_error_get_errno (err) == ENOENT)
        {
          cleanup_free حرف *statedir = عدم;
          libcrun_error_t tmp_err;
          صحيح tmp_ret;

          tmp_ret = libcrun_get_state_directory (&statedir, state_پاڙ, id, &tmp_err);
          إذا (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
          وإلا
            {
              tmp_ret = crun_رستو_exists (statedir, &tmp_err);
              إذا (UNLIKELY (tmp_ret < 0))
                crun_error_release (&tmp_err);
              وإلا إذا (tmp_ret == 0)
                أرجع crun_error_wrap (err, "container `%s` does not exist", id);
            }
        }
      أرجع ret;
    }

  ret = parse_json_file (&doc, بفر, عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  {
    tmp = json_object_object_get (doc, "pid");
    إذا (UNLIKELY (tmp == عدم))
      {
        ret = crun_make_error (err, 0, "`pid` missing in `%s`", file);
        اذهب ٻاهر_نڪرو;
      }
    إذا (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`pid` is not a numeric value in `%s`", file);
        اذهب ٻاهر_نڪرو;
      }
    حالت->pid = (pid_t) json_object_get_int64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "process-start-time");
    إذا (UNLIKELY (tmp == عدم))
      حالت->process_شروعات_وقت = 0; /* backwards compatibility */
    وإلا إذا (UNLIKELY (! json_object_is_type (tmp, json_type_int)))
      {
        ret = crun_make_error (err, 0, "`process-start-time` is not a numeric value in `%s`", file);
        اذهب ٻاهر_نڪرو;
      }
    وإلا
      حالت->process_شروعات_وقت = json_object_get_uint64 (tmp);
  }
  {
    tmp = json_object_object_get (doc, "cgroup-path");
    إذا (UNLIKELY (tmp == عدم))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` missing in `%s`", file);
        اذهب ٻاهر_نڪرو;
      }
    val = json_object_get_string (tmp);
    إذا (UNLIKELY (val == عدم))
      {
        ret = crun_make_error (err, 0, "`cgroup-path` is not a string in `%s`", file);
        اذهب ٻاهر_نڪرو;
      }
    حالت->cgroup_رستو = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "scope");
    val = tmp ? json_object_get_string (tmp) : عدم;
    حالت->scope = val ? xstrdup (val) : عدم;
  }
  {
    tmp = json_object_object_get (doc, "rootfs");
    إذا (UNLIKELY (tmp == عدم))
      {
        ret = crun_make_error (err, 0, "`rootfs` missing in `%s`", file);
        اذهب ٻاهر_نڪرو;
      }
    val = json_object_get_string (tmp);
    إذا (UNLIKELY (val == عدم))
      {
        ret = crun_make_error (err, 0, "`rootfs` is not a string in `%s`", file);
        اذهب ٻاهر_نڪرو;
      }
    حالت->rootfs = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "systemd-cgroup");
    حالت->systemd_cgroup = tmp != عدم && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "bundle");
    إذا (UNLIKELY (tmp == عدم))
      {
        ret = crun_make_error (err, 0, "`bundle` missing in `%s`", file);
        اذهب ٻاهر_نڪرو;
      }
    val = json_object_get_string (tmp);
    إذا (UNLIKELY (val == عدم))
      {
        ret = crun_make_error (err, 0, "`bundle` is not a string in `%s`", file);
        اذهب ٻاهر_نڪرو;
      }
    حالت->bundle = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "created");
    إذا (UNLIKELY (tmp == عدم))
      {
        ret = crun_make_error (err, 0, "`created` missing in `%s`", file);
        اذهب ٻاهر_نڪرو;
      }
    val = json_object_get_string (tmp);
    إذا (UNLIKELY (val == عدم))
      {
        ret = crun_make_error (err, 0, "`created` is not a string in `%s`", file);
        اذهب ٻاهر_نڪرو;
      }
    حالت->created = xstrdup (val);
  }
  {
    tmp = json_object_object_get (doc, "owner");
    val = tmp ? json_object_get_string (tmp) : عدم;
    حالت->owner = val ? xstrdup (val) : عدم;
  }
  {
    tmp = json_object_object_get (doc, "detached");
    حالت->detached = tmp != عدم && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "no-new-keyring");
    حالت->no_new_keyring = tmp != عدم && json_object_get_boolean (tmp);
  }
  {
    tmp = json_object_object_get (doc, "external_descriptors");
    val = tmp ? json_object_get_string (tmp) : عدم;
    حالت->external_descriptors = val ? xstrdup (val) : عدم;
  }
  ret = 0;

ٻاهر_نڪرو:
  json_object_put (doc);
  أرجع ret;
}

صحيح
libcrun_حالت_check_directories (ثابت حرف *state_پاڙ, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *dir = عدم;
  صحيح ret;

  ret = libcrun_get_state_directory (&dir, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = crun_رستو_exists (dir, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (ret)
    أرجع crun_make_error (err, 0, "container `%s` already exists", id);

  إذا (UNLIKELY (mkdir (dir, 0700) < 0))
    أرجع crun_make_error (err, 0, "cannot create state directory for `%s`", id);

  أرجع 0;
}

/* Upper bound on the directories visited while removing a tree, so that an
   attacker constantly creating new ones cannot keep the walk going forever.  */
#وصف_ڪريو RMDIRFD_MAX_چالون (1U << 20)

ساكن صحيح
rmdirfd_move_منزل (DIR **d, ثابت حرف *name, libcrun_error_t *err)
{
  cleanup_close صحيح fd = -1;
  DIR *new_d;

  fd = openat (dirfd (*d), name, O_DIRECTORY | O_RDONLY | O_NOFOLLOW | O_CLOEXEC);
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "cannot open directory `%s`", name);

  new_d = fdopendir (fd);
  إذا (UNLIKELY (new_d == عدم))
    أرجع crun_make_error (err, errno, "cannot open directory `%s`", name);

  fd = -1;

  closedir (*d);
  *d = new_d;
  أرجع 0;
}

ساكن صحيح
rmdirfd (ثابت حرف *namedir, صحيح fd, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_dir DIR *d = عدم;
  بنية dirent *de;
  بنية stat پاڙ_st;
  bool is_پاڙ = true;
  بنانشان صحيح budget = RMDIRFD_MAX_چالون;
  __attribute__ ((اڻ_استعمال_ٿيل)) cleanup_close صحيح fd_cleanup = fd;

  ret = fstat (fd, &پاڙ_st);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "stat `%s`", namedir);

  d = fdopendir (fd);
  إذا (d == عدم)
    أرجع crun_make_error (err, errno, "cannot open directory `%s`", namedir);

  /* Now D owns FD. */
  fd_cleanup = -1;

  لكل (;;)
    {
      errno = 0;
      de = readdir (d);
      إذا (de == عدم)
        {
          بنية stat st;

          إذا (UNLIKELY (errno != 0))
            أرجع crun_make_error (err, errno, "cannot read directory");

          إذا (is_پاڙ)
            اكسر;

          إذا (UNLIKELY (--budget == 0))
            أرجع crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

          ret = rmdirfd_move_منزل (&d, "..", err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          ret = fstat (dirfd (d), &st);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "stat `..`");

          is_پاڙ = st.st_dev == پاڙ_st.st_dev && st.st_ino == پاڙ_st.st_ino;
          استمر;
        }

      إذا ((strcmp (de->d_name, ".") == 0) || (strcmp (de->d_name, "..") == 0))
        استمر;

      /* Ignore errors here and keep deleting, the final unlinkat (AT_REMOVEDIR) will fail anyway.  */
      ret = unlinkat (dirfd (d), de->d_name, 0);
      إذا (ret < 0)
        {
        retry_unlink:
          ret = unlinkat (dirfd (d), de->d_name, AT_REMOVEDIR);
          إذا (ret < 0 && errno == EBUSY)
            {
              cleanup_close صحيح tfd = openat (dirfd (d), de->d_name, O_CLOEXEC | O_رستو | O_NOFOLLOW);
              إذا (tfd >= 0)
                {
                  proc_fd_رستو_t procpath;

                  get_proc_self_fd_رستو (procpath, tfd);
                  إذا (umount2 (procpath, MNT_DETACH) == 0)
                    اذهب retry_unlink;
                }
              إذا (! is_پاڙ)
                أرجع crun_make_error (err, EBUSY, "cannot rm directory `%s`", de->d_name);
            }
          وإلا إذا (ret < 0 && errno == ENOTEMPTY)
            {
              إذا (UNLIKELY (--budget == 0))
                أرجع crun_make_error (err, ELOOP, "the directory tree keeps changing while removing `%s`", namedir);

              ret = rmdirfd_move_منزل (&d, de->d_name, err);
              إذا (UNLIKELY (ret < 0))
                أرجع ret;

              is_پاڙ = false;
            }
          وإلا إذا (ret < 0 && errno != ENOENT && ! is_پاڙ)
            {
              أرجع crun_make_error (err, errno, "cannot rm `%s`", de->d_name);
            }
        }
    }

  أرجع 0;
}

صحيح
libcrun_container_delete_حالت (ثابت حرف *state_پاڙ, ثابت حرف *id, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_close صحيح rundir_dfd = -1;
  cleanup_close صحيح dfd = -1;
  cleanup_free حرف *dir = عدم;

  ret = validate_id (id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = get_لاڳيتو_directory (&dir, state_پاڙ, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  rundir_dfd = عارضي_FAILURE_RETRY (open (dir, O_DIRECTORY | O_رستو | O_CLOEXEC));
  إذا (UNLIKELY (rundir_dfd < 0))
    أرجع crun_make_error (err, errno, "cannot open run directory `%s`", dir);

  dfd = openat (rundir_dfd, id, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  إذا (UNLIKELY (dfd < 0))
    أرجع crun_make_error (err, errno, "cannot open directory `%s/%s`", dir, id);

  ret = rmdirfd (dir, dfd, err);

  /* rmdirfd owns DFD.  */
  dfd = -1;

  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = unlinkat (rundir_dfd, id, AT_REMOVEDIR);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "cannot rm state directory `%s/%s`", dir, id);

  أرجع 0;
}

فراغ
libcrun_free_container_حالت (libcrun_container_حالت_t *حالت)
{
  إذا (حالت == عدم)
    أرجع;
  حرر (حالت->cgroup_رستو);
  حرر (حالت->bundle);
  حرر (حالت->rootfs);
  حرر (حالت->external_descriptors);
  حرر (حالت->created);
  حرر (حالت->scope);
  حرر (حالت->owner);
}

صحيح
libcrun_get_containers_list (libcrun_container_list_t **ٻاهريون, ثابت حرف *state_پاڙ, libcrun_error_t *err)
{
  بنية dirent *ايندڙ;
  cleanup_container_list libcrun_container_list_t *tmp = عدم;
  cleanup_free حرف *پاڙ = عدم;
  cleanup_dir DIR *dir = عدم;
  صحيح ret;

  *ٻاهريون = عدم;

  ret = get_لاڳيتو_directory (&پاڙ, state_پاڙ, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  dir = opendir (پاڙ);
  إذا (UNLIKELY (dir == عدم))
    أرجع crun_make_error (err, errno, "cannot opendir `%s`", پاڙ);

  لكل (ايندڙ = readdir (dir); ايندڙ; ايندڙ = readdir (dir))
    {
      صحيح exists;
      cleanup_free حرف *حالت_file = عدم;

      libcrun_container_list_t *ايندڙ_container;

      إذا (ايندڙ->d_name[0] == '.')
        استمر;

      ret = append_paths (&حالت_file, err, پاڙ, ايندڙ->d_name, "status", عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      exists = crun_رستو_exists (حالت_file, err);
      إذا (exists < 0)
        {
          أرجع exists;
        }

      إذا (! exists)
        {
          libcrun_error (errno, "error opening file `%s`", حالت_file);
          استمر;
        }

      ايندڙ_container = xmalloc (حجم (libcrun_container_list_t));
      ايندڙ_container->name = xstrdup (ايندڙ->d_name);
      ايندڙ_container->ايندڙ = tmp;
      tmp = ايندڙ_container;
    }

  STEAL_اشارو (ٻاهريون, tmp);

  أرجع 0;
}

فراغ
libcrun_free_containers_list (libcrun_container_list_t *list)
{
  libcrun_container_list_t *ايندڙ;
  طالما (list)
    {
      ايندڙ = list->ايندڙ;
      حرر (list->name);
      حرر (list);
      list = ايندڙ;
    }
}

/* check if the container pid is still valid.
   Returns:
   -1: on errors
    0: pid not valid
    1: pid valid and container in the running/created/paused state
*/
صحيح
libcrun_check_pid_valid (libcrun_container_حالت_t *حالت, libcrun_error_t *err)
{
  بنية pid_stat st;
  صحيح ret;

  /* For backwards compatibility, check start time only if available. */
  إذا (! حالت->process_شروعات_وقت)
    أرجع 1;

  ret = read_pid_stat (حالت->pid, &st, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (حالت->process_شروعات_وقت != st.starttime || st.state == 'X')
    أرجع 0; /* stopped */

  /* /proc/PID/stat describes only the thread group leader.  If it exited on
     its own, it stays a zombie while other threads of the process are still
     running.  */
  إذا (st.state == 'Z' && st.انگ_ڌاڳا <= 1)
    أرجع 0; /* stopped */

  أرجع 1; /* running, created, or paused */
}

صحيح
libcrun_is_container_running (libcrun_container_حالت_t *حالت, libcrun_error_t *err)
{
  صحيح ret;

  ret = kill (حالت->pid, 0);
  إذا (UNLIKELY (ret < 0) && errno != ESRCH)
    أرجع crun_make_error (err, errno, "kill");

  إذا (ret == 0)
    أرجع libcrun_check_pid_valid (حالت, err);

  أرجع 0; /* stopped */
}

ساكن صحيح
get_exec_fifo_رستو (حرف **ٻاهريون, ثابت حرف *state_پاڙ, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *state_dir = عدم;
  صحيح ret;

  ret = libcrun_get_state_directory (&state_dir, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع append_paths (ٻاهريون, err, state_dir, "exec.fifo", عدم);
}

صحيح
libcrun_حالت_create_exec_fifo (ثابت حرف *state_پاڙ, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *fifo_رستو = عدم;
  صحيح ret, fd = -1;

  ret = get_exec_fifo_رستو (&fifo_رستو, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  libcrun_debug ("Creating exec fifo: %s", fifo_رستو);
  ret = mkfifo (fifo_رستو, 0600);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "mkfifo");

  fd = open (fifo_رستو, O_NONBLOCK | O_CLOEXEC);
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "cannot open pipe `%s`", fifo_رستو);

  أرجع fd;
}

صحيح
libcrun_حالت_write_exec_fifo (ثابت حرف *state_پاڙ, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *fifo_رستو = عدم;
  حرف بفر[1] = {
    0,
  };
  cleanup_close صحيح fd = -1;
  صحيح ret;

  ret = get_exec_fifo_رستو (&fifo_رستو, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  fd = open (fifo_رستو, O_WRONLY | O_CLOEXEC);
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "cannot open `%s`", fifo_رستو);

  ret = unlink (fifo_رستو);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "unlink `%s`", fifo_رستو);

  ret = عارضي_FAILURE_RETRY (اكتب (fd, بفر, 1));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "write to exec.fifo");

  أرجع 0;
}

صحيح
libcrun_حالت_has_read_exec_fifo (ثابت حرف *state_پاڙ, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_free حرف *fifo_رستو = عدم;
  صحيح ret;

  ret = get_exec_fifo_رستو (&fifo_رستو, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع crun_رستو_exists (fifo_رستو, err);
}

بنية libcrun_حالت_s
{
  libcrun_container_حالت_t حالت;
  حرف *id;
  حرف *state_پاڙ;

  /* The live state requires probing the container (and its cgroup), so it is
     resolved on demand and cached here.  */
  bool state_resolved;
  libcrun_container_state_t state;
  صحيح running;
};

صحيح
libcrun_container_حالت_ڀريو (libcrun_context_t *context, ثابت حرف *id, libcrun_حالت_t **ٻاهريون, libcrun_error_t *err)
{
  libcrun_حالت_t *st;
  صحيح ret;

  st = xmalloc0 (حجم (*st));

  ret = libcrun_read_container_حالت (&st->حالت, context->state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    {
      حرر (st);
      أرجع ret;
    }

  st->id = xstrdup (id);
  st->state_پاڙ = xstrdup (context->state_پاڙ);

  *ٻاهريون = st;
  أرجع 0;
}

فراغ
libcrun_container_حالت_free (libcrun_حالت_t *st)
{
  إذا (st == عدم)
    أرجع;
  libcrun_free_container_حالت (&st->حالت);
  حرر (st->state_پاڙ);
  حرر (st->id);
  حرر (st);
}

صحيح
libcrun_حالت_get_state (libcrun_حالت_t *st, libcrun_container_state_t *state, صحيح *running, libcrun_error_t *err)
{
  إذا (! st->state_resolved)
    {
      ثابت حرف *state_string = عدم;
      صحيح ret;

      ret = libcrun_get_container_state_string (st->id, &st->حالت, st->state_پاڙ, &state_string, &st->running, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      إذا (state_string == عدم)
        st->state = LIBCRUN_CONTAINER_حالت_STOPPED;
      وإلا إذا (strcmp (state_string, "creating") == 0)
        st->state = LIBCRUN_CONTAINER_حالت_CREATING;
      وإلا إذا (strcmp (state_string, "created") == 0)
        st->state = LIBCRUN_CONTAINER_حالت_CREATED;
      وإلا إذا (strcmp (state_string, "running") == 0)
        st->state = LIBCRUN_CONTAINER_حالت_RUNNING;
      وإلا إذا (strcmp (state_string, "paused") == 0)
        st->state = LIBCRUN_CONTAINER_حالت_PAUSED;
      وإلا
        st->state = LIBCRUN_CONTAINER_حالت_STOPPED;

      st->state_resolved = true;
    }

  إذا (state)
    *state = st->state;
  إذا (running)
    *running = st->running;
  أرجع 0;
}

ثابت حرف *
libcrun_container_state_منزل_string (libcrun_container_state_t state)
{
  بدل (state)
    {
    حالة LIBCRUN_CONTAINER_حالت_CREATING:
      أرجع "creating";
    حالة LIBCRUN_CONTAINER_حالت_CREATED:
      أرجع "created";
    حالة LIBCRUN_CONTAINER_حالت_RUNNING:
      أرجع "running";
    حالة LIBCRUN_CONTAINER_حالت_PAUSED:
      أرجع "paused";
    حالة LIBCRUN_CONTAINER_حالت_STOPPED:
      أرجع "stopped";
    }
  أرجع "unknown";
}

pid_t
libcrun_حالت_get_pid (libcrun_حالت_t *st)
{
  أرجع st->حالت.pid;
}

ثابت حرف *
libcrun_حالت_get_bundle (libcrun_حالت_t *st)
{
  أرجع st->حالت.bundle;
}

ثابت حرف *
libcrun_حالت_get_rootfs (libcrun_حالت_t *st)
{
  أرجع st->حالت.rootfs;
}

ثابت حرف *
libcrun_حالت_get_created (libcrun_حالت_t *st)
{
  أرجع st->حالت.created;
}

ثابت حرف *
libcrun_حالت_get_owner (libcrun_حالت_t *st)
{
  أرجع st->حالت.owner;
}

ثابت حرف *
libcrun_حالت_get_scope (libcrun_حالت_t *st)
{
  أرجع st->حالت.scope;
}

ثابت حرف *
libcrun_حالت_get_external_descriptors (libcrun_حالت_t *st)
{
  أرجع st->حالت.external_descriptors;
}

صحيح
libcrun_container_get_state_string (libcrun_context_t *context, ثابت حرف *id, ثابت حرف **ٻاهريون, صحيح *running,
                                    libcrun_error_t *err)
{
  cleanup_container_حالت libcrun_container_حالت_t حالت = {};
  صحيح local_running;
  صحيح ret;

  ret = libcrun_read_container_حالت (&حالت, context->state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_get_container_state_string (id, &حالت, context->state_پاڙ, ٻاهريون, &local_running, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (running)
    *running = local_running;
  أرجع 0;
}

صحيح
libcrun_get_state_dir (حرف **ٻاهريون, ثابت حرف *state_پاڙ, ثابت حرف *id, libcrun_error_t *err)
{
  أرجع libcrun_get_state_directory (ٻاهريون, state_پاڙ, id, err);
}

صحيح
libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **ٻاهريون, libcrun_error_t *err)
{
  أرجع libcrun_get_containers_list (ٻاهريون, context->state_پاڙ, err);
}

libcrun_container_iter_t *
libcrun_container_list_iter (libcrun_container_list_t *l)
{
  libcrun_container_iter_t *it = xmalloc0 (حجم (*it));
  it->cur = l;
  أرجع it;
}

bool
libcrun_container_iter_ايندڙ (libcrun_container_iter_t *it, ثابت حرف **id)
{
  إذا (it == عدم || it->cur == عدم)
    أرجع false;

  إذا (id)
    *id = it->cur->name;
  it->cur = it->cur->ايندڙ;
  أرجع true;
}

فراغ
libcrun_container_iter_free (libcrun_container_iter_t *it)
{
  حرر (it);
}

فراغ
libcrun_container_list_free (libcrun_container_list_t *l)
{
  libcrun_free_containers_list (l);
}
