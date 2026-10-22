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

#شامل <config.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <argp.h>
#شامل <string.h>
#شامل <unistd.h>
#شامل <errno.h>
#شامل <limits.h>

#شامل "crun.h"

ساکن حرف doc[] = "OCI runtime";

ساخت exec_options_s
{
  bool tty;
  bool detach;
  bool no_new_privs;
  صحیح preserve_fds;
  ثابت حرف *process;
  ثابت حرف *console_socket;
  ثابت حرف *pid_file;
  حرف *process_label;
  حرف *apparmor;
  حرف *cwd;
  حرف *user;
  حرف **env;
  حرف **cap;
  size_t cap_سائز;
  size_t env_سائز;
  حرف *cgroup;
};

شمار
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

ساکن ساخت exec_options_s exec_options;

ساکن ساخت argp_option options[]
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

ساکن حرف دلائل_doc[] = "exec CONTAINER cmd";

ساکن خالی
append_مقصد_string_array (حرف ***arr, size_t *سائز, ثابت حرف *arg)
{
  *arr = xrealloc (*arr, (*سائز + 2) * حجم (**arr));
  (*arr)[*سائز + 1] = تہی;
  (*arr)[*سائز] = xstrdup (arg);
  (*سائز)++;
}

ساکن خالی
append_env (ثابت حرف *arg)
{
  append_مقصد_string_array (&exec_options.env, &exec_options.env_سائز, arg);
}

ساکن خالی
append_cap (ثابت حرف *arg)
{
  append_مقصد_string_array (&exec_options.cap, &exec_options.cap_سائز, arg);
}

ساکن error_t
parse_opt (صحیح کلید, حرف *arg, ساخت argp_state *state)
{
  انتخاب (کلید)
    {
    حالت OPTION_CONSOLE_SOCKET:
      exec_options.console_socket = arg;
      توڑو;

    حالت OPTION_PID_FILE:
      exec_options.pid_file = arg;
      توڑو;

    حالت OPTION_NO_NEW_PRIVS:
      exec_options.no_new_privs = true;
      توڑو;

    حالت OPTION_PROCESS_LABEL:
      exec_options.process_label = argp_mandatory_دلیل (arg, state);
      توڑو;

    حالت OPTION_APPARMOR:
      exec_options.apparmor = argp_mandatory_دلیل (arg, state);
      توڑو;

    حالت OPTION_PRESERVE_FDS:
      exec_options.preserve_fds = parse_id_or_fail (argp_mandatory_دلیل (arg, state), تہی, "preserve-fds");
      توڑو;

    حالت OPTION_CGROUP:
      exec_options.cgroup = argp_mandatory_دلیل (arg, state);
      توڑو;

    حالت 'd':
      exec_options.detach = true;
      توڑو;

    حالت 'p':
      exec_options.process = arg;
      توڑو;

    حالت 't':
      exec_options.tty = arg == تہی || (strcmp (arg, "false") != 0 && strcmp (arg, "no") != 0);
      توڑو;

    حالت 'u':
      exec_options.user = arg;
      توڑو;

    حالت 'e':
      append_env (arg);
      توڑو;

    حالت 'c':
      append_cap (arg);
      توڑو;

    حالت OPTION_CWD:
      exec_options.cwd = xstrdup (arg);
      توڑو;

    حالت ARGP_کلید_NO_دلائل:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    طےشدہ:
      واپس ARGP_ERR_UNKNOWN;
    }

  واپس 0;
}

ساکن ساخت argp تسلسل_argp = { options, parse_opt, دلائل_doc, doc, تہی, تہی, تہی };

/* Minimal growable string buffer used to assemble the OCI Process JSON.  The
   binary is a pure consumer of the public API, so it builds the process as a
   JSON string rather than reaching for the internal ocispec types.  */
ساخت strbuf
{
  حرف *buf;
  size_t len;
  size_t cap;
};

ساکن خالی
sb_ensure (ساخت strbuf *sb, size_t extra)
{
  اگر (sb->len + extra + 1 > sb->cap)
    {
      size_t ncap = sb->cap ? sb->cap : 256;
      جب (ncap < sb->len + extra + 1)
        ncap *= 2;
      sb->buf = xrealloc (sb->buf, ncap);
      sb->cap = ncap;
    }
}

ساکن خالی
sb_puts (ساخت strbuf *sb, ثابت حرف *s)
{
  size_t l = لڑی_لمبائی (s);
  sb_ensure (sb, l);
  حافظہ_نقل (sb->buf + sb->len, s, l);
  sb->len += l;
  sb->buf[sb->len] = '\0';
}

