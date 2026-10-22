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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <argp.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <limits.h>

#ಅನ್ತರ್ಭಾವ "crun.h"

ಸ್ಥಿರ ಅಕ್ಷರ doc[] = "OCI runtime";

ರಚನೆ exec_options_s
{
  bool tty;
  bool detach;
  bool no_new_privs;
  ಪೂರ್ಣಾಂಕ preserve_fds;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *process;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *console_socket;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pid_file;
  ಅಕ್ಷರ *process_label;
  ಅಕ್ಷರ *apparmor;
  ಅಕ್ಷರ *cwd;
  ಅಕ್ಷರ *user;
  ಅಕ್ಷರ **env;
  ಅಕ್ಷರ **cap;
  size_t cap_ಗಾತ್ರ;
  size_t env_ಗಾತ್ರ;
  ಅಕ್ಷರ *cgroup;
};

ಎಣಿಕೆ
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

ಸ್ಥಿರ ರಚನೆ exec_options_s exec_options;

ಸ್ಥಿರ ರಚನೆ argp_option options[]
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

ಸ್ಥಿರ ಅಕ್ಷರ ವಾದಗಳು_doc[] = "exec CONTAINER cmd";

ಸ್ಥಿರ ಶೂನ್ಯ
append_ಗಮ್ಯ_string_array (ಅಕ್ಷರ ***arr, size_t *ಗಾತ್ರ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
  *arr = xrealloc (*arr, (*ಗಾತ್ರ_2 + 2) * ಗಾತ್ರ (**arr));
  (*arr)[*ಗಾತ್ರ_2 + 1] = ಖಾಲಿ;
  (*arr)[*ಗಾತ್ರ_2] = xstrdup (arg);
  (*ಗಾತ್ರ_2)++;
}

ಸ್ಥಿರ ಶೂನ್ಯ
append_env (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
  append_ಗಮ್ಯ_string_array (&exec_options.env, &exec_options.env_ಗಾತ್ರ, arg);
}

ಸ್ಥಿರ ಶೂನ್ಯ
append_cap (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
  append_ಗಮ್ಯ_string_array (&exec_options.cap, &exec_options.cap_ಗಾತ್ರ, arg);
}

