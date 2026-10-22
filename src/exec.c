/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
 * crun is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * crun is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with crun.  If not, see <http://www.gnu.org/licenses/>.
 */

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <argp.h>
#ضمن <string.h>
#ضمن <unistd.h>
#ضمن <errno.h>
#ضمن <limits.h>

#ضمن "crun.h"

ساكن حرف doc[] = "OCI runtime";

بنية exec_options_s
{
  bool tty;
  bool detach;
  bool no_new_privs;
  صحيح preserve_fds;
  ثابت حرف *process;
  ثابت حرف *console_socket;
  ثابت حرف *pid_file;
  حرف *process_label;
  حرف *apparmor;
  حرف *cwd;
  حرف *user;
  حرف **env;
  حرف **cap;
  size_t cap_size;
  size_t env_size;
  حرف *cgroup;
};

تعداد
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_CWD,
  OPTION_PRESERVE_FDS,
  OPTION_NO_NEW_PRIVS,
  OPTION_PROCESS_LABEL,
  OPTION_APPARMOR,
  OPTION_CGROUP,
};

ساكن بنية exec_options_s exec_options;

ساكن بنية argp_option options[]
    = { { "console-socket", OPTION_CONSOLE_SOCKET, "SOCKET", 0,
          "path to a socket that will receive the ptmx end of the tty", 0 },
        { "tty", 't', "TTY", OPTION_ARG_OPTIONAL, "allocate a pseudo-TTY", 0 },
        { "process", 'p', "FILE", 0, "path to the process.json", 0 },
        { "cwd", OPTION_CWD, "CWD", 0, "current working directory", 0 },
        { "cgroup", OPTION_CGROUP, "PATH", 0, "sub-cgroup in the container", 0 },
        { "detach", 'd', 0, 0, "detach the command in the background", 0 },
        { "user", 'u', "USERSPEC", 0, "specify the user in the form UID[:GID]", 0 },
        { "env", 'e', "ENV", 0, "add an environment variable", 0 },
        { "cap", 'c', "CAP", 0, "add a capability", 0 },
        { "pid-file", OPTION_PID_FILE, "FILE", 0, "where to write the PID of the container", 0 },
        { "preserve-fds", OPTION_PRESERVE_FDS, "N", 0, "pass additional FDs to the container", 0 },
        { "no-new-privs", OPTION_NO_NEW_PRIVS, 0, 0, "set the no new privileges value for the process", 0 },
        { "process-label", OPTION_PROCESS_LABEL, "VALUE", 0, "set the asm process label for the process commonly used with selinux", 0 },
        { "apparmor", OPTION_APPARMOR, "VALUE", 0, "set the apparmor profile for the process", 0 },
        {
            0,
        } };

ساكن حرف args_doc[] = "exec CONTAINER cmd";

ساكن فراغ
append_to_string_array (حرف ***arr, size_t *size, ثابت حرف *arg)
{
  *arr = xrealloc (*arr, (*size + 2) * حجم (**arr));
  (*arr)[*size + 1] = عدم;
  (*arr)[*size] = xstrdup (arg);
  (*size)++;
}

ساكن فراغ
append_env (ثابت حرف *arg)
{
  append_to_string_array (&exec_options.env, &exec_options.env_size, arg);
}

ساكن فراغ
append_cap (ثابت حرف *arg)
{
  append_to_string_array (&exec_options.cap, &exec_options.cap_size, arg);
}

ساكن error_t
parse_opt (صحيح مفتاح, حرف *arg, بنية argp_state *state)
{
  بدل (مفتاح)
    {
    حالة OPTION_CONSOLE_SOCKET:
      exec_options.console_socket = arg;
      اكسر;

    حالة OPTION_PID_FILE:
      exec_options.pid_file = arg;
      اكسر;

    حالة OPTION_NO_NEW_PRIVS:
      exec_options.no_new_privs = true;
      اكسر;

    حالة OPTION_PROCESS_LABEL:
      exec_options.process_label = argp_mandatory_argument (arg, state);
      اكسر;

    حالة OPTION_APPARMOR:
      exec_options.apparmor = argp_mandatory_argument (arg, state);
      اكسر;

    حالة OPTION_PRESERVE_FDS:
      exec_options.preserve_fds = parse_id_or_fail (argp_mandatory_argument (arg, state), عدم, "preserve-fds");
      اكسر;

    حالة OPTION_CGROUP:
      exec_options.cgroup = argp_mandatory_argument (arg, state);
      اكسر;

    حالة 'd':
      exec_options.detach = true;
      اكسر;

    حالة 'p':
      exec_options.process = arg;
      اكسر;

    حالة 't':
      exec_options.tty = arg == عدم || (strcmp (arg, "false") != 0 && strcmp (arg, "no") != 0);
      اكسر;

    حالة 'u':
      exec_options.user = arg;
      اكسر;

    حالة 'e':
      append_env (arg);
      اكسر;

    حالة 'c':
      append_cap (arg);
      اكسر;

    حالة OPTION_CWD:
      exec_options.cwd = xstrdup (arg);
      اكسر;

    حالة ARGP_مفتاح_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    افتراضي:
      إرجاع ARGP_ERR_UNKNOWN;
    }

  إرجاع 0;
}