ساکن خالی
sb_putc (ساخت strbuf *sb, حرف c)
{
  sb_ensure (sb, 1);
  sb->buf[sb->len++] = c;
  sb->buf[sb->len] = '\0';
}

/* Append S as a quoted, escaped JSON string.  */
ساکن خالی
sb_put_json_string (ساخت strbuf *sb, ثابت حرف *s)
{
  sb_putc (sb, '"');
  برائے (; *s; s++)
    {
      غیرعلامتی حرف c = (غیرعلامتی حرف) *s;
      انتخاب (c)
        {
        حالت '"':
          sb_puts (sb, "\\\"");
          توڑو;
        حالت '\\':
          sb_puts (sb, "\\\\");
          توڑو;
        حالت '\b':
          sb_puts (sb, "\\b");
          توڑو;
        حالت '\f':
          sb_puts (sb, "\\f");
          توڑو;
        حالت '\n':
          sb_puts (sb, "\\n");
          توڑو;
        حالت '\r':
          sb_puts (sb, "\\r");
          توڑو;
        حالت '\t':
          sb_puts (sb, "\\t");
          توڑو;
        طےشدہ:
          اگر (c < 0x20)
            {
              حرف tmp[8];
              snprintf (tmp, حجم (tmp), "\\u%04x", c);
              sb_puts (sb, tmp);
            }
          ورنہ
            sb_putc (sb, c);
        }
    }
  sb_putc (sb, '"');
}

ساکن خالی
sb_put_json_string_array (ساخت strbuf *sb, حرف **arr, size_t len)
{
  size_t i;
  sb_putc (sb, '[');
  برائے (i = 0; i < len; i++)
    {
      اگر (i)
        sb_putc (sb, ',');
      sb_put_json_string (sb, arr[i]);
    }
  sb_putc (sb, ']');
}

/* Read the whole file at PATH into a NUL-terminated string.  A PATH of `-`
   reads from stdin.  */
ساکن حرف *
read_file_مقصد_string (ثابت حرف *راستہ, libcrun_error_t *err)
{
  cleanup_file FILE *f = تہی;
  ساخت strbuf sb = { 0 };
  حرف chunk[4096];
  size_t n;

  اگر (strcmp (راستہ, "-") == 0)
    راستہ = "/dev/stdin";

  f = fopen (راستہ, "re");
  اگر (f == تہی)
    {
      libcrun_make_error (err, errno, "cannot open `%s`", راستہ);
      واپس تہی;
    }

  جب ((n = fread (chunk, 1, حجم (chunk), f)) > 0)
    {
      sb_ensure (&sb, n);
      حافظہ_نقل (sb.buf + sb.len, chunk, n);
      sb.len += n;
      sb.buf[sb.len] = '\0';
    }
  اگر (ferror (f))
    {
      آزاد (sb.buf);
      libcrun_make_error (err, errno, "cannot read `%s`", راستہ);
      واپس تہی;
    }

  اگر (sb.buf == تہی)
    sb.buf = xstrdup ("");
  واپس sb.buf;
}

