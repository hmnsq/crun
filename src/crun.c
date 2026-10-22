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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <git-version.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <argp.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <libgen.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <limits.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#അന്തര്ഭാവ "crun.h"

/* Commands.  */
#അന്തര്ഭാവ "run.h"
#അന്തര്ഭാവ "delete.h"
#അന്തര്ഭാവ "kill.h"
#അന്തര്ഭാവ "list.h"
#അന്തര്ഭാവ "start.h"
#അന്തര്ഭാവ "create.h"
#അന്തര്ഭാവ "exec.h"
#അന്തര്ഭാവ "state.h"
#അന്തര്ഭാവ "update.h"
#അന്തര്ഭാവ "spec.h"
#അന്തര്ഭാവ "pause.h"
#അന്തര്ഭാവ "unpause.h"
#അന്തര്ഭാവ "oci_features.h"
#അന്തര്ഭാവ "ps.h"
#അന്തര്ഭാവ "checkpoint.h"
#അന്തര്ഭാവ "mounts.h"
#അന്തര്ഭാവ "restore.h"

/* crun exec exits with the exit code of the executed process, so use this
   code for its own errors, to tell those apart (like runc and ssh do).  */
#define EXIT_EXEC_FAILURE 255

സ്ഥിതം ഘടന crun_global_arguments arguments;

ഘടന commands_s
{
  പൂർണ്ണസംഖ്യ മൂല്യം;
  സ്ഥിരം അക്ഷരം *name;
  പൂർണ്ണസംഖ്യ (*handler) (ഘടന crun_global_arguments *, പൂർണ്ണസംഖ്യ, അക്ഷരം **, libcrun_error_t *);
};

