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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <argp.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <limits.h>

#அந்தர்பாவ "crun.h"

நிலையான எழுத்து doc[] = "OCI runtime";

கட்டமைப்பு exec_options_s
{
  bool tty;
  bool detach;
  bool no_new_privs;
  முழு preserve_fds;
  மாறிலி எழுத்து *process;
  மாறிலி எழுத்து *console_socket;
  மாறிலி எழுத்து *pid_file;
  எழுத்து *process_label;
  எழுத்து *apparmor;
  எழுத்து *cwd;
  எழுத்து *user;
  எழுத்து **env;
  எழுத்து **cap;
  size_t cap_size;
  size_t env_size;
  எழுத்து *cgroup;
};

எண்ணிக்கை
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

நிலையான கட்டமைப்பு exec_options_s exec_options;

நிலையான கட்டமைப்பு argp_option options[]
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

நிலையான எழுத்து args_doc[] = "exec CONTAINER cmd";

நிலையான வெறுமை
append_to_string_array (எழுத்து ***arr, size_t *size, மாறிலி எழுத்து *arg)
{
  *arr = xrealloc (*arr, (*size + 2) * அளவு (**arr));
  (*arr)[*size + 1] = வெற்று;
  (*arr)[*size] = xstrdup (arg);
  (*size)++;
}

நிலையான வெறுமை
append_env (மாறிலி எழுத்து *arg)
{
  append_to_string_array (&exec_options.env, &exec_options.env_size, arg);
}

நிலையான வெறுமை
append_cap (மாறிலி எழுத்து *arg)
{
  append_to_string_array (&exec_options.cap, &exec_options.cap_size, arg);
}

நிலையான error_t
parse_opt (முழு சாவி, எழுத்து *arg, கட்டமைப்பு argp_state *state)
{
  தேர்வு (சாவி)
    {
    நிலை OPTION_CONSOLE_SOCKET:
      exec_options.console_socket = arg;
      நிறுத்து;

    நிலை OPTION_PID_FILE:
      exec_options.pid_file = arg;
      நிறுத்து;

    நிலை OPTION_NO_NEW_PRIVS:
      exec_options.no_new_privs = true;
      நிறுத்து;

    நிலை OPTION_PROCESS_LABEL:
      exec_options.process_label = argp_mandatory_argument (arg, state);
      நிறுத்து;

    நிலை OPTION_APPARMOR:
      exec_options.apparmor = argp_mandatory_argument (arg, state);
      நிறுத்து;

    நிலை OPTION_PRESERVE_FDS:
      exec_options.preserve_fds = parse_id_or_fail (argp_mandatory_argument (arg, state), வெற்று, "preserve-fds");
      நிறுத்து;

    நிலை OPTION_CGROUP:
      exec_options.cgroup = argp_mandatory_argument (arg, state);
      நிறுத்து;

    நிலை 'd':
      exec_options.detach = true;
      நிறுத்து;

    நிலை 'p':
      exec_options.process = arg;
      நிறுத்து;

    நிலை 't':
      exec_options.tty = arg == வெற்று || (strcmp (arg, "false") != 0 && strcmp (arg, "no") != 0);
      நிறுத்து;

    நிலை 'u':
      exec_options.user = arg;
      நிறுத்து;

    நிலை 'e':
      append_env (arg);
      நிறுத்து;

    நிலை 'c':
      append_cap (arg);
      நிறுத்து;

    நிலை OPTION_CWD:
      exec_options.cwd = xstrdup (arg);
      நிறுத்து;

    நிலை ARGP_சாவி_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    இயல்பு:
      திரும்பு ARGP_ERR_UNKNOWN;
    }

  திரும்பு 0;
}

நிலையான கட்டமைப்பு argp run_argp = { options, parse_opt, args_doc, doc, வெற்று, வெற்று, வெற்று };

/* Minimal growable string buffer used to assemble the OCI Process JSON.  The
   binary is a pure consumer of the public API, so it builds the process as a
   JSON string rather than reaching for the internal ocispec types.  */
கட்டமைப்பு strbuf
{
  எழுத்து *buf;
  size_t len;
  size_t cap;
};

