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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <argp.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <errno.h>
#అన్తర్భావ <limits.h>

#అన్తర్భావ "crun.h"

స్థిర అక్షరం doc[] = "OCI runtime";

నిర్మాణం exec_options_s
{
  bool tty;
  bool detach;
  bool no_new_privs;
  పూర్ణాంకం preserve_fds;
  స్థిరరాశి అక్షరం *process;
  స్థిరరాశి అక్షరం *console_socket;
  స్థిరరాశి అక్షరం *pid_file;
  అక్షరం *process_label;
  అక్షరం *apparmor;
  అక్షరం *cwd;
  అక్షరం *user;
  అక్షరం **env;
  అక్షరం **cap;
  size_t cap_కొలత;
  size_t env_కొలత;
  అక్షరం *cgroup;
};

గణన
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

స్థిర నిర్మాణం exec_options_s exec_options;

స్థిర నిర్మాణం argp_option options[]
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

స్థిర అక్షరం వాదనలు_doc[] = "exec CONTAINER cmd";

స్థిర శూన్య
append_గమ్యం_string_array (అక్షరం ***arr, size_t *కొలత, స్థిరరాశి అక్షరం *arg)
{
  *arr = xrealloc (*arr, (*కొలత + 2) * పరిమాణం (**arr));
  (*arr)[*కొలత + 1] = ఖాళీ;
  (*arr)[*కొలత] = xstrdup (arg);
  (*కొలత)++;
}

స్థిర శూన్య
append_env (స్థిరరాశి అక్షరం *arg)
{
  append_గమ్యం_string_array (&exec_options.env, &exec_options.env_కొలత, arg);
}

స్థిర శూన్య
append_cap (స్థిరరాశి అక్షరం *arg)
{
  append_గమ్యం_string_array (&exec_options.cap, &exec_options.cap_కొలత, arg);
}

స్థిర error_t
parse_opt (పూర్ణాంకం కీలకం, అక్షరం *arg, నిర్మాణం argp_state *state)
{
  ఎంపిక (కీలకం)
    {
    స్థితి OPTION_CONSOLE_SOCKET:
      exec_options.console_socket = arg;
      ఆపు;

    స్థితి OPTION_PID_FILE:
      exec_options.pid_file = arg;
      ఆపు;

    స్థితి OPTION_NO_NEW_PRIVS:
      exec_options.no_new_privs = true;
      ఆపు;

    స్థితి OPTION_PROCESS_LABEL:
      exec_options.process_label = argp_mandatory_వాదం (arg, state);
      ఆపు;

    స్థితి OPTION_APPARMOR:
      exec_options.apparmor = argp_mandatory_వాదం (arg, state);
      ఆపు;

    స్థితి OPTION_PRESERVE_FDS:
      exec_options.preserve_fds = parse_id_or_fail (argp_mandatory_వాదం (arg, state), ఖాళీ, "preserve-fds");
      ఆపు;

    స్థితి OPTION_CGROUP:
      exec_options.cgroup = argp_mandatory_వాదం (arg, state);
      ఆపు;

    స్థితి 'd':
      exec_options.detach = true;
      ఆపు;

    స్థితి 'p':
      exec_options.process = arg;
      ఆపు;

    స్థితి 't':
      exec_options.tty = arg == ఖాళీ || (strcmp (arg, "false") != 0 && strcmp (arg, "no") != 0);
      ఆపు;

    స్థితి 'u':
      exec_options.user = arg;
      ఆపు;

    స్థితి 'e':
      append_env (arg);
      ఆపు;

    స్థితి 'c':
      append_cap (arg);
      ఆపు;

    స్థితి OPTION_CWD:
      exec_options.cwd = xstrdup (arg);
      ఆపు;

    స్థితి ARGP_కీలకం_NO_వాదనలు:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    అప్రమేయ:
      తిరిగి ARGP_ERR_UNKNOWN;
    }

  తిరిగి 0;
}

స్థిర నిర్మాణం argp నిరంతరం_argp = { options, parse_opt, వాదనలు_doc, doc, ఖాళీ, ఖాళీ, ఖాళీ };

/* Minimal growable string buffer used to assemble the OCI Process JSON.  The
   binary is a pure consumer of the public API, so it builds the process as a
   JSON string rather than reaching for the internal ocispec types.  */
నిర్మాణం strbuf
{
  అక్షరం *buf;
  size_t len;
  size_t cap;
};

స్థిర శూన్య
sb_ensure (నిర్మాణం strbuf *sb, size_t extra)
{
  ఒకవేళ (sb->len + extra + 1 > sb->cap)
    {
      size_t ncap = sb->cap ? sb->cap : 256;
      అయితే (ncap < sb->len + extra + 1)
        ncap *= 2;
      sb->buf = xrealloc (sb->buf, ncap);
      sb->cap = ncap;
    }
}

