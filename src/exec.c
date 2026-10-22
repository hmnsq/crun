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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <argp.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <limits.h>

#अन्तर्भाव "crun.h"

स्थिर वर्ण doc[] = "OCI runtime";

संरचना exec_options_s
{
  bool tty;
  bool detach;
  bool no_new_privs;
  पूर्णाङ्क preserve_fds;
  ध्रुव वर्ण *process;
  ध्रुव वर्ण *console_socket;
  ध्रुव वर्ण *pid_file;
  वर्ण *process_label;
  वर्ण *apparmor;
  वर्ण *cwd;
  वर्ण *user;
  वर्ण **env;
  वर्ण **cap;
  size_t cap_size;
  size_t env_size;
  वर्ण *cgroup;
};

गणना
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

स्थिर संरचना exec_options_s exec_options;

स्थिर संरचना argp_option options[]
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

स्थिर वर्ण args_doc[] = "exec CONTAINER cmd";

स्थिर शून्य
append_to_string_array (वर्ण ***arr, size_t *size, ध्रुव वर्ण *arg)
{
  *arr = xrealloc (*arr, (*size + 2) * परिमाण (**arr));
  (*arr)[*size + 1] = रिक्त;
  (*arr)[*size] = xstrdup (arg);
  (*size)++;
}

स्थिर शून्य
append_env (ध्रुव वर्ण *arg)
{
  append_to_string_array (&exec_options.env, &exec_options.env_size, arg);
}

स्थिर शून्य
append_cap (ध्रुव वर्ण *arg)
{
  append_to_string_array (&exec_options.cap, &exec_options.cap_size, arg);
}

स्थिर error_t
parse_opt (पूर्णाङ्क कुञ्चिका, वर्ण *arg, संरचना argp_state *state)
{
  विकल्प (कुञ्चिका)
    {
    पक्ष OPTION_CONSOLE_SOCKET:
      exec_options.console_socket = arg;
      भङ्ग;

    पक्ष OPTION_PID_FILE:
      exec_options.pid_file = arg;
      भङ्ग;

    पक्ष OPTION_NO_NEW_PRIVS:
      exec_options.no_new_privs = true;
      भङ्ग;

    पक्ष OPTION_PROCESS_LABEL:
      exec_options.process_label = argp_mandatory_argument (arg, state);
      भङ्ग;

    पक्ष OPTION_APPARMOR:
      exec_options.apparmor = argp_mandatory_argument (arg, state);
      भङ्ग;

    पक्ष OPTION_PRESERVE_FDS:
      exec_options.preserve_fds = parse_id_or_fail (argp_mandatory_argument (arg, state), रिक्त, "preserve-fds");
      भङ्ग;

    पक्ष OPTION_CGROUP:
      exec_options.cgroup = argp_mandatory_argument (arg, state);
      भङ्ग;

    पक्ष 'd':
      exec_options.detach = true;
      भङ्ग;

    पक्ष 'p':
      exec_options.process = arg;
      भङ्ग;

    पक्ष 't':
      exec_options.tty = arg == रिक्त || (strcmp (arg, "false") != 0 && strcmp (arg, "no") != 0);
      भङ्ग;

    पक्ष 'u':
      exec_options.user = arg;
      भङ्ग;

    पक्ष 'e':
      append_env (arg);
      भङ्ग;

    पक्ष 'c':
      append_cap (arg);
      भङ्ग;

    पक्ष OPTION_CWD:
      exec_options.cwd = xstrdup (arg);
      भङ्ग;

    पक्ष ARGP_कुञ्चिका_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    स्वभाव:
      प्रत्यागम ARGP_ERR_UNKNOWN;
    }

  प्रत्यागम 0;
}

स्थिर संरचना argp run_argp = { options, parse_opt, args_doc, doc, रिक्त, रिक्त, रिक्त };

/* Minimal growable string buffer used to assemble the OCI Process JSON.  The
   binary is a pure consumer of the public API, so it builds the process as a
   JSON string rather than reaching for the internal ocispec types.  */
संरचना strbuf
{
  वर्ण *buf;
  size_t len;
  size_t cap;
};