ಸ್ಥಿರ error_t
parse_opt (ಪೂರ್ಣಾಂಕ ಕೀಲಿ, ಅಕ್ಷರ *arg, ರಚನೆ argp_state *state)
{
  ಆಯ್ಕೆ (ಕೀಲಿ)
    {
    ಸ್ಥಿತಿ OPTION_CONSOLE_SOCKET:
      exec_options.console_socket = arg;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_PID_FILE:
      exec_options.pid_file = arg;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_NO_NEW_PRIVS:
      exec_options.no_new_privs = true;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_PROCESS_LABEL:
      exec_options.process_label = argp_mandatory_ವಾದ (arg, state);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_APPARMOR:
      exec_options.apparmor = argp_mandatory_ವಾದ (arg, state);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_PRESERVE_FDS:
      exec_options.preserve_fds = parse_id_or_fail (argp_mandatory_ವಾದ (arg, state), ಖಾಲಿ, "preserve-fds");
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_CGROUP:
      exec_options.cgroup = argp_mandatory_ವಾದ (arg, state);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'd':
      exec_options.detach = true;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'p':
      exec_options.process = arg;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 't':
      exec_options.tty = arg == ಖಾಲಿ || (strcmp (arg, "false") != 0 && strcmp (arg, "no") != 0);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'u':
      exec_options.user = arg;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'e':
      append_env (arg);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'c':
      append_cap (arg);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_CWD:
      exec_options.cwd = xstrdup (arg);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ARGP_ಕೀಲಿ_NO_ವಾದಗಳು:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ ARGP_ERR_UNKNOWN;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ರಚನೆ argp ಸತತ_argp = { options, parse_opt, ವಾದಗಳು_doc, doc, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ };

/* Minimal growable string buffer used to assemble the OCI Process JSON.  The
   binary is a pure consumer of the public API, so it builds the process as a
   JSON string rather than reaching for the internal ocispec types.  */
ರಚನೆ strbuf
{
  ಅಕ್ಷರ *buf;
  size_t len;
  size_t cap;
};

ಸ್ಥಿರ ಶೂನ್ಯ
sb_ensure (ರಚನೆ strbuf *sb, size_t extra)
{
  ಆದರೆ (sb->len + extra + 1 > sb->cap)
    {
      size_t ncap = sb->cap ? sb->cap : 256;
      ಆಗ (ncap < sb->len + extra + 1)
        ncap *= 2;
      sb->buf = xrealloc (sb->buf, ncap);
      sb->cap = ncap;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
sb_puts (ರಚನೆ strbuf *sb, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
  size_t l = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (s);
  sb_ensure (sb, l);
  ಸ್ಮರಣೆ_ನಕಲು (sb->buf + sb->len, s, l);
  sb->len += l;
  sb->buf[sb->len] = '\0';
}

ಸ್ಥಿರ ಶೂನ್ಯ
sb_putc (ರಚನೆ strbuf *sb, ಅಕ್ಷರ c)
{
  sb_ensure (sb, 1);
  sb->buf[sb->len++] = c;
  sb->buf[sb->len] = '\0';
}

/* Append S as a quoted, escaped JSON string.  */
ಸ್ಥಿರ ಶೂನ್ಯ
sb_put_json_string (ರಚನೆ strbuf *sb, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
  sb_putc (sb, '"');
  ಗಾಗಿ (; *s; s++)
    {
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) *s;
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ '"':
          sb_puts (sb, "\\\"");
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ '\\':
          sb_puts (sb, "\\\\");
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ '\b':
          sb_puts (sb, "\\b");
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ '\f':
          sb_puts (sb, "\\f");
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ '\n':
          sb_puts (sb, "\\n");
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ '\r':
          sb_puts (sb, "\\r");
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ '\t':
          sb_puts (sb, "\\t");
          ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
          ಆದರೆ (c < 0x20)
            {
              ಅಕ್ಷರ tmp[8];
              snprintf (tmp, ಗಾತ್ರ (tmp), "\\u%04x", c);
              sb_puts (sb, tmp);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            sb_putc (sb, c);
        }
    }
  sb_putc (sb, '"');
}

ಸ್ಥಿರ ಶೂನ್ಯ
sb_put_json_string_array (ರಚನೆ strbuf *sb, ಅಕ್ಷರ **arr, size_t len)
{
  size_t i;
  sb_putc (sb, '[');
  ಗಾಗಿ (i = 0; i < len; i++)
    {
      ಆದರೆ (i)
        sb_putc (sb, ',');
      sb_put_json_string (sb, arr[i]);
    }
  sb_putc (sb, ']');
}

/* Read the whole file at PATH into a NUL-terminated string.  A PATH of `-`
   reads from stdin.  */
ಸ್ಥಿರ ಅಕ್ಷರ *
read_file_ಗಮ್ಯ_string (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err)
{
  cleanup_file FILE *f = ಖಾಲಿ;
  ರಚನೆ strbuf sb = { 0 };
  ಅಕ್ಷರ chunk[4096];
  size_t n;

  ಆದರೆ (strcmp (ಮಾರ್ಗ, "-") == 0)
    ಮಾರ್ಗ = "/dev/stdin";

  f = fopen (ಮಾರ್ಗ, "re");
  ಆದರೆ (f == ಖಾಲಿ)
    {
      libcrun_make_error (err, errno, "cannot open `%s`", ಮಾರ್ಗ);
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

  ಆಗ ((n = fread (chunk, 1, ಗಾತ್ರ (chunk), f)) > 0)
    {
      sb_ensure (&sb, n);
      ಸ್ಮರಣೆ_ನಕಲು (sb.buf + sb.len, chunk, n);
      sb.len += n;
      sb.buf[sb.len] = '\0';
    }
  ಆದರೆ (ferror (f))
    {
      ಬಿಡುಗಡೆ (sb.buf);
      libcrun_make_error (err, errno, "cannot read `%s`", ಮಾರ್ಗ);
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

  ಆದರೆ (sb.buf == ಖಾಲಿ)
    sb.buf = xstrdup ("");
  ಹಿಂತಿರುಗಿ sb.buf;
}

/* Build the OCI Process JSON from the CLI options.  */
ಸ್ಥಿರ ಅಕ್ಷರ *
build_process_json (ರಚನೆ crun_global_arguments *global_ವಾದಗಳು arg_ಬಳಸದ, ಅಕ್ಷರ **ವಾದಗಳು, size_t ವಾದಗಳು_len)
{
  ರಚನೆ strbuf sb = { 0 };

  sb_putc (&sb, '{');

  sb_puts (&sb, "\"args\":");
  sb_put_json_string_array (&sb, ವಾದಗಳು, ವಾದಗಳು_len);

  /* cwd is a required field in the OCI process schema; the exec path treats a
     missing value as "/".  */
  sb_puts (&sb, ",\"cwd\":");
  sb_put_json_string (&sb, exec_options.cwd ? exec_options.cwd : "/");

  sb_puts (&sb, ",\"terminal\":");
  sb_puts (&sb, exec_options.tty ? "true" : "false");

  ಆದರೆ (exec_options.env_ಗಾತ್ರ > 0)
    {
      sb_puts (&sb, ",\"env\":");
      sb_put_json_string_array (&sb, exec_options.env, exec_options.env_ಗಾತ್ರ);
    }

  ಆದರೆ (exec_options.user)
    {
      ಅಕ್ಷರ *endptr = ಖಾಲಿ;
      ಅಕ್ಷರ *gidstr;
      ಪೂರ್ಣಾಂಕ uid, gid = 0;
      ಅಕ್ಷರ tmp[32];

      uid = parse_id_or_fail (exec_options.user, &endptr, "UID");
      ಆದರೆ (*endptr == ':')
        {
          gidstr = endptr + 1;
          gid = parse_id_or_fail (gidstr, &endptr, "GID");
          ಆದರೆ (*endptr != '\0')
            libcrun_fail_with_error (0, "invalid USERSPEC specified");
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*endptr != '\0')
        libcrun_fail_with_error (0, "invalid USERSPEC specified");

      sb_puts (&sb, ",\"user\":{\"uid\":");
      snprintf (tmp, ಗಾತ್ರ (tmp), "%d", uid);
      sb_puts (&sb, tmp);
      sb_puts (&sb, ",\"gid\":");
      snprintf (tmp, ಗಾತ್ರ (tmp), "%d", gid);
      sb_puts (&sb, tmp);
      sb_putc (&sb, '}');
    }

  ಆದರೆ (exec_options.process_label != ಖಾಲಿ)
    {
      sb_puts (&sb, ",\"selinuxLabel\":");
      sb_put_json_string (&sb, exec_options.process_label);
    }

  ಆದರೆ (exec_options.apparmor != ಖಾಲಿ)
    {
      sb_puts (&sb, ",\"apparmorProfile\":");
      sb_put_json_string (&sb, exec_options.apparmor);
    }

  ಆದರೆ (exec_options.cap_ಗಾತ್ರ > 0)
    {
      sb_puts (&sb, ",\"capabilities\":{");
      sb_puts (&sb, "\"effective\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_ಗಾತ್ರ);
      sb_puts (&sb, ",\"bounding\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_ಗಾತ್ರ);
      sb_puts (&sb, ",\"ambient\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_ಗಾತ್ರ);
      sb_puts (&sb, ",\"permitted\":");
      sb_put_json_string_array (&sb, exec_options.cap, exec_options.cap_ಗಾತ್ರ);
      sb_putc (&sb, '}');
    }

  /* noNewPrivileges remains at the config default unless explicitly requested.  */
  ಆದರೆ (exec_options.no_new_privs)
    sb_puts (&sb, ",\"noNewPrivileges\":true");

  sb_putc (&sb, '}');

  ಹಿಂತಿರುಗಿ sb.buf;
}

ಪೂರ್ಣಾಂಕ
crun_command_exec (ರಚನೆ crun_global_arguments *global_ವಾದಗಳು, ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ಮೊದಲ_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *process_json = ಖಾಲಿ;
  ರಚನೆ libcrun_exec_options_s exec_opts;

  memset (&exec_opts, 0, ಗಾತ್ರ (exec_opts));
  exec_opts.struct_ಗಾತ್ರ = ಗಾತ್ರ (exec_opts);

  argp_parse (&ಸತತ_argp, argc, argv, ARGP_ಒಳ_ಕ್ರಮ, &ಮೊದಲ_arg, &exec_options);
  crun_assert_n_ವಾದಗಳು (argc - ಮೊದಲ_arg, exec_options.process ? 1 : 2, -1);

  crun_context = new_libcrun_context (global_ವಾದಗಳು);

  libcrun_context_ಗಣ_preserve_fds (crun_context, exec_options.preserve_fds);

  ಆದರೆ (getenv ("LISTEN_FDS"))
    {
      ಪೂರ್ಣಾಂಕ listen_fds = strtoll (getenv ("LISTEN_FDS"), ಖಾಲಿ, 10);
      libcrun_context_ಗಣ_listen_fds (crun_context, listen_fds);
      libcrun_context_ಗಣ_preserve_fds (crun_context, exec_options.preserve_fds + listen_fds);
    }

  /* The process file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  ಆದರೆ (exec_options.process)
    {
      process_json = read_file_ಗಮ್ಯ_string (exec_options.process, err);
      ಆದರೆ (process_json == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ -1;
      exec_opts.ವಿಲೀನ_env = false;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      cleanup_free ಅಕ್ಷರ **ವಾದಗಳು = ಖಾಲಿ;
      size_t ವಾದಗಳು_len = argc - ಮೊದಲ_arg - 1;
      size_t i;

      /* argv[first_arg] is the container id, the command follows it.  */
      ವಾದಗಳು = xmalloc0 ((ವಾದಗಳು_len + 1) * ಗಾತ್ರ (*ವಾದಗಳು));
      ಗಾಗಿ (i = 0; i < ವಾದಗಳು_len; i++)
        ವಾದಗಳು[i] = argv[ಮೊದಲ_arg + i + 1];
      ವಾದಗಳು[i] = ಖಾಲಿ;

      process_json = build_process_json (global_ವಾದಗಳು, ವಾದಗಳು, ವಾದಗಳು_len);
      exec_opts.ವಿಲೀನ_env = true;
    }

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = init_libcrun_context (crun_context, argv[ಮೊದಲ_arg], global_ವಾದಗಳು, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  libcrun_context_ಗಣ_detach (crun_context, exec_options.detach);
  libcrun_context_ಗಣ_console_socket (crun_context, exec_options.console_socket);
  libcrun_context_ಗಣ_pid_file (crun_context, exec_options.pid_file);

  exec_opts.process_json = process_json;
  exec_opts.cgroup = exec_options.cgroup;

  ಹಿಂತಿರುಗಿ libcrun_container_exec_with_options_json (crun_context, argv[ಮೊದಲ_arg], &exec_opts, err);
}