స్థిర శూన్య
sb_puts (నిర్మాణం strbuf *sb, స్థిరరాశి అక్షరం *s)
{
  size_t l = అక్షరశ్రేణి_పొడవు (s);
  sb_ensure (sb, l);
  జ్ఞాపకం_నకలు (sb->buf + sb->len, s, l);
  sb->len += l;
  sb->buf[sb->len] = '\0';
}

స్థిర శూన్య
sb_putc (నిర్మాణం strbuf *sb, అక్షరం c)
{
  sb_ensure (sb, 1);
  sb->buf[sb->len++] = c;
  sb->buf[sb->len] = '\0';
}

/* Append S as a quoted, escaped JSON string.  */
స్థిర శూన్య
sb_put_json_string (నిర్మాణం strbuf *sb, స్థిరరాశి అక్షరం *s)
{
  sb_putc (sb, '"');
  కొరకు (; *s; s++)
    {
      గుర్తులేని అక్షరం c = (గుర్తులేని అక్షరం) *s;
      ఎంపిక (c)
        {
        స్థితి '"':
          sb_puts (sb, "\\\"");
          ఆపు;
        స్థితి '\\':
          sb_puts (sb, "\\\\");
          ఆపు;
        స్థితి '\b':
          sb_puts (sb, "\\b");
          ఆపు;
        స్థితి '\f':
          sb_puts (sb, "\\f");
          ఆపు;
        స్థితి '\n':
          sb_puts (sb, "\\n");
          ఆపు;
        స్థితి '\r':
          sb_puts (sb, "\\r");
          ఆపు;
        స్థితి '\t':
          sb_puts (sb, "\\t");
          ఆపు;
        అప్రమేయ:
          ఒకవేళ (c < 0x20)
            {
              అక్షరం tmp[8];
              snprintf (tmp, పరిమాణం (tmp), "\\u%04x", c);
              sb_puts (sb, tmp);
            }
          లేకపోతే
            sb_putc (sb, c);
        }
    }
  sb_putc (sb, '"');
}

స్థిర శూన్య
sb_put_json_string_array (నిర్మాణం strbuf *sb, అక్షరం **arr, size_t len)
{
  size_t i;
  sb_putc (sb, '[');
  కొరకు (i = 0; i < len; i++)
    {
      ఒకవేళ (i)
        sb_putc (sb, ',');
      sb_put_json_string (sb, arr[i]);
    }
  sb_putc (sb, ']');
}

/* Read the whole file at PATH into a NUL-terminated string.  A PATH of `-`
   reads from stdin.  */
స్థిర అక్షరం *
read_file_గమ్యం_string (స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err)
{
  cleanup_file FILE *f = ఖాళీ;
  నిర్మాణం strbuf sb = { 0 };
  అక్షరం chunk[4096];
  size_t n;

  ఒకవేళ (strcmp (మార్గం, "-") == 0)
    మార్గం = "/dev/stdin";

  f = fopen (మార్గం, "re");
  ఒకవేళ (f == ఖాళీ)
    {
      libcrun_make_error (err, errno, "cannot open `%s`", మార్గం);
      తిరిగి ఖాళీ;
    }

  అయితే ((n = fread (chunk, 1, పరిమాణం (chunk), f)) > 0)
    {
      sb_ensure (&sb, n);
      జ్ఞాపకం_నకలు (sb.buf + sb.len, chunk, n);
      sb.len += n;
      sb.buf[sb.len] = '\0';
    }
  ఒకవేళ (ferror (f))
    {
      విడుదల (sb.buf);
      libcrun_make_error (err, errno, "cannot read `%s`", మార్గం);
      తిరిగి ఖాళీ;
    }

  ఒకవేళ (sb.buf == ఖాళీ)
    sb.buf = xstrdup ("");
  తిరిగి sb.buf;
}

