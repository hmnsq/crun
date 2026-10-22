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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <git-version.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <argp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <libgen.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DLOPEN
#  include <dlfcn.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "crun.h"

/* Commands.  */
#ਅਨ੍ਤਰ੍ਭਾਵ "run.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "delete.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "kill.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "start.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "create.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "exec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "state.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "update.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "spec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pause.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unpause.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "oci_features.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ps.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "checkpoint.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mounts.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "restore.h"

/* crun exec exits with the exit code of the executed process, so use this
   code for its own errors, to tell those apart (like runc and ssh do).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXIT_EXEC_FAILURE 255

ਸਥਿਰ ਬਣਤਰ crun_global_arguments arguments;

ਬਣਤਰ commands_s
{
  ਪੂਰਨ ਮੁੱਲ;
  ਅਚਲ ਅੱਖਰ *name;
  ਪੂਰਨ (*handler) (ਬਣਤਰ crun_global_arguments *, ਪੂਰਨ, ਅੱਖਰ **, libcrun_error_t *);
};

ਪੂਰਨ
init_libcrun_context (libcrun_context_t *con, ਅਚਲ ਅੱਖਰ *id, ਬਣਤਰ crun_global_arguments *glob, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  libcrun_context_ਸੈੱਟ_id (con, id);
  libcrun_context_ਸੈੱਟ_state_ਜੜ੍ਹ (con, glob->ਜੜ੍ਹ);
  libcrun_context_ਸੈੱਟ_systemd_cgroup (con, glob->option_systemd_cgroup);
  libcrun_context_ਸੈੱਟ_force_no_cgroup (con, glob->option_force_no_cgroup);
  libcrun_context_ਸੈੱਟ_notify_socket (con, getenv ("NOTIFY_SOCKET"));
  libcrun_context_ਸੈੱਟ_ਦਲੀਲਾਂ (con, glob->argc, glob->argv);

  /* Check if global handler is configured and pass it down to crun context */
  libcrun_context_ਸੈੱਟ_handler (con, glob->handler);

  ret = libcrun_context_init_logging (con, glob->ਲਘੂਗਣਕ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (glob->log_format)
    {
      ret = libcrun_ਸੈੱਟ_log_format (glob->log_format, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  libcrun_ਸੈੱਟ_verbosity (glob->verbosity);
  libcrun_debug ("Using debug verbosity");

  ਵਾਪਸ 0;
}

/* Allocate a public libcrun context (with handlers loaded) for a subcommand.
   Terminates the process on failure.  */
libcrun_context_t *
new_libcrun_context (ਬਣਤਰ crun_global_arguments *glob)
{
  libcrun_error_t err = ਨਿਰਾ;
  libcrun_context_t *con;

  con = libcrun_context_new (ਨਿਰਾ, glob->ਜੜ੍ਹ, &err);
  ਜੇ (UNLIKELY (con == ਨਿਰਾ))
    libcrun_fail_with_error (libcrun_error_get_ਸਥਿਤੀ (err), "%s", libcrun_error_get_ਸੁਨੇਹਾ (err));

  ਵਾਪਸ con;
}

ਗਿਣਤੀ
{
  COMMAND_CREATE = 1000,
  COMMAND_DELETE,
  COMMAND_EXEC,
  COMMAND_LIST,
  COMMAND_KILL,
  COMMAND_ਲੜੀਵਾਰ,
  COMMAND_SPEC,
  COMMAND_ਸ਼ੁਰੂ,
  COMMAND_STATE,
  COMMAND_UPDATE,
  COMMAND_PAUSE,
  COMMAND_UNPAUSE,
  COMMAND_FEATURES,
  COMMAND_PS,
  COMMAND_CHECKPOINT,
  COMMAND_RESTORE,
  COMMAND_MOUNTS,
};

ਬਣਤਰ commands_s commands[] = { { COMMAND_CREATE, "create", crun_command_create },
                                 { COMMAND_DELETE, "delete", crun_command_delete },
                                 { COMMAND_EXEC, "exec", crun_command_exec },
                                 { COMMAND_LIST, "list", crun_command_list },
                                 { COMMAND_KILL, "kill", crun_command_kill },
                                 { COMMAND_PS, "ps", crun_command_ps },
                                 { COMMAND_ਲੜੀਵਾਰ, "run", crun_command_ਲੜੀਵਾਰ },
                                 { COMMAND_SPEC, "spec", crun_command_spec },
                                 { COMMAND_ਸ਼ੁਰੂ, "start", crun_command_ਸ਼ੁਰੂ },
                                 { COMMAND_STATE, "state", crun_command_state },
                                 { COMMAND_UPDATE, "update", crun_command_update },
                                 { COMMAND_PAUSE, "pause", crun_command_pause },
                                 { COMMAND_UNPAUSE, "resume", crun_command_unpause },
                                 { COMMAND_FEATURES, "features", crun_command_features },
#ਜੇ HAVE_CRIU && HAVE_DLOPEN
                                 { COMMAND_CHECKPOINT, "checkpoint", crun_command_checkpoint },
                                 { COMMAND_RESTORE, "restore", crun_command_restore },
#ਜੇ_ਅੰਤ
                                 { COMMAND_MOUNTS, "mounts", crun_command_mounts },
                                 {
                                     0,
                                 } };

ਸਥਿਰ ਅੱਖਰ doc[] = "\nCOMMANDS:\n"
#ਜੇ HAVE_CRIU && HAVE_DLOPEN
                    "\tcheckpoint  - checkpoint a container\n"
#ਜੇ_ਅੰਤ
                    "\tcreate      - create a container\n"
                    "\tdelete      - remove definition for a container\n"
                    "\texec        - exec a command in a running container\n"
                    "\tfeatures    - show the enabled features\n"
                    "\tlist        - list known containers\n"
                    "\tmounts      - add or remove mounts from a running container\n"
                    "\tkill        - send a signal to the container init process\n"
                    "\tps          - show the processes in the container\n"
#ਜੇ HAVE_CRIU && HAVE_DLOPEN
                    "\trestore     - restore a container\n"
#ਜੇ_ਅੰਤ
                    "\trun         - run a container\n"
                    "\tspec        - generate a configuration file\n"
                    "\tstart       - start a container\n"
                    "\tstate       - output the state of a container\n"
                    "\tpause       - pause all the processes in the container\n"
                    "\tresume      - unpause the processes in the container\n"
                    "\tupdate      - update container resource constraints\n";

ਸਥਿਰ ਅੱਖਰ ਦਲੀਲਾਂ_doc[] = "COMMAND [OPTION...]";

ਸਥਿਰ ਬਣਤਰ commands_s *
get_command (ਅਚਲ ਅੱਖਰ *arg)
{
  ਬਣਤਰ commands_s *it;
  ਲਈ (it = commands; it->ਮੁੱਲ; it++)
    ਜੇ (strcmp (it->name, arg) == 0)
      ਵਾਪਸ it;
  ਵਾਪਸ ਨਿਰਾ;
}

ਗਿਣਤੀ
{
  OPTION_ਸੰਸਕਰਣ = 'v',
  OPTION_ਸੰਸਕਰਣ_CAP = 'V',
  OPTION_DEBUG = 1000,
  OPTION_SYSTEMD_CGROUP,
  OPTION_CGROUP_MANAGER,
  OPTION_LOG,
  OPTION_LOG_FORMAT,
  OPTION_LOG_LEVEL,
  OPTION_ਜੜ੍ਹ,
  OPTION_ROOTLESS
};

ਅਚਲ ਅੱਖਰ *argp_ਪ੍ਰੋਗਰਾਮ_bug_ਪਤਾ = "https://github.com/containers/crun/issues";

ਸਥਿਰ ਬਣਤਰ argp_option options[] = { { "debug", OPTION_DEBUG, 0, 0, "produce verbose output, similar to --log-level=debug", 0 },
                                        { "cgroup-manager", OPTION_CGROUP_MANAGER, "MANAGER", 0, "cgroup manager", 0 },
                                        { "systemd-cgroup", OPTION_SYSTEMD_CGROUP, 0, 0, "use systemd cgroups", 0 },
                                        { "log", OPTION_LOG, "FILE", 0, "log destination: '[file:]PATH', 'journald:ID' or 'syslog:ID' (defaults to stderr)", 0 },
                                        { "log-format", OPTION_LOG_FORMAT, "FORMAT", 0, "log format: 'text' (default) or 'json'", 0 },
                                        { "log-level", OPTION_LOG_LEVEL, "LEVEL", 0, "log level to use: 'error' (default), 'warning' or 'debug'", 0 },
                                        { "root", OPTION_ਜੜ੍ਹ, "DIR", 0, ਨਿਰਾ, 0 },
                                        { "rootless", OPTION_ROOTLESS, "VALUE", 0, ਨਿਰਾ, 0 },
                                        { "version", OPTION_ਸੰਸਕਰਣ, 0, 0, ਨਿਰਾ, 0 },
                                        // alias OPTION_VERSION_CAP with OPTION_VERSION
                                        { ਨਿਰਾ, OPTION_ਸੰਸਕਰਣ_CAP, 0, OPTION_ALIAS, ਨਿਰਾ, 0 },
                                        {
                                            0,
                                        } };

ਸਥਿਰ ਖਾਲੀ
print_ਸੰਸਕਰਣ (FILE *ਧਾਰਾ, ਬਣਤਰ argp_state *state arg_ਅਣਵਰਤਿਆ)
{
  libcrun_error_t err = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *rundir = ਨਿਰਾ;
  libcrun_context_t *ctx;
  ਅੱਖਰ **ਟੈਗ_ਸੂਚੀ;
  ਪੂਰਨ ret;

  fprintf (ਧਾਰਾ, "%s version %s\n", PACKAGE_NAME, PACKAGE_ਸੰਸਕਰਣ);
  fprintf (ਧਾਰਾ, "commit: %s\n", GIT_ਸੰਸਕਰਣ);

  ret = libcrun_get_state_dir (&rundir, arguments.ਜੜ੍ਹ, ਨਿਰਾ, &err);
  ਜੇ (LIKELY (ret == 0))
    fprintf (ਧਾਰਾ, "rundir: %s\n", rundir);
  ਨਹੀਂਤਾਂ
    libcrun_error_release (&err);

  fprintf (ਧਾਰਾ, "spec: 1.0.0\n");

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYSTEMD
  fprintf (ਧਾਰਾ, "+SYSTEMD ");
#ਜੇ_ਅੰਤ
  fprintf (ਧਾਰਾ, "+SELINUX ");
  fprintf (ਧਾਰਾ, "+APPARMOR ");
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CAP
  fprintf (ਧਾਰਾ, "+CAP ");
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SECCOMP
  fprintf (ਧਾਰਾ, "+SECCOMP ");
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_EBPF
  fprintf (ਧਾਰਾ, "+EBPF ");
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_CRIU && HAVE_DLOPEN
  fprintf (ਧਾਰਾ, "+CRIU ");
#ਜੇ_ਅੰਤ

  ctx = libcrun_context_new (ਨਿਰਾ, arguments.ਜੜ੍ਹ, &err);
  ਜੇ (ctx)
    {
      ਟੈਗ_ਸੂਚੀ = libcrun_context_get_handler_feature_ਟੈਗ_ਸੂਚੀ (ctx, &err);
      ਜੇ (ਟੈਗ_ਸੂਚੀ)
        {
          ਲਈ (size_t i = 0; ਟੈਗ_ਸੂਚੀ[i]; i++)
            {
              fprintf (ਧਾਰਾ, "+%s ", ਟੈਗ_ਸੂਚੀ[i]);
              ਮੁਕਤ (ਟੈਗ_ਸੂਚੀ[i]);
            }
          ਮੁਕਤ (ਟੈਗ_ਸੂਚੀ);
        }
      ਨਹੀਂਤਾਂ
        libcrun_error_release (&err);
      libcrun_context_free (ctx);
    }
  ਨਹੀਂਤਾਂ
    libcrun_error_release (&err);

  fprintf (ਧਾਰਾ, "+JSON_C\n");
}

ਸਥਿਰ error_t
parse_opt (ਪੂਰਨ ਕੁੰਜੀ, ਅੱਖਰ *arg, ਬਣਤਰ argp_state *state)
{
  ਅਚਲ ਅੱਖਰ *tmp;

  ਚੋਣ (ਕੁੰਜੀ)
    {
    ਹਾਲਤ OPTION_DEBUG:
      arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_CGROUP_MANAGER:
      tmp = argp_mandatory_ਦਲੀਲ (arg, state);
      ਜੇ (strcmp (tmp, "systemd") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = true;
        }
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (tmp, "cgroupfs") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = false;
        }
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (tmp, "disabled") == 0)
        {
          arguments.option_systemd_cgroup = false;
          arguments.option_force_no_cgroup = true;
        }
      ਨਹੀਂਤਾਂ
        {
          libcrun_fail_with_error (0, "unknown cgroup manager specified");
        }
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_SYSTEMD_CGROUP:
      arguments.option_force_no_cgroup = false;
      arguments.option_systemd_cgroup = true;
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_LOG:
      arguments.ਲਘੂਗਣਕ = argp_mandatory_ਦਲੀਲ (arg, state);
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_LOG_FORMAT:
      arguments.log_format = argp_mandatory_ਦਲੀਲ (arg, state);
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_LOG_LEVEL:
      tmp = argp_mandatory_ਦਲੀਲ (arg, state);
      ਜੇ (strcmp (tmp, "error") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_ERROR;
        }
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (tmp, "warning") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_WARNING;
        }
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (tmp, "debug") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
        }
      ਨਹੀਂਤਾਂ
        {
          libcrun_fail_with_error (0, "unknown verbosity `%s` specified", arg);
        }
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_ਜੜ੍ਹ:
      arguments.ਜੜ੍ਹ = argp_mandatory_ਦਲੀਲ (arg, state);
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_ROOTLESS:
      /* Ignored.  So that a runc command line won't fail.  */
      ਰੋਕੋ;

    ਹਾਲਤ ARGP_ਕੁੰਜੀ_NO_ਦਲੀਲਾਂ:
      libcrun_fail_with_error (0, "please specify a command");

    ਹਾਲਤ OPTION_ਸੰਸਕਰਣ:
    ਹਾਲਤ OPTION_ਸੰਸਕਰਣ_CAP:
      print_ਸੰਸਕਰਣ (stdout, state);
      ਬਾਹਰ_ਨਿਕਲੋ (EXIT_SUCCESS);
    ਮੂਲ:
      ਵਾਪਸ ARGP_ERR_UNKNOWN;
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਣਤਰ commands_s *command;

