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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <argp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "crun.h"

ਸਥਿਰ ਅੱਖਰ doc[] = "OCI runtime";

ਬਣਤਰ exec_options_s
{
  bool tty;
  bool detach;
  bool no_new_privs;
  ਪੂਰਨ preserve_fds;
  ਅਚਲ ਅੱਖਰ *process;
  ਅਚਲ ਅੱਖਰ *console_socket;
  ਅਚਲ ਅੱਖਰ *pid_file;
  ਅੱਖਰ *process_label;
  ਅੱਖਰ *apparmor;
  ਅੱਖਰ *cwd;
  ਅੱਖਰ *user;
  ਅੱਖਰ **env;
  ਅੱਖਰ **cap;
  size_t cap_ਮਾਪ;
  size_t env_ਮਾਪ;
  ਅੱਖਰ *cgroup;
};

ਗਿਣਤੀ
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

ਸਥਿਰ ਬਣਤਰ exec_options_s exec_options;

ਸਥਿਰ ਬਣਤਰ argp_option options[]
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

ਸਥਿਰ ਅੱਖਰ ਦਲੀਲਾਂ_doc[] = "exec CONTAINER cmd";

ਸਥਿਰ ਖਾਲੀ
append_ਮੰਜ਼ਿਲ_string_array (ਅੱਖਰ ***arr, size_t *ਮਾਪ, ਅਚਲ ਅੱਖਰ *arg)
{
  *arr = xrealloc (*arr, (*ਮਾਪ + 2) * ਆਕਾਰ (**arr));
  (*arr)[*ਮਾਪ + 1] = ਨਿਰਾ;
  (*arr)[*ਮਾਪ] = xstrdup (arg);
  (*ਮਾਪ)++;
}

ਸਥਿਰ ਖਾਲੀ
append_env (ਅਚਲ ਅੱਖਰ *arg)
{
  append_ਮੰਜ਼ਿਲ_string_array (&exec_options.env, &exec_options.env_ਮਾਪ, arg);
}

ਸਥਿਰ ਖਾਲੀ
append_cap (ਅਚਲ ਅੱਖਰ *arg)
{
  append_ਮੰਜ਼ਿਲ_string_array (&exec_options.cap, &exec_options.cap_ਮਾਪ, arg);
}

