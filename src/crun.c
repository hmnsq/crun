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

#ضمن <config.h>
#ضمن <git-version.h>
#ضمن <stdlib.h>
#ضمن <argp.h>
#ضمن <string.h>
#ضمن <libgen.h>
#ضمن <errno.h>
#ضمن <limits.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ضمن "crun.h"

/* Commands.  */
#ضمن "run.h"
#ضمن "delete.h"
#ضمن "kill.h"
#ضمن "list.h"
#ضمن "start.h"
#ضمن "create.h"
#ضمن "exec.h"
#ضمن "state.h"
#ضمن "update.h"
#ضمن "spec.h"
#ضمن "pause.h"
#ضمن "unpause.h"
#ضمن "oci_features.h"
#ضمن "ps.h"
#ضمن "checkpoint.h"
#ضمن "mounts.h"
#ضمن "restore.h"

/* crun exec exits with the exit code of the executed process, so use this
   code for its own errors, to tell those apart (like runc and ssh do).  */
#define EXIT_EXEC_FAILURE 255

ساكن بنية crun_global_arguments arguments;

بنية commands_s
{
  صحيح value;
  ثابت حرف *name;
  صحيح (*handler) (بنية crun_global_arguments *, صحيح, حرف **, libcrun_error_t *);
};

صحيح
init_libcrun_context (libcrun_context_t *con, ثابت حرف *id, بنية crun_global_arguments *glob, libcrun_error_t *err)
{
  صحيح ret;

  libcrun_context_set_id (con, id);
  libcrun_context_set_state_root (con, glob->root);
  libcrun_context_set_systemd_cgroup (con, glob->option_systemd_cgroup);
  libcrun_context_set_force_no_cgroup (con, glob->option_force_no_cgroup);
  libcrun_context_set_notify_socket (con, getenv ("NOTIFY_SOCKET"));
  libcrun_context_set_args (con, glob->argc, glob->argv);

  /* Check if global handler is configured and pass it down to crun context */
  libcrun_context_set_handler (con, glob->handler);

  ret = libcrun_context_init_logging (con, glob->log, err);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;

  ܐܢ (glob->log_format)
    {
      ret = libcrun_set_log_format (glob->log_format, err);
      ܐܢ (UNLIKELY (ret < 0))
        ܦܢܐ ret;
    }

  libcrun_set_verbosity (glob->verbosity);
  libcrun_debug ("Using debug verbosity");

  ܦܢܐ 0;
}

/* Allocate a public libcrun context (with handlers loaded) for a subcommand.
   Terminates the process on failure.  */
libcrun_context_t *
new_libcrun_context (بنية crun_global_arguments *glob)
{
  libcrun_error_t err = عدم;
  libcrun_context_t *con;

  con = libcrun_context_new (عدم, glob->root, &err);
  ܐܢ (UNLIKELY (con == عدم))
    libcrun_fail_with_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));

  ܦܢܐ con;
}

تعداد
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

بنية commands_s commands[] = { { COMMAND_CREATE, "create", crun_command_create },
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
#ܐܢ HAVE_CRIU && HAVE_DLOPEN
                                 { COMMAND_CHECKPOINT, "checkpoint", crun_command_checkpoint },
                                 { COMMAND_RESTORE, "restore", crun_command_restore },
#endif
                                 { COMMAND_MOUNTS, "mounts", crun_command_mounts },
                                 {
                                     0,
                                 } };

ساكن حرف doc[] = "\nCOMMANDS:\n"
#ܐܢ HAVE_CRIU && HAVE_DLOPEN
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
#ܐܢ HAVE_CRIU && HAVE_DLOPEN
                    "\trestore     - restore a container\n"
#endif
                    "\trun         - run a container\n"
                    "\tspec        - generate a configuration file\n"
                    "\tstart       - start a container\n"
                    "\tstate       - output the state of a container\n"
                    "\tpause       - pause all the processes in the container\n"
                    "\tresume      - unpause the processes in the container\n"
                    "\tupdate      - update container resource constraints\n";

ساكن حرف args_doc[] = "COMMAND [OPTION...]";

ساكن بنية commands_s *
get_command (ثابت حرف *arg)
{
  بنية commands_s *it;
  لكل (it = commands; it->value; it++)
    ܐܢ (strcmp (it->name, arg) == 0)
      ܦܢܐ it;
  ܦܢܐ عدم;
}

تعداد
{
  OPTION_VERSION = 'v',
  OPTION_VERSION_CAP = 'V',
  OPTION_DEBUG = 1000,
  OPTION_SYSTEMD_CGROUP,
  OPTION_CGROUP_MANAGER,
  OPTION_LOG,
  OPTION_LOG_FORMAT,
  OPTION_LOG_LEVEL,
  OPTION_ROOT,
  OPTION_ROOTLESS
};

