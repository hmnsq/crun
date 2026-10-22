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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <argp.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <limits.h>

#അന്തര്ഭാവ "crun.h"

സ്ഥിതം അക്ഷരം doc[] = "OCI runtime";

ഘടന exec_options_s
{
  bool tty;
  bool detach;
  bool no_new_privs;
  പൂർണ്ണസംഖ്യ preserve_fds;
  സ്ഥിരം അക്ഷരം *process;
  സ്ഥിരം അക്ഷരം *console_socket;
  സ്ഥിരം അക്ഷരം *pid_file;
  അക്ഷരം *process_label;
  അക്ഷരം *apparmor;
  അക്ഷരം *cwd;
  അക്ഷരം *user;
  അക്ഷരം **env;
  അക്ഷരം **cap;
  size_t cap_size;
  size_t env_size;
  അക്ഷരം *cgroup;
};

എണ്ണൽ
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

സ്ഥിതം ഘടന exec_options_s exec_options;

സ്ഥിതം ഘടന argp_option options[]
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

സ്ഥിതം അക്ഷരം args_doc[] = "exec CONTAINER cmd";

സ്ഥിതം ശൂന്യം
append_to_string_array (അക്ഷരം ***arr, size_t *size, സ്ഥിരം അക്ഷരം *arg)
{
  *arr = xrealloc (*arr, (*size + 2) * വലുപ്പം (**arr));
  (*arr)[*size + 1] = ശൂന്യ;
  (*arr)[*size] = xstrdup (arg);
  (*size)++;
}

സ്ഥിതം ശൂന്യം
append_env (സ്ഥിരം അക്ഷരം *arg)
{
  append_to_string_array (&exec_options.env, &exec_options.env_size, arg);
}

സ്ഥിതം ശൂന്യം
append_cap (സ്ഥിരം അക്ഷരം *arg)
{
  append_to_string_array (&exec_options.cap, &exec_options.cap_size, arg);
}

സ്ഥിതം error_t
parse_opt (പൂർണ്ണസംഖ്യ താക്കോൽ, അക്ഷരം *arg, ഘടന argp_state *state)
{
  തിരഞ്ഞെടുപ്പ് (താക്കോൽ)
    {
    അവസ്ഥ OPTION_CONSOLE_SOCKET:
      exec_options.console_socket = arg;
      നിർത്തുക;

    അവസ്ഥ OPTION_PID_FILE:
      exec_options.pid_file = arg;
      നിർത്തുക;

    അവസ്ഥ OPTION_NO_NEW_PRIVS:
      exec_options.no_new_privs = true;
      നിർത്തുക;

    അവസ്ഥ OPTION_PROCESS_LABEL:
      exec_options.process_label = argp_mandatory_argument (arg, state);
      നിർത്തുക;

    അവസ്ഥ OPTION_APPARMOR:
      exec_options.apparmor = argp_mandatory_argument (arg, state);
      നിർത്തുക;

    അവസ്ഥ OPTION_PRESERVE_FDS:
      exec_options.preserve_fds = parse_id_or_fail (argp_mandatory_argument (arg, state), ശൂന്യ, "preserve-fds");
      നിർത്തുക;

    അവസ്ഥ OPTION_CGROUP:
      exec_options.cgroup = argp_mandatory_argument (arg, state);
      നിർത്തുക;

    അവസ്ഥ 'd':
      exec_options.detach = true;
      നിർത്തുക;

    അവസ്ഥ 'p':
      exec_options.process = arg;
      നിർത്തുക;

    അവസ്ഥ 't':
      exec_options.tty = arg == ശൂന്യ || (strcmp (arg, "false") != 0 && strcmp (arg, "no") != 0);
      നിർത്തുക;

    അവസ്ഥ 'u':
      exec_options.user = arg;
      നിർത്തുക;

    അവസ്ഥ 'e':
      append_env (arg);
      നിർത്തുക;

    അവസ്ഥ 'c':
      append_cap (arg);
      നിർത്തുക;

    അവസ്ഥ OPTION_CWD:
      exec_options.cwd = xstrdup (arg);
      നിർത്തുക;

    അവസ്ഥ ARGP_താക്കോൽ_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    സ്ഥിരസ്ഥിതി:
      തിരികെ ARGP_ERR_UNKNOWN;
    }

  തിരികെ 0;
}

സ്ഥിതം ഘടന argp run_argp = { options, parse_opt, args_doc, doc, ശൂന്യ, ശൂന്യ, ശൂന്യ };

/* Minimal growable string buffer used to assemble the OCI Process JSON.  The
   binary is a pure consumer of the public API, so it builds the process as a
   JSON string rather than reaching for the internal ocispec types.  */
ഘടന strbuf
{
  അക്ഷരം *buf;
  size_t len;
  size_t cap;
};

