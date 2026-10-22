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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <argp.h>
#समावेश <string.h>
#समावेश <unistd.h>
#समावेश <errno.h>
#समावेश <limits.h>

#समावेश "crun.h"

अटल अक्षर doc[] = "OCI runtime";

काष्ठा exec_options_s
{
  bool tty;
  bool detach;
  bool no_new_privs;
  पूर्णांक preserve_fds;
  स्थिर अक्षर *process;
  स्थिर अक्षर *console_socket;
  स्थिर अक्षर *pid_file;
  अक्षर *process_label;
  अक्षर *apparmor;
  अक्षर *cwd;
  अक्षर *user;
  अक्षर **env;
  अक्षर **cap;
  माप_प्रकार cap_आकार;
  माप_प्रकार env_आकार;
  अक्षर *cgroup;
};

क्रमागत
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

अटल काष्ठा exec_options_s exec_options;

अटल काष्ठा argp_option options[]
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

अटल अक्षर args_doc[] = "exec CONTAINER cmd";

अटल व्योम
append_to_string_array (अक्षर ***arr, माप_प्रकार *आकार, स्थिर अक्षर *arg)
{
  *arr = xrealloc (*arr, (*आकार + 2) * माप (**arr));
  (*arr)[*आकार + 1] = शून्य;
  (*arr)[*आकार] = xstrdup (arg);
  (*आकार)++;
}

अटल व्योम
append_env (स्थिर अक्षर *arg)
{
  append_to_string_array (&exec_options.env, &exec_options.env_आकार, arg);
}

अटल व्योम
append_cap (स्थिर अक्षर *arg)
{
  append_to_string_array (&exec_options.cap, &exec_options.cap_आकार, arg);
}

अटल error_t
parse_opt (पूर्णांक कुंजी, अक्षर *arg, काष्ठा argp_state *state)
{
  चयन (कुंजी)
    {
    हाल OPTION_CONSOLE_SOCKET:
      exec_options.console_socket = arg;
      अवरोध;

    हाल OPTION_PID_FILE:
      exec_options.pid_file = arg;
      अवरोध;

    हाल OPTION_NO_NEW_PRIVS:
      exec_options.no_new_privs = true;
      अवरोध;

    हाल OPTION_PROCESS_LABEL:
      exec_options.process_label = argp_mandatory_argument (arg, state);
      अवरोध;

    हाल OPTION_APPARMOR:
      exec_options.apparmor = argp_mandatory_argument (arg, state);
      अवरोध;

    हाल OPTION_PRESERVE_FDS:
      exec_options.preserve_fds = parse_id_or_fail (argp_mandatory_argument (arg, state), शून्य, "preserve-fds");
      अवरोध;

    हाल OPTION_CGROUP:
      exec_options.cgroup = argp_mandatory_argument (arg, state);
      अवरोध;

    हाल 'd':
      exec_options.detach = true;
      अवरोध;

    हाल 'p':
      exec_options.process = arg;
      अवरोध;

    हाल 't':
      exec_options.tty = arg == शून्य || (म_भेद (arg, "false") != 0 && म_भेद (arg, "no") != 0);
      अवरोध;

    हाल 'u':
      exec_options.user = arg;
      अवरोध;

    हाल 'e':
      append_env (arg);
      अवरोध;

    हाल 'c':
      append_cap (arg);
      अवरोध;

    हाल OPTION_CWD:
      exec_options.cwd = xstrdup (arg);
      अवरोध;

    हाल ARGP_कुंजी_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    शेष:
      वापस ARGP_ERR_UNKNOWN;
    }

  वापस 0;
}

अटल काष्ठा argp run_argp = { options, parse_opt, args_doc, doc, शून्य, शून्य, शून्य };

/* Minimal growable string buffer used to assemble the OCI Process JSON.  The
   binary is a pure consumer of the public API, so it builds the process as a
   JSON string rather than reaching for the internal ocispec types.  */
काष्ठा strbuf
{
  अक्षर *buf;
  माप_प्रकार len;
  माप_प्रकार cap;
};

अटल व्योम
sb_ensure (काष्ठा strbuf *sb, माप_प्रकार extra)
{
  अगर (sb->len + extra + 1 > sb->cap)
    {
      माप_प्रकार ncap = sb->cap ? sb->cap : 256;
      जबतक (ncap < sb->len + extra + 1)
        ncap *= 2;
      sb->buf = xrealloc (sb->buf, ncap);
      sb->cap = ncap;
    }
}