/* Build the OCI Process JSON from the CLI options.  */
ساکن حرف *
build_process_json (ساخت crun_global_arguments *global_دلائل arg_غیر_مستعمل, حرف **دلائل, size_t دلائل_len)
{
  ساخت strbuf sb = { 0 };

  sb_putc (&sb, '{');

  sb_puts (&sb, "\"args\":");
  sb_put_json_string_array (&sb, دلائل, دلائل_len);

  /* cwd is a required field in the OCI process schema; the exec path treats a
     missing value as "/".  */
  sb_puts (&sb, ",\"cwd\":");
  sb_put_json_string (&sb, exec_options.cwd ? exec_options.cwd : "/");

  sb_puts (&sb, ",\"terminal\":");
  sb_puts (&sb, exec_options.tty ? "true" : "false");

  اگر (exec_options.env_سائز > 0)
    {
      sb_puts (&sb, ",\"env\":");
      sb_put_json_string_array (&sb, exec_options.env, exec_options.env_سائز);
    }

  اگر (exec_options.user)
    {
      حرف *endptr = تہی;
      حرف *gidstr;
      صحیح uid, gid = 0;
      حرف tmp[32];

      uid = parse_id_or_fail (exec_options.user, &endptr, "UID");
      اگر (*endptr == ':')
        {
          gidstr = endptr + 1;
          gid = parse_id_or_fail (gidstr, &endptr, "GID");
          اگر (*endptr != '\0')
            libcrun_fail_with_error (0, "invalid USERSPEC specified");
        }
      ورنہ اگر (*endptr != '\0')
        libcrun_fail_with_error (0, "invalid USERSPEC specified");

      sb_puts (&sb, ",\"user\":{\"uid\":");
      snprintf (tmp, حجم (tmp), "%d", uid);
      sb_puts (&sb, tmp);
      sb_puts (&sb, ",\"gid\":");
      snprintf (tmp, حجم (tmp), "%d", gid);
      sb_puts (&sb, tmp);
      sb_putc (&sb, '}');
    }

  اگر (exec_options.process_label != تہی)
    {
      sb_puts (&sb, ",\"selinuxLabel\":");
      sb_put_json_string (&sb, exec_options.process_label);
    }

  اگر (exec_options.apparmor != تہی)
    {
      sb_puts (&sb, ",\"apparmorProfile\":");
      sb_put_json_string (&sb, exec_options.apparmor);
    }

  اگر (exec_options.cap_سائز > 0)
    {
      sb_puts (&sb, ",\"capabilities\":{");
      sb_puts (&sb, "\"effective\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_سائز);
      sb_puts (&sb, ",\"bounding\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_سائز);
      sb_puts (&sb, ",\"ambient\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_سائز);
      sb_puts (&sb, ",\"permitted\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_سائز);
      sb_putc (&sb, '}');
    }

  /* noNewPrivileges remains at the config default unless explicitly requested.  */
  اگر (exec_options.no_new_privs)
    sb_puts (&sb, ",\"noNewPrivileges\":true");

  sb_putc (&sb, '}');

  واپس sb.buf;
}

صحیح
crun_command_exec (ساخت crun_global_arguments *global_دلائل, صحیح argc, حرف **argv, libcrun_error_t *err)
{
  صحیح پہلا_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = تہی;
  cleanup_free حرف *process_json = تہی;
  ساخت libcrun_exec_options_s exec_opts;

  memset (&exec_opts, 0, حجم (exec_opts));
  exec_opts.struct_سائز = حجم (exec_opts);

  argp_parse (&تسلسل_argp, argc, argv, ARGP_اندر_ترتیب, &پہلا_arg, &exec_options);
  crun_assert_n_دلائل (argc - پہلا_arg, exec_options.process ? 1 : 2, -1);

  crun_context = new_libcrun_context (global_دلائل);

  libcrun_context_سیٹ_preserve_fds (crun_context, exec_options.preserve_fds);

  اگر (getenv ("LISTEN_FDS"))
    {
      صحیح listen_fds = strtoll (getenv ("LISTEN_FDS"), تہی, 10);
      libcrun_context_سیٹ_listen_fds (crun_context, listen_fds);
      libcrun_context_سیٹ_preserve_fds (crun_context, exec_options.preserve_fds + listen_fds);
    }

  /* The process file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  اگر (exec_options.process)
    {
      process_json = read_file_مقصد_string (exec_options.process, err);
      اگر (process_json == تہی)
        واپس -1;
      exec_opts.ملاؤ_env = false;
    }
  ورنہ
    {
      cleanup_free حرف **دلائل = تہی;
      size_t دلائل_len = argc - پہلا_arg - 1;
      size_t i;

      /* argv[first_arg] is the container id, the command follows it.  */
      دلائل = xmalloc0 ((دلائل_len + 1) * حجم (*دلائل));
      برائے (i = 0; i < دلائل_len; i++)
        دلائل[i] = argv[پہلا_arg + i + 1];
      دلائل[i] = تہی;

      process_json = build_process_json (global_دلائل, دلائل, دلائل_len);
      exec_opts.ملاؤ_env = true;
    }

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = init_libcrun_context (crun_context, argv[پہلا_arg], global_دلائل, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  libcrun_context_سیٹ_detach (crun_context, exec_options.detach);
  libcrun_context_سیٹ_console_socket (crun_context, exec_options.console_socket);
  libcrun_context_سیٹ_pid_file (crun_context, exec_options.pid_file);

  exec_opts.process_json = process_json;
  exec_opts.cgroup = exec_options.cgroup;

  واپس libcrun_container_exec_with_options_json (crun_context, argv[پہلا_arg], &exec_opts, err);
}
