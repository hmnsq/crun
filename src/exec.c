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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <argp.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <limits.h>

#অন্তর্ভাব "crun.h"

স্থির বর্ণ doc[] = "OCI runtime";

সংরচনা exec_options_s
{
  bool tty;
  bool detach;
  bool no_new_privs;
  পূর্ণাঙ্ক preserve_fds;
  ধ্রুব বর্ণ *process;
  ধ্রুব বর্ণ *console_socket;
  ধ্রুব বর্ণ *pid_file;
  বর্ণ *process_label;
  বর্ণ *apparmor;
  বর্ণ *cwd;
  বর্ণ *user;
  বর্ণ **env;
  বর্ণ **cap;
  size_t cap_size;
  size_t env_size;
  বর্ণ *cgroup;
};

গণনা
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

স্থির সংরচনা exec_options_s exec_options;

স্থির সংরচনা argp_option options[]
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

স্থির বর্ণ args_doc[] = "exec CONTAINER cmd";

স্থির শূন্য
append_to_string_array (বর্ণ ***arr, size_t *size, ধ্রুব বর্ণ *arg)
{
  *arr = xrealloc (*arr, (*size + 2) * পরিমাণ (**arr));
  (*arr)[*size + 1] = রিক্ত;
  (*arr)[*size] = xstrdup (arg);
  (*size)++;
}

স্থির শূন্য
append_env (ধ্রুব বর্ণ *arg)
{
  append_to_string_array (&exec_options.env, &exec_options.env_size, arg);
}

স্থির শূন্য
append_cap (ধ্রুব বর্ণ *arg)
{
  append_to_string_array (&exec_options.cap, &exec_options.cap_size, arg);
}

স্থির error_t
parse_opt (পূর্ণাঙ্ক চাবি, বর্ণ *arg, সংরচনা argp_state *state)
{
  বিকল্প (চাবি)
    {
    পক্ষ OPTION_CONSOLE_SOCKET:
      exec_options.console_socket = arg;
      ভঙ্গ;

    পক্ষ OPTION_PID_FILE:
      exec_options.pid_file = arg;
      ভঙ্গ;

    পক্ষ OPTION_NO_NEW_PRIVS:
      exec_options.no_new_privs = true;
      ভঙ্গ;

    পক্ষ OPTION_PROCESS_LABEL:
      exec_options.process_label = argp_mandatory_argument (arg, state);
      ভঙ্গ;

    পক্ষ OPTION_APPARMOR:
      exec_options.apparmor = argp_mandatory_argument (arg, state);
      ভঙ্গ;

    পক্ষ OPTION_PRESERVE_FDS:
      exec_options.preserve_fds = parse_id_or_fail (argp_mandatory_argument (arg, state), রিক্ত, "preserve-fds");
      ভঙ্গ;

    পক্ষ OPTION_CGROUP:
      exec_options.cgroup = argp_mandatory_argument (arg, state);
      ভঙ্গ;

    পক্ষ 'd':
      exec_options.detach = true;
      ভঙ্গ;

    পক্ষ 'p':
      exec_options.process = arg;
      ভঙ্গ;

    পক্ষ 't':
      exec_options.tty = arg == রিক্ত || (strcmp (arg, "false") != 0 && strcmp (arg, "no") != 0);
      ভঙ্গ;

    পক্ষ 'u':
      exec_options.user = arg;
      ভঙ্গ;

    পক্ষ 'e':
      append_env (arg);
      ভঙ্গ;

    পক্ষ 'c':
      append_cap (arg);
      ভঙ্গ;

    পক্ষ OPTION_CWD:
      exec_options.cwd = xstrdup (arg);
      ভঙ্গ;

    পক্ষ ARGP_চাবি_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    স্বভাব:
      প্রত্যাগম ARGP_ERR_UNKNOWN;
    }

  প্রত্যাগম 0;
}

স্থির সংরচনা argp run_argp = { options, parse_opt, args_doc, doc, রিক্ত, রিক্ত, রিক্ত };

/* Minimal growable string buffer used to assemble the OCI Process JSON.  The
   binary is a pure consumer of the public API, so it builds the process as a
   JSON string rather than reaching for the internal ocispec types.  */
সংরচনা strbuf
{
  বর্ণ *buf;
  size_t len;
  size_t cap;
};

