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
#ଅନ୍ତର୍ଭାଵ <git-version.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <argp.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <libgen.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <limits.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ଅନ୍ତର୍ଭାଵ "crun.h"

/* Commands.  */
#ଅନ୍ତର୍ଭାଵ "run.h"
#ଅନ୍ତର୍ଭାଵ "delete.h"
#ଅନ୍ତର୍ଭାଵ "kill.h"
#ଅନ୍ତର୍ଭାଵ "list.h"
#ଅନ୍ତର୍ଭାଵ "start.h"
#ଅନ୍ତର୍ଭାଵ "create.h"
#ଅନ୍ତର୍ଭାଵ "exec.h"
#ଅନ୍ତର୍ଭାଵ "state.h"
#ଅନ୍ତର୍ଭାଵ "update.h"
#ଅନ୍ତର୍ଭାଵ "spec.h"
#ଅନ୍ତର୍ଭାଵ "pause.h"
#ଅନ୍ତର୍ଭାଵ "unpause.h"
#ଅନ୍ତର୍ଭାଵ "oci_features.h"
#ଅନ୍ତର୍ଭାଵ "ps.h"
#ଅନ୍ତର୍ଭାଵ "checkpoint.h"
#ଅନ୍ତର୍ଭାଵ "mounts.h"
#ଅନ୍ତର୍ଭାଵ "restore.h"

/* crun exec exits with the exit code of the executed process, so use this
   code for its own errors, to tell those apart (like runc and ssh do).  */
#define EXIT_EXEC_FAILURE 255

ସ୍ଥିର ସଂରଚନା crun_global_arguments arguments;

