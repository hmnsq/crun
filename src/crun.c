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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <git-version.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <argp.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <libgen.h>
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <limits.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#અન્તર્ભાવ "crun.h"

/* Commands.  */
#અન્તર્ભાવ "run.h"
#અન્તર્ભાવ "delete.h"
#અન્તર્ભાવ "kill.h"
#અન્તર્ભાવ "list.h"
#અન્તર્ભાવ "start.h"
#અન્તર્ભાવ "create.h"
#અન્તર્ભાવ "exec.h"
#અન્તર્ભાવ "state.h"
#અન્તર્ભાવ "update.h"
#અન્તર્ભાવ "spec.h"
#અન્તર્ભાવ "pause.h"
#અન્તર્ભાવ "unpause.h"
#અન્તર્ભાવ "oci_features.h"
#અન્તર્ભાવ "ps.h"
#અન્તર્ભાવ "checkpoint.h"
#અન્તર્ભાવ "mounts.h"
#અન્તર્ભાવ "restore.h"

/* crun exec exits with the exit code of the executed process, so use this
   code for its own errors, to tell those apart (like runc and ssh do).  */
#define EXIT_EXEC_FAILURE 255

સ્થિર સંરચના crun_global_arguments arguments;

સંરચના commands_s
{
  પૂર્ણાંક કિંમત;
  અચલ અક્ષર *name;
  પૂર્ણાંક (*handler) (સંરચના crun_global_arguments *, પૂર્ણાંક, અક્ષર **, libcrun_error_t *);
};