স্থির শূন্য
sb_ensure (সংরচনা strbuf *sb, size_t extra)
{
  যদি (sb->len + extra + 1 > sb->cap)
    {
      size_t ncap = sb->cap ? sb->cap : 256;
      যাবত্ (ncap < sb->len + extra + 1)
        ncap *= 2;
      sb->buf = xrealloc (sb->buf, ncap);
      sb->cap = ncap;
    }
}

স্থির শূন্য
sb_puts (সংরচনা strbuf *sb, ধ্রুব বর্ণ *s)
{
  size_t l = strlen (s);
  sb_ensure (sb, l);
  memcpy (sb->buf + sb->len, s, l);
  sb->len += l;
  sb->buf[sb->len] = '\0';
}

স্থির শূন্য
sb_putc (সংরচনা strbuf *sb, বর্ণ c)
{
  sb_ensure (sb, 1);
  sb->buf[sb->len++] = c;
  sb->buf[sb->len] = '\0';
}

/* Append S as a quoted, escaped JSON string.  */
স্থির শূন্য
sb_put_json_string (সংরচনা strbuf *sb, ধ্রুব বর্ণ *s)
{
  sb_putc (sb, '"');
  প্রতি (; *s; s++)
    {
      unsigned বর্ণ c = (unsigned বর্ণ) *s;
      বিকল্প (c)
        {
        পক্ষ '"':
          sb_puts (sb, "\\\"");
          ভঙ্গ;
        পক্ষ '\\':
          sb_puts (sb, "\\\\");
          ভঙ্গ;
        পক্ষ '\b':
          sb_puts (sb, "\\b");
          ভঙ্গ;
        পক্ষ '\f':
          sb_puts (sb, "\\f");
          ভঙ্গ;
        পক্ষ '\n':
          sb_puts (sb, "\\n");
          ভঙ্গ;
        পক্ষ '\r':
          sb_puts (sb, "\\r");
          ভঙ্গ;
        পক্ষ '\t':
          sb_puts (sb, "\\t");
          ভঙ্গ;
        স্বভাব:
          যদি (c < 0x20)
            {
              বর্ণ tmp[8];
              snprintf (tmp, পরিমাণ (tmp), "\\u%04x", c);
              sb_puts (sb, tmp);
            }
          অন্যথা
            sb_putc (sb, c);
        }
    }
  sb_putc (sb, '"');
}

স্থির শূন্য
sb_put_json_string_array (সংরচনা strbuf *sb, বর্ণ **arr, size_t len)
{
  size_t i;
  sb_putc (sb, '[');
  প্রতি (i = 0; i < len; i++)
    {
      যদি (i)
        sb_putc (sb, ',');
      sb_put_json_string (sb, arr[i]);
    }
  sb_putc (sb, ']');
}

/* Read the whole file at PATH into a NUL-terminated string.  A PATH of `-`
   reads from stdin.  */