स्थिर शून्य
sb_ensure (संरचना strbuf *sb, size_t extra)
{
  जइ (sb->len + extra + 1 > sb->cap)
    {
      size_t ncap = sb->cap ? sb->cap : 256;
      जाव (ncap < sb->len + extra + 1)
        ncap *= 2;
      sb->buf = xrealloc (sb->buf, ncap);
      sb->cap = ncap;
    }
}

स्थिर शून्य
sb_puts (संरचना strbuf *sb, ध्रुव वर्ण *s)
{
  size_t l = strlen (s);
  sb_ensure (sb, l);
  memcpy (sb->buf + sb->len, s, l);
  sb->len += l;
  sb->buf[sb->len] = '\0';
}

स्थिर शून्य
sb_putc (संरचना strbuf *sb, वर्ण c)
{
  sb_ensure (sb, 1);
  sb->buf[sb->len++] = c;
  sb->buf[sb->len] = '\0';
}

/* Append S as a quoted, escaped JSON string.  */
स्थिर शून्य
sb_put_json_string (संरचना strbuf *sb, ध्रुव वर्ण *s)
{
  sb_putc (sb, '"');
  प्रति (; *s; s++)
    {
      unsigned वर्ण c = (unsigned वर्ण) *s;
      विकल्प (c)
        {
        पक्ष '"':
          sb_puts (sb, "\\\"");
          भङ्ग;
        पक्ष '\\':
          sb_puts (sb, "\\\\");
          भङ्ग;
        पक्ष '\b':
          sb_puts (sb, "\\b");
          भङ्ग;
        पक्ष '\f':
          sb_puts (sb, "\\f");
          भङ्ग;
        पक्ष '\n':
          sb_puts (sb, "\\n");
          भङ्ग;
        पक्ष '\r':
          sb_puts (sb, "\\r");
          भङ्ग;
        पक्ष '\t':
          sb_puts (sb, "\\t");
          भङ्ग;
        स्वभाव:
          जइ (c < 0x20)
            {
              वर्ण tmp[8];
              snprintf (tmp, परिमाण (tmp), "\\u%04x", c);
              sb_puts (sb, tmp);
            }
          अण्णहा
            sb_putc (sb, c);
        }
    }
  sb_putc (sb, '"');
}

स्थिर शून्य
sb_put_json_string_array (संरचना strbuf *sb, वर्ण **arr, size_t len)
{
  size_t i;
  sb_putc (sb, '[');
  प्रति (i = 0; i < len; i++)
    {
      जइ (i)
        sb_putc (sb, ',');
      sb_put_json_string (sb, arr[i]);
    }
  sb_putc (sb, ']');
}

/* Read the whole file at PATH into a NUL-terminated string.  A PATH of `-`
   reads from stdin.  */
