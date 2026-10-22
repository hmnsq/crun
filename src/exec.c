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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <argp.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <limits.h>

#અન્તર્ભાવ "crun.h"

સ્થિર અક્ષર doc[] = "OCI runtime";

સંરચના exec_options_s
{
  bool tty;
  bool detach;
  bool no_new_privs;
  પૂર્ણાંક preserve_fds;
  અચલ અક્ષર *process;
  અચલ અક્ષર *console_socket;
  અચલ અક્ષર *pid_file;
  અક્ષર *process_label;
  અક્ષર *apparmor;
  અક્ષર *cwd;
  અક્ષર *user;
  અક્ષર **env;
  અક્ષર **cap;
  size_t cap_size;
  size_t env_size;
  અક્ષર *cgroup;
};

ગણના
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

સ્થિર સંરચના exec_options_s exec_options;

સ્થિર સંરચના argp_option options[]
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

સ્થિર અક્ષર args_doc[] = "exec CONTAINER cmd";

સ્થિર શૂન્ય
append_to_string_array (અક્ષર ***arr, size_t *size, અચલ અક્ષર *arg)
{
  *arr = xrealloc (*arr, (*size + 2) * કદ (**arr));
  (*arr)[*size + 1] = ખાલી;
  (*arr)[*size] = xstrdup (arg);
  (*size)++;
}

સ્થિર શૂન્ય
append_env (અચલ અક્ષર *arg)
{
  append_to_string_array (&exec_options.env, &exec_options.env_size, arg);
}

સ્થિર શૂન્ય
append_cap (અચલ અક્ષર *arg)
{
  append_to_string_array (&exec_options.cap, &exec_options.cap_size, arg);
}

સ્થિર error_t
parse_opt (પૂર્ણાંક ચાવી, અક્ષર *arg, સંરચના argp_state *state)
{
  પસંદગી (ચાવી)
    {
    સ્થિતિ OPTION_CONSOLE_SOCKET:
      exec_options.console_socket = arg;
      તોડો;

    સ્થિતિ OPTION_PID_FILE:
      exec_options.pid_file = arg;
      તોડો;

    સ્થિતિ OPTION_NO_NEW_PRIVS:
      exec_options.no_new_privs = true;
      તોડો;

    સ્થિતિ OPTION_PROCESS_LABEL:
      exec_options.process_label = argp_mandatory_argument (arg, state);
      તોડો;

    સ્થિતિ OPTION_APPARMOR:
      exec_options.apparmor = argp_mandatory_argument (arg, state);
      તોડો;

    સ્થિતિ OPTION_PRESERVE_FDS:
      exec_options.preserve_fds = parse_id_or_fail (argp_mandatory_argument (arg, state), ખાલી, "preserve-fds");
      તોડો;

    સ્થિતિ OPTION_CGROUP:
      exec_options.cgroup = argp_mandatory_argument (arg, state);
      તોડો;

    સ્થિતિ 'd':
      exec_options.detach = true;
      તોડો;

    સ્થિતિ 'p':
      exec_options.process = arg;
      તોડો;

    સ્થિતિ 't':
      exec_options.tty = arg == ખાલી || (strcmp (arg, "false") != 0 && strcmp (arg, "no") != 0);
      તોડો;

    સ્થિતિ 'u':
      exec_options.user = arg;
      તોડો;

    સ્થિતિ 'e':
      append_env (arg);
      તોડો;

    સ્થિતિ 'c':
      append_cap (arg);
      તોડો;

    સ્થિતિ OPTION_CWD:
      exec_options.cwd = xstrdup (arg);
      તોડો;

    સ્થિતિ ARGP_ચાવી_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    મૂળભૂત:
      પાછા ARGP_ERR_UNKNOWN;
    }

  પાછા 0;
}

સ્થિર સંરચના argp run_argp = { options, parse_opt, args_doc, doc, ખાલી, ખાલી, ખાલી };

/* Minimal growable string buffer used to assemble the OCI Process JSON.  The
   binary is a pure consumer of the public API, so it builds the process as a
   JSON string rather than reaching for the internal ocispec types.  */
સંરચના strbuf
{
  અક્ષર *buf;
  size_t len;
  size_t cap;
};