ਖਾਲੀ
crun_assert_n_ਦਲੀਲਾਂ (ਪੂਰਨ n, ਪੂਰਨ min, ਪੂਰਨ max)
{
  ਜੇ (min >= 0 && n < min)
    libcrun_fail_with_error (0, "`%s` requires a minimum of %d arguments", command->name, min);
  ਜੇ (max >= 0 && n > max)
    libcrun_fail_with_error (0, "`%s` requires a maximum of %d arguments", command->name, max);
}

ਅੱਖਰ *
argp_mandatory_ਦਲੀਲ (ਅੱਖਰ *arg, ਬਣਤਰ argp_state *state)
{
  ਜੇ (arg)
    ਵਾਪਸ arg;
  ਵਾਪਸ state->argv[state->ਅਗਲਾ++];
}

/* Parse a numeric value from STR, aborting on an out-of-range or malformed
   value.  KIND names the field for error messages.  If ENDPTR is not NULL it
   is set past the parsed number (so the caller can continue parsing, e.g. a
   "UID:GID" pair); otherwise the whole string must be a valid number.  */
ਪੂਰਨ
parse_id_or_fail (ਅਚਲ ਅੱਖਰ *str, ਅੱਖਰ **endptr, ਅਚਲ ਅੱਖਰ *ਕਿਸਮ)
{
  ਅੱਖਰ *ਸਿਰਾ = ਨਿਰਾ;
  ਲੰਮਾ ਲੰਮਾ l;

  errno = 0;
  l = strtoll (str, &ਸਿਰਾ, 10);
  ਜੇ (ਸਿਰਾ == str)
    libcrun_fail_with_error (0, "invalid %s specified", ਕਿਸਮ);
  ਜੇ (errno == ERANGE)
    libcrun_fail_with_error (0, "invalid %s specified", ਕਿਸਮ);
  ਜੇ (l < INT_MIN || l > ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)
    libcrun_fail_with_error (0, "invalid %s specified", ਕਿਸਮ);

  ਜੇ (endptr)
    *endptr = ਸਿਰਾ;
  ਨਹੀਂਤਾਂ ਜੇ (*ਸਿਰਾ != '\0')
    libcrun_fail_with_error (0, "invalid %s specified", ਕਿਸਮ);

  ਵਾਪਸ (ਪੂਰਨ) l;
}