स्थिर वर्ण *
read_file_to_string (ध्रुव वर्ण *path, libcrun_error_t *err)
{
  cleanup_file FILE *f = रिक्त;
  संरचना strbuf sb = { 0 };
  वर्ण chunk[4096];
  size_t n;

  जइ (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  f = fopen (path, "re");
  जइ (f == रिक्त)
    {
      libcrun_make_error (err, errno, "cannot open `%s`", path);
      प्रत्यागम रिक्त;
    }

  जाव ((n = fread (chunk, 1, परिमाण (chunk), f)) > 0)
    {
      sb_ensure (&sb, n);
      memcpy (sb.buf + sb.len, chunk, n);
      sb.len += n;
      sb.buf[sb.len] = '\0';
    }
  जइ (ferror (f))
    {
      मुक्त (sb.buf);
      libcrun_make_error (err, errno, "cannot read `%s`", path);
      प्रत्यागम रिक्त;
    }

  जइ (sb.buf == रिक्त)
    sb.buf = xstrdup ("");
  प्रत्यागम sb.buf;
}

/* Build the OCI Process JSON from the CLI options.  */
स्थिर वर्ण *
build_process_json (संरचना crun_global_arguments *global_args arg_unused, वर्ण **args, size_t args_len)
{
  संरचना strbuf sb = { 0 };

  sb_putc (&sb, '{');

  sb_puts (&sb, "\"args\":");
  sb_put_json_string_array (&sb, args, args_len);

  /* cwd is a required field in the OCI process schema; the exec path treats a
     missing value as "/".  */
  sb_puts (&sb, ",\"cwd\":");
  sb_put_json_string (&sb, exec_options.cwd ? exec_options.cwd : "/");

  sb_puts (&sb, ",\"terminal\":");
  sb_puts (&sb, exec_options.tty ? "true" : "false");

  जइ (exec_options.env_size > 0)
    {
      sb_puts (&sb, ",\"env\":");
      sb_put_json_string_array (&sb, exec_options.env, exec_options.env_size);
    }

  जइ (exec_options.user)
    {
      वर्ण *endptr = रिक्त;
      वर्ण *gidstr;
      पूर्णाङ्क uid, gid = 0;
      वर्ण tmp[32];

      uid = parse_id_or_fail (exec_options.user, &endptr, "UID");
      जइ (*endptr == ':')
        {
          gidstr = endptr + 1;
          gid = parse_id_or_fail (gidstr, &endptr, "GID");
          जइ (*endptr != '\0')
            libcrun_fail_with_error (0, "invalid USERSPEC specified");
        }
      अण्णहा जइ (*endptr != '\0')
        libcrun_fail_with_error (0, "invalid USERSPEC specified");

      sb_puts (&sb, ",\"user\":{\"uid\":");
      snprintf (tmp, परिमाण (tmp), "%d", uid);
      sb_puts (&sb, tmp);
      sb_puts (&sb, ",\"gid\":");
      snprintf (tmp, परिमाण (tmp), "%d", gid);
      sb_puts (&sb, tmp);
      sb_putc (&sb, '}');
    }

  जइ (exec_options.process_label != रिक्त)
    {
      sb_puts (&sb, ",\"selinuxLabel\":");
      sb_put_json_string (&sb, exec_options.process_label);
    }

  जइ (exec_options.apparmor != रिक्त)
    {
      sb_puts (&sb, ",\"apparmorProfile\":");
      sb_put_json_string (&sb, exec_options.apparmor);
    }

  जइ (exec_options.cap_size > 0)
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
  जइ (exec_options.no_new_privs)
    sb_puts (&sb, ",\"noNewPrivileges\":true");

  sb_putc (&sb, '}');

  प्रत्यागम sb.buf;
}

पूर्णाङ्क
crun_command_exec (संरचना crun_global_arguments *global_args, पूर्णाङ्क argc, वर्ण **argv, libcrun_error_t *err)
{
  पूर्णाङ्क first_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = रिक्त;
  cleanup_free वर्ण *process_json = रिक्त;
  संरचना libcrun_exec_options_s exec_opts;

  memset (&exec_opts, 0, परिमाण (exec_opts));
  exec_opts.struct_size = परिमाण (exec_opts);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &exec_options);
  crun_assert_n_args (argc - first_arg, exec_options.process ? 1 : 2, -1);

  crun_context = new_libcrun_context (global_args);

  libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds);

  जइ (getenv ("LISTEN_FDS"))
    {
      पूर्णाङ्क listen_fds = strtoll (getenv ("LISTEN_FDS"), रिक्त, 10);
      libcrun_context_set_listen_fds (crun_context, listen_fds);
      libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds + listen_fds);
    }

  /* The process file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  जइ (exec_options.process)
    {
      process_json = read_file_to_string (exec_options.process, err);
      जइ (process_json == रिक्त)
        प्रत्यागम -1;
      exec_opts.merge_env = false;
    }
  अण्णहा
    {
      cleanup_free वर्ण **args = रिक्त;
      size_t args_len = argc - first_arg - 1;
      size_t i;

      /* argv[first_arg] is the container id, the command follows it.  */
      args = xmalloc0 ((args_len + 1) * परिमाण (*args));
      प्रति (i = 0; i < args_len; i++)
        args[i] = argv[first_arg + i + 1];
      args[i] = रिक्त;

      process_json = build_process_json (global_args, args, args_len);
      exec_opts.merge_env = true;
    }

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  libcrun_context_set_detach (crun_context, exec_options.detach);
  libcrun_context_set_console_socket (crun_context, exec_options.console_socket);
  libcrun_context_set_pid_file (crun_context, exec_options.pid_file);

  exec_opts.process_json = process_json;
  exec_opts.cgroup = exec_options.cgroup;

  प्रत्यागम libcrun_container_exec_with_options_json (crun_context, argv[first_arg], &exec_opts, err);
}