સ્થિર શૂન્ય
sb_ensure (સંરચના strbuf *sb, size_t extra)
{
  જો (sb->len + extra + 1 > sb->cap)
    {
      size_t ncap = sb->cap ? sb->cap : 256;
      જ્યારે (ncap < sb->len + extra + 1)
        ncap *= 2;
      sb->buf = xrealloc (sb->buf, ncap);
      sb->cap = ncap;
    }
}

સ્થિર શૂન્ય
sb_puts (સંરચના strbuf *sb, અચલ અક્ષર *s)
{
  size_t l = strlen (s);
  sb_ensure (sb, l);
  memcpy (sb->buf + sb->len, s, l);
  sb->len += l;
  sb->buf[sb->len] = '\0';
}

સ્થિર શૂન્ય
sb_putc (સંરચના strbuf *sb, અક્ષર c)
{
  sb_ensure (sb, 1);
  sb->buf[sb->len++] = c;
  sb->buf[sb->len] = '\0';
}

/* Append S as a quoted, escaped JSON string.  */
સ્થિર શૂન્ય
sb_put_json_string (સંરચના strbuf *sb, અચલ અક્ષર *s)
{
  sb_putc (sb, '"');
  માટે (; *s; s++)
    {
      unsigned અક્ષર c = (unsigned અક્ષર) *s;
      પસંદગી (c)
        {
        સ્થિતિ '"':
          sb_puts (sb, "\\\"");
          તોડો;
        સ્થિતિ '\\':
          sb_puts (sb, "\\\\");
          તોડો;
        સ્થિતિ '\b':
          sb_puts (sb, "\\b");
          તોડો;
        સ્થિતિ '\f':
          sb_puts (sb, "\\f");
          તોડો;
        સ્થિતિ '\n':
          sb_puts (sb, "\\n");
          તોડો;
        સ્થિતિ '\r':
          sb_puts (sb, "\\r");
          તોડો;
        સ્થિતિ '\t':
          sb_puts (sb, "\\t");
          તોડો;
        મૂળભૂત:
          જો (c < 0x20)
            {
              અક્ષર tmp[8];
              snprintf (tmp, કદ (tmp), "\\u%04x", c);
              sb_puts (sb, tmp);
            }
          નહીંતર
            sb_putc (sb, c);
        }
    }
  sb_putc (sb, '"');
}

સ્થિર શૂન્ય
sb_put_json_string_array (સંરચના strbuf *sb, અક્ષર **arr, size_t len)
{
  size_t i;
  sb_putc (sb, '[');
  માટે (i = 0; i < len; i++)
    {
      જો (i)
        sb_putc (sb, ',');
      sb_put_json_string (sb, arr[i]);
    }
  sb_putc (sb, ']');
}

/* Read the whole file at PATH into a NUL-terminated string.  A PATH of `-`
   reads from stdin.  */
