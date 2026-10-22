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
#ಅನ್ತರ್ಭಾವ <git-version.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <argp.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <libgen.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <limits.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ಅನ್ತರ್ಭಾವ "crun.h"

/* Commands.  */
#ಅನ್ತರ್ಭಾವ "run.h"
#ಅನ್ತರ್ಭಾವ "delete.h"
#ಅನ್ತರ್ಭಾವ "kill.h"
#ಅನ್ತರ್ಭಾವ "list.h"
#ಅನ್ತರ್ಭಾವ "start.h"
#ಅನ್ತರ್ಭಾವ "create.h"
#ಅನ್ತರ್ಭಾವ "exec.h"
#ಅನ್ತರ್ಭಾವ "state.h"
#ಅನ್ತರ್ಭಾವ "update.h"
#ಅನ್ತರ್ಭಾವ "spec.h"
#ಅನ್ತರ್ಭಾವ "pause.h"
#ಅನ್ತರ್ಭಾವ "unpause.h"
#ಅನ್ತರ್ಭಾವ "oci_features.h"
#ಅನ್ತರ್ಭಾವ "ps.h"
#ಅನ್ತರ್ಭಾವ "checkpoint.h"
#ಅನ್ತರ್ಭಾವ "mounts.h"
#ಅನ್ತರ್ಭಾವ "restore.h"

/* crun exec exits with the exit code of the executed process, so use this
   code for its own errors, to tell those apart (like runc and ssh do).  */
#define EXIT_EXEC_FAILURE 255

ಸ್ಥಿರ ಸಂರಚನಾ crun_global_arguments arguments;

ಸಂರಚನಾ commands_s
{
  ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ;
  ಧ್ರುವ ವರ್ಣ *name;
  ಪೂರ್ಣಾಙ್ಕ (*handler) (ಸಂರಚನಾ crun_global_arguments *, ಪೂರ್ಣಾಙ್ಕ, ವರ್ಣ **, libcrun_error_t *);
};