ସଂରଚନା commands_s
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ;
  ଅଚଳ ଅକ୍ଷର *name;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*handler) (ସଂରଚନା crun_global_arguments *, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଅକ୍ଷର **, libcrun_error_t *);
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
init_libcrun_context (libcrun_context_t *con, ଅଚଳ ଅକ୍ଷର *id, ସଂରଚନା crun_global_arguments *glob, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  libcrun_context_set_id (con, id);
  libcrun_context_set_state_ମୂଳ (con, glob->ମୂଳ);
  libcrun_context_set_systemd_cgroup (con, glob->option_systemd_cgroup);
  libcrun_context_set_force_no_cgroup (con, glob->option_force_no_cgroup);
  libcrun_context_set_notify_socket (con, getenv ("NOTIFY_SOCKET"));
  libcrun_context_set_args (con, glob->argc, glob->argv);

  /* Check if global handler is configured and pass it down to crun context */
  libcrun_context_set_handler (con, glob->handler);

  ret = libcrun_context_init_logging (con, glob->log, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (glob->log_format)
    {
      ret = libcrun_set_log_format (glob->log_format, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  libcrun_set_verbosity (glob->verbosity);
  libcrun_debug ("Using debug verbosity");

  ଫେରନ୍ତୁ 0;
}

/* Allocate a public libcrun context (with handlers loaded) for a subcommand.
   Terminates the process on failure.  */
libcrun_context_t *
new_libcrun_context (ସଂରଚନା crun_global_arguments *glob)
{
  libcrun_error_t err = ଖାଲି;
  libcrun_context_t *con;

  con = libcrun_context_new (ଖାଲି, glob->ମୂଳ, &err);
  ଯଦି (UNLIKELY (con == ଖାଲି))
    libcrun_fail_with_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));

  ଫେରନ୍ତୁ con;
}

ଗଣନା
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

ସଂରଚନା commands_s commands[] = { { COMMAND_CREATE, "create", crun_command_create },
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
#ଯଦି HAVE_CRIU && HAVE_DLOPEN
                                 { COMMAND_CHECKPOINT, "checkpoint", crun_command_checkpoint },
                                 { COMMAND_RESTORE, "restore", crun_command_restore },
#endif
                                 { COMMAND_MOUNTS, "mounts", crun_command_mounts },
                                 {
                                     0,
                                 } };

ସ୍ଥିର ଅକ୍ଷର doc[] = "\nCOMMANDS:\n"
#ଯଦି HAVE_CRIU && HAVE_DLOPEN
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
#ଯଦି HAVE_CRIU && HAVE_DLOPEN
                    "\trestore     - restore a container\n"
#endif
                    "\trun         - run a container\n"
                    "\tspec        - generate a configuration file\n"
                    "\tstart       - start a container\n"
                    "\tstate       - output the state of a container\n"
                    "\tpause       - pause all the processes in the container\n"
                    "\tresume      - unpause the processes in the container\n"
                    "\tupdate      - update container resource constraints\n";

ସ୍ଥିର ଅକ୍ଷର args_doc[] = "COMMAND [OPTION...]";

ସ୍ଥିର ସଂରଚନା commands_s *
get_command (ଅଚଳ ଅକ୍ଷର *arg)
{
  ସଂରଚନା commands_s *it;
  ପାଇଁ (it = commands; it->ମୂଲ୍ୟ; it++)
    ଯଦି (strcmp (it->name, arg) == 0)
      ଫେରନ୍ତୁ it;
  ଫେରନ୍ତୁ ଖାଲି;
}

ଗଣନା
{
  OPTION_VERSION = 'v',
  OPTION_VERSION_CAP = 'V',
  OPTION_DEBUG = 1000,
  OPTION_SYSTEMD_CGROUP,
  OPTION_CGROUP_MANAGER,
  OPTION_LOG,
  OPTION_LOG_FORMAT,
  OPTION_LOG_LEVEL,
  OPTION_ମୂଳ,
  OPTION_ROOTLESS
};

ଅଚଳ ଅକ୍ଷର *argp_program_bug_address = "https://github.com/containers/crun/issues";

ସ୍ଥିର ସଂରଚନା argp_option options[] = { { "debug", OPTION_DEBUG, 0, 0, "produce verbose output, similar to --log-level=debug", 0 },
                                        { "cgroup-manager", OPTION_CGROUP_MANAGER, "MANAGER", 0, "cgroup manager", 0 },
                                        { "systemd-cgroup", OPTION_SYSTEMD_CGROUP, 0, 0, "use systemd cgroups", 0 },
                                        { "log", OPTION_LOG, "FILE", 0, "log destination: '[file:]PATH', 'journald:ID' or 'syslog:ID' (defaults to stderr)", 0 },
                                        { "log-format", OPTION_LOG_FORMAT, "FORMAT", 0, "log format: 'text' (default) or 'json'", 0 },
                                        { "log-level", OPTION_LOG_LEVEL, "LEVEL", 0, "log level to use: 'error' (default), 'warning' or 'debug'", 0 },
                                        { "root", OPTION_ମୂଳ, "DIR", 0, ଖାଲି, 0 },
                                        { "rootless", OPTION_ROOTLESS, "VALUE", 0, ଖାଲି, 0 },
                                        { "version", OPTION_VERSION, 0, 0, ଖାଲି, 0 },
                                        // alias OPTION_VERSION_CAP with OPTION_VERSION
                                        { ଖାଲି, OPTION_VERSION_CAP, 0, OPTION_ALIAS, ଖାଲି, 0 },
                                        {
                                            0,
                                        } };

ସ୍ଥିର ଶୂନ୍ୟ
print_version (FILE *stream, ସଂରଚନା argp_state *state arg_unused)
{
  libcrun_error_t err = ଖାଲି;
  cleanup_free ଅକ୍ଷର *rundir = ଖାଲି;
  libcrun_context_t *ctx;
  ଅକ୍ଷର **tags;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  fprintf (stream, "%s version %s\n", PACKAGE_NAME, PACKAGE_VERSION);
  fprintf (stream, "commit: %s\n", GIT_VERSION);

  ret = libcrun_get_state_dir (&rundir, arguments.ମୂଳ, ଖାଲି, &err);
  ଯଦି (LIKELY (ret == 0))
    fprintf (stream, "rundir: %s\n", rundir);
  ଅନ୍ୟଥା
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
#ଯଦି HAVE_CRIU && HAVE_DLOPEN
  fprintf (stream, "+CRIU ");
#endif

  ctx = libcrun_context_new (ଖାଲି, arguments.ମୂଳ, &err);
  ଯଦି (ctx)
    {
      tags = libcrun_context_get_handler_feature_tags (ctx, &err);
      ଯଦି (tags)
        {
          ପାଇଁ (size_t i = 0; tags[i]; i++)
            {
              fprintf (stream, "+%s ", tags[i]);
              ମୁକ୍ତ (tags[i]);
            }
          ମୁକ୍ତ (tags);
        }
      ଅନ୍ୟଥା
        libcrun_error_release (&err);
      libcrun_context_free (ctx);
    }
  ଅନ୍ୟଥା
    libcrun_error_release (&err);

  fprintf (stream, "+JSON_C\n");
}

ସ୍ଥିର error_t
parse_opt (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଚାବି, ଅକ୍ଷର *arg, ସଂରଚନା argp_state *state)
{
  ଅଚଳ ଅକ୍ଷର *tmp;

  ବାଛ (ଚାବି)
    {
    ସ୍ଥିତି OPTION_DEBUG:
      arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_CGROUP_MANAGER:
      tmp = argp_mandatory_argument (arg, state);
      ଯଦି (strcmp (tmp, "systemd") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = true;
        }
      ଅନ୍ୟଥା ଯଦି (strcmp (tmp, "cgroupfs") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = false;
        }
      ଅନ୍ୟଥା ଯଦି (strcmp (tmp, "disabled") == 0)
        {
          arguments.option_systemd_cgroup = false;
          arguments.option_force_no_cgroup = true;
        }
      ଅନ୍ୟଥା
        {
          libcrun_fail_with_error (0, "unknown cgroup manager specified");
        }
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_SYSTEMD_CGROUP:
      arguments.option_force_no_cgroup = false;
      arguments.option_systemd_cgroup = true;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_LOG:
      arguments.log = argp_mandatory_argument (arg, state);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_LOG_FORMAT:
      arguments.log_format = argp_mandatory_argument (arg, state);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_LOG_LEVEL:
      tmp = argp_mandatory_argument (arg, state);
      ଯଦି (strcmp (tmp, "error") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_ERROR;
        }
      ଅନ୍ୟଥା ଯଦି (strcmp (tmp, "warning") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_WARNING;
        }
      ଅନ୍ୟଥା ଯଦି (strcmp (tmp, "debug") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
        }
      ଅନ୍ୟଥା
        {
          libcrun_fail_with_error (0, "unknown verbosity `%s` specified", arg);
        }
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_ମୂଳ:
      arguments.ମୂଳ = argp_mandatory_argument (arg, state);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_ROOTLESS:
      /* Ignored.  So that a runc command line won't fail.  */
      ଭାଙ୍ଗ;

    ସ୍ଥିତି ARGP_ଚାବି_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a command");

    ସ୍ଥିତି OPTION_VERSION:
    ସ୍ଥିତି OPTION_VERSION_CAP:
      print_version (stdout, state);
      exit (EXIT_SUCCESS);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ ARGP_ERR_UNKNOWN;
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ସଂରଚନା commands_s *command;

ଶୂନ୍ୟ
crun_assert_n_args (ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max)
{
  ଯଦି (min >= 0 && n < min)
    libcrun_fail_with_error (0, "`%s` requires a minimum of %d arguments", command->name, min);
  ଯଦି (max >= 0 && n > max)
    libcrun_fail_with_error (0, "`%s` requires a maximum of %d arguments", command->name, max);
}

ଅକ୍ଷର *
argp_mandatory_argument (ଅକ୍ଷର *arg, ସଂରଚନା argp_state *state)
{
  ଯଦି (arg)
    ଫେରନ୍ତୁ arg;
  ଫେରନ୍ତୁ state->argv[state->next++];
}

/* Parse a numeric value from STR, aborting on an out-of-range or malformed
   value.  KIND names the field for error messages.  If ENDPTR is not NULL it
   is set past the parsed number (so the caller can continue parsing, e.g. a
   "UID:GID" pair); otherwise the whole string must be a valid number.  */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
parse_id_or_fail (ଅଚଳ ଅକ୍ଷର *str, ଅକ୍ଷର **endptr, ଅଚଳ ଅକ୍ଷର *kind)
{
  ଅକ୍ଷର *end = ଖାଲି;
  long long l;

  errno = 0;
  l = strtoll (str, &end, 10);
  ଯଦି (end == str)
    libcrun_fail_with_error (0, "invalid %s specified", kind);
  ଯଦି (errno == ERANGE)
    libcrun_fail_with_error (0, "invalid %s specified", kind);
  ଯଦି (l < INT_MIN || l > INT_MAX)
    libcrun_fail_with_error (0, "invalid %s specified", kind);

  ଯଦି (endptr)
    *endptr = end;
  ଅନ୍ୟଥା ଯଦି (*end != '\0')
    libcrun_fail_with_error (0, "invalid %s specified", kind);

  ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) l;
}

ସ୍ଥିର ସଂରଚନା argp argp = { options, parse_opt, args_doc, doc, ଖାଲି, ଖାଲି, ଖାଲି };

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ensure_cloned_ଦ୍ୱିମିକ (ଶୂନ୍ୟ);

ସ୍ଥିର ଶୂନ୍ୟ
fill_handler_from_argv0 (ଅକ୍ଷର *argv0, ସଂରଚନା crun_global_arguments *args)
{
  ଅଚଳ ଅକ୍ଷର *b = basename (argv0);
#ifdef HAVE_LIBKRUN
  ଯଦି (strcmp (b, "krun") == 0)
    {
      args->handler = "krun";
      ଫେରନ୍ତୁ;
    }
#endif
  ଯଦି (strncmp (b, "crun-", 5) == 0 && b[5] != '\0')
    args->handler = b + 5;
}

ସ୍ଥିର ଅକ୍ଷର **
copy_args (ଅକ୍ଷର **argv, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc)
{
  ଅକ୍ଷର **buff = xmalloc0 ((argc + 1) * ଆକାର (ଅକ୍ଷର *));
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < argc; i++)
    buff[i] = argv[i];

  ଫେରନ୍ତୁ buff;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  libcrun_error_t err = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret, first_argument = 0;

  arguments.argc = argc;
  arguments.argv = argv;

#ifdef DYNLOAD_LIBCRUN
  ଯଦି (ensure_cloned_ଦ୍ୱିମିକ () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      exit (EXIT_FAILURE);
    }
  /* Resolve all libcrun weak dependencies.  */
  ଯଦି (dlopen ("libcrun.so", RTLD_GLOBAL | RTLD_DEEPBIND | RTLD_LAZY) == ଖାଲି)
    libcrun_fail_with_error (0, "could not load `libcrun.so`: `%s`", dlerror ());
#endif

  fill_handler_from_argv0 (argv[0], &arguments);
  argp_parse (&argp, argc, argv, ARGP_IN_ORDER, &first_argument, &arguments);

  command = get_command (argv[first_argument]);
  ଯଦି (command == ଖାଲି)
    libcrun_fail_with_error (0, "unknown command %s", argv[first_argument]);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା command_argc = argc - first_argument;
  cleanup_free ଅକ୍ଷର **command_argv = copy_args (argv + first_argument, command_argc);
  command_argv[0] = argv[0];

  ret = command->handler (&arguments, command_argc, command_argv, &err);
  ଯଦି (ret && err)
    {
      ଯଦି (command->ମୂଲ୍ୟ == COMMAND_EXEC)
        {
          libcrun_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));
          exit (EXIT_EXEC_FAILURE);
        }
      libcrun_fail_with_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));
    }

  ଫେରନ୍ତୁ ret;
}