நிலையான வெறுமை
sb_ensure (கட்டமைப்பு strbuf *sb, size_t extra)
{
  எனில் (sb->len + extra + 1 > sb->cap)
    {
      size_t ncap = sb->cap ? sb->cap : 256;
      போது (ncap < sb->len + extra + 1)
        ncap *= 2;
      sb->buf = xrealloc (sb->buf, ncap);
      sb->cap = ncap;
    }
}

நிலையான வெறுமை
sb_puts (கட்டமைப்பு strbuf *sb, மாறிலி எழுத்து *s)
{
  size_t l = strlen (s);
  sb_ensure (sb, l);
  memcpy (sb->buf + sb->len, s, l);
  sb->len += l;
  sb->buf[sb->len] = '\0';
}

நிலையான வெறுமை
sb_putc (கட்டமைப்பு strbuf *sb, எழுத்து c)
{
  sb_ensure (sb, 1);
  sb->buf[sb->len++] = c;
  sb->buf[sb->len] = '\0';
}

/* Append S as a quoted, escaped JSON string.  */
நிலையான வெறுமை
sb_put_json_string (கட்டமைப்பு strbuf *sb, மாறிலி எழுத்து *s)
{
  sb_putc (sb, '"');
  க்கு (; *s; s++)
    {
      unsigned எழுத்து c = (unsigned எழுத்து) *s;
      தேர்வு (c)
        {
        நிலை '"':
          sb_puts (sb, "\\\"");
          நிறுத்து;
        நிலை '\\':
          sb_puts (sb, "\\\\");
          நிறுத்து;
        நிலை '\b':
          sb_puts (sb, "\\b");
          நிறுத்து;
        நிலை '\f':
          sb_puts (sb, "\\f");
          நிறுத்து;
        நிலை '\n':
          sb_puts (sb, "\\n");
          நிறுத்து;
        நிலை '\r':
          sb_puts (sb, "\\r");
          நிறுத்து;
        நிலை '\t':
          sb_puts (sb, "\\t");
          நிறுத்து;
        இயல்பு:
          எனில் (c < 0x20)
            {
              எழுத்து tmp[8];
              snprintf (tmp, அளவு (tmp), "\\u%04x", c);
              sb_puts (sb, tmp);
            }
          இல்லையெனில்
            sb_putc (sb, c);
        }
    }
  sb_putc (sb, '"');
}

நிலையான வெறுமை
sb_put_json_string_array (கட்டமைப்பு strbuf *sb, எழுத்து **arr, size_t len)
{
  size_t i;
  sb_putc (sb, '[');
  க்கு (i = 0; i < len; i++)
    {
      எனில் (i)
        sb_putc (sb, ',');
      sb_put_json_string (sb, arr[i]);
    }
  sb_putc (sb, ']');
}

/* Read the whole file at PATH into a NUL-terminated string.  A PATH of `-`
   reads from stdin.  */
