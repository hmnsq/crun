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

स्थिर अक्षर doc[] = "OCI runtime";

संरचना exec_options_s
{
  bool tty;
  bool detach;
  bool no_new_privs;
  पूर्णांक preserve_fds;
  स्थिरांक अक्षर *process;
  स्थिरांक अक्षर *console_socket;
  स्थिरांक अक्षर *pid_file;
  अक्षर *process_label;
  अक्षर *apparmor;
  अक्षर *cwd;
  अक्षर *user;
  अक्षर **env;
  अक्षर **cap;
  माप_प्रकार cap_size;
  माप_प्रकार env_size;
  अक्षर *cgroup;
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

स्थिर अक्षर args_doc[] = "exec CONTAINER cmd";

स्थिर शून्य
append_to_string_array (अक्षर ***arr, माप_प्रकार *size, स्थिरांक अक्षर *arg)
{
  *arr = xrealloc (*arr, (*size + 2) * आकार (**arr));
  (*arr)[*size + 1] = रिक्त;
  (*arr)[*size] = xstrdup (arg);
  (*size)++;
}

स्थिर शून्य
append_env (स्थिरांक अक्षर *arg)
{
  append_to_string_array (&exec_options.env, &exec_options.env_size, arg);
}

स्थिर शून्य
append_cap (स्थिरांक अक्षर *arg)
{
  append_to_string_array (&exec_options.cap, &exec_options.cap_size, arg);
}

स्थिर error_t
parse_opt (पूर्णांक किल्ली, अक्षर *arg, संरचना argp_state *state)
{
  निवड (किल्ली)
    {
    स्थिती OPTION_CONSOLE_SOCKET:
      exec_options.console_socket = arg;
      थांबा;

    स्थिती OPTION_PID_FILE:
      exec_options.pid_file = arg;
      थांबा;

    स्थिती OPTION_NO_NEW_PRIVS:
      exec_options.no_new_privs = true;
      थांबा;

    स्थिती OPTION_PROCESS_LABEL:
      exec_options.process_label = argp_mandatory_argument (arg, state);
      थांबा;

    स्थिती OPTION_APPARMOR:
      exec_options.apparmor = argp_mandatory_argument (arg, state);
      थांबा;

    स्थिती OPTION_PRESERVE_FDS:
      exec_options.preserve_fds = parse_id_or_fail (argp_mandatory_argument (arg, state), रिक्त, "preserve-fds");
      थांबा;

    स्थिती OPTION_CGROUP:
      exec_options.cgroup = argp_mandatory_argument (arg, state);
      थांबा;

    स्थिती 'd':
      exec_options.detach = true;
      थांबा;

    स्थिती 'p':
      exec_options.process = arg;
      थांबा;

    स्थिती 't':
      exec_options.tty = arg == रिक्त || (म_भेद (arg, "false") != 0 && म_भेद (arg, "no") != 0);
      थांबा;

    स्थिती 'u':
      exec_options.user = arg;
      थांबा;

    स्थिती 'e':
      append_env (arg);
      थांबा;

    स्थिती 'c':
      append_cap (arg);
      थांबा;

    स्थिती OPTION_CWD:
      exec_options.cwd = xstrdup (arg);
      थांबा;

    स्थिती ARGP_किल्ली_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    पूर्वनिर्धारित:
      परत ARGP_ERR_UNKNOWN;
    }

  परत 0;
}

स्थिर संरचना argp run_argp = { options, parse_opt, args_doc, doc, रिक्त, रिक्त, रिक्त };

/* Minimal growable string buffer used to assemble the OCI Process JSON.  The
   binary is a pure consumer of the public API, so it builds the process as a
   JSON string rather than reaching for the internal ocispec types.  */
संरचना strbuf
{
  अक्षर *buf;
  माप_प्रकार len;
  माप_प्रकार cap;
};

स्थिर शून्य
sb_ensure (संरचना strbuf *sb, माप_प्रकार extra)
{
  जर (sb->len + extra + 1 > sb->cap)
    {
      माप_प्रकार ncap = sb->cap ? sb->cap : 256;
      पर्यंत (ncap < sb->len + extra + 1)
        ncap *= 2;
      sb->buf = xrealloc (sb->buf, ncap);
      sb->cap = ncap;
    }
}

स्थिर शून्य
sb_puts (संरचना strbuf *sb, स्थिरांक अक्षर *s)
{
  माप_प्रकार l = म_माप (s);
  sb_ensure (sb, l);
  स_नकल (sb->buf + sb->len, s, l);
  sb->len += l;
  sb->buf[sb->len] = '\0';
}

स्थिर शून्य
sb_putc (संरचना strbuf *sb, अक्षर c)
{
  sb_ensure (sb, 1);
  sb->buf[sb->len++] = c;
  sb->buf[sb->len] = '\0';
}

/* Append S as a quoted, escaped JSON string.  */
स्थिर शून्य
sb_put_json_string (संरचना strbuf *sb, स्थिरांक अक्षर *s)
{
  sb_putc (sb, '"');
  साठी (; *s; s++)
    {
      अचिन्हित अक्षर c = (अचिन्हित अक्षर) *s;
      निवड (c)
        {
        स्थिती '"':
          sb_puts (sb, "\\\"");
          थांबा;
        स्थिती '\\':
          sb_puts (sb, "\\\\");
          थांबा;
        स्थिती '\b':
          sb_puts (sb, "\\b");
          थांबा;
        स्थिती '\f':
          sb_puts (sb, "\\f");
          थांबा;
        स्थिती '\n':
          sb_puts (sb, "\\n");
          थांबा;
        स्थिती '\r':
          sb_puts (sb, "\\r");
          थांबा;
        स्थिती '\t':
          sb_puts (sb, "\\t");
          थांबा;
        पूर्वनिर्धारित:
          जर (c < 0x20)
            {
              अक्षर tmp[8];
              snprintf (tmp, आकार (tmp), "\\u%04x", c);
              sb_puts (sb, tmp);
            }
          अन्यथा
            sb_putc (sb, c);
        }
    }
  sb_putc (sb, '"');
}

स्थिर शून्य
sb_put_json_string_array (संरचना strbuf *sb, अक्षर **arr, माप_प्रकार len)
{
  माप_प्रकार i;
  sb_putc (sb, '[');
  साठी (i = 0; i < len; i++)
    {
      जर (i)
        sb_putc (sb, ',');
      sb_put_json_string (sb, arr[i]);
    }
  sb_putc (sb, ']');
}

/* Read the whole file at PATH into a NUL-terminated string.  A PATH of `-`
   reads from stdin.  */
स्थिर अक्षर *
read_file_to_string (स्थिरांक अक्षर *path, libcrun_error_t *err)
{
  cleanup_file खाता *f = रिक्त;
  संरचना strbuf sb = { 0 };
  अक्षर chunk[4096];
  माप_प्रकार n;

  जर (म_भेद (path, "-") == 0)
    path = "/dev/stdin";

  f = ख_खोलो (path, "re");
  जर (f == रिक्त)
    {
      libcrun_make_error (err, त्रुटि_सं, "cannot open `%s`", path);
      परत रिक्त;
    }

  पर्यंत ((n = ख_पढ़ो (chunk, 1, आकार (chunk), f)) > 0)
    {
      sb_ensure (&sb, n);
      स_नकल (sb.buf + sb.len, chunk, n);
      sb.len += n;
      sb.buf[sb.len] = '\0';
    }
  जर (ख_त्रुटि (f))
    {
      मुक्त (sb.buf);
      libcrun_make_error (err, त्रुटि_सं, "cannot read `%s`", path);
      परत रिक्त;
    }

  जर (sb.buf == रिक्त)
    sb.buf = xstrdup ("");
  परत sb.buf;
}

/* Build the OCI Process JSON from the CLI options.  */
स्थिर अक्षर *
build_process_json (संरचना crun_global_arguments *global_args arg_unused, अक्षर **args, माप_प्रकार args_len)
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

  जर (exec_options.env_size > 0)
    {
      sb_puts (&sb, ",\"env\":");
      sb_put_json_string_array (&sb, exec_options.env, exec_options.env_size);
    }

  जर (exec_options.user)
    {
      अक्षर *endptr = रिक्त;
      अक्षर *gidstr;
      पूर्णांक uid, gid = 0;
      अक्षर tmp[32];

      uid = parse_id_or_fail (exec_options.user, &endptr, "UID");
      जर (*endptr == ':')
        {
          gidstr = endptr + 1;
          gid = parse_id_or_fail (gidstr, &endptr, "GID");
          जर (*endptr != '\0')
            libcrun_fail_with_error (0, "invalid USERSPEC specified");
        }
      अन्यथा जर (*endptr != '\0')
        libcrun_fail_with_error (0, "invalid USERSPEC specified");

      sb_puts (&sb, ",\"user\":{\"uid\":");
      snprintf (tmp, आकार (tmp), "%d", uid);
      sb_puts (&sb, tmp);
      sb_puts (&sb, ",\"gid\":");
      snprintf (tmp, आकार (tmp), "%d", gid);
      sb_puts (&sb, tmp);
      sb_putc (&sb, '}');
    }

  जर (exec_options.process_label != रिक्त)
    {
      sb_puts (&sb, ",\"selinuxLabel\":");
      sb_put_json_string (&sb, exec_options.process_label);
    }

  जर (exec_options.apparmor != रिक्त)
    {
      sb_puts (&sb, ",\"apparmorProfile\":");
      sb_put_json_string (&sb, exec_options.apparmor);
    }

  जर (exec_options.cap_size > 0)
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
  जर (exec_options.no_new_privs)
    sb_puts (&sb, ",\"noNewPrivileges\":true");

  sb_putc (&sb, '}');

  परत sb.buf;
}