സ്ഥിതം ശൂന്യം
sb_ensure (ഘടന strbuf *sb, size_t extra)
{
  എങ്കിൽ (sb->len + extra + 1 > sb->cap)
    {
      size_t ncap = sb->cap ? sb->cap : 256;
      ആകുമ്പോൾ (ncap < sb->len + extra + 1)
        ncap *= 2;
      sb->buf = xrealloc (sb->buf, ncap);
      sb->cap = ncap;
    }
}

സ്ഥിതം ശൂന്യം
sb_puts (ഘടന strbuf *sb, സ്ഥിരം അക്ഷരം *s)
{
  size_t l = strlen (s);
  sb_ensure (sb, l);
  memcpy (sb->buf + sb->len, s, l);
  sb->len += l;
  sb->buf[sb->len] = '\0';
}

സ്ഥിതം ശൂന്യം
sb_putc (ഘടന strbuf *sb, അക്ഷരം c)
{
  sb_ensure (sb, 1);
  sb->buf[sb->len++] = c;
  sb->buf[sb->len] = '\0';
}

/* Append S as a quoted, escaped JSON string.  */
സ്ഥിതം ശൂന്യം
sb_put_json_string (ഘടന strbuf *sb, സ്ഥിരം അക്ഷരം *s)
{
  sb_putc (sb, '"');
  വേണ്ടി (; *s; s++)
    {
      unsigned അക്ഷരം c = (unsigned അക്ഷരം) *s;
      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ '"':
          sb_puts (sb, "\\\"");
          നിർത്തുക;
        അവസ്ഥ '\\':
          sb_puts (sb, "\\\\");
          നിർത്തുക;
        അവസ്ഥ '\b':
          sb_puts (sb, "\\b");
          നിർത്തുക;
        അവസ്ഥ '\f':
          sb_puts (sb, "\\f");
          നിർത്തുക;
        അവസ്ഥ '\n':
          sb_puts (sb, "\\n");
          നിർത്തുക;
        അവസ്ഥ '\r':
          sb_puts (sb, "\\r");
          നിർത്തുക;
        അവസ്ഥ '\t':
          sb_puts (sb, "\\t");
          നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
          എങ്കിൽ (c < 0x20)
            {
              അക്ഷരം tmp[8];
              snprintf (tmp, വലുപ്പം (tmp), "\\u%04x", c);
              sb_puts (sb, tmp);
            }
          അല്ലെങ്കിൽ
            sb_putc (sb, c);
        }
    }
  sb_putc (sb, '"');
}

സ്ഥിതം ശൂന്യം
sb_put_json_string_array (ഘടന strbuf *sb, അക്ഷരം **arr, size_t len)
{
  size_t i;
  sb_putc (sb, '[');
  വേണ്ടി (i = 0; i < len; i++)
    {
      എങ്കിൽ (i)
        sb_putc (sb, ',');
      sb_put_json_string (sb, arr[i]);
    }
  sb_putc (sb, ']');
}

/* Read the whole file at PATH into a NUL-terminated string.  A PATH of `-`
   reads from stdin.  */