স্থির বর্ণ *
read_file_to_string (ধ্রুব বর্ণ *path, libcrun_error_t *err)
{
  cleanup_file FILE *f = রিক্ত;
  সংরচনা strbuf sb = { 0 };
  বর্ণ chunk[4096];
  size_t n;

  যদি (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  f = fopen (path, "re");
  যদি (f == রিক্ত)
    {
      libcrun_make_error (err, errno, "cannot open `%s`", path);
      প্রত্যাগম রিক্ত;
    }

  যাবত্ ((n = fread (chunk, 1, পরিমাণ (chunk), f)) > 0)
    {
      sb_ensure (&sb, n);
      memcpy (sb.buf + sb.len, chunk, n);
      sb.len += n;
      sb.buf[sb.len] = '\0';
    }
  যদি (ferror (f))
    {
      মুক্ত (sb.buf);
      libcrun_make_error (err, errno, "cannot read `%s`", path);
      প্রত্যাগম রিক্ত;
    }

  যদি (sb.buf == রিক্ত)
    sb.buf = xstrdup ("");
  প্রত্যাগম sb.buf;
}

/* Build the OCI Process JSON from the CLI options.  */
স্থির বর্ণ *
build_process_json (সংরচনা crun_global_arguments *global_args arg_unused, বর্ণ **args, size_t args_len)
{
  সংরচনা strbuf sb = { 0 };

  sb_putc (&sb, '{');

  sb_puts (&sb, "\"args\":");
  sb_put_json_string_array (&sb, args, args_len);

  /* cwd is a required field in the OCI process schema; the exec path treats a
     missing value as "/".  */
  sb_puts (&sb, ",\"cwd\":");
  sb_put_json_string (&sb, exec_options.cwd ? exec_options.cwd : "/");

  sb_puts (&sb, ",\"terminal\":");
  sb_puts (&sb, exec_options.tty ? "true" : "false");

  যদি (exec_options.env_size > 0)
    {
      sb_puts (&sb, ",\"env\":");
      sb_put_json_string_array (&sb, exec_options.env, exec_options.env_size);
    }

  যদি (exec_options.user)
    {
      বর্ণ *endptr = রিক্ত;
      বর্ণ *gidstr;
      পূর্ণাঙ্ক uid, gid = 0;
      বর্ণ tmp[32];

      uid = parse_id_or_fail (exec_options.user, &endptr, "UID");
      যদি (*endptr == ':')
        {
          gidstr = endptr + 1;
          gid = parse_id_or_fail (gidstr, &endptr, "GID");
          যদি (*endptr != '\0')
            libcrun_fail_with_error (0, "invalid USERSPEC specified");
        }
      অন্যথা যদি (*endptr != '\0')
        libcrun_fail_with_error (0, "invalid USERSPEC specified");

      sb_puts (&sb, ",\"user\":{\"uid\":");
      snprintf (tmp, পরিমাণ (tmp), "%d", uid);
      sb_puts (&sb, tmp);
      sb_puts (&sb, ",\"gid\":");
      snprintf (tmp, পরিমাণ (tmp), "%d", gid);
      sb_puts (&sb, tmp);
      sb_putc (&sb, '}');
    }

  যদি (exec_options.process_label != রিক্ত)
    {
      sb_puts (&sb, ",\"selinuxLabel\":");
      sb_put_json_string (&sb, exec_options.process_label);
    }

  যদি (exec_options.apparmor != রিক্ত)
    {
      sb_puts (&sb, ",\"apparmorProfile\":");
      sb_put_json_string (&sb, exec_options.apparmor);
    }

  যদি (exec_options.cap_size > 0)
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
  যদি (exec_options.no_new_privs)
    sb_puts (&sb, ",\"noNewPrivileges\":true");

  sb_putc (&sb, '}');

  প্রত্যাগম sb.buf;
}

পূর্ণাঙ্ক
crun_command_exec (সংরচনা crun_global_arguments *global_args, পূর্ণাঙ্ক argc, বর্ণ **argv, libcrun_error_t *err)
{
  পূর্ণাঙ্ক first_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = রিক্ত;
  cleanup_free বর্ণ *process_json = রিক্ত;
  সংরচনা libcrun_exec_options_s exec_opts;

  memset (&exec_opts, 0, পরিমাণ (exec_opts));
  exec_opts.struct_size = পরিমাণ (exec_opts);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &exec_options);
  crun_assert_n_args (argc - first_arg, exec_options.process ? 1 : 2, -1);

  crun_context = new_libcrun_context (global_args);

  libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds);

  যদি (getenv ("LISTEN_FDS"))
    {
      পূর্ণাঙ্ক listen_fds = strtoll (getenv ("LISTEN_FDS"), রিক্ত, 10);
      libcrun_context_set_listen_fds (crun_context, listen_fds);
      libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds + listen_fds);
    }

  /* The process file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  যদি (exec_options.process)
    {
      process_json = read_file_to_string (exec_options.process, err);
      যদি (process_json == রিক্ত)
        প্রত্যাগম -1;
      exec_opts.merge_env = false;
    }
  অন্যথা
    {
      cleanup_free বর্ণ **args = রিক্ত;
      size_t args_len = argc - first_arg - 1;
      size_t i;

      /* argv[first_arg] is the container id, the command follows it.  */
      args = xmalloc0 ((args_len + 1) * পরিমাণ (*args));
      প্রতি (i = 0; i < args_len; i++)
        args[i] = argv[first_arg + i + 1];
      args[i] = রিক্ত;

      process_json = build_process_json (global_args, args, args_len);
      exec_opts.merge_env = true;
    }

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  libcrun_context_set_detach (crun_context, exec_options.detach);
  libcrun_context_set_console_socket (crun_context, exec_options.console_socket);
  libcrun_context_set_pid_file (crun_context, exec_options.pid_file);

  exec_opts.process_json = process_json;
  exec_opts.cgroup = exec_options.cgroup;

  প্রত্যাগম libcrun_container_exec_with_options_json (crun_context, argv[first_arg], &exec_opts, err);
}
