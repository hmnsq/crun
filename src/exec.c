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

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

#include "crun.h"

static var_na doc[] = "OCI runtime";

sa_nracanaa exec_options_s
{
  bool tty;
  bool detach;
  bool no_new_privs;
  puur_naa_nka preserve_fds;
  const var_na *process;
  const var_na *console_socket;
  const var_na *pid_file;
  var_na *process_label;
  var_na *apparmor;
  var_na *cwd;
  var_na *user;
  var_na **env;
  var_na **cap;
  size_t cap_size;
  size_t env_size;
  var_na *cgroup;
};

ga_nanaa
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

static sa_nracanaa exec_options_s exec_options;

static sa_nracanaa argp_option options[]
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

static var_na args_doc[] = "exec CONTAINER cmd";

static void
append_to_string_array (var_na ***arr, size_t *size, const var_na *arg)
{
  *arr = xrealloc (*arr, (*size + 2) * parimaa_na (**arr));
  (*arr)[*size + 1] = NULL;
  (*arr)[*size] = xstrdup (arg);
  (*size)++;
}

static void
append_env (const var_na *arg)
{
  append_to_string_array (&exec_options.env, &exec_options.env_size, arg);
}

static void
append_cap (const var_na *arg)
{
  append_to_string_array (&exec_options.cap, &exec_options.cap_size, arg);
}

static error_t
parse_opt (puur_naa_nka key, var_na *arg, sa_nracanaa argp_state *state)
{
  switch (key)
    {
    pak_sa OPTION_CONSOLE_SOCKET:
      exec_options.console_socket = arg;
      bha_nga;

    pak_sa OPTION_PID_FILE:
      exec_options.pid_file = arg;
      bha_nga;

    pak_sa OPTION_NO_NEW_PRIVS:
      exec_options.no_new_privs = true;
      bha_nga;

    pak_sa OPTION_PROCESS_LABEL:
      exec_options.process_label = argp_mandatory_argument (arg, state);
      bha_nga;

    pak_sa OPTION_APPARMOR:
      exec_options.apparmor = argp_mandatory_argument (arg, state);
      bha_nga;

    pak_sa OPTION_PRESERVE_FDS:
      exec_options.preserve_fds = parse_id_or_fail (argp_mandatory_argument (arg, state), NULL, "preserve-fds");
      bha_nga;

    pak_sa OPTION_CGROUP:
      exec_options.cgroup = argp_mandatory_argument (arg, state);
      bha_nga;

    pak_sa 'd':
      exec_options.detach = true;
      bha_nga;

    pak_sa 'p':
      exec_options.process = arg;
      bha_nga;

    pak_sa 't':
      exec_options.tty = arg == NULL || (strcmp (arg, "false") != 0 && strcmp (arg, "no") != 0);
      bha_nga;

    pak_sa 'u':
      exec_options.user = arg;
      bha_nga;

    pak_sa 'e':
      append_env (arg);
      bha_nga;

    pak_sa 'c':
      append_cap (arg);
      bha_nga;

    pak_sa OPTION_CWD:
      exec_options.cwd = xstrdup (arg);
      bha_nga;

    pak_sa ARGP_KEY_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    default:
      ᬯᬮᬶ ARGP_ERR_UNKNOWN;
    }

  ᬯᬮᬶ 0;
}

static sa_nracanaa argp run_argp = { options, parse_opt, args_doc, doc, NULL, NULL, NULL };

/* Minimal growable string buffer used to assemble the OCI Process JSON.  The
   binary is a pure consumer of the public API, so it builds the process as a
   JSON string rather than reaching for the internal ocispec types.  */
sa_nracanaa strbuf
{
  var_na *buf;
  size_t len;
  size_t cap;
};

static void
sb_ensure (sa_nracanaa strbuf *sb, size_t extra)
{
  ᬬᬾᬦ᭄ (sb->len + extra + 1 > sb->cap)
    {
      size_t ncap = sb->cap ? sb->cap : 256;
      ᬲᭂᬤᭂᬮ (ncap < sb->len + extra + 1)
        ncap *= 2;
      sb->buf = xrealloc (sb->buf, ncap);
      sb->cap = ncap;
    }
}

static void
sb_puts (sa_nracanaa strbuf *sb, const var_na *s)
{
  size_t l = strlen (s);
  sb_ensure (sb, l);
  memcpy (sb->buf + sb->len, s, l);
  sb->len += l;
  sb->buf[sb->len] = '\0';
}