ਸਥਿਰ ਬਣਤਰ argp argp = { options, parse_opt, ਦਲੀਲਾਂ_doc, doc, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ };

ਪੂਰਨ ensure_cloned_ਦੁਆਧਾਰੀ (ਖਾਲੀ);

ਸਥਿਰ ਖਾਲੀ
fill_handler_from_argv0 (ਅੱਖਰ *argv0, ਬਣਤਰ crun_global_arguments *ਦਲੀਲਾਂ)
{
  ਅਚਲ ਅੱਖਰ *b = basename (argv0);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_LIBKRUN
  ਜੇ (strcmp (b, "krun") == 0)
    {
      ਦਲੀਲਾਂ->handler = "krun";
      ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ
  ਜੇ (ਲੜੀ_ਤੁਲਨਾ (b, "crun-", 5) == 0 && b[5] != '\0')
    ਦਲੀਲਾਂ->handler = b + 5;
}

ਸਥਿਰ ਅੱਖਰ **
copy_ਦਲੀਲਾਂ (ਅੱਖਰ **argv, ਪੂਰਨ argc)
{
  ਅੱਖਰ **buff = xmalloc0 ((argc + 1) * ਆਕਾਰ (ਅੱਖਰ *));
  ਲਈ (ਪੂਰਨ i = 0; i < argc; i++)
    buff[i] = argv[i];

  ਵਾਪਸ buff;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  libcrun_error_t err = ਨਿਰਾ;
  ਪੂਰਨ ret, ਪਹਿਲਾ_ਦਲੀਲ = 0;

  arguments.argc = argc;
  arguments.argv = argv;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DYNLOAD_LIBCRUN
  ਜੇ (ensure_cloned_ਦੁਆਧਾਰੀ () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      ਬਾਹਰ_ਨਿਕਲੋ (EXIT_FAILURE);
    }
  /* Resolve all libcrun weak dependencies.  */
  ਜੇ (dlopen ("libcrun.so", RTLD_GLOBAL | RTLD_DEEPBIND | RTLD_LAZY) == ਨਿਰਾ)
    libcrun_fail_with_error (0, "could not load `libcrun.so`: `%s`", dlerror ());
#ਜੇ_ਅੰਤ

  fill_handler_from_argv0 (argv[0], &arguments);
  argp_parse (&argp, argc, argv, ARGP_ਵਿੱਚ_ਕ੍ਰਮ, &ਪਹਿਲਾ_ਦਲੀਲ, &arguments);

  command = get_command (argv[ਪਹਿਲਾ_ਦਲੀਲ]);
  ਜੇ (command == ਨਿਰਾ)
    libcrun_fail_with_error (0, "unknown command %s", argv[ਪਹਿਲਾ_ਦਲੀਲ]);

  ਪੂਰਨ command_argc = argc - ਪਹਿਲਾ_ਦਲੀਲ;
  cleanup_free ਅੱਖਰ **command_argv = copy_ਦਲੀਲਾਂ (argv + ਪਹਿਲਾ_ਦਲੀਲ, command_argc);
  command_argv[0] = argv[0];

  ret = command->handler (&arguments, command_argc, command_argv, &err);
  ਜੇ (ret && err)
    {
      ਜੇ (command->ਮੁੱਲ == COMMAND_EXEC)
        {
          libcrun_error (libcrun_error_get_ਸਥਿਤੀ (err), "%s", libcrun_error_get_ਸੁਨੇਹਾ (err));
          ਬਾਹਰ_ਨਿਕਲੋ (EXIT_EXEC_FAILURE);
        }
      libcrun_fail_with_error (libcrun_error_get_ਸਥਿਤੀ (err), "%s", libcrun_error_get_ਸੁਨੇਹਾ (err));
    }

  ਵਾਪਸ ret;
}