अटल व्योम
sb_puts (काष्ठा strbuf *sb, स्थिर अक्षर *s)
{
  माप_प्रकार l = म_माप (s);
  sb_ensure (sb, l);
  स_नकल (sb->buf + sb->len, s, l);
  sb->len += l;
  sb->buf[sb->len] = '\0';
}

अटल व्योम
sb_putc (काष्ठा strbuf *sb, अक्षर c)
{
  sb_ensure (sb, 1);
  sb->buf[sb->len++] = c;
  sb->buf[sb->len] = '\0';
}

/* Append S as a quoted, escaped JSON string.  */
अटल व्योम
sb_put_json_string (काष्ठा strbuf *sb, स्थिर अक्षर *s)
{
  sb_putc (sb, '"');
  क्रम (; *s; s++)
    {
      अचिन्हित अक्षर c = (अचिन्हित अक्षर) *s;
      चयन (c)
        {
        हाल '"':
          sb_puts (sb, "\\\"");
          अवरोध;
        हाल '\\':
          sb_puts (sb, "\\\\");
          अवरोध;
        हाल '\b':
          sb_puts (sb, "\\b");
          अवरोध;
        हाल '\f':
          sb_puts (sb, "\\f");
          अवरोध;
        हाल '\n':
          sb_puts (sb, "\\n");
          अवरोध;
        हाल '\r':
          sb_puts (sb, "\\r");
          अवरोध;
        हाल '\t':
          sb_puts (sb, "\\t");
          अवरोध;
        शेष:
          अगर (c < 0x20)
            {
              अक्षर tmp[8];
              snprintf (tmp, माप (tmp), "\\u%04x", c);
              sb_puts (sb, tmp);
            }
          अन्यथा
            sb_putc (sb, c);
        }
    }
  sb_putc (sb, '"');
}

अटल व्योम
sb_put_json_string_array (काष्ठा strbuf *sb, अक्षर **arr, माप_प्रकार len)
{
  माप_प्रकार i;
  sb_putc (sb, '[');
  क्रम (i = 0; i < len; i++)
    {
      अगर (i)
        sb_putc (sb, ',');
      sb_put_json_string (sb, arr[i]);
    }
  sb_putc (sb, ']');
}

/* Read the whole file at PATH into a NUL-terminated string.  A PATH of `-`
   reads from stdin.  */
अटल अक्षर *
read_file_to_string (स्थिर अक्षर *path, libcrun_error_t *err)
{
  cleanup_file खाता *f = शून्य;
  काष्ठा strbuf sb = { 0 };
  अक्षर chunk[4096];
  माप_प्रकार n;

  अगर (म_भेद (path, "-") == 0)
    path = "/dev/stdin";

  f = ख_खोलो (path, "re");
  अगर (f == शून्य)
    {
      libcrun_make_error (err, त्रुटि_सं, "cannot open `%s`", path);
      वापस शून्य;
    }

  जबतक ((n = ख_पढ़ो (chunk, 1, माप (chunk), f)) > 0)
    {
      sb_ensure (&sb, n);
      स_नकल (sb.buf + sb.len, chunk, n);
      sb.len += n;
      sb.buf[sb.len] = '\0';
    }
  अगर (ख_त्रुटि (f))
    {
      मुक्त (sb.buf);
      libcrun_make_error (err, त्रुटि_सं, "cannot read `%s`", path);
      वापस शून्य;
    }

  अगर (sb.buf == शून्य)
    sb.buf = xstrdup ("");
  वापस sb.buf;
}