static void
sb_putc (sa_nracanaa strbuf *sb, var_na c)
{
  sb_ensure (sb, 1);
  sb->buf[sb->len++] = c;
  sb->buf[sb->len] = '\0';
}

/* Append S as a quoted, escaped JSON string.  */
static void
sb_put_json_string (sa_nracanaa strbuf *sb, const var_na *s)
{
  sb_putc (sb, '"');
  for (; *s; s++)
    {
      unsigned var_na c = (unsigned var_na) *s;
      switch (c)
        {
        pak_sa '"':
          sb_puts (sb, "\\\"");
          bha_nga;
        pak_sa '\\':
          sb_puts (sb, "\\\\");
          bha_nga;
        pak_sa '\b':
          sb_puts (sb, "\\b");
          bha_nga;
        pak_sa '\f':
          sb_puts (sb, "\\f");
          bha_nga;
        pak_sa '\n':
          sb_puts (sb, "\\n");
          bha_nga;
        pak_sa '\r':
          sb_puts (sb, "\\r");
          bha_nga;
        pak_sa '\t':
          sb_puts (sb, "\\t");
          bha_nga;
        default:
          ᬬᬾᬦ᭄ (c < 0x20)
            {
              var_na tmp[8];
              snprintf (tmp, parimaa_na (tmp), "\\u%04x", c);
              sb_puts (sb, tmp);
            }
          ᬮᬶᬬ
            sb_putc (sb, c);
        }
    }
  sb_putc (sb, '"');
}

static void
sb_put_json_string_array (sa_nracanaa strbuf *sb, var_na **arr, size_t len)
{
  size_t i;
  sb_putc (sb, '[');
  for (i = 0; i < len; i++)
    {
      ᬬᬾᬦ᭄ (i)
        sb_putc (sb, ',');
      sb_put_json_string (sb, arr[i]);
    }
  sb_putc (sb, ']');
}

/* Read the whole file at PATH into a NUL-terminated string.  A PATH of `-`
   reads from stdin.  */
static var_na *
read_file_to_string (const var_na *path, libcrun_error_t *err)
{
  cleanup_file FILE *f = NULL;
  sa_nracanaa strbuf sb = { 0 };
  var_na chunk[4096];
  size_t n;

  ᬬᬾᬦ᭄ (strcmp (path, "-") == 0)
    path = "/dev/stdin";

  f = fopen (path, "re");
  ᬬᬾᬦ᭄ (f == NULL)
    {
      libcrun_make_error (err, errno, "cannot open `%s`", path);
      ᬯᬮᬶ NULL;
    }

  ᬲᭂᬤᭂᬮ ((n = fread (chunk, 1, parimaa_na (chunk), f)) > 0)
    {
      sb_ensure (&sb, n);
      memcpy (sb.buf + sb.len, chunk, n);
      sb.len += n;
      sb.buf[sb.len] = '\0';
    }
  ᬬᬾᬦ᭄ (ferror (f))
    {
      free (sb.buf);
      libcrun_make_error (err, errno, "cannot read `%s`", path);
      ᬯᬮᬶ NULL;
    }

  ᬬᬾᬦ᭄ (sb.buf == NULL)
    sb.buf = xstrdup ("");
  ᬯᬮᬶ sb.buf;
}