நிலையான எழுத்து *
read_file_to_string (மாறிலி எழுத்து *path, libcrun_error_t *err)
{
  cleanup_file FILE *f = வெற்று;
  கட்டமைப்பு strbuf sb = { 0 };
  எழுத்து chunk[4096];
  size_t n;

  எனில் (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  f = fopen (path, "re");
  எனில் (f == வெற்று)
    {
      libcrun_make_error (err, errno, "cannot open `%s`", path);
      திரும்பு வெற்று;
    }

  போது ((n = fread (chunk, 1, அளவு (chunk), f)) > 0)
    {
      sb_ensure (&sb, n);
      memcpy (sb.buf + sb.len, chunk, n);
      sb.len += n;
      sb.buf[sb.len] = '\0';
    }
  எனில் (ferror (f))
    {
      விடுவி (sb.buf);
      libcrun_make_error (err, errno, "cannot read `%s`", path);
      திரும்பு வெற்று;
    }

  எனில் (sb.buf == வெற்று)
    sb.buf = xstrdup ("");
  திரும்பு sb.buf;
}

/* Build the OCI Process JSON from the CLI options.  */
நிலையான எழுத்து *
build_process_json (கட்டமைப்பு crun_global_arguments *global_args arg_unused, எழுத்து **args, size_t args_len)
{
  கட்டமைப்பு strbuf sb = { 0 };

  sb_putc (&sb, '{');

  sb_puts (&sb, "\"args\":");
  sb_put_json_string_array (&sb, args, args_len);

  /* cwd is a required field in the OCI process schema; the exec path treats a
     missing value as "/".  */
  sb_puts (&sb, ",\"cwd\":");
  sb_put_json_string (&sb, exec_options.cwd ? exec_options.cwd : "/");

  sb_puts (&sb, ",\"terminal\":");
  sb_puts (&sb, exec_options.tty ? "true" : "false");

  எனில் (exec_options.env_size > 0)
    {
      sb_puts (&sb, ",\"env\":");
      sb_put_json_string_array (&sb, exec_options.env, exec_options.env_size);
    }

  எனில் (exec_options.user)
    {
      எழுத்து *endptr = வெற்று;
      எழுத்து *gidstr;
      முழு uid, gid = 0;
      எழுத்து tmp[32];

      uid = parse_id_or_fail (exec_options.user, &endptr, "UID");
      எனில் (*endptr == ':')
        {
          gidstr = endptr + 1;
          gid = parse_id_or_fail (gidstr, &endptr, "GID");
          எனில் (*endptr != '\0')
            libcrun_fail_with_error (0, "invalid USERSPEC specified");
        }
      இல்லையெனில் எனில் (*endptr != '\0')
        libcrun_fail_with_error (0, "invalid USERSPEC specified");

      sb_puts (&sb, ",\"user\":{\"uid\":");
      snprintf (tmp, அளவு (tmp), "%d", uid);
      sb_puts (&sb, tmp);
      sb_puts (&sb, ",\"gid\":");
      snprintf (tmp, அளவு (tmp), "%d", gid);
      sb_puts (&sb, tmp);
      sb_putc (&sb, '}');
    }

  எனில் (exec_options.process_label != வெற்று)
    {
      sb_puts (&sb, ",\"selinuxLabel\":");
      sb_put_json_string (&sb, exec_options.process_label);
    }

  எனில் (exec_options.apparmor != வெற்று)
    {
      sb_puts (&sb, ",\"apparmorProfile\":");
      sb_put_json_string (&sb, exec_options.apparmor);
    }

  எனில் (exec_options.cap_size > 0)
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
  எனில் (exec_options.no_new_privs)
    sb_puts (&sb, ",\"noNewPrivileges\":true");

  sb_putc (&sb, '}');

  திரும்பு sb.buf;
}

முழு
crun_command_exec (கட்டமைப்பு crun_global_arguments *global_args, முழு argc, எழுத்து **argv, libcrun_error_t *err)
{
  முழு first_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = வெற்று;
  cleanup_free எழுத்து *process_json = வெற்று;
  கட்டமைப்பு libcrun_exec_options_s exec_opts;

  memset (&exec_opts, 0, அளவு (exec_opts));
  exec_opts.struct_size = அளவு (exec_opts);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &exec_options);
  crun_assert_n_args (argc - first_arg, exec_options.process ? 1 : 2, -1);

  crun_context = new_libcrun_context (global_args);

  libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds);

  எனில் (getenv ("LISTEN_FDS"))
    {
      முழு listen_fds = strtoll (getenv ("LISTEN_FDS"), வெற்று, 10);
      libcrun_context_set_listen_fds (crun_context, listen_fds);
      libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds + listen_fds);
    }

  /* The process file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  எனில் (exec_options.process)
    {
      process_json = read_file_to_string (exec_options.process, err);
      எனில் (process_json == வெற்று)
        திரும்பு -1;
      exec_opts.merge_env = false;
    }
  இல்லையெனில்
    {
      cleanup_free எழுத்து **args = வெற்று;
      size_t args_len = argc - first_arg - 1;
      size_t i;

      /* argv[first_arg] is the container id, the command follows it.  */
      args = xmalloc0 ((args_len + 1) * அளவு (*args));
      க்கு (i = 0; i < args_len; i++)
        args[i] = argv[first_arg + i + 1];
      args[i] = வெற்று;

      process_json = build_process_json (global_args, args, args_len);
      exec_opts.merge_env = true;
    }

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  libcrun_context_set_detach (crun_context, exec_options.detach);
  libcrun_context_set_console_socket (crun_context, exec_options.console_socket);
  libcrun_context_set_pid_file (crun_context, exec_options.pid_file);

  exec_opts.process_json = process_json;
  exec_opts.cgroup = exec_options.cgroup;

  திரும்பு libcrun_container_exec_with_options_json (crun_context, argv[first_arg], &exec_opts, err);
}