ساكن بنية argp run_argp = { options, parse_opt, args_doc, doc, عدم, عدم, عدم };

/* Minimal growable string buffer used to assemble the OCI Process JSON.  The
   binary is a pure consumer of the public API, so it builds the process as a
   JSON string rather than reaching for the internal ocispec types.  */
بنية strbuf
{
  حرف *buf;
  size_t len;
  size_t cap;
};

ساكن فراغ
sb_ensure (بنية strbuf *sb, size_t extra)
{
  إذا (sb->len + extra + 1 > sb->cap)
    {
      size_t ncap = sb->cap ? sb->cap : 256;
      بينما (ncap < sb->len + extra + 1)
        ncap *= 2;
      sb->buf = xrealloc (sb->buf, ncap);
      sb->cap = ncap;
    }
}

ساكن فراغ
sb_puts (بنية strbuf *sb, ثابت حرف *s)
{
  size_t l = strlen (s);
  sb_ensure (sb, l);
  memcpy (sb->buf + sb->len, s, l);
  sb->len += l;
  sb->buf[sb->len] = '\0';
}

ساكن فراغ
sb_putc (بنية strbuf *sb, حرف c)
{
  sb_ensure (sb, 1);
  sb->buf[sb->len++] = c;
  sb->buf[sb->len] = '\0';
}

/* Append S as a quoted, escaped JSON string.  */
ساكن فراغ
sb_put_json_string (بنية strbuf *sb, ثابت حرف *s)
{
  sb_putc (sb, '"');
  لكل (; *s; s++)
    {
      unsigned حرف c = (unsigned حرف) *s;
      بدل (c)
        {
        حالة '"':
          sb_puts (sb, "\\\"");
          اكسر;
        حالة '\\':
          sb_puts (sb, "\\\\");
          اكسر;
        حالة '\b':
          sb_puts (sb, "\\b");
          اكسر;
        حالة '\f':
          sb_puts (sb, "\\f");
          اكسر;
        حالة '\n':
          sb_puts (sb, "\\n");
          اكسر;
        حالة '\r':
          sb_puts (sb, "\\r");
          اكسر;
        حالة '\t':
          sb_puts (sb, "\\t");
          اكسر;
        افتراضي:
          إذا (c < 0x20)
            {
              حرف tmp[8];
              snprintf (tmp, حجم (tmp), "\\u%04x", c);
              sb_puts (sb, tmp);
            }
          وإلا
            sb_putc (sb, c);
        }
    }
  sb_putc (sb, '"');
}

ساكن فراغ
sb_put_json_string_array (بنية strbuf *sb, حرف **arr, size_t len)
{
  size_t i;
  sb_putc (sb, '[');
  لكل (i = 0; i < len; i++)
    {
      إذا (i)
        sb_putc (sb, ',');
      sb_put_json_string (sb, arr[i]);
    }
  sb_putc (sb, ']');
}

/* Read the whole file at PATH into a NUL-terminated string.  A PATH of `-`
   reads from stdin.  */