/* Build the OCI Process JSON from the CLI options.  */
static var_na *
build_process_json (sa_nracanaa crun_global_arguments *global_args arg_unused, var_na **args, size_t args_len)
{
  sa_nracanaa strbuf sb = { 0 };

  sb_putc (&sb, '{');

  sb_puts (&sb, "\"args\":");
  sb_put_json_string_array (&sb, args, args_len);

  /* cwd is a required field in the OCI process schema; the exec path treats a
     missing value as "/".  */
  sb_puts (&sb, ",\"cwd\":");
  sb_put_json_string (&sb, exec_options.cwd ? exec_options.cwd : "/");

  sb_puts (&sb, ",\"terminal\":");
  sb_puts (&sb, exec_options.tty ? "true" : "false");

  ᬬᬾᬦ᭄ (exec_options.env_size > 0)
    {
      sb_puts (&sb, ",\"env\":");
      sb_put_json_string_array (&sb, exec_options.env, exec_options.env_size);
    }

  ᬬᬾᬦ᭄ (exec_options.user)
    {
      var_na *endptr = NULL;
      var_na *gidstr;
      puur_naa_nka uid, gid = 0;
      var_na tmp[32];

      uid = parse_id_or_fail (exec_options.user, &endptr, "UID");
      ᬬᬾᬦ᭄ (*endptr == ':')
        {
          gidstr = endptr + 1;
          gid = parse_id_or_fail (gidstr, &endptr, "GID");
          ᬬᬾᬦ᭄ (*endptr != '\0')
            libcrun_fail_with_error (0, "invalid USERSPEC specified");
        }
      ᬮᬶᬬ ᬬᬾᬦ᭄ (*endptr != '\0')
        libcrun_fail_with_error (0, "invalid USERSPEC specified");

      sb_puts (&sb, ",\"user\":{\"uid\":");
      snprintf (tmp, parimaa_na (tmp), "%d", uid);
      sb_puts (&sb, tmp);
      sb_puts (&sb, ",\"gid\":");
      snprintf (tmp, parimaa_na (tmp), "%d", gid);
      sb_puts (&sb, tmp);
      sb_putc (&sb, '}');
    }

  ᬬᬾᬦ᭄ (exec_options.process_label != NULL)
    {
      sb_puts (&sb, ",\"selinuxLabel\":");
      sb_put_json_string (&sb, exec_options.process_label);
    }

  ᬬᬾᬦ᭄ (exec_options.apparmor != NULL)
    {
      sb_puts (&sb, ",\"apparmorProfile\":");
      sb_put_json_string (&sb, exec_options.apparmor);
    }

  ᬬᬾᬦ᭄ (exec_options.cap_size > 0)
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
  ᬬᬾᬦ᭄ (exec_options.no_new_privs)
    sb_puts (&sb, ",\"noNewPrivileges\":true");

  sb_putc (&sb, '}');

  ᬯᬮᬶ sb.buf;
}

puur_naa_nka
crun_command_exec (sa_nracanaa crun_global_arguments *global_args, puur_naa_nka argc, var_na **argv, libcrun_error_t *err)
{
  puur_naa_nka first_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = NULL;
  cleanup_free var_na *process_json = NULL;
  sa_nracanaa libcrun_exec_options_s exec_opts;

  memset (&exec_opts, 0, parimaa_na (exec_opts));
  exec_opts.struct_size = parimaa_na (exec_opts);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &exec_options);
  crun_assert_n_args (argc - first_arg, exec_options.process ? 1 : 2, -1);

  crun_context = new_libcrun_context (global_args);

  libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds);

  ᬬᬾᬦ᭄ (getenv ("LISTEN_FDS"))
    {
      puur_naa_nka listen_fds = strtoll (getenv ("LISTEN_FDS"), NULL, 10);
      libcrun_context_set_listen_fds (crun_context, listen_fds);
      libcrun_context_set_preserve_fds (crun_context, exec_options.preserve_fds + listen_fds);
    }

  /* The process file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  ᬬᬾᬦ᭄ (exec_options.process)
    {
      process_json = read_file_to_string (exec_options.process, err);
      ᬬᬾᬦ᭄ (process_json == NULL)
        ᬯᬮᬶ -1;
      exec_opts.merge_env = false;
    }
  ᬮᬶᬬ
    {
      cleanup_free var_na **args = NULL;
      size_t args_len = argc - first_arg - 1;
      size_t i;

      /* argv[first_arg] is the container id, the command follows it.  */
      args = xmalloc0 ((args_len + 1) * parimaa_na (*args));
      for (i = 0; i < args_len; i++)
        args[i] = argv[first_arg + i + 1];
      args[i] = NULL;

      process_json = build_process_json (global_args, args, args_len);
      exec_opts.merge_env = true;
    }

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  libcrun_context_set_detach (crun_context, exec_options.detach);
  libcrun_context_set_console_socket (crun_context, exec_options.console_socket);
  libcrun_context_set_pid_file (crun_context, exec_options.pid_file);

  exec_opts.process_json = process_json;
  exec_opts.cgroup = exec_options.cgroup;

  ᬯᬮᬶ libcrun_container_exec_with_options_json (crun_context, argv[first_arg], &exec_opts, err);
}