पूर्णांक
crun_command_exec (संरचना crun_global_arguments *global_args, पूर्णांक argc, अक्षर **argv, libcrun_error_t *err)
{
  पूर्णांक first_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = रिक्त;
  cleanup_free अक्षर *process_json = रिक्त;
  संरचना libcrun_exec_options_s exec_opts;

  स_रखो (&exec_opts, 0, आकार (exec_opts));
  exec_opts.struct_size = आकार (exec_opts);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &exec_options);
  crun_assert_n_args (argc - first_arg, exec_options.process ? 1 : 2, -1);

  crun_context = new_libcrun_context (global_args);

  libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds);

  जर (दो_पर्या ("LISTEN_FDS"))
    {
      पूर्णांक listen_fds = strtoll (दो_पर्या ("LISTEN_FDS"), रिक्त, 10);
      libcrun_context_set_listen_fds (crun_context, listen_fds);
      libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds + listen_fds);
    }

  /* The process file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  जर (exec_options.process)
    {
      process_json = read_file_to_string (exec_options.process, err);
      जर (process_json == रिक्त)
        परत -1;
      exec_opts.merge_env = false;
    }
  अन्यथा
    {
      cleanup_free अक्षर **args = रिक्त;
      माप_प्रकार args_len = argc - first_arg - 1;
      माप_प्रकार i;

      /* argv[first_arg] is the container id, the command follows it.  */
      args = xmalloc0 ((args_len + 1) * आकार (*args));
      साठी (i = 0; i < args_len; i++)
        args[i] = argv[first_arg + i + 1];
      args[i] = रिक्त;

      process_json = build_process_json (global_args, args, args_len);
      exec_opts.merge_env = true;
    }

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  libcrun_context_set_detach (crun_context, exec_options.detach);
  libcrun_context_set_console_socket (crun_context, exec_options.console_socket);
  libcrun_context_set_pid_file (crun_context, exec_options.pid_file);

  exec_opts.process_json = process_json;
  exec_opts.cgroup = exec_options.cgroup;

  परत libcrun_container_exec_with_options_json (crun_context, argv[first_arg], &exec_opts, err);
}
