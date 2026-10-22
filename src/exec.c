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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <argp.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <limits.h>

#ಅನ್ತರ್ಭಾವ "crun.h"

ಸ್ಥಿರ ವರ್ಣ doc[] = "OCI runtime";

ಸಂರಚನಾ exec_options_s
{
  bool tty;
  bool detach;
  bool no_new_privs;
  ಪೂರ್ಣಾಙ್ಕ preserve_fds;
  ಧ್ರುವ ವರ್ಣ *process;
  ಧ್ರುವ ವರ್ಣ *console_socket;
  ಧ್ರುವ ವರ್ಣ *pid_file;
  ವರ್ಣ *process_label;
  ವರ್ಣ *apparmor;
  ವರ್ಣ *cwd;
  ವರ್ಣ *user;
  ವರ್ಣ **env;
  ವರ್ಣ **cap;
  size_t cap_size;
  size_t env_size;
  ವರ್ಣ *cgroup;
};

ಗಣನಾ
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

ಸ್ಥಿರ ಸಂರಚನಾ exec_options_s exec_options;

ಸ್ಥಿರ ಸಂರಚನಾ argp_option options[]
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

ಸ್ಥಿರ ವರ್ಣ args_doc[] = "exec CONTAINER cmd";

ಸ್ಥಿರ ಶೂನ್ಯ
append_to_string_array (ವರ್ಣ ***arr, size_t *size, ಧ್ರುವ ವರ್ಣ *arg)
{
  *arr = xrealloc (*arr, (*size + 2) * ಪರಿಮಾಣ (**arr));
  (*arr)[*size + 1] = ರಿಕ್ತ;
  (*arr)[*size] = xstrdup (arg);
  (*size)++;
}

ಸ್ಥಿರ ಶೂನ್ಯ
append_env (ಧ್ರುವ ವರ್ಣ *arg)
{
  append_to_string_array (&exec_options.env, &exec_options.env_size, arg);
}

ಸ್ಥಿರ ಶೂನ್ಯ
append_cap (ಧ್ರುವ ವರ್ಣ *arg)
{
  append_to_string_array (&exec_options.cap, &exec_options.cap_size, arg);
}