પૂર્ણાંક
init_libcrun_context (libcrun_context_t *con, અચલ અક્ષર *id, સંરચના crun_global_arguments *glob, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  libcrun_context_set_id (con, id);
  libcrun_context_set_state_મૂળ (con, glob->મૂળ);
  libcrun_context_set_systemd_cgroup (con, glob->option_systemd_cgroup);
  libcrun_context_set_force_no_cgroup (con, glob->option_force_no_cgroup);
  libcrun_context_set_notify_socket (con, getenv ("NOTIFY_SOCKET"));
  libcrun_context_set_args (con, glob->argc, glob->argv);

  /* Check if global handler is configured and pass it down to crun context */
  libcrun_context_set_handler (con, glob->handler);

  ret = libcrun_context_init_logging (con, glob->log, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (glob->log_format)
    {
      ret = libcrun_set_log_format (glob->log_format, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  libcrun_set_verbosity (glob->verbosity);
  libcrun_debug ("Using debug verbosity");

  પાછા 0;
}

/* Allocate a public libcrun context (with handlers loaded) for a subcommand.
   Terminates the process on failure.  */
libcrun_context_t *
new_libcrun_context (સંરચના crun_global_arguments *glob)
{
  libcrun_error_t err = ખાલી;
  libcrun_context_t *con;

  con = libcrun_context_new (ખાલી, glob->મૂળ, &err);
  જો (UNLIKELY (con == ખાલી))
    libcrun_fail_with_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));

  પાછા con;
}

ગણના
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

સંરચના commands_s commands[] = { { COMMAND_CREATE, "create", crun_command_create },
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
#જો HAVE_CRIU && HAVE_DLOPEN
                                 { COMMAND_CHECKPOINT, "checkpoint", crun_command_checkpoint },
                                 { COMMAND_RESTORE, "restore", crun_command_restore },
#endif
                                 { COMMAND_MOUNTS, "mounts", crun_command_mounts },
                                 {
                                     0,
                                 } };

સ્થિર અક્ષર doc[] = "\nCOMMANDS:\n"
#જો HAVE_CRIU && HAVE_DLOPEN
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
#જો HAVE_CRIU && HAVE_DLOPEN
                    "\trestore     - restore a container\n"
#endif
                    "\trun         - run a container\n"
                    "\tspec        - generate a configuration file\n"
                    "\tstart       - start a container\n"
                    "\tstate       - output the state of a container\n"
                    "\tpause       - pause all the processes in the container\n"
                    "\tresume      - unpause the processes in the container\n"
                    "\tupdate      - update container resource constraints\n";

સ્થિર અક્ષર args_doc[] = "COMMAND [OPTION...]";

સ્થિર સંરચના commands_s *
get_command (અચલ અક્ષર *arg)
{
  સંરચના commands_s *it;
  માટે (it = commands; it->કિંમત; it++)
    જો (strcmp (it->name, arg) == 0)
      પાછા it;
  પાછા ખાલી;
}

ગણના
{
  OPTION_VERSION = 'v',
  OPTION_VERSION_CAP = 'V',
  OPTION_DEBUG = 1000,
  OPTION_SYSTEMD_CGROUP,
  OPTION_CGROUP_MANAGER,
  OPTION_LOG,
  OPTION_LOG_FORMAT,
  OPTION_LOG_LEVEL,
  OPTION_મૂળ,
  OPTION_ROOTLESS
};

અચલ અક્ષર *argp_program_bug_address = "https://github.com/containers/crun/issues";

સ્થિર સંરચના argp_option options[] = { { "debug", OPTION_DEBUG, 0, 0, "produce verbose output, similar to --log-level=debug", 0 },
                                        { "cgroup-manager", OPTION_CGROUP_MANAGER, "MANAGER", 0, "cgroup manager", 0 },
                                        { "systemd-cgroup", OPTION_SYSTEMD_CGROUP, 0, 0, "use systemd cgroups", 0 },
                                        { "log", OPTION_LOG, "FILE", 0, "log destination: '[file:]PATH', 'journald:ID' or 'syslog:ID' (defaults to stderr)", 0 },
                                        { "log-format", OPTION_LOG_FORMAT, "FORMAT", 0, "log format: 'text' (default) or 'json'", 0 },
                                        { "log-level", OPTION_LOG_LEVEL, "LEVEL", 0, "log level to use: 'error' (default), 'warning' or 'debug'", 0 },
                                        { "root", OPTION_મૂળ, "DIR", 0, ખાલી, 0 },
                                        { "rootless", OPTION_ROOTLESS, "VALUE", 0, ખાલી, 0 },
                                        { "version", OPTION_VERSION, 0, 0, ખાલી, 0 },
                                        // alias OPTION_VERSION_CAP with OPTION_VERSION
                                        { ખાલી, OPTION_VERSION_CAP, 0, OPTION_ALIAS, ખાલી, 0 },
                                        {
                                            0,
                                        } };

સ્થિર શૂન્ય
print_version (FILE *stream, સંરચના argp_state *state arg_unused)
{
  libcrun_error_t err = ખાલી;
  cleanup_free અક્ષર *rundir = ખાલી;
  libcrun_context_t *ctx;
  અક્ષર **tags;
  પૂર્ણાંક ret;

  fprintf (stream, "%s version %s\n", PACKAGE_NAME, PACKAGE_VERSION);
  fprintf (stream, "commit: %s\n", GIT_VERSION);

  ret = libcrun_get_state_dir (&rundir, arguments.મૂળ, ખાલી, &err);
  જો (LIKELY (ret == 0))
    fprintf (stream, "rundir: %s\n", rundir);
  નહીંતર
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
#જો HAVE_CRIU && HAVE_DLOPEN
  fprintf (stream, "+CRIU ");
#endif

  ctx = libcrun_context_new (ખાલી, arguments.મૂળ, &err);
  જો (ctx)
    {
      tags = libcrun_context_get_handler_feature_tags (ctx, &err);
      જો (tags)
        {
          માટે (size_t i = 0; tags[i]; i++)
            {
              fprintf (stream, "+%s ", tags[i]);
              મુક્ત (tags[i]);
            }
          મુક્ત (tags);
        }
      નહીંતર
        libcrun_error_release (&err);
      libcrun_context_free (ctx);
    }
  નહીંતર
    libcrun_error_release (&err);

  fprintf (stream, "+JSON_C\n");
}

સ્થિર error_t
parse_opt (પૂર્ણાંક ચાવી, અક્ષર *arg, સંરચના argp_state *state)
{
  અચલ અક્ષર *tmp;

  પસંદગી (ચાવી)
    {
    સ્થિતિ OPTION_DEBUG:
      arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
      તોડો;

    સ્થિતિ OPTION_CGROUP_MANAGER:
      tmp = argp_mandatory_argument (arg, state);
      જો (strcmp (tmp, "systemd") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = true;
        }
      નહીંતર જો (strcmp (tmp, "cgroupfs") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = false;
        }
      નહીંતર જો (strcmp (tmp, "disabled") == 0)
        {
          arguments.option_systemd_cgroup = false;
          arguments.option_force_no_cgroup = true;
        }
      નહીંતર
        {
          libcrun_fail_with_error (0, "unknown cgroup manager specified");
        }
      તોડો;

    સ્થિતિ OPTION_SYSTEMD_CGROUP:
      arguments.option_force_no_cgroup = false;
      arguments.option_systemd_cgroup = true;
      તોડો;

    સ્થિતિ OPTION_LOG:
      arguments.log = argp_mandatory_argument (arg, state);
      તોડો;

    સ્થિતિ OPTION_LOG_FORMAT:
      arguments.log_format = argp_mandatory_argument (arg, state);
      તોડો;

    સ્થિતિ OPTION_LOG_LEVEL:
      tmp = argp_mandatory_argument (arg, state);
      જો (strcmp (tmp, "error") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_ERROR;
        }
      નહીંતર જો (strcmp (tmp, "warning") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_WARNING;
        }
      નહીંતર જો (strcmp (tmp, "debug") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
        }
      નહીંતર
        {
          libcrun_fail_with_error (0, "unknown verbosity `%s` specified", arg);
        }
      તોડો;

    સ્થિતિ OPTION_મૂળ:
      arguments.મૂળ = argp_mandatory_argument (arg, state);
      તોડો;

    સ્થિતિ OPTION_ROOTLESS:
      /* Ignored.  So that a runc command line won't fail.  */
      તોડો;

    સ્થિતિ ARGP_ચાવી_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a command");

    સ્થિતિ OPTION_VERSION:
    સ્થિતિ OPTION_VERSION_CAP:
      print_version (stdout, state);
      exit (EXIT_SUCCESS);
    મૂળભૂત:
      પાછા ARGP_ERR_UNKNOWN;
    }

  પાછા 0;
}

સ્થિર સંરચના commands_s *command;

શૂન્ય
crun_assert_n_args (પૂર્ણાંક n, પૂર્ણાંક min, પૂર્ણાંક max)
{
  જો (min >= 0 && n < min)
    libcrun_fail_with_error (0, "`%s` requires a minimum of %d arguments", command->name, min);
  જો (max >= 0 && n > max)
    libcrun_fail_with_error (0, "`%s` requires a maximum of %d arguments", command->name, max);
}

અક્ષર *
argp_mandatory_argument (અક્ષર *arg, સંરચના argp_state *state)
{
  જો (arg)
    પાછા arg;
  પાછા state->argv[state->next++];
}

/* Parse a numeric value from STR, aborting on an out-of-range or malformed
   value.  KIND names the field for error messages.  If ENDPTR is not NULL it
   is set past the parsed number (so the caller can continue parsing, e.g. a
   "UID:GID" pair); otherwise the whole string must be a valid number.  */
પૂર્ણાંક
parse_id_or_fail (અચલ અક્ષર *str, અક્ષર **endptr, અચલ અક્ષર *kind)
{
  અક્ષર *end = ખાલી;
  long long l;

  errno = 0;
  l = strtoll (str, &end, 10);
  જો (end == str)
    libcrun_fail_with_error (0, "invalid %s specified", kind);
  જો (errno == ERANGE)
    libcrun_fail_with_error (0, "invalid %s specified", kind);
  જો (l < INT_MIN || l > INT_MAX)
    libcrun_fail_with_error (0, "invalid %s specified", kind);

  જો (endptr)
    *endptr = end;
  નહીંતર જો (*end != '\0')
    libcrun_fail_with_error (0, "invalid %s specified", kind);

  પાછા (પૂર્ણાંક) l;
}

સ્થિર સંરચના argp argp = { options, parse_opt, args_doc, doc, ખાલી, ખાલી, ખાલી };

પૂર્ણાંક ensure_cloned_દ્વિઅંકી (શૂન્ય);

સ્થિર શૂન્ય
fill_handler_from_argv0 (અક્ષર *argv0, સંરચના crun_global_arguments *args)
{
  અચલ અક્ષર *b = basename (argv0);
#ifdef HAVE_LIBKRUN
  જો (strcmp (b, "krun") == 0)
    {
      args->handler = "krun";
      પાછા;
    }
#endif
  જો (strncmp (b, "crun-", 5) == 0 && b[5] != '\0')
    args->handler = b + 5;
}

સ્થિર અક્ષર **
copy_args (અક્ષર **argv, પૂર્ણાંક argc)
{
  અક્ષર **buff = xmalloc0 ((argc + 1) * કદ (અક્ષર *));
  માટે (પૂર્ણાંક i = 0; i < argc; i++)
    buff[i] = argv[i];

  પાછા buff;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  libcrun_error_t err = ખાલી;
  પૂર્ણાંક ret, first_argument = 0;

  arguments.argc = argc;
  arguments.argv = argv;

#ifdef DYNLOAD_LIBCRUN
  જો (ensure_cloned_દ્વિઅંકી () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      exit (EXIT_FAILURE);
    }
  /* Resolve all libcrun weak dependencies.  */
  જો (dlopen ("libcrun.so", RTLD_GLOBAL | RTLD_DEEPBIND | RTLD_LAZY) == ખાલી)
    libcrun_fail_with_error (0, "could not load `libcrun.so`: `%s`", dlerror ());
#endif

  fill_handler_from_argv0 (argv[0], &arguments);
  argp_parse (&argp, argc, argv, ARGP_IN_ORDER, &first_argument, &arguments);

  command = get_command (argv[first_argument]);
  જો (command == ખાલી)
    libcrun_fail_with_error (0, "unknown command %s", argv[first_argument]);

  પૂર્ણાંક command_argc = argc - first_argument;
  cleanup_free અક્ષર **command_argv = copy_args (argv + first_argument, command_argc);
  command_argv[0] = argv[0];

  ret = command->handler (&arguments, command_argc, command_argv, &err);
  જો (ret && err)
    {
      જો (command->કિંમત == COMMAND_EXEC)
        {
          libcrun_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));
          exit (EXIT_EXEC_FAILURE);
        }
      libcrun_fail_with_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));
    }

  પાછા ret;
}