ثابت حرف *argp_program_bug_address = "https://github.com/containers/crun/issues";

ساكن بنية argp_option options[] = { { "debug", OPTION_DEBUG, 0, 0, "produce verbose output, similar to --log-level=debug", 0 },
                                        { "cgroup-manager", OPTION_CGROUP_MANAGER, "MANAGER", 0, "cgroup manager", 0 },
                                        { "systemd-cgroup", OPTION_SYSTEMD_CGROUP, 0, 0, "use systemd cgroups", 0 },
                                        { "log", OPTION_LOG, "FILE", 0, "log destination: '[file:]PATH', 'journald:ID' or 'syslog:ID' (defaults to stderr)", 0 },
                                        { "log-format", OPTION_LOG_FORMAT, "FORMAT", 0, "log format: 'text' (default) or 'json'", 0 },
                                        { "log-level", OPTION_LOG_LEVEL, "LEVEL", 0, "log level to use: 'error' (default), 'warning' or 'debug'", 0 },
                                        { "root", OPTION_ROOT, "DIR", 0, عدم, 0 },
                                        { "rootless", OPTION_ROOTLESS, "VALUE", 0, عدم, 0 },
                                        { "version", OPTION_VERSION, 0, 0, عدم, 0 },
                                        // alias OPTION_VERSION_CAP with OPTION_VERSION
                                        { عدم, OPTION_VERSION_CAP, 0, OPTION_ALIAS, عدم, 0 },
                                        {
                                            0,
                                        } };

ساكن فراغ
print_version (FILE *stream, بنية argp_state *state arg_unused)
{
  libcrun_error_t err = عدم;
  cleanup_free حرف *rundir = عدم;
  libcrun_context_t *ctx;
  حرف **tags;
  صحيح ret;

  fprintf (stream, "%s version %s\n", PACKAGE_NAME, PACKAGE_VERSION);
  fprintf (stream, "commit: %s\n", GIT_VERSION);

  ret = libcrun_get_state_dir (&rundir, arguments.root, عدم, &err);
  ܐܢ (LIKELY (ret == 0))
    fprintf (stream, "rundir: %s\n", rundir);
  ܐܚܪܢܐ
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
#ܐܢ HAVE_CRIU && HAVE_DLOPEN
  fprintf (stream, "+CRIU ");
#endif

  ctx = libcrun_context_new (عدم, arguments.root, &err);
  ܐܢ (ctx)
    {
      tags = libcrun_context_get_handler_feature_tags (ctx, &err);
      ܐܢ (tags)
        {
          لكل (size_t i = 0; tags[i]; i++)
            {
              fprintf (stream, "+%s ", tags[i]);
              حرر (tags[i]);
            }
          حرر (tags);
        }
      ܐܚܪܢܐ
        libcrun_error_release (&err);
      libcrun_context_free (ctx);
    }
  ܐܚܪܢܐ
    libcrun_error_release (&err);

  fprintf (stream, "+JSON_C\n");
}

ساكن error_t
parse_opt (صحيح key, حرف *arg, بنية argp_state *state)
{
  ثابت حرف *tmp;

  بدل (key)
    {
    حالة OPTION_DEBUG:
      arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
      اكسر;

    حالة OPTION_CGROUP_MANAGER:
      tmp = argp_mandatory_argument (arg, state);
      ܐܢ (strcmp (tmp, "systemd") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = true;
        }
      ܐܚܪܢܐ ܐܢ (strcmp (tmp, "cgroupfs") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = false;
        }
      ܐܚܪܢܐ ܐܢ (strcmp (tmp, "disabled") == 0)
        {
          arguments.option_systemd_cgroup = false;
          arguments.option_force_no_cgroup = true;
        }
      ܐܚܪܢܐ
        {
          libcrun_fail_with_error (0, "unknown cgroup manager specified");
        }
      اكسر;

    حالة OPTION_SYSTEMD_CGROUP:
      arguments.option_force_no_cgroup = false;
      arguments.option_systemd_cgroup = true;
      اكسر;

    حالة OPTION_LOG:
      arguments.log = argp_mandatory_argument (arg, state);
      اكسر;

    حالة OPTION_LOG_FORMAT:
      arguments.log_format = argp_mandatory_argument (arg, state);
      اكسر;

    حالة OPTION_LOG_LEVEL:
      tmp = argp_mandatory_argument (arg, state);
      ܐܢ (strcmp (tmp, "error") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_ERROR;
        }
      ܐܚܪܢܐ ܐܢ (strcmp (tmp, "warning") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_WARNING;
        }
      ܐܚܪܢܐ ܐܢ (strcmp (tmp, "debug") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
        }
      ܐܚܪܢܐ
        {
          libcrun_fail_with_error (0, "unknown verbosity `%s` specified", arg);
        }
      اكسر;

    حالة OPTION_ROOT:
      arguments.root = argp_mandatory_argument (arg, state);
      اكسر;

    حالة OPTION_ROOTLESS:
      /* Ignored.  So that a runc command line won't fail.  */
      اكسر;

    حالة ARGP_KEY_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a command");

    حالة OPTION_VERSION:
    حالة OPTION_VERSION_CAP:
      print_version (stdout, state);
      exit (EXIT_SUCCESS);
    افتراضي:
      ܦܢܐ ARGP_ERR_UNKNOWN;
    }

  ܦܢܐ 0;
}