ساكن حرف *
read_file_to_string (ثابت حرف *path, libcrun_error_t *err)
{
  cleanup_file FILE *f = عدم;
  بنية strbuf sb = { 0 };
  حرف chunk[4096];
  size_t n;

  إذا (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  f = fopen (path, "re");
  إذا (f == عدم)
    {
      libcrun_make_error (err, errno, "cannot open `%s`", path);
      إرجاع عدم;
    }

  بينما ((n = fread (chunk, 1, حجم (chunk), f)) > 0)
    {
      sb_ensure (&sb, n);
      memcpy (sb.buf + sb.len, chunk, n);
      sb.len += n;
      sb.buf[sb.len] = '\0';
    }
  إذا (ferror (f))
    {
      حرر (sb.buf);
      libcrun_make_error (err, errno, "cannot read `%s`", path);
      إرجاع عدم;
    }

  إذا (sb.buf == عدم)
    sb.buf = xstrdup ("");
  إرجاع sb.buf;
}

/* Build the OCI Process JSON from the CLI options.  */
ساكن حرف *
build_process_json (بنية crun_global_arguments *global_args arg_unused, حرف **args, size_t args_len)
{
  بنية strbuf sb = { 0 };

  sb_putc (&sb, '{');

  sb_puts (&sb, "\"args\":");
  sb_put_json_string_array (&sb, args, args_len);

  /* cwd is a required field in the OCI process schema; the exec path treats a
     missing value as "/".  */
  sb_puts (&sb, ",\"cwd\":");
  sb_put_json_string (&sb, exec_options.cwd ? exec_options.cwd : "/");

  sb_puts (&sb, ",\"terminal\":");
  sb_puts (&sb, exec_options.tty ? "true" : "false");

  إذا (exec_options.env_size > 0)
    {
      sb_puts (&sb, ",\"env\":");
      sb_put_json_string_array (&sb, exec_options.env, exec_options.env_size);
    }

  إذا (exec_options.user)
    {
      حرف *endptr = عدم;
      حرف *gidstr;
      صحيح uid, gid = 0;
      حرف tmp[32];

      uid = parse_id_or_fail (exec_options.user, &endptr, "UID");
      إذا (*endptr == ':')
        {
          gidstr = endptr + 1;
          gid = parse_id_or_fail (gidstr, &endptr, "GID");
          إذا (*endptr != '\0')
            libcrun_fail_with_error (0, "invalid USERSPEC specified");
        }
      وإلا إذا (*endptr != '\0')
        libcrun_fail_with_error (0, "invalid USERSPEC specified");

      sb_puts (&sb, ",\"user\":{\"uid\":");
      snprintf (tmp, حجم (tmp), "%d", uid);
      sb_puts (&sb, tmp);
      sb_puts (&sb, ",\"gid\":");
      snprintf (tmp, حجم (tmp), "%d", gid);
      sb_puts (&sb, tmp);
      sb_putc (&sb, '}');
    }

  إذا (exec_options.process_label != عدم)
    {
      sb_puts (&sb, ",\"selinuxLabel\":");
      sb_put_json_string (&sb, exec_options.process_label);
    }

  إذا (exec_options.apparmor != عدم)
    {
      sb_puts (&sb, ",\"apparmorProfile\":");
      sb_put_json_string (&sb, exec_options.apparmor);
    }

  إذا (exec_options.cap_size > 0)
    {
      sb_puts (&sb, ",\"capabilities\":{");
      sb_puts (&sb, "\"effective\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_size);
      sb_puts (&sb, ",\"bounding\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_size);
      sb_puts (&sb, ",\"ambient\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_size);
      sb_puts (&sb, ",\"permitted\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_size);
      sb_putc (&sb, '}');
    }

  /* noNewPrivileges remains at the config default unless explicitly requested.  */
  إذا (exec_options.no_new_privs)
    sb_puts (&sb, ",\"noNewPrivileges\":true");

  sb_putc (&sb, '}');

  إرجاع sb.buf;
}

صحيح
crun_command_exec (بنية crun_global_arguments *global_args, صحيح argc, حرف **argv, libcrun_error_t *err)
{
  صحيح first_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = عدم;
  cleanup_free حرف *process_json = عدم;
  بنية libcrun_exec_options_s exec_opts;

  memset (&exec_opts, 0, حجم (exec_opts));
  exec_opts.struct_size = حجم (exec_opts);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &exec_options);
  crun_assert_n_args (argc - first_arg, exec_options.process ? 1 : 2, -1);

  crun_context = new_libcrun_context (global_args);

  libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds);

  إذا (getenv ("LISTEN_FDS"))
    {
      صحيح listen_fds = strtoll (getenv ("LISTEN_FDS"), عدم, 10);
      libcrun_context_set_listen_fds (crun_context, listen_fds);
      libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds + listen_fds);
    }

  /* The process file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  إذا (exec_options.process)
    {
      process_json = read_file_to_string (exec_options.process, err);
      إذا (process_json == عدم)
        إرجاع -1;
      exec_opts.merge_env = false;
    }
  وإلا
    {
      cleanup_free حرف **args = عدم;
      size_t args_len = argc - first_arg - 1;
      size_t i;

      /* argv[first_arg] is the container id, the command follows it.  */
      args = xmalloc0 ((args_len + 1) * حجم (*args));
      لكل (i = 0; i < args_len; i++)
        args[i] = argv[first_arg + i + 1];
      args[i] = عدم;

      process_json = build_process_json (global_args, args, args_len);
      exec_opts.merge_env = true;
    }

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  libcrun_context_set_detach (crun_context, exec_options.detach);
  libcrun_context_set_console_socket (crun_context, exec_options.console_socket);
  libcrun_context_set_pid_file (crun_context, exec_options.pid_file);

  exec_opts.process_json = process_json;
  exec_opts.cgroup = exec_options.cgroup;

  إرجاع libcrun_container_exec_with_options_json (crun_context, argv[first_arg], &exec_opts, err);
}