સ્થિર અક્ષર *
read_file_to_string (અચલ અક્ષર *path, libcrun_error_t *err)
{
  cleanup_file FILE *f = ખાલી;
  સંરચના strbuf sb = { 0 };
  અક્ષર chunk[4096];
  size_t n;

  જો (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  f = fopen (path, "re");
  જો (f == ખાલી)
    {
      libcrun_make_error (err, errno, "cannot open `%s`", path);
      પાછા ખાલી;
    }

  જ્યારે ((n = fread (chunk, 1, કદ (chunk), f)) > 0)
    {
      sb_ensure (&sb, n);
      memcpy (sb.buf + sb.len, chunk, n);
      sb.len += n;
      sb.buf[sb.len] = '\0';
    }
  જો (ferror (f))
    {
      મુક્ત (sb.buf);
      libcrun_make_error (err, errno, "cannot read `%s`", path);
      પાછા ખાલી;
    }

  જો (sb.buf == ખાલી)
    sb.buf = xstrdup ("");
  પાછા sb.buf;
}

/* Build the OCI Process JSON from the CLI options.  */
સ્થિર અક્ષર *
build_process_json (સંરચના crun_global_arguments *global_args arg_unused, અક્ષર **args, size_t args_len)
{
  સંરચના strbuf sb = { 0 };

  sb_putc (&sb, '{');

  sb_puts (&sb, "\"args\":");
  sb_put_json_string_array (&sb, args, args_len);

  /* cwd is a required field in the OCI process schema; the exec path treats a
     missing value as "/".  */
  sb_puts (&sb, ",\"cwd\":");
  sb_put_json_string (&sb, exec_options.cwd ? exec_options.cwd : "/");

  sb_puts (&sb, ",\"terminal\":");
  sb_puts (&sb, exec_options.tty ? "true" : "false");

  જો (exec_options.env_size > 0)
    {
      sb_puts (&sb, ",\"env\":");
      sb_put_json_string_array (&sb, exec_options.env, exec_options.env_size);
    }

  જો (exec_options.user)
    {
      અક્ષર *endptr = ખાલી;
      અક્ષર *gidstr;
      પૂર્ણાંક uid, gid = 0;
      અક્ષર tmp[32];

      uid = parse_id_or_fail (exec_options.user, &endptr, "UID");
      જો (*endptr == ':')
        {
          gidstr = endptr + 1;
          gid = parse_id_or_fail (gidstr, &endptr, "GID");
          જો (*endptr != '\0')
            libcrun_fail_with_error (0, "invalid USERSPEC specified");
        }
      નહીંતર જો (*endptr != '\0')
        libcrun_fail_with_error (0, "invalid USERSPEC specified");

      sb_puts (&sb, ",\"user\":{\"uid\":");
      snprintf (tmp, કદ (tmp), "%d", uid);
      sb_puts (&sb, tmp);
      sb_puts (&sb, ",\"gid\":");
      snprintf (tmp, કદ (tmp), "%d", gid);
      sb_puts (&sb, tmp);
      sb_putc (&sb, '}');
    }

  જો (exec_options.process_label != ખાલી)
    {
      sb_puts (&sb, ",\"selinuxLabel\":");
      sb_put_json_string (&sb, exec_options.process_label);
    }

  જો (exec_options.apparmor != ખાલી)
    {
      sb_puts (&sb, ",\"apparmorProfile\":");
      sb_put_json_string (&sb, exec_options.apparmor);
    }

  જો (exec_options.cap_size > 0)
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
  જો (exec_options.no_new_privs)
    sb_puts (&sb, ",\"noNewPrivileges\":true");

  sb_putc (&sb, '}');

  પાછા sb.buf;
}

પૂર્ણાંક
crun_command_exec (સંરચના crun_global_arguments *global_args, પૂર્ણાંક argc, અક્ષર **argv, libcrun_error_t *err)
{
  પૂર્ણાંક first_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = ખાલી;
  cleanup_free અક્ષર *process_json = ખાલી;
  સંરચના libcrun_exec_options_s exec_opts;

  memset (&exec_opts, 0, કદ (exec_opts));
  exec_opts.struct_size = કદ (exec_opts);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &exec_options);
  crun_assert_n_args (argc - first_arg, exec_options.process ? 1 : 2, -1);

  crun_context = new_libcrun_context (global_args);

  libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds);

  જો (getenv ("LISTEN_FDS"))
    {
      પૂર્ણાંક listen_fds = strtoll (getenv ("LISTEN_FDS"), ખાલી, 10);
      libcrun_context_set_listen_fds (crun_context, listen_fds);
      libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds + listen_fds);
    }

  /* The process file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  જો (exec_options.process)
    {
      process_json = read_file_to_string (exec_options.process, err);
      જો (process_json == ખાલી)
        પાછા -1;
      exec_opts.merge_env = false;
    }
  નહીંતર
    {
      cleanup_free અક્ષર **args = ખાલી;
      size_t args_len = argc - first_arg - 1;
      size_t i;

      /* argv[first_arg] is the container id, the command follows it.  */
      args = xmalloc0 ((args_len + 1) * કદ (*args));
      માટે (i = 0; i < args_len; i++)
        args[i] = argv[first_arg + i + 1];
      args[i] = ખાલી;

      process_json = build_process_json (global_args, args, args_len);
      exec_opts.merge_env = true;
    }

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  libcrun_context_set_detach (crun_context, exec_options.detach);
  libcrun_context_set_console_socket (crun_context, exec_options.console_socket);
  libcrun_context_set_pid_file (crun_context, exec_options.pid_file);

  exec_opts.process_json = process_json;
  exec_opts.cgroup = exec_options.cgroup;

  પાછા libcrun_container_exec_with_options_json (crun_context, argv[first_arg], &exec_opts, err);
}