/* Build the OCI Process JSON from the CLI options.  */
अटल अक्षर *
build_process_json (काष्ठा crun_global_arguments *global_args arg_unused, अक्षर **args, माप_प्रकार args_len)
{
  काष्ठा strbuf sb = { 0 };

  sb_putc (&sb, '{');

  sb_puts (&sb, "\"args\":");
  sb_put_json_string_array (&sb, args, args_len);

  /* cwd is a required field in the OCI process schema; the exec path treats a
     missing value as "/".  */
  sb_puts (&sb, ",\"cwd\":");
  sb_put_json_string (&sb, exec_options.cwd ? exec_options.cwd : "/");

  sb_puts (&sb, ",\"terminal\":");
  sb_puts (&sb, exec_options.tty ? "true" : "false");

  अगर (exec_options.env_आकार > 0)
    {
      sb_puts (&sb, ",\"env\":");
      sb_put_json_string_array (&sb, exec_options.env, exec_options.env_आकार);
    }

  अगर (exec_options.user)
    {
      अक्षर *endptr = शून्य;
      अक्षर *gidstr;
      पूर्णांक uid, gid = 0;
      अक्षर tmp[32];

      uid = parse_id_or_fail (exec_options.user, &endptr, "UID");
      अगर (*endptr == ':')
        {
          gidstr = endptr + 1;
          gid = parse_id_or_fail (gidstr, &endptr, "GID");
          अगर (*endptr != '\0')
            libcrun_fail_with_error (0, "invalid USERSPEC specified");
        }
      अन्यथा अगर (*endptr != '\0')
        libcrun_fail_with_error (0, "invalid USERSPEC specified");

      sb_puts (&sb, ",\"user\":{\"uid\":");
      snprintf (tmp, माप (tmp), "%d", uid);
      sb_puts (&sb, tmp);
      sb_puts (&sb, ",\"gid\":");
      snprintf (tmp, माप (tmp), "%d", gid);
      sb_puts (&sb, tmp);
      sb_putc (&sb, '}');
    }

  अगर (exec_options.process_label != शून्य)
    {
      sb_puts (&sb, ",\"selinuxLabel\":");
      sb_put_json_string (&sb, exec_options.process_label);
    }

  अगर (exec_options.apparmor != शून्य)
    {
      sb_puts (&sb, ",\"apparmorProfile\":");
      sb_put_json_string (&sb, exec_options.apparmor);
    }

  अगर (exec_options.cap_आकार > 0)
    {
      sb_puts (&sb, ",\"capabilities\":{");
      sb_puts (&sb, "\"effective\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_आकार);
      sb_puts (&sb, ",\"bounding\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_आकार);
      sb_puts (&sb, ",\"ambient\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_आकार);
      sb_puts (&sb, ",\"permitted\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_आकार);
      sb_putc (&sb, '}');
    }

  /* noNewPrivileges remains at the config default unless explicitly requested.  */
  अगर (exec_options.no_new_privs)
    sb_puts (&sb, ",\"noNewPrivileges\":true");

  sb_putc (&sb, '}');

  वापस sb.buf;
}

पूर्णांक
crun_command_exec (काष्ठा crun_global_arguments *global_args, पूर्णांक argc, अक्षर **argv, libcrun_error_t *err)
{
  पूर्णांक first_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = शून्य;
  cleanup_free अक्षर *process_json = शून्य;
  काष्ठा libcrun_exec_options_s exec_opts;

  स_रखो (&exec_opts, 0, माप (exec_opts));
  exec_opts.struct_आकार = माप (exec_opts);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &exec_options);
  crun_assert_n_args (argc - first_arg, exec_options.process ? 1 : 2, -1);

  crun_context = new_libcrun_context (global_args);

  libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds);

  अगर (दो_पर्या ("LISTEN_FDS"))
    {
      पूर्णांक listen_fds = strtoll (दो_पर्या ("LISTEN_FDS"), शून्य, 10);
      libcrun_context_set_listen_fds (crun_context, listen_fds);
      libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds + listen_fds);
    }

  /* The process file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  अगर (exec_options.process)
    {
      process_json = read_file_to_string (exec_options.process, err);
      अगर (process_json == शून्य)
        वापस -1;
      exec_opts.merge_env = false;
    }
  अन्यथा
    {
      cleanup_free अक्षर **args = शून्य;
      माप_प्रकार args_len = argc - first_arg - 1;
      माप_प्रकार i;

      /* argv[first_arg] is the container id, the command follows it.  */
      args = xmalloc0 ((args_len + 1) * माप (*args));
      क्रम (i = 0; i < args_len; i++)
        args[i] = argv[first_arg + i + 1];
      args[i] = शून्य;

      process_json = build_process_json (global_args, args, args_len);
      exec_opts.merge_env = true;
    }

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  libcrun_context_set_detach (crun_context, exec_options.detach);
  libcrun_context_set_console_socket (crun_context, exec_options.console_socket);
  libcrun_context_set_pid_file (crun_context, exec_options.pid_file);

  exec_opts.process_json = process_json;
  exec_opts.cgroup = exec_options.cgroup;

  वापस libcrun_container_exec_with_options_json (crun_context, argv[first_arg], &exec_opts, err);
}
