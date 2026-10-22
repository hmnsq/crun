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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <argp.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <limits.h>

#ଅନ୍ତର୍ଭାଵ "crun.h"

ସ୍ଥିର ଅକ୍ଷର doc[] = "OCI runtime";

ସଂରଚନା exec_options_s
{
  bool tty;
  bool detach;
  bool no_new_privs;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା preserve_fds;
  ଅଚଳ ଅକ୍ଷର *process;
  ଅଚଳ ଅକ୍ଷର *console_socket;
  ଅଚଳ ଅକ୍ଷର *pid_file;
  ଅକ୍ଷର *process_label;
  ଅକ୍ଷର *apparmor;
  ଅକ୍ଷର *cwd;
  ଅକ୍ଷର *user;
  ଅକ୍ଷର **env;
  ଅକ୍ଷର **cap;
  size_t cap_size;
  size_t env_size;
  ଅକ୍ଷର *cgroup;
};

ଗଣନା
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

ସ୍ଥିର ସଂରଚନା exec_options_s exec_options;

ସ୍ଥିର ସଂରଚନା argp_option options[]
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

ସ୍ଥିର ଅକ୍ଷର args_doc[] = "exec CONTAINER cmd";

ସ୍ଥିର ଶୂନ୍ୟ
append_to_string_array (ଅକ୍ଷର ***arr, size_t *size, ଅଚଳ ଅକ୍ଷର *arg)
{
  *arr = xrealloc (*arr, (*size + 2) * ଆକାର (**arr));
  (*arr)[*size + 1] = ଖାଲି;
  (*arr)[*size] = xstrdup (arg);
  (*size)++;
}

ସ୍ଥିର ଶୂନ୍ୟ
append_env (ଅଚଳ ଅକ୍ଷର *arg)
{
  append_to_string_array (&exec_options.env, &exec_options.env_size, arg);
}

ସ୍ଥିର ଶୂନ୍ୟ
append_cap (ଅଚଳ ଅକ୍ଷର *arg)
{
  append_to_string_array (&exec_options.cap, &exec_options.cap_size, arg);
}