ਸਥਿਰ error_t
parse_opt (ਪੂਰਨ ਕੁੰਜੀ, ਅੱਖਰ *arg, ਬਣਤਰ argp_state *state)
{
  ਚੋਣ (ਕੁੰਜੀ)
    {
    ਹਾਲਤ OPTION_CONSOLE_SOCKET:
      exec_options.console_socket = arg;
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_PID_FILE:
      exec_options.pid_file = arg;
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_NO_NEW_PRIVS:
      exec_options.no_new_privs = true;
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_PROCESS_LABEL:
      exec_options.process_label = argp_mandatory_ਦਲੀਲ (arg, state);
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_APPARMOR:
      exec_options.apparmor = argp_mandatory_ਦਲੀਲ (arg, state);
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_PRESERVE_FDS:
      exec_options.preserve_fds = parse_id_or_fail (argp_mandatory_ਦਲੀਲ (arg, state), ਨਿਰਾ, "preserve-fds");
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_CGROUP:
      exec_options.cgroup = argp_mandatory_ਦਲੀਲ (arg, state);
      ਰੋਕੋ;

    ਹਾਲਤ 'd':
      exec_options.detach = true;
      ਰੋਕੋ;

    ਹਾਲਤ 'p':
      exec_options.process = arg;
      ਰੋਕੋ;

    ਹਾਲਤ 't':
      exec_options.tty = arg == ਨਿਰਾ || (strcmp (arg, "false") != 0 && strcmp (arg, "no") != 0);
      ਰੋਕੋ;

    ਹਾਲਤ 'u':
      exec_options.user = arg;
      ਰੋਕੋ;

    ਹਾਲਤ 'e':
      append_env (arg);
      ਰੋਕੋ;

    ਹਾਲਤ 'c':
      append_cap (arg);
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_CWD:
      exec_options.cwd = xstrdup (arg);
      ਰੋਕੋ;

    ਹਾਲਤ ARGP_ਕੁੰਜੀ_NO_ਦਲੀਲਾਂ:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ਮੂਲ:
      ਵਾਪਸ ARGP_ERR_UNKNOWN;
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਣਤਰ argp ਲੜੀਵਾਰ_argp = { options, parse_opt, ਦਲੀਲਾਂ_doc, doc, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ };

/* Minimal growable string buffer used to assemble the OCI Process JSON.  The
   binary is a pure consumer of the public API, so it builds the process as a
   JSON string rather than reaching for the internal ocispec types.  */
ਬਣਤਰ strbuf
{
  ਅੱਖਰ *buf;
  size_t len;
  size_t cap;
};

ਸਥਿਰ ਖਾਲੀ
sb_ensure (ਬਣਤਰ strbuf *sb, size_t extra)
{
  ਜੇ (sb->len + extra + 1 > sb->cap)
    {
      size_t ncap = sb->cap ? sb->cap : 256;
      ਜਦੋਂ (ncap < sb->len + extra + 1)
        ncap *= 2;
      sb->buf = xrealloc (sb->buf, ncap);
      sb->cap = ncap;
    }
}

ਸਥਿਰ ਖਾਲੀ
sb_puts (ਬਣਤਰ strbuf *sb, ਅਚਲ ਅੱਖਰ *s)
{
  size_t l = ਲੜੀ_ਲੰਬਾਈ (s);
  sb_ensure (sb, l);
  ਸਮ੍ਰਿਤੀ_ਨਕਲ (sb->buf + sb->len, s, l);
  sb->len += l;
  sb->buf[sb->len] = '\0';
}

ਸਥਿਰ ਖਾਲੀ
sb_putc (ਬਣਤਰ strbuf *sb, ਅੱਖਰ c)
{
  sb_ensure (sb, 1);
  sb->buf[sb->len++] = c;
  sb->buf[sb->len] = '\0';
}

/* Append S as a quoted, escaped JSON string.  */
ਸਥਿਰ ਖਾਲੀ
sb_put_json_string (ਬਣਤਰ strbuf *sb, ਅਚਲ ਅੱਖਰ *s)
{
  sb_putc (sb, '"');
  ਲਈ (; *s; s++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) *s;
      ਚੋਣ (c)
        {
        ਹਾਲਤ '"':
          sb_puts (sb, "\\\"");
          ਰੋਕੋ;
        ਹਾਲਤ '\\':
          sb_puts (sb, "\\\\");
          ਰੋਕੋ;
        ਹਾਲਤ '\b':
          sb_puts (sb, "\\b");
          ਰੋਕੋ;
        ਹਾਲਤ '\f':
          sb_puts (sb, "\\f");
          ਰੋਕੋ;
        ਹਾਲਤ '\n':
          sb_puts (sb, "\\n");
          ਰੋਕੋ;
        ਹਾਲਤ '\r':
          sb_puts (sb, "\\r");
          ਰੋਕੋ;
        ਹਾਲਤ '\t':
          sb_puts (sb, "\\t");
          ਰੋਕੋ;
        ਮੂਲ:
          ਜੇ (c < 0x20)
            {
              ਅੱਖਰ tmp[8];
              snprintf (tmp, ਆਕਾਰ (tmp), "\\u%04x", c);
              sb_puts (sb, tmp);
            }
          ਨਹੀਂਤਾਂ
            sb_putc (sb, c);
        }
    }
  sb_putc (sb, '"');
}

ਸਥਿਰ ਖਾਲੀ
sb_put_json_string_array (ਬਣਤਰ strbuf *sb, ਅੱਖਰ **arr, size_t len)
{
  size_t i;
  sb_putc (sb, '[');
  ਲਈ (i = 0; i < len; i++)
    {
      ਜੇ (i)
        sb_putc (sb, ',');
      sb_put_json_string (sb, arr[i]);
    }
  sb_putc (sb, ']');
}

/* Read the whole file at PATH into a NUL-terminated string.  A PATH of `-`
   reads from stdin.  */
ਸਥਿਰ ਅੱਖਰ *
read_file_ਮੰਜ਼ਿਲ_string (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err)
{
  cleanup_file FILE *f = ਨਿਰਾ;
  ਬਣਤਰ strbuf sb = { 0 };
  ਅੱਖਰ chunk[4096];
  size_t n;

  ਜੇ (strcmp (ਰਸਤਾ, "-") == 0)
    ਰਸਤਾ = "/dev/stdin";

  f = fopen (ਰਸਤਾ, "re");
  ਜੇ (f == ਨਿਰਾ)
    {
      libcrun_make_error (err, errno, "cannot open `%s`", ਰਸਤਾ);
      ਵਾਪਸ ਨਿਰਾ;
    }

  ਜਦੋਂ ((n = fread (chunk, 1, ਆਕਾਰ (chunk), f)) > 0)
    {
      sb_ensure (&sb, n);
      ਸਮ੍ਰਿਤੀ_ਨਕਲ (sb.buf + sb.len, chunk, n);
      sb.len += n;
      sb.buf[sb.len] = '\0';
    }
  ਜੇ (ferror (f))
    {
      ਮੁਕਤ (sb.buf);
      libcrun_make_error (err, errno, "cannot read `%s`", ਰਸਤਾ);
      ਵਾਪਸ ਨਿਰਾ;
    }

  ਜੇ (sb.buf == ਨਿਰਾ)
    sb.buf = xstrdup ("");
  ਵਾਪਸ sb.buf;
}

/* Build the OCI Process JSON from the CLI options.  */
ਸਥਿਰ ਅੱਖਰ *
build_process_json (ਬਣਤਰ crun_global_arguments *global_ਦਲੀਲਾਂ arg_ਅਣਵਰਤਿਆ, ਅੱਖਰ **ਦਲੀਲਾਂ, size_t ਦਲੀਲਾਂ_len)
{
  ਬਣਤਰ strbuf sb = { 0 };

  sb_putc (&sb, '{');

  sb_puts (&sb, "\"args\":");
  sb_put_json_string_array (&sb, ਦਲੀਲਾਂ, ਦਲੀਲਾਂ_len);

  /* cwd is a required field in the OCI process schema; the exec path treats a
     missing value as "/".  */
  sb_puts (&sb, ",\"cwd\":");
  sb_put_json_string (&sb, exec_options.cwd ? exec_options.cwd : "/");

  sb_puts (&sb, ",\"terminal\":");
  sb_puts (&sb, exec_options.tty ? "true" : "false");

  ਜੇ (exec_options.env_ਮਾਪ > 0)
    {
      sb_puts (&sb, ",\"env\":");
      sb_put_json_string_array (&sb, exec_options.env, exec_options.env_ਮਾਪ);
    }

  ਜੇ (exec_options.user)
    {
      ਅੱਖਰ *endptr = ਨਿਰਾ;
      ਅੱਖਰ *gidstr;
      ਪੂਰਨ uid, gid = 0;
      ਅੱਖਰ tmp[32];

      uid = parse_id_or_fail (exec_options.user, &endptr, "UID");
      ਜੇ (*endptr == ':')
        {
          gidstr = endptr + 1;
          gid = parse_id_or_fail (gidstr, &endptr, "GID");
          ਜੇ (*endptr != '\0')
            libcrun_fail_with_error (0, "invalid USERSPEC specified");
        }
      ਨਹੀਂਤਾਂ ਜੇ (*endptr != '\0')
        libcrun_fail_with_error (0, "invalid USERSPEC specified");

      sb_puts (&sb, ",\"user\":{\"uid\":");
      snprintf (tmp, ਆਕਾਰ (tmp), "%d", uid);
      sb_puts (&sb, tmp);
      sb_puts (&sb, ",\"gid\":");
      snprintf (tmp, ਆਕਾਰ (tmp), "%d", gid);
      sb_puts (&sb, tmp);
      sb_putc (&sb, '}');
    }

  ਜੇ (exec_options.process_label != ਨਿਰਾ)
    {
      sb_puts (&sb, ",\"selinuxLabel\":");
      sb_put_json_string (&sb, exec_options.process_label);
    }

  ਜੇ (exec_options.apparmor != ਨਿਰਾ)
    {
      sb_puts (&sb, ",\"apparmorProfile\":");
      sb_put_json_string (&sb, exec_options.apparmor);
    }

  ਜੇ (exec_options.cap_ਮਾਪ > 0)
    {
      sb_puts (&sb, ",\"capabilities\":{");
      sb_puts (&sb, "\"effective\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_ਮਾਪ);
      sb_puts (&sb, ",\"bounding\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_ਮਾਪ);
      sb_puts (&sb, ",\"ambient\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_ਮਾਪ);
      sb_puts (&sb, ",\"permitted\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_ਮਾਪ);
      sb_putc (&sb, '}');
    }

  /* noNewPrivileges remains at the config default unless explicitly requested.  */
  ਜੇ (exec_options.no_new_privs)
    sb_puts (&sb, ",\"noNewPrivileges\":true");

  sb_putc (&sb, '}');

  ਵਾਪਸ sb.buf;
}

ਪੂਰਨ
crun_command_exec (ਬਣਤਰ crun_global_arguments *global_ਦਲੀਲਾਂ, ਪੂਰਨ argc, ਅੱਖਰ **argv, libcrun_error_t *err)
{
  ਪੂਰਨ ਪਹਿਲਾ_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *process_json = ਨਿਰਾ;
  ਬਣਤਰ libcrun_exec_options_s exec_opts;

  memset (&exec_opts, 0, ਆਕਾਰ (exec_opts));
  exec_opts.struct_ਮਾਪ = ਆਕਾਰ (exec_opts);

  argp_parse (&ਲੜੀਵਾਰ_argp, argc, argv, ARGP_ਵਿੱਚ_ਕ੍ਰਮ, &ਪਹਿਲਾ_arg, &exec_options);
  crun_assert_n_ਦਲੀਲਾਂ (argc - ਪਹਿਲਾ_arg, exec_options.process ? 1 : 2, -1);

  crun_context = new_libcrun_context (global_ਦਲੀਲਾਂ);

  libcrun_context_ਸੈੱਟ_preserve_fds (crun_context, exec_options.preserve_fds);

  ਜੇ (getenv ("LISTEN_FDS"))
    {
      ਪੂਰਨ listen_fds = strtoll (getenv ("LISTEN_FDS"), ਨਿਰਾ, 10);
      libcrun_context_ਸੈੱਟ_listen_fds (crun_context, listen_fds);
      libcrun_context_ਸੈੱਟ_preserve_fds (crun_context, exec_options.preserve_fds + listen_fds);
    }

  /* The process file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  ਜੇ (exec_options.process)
    {
      process_json = read_file_ਮੰਜ਼ਿਲ_string (exec_options.process, err);
      ਜੇ (process_json == ਨਿਰਾ)
        ਵਾਪਸ -1;
      exec_opts.ਮਿਲਾਓ_env = false;
    }
  ਨਹੀਂਤਾਂ
    {
      cleanup_free ਅੱਖਰ **ਦਲੀਲਾਂ = ਨਿਰਾ;
      size_t ਦਲੀਲਾਂ_len = argc - ਪਹਿਲਾ_arg - 1;
      size_t i;

      /* argv[first_arg] is the container id, the command follows it.  */
      ਦਲੀਲਾਂ = xmalloc0 ((ਦਲੀਲਾਂ_len + 1) * ਆਕਾਰ (*ਦਲੀਲਾਂ));
      ਲਈ (i = 0; i < ਦਲੀਲਾਂ_len; i++)
        ਦਲੀਲਾਂ[i] = argv[ਪਹਿਲਾ_arg + i + 1];
      ਦਲੀਲਾਂ[i] = ਨਿਰਾ;

      process_json = build_process_json (global_ਦਲੀਲਾਂ, ਦਲੀਲਾਂ, ਦਲੀਲਾਂ_len);
      exec_opts.ਮਿਲਾਓ_env = true;
    }

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = init_libcrun_context (crun_context, argv[ਪਹਿਲਾ_arg], global_ਦਲੀਲਾਂ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  libcrun_context_ਸੈੱਟ_detach (crun_context, exec_options.detach);
  libcrun_context_ਸੈੱਟ_console_socket (crun_context, exec_options.console_socket);
  libcrun_context_ਸੈੱਟ_pid_file (crun_context, exec_options.pid_file);

  exec_opts.process_json = process_json;
  exec_opts.cgroup = exec_options.cgroup;

  ਵਾਪਸ libcrun_container_exec_with_options_json (crun_context, argv[ਪਹਿਲਾ_arg], &exec_opts, err);
}