/* Build the OCI Process JSON from the CLI options.  */
స్థిర అక్షరం *
build_process_json (నిర్మాణం crun_global_arguments *global_వాదనలు arg_వాడని, అక్షరం **వాదనలు, size_t వాదనలు_len)
{
  నిర్మాణం strbuf sb = { 0 };

  sb_putc (&sb, '{');

  sb_puts (&sb, "\"args\":");
  sb_put_json_string_array (&sb, వాదనలు, వాదనలు_len);

  /* cwd is a required field in the OCI process schema; the exec path treats a
     missing value as "/".  */
  sb_puts (&sb, ",\"cwd\":");
  sb_put_json_string (&sb, exec_options.cwd ? exec_options.cwd : "/");

  sb_puts (&sb, ",\"terminal\":");
  sb_puts (&sb, exec_options.tty ? "true" : "false");

  ఒకవేళ (exec_options.env_కొలత > 0)
    {
      sb_puts (&sb, ",\"env\":");
      sb_put_json_string_array (&sb, exec_options.env, exec_options.env_కొలత);
    }

  ఒకవేళ (exec_options.user)
    {
      అక్షరం *endptr = ఖాళీ;
      అక్షరం *gidstr;
      పూర్ణాంకం uid, gid = 0;
      అక్షరం tmp[32];

      uid = parse_id_or_fail (exec_options.user, &endptr, "UID");
      ఒకవేళ (*endptr == ':')
        {
          gidstr = endptr + 1;
          gid = parse_id_or_fail (gidstr, &endptr, "GID");
          ఒకవేళ (*endptr != '\0')
            libcrun_fail_with_error (0, "invalid USERSPEC specified");
        }
      లేకపోతే ఒకవేళ (*endptr != '\0')
        libcrun_fail_with_error (0, "invalid USERSPEC specified");

      sb_puts (&sb, ",\"user\":{\"uid\":");
      snprintf (tmp, పరిమాణం (tmp), "%d", uid);
      sb_puts (&sb, tmp);
      sb_puts (&sb, ",\"gid\":");
      snprintf (tmp, పరిమాణం (tmp), "%d", gid);
      sb_puts (&sb, tmp);
      sb_putc (&sb, '}');
    }

  ఒకవేళ (exec_options.process_label != ఖాళీ)
    {
      sb_puts (&sb, ",\"selinuxLabel\":");
      sb_put_json_string (&sb, exec_options.process_label);
    }

  ఒకవేళ (exec_options.apparmor != ఖాళీ)
    {
      sb_puts (&sb, ",\"apparmorProfile\":");
      sb_put_json_string (&sb, exec_options.apparmor);
    }

  ఒకవేళ (exec_options.cap_కొలత > 0)
    {
      sb_puts (&sb, ",\"capabilities\":{");
      sb_puts (&sb, "\"effective\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_కొలత);
      sb_puts (&sb, ",\"bounding\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_కొలత);
      sb_puts (&sb, ",\"ambient\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_కొలత);
      sb_puts (&sb, ",\"permitted\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_కొలత);
      sb_putc (&sb, '}');
    }

  /* noNewPrivileges remains at the config default unless explicitly requested.  */
  ఒకవేళ (exec_options.no_new_privs)
    sb_puts (&sb, ",\"noNewPrivileges\":true");

  sb_putc (&sb, '}');

  తిరిగి sb.buf;
}

పూర్ణాంకం
crun_command_exec (నిర్మాణం crun_global_arguments *global_వాదనలు, పూర్ణాంకం argc, అక్షరం **argv, libcrun_error_t *err)
{
  పూర్ణాంకం మొదటి_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = ఖాళీ;
  cleanup_free అక్షరం *process_json = ఖాళీ;
  నిర్మాణం libcrun_exec_options_s exec_opts;

  memset (&exec_opts, 0, పరిమాణం (exec_opts));
  exec_opts.struct_కొలత = పరిమాణం (exec_opts);

  argp_parse (&నిరంతరం_argp, argc, argv, ARGP_లోపల_క్రమం, &మొదటి_arg, &exec_options);
  crun_assert_n_వాదనలు (argc - మొదటి_arg, exec_options.process ? 1 : 2, -1);

  crun_context = new_libcrun_context (global_వాదనలు);

  libcrun_context_సమితి_preserve_fds (crun_context, exec_options.preserve_fds);

  ఒకవేళ (getenv ("LISTEN_FDS"))
    {
      పూర్ణాంకం listen_fds = strtoll (getenv ("LISTEN_FDS"), ఖాళీ, 10);
      libcrun_context_సమితి_listen_fds (crun_context, listen_fds);
      libcrun_context_సమితి_preserve_fds (crun_context, exec_options.preserve_fds + listen_fds);
    }

  /* The process file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  ఒకవేళ (exec_options.process)
    {
      process_json = read_file_గమ్యం_string (exec_options.process, err);
      ఒకవేళ (process_json == ఖాళీ)
        తిరిగి -1;
      exec_opts.విలీనం_env = false;
    }
  లేకపోతే
    {
      cleanup_free అక్షరం **వాదనలు = ఖాళీ;
      size_t వాదనలు_len = argc - మొదటి_arg - 1;
      size_t i;

      /* argv[first_arg] is the container id, the command follows it.  */
      వాదనలు = xmalloc0 ((వాదనలు_len + 1) * పరిమాణం (*వాదనలు));
      కొరకు (i = 0; i < వాదనలు_len; i++)
        వాదనలు[i] = argv[మొదటి_arg + i + 1];
      వాదనలు[i] = ఖాళీ;

      process_json = build_process_json (global_వాదనలు, వాదనలు, వాదనలు_len);
      exec_opts.విలీనం_env = true;
    }

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = init_libcrun_context (crun_context, argv[మొదటి_arg], global_వాదనలు, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  libcrun_context_సమితి_detach (crun_context, exec_options.detach);
  libcrun_context_సమితి_console_socket (crun_context, exec_options.console_socket);
  libcrun_context_సమితి_pid_file (crun_context, exec_options.pid_file);

  exec_opts.process_json = process_json;
  exec_opts.cgroup = exec_options.cgroup;

  తిరిగి libcrun_container_exec_with_options_json (crun_context, argv[మొదటి_arg], &exec_opts, err);
}