ಸ್ಥಿರ error_t
parse_opt (ಪೂರ್ಣಾಙ್ಕ ಕೀಲಿ, ವರ್ಣ *arg, ಸಂರಚನಾ argp_state *state)
{
  ವಿಕಲ್ಪ (ಕೀಲಿ)
    {
    ಪಕ್ಷ OPTION_CONSOLE_SOCKET:
      exec_options.console_socket = arg;
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_PID_FILE:
      exec_options.pid_file = arg;
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_NO_NEW_PRIVS:
      exec_options.no_new_privs = true;
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_PROCESS_LABEL:
      exec_options.process_label = argp_mandatory_argument (arg, state);
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_APPARMOR:
      exec_options.apparmor = argp_mandatory_argument (arg, state);
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_PRESERVE_FDS:
      exec_options.preserve_fds = parse_id_or_fail (argp_mandatory_argument (arg, state), ರಿಕ್ತ, "preserve-fds");
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_CGROUP:
      exec_options.cgroup = argp_mandatory_argument (arg, state);
      ಭಙ್ಗ;

    ಪಕ್ಷ 'd':
      exec_options.detach = true;
      ಭಙ್ಗ;

    ಪಕ್ಷ 'p':
      exec_options.process = arg;
      ಭಙ್ಗ;

    ಪಕ್ಷ 't':
      exec_options.tty = arg == ರಿಕ್ತ || (strcmp (arg, "false") != 0 && strcmp (arg, "no") != 0);
      ಭಙ್ಗ;

    ಪಕ್ಷ 'u':
      exec_options.user = arg;
      ಭಙ್ಗ;

    ಪಕ್ಷ 'e':
      append_env (arg);
      ಭಙ್ಗ;

    ಪಕ್ಷ 'c':
      append_cap (arg);
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_CWD:
      exec_options.cwd = xstrdup (arg);
      ಭಙ್ಗ;

    ಪಕ್ಷ ARGP_ಕೀಲಿ_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ ARGP_ERR_UNKNOWN;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಸಂರಚನಾ argp run_argp = { options, parse_opt, args_doc, doc, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ };

/* Minimal growable string buffer used to assemble the OCI Process JSON.  The
   binary is a pure consumer of the public API, so it builds the process as a
   JSON string rather than reaching for the internal ocispec types.  */
ಸಂರಚನಾ strbuf
{
  ವರ್ಣ *buf;
  size_t len;
  size_t cap;
};

ಸ್ಥಿರ ಶೂನ್ಯ
sb_ensure (ಸಂರಚನಾ strbuf *sb, size_t extra)
{
  ಯದಿ (sb->len + extra + 1 > sb->cap)
    {
      size_t ncap = sb->cap ? sb->cap : 256;
      ಯಾವತ್ (ncap < sb->len + extra + 1)
        ncap *= 2;
      sb->buf = xrealloc (sb->buf, ncap);
      sb->cap = ncap;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
sb_puts (ಸಂರಚನಾ strbuf *sb, ಧ್ರುವ ವರ್ಣ *s)
{
  size_t l = strlen (s);
  sb_ensure (sb, l);
  memcpy (sb->buf + sb->len, s, l);
  sb->len += l;
  sb->buf[sb->len] = '\0';
}

ಸ್ಥಿರ ಶೂನ್ಯ
sb_putc (ಸಂರಚನಾ strbuf *sb, ವರ್ಣ c)
{
  sb_ensure (sb, 1);
  sb->buf[sb->len++] = c;
  sb->buf[sb->len] = '\0';
}

/* Append S as a quoted, escaped JSON string.  */
ಸ್ಥಿರ ಶೂನ್ಯ
sb_put_json_string (ಸಂರಚನಾ strbuf *sb, ಧ್ರುವ ವರ್ಣ *s)
{
  sb_putc (sb, '"');
  ಪ್ರತಿ (; *s; s++)
    {
      unsigned ವರ್ಣ c = (unsigned ವರ್ಣ) *s;
      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ '"':
          sb_puts (sb, "\\\"");
          ಭಙ್ಗ;
        ಪಕ್ಷ '\\':
          sb_puts (sb, "\\\\");
          ಭಙ್ಗ;
        ಪಕ್ಷ '\b':
          sb_puts (sb, "\\b");
          ಭಙ್ಗ;
        ಪಕ್ಷ '\f':
          sb_puts (sb, "\\f");
          ಭಙ್ಗ;
        ಪಕ್ಷ '\n':
          sb_puts (sb, "\\n");
          ಭಙ್ಗ;
        ಪಕ್ಷ '\r':
          sb_puts (sb, "\\r");
          ಭಙ್ಗ;
        ಪಕ್ಷ '\t':
          sb_puts (sb, "\\t");
          ಭಙ್ಗ;
        ಸ್ವಭಾವ:
          ಯದಿ (c < 0x20)
            {
              ವರ್ಣ tmp[8];
              snprintf (tmp, ಪರಿಮಾಣ (tmp), "\\u%04x", c);
              sb_puts (sb, tmp);
            }
          ಅನ್ಯಥಾ
            sb_putc (sb, c);
        }
    }
  sb_putc (sb, '"');
}

ಸ್ಥಿರ ಶೂನ್ಯ
sb_put_json_string_array (ಸಂರಚನಾ strbuf *sb, ವರ್ಣ **arr, size_t len)
{
  size_t i;
  sb_putc (sb, '[');
  ಪ್ರತಿ (i = 0; i < len; i++)
    {
      ಯದಿ (i)
        sb_putc (sb, ',');
      sb_put_json_string (sb, arr[i]);
    }
  sb_putc (sb, ']');
}

/* Read the whole file at PATH into a NUL-terminated string.  A PATH of `-`
   reads from stdin.  */
ಸ್ಥಿರ ವರ್ಣ *
read_file_to_string (ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err)
{
  cleanup_file FILE *f = ರಿಕ್ತ;
  ಸಂರಚನಾ strbuf sb = { 0 };
  ವರ್ಣ chunk[4096];
  size_t n;

  ಯದಿ (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  f = fopen (path, "re");
  ಯದಿ (f == ರಿಕ್ತ)
    {
      libcrun_make_error (err, errno, "cannot open `%s`", path);
      ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

  ಯಾವತ್ ((n = fread (chunk, 1, ಪರಿಮಾಣ (chunk), f)) > 0)
    {
      sb_ensure (&sb, n);
      memcpy (sb.buf + sb.len, chunk, n);
      sb.len += n;
      sb.buf[sb.len] = '\0';
    }
  ಯದಿ (ferror (f))
    {
      ಮುಕ್ತ (sb.buf);
      libcrun_make_error (err, errno, "cannot read `%s`", path);
      ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

  ಯದಿ (sb.buf == ರಿಕ್ತ)
    sb.buf = xstrdup ("");
  ಪ್ರತ್ಯಾಗಮ sb.buf;
}

/* Build the OCI Process JSON from the CLI options.  */
ಸ್ಥಿರ ವರ್ಣ *
build_process_json (ಸಂರಚನಾ crun_global_arguments *global_args arg_unused, ವರ್ಣ **args, size_t args_len)
{
  ಸಂರಚನಾ strbuf sb = { 0 };

  sb_putc (&sb, '{');

  sb_puts (&sb, "\"args\":");
  sb_put_json_string_array (&sb, args, args_len);

  /* cwd is a required field in the OCI process schema; the exec path treats a
     missing value as "/".  */
  sb_puts (&sb, ",\"cwd\":");
  sb_put_json_string (&sb, exec_options.cwd ? exec_options.cwd : "/");

  sb_puts (&sb, ",\"terminal\":");
  sb_puts (&sb, exec_options.tty ? "true" : "false");

  ಯದಿ (exec_options.env_size > 0)
    {
      sb_puts (&sb, ",\"env\":");
      sb_put_json_string_array (&sb, exec_options.env, exec_options.env_size);
    }

  ಯದಿ (exec_options.user)
    {
      ವರ್ಣ *endptr = ರಿಕ್ತ;
      ವರ್ಣ *gidstr;
      ಪೂರ್ಣಾಙ್ಕ uid, gid = 0;
      ವರ್ಣ tmp[32];

      uid = parse_id_or_fail (exec_options.user, &endptr, "UID");
      ಯದಿ (*endptr == ':')
        {
          gidstr = endptr + 1;
          gid = parse_id_or_fail (gidstr, &endptr, "GID");
          ಯದಿ (*endptr != '\0')
            libcrun_fail_with_error (0, "invalid USERSPEC specified");
        }
      ಅನ್ಯಥಾ ಯದಿ (*endptr != '\0')
        libcrun_fail_with_error (0, "invalid USERSPEC specified");

      sb_puts (&sb, ",\"user\":{\"uid\":");
      snprintf (tmp, ಪರಿಮಾಣ (tmp), "%d", uid);
      sb_puts (&sb, tmp);
      sb_puts (&sb, ",\"gid\":");
      snprintf (tmp, ಪರಿಮಾಣ (tmp), "%d", gid);
      sb_puts (&sb, tmp);
      sb_putc (&sb, '}');
    }

  ಯದಿ (exec_options.process_label != ರಿಕ್ತ)
    {
      sb_puts (&sb, ",\"selinuxLabel\":");
      sb_put_json_string (&sb, exec_options.process_label);
    }

  ಯದಿ (exec_options.apparmor != ರಿಕ್ತ)
    {
      sb_puts (&sb, ",\"apparmorProfile\":");
      sb_put_json_string (&sb, exec_options.apparmor);
    }

  ಯದಿ (exec_options.cap_size > 0)
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
  ಯದಿ (exec_options.no_new_privs)
    sb_puts (&sb, ",\"noNewPrivileges\":true");

  sb_putc (&sb, '}');

  ಪ್ರತ್ಯಾಗಮ sb.buf;
}

ಪೂರ್ಣಾಙ್ಕ
crun_command_exec (ಸಂರಚನಾ crun_global_arguments *global_args, ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ first_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *process_json = ರಿಕ್ತ;
  ಸಂರಚನಾ libcrun_exec_options_s exec_opts;

  memset (&exec_opts, 0, ಪರಿಮಾಣ (exec_opts));
  exec_opts.struct_size = ಪರಿಮಾಣ (exec_opts);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &exec_options);
  crun_assert_n_args (argc - first_arg, exec_options.process ? 1 : 2, -1);

  crun_context = new_libcrun_context (global_args);

  libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds);

  ಯದಿ (getenv ("LISTEN_FDS"))
    {
      ಪೂರ್ಣಾಙ್ಕ listen_fds = strtoll (getenv ("LISTEN_FDS"), ರಿಕ್ತ, 10);
      libcrun_context_set_listen_fds (crun_context, listen_fds);
      libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds + listen_fds);
    }

  /* The process file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  ಯದಿ (exec_options.process)
    {
      process_json = read_file_to_string (exec_options.process, err);
      ಯದಿ (process_json == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ -1;
      exec_opts.merge_env = false;
    }
  ಅನ್ಯಥಾ
    {
      cleanup_free ವರ್ಣ **args = ರಿಕ್ತ;
      size_t args_len = argc - first_arg - 1;
      size_t i;

      /* argv[first_arg] is the container id, the command follows it.  */
      args = xmalloc0 ((args_len + 1) * ಪರಿಮಾಣ (*args));
      ಪ್ರತಿ (i = 0; i < args_len; i++)
        args[i] = argv[first_arg + i + 1];
      args[i] = ರಿಕ್ತ;

      process_json = build_process_json (global_args, args, args_len);
      exec_opts.merge_env = true;
    }

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  libcrun_context_set_detach (crun_context, exec_options.detach);
  libcrun_context_set_console_socket (crun_context, exec_options.console_socket);
  libcrun_context_set_pid_file (crun_context, exec_options.pid_file);

  exec_opts.process_json = process_json;
  exec_opts.cgroup = exec_options.cgroup;

  ಪ್ರತ್ಯಾಗಮ libcrun_container_exec_with_options_json (crun_context, argv[first_arg], &exec_opts, err);
}