പൂർണ്ണസംഖ്യ
init_libcrun_context (libcrun_context_t *con, സ്ഥിരം അക്ഷരം *id, ഘടന crun_global_arguments *glob, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  libcrun_context_set_id (con, id);
  libcrun_context_set_state_വേര് (con, glob->വേര്);
  libcrun_context_set_systemd_cgroup (con, glob->option_systemd_cgroup);
  libcrun_context_set_force_no_cgroup (con, glob->option_force_no_cgroup);
  libcrun_context_set_notify_socket (con, getenv ("NOTIFY_SOCKET"));
  libcrun_context_set_args (con, glob->argc, glob->argv);

  /* Check if global handler is configured and pass it down to crun context */
  libcrun_context_set_handler (con, glob->handler);

  ret = libcrun_context_init_logging (con, glob->log, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (glob->log_format)
    {
      ret = libcrun_set_log_format (glob->log_format, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  libcrun_set_verbosity (glob->verbosity);
  libcrun_debug ("Using debug verbosity");

  തിരികെ 0;
}

/* Allocate a public libcrun context (with handlers loaded) for a subcommand.
   Terminates the process on failure.  */
libcrun_context_t *
new_libcrun_context (ഘടന crun_global_arguments *glob)
{
  libcrun_error_t err = ശൂന്യ;
  libcrun_context_t *con;

  con = libcrun_context_new (ശൂന്യ, glob->വേര്, &err);
  എങ്കിൽ (UNLIKELY (con == ശൂന്യ))
    libcrun_fail_with_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));

  തിരികെ con;
}

എണ്ണൽ
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

ഘടന commands_s commands[] = { { COMMAND_CREATE, "create", crun_command_create },
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
#എങ്കിൽ HAVE_CRIU && HAVE_DLOPEN
                                 { COMMAND_CHECKPOINT, "checkpoint", crun_command_checkpoint },
                                 { COMMAND_RESTORE, "restore", crun_command_restore },
#endif
                                 { COMMAND_MOUNTS, "mounts", crun_command_mounts },
                                 {
                                     0,
                                 } };

സ്ഥിതം അക്ഷരം doc[] = "\nCOMMANDS:\n"
#എങ്കിൽ HAVE_CRIU && HAVE_DLOPEN
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
#എങ്കിൽ HAVE_CRIU && HAVE_DLOPEN
                    "\trestore     - restore a container\n"
#endif
                    "\trun         - run a container\n"
                    "\tspec        - generate a configuration file\n"
                    "\tstart       - start a container\n"
                    "\tstate       - output the state of a container\n"
                    "\tpause       - pause all the processes in the container\n"
                    "\tresume      - unpause the processes in the container\n"
                    "\tupdate      - update container resource constraints\n";

സ്ഥിതം അക്ഷരം args_doc[] = "COMMAND [OPTION...]";

സ്ഥിതം ഘടന commands_s *
get_command (സ്ഥിരം അക്ഷരം *arg)
{
  ഘടന commands_s *it;
  വേണ്ടി (it = commands; it->മൂല്യം; it++)
    എങ്കിൽ (strcmp (it->name, arg) == 0)
      തിരികെ it;
  തിരികെ ശൂന്യ;
}

എണ്ണൽ
{
  OPTION_VERSION = 'v',
  OPTION_VERSION_CAP = 'V',
  OPTION_DEBUG = 1000,
  OPTION_SYSTEMD_CGROUP,
  OPTION_CGROUP_MANAGER,
  OPTION_LOG,
  OPTION_LOG_FORMAT,
  OPTION_LOG_LEVEL,
  OPTION_വേര്,
  OPTION_ROOTLESS
};

സ്ഥിരം അക്ഷരം *argp_program_bug_address = "https://github.com/containers/crun/issues";

സ്ഥിതം ഘടന argp_option options[] = { { "debug", OPTION_DEBUG, 0, 0, "produce verbose output, similar to --log-level=debug", 0 },
                                        { "cgroup-manager", OPTION_CGROUP_MANAGER, "MANAGER", 0, "cgroup manager", 0 },
                                        { "systemd-cgroup", OPTION_SYSTEMD_CGROUP, 0, 0, "use systemd cgroups", 0 },
                                        { "log", OPTION_LOG, "FILE", 0, "log destination: '[file:]PATH', 'journald:ID' or 'syslog:ID' (defaults to stderr)", 0 },
                                        { "log-format", OPTION_LOG_FORMAT, "FORMAT", 0, "log format: 'text' (default) or 'json'", 0 },
                                        { "log-level", OPTION_LOG_LEVEL, "LEVEL", 0, "log level to use: 'error' (default), 'warning' or 'debug'", 0 },
                                        { "root", OPTION_വേര്, "DIR", 0, ശൂന്യ, 0 },
                                        { "rootless", OPTION_ROOTLESS, "VALUE", 0, ശൂന്യ, 0 },
                                        { "version", OPTION_VERSION, 0, 0, ശൂന്യ, 0 },
                                        // alias OPTION_VERSION_CAP with OPTION_VERSION
                                        { ശൂന്യ, OPTION_VERSION_CAP, 0, OPTION_ALIAS, ശൂന്യ, 0 },
                                        {
                                            0,
                                        } };

സ്ഥിതം ശൂന്യം
print_version (FILE *stream, ഘടന argp_state *state arg_unused)
{
  libcrun_error_t err = ശൂന്യ;
  cleanup_free അക്ഷരം *rundir = ശൂന്യ;
  libcrun_context_t *ctx;
  അക്ഷരം **tags;
  പൂർണ്ണസംഖ്യ ret;

  fprintf (stream, "%s version %s\n", PACKAGE_NAME, PACKAGE_VERSION);
  fprintf (stream, "commit: %s\n", GIT_VERSION);

  ret = libcrun_get_state_dir (&rundir, arguments.വേര്, ശൂന്യ, &err);
  എങ്കിൽ (LIKELY (ret == 0))
    fprintf (stream, "rundir: %s\n", rundir);
  അല്ലെങ്കിൽ
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
#എങ്കിൽ HAVE_CRIU && HAVE_DLOPEN
  fprintf (stream, "+CRIU ");
#endif

  ctx = libcrun_context_new (ശൂന്യ, arguments.വേര്, &err);
  എങ്കിൽ (ctx)
    {
      tags = libcrun_context_get_handler_feature_tags (ctx, &err);
      എങ്കിൽ (tags)
        {
          വേണ്ടി (size_t i = 0; tags[i]; i++)
            {
              fprintf (stream, "+%s ", tags[i]);
              സ്വതന്ത്രം (tags[i]);
            }
          സ്വതന്ത്രം (tags);
        }
      അല്ലെങ്കിൽ
        libcrun_error_release (&err);
      libcrun_context_free (ctx);
    }
  അല്ലെങ്കിൽ
    libcrun_error_release (&err);

  fprintf (stream, "+JSON_C\n");
}

സ്ഥിതം error_t
parse_opt (പൂർണ്ണസംഖ്യ താക്കോൽ, അക്ഷരം *arg, ഘടന argp_state *state)
{
  സ്ഥിരം അക്ഷരം *tmp;

  തിരഞ്ഞെടുപ്പ് (താക്കോൽ)
    {
    അവസ്ഥ OPTION_DEBUG:
      arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
      നിർത്തുക;

    അവസ്ഥ OPTION_CGROUP_MANAGER:
      tmp = argp_mandatory_argument (arg, state);
      എങ്കിൽ (strcmp (tmp, "systemd") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = true;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (tmp, "cgroupfs") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = false;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (tmp, "disabled") == 0)
        {
          arguments.option_systemd_cgroup = false;
          arguments.option_force_no_cgroup = true;
        }
      അല്ലെങ്കിൽ
        {
          libcrun_fail_with_error (0, "unknown cgroup manager specified");
        }
      നിർത്തുക;

    അവസ്ഥ OPTION_SYSTEMD_CGROUP:
      arguments.option_force_no_cgroup = false;
      arguments.option_systemd_cgroup = true;
      നിർത്തുക;

    അവസ്ഥ OPTION_LOG:
      arguments.log = argp_mandatory_argument (arg, state);
      നിർത്തുക;

    അവസ്ഥ OPTION_LOG_FORMAT:
      arguments.log_format = argp_mandatory_argument (arg, state);
      നിർത്തുക;

    അവസ്ഥ OPTION_LOG_LEVEL:
      tmp = argp_mandatory_argument (arg, state);
      എങ്കിൽ (strcmp (tmp, "error") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_ERROR;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (tmp, "warning") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_WARNING;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (tmp, "debug") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
        }
      അല്ലെങ്കിൽ
        {
          libcrun_fail_with_error (0, "unknown verbosity `%s` specified", arg);
        }
      നിർത്തുക;

    അവസ്ഥ OPTION_വേര്:
      arguments.വേര് = argp_mandatory_argument (arg, state);
      നിർത്തുക;

    അവസ്ഥ OPTION_ROOTLESS:
      /* Ignored.  So that a runc command line won't fail.  */
      നിർത്തുക;

    അവസ്ഥ ARGP_താക്കോൽ_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a command");

    അവസ്ഥ OPTION_VERSION:
    അവസ്ഥ OPTION_VERSION_CAP:
      print_version (stdout, state);
      exit (EXIT_SUCCESS);
    സ്ഥിരസ്ഥിതി:
      തിരികെ ARGP_ERR_UNKNOWN;
    }

  തിരികെ 0;
}

സ്ഥിതം ഘടന commands_s *command;

ശൂന്യം
crun_assert_n_args (പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ min, പൂർണ്ണസംഖ്യ max)
{
  എങ്കിൽ (min >= 0 && n < min)
    libcrun_fail_with_error (0, "`%s` requires a minimum of %d arguments", command->name, min);
  എങ്കിൽ (max >= 0 && n > max)
    libcrun_fail_with_error (0, "`%s` requires a maximum of %d arguments", command->name, max);
}

അക്ഷരം *
argp_mandatory_argument (അക്ഷരം *arg, ഘടന argp_state *state)
{
  എങ്കിൽ (arg)
    തിരികെ arg;
  തിരികെ state->argv[state->next++];
}

/* Parse a numeric value from STR, aborting on an out-of-range or malformed
   value.  KIND names the field for error messages.  If ENDPTR is not NULL it
   is set past the parsed number (so the caller can continue parsing, e.g. a
   "UID:GID" pair); otherwise the whole string must be a valid number.  */
പൂർണ്ണസംഖ്യ
parse_id_or_fail (സ്ഥിരം അക്ഷരം *str, അക്ഷരം **endptr, സ്ഥിരം അക്ഷരം *kind)
{
  അക്ഷരം *end = ശൂന്യ;
  long long l;

  errno = 0;
  l = strtoll (str, &end, 10);
  എങ്കിൽ (end == str)
    libcrun_fail_with_error (0, "invalid %s specified", kind);
  എങ്കിൽ (errno == ERANGE)
    libcrun_fail_with_error (0, "invalid %s specified", kind);
  എങ്കിൽ (l < INT_MIN || l > INT_MAX)
    libcrun_fail_with_error (0, "invalid %s specified", kind);

  എങ്കിൽ (endptr)
    *endptr = end;
  അല്ലെങ്കിൽ എങ്കിൽ (*end != '\0')
    libcrun_fail_with_error (0, "invalid %s specified", kind);

  തിരികെ (പൂർണ്ണസംഖ്യ) l;
}

സ്ഥിതം ഘടന argp argp = { options, parse_opt, args_doc, doc, ശൂന്യ, ശൂന്യ, ശൂന്യ };

പൂർണ്ണസംഖ്യ ensure_cloned_ദ്വിമാന (ശൂന്യം);

സ്ഥിതം ശൂന്യം
fill_handler_from_argv0 (അക്ഷരം *argv0, ഘടന crun_global_arguments *args)
{
  സ്ഥിരം അക്ഷരം *b = basename (argv0);
#ifdef HAVE_LIBKRUN
  എങ്കിൽ (strcmp (b, "krun") == 0)
    {
      args->handler = "krun";
      തിരികെ;
    }
#endif
  എങ്കിൽ (strncmp (b, "crun-", 5) == 0 && b[5] != '\0')
    args->handler = b + 5;
}

സ്ഥിതം അക്ഷരം **
copy_args (അക്ഷരം **argv, പൂർണ്ണസംഖ്യ argc)
{
  അക്ഷരം **buff = xmalloc0 ((argc + 1) * വലുപ്പം (അക്ഷരം *));
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < argc; i++)
    buff[i] = argv[i];

  തിരികെ buff;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  libcrun_error_t err = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret, first_argument = 0;

  arguments.argc = argc;
  arguments.argv = argv;

#ifdef DYNLOAD_LIBCRUN
  എങ്കിൽ (ensure_cloned_ദ്വിമാന () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      exit (EXIT_FAILURE);
    }
  /* Resolve all libcrun weak dependencies.  */
  എങ്കിൽ (dlopen ("libcrun.so", RTLD_GLOBAL | RTLD_DEEPBIND | RTLD_LAZY) == ശൂന്യ)
    libcrun_fail_with_error (0, "could not load `libcrun.so`: `%s`", dlerror ());
#endif

  fill_handler_from_argv0 (argv[0], &arguments);
  argp_parse (&argp, argc, argv, ARGP_IN_ORDER, &first_argument, &arguments);

  command = get_command (argv[first_argument]);
  എങ്കിൽ (command == ശൂന്യ)
    libcrun_fail_with_error (0, "unknown command %s", argv[first_argument]);

  പൂർണ്ണസംഖ്യ command_argc = argc - first_argument;
  cleanup_free അക്ഷരം **command_argv = copy_args (argv + first_argument, command_argc);
  command_argv[0] = argv[0];

  ret = command->handler (&arguments, command_argc, command_argv, &err);
  എങ്കിൽ (ret && err)
    {
      എങ്കിൽ (command->മൂല്യം == COMMAND_EXEC)
        {
          libcrun_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));
          exit (EXIT_EXEC_FAILURE);
        }
      libcrun_fail_with_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));
    }

  തിരികെ ret;
}