ساكن بنية commands_s *command;

فراغ
crun_assert_n_args (صحيح n, صحيح min, صحيح max)
{
  ܐܢ (min >= 0 && n < min)
    libcrun_fail_with_error (0, "`%s` requires a minimum of %d arguments", command->name, min);
  ܐܢ (max >= 0 && n > max)
    libcrun_fail_with_error (0, "`%s` requires a maximum of %d arguments", command->name, max);
}

حرف *
argp_mandatory_argument (حرف *arg, بنية argp_state *state)
{
  ܐܢ (arg)
    ܦܢܐ arg;
  ܦܢܐ state->argv[state->next++];
}

/* Parse a numeric value from STR, aborting on an out-of-range or malformed
   value.  KIND names the field for error messages.  If ENDPTR is not NULL it
   is set past the parsed number (so the caller can continue parsing, e.g. a
   "UID:GID" pair); otherwise the whole string must be a valid number.  */
صحيح
parse_id_or_fail (ثابت حرف *str, حرف **endptr, ثابت حرف *kind)
{
  حرف *end = عدم;
  long long l;

  errno = 0;
  l = strtoll (str, &end, 10);
  ܐܢ (end == str)
    libcrun_fail_with_error (0, "invalid %s specified", kind);
  ܐܢ (errno == ERANGE)
    libcrun_fail_with_error (0, "invalid %s specified", kind);
  ܐܢ (l < INT_MIN || l > INT_MAX)
    libcrun_fail_with_error (0, "invalid %s specified", kind);

  ܐܢ (endptr)
    *endptr = end;
  ܐܚܪܢܐ ܐܢ (*end != '\0')
    libcrun_fail_with_error (0, "invalid %s specified", kind);

  ܦܢܐ (صحيح) l;
}

ساكن بنية argp argp = { options, parse_opt, args_doc, doc, عدم, عدم, عدم };

صحيح ensure_cloned_binary (فراغ);

ساكن فراغ
fill_handler_from_argv0 (حرف *argv0, بنية crun_global_arguments *args)
{
  ثابت حرف *b = basename (argv0);
#ifdef HAVE_LIBKRUN
  ܐܢ (strcmp (b, "krun") == 0)
    {
      args->handler = "krun";
      ܦܢܐ;
    }
#endif
  ܐܢ (strncmp (b, "crun-", 5) == 0 && b[5] != '\0')
    args->handler = b + 5;
}

ساكن حرف **
copy_args (حرف **argv, صحيح argc)
{
  حرف **buff = xmalloc0 ((argc + 1) * حجم (حرف *));
  لكل (صحيح i = 0; i < argc; i++)
    buff[i] = argv[i];

  ܦܢܐ buff;
}

صحيح
main (صحيح argc, حرف **argv)
{
  libcrun_error_t err = عدم;
  صحيح ret, first_argument = 0;

  arguments.argc = argc;
  arguments.argv = argv;

#ifdef DYNLOAD_LIBCRUN
  ܐܢ (ensure_cloned_binary () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      exit (EXIT_FAILURE);
    }
  /* Resolve all libcrun weak dependencies.  */
  ܐܢ (dlopen ("libcrun.so", RTLD_GLOBAL | RTLD_DEEPBIND | RTLD_LAZY) == عدم)
    libcrun_fail_with_error (0, "could not load `libcrun.so`: `%s`", dlerror ());
#endif

  fill_handler_from_argv0 (argv[0], &arguments);
  argp_parse (&argp, argc, argv, ARGP_IN_ORDER, &first_argument, &arguments);

  command = get_command (argv[first_argument]);
  ܐܢ (command == عدم)
    libcrun_fail_with_error (0, "unknown command %s", argv[first_argument]);

  صحيح command_argc = argc - first_argument;
  cleanup_free حرف **command_argv = copy_args (argv + first_argument, command_argc);
  command_argv[0] = argv[0];

  ret = command->handler (&arguments, command_argc, command_argv, &err);
  ܐܢ (ret && err)
    {
      ܐܢ (command->value == COMMAND_EXEC)
        {
          libcrun_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));
          exit (EXIT_EXEC_FAILURE);
        }
      libcrun_fail_with_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));
    }

  ܦܢܐ ret;
}