ସ୍ଥିର error_t
parse_opt (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଚାବି, ଅକ୍ଷର *arg, ସଂରଚନା argp_state *state)
{
  ବାଛ (ଚାବି)
    {
    ସ୍ଥିତି OPTION_CONSOLE_SOCKET:
      exec_options.console_socket = arg;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_PID_FILE:
      exec_options.pid_file = arg;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_NO_NEW_PRIVS:
      exec_options.no_new_privs = true;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_PROCESS_LABEL:
      exec_options.process_label = argp_mandatory_argument (arg, state);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_APPARMOR:
      exec_options.apparmor = argp_mandatory_argument (arg, state);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_PRESERVE_FDS:
      exec_options.preserve_fds = parse_id_or_fail (argp_mandatory_argument (arg, state), ଖାଲି, "preserve-fds");
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_CGROUP:
      exec_options.cgroup = argp_mandatory_argument (arg, state);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 'd':
      exec_options.detach = true;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 'p':
      exec_options.process = arg;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 't':
      exec_options.tty = arg == ଖାଲି || (strcmp (arg, "false") != 0 && strcmp (arg, "no") != 0);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 'u':
      exec_options.user = arg;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 'e':
      append_env (arg);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 'c':
      append_cap (arg);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_CWD:
      exec_options.cwd = xstrdup (arg);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି ARGP_ଚାବି_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ ARGP_ERR_UNKNOWN;
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ସଂରଚନା argp run_argp = { options, parse_opt, args_doc, doc, ଖାଲି, ଖାଲି, ଖାଲି };

/* Minimal growable string buffer used to assemble the OCI Process JSON.  The
   binary is a pure consumer of the public API, so it builds the process as a
   JSON string rather than reaching for the internal ocispec types.  */
ସଂରଚନା strbuf
{
  ଅକ୍ଷର *buf;
  size_t len;
  size_t cap;
};

ସ୍ଥିର ଶୂନ୍ୟ
sb_ensure (ସଂରଚନା strbuf *sb, size_t extra)
{
  ଯଦି (sb->len + extra + 1 > sb->cap)
    {
      size_t ncap = sb->cap ? sb->cap : 256;
      ଯେତେବେଳେ (ncap < sb->len + extra + 1)
        ncap *= 2;
      sb->buf = xrealloc (sb->buf, ncap);
      sb->cap = ncap;
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
sb_puts (ସଂରଚନା strbuf *sb, ଅଚଳ ଅକ୍ଷର *s)
{
  size_t l = strlen (s);
  sb_ensure (sb, l);
  memcpy (sb->buf + sb->len, s, l);
  sb->len += l;
  sb->buf[sb->len] = '\0';
}

ସ୍ଥିର ଶୂନ୍ୟ
sb_putc (ସଂରଚନା strbuf *sb, ଅକ୍ଷର c)
{
  sb_ensure (sb, 1);
  sb->buf[sb->len++] = c;
  sb->buf[sb->len] = '\0';
}

/* Append S as a quoted, escaped JSON string.  */
ସ୍ଥିର ଶୂନ୍ୟ
sb_put_json_string (ସଂରଚନା strbuf *sb, ଅଚଳ ଅକ୍ଷର *s)
{
  sb_putc (sb, '"');
  ପାଇଁ (; *s; s++)
    {
      unsigned ଅକ୍ଷର c = (unsigned ଅକ୍ଷର) *s;
      ବାଛ (c)
        {
        ସ୍ଥିତି '"':
          sb_puts (sb, "\\\"");
          ଭାଙ୍ଗ;
        ସ୍ଥିତି '\\':
          sb_puts (sb, "\\\\");
          ଭାଙ୍ଗ;
        ସ୍ଥିତି '\b':
          sb_puts (sb, "\\b");
          ଭାଙ୍ଗ;
        ସ୍ଥିତି '\f':
          sb_puts (sb, "\\f");
          ଭାଙ୍ଗ;
        ସ୍ଥିତି '\n':
          sb_puts (sb, "\\n");
          ଭାଙ୍ଗ;
        ସ୍ଥିତି '\r':
          sb_puts (sb, "\\r");
          ଭାଙ୍ଗ;
        ସ୍ଥିତି '\t':
          sb_puts (sb, "\\t");
          ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          ଯଦି (c < 0x20)
            {
              ଅକ୍ଷର tmp[8];
              snprintf (tmp, ଆକାର (tmp), "\\u%04x", c);
              sb_puts (sb, tmp);
            }
          ଅନ୍ୟଥା
            sb_putc (sb, c);
        }
    }
  sb_putc (sb, '"');
}

ସ୍ଥିର ଶୂନ୍ୟ
sb_put_json_string_array (ସଂରଚନା strbuf *sb, ଅକ୍ଷର **arr, size_t len)
{
  size_t i;
  sb_putc (sb, '[');
  ପାଇଁ (i = 0; i < len; i++)
    {
      ଯଦି (i)
        sb_putc (sb, ',');
      sb_put_json_string (sb, arr[i]);
    }
  sb_putc (sb, ']');
}

/* Read the whole file at PATH into a NUL-terminated string.  A PATH of `-`
   reads from stdin.  */
ସ୍ଥିର ଅକ୍ଷର *
read_file_to_string (ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err)
{
  cleanup_file FILE *f = ଖାଲି;
  ସଂରଚନା strbuf sb = { 0 };
  ଅକ୍ଷର chunk[4096];
  size_t n;

  ଯଦି (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  f = fopen (path, "re");
  ଯଦି (f == ଖାଲି)
    {
      libcrun_make_error (err, errno, "cannot open `%s`", path);
      ଫେରନ୍ତୁ ଖାଲି;
    }

  ଯେତେବେଳେ ((n = fread (chunk, 1, ଆକାର (chunk), f)) > 0)
    {
      sb_ensure (&sb, n);
      memcpy (sb.buf + sb.len, chunk, n);
      sb.len += n;
      sb.buf[sb.len] = '\0';
    }
  ଯଦି (ferror (f))
    {
      ମୁକ୍ତ (sb.buf);
      libcrun_make_error (err, errno, "cannot read `%s`", path);
      ଫେରନ୍ତୁ ଖାଲି;
    }

  ଯଦି (sb.buf == ଖାଲି)
    sb.buf = xstrdup ("");
  ଫେରନ୍ତୁ sb.buf;
}

/* Build the OCI Process JSON from the CLI options.  */
ସ୍ଥିର ଅକ୍ଷର *
build_process_json (ସଂରଚନା crun_global_arguments *global_args arg_unused, ଅକ୍ଷର **args, size_t args_len)
{
  ସଂରଚନା strbuf sb = { 0 };

  sb_putc (&sb, '{');

  sb_puts (&sb, "\"args\":");
  sb_put_json_string_array (&sb, args, args_len);

  /* cwd is a required field in the OCI process schema; the exec path treats a
     missing value as "/".  */
  sb_puts (&sb, ",\"cwd\":");
  sb_put_json_string (&sb, exec_options.cwd ? exec_options.cwd : "/");

  sb_puts (&sb, ",\"terminal\":");
  sb_puts (&sb, exec_options.tty ? "true" : "false");

  ଯଦି (exec_options.env_size > 0)
    {
      sb_puts (&sb, ",\"env\":");
      sb_put_json_string_array (&sb, exec_options.env, exec_options.env_size);
    }

  ଯଦି (exec_options.user)
    {
      ଅକ୍ଷର *endptr = ଖାଲି;
      ଅକ୍ଷର *gidstr;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା uid, gid = 0;
      ଅକ୍ଷର tmp[32];

      uid = parse_id_or_fail (exec_options.user, &endptr, "UID");
      ଯଦି (*endptr == ':')
        {
          gidstr = endptr + 1;
          gid = parse_id_or_fail (gidstr, &endptr, "GID");
          ଯଦି (*endptr != '\0')
            libcrun_fail_with_error (0, "invalid USERSPEC specified");
        }
      ଅନ୍ୟଥା ଯଦି (*endptr != '\0')
        libcrun_fail_with_error (0, "invalid USERSPEC specified");

      sb_puts (&sb, ",\"user\":{\"uid\":");
      snprintf (tmp, ଆକାର (tmp), "%d", uid);
      sb_puts (&sb, tmp);
      sb_puts (&sb, ",\"gid\":");
      snprintf (tmp, ଆକାର (tmp), "%d", gid);
      sb_puts (&sb, tmp);
      sb_putc (&sb, '}');
    }

  ଯଦି (exec_options.process_label != ଖାଲି)
    {
      sb_puts (&sb, ",\"selinuxLabel\":");
      sb_put_json_string (&sb, exec_options.process_label);
    }

  ଯଦି (exec_options.apparmor != ଖାଲି)
    {
      sb_puts (&sb, ",\"apparmorProfile\":");
      sb_put_json_string (&sb, exec_options.apparmor);
    }

  ଯଦି (exec_options.cap_size > 0)
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
  ଯଦି (exec_options.no_new_privs)
    sb_puts (&sb, ",\"noNewPrivileges\":true");

  sb_putc (&sb, '}');

  ଫେରନ୍ତୁ sb.buf;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_command_exec (ସଂରଚନା crun_global_arguments *global_args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = ଖାଲି;
  cleanup_free ଅକ୍ଷର *process_json = ଖାଲି;
  ସଂରଚନା libcrun_exec_options_s exec_opts;

  memset (&exec_opts, 0, ଆକାର (exec_opts));
  exec_opts.struct_size = ଆକାର (exec_opts);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &exec_options);
  crun_assert_n_args (argc - first_arg, exec_options.process ? 1 : 2, -1);

  crun_context = new_libcrun_context (global_args);

  libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds);

  ଯଦି (getenv ("LISTEN_FDS"))
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା listen_fds = strtoll (getenv ("LISTEN_FDS"), ଖାଲି, 10);
      libcrun_context_set_listen_fds (crun_context, listen_fds);
      libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds + listen_fds);
    }

  /* The process file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  ଯଦି (exec_options.process)
    {
      process_json = read_file_to_string (exec_options.process, err);
      ଯଦି (process_json == ଖାଲି)
        ଫେରନ୍ତୁ -1;
      exec_opts.merge_env = false;
    }
  ଅନ୍ୟଥା
    {
      cleanup_free ଅକ୍ଷର **args = ଖାଲି;
      size_t args_len = argc - first_arg - 1;
      size_t i;

      /* argv[first_arg] is the container id, the command follows it.  */
      args = xmalloc0 ((args_len + 1) * ଆକାର (*args));
      ପାଇଁ (i = 0; i < args_len; i++)
        args[i] = argv[first_arg + i + 1];
      args[i] = ଖାଲି;

      process_json = build_process_json (global_args, args, args_len);
      exec_opts.merge_env = true;
    }

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  libcrun_context_set_detach (crun_context, exec_options.detach);
  libcrun_context_set_console_socket (crun_context, exec_options.console_socket);
  libcrun_context_set_pid_file (crun_context, exec_options.pid_file);

  exec_opts.process_json = process_json;
  exec_opts.cgroup = exec_options.cgroup;

  ଫେରନ୍ତୁ libcrun_container_exec_with_options_json (crun_context, argv[first_arg], &exec_opts, err);
}