ಪೂರ್ಣಾಙ್ಕ
init_libcrun_context (libcrun_context_t *con, ಧ್ರುವ ವರ್ಣ *id, ಸಂರಚನಾ crun_global_arguments *glob, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  libcrun_context_set_id (con, id);
  libcrun_context_set_state_ಬೇರು (con, glob->ಬೇರು);
  libcrun_context_set_systemd_cgroup (con, glob->option_systemd_cgroup);
  libcrun_context_set_force_no_cgroup (con, glob->option_force_no_cgroup);
  libcrun_context_set_notify_socket (con, getenv ("NOTIFY_SOCKET"));
  libcrun_context_set_args (con, glob->argc, glob->argv);

  /* Check if global handler is configured and pass it down to crun context */
  libcrun_context_set_handler (con, glob->handler);

  ret = libcrun_context_init_logging (con, glob->log, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (glob->log_format)
    {
      ret = libcrun_set_log_format (glob->log_format, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  libcrun_set_verbosity (glob->verbosity);
  libcrun_debug ("Using debug verbosity");

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Allocate a public libcrun context (with handlers loaded) for a subcommand.
   Terminates the process on failure.  */
libcrun_context_t *
new_libcrun_context (ಸಂರಚನಾ crun_global_arguments *glob)
{
  libcrun_error_t err = ರಿಕ್ತ;
  libcrun_context_t *con;

  con = libcrun_context_new (ರಿಕ್ತ, glob->ಬೇರು, &err);
  ಯದಿ (UNLIKELY (con == ರಿಕ್ತ))
    libcrun_fail_with_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));

  ಪ್ರತ್ಯಾಗಮ con;
}

ಗಣನಾ
{
  COMMAND_CREATE = 1000,
  COMMAND_DELETE,
  COMMAND_EXEC,
  COMMAND_LIST,
  COMMAND_KILL,
  COMMAND_RUN,
  COMMAND_SPEC,
  COMMAND_START,
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

ಸಂರಚನಾ commands_s commands[] = { { COMMAND_CREATE, "create", crun_command_create },
                                 { COMMAND_DELETE, "delete", crun_command_delete },
                                 { COMMAND_EXEC, "exec", crun_command_exec },
                                 { COMMAND_LIST, "list", crun_command_list },
                                 { COMMAND_KILL, "kill", crun_command_kill },
                                 { COMMAND_PS, "ps", crun_command_ps },
                                 { COMMAND_RUN, "run", crun_command_run },
                                 { COMMAND_SPEC, "spec", crun_command_spec },
                                 { COMMAND_START, "start", crun_command_start },
                                 { COMMAND_STATE, "state", crun_command_state },
                                 { COMMAND_UPDATE, "update", crun_command_update },
                                 { COMMAND_PAUSE, "pause", crun_command_pause },
                                 { COMMAND_UNPAUSE, "resume", crun_command_unpause },
                                 { COMMAND_FEATURES, "features", crun_command_features },
#ಯದಿ HAVE_CRIU && HAVE_DLOPEN
                                 { COMMAND_CHECKPOINT, "checkpoint", crun_command_checkpoint },
                                 { COMMAND_RESTORE, "restore", crun_command_restore },
#endif
                                 { COMMAND_MOUNTS, "mounts", crun_command_mounts },
                                 {
                                     0,
                                 } };

ಸ್ಥಿರ ವರ್ಣ doc[] = "\nCOMMANDS:\n"
#ಯದಿ HAVE_CRIU && HAVE_DLOPEN
                    "\tcheckpoint  - checkpoint a container\n"
#endif
                    "\tcreate      - create a container\n"
                    "\tdelete      - remove definition for a container\n"
                    "\texec        - exec a command in a running container\n"
                    "\tfeatures    - show the enabled features\n"
                    "\tlist        - list known containers\n"
                    "\tmounts      - add or remove mounts from a running container\n"
                    "\tkill        - send a signal to the container init process\n"
                    "\tps          - show the processes in the container\n"
#ಯದಿ HAVE_CRIU && HAVE_DLOPEN
                    "\trestore     - restore a container\n"
#endif
                    "\trun         - run a container\n"
                    "\tspec        - generate a configuration file\n"
                    "\tstart       - start a container\n"
                    "\tstate       - output the state of a container\n"
                    "\tpause       - pause all the processes in the container\n"
                    "\tresume      - unpause the processes in the container\n"
                    "\tupdate      - update container resource constraints\n";

ಸ್ಥಿರ ವರ್ಣ args_doc[] = "COMMAND [OPTION...]";

ಸ್ಥಿರ ಸಂರಚನಾ commands_s *
get_command (ಧ್ರುವ ವರ್ಣ *arg)
{
  ಸಂರಚನಾ commands_s *it;
  ಪ್ರತಿ (it = commands; it->ಮೌಲ್ಯ; it++)
    ಯದಿ (strcmp (it->name, arg) == 0)
      ಪ್ರತ್ಯಾಗಮ it;
  ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಗಣನಾ
{
  OPTION_VERSION = 'v',
  OPTION_VERSION_CAP = 'V',
  OPTION_DEBUG = 1000,
  OPTION_SYSTEMD_CGROUP,
  OPTION_CGROUP_MANAGER,
  OPTION_LOG,
  OPTION_LOG_FORMAT,
  OPTION_LOG_LEVEL,
  OPTION_ಬೇರು,
  OPTION_ROOTLESS
};

ಧ್ರುವ ವರ್ಣ *argp_program_bug_address = "https://github.com/containers/crun/issues";

ಸ್ಥಿರ ಸಂರಚನಾ argp_option options[] = { { "debug", OPTION_DEBUG, 0, 0, "produce verbose output, similar to --log-level=debug", 0 },
                                        { "cgroup-manager", OPTION_CGROUP_MANAGER, "MANAGER", 0, "cgroup manager", 0 },
                                        { "systemd-cgroup", OPTION_SYSTEMD_CGROUP, 0, 0, "use systemd cgroups", 0 },
                                        { "log", OPTION_LOG, "FILE", 0, "log destination: '[file:]PATH', 'journald:ID' or 'syslog:ID' (defaults to stderr)", 0 },
                                        { "log-format", OPTION_LOG_FORMAT, "FORMAT", 0, "log format: 'text' (default) or 'json'", 0 },
                                        { "log-level", OPTION_LOG_LEVEL, "LEVEL", 0, "log level to use: 'error' (default), 'warning' or 'debug'", 0 },
                                        { "root", OPTION_ಬೇರು, "DIR", 0, ರಿಕ್ತ, 0 },
                                        { "rootless", OPTION_ROOTLESS, "VALUE", 0, ರಿಕ್ತ, 0 },
                                        { "version", OPTION_VERSION, 0, 0, ರಿಕ್ತ, 0 },
                                        // alias OPTION_VERSION_CAP with OPTION_VERSION
                                        { ರಿಕ್ತ, OPTION_VERSION_CAP, 0, OPTION_ALIAS, ರಿಕ್ತ, 0 },
                                        {
                                            0,
                                        } };

ಸ್ಥಿರ ಶೂನ್ಯ
print_version (FILE *stream, ಸಂರಚನಾ argp_state *state arg_unused)
{
  libcrun_error_t err = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *rundir = ರಿಕ್ತ;
  libcrun_context_t *ctx;
  ವರ್ಣ **tags;
  ಪೂರ್ಣಾಙ್ಕ ret;

  fprintf (stream, "%s version %s\n", PACKAGE_NAME, PACKAGE_VERSION);
  fprintf (stream, "commit: %s\n", GIT_VERSION);

  ret = libcrun_get_state_dir (&rundir, arguments.ಬೇರು, ರಿಕ್ತ, &err);
  ಯದಿ (LIKELY (ret == 0))
    fprintf (stream, "rundir: %s\n", rundir);
  ಅನ್ಯಥಾ
    libcrun_error_release (&err);

  fprintf (stream, "spec: 1.0.0\n");

#ifdef HAVE_SYSTEMD
  fprintf (stream, "+SYSTEMD ");
#endif
  fprintf (stream, "+SELINUX ");
  fprintf (stream, "+APPARMOR ");
#ifdef HAVE_CAP
  fprintf (stream, "+CAP ");
#endif
#ifdef HAVE_SECCOMP
  fprintf (stream, "+SECCOMP ");
#endif
#ifdef HAVE_EBPF
  fprintf (stream, "+EBPF ");
#endif
#ಯದಿ HAVE_CRIU && HAVE_DLOPEN
  fprintf (stream, "+CRIU ");
#endif

  ctx = libcrun_context_new (ರಿಕ್ತ, arguments.ಬೇರು, &err);
  ಯದಿ (ctx)
    {
      tags = libcrun_context_get_handler_feature_tags (ctx, &err);
      ಯದಿ (tags)
        {
          ಪ್ರತಿ (size_t i = 0; tags[i]; i++)
            {
              fprintf (stream, "+%s ", tags[i]);
              ಮುಕ್ತ (tags[i]);
            }
          ಮುಕ್ತ (tags);
        }
      ಅನ್ಯಥಾ
        libcrun_error_release (&err);
      libcrun_context_free (ctx);
    }
  ಅನ್ಯಥಾ
    libcrun_error_release (&err);

  fprintf (stream, "+JSON_C\n");
}

ಸ್ಥಿರ error_t
parse_opt (ಪೂರ್ಣಾಙ್ಕ ಕೀಲಿ, ವರ್ಣ *arg, ಸಂರಚನಾ argp_state *state)
{
  ಧ್ರುವ ವರ್ಣ *tmp;

  ವಿಕಲ್ಪ (ಕೀಲಿ)
    {
    ಪಕ್ಷ OPTION_DEBUG:
      arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_CGROUP_MANAGER:
      tmp = argp_mandatory_argument (arg, state);
      ಯದಿ (strcmp (tmp, "systemd") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = true;
        }
      ಅನ್ಯಥಾ ಯದಿ (strcmp (tmp, "cgroupfs") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = false;
        }
      ಅನ್ಯಥಾ ಯದಿ (strcmp (tmp, "disabled") == 0)
        {
          arguments.option_systemd_cgroup = false;
          arguments.option_force_no_cgroup = true;
        }
      ಅನ್ಯಥಾ
        {
          libcrun_fail_with_error (0, "unknown cgroup manager specified");
        }
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_SYSTEMD_CGROUP:
      arguments.option_force_no_cgroup = false;
      arguments.option_systemd_cgroup = true;
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_LOG:
      arguments.log = argp_mandatory_argument (arg, state);
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_LOG_FORMAT:
      arguments.log_format = argp_mandatory_argument (arg, state);
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_LOG_LEVEL:
      tmp = argp_mandatory_argument (arg, state);
      ಯದಿ (strcmp (tmp, "error") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_ERROR;
        }
      ಅನ್ಯಥಾ ಯದಿ (strcmp (tmp, "warning") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_WARNING;
        }
      ಅನ್ಯಥಾ ಯದಿ (strcmp (tmp, "debug") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
        }
      ಅನ್ಯಥಾ
        {
          libcrun_fail_with_error (0, "unknown verbosity `%s` specified", arg);
        }
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_ಬೇರು:
      arguments.ಬೇರು = argp_mandatory_argument (arg, state);
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_ROOTLESS:
      /* Ignored.  So that a runc command line won't fail.  */
      ಭಙ್ಗ;

    ಪಕ್ಷ ARGP_ಕೀಲಿ_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a command");

    ಪಕ್ಷ OPTION_VERSION:
    ಪಕ್ಷ OPTION_VERSION_CAP:
      print_version (stdout, state);
      exit (EXIT_SUCCESS);
    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ ARGP_ERR_UNKNOWN;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಸಂರಚನಾ commands_s *command;

ಶೂನ್ಯ
crun_assert_n_args (ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ min, ಪೂರ್ಣಾಙ್ಕ max)
{
  ಯದಿ (min >= 0 && n < min)
    libcrun_fail_with_error (0, "`%s` requires a minimum of %d arguments", command->name, min);
  ಯದಿ (max >= 0 && n > max)
    libcrun_fail_with_error (0, "`%s` requires a maximum of %d arguments", command->name, max);
}

ವರ್ಣ *
argp_mandatory_argument (ವರ್ಣ *arg, ಸಂರಚನಾ argp_state *state)
{
  ಯದಿ (arg)
    ಪ್ರತ್ಯಾಗಮ arg;
  ಪ್ರತ್ಯಾಗಮ state->argv[state->next++];
}

/* Parse a numeric value from STR, aborting on an out-of-range or malformed
   value.  KIND names the field for error messages.  If ENDPTR is not NULL it
   is set past the parsed number (so the caller can continue parsing, e.g. a
   "UID:GID" pair); otherwise the whole string must be a valid number.  */
ಪೂರ್ಣಾಙ್ಕ
parse_id_or_fail (ಧ್ರುವ ವರ್ಣ *str, ವರ್ಣ **endptr, ಧ್ರುವ ವರ್ಣ *kind)
{
  ವರ್ಣ *end = ರಿಕ್ತ;
  long long l;

  errno = 0;
  l = strtoll (str, &end, 10);
  ಯದಿ (end == str)
    libcrun_fail_with_error (0, "invalid %s specified", kind);
  ಯದಿ (errno == ERANGE)
    libcrun_fail_with_error (0, "invalid %s specified", kind);
  ಯದಿ (l < INT_MIN || l > INT_MAX)
    libcrun_fail_with_error (0, "invalid %s specified", kind);

  ಯದಿ (endptr)
    *endptr = end;
  ಅನ್ಯಥಾ ಯದಿ (*end != '\0')
    libcrun_fail_with_error (0, "invalid %s specified", kind);

  ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) l;
}

ಸ್ಥಿರ ಸಂರಚನಾ argp argp = { options, parse_opt, args_doc, doc, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ };

ಪೂರ್ಣಾಙ್ಕ ensure_cloned_ದ್ವಿಮಾನ (ಶೂನ್ಯ);

ಸ್ಥಿರ ಶೂನ್ಯ
fill_handler_from_argv0 (ವರ್ಣ *argv0, ಸಂರಚನಾ crun_global_arguments *args)
{
  ಧ್ರುವ ವರ್ಣ *b = basename (argv0);
#ifdef HAVE_LIBKRUN
  ಯದಿ (strcmp (b, "krun") == 0)
    {
      args->handler = "krun";
      ಪ್ರತ್ಯಾಗಮ;
    }
#endif
  ಯದಿ (strncmp (b, "crun-", 5) == 0 && b[5] != '\0')
    args->handler = b + 5;
}

ಸ್ಥಿರ ವರ್ಣ **
copy_args (ವರ್ಣ **argv, ಪೂರ್ಣಾಙ್ಕ argc)
{
  ವರ್ಣ **buff = xmalloc0 ((argc + 1) * ಪರಿಮಾಣ (ವರ್ಣ *));
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < argc; i++)
    buff[i] = argv[i];

  ಪ್ರತ್ಯಾಗಮ buff;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  libcrun_error_t err = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret, first_argument = 0;

  arguments.argc = argc;
  arguments.argv = argv;

#ifdef DYNLOAD_LIBCRUN
  ಯದಿ (ensure_cloned_ದ್ವಿಮಾನ () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      exit (EXIT_FAILURE);
    }
  /* Resolve all libcrun weak dependencies.  */
  ಯದಿ (dlopen ("libcrun.so", RTLD_GLOBAL | RTLD_DEEPBIND | RTLD_LAZY) == ರಿಕ್ತ)
    libcrun_fail_with_error (0, "could not load `libcrun.so`: `%s`", dlerror ());
#endif

  fill_handler_from_argv0 (argv[0], &arguments);
  argp_parse (&argp, argc, argv, ARGP_IN_ORDER, &first_argument, &arguments);

  command = get_command (argv[first_argument]);
  ಯದಿ (command == ರಿಕ್ತ)
    libcrun_fail_with_error (0, "unknown command %s", argv[first_argument]);

  ಪೂರ್ಣಾಙ್ಕ command_argc = argc - first_argument;
  cleanup_free ವರ್ಣ **command_argv = copy_args (argv + first_argument, command_argc);
  command_argv[0] = argv[0];

  ret = command->handler (&arguments, command_argc, command_argv, &err);
  ಯದಿ (ret && err)
    {
      ಯದಿ (command->ಮೌಲ್ಯ == COMMAND_EXEC)
        {
          libcrun_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));
          exit (EXIT_EXEC_FAILURE);
        }
      libcrun_fail_with_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));
    }

  ಪ್ರತ್ಯಾಗಮ ret;
}