സ്ഥിതം അക്ഷരം *
read_file_to_string (സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err)
{
  cleanup_file FILE *f = ശൂന്യ;
  ഘടന strbuf sb = { 0 };
  അക്ഷരം chunk[4096];
  size_t n;

  എങ്കിൽ (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  f = fopen (path, "re");
  എങ്കിൽ (f == ശൂന്യ)
    {
      libcrun_make_error (err, errno, "cannot open `%s`", path);
      തിരികെ ശൂന്യ;
    }

  ആകുമ്പോൾ ((n = fread (chunk, 1, വലുപ്പം (chunk), f)) > 0)
    {
      sb_ensure (&sb, n);
      memcpy (sb.buf + sb.len, chunk, n);
      sb.len += n;
      sb.buf[sb.len] = '\0';
    }
  എങ്കിൽ (ferror (f))
    {
      സ്വതന്ത്രം (sb.buf);
      libcrun_make_error (err, errno, "cannot read `%s`", path);
      തിരികെ ശൂന്യ;
    }

  എങ്കിൽ (sb.buf == ശൂന്യ)
    sb.buf = xstrdup ("");
  തിരികെ sb.buf;
}

/* Build the OCI Process JSON from the CLI options.  */
സ്ഥിതം അക്ഷരം *
build_process_json (ഘടന crun_global_arguments *global_args arg_unused, അക്ഷരം **args, size_t args_len)
{
  ഘടന strbuf sb = { 0 };

  sb_putc (&sb, '{');

  sb_puts (&sb, "\"args\":");
  sb_put_json_string_array (&sb, args, args_len);

  /* cwd is a required field in the OCI process schema; the exec path treats a
     missing value as "/".  */
  sb_puts (&sb, ",\"cwd\":");
  sb_put_json_string (&sb, exec_options.cwd ? exec_options.cwd : "/");

  sb_puts (&sb, ",\"terminal\":");
  sb_puts (&sb, exec_options.tty ? "true" : "false");

  എങ്കിൽ (exec_options.env_size > 0)
    {
      sb_puts (&sb, ",\"env\":");
      sb_put_json_string_array (&sb, exec_options.env, exec_options.env_size);
    }

  എങ്കിൽ (exec_options.user)
    {
      അക്ഷരം *endptr = ശൂന്യ;
      അക്ഷരം *gidstr;
      പൂർണ്ണസംഖ്യ uid, gid = 0;
      അക്ഷരം tmp[32];

      uid = parse_id_or_fail (exec_options.user, &endptr, "UID");
      എങ്കിൽ (*endptr == ':')
        {
          gidstr = endptr + 1;
          gid = parse_id_or_fail (gidstr, &endptr, "GID");
          എങ്കിൽ (*endptr != '\0')
            libcrun_fail_with_error (0, "invalid USERSPEC specified");
        }
      അല്ലെങ്കിൽ എങ്കിൽ (*endptr != '\0')
        libcrun_fail_with_error (0, "invalid USERSPEC specified");

      sb_puts (&sb, ",\"user\":{\"uid\":");
      snprintf (tmp, വലുപ്പം (tmp), "%d", uid);
      sb_puts (&sb, tmp);
      sb_puts (&sb, ",\"gid\":");
      snprintf (tmp, വലുപ്പം (tmp), "%d", gid);
      sb_puts (&sb, tmp);
      sb_putc (&sb, '}');
    }

  എങ്കിൽ (exec_options.process_label != ശൂന്യ)
    {
      sb_puts (&sb, ",\"selinuxLabel\":");
      sb_put_json_string (&sb, exec_options.process_label);
    }

  എങ്കിൽ (exec_options.apparmor != ശൂന്യ)
    {
      sb_puts (&sb, ",\"apparmorProfile\":");
      sb_put_json_string (&sb, exec_options.apparmor);
    }

  എങ്കിൽ (exec_options.cap_size > 0)
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
  എങ്കിൽ (exec_options.no_new_privs)
    sb_puts (&sb, ",\"noNewPrivileges\":true");

  sb_putc (&sb, '}');

  തിരികെ sb.buf;
}

പൂർണ്ണസംഖ്യ
crun_command_exec (ഘടന crun_global_arguments *global_args, പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ first_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = ശൂന്യ;
  cleanup_free അക്ഷരം *process_json = ശൂന്യ;
  ഘടന libcrun_exec_options_s exec_opts;

  memset (&exec_opts, 0, വലുപ്പം (exec_opts));
  exec_opts.struct_size = വലുപ്പം (exec_opts);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &exec_options);
  crun_assert_n_args (argc - first_arg, exec_options.process ? 1 : 2, -1);

  crun_context = new_libcrun_context (global_args);

  libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds);

  എങ്കിൽ (getenv ("LISTEN_FDS"))
    {
      പൂർണ്ണസംഖ്യ listen_fds = strtoll (getenv ("LISTEN_FDS"), ശൂന്യ, 10);
      libcrun_context_set_listen_fds (crun_context, listen_fds);
      libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds + listen_fds);
    }

  /* The process file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  എങ്കിൽ (exec_options.process)
    {
      process_json = read_file_to_string (exec_options.process, err);
      എങ്കിൽ (process_json == ശൂന്യ)
        തിരികെ -1;
      exec_opts.merge_env = false;
    }
  അല്ലെങ്കിൽ
    {
      cleanup_free അക്ഷരം **args = ശൂന്യ;
      size_t args_len = argc - first_arg - 1;
      size_t i;

      /* argv[first_arg] is the container id, the command follows it.  */
      args = xmalloc0 ((args_len + 1) * വലുപ്പം (*args));
      വേണ്ടി (i = 0; i < args_len; i++)
        args[i] = argv[first_arg + i + 1];
      args[i] = ശൂന്യ;

      process_json = build_process_json (global_args, args, args_len);
      exec_opts.merge_env = true;
    }

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  libcrun_context_set_detach (crun_context, exec_options.detach);
  libcrun_context_set_console_socket (crun_context, exec_options.console_socket);
  libcrun_context_set_pid_file (crun_context, exec_options.pid_file);

  exec_opts.process_json = process_json;
  exec_opts.cgroup = exec_options.cgroup;

  തിരികെ libcrun_container_exec_with_options_json (crun_context, argv[first_arg], &exec_opts, err);
}
