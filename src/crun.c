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

#شامل <config.h>
#شامل <git-version.h>
#شامل <stdlib.h>
#شامل <argp.h>
#شامل <string.h>
#شامل <libgen.h>
#شامل <errno.h>
#شامل <limits.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#شامل "crun.h"

/* Commands.  */
#شامل "run.h"
#شامل "delete.h"
#شامل "kill.h"
#شامل "list.h"
#شامل "start.h"
#شامل "create.h"
#شامل "exec.h"
#شامل "state.h"
#شامل "update.h"
#شامل "spec.h"
#شامل "pause.h"
#شامل "unpause.h"
#شامل "oci_features.h"
#شامل "ps.h"
#شامل "checkpoint.h"
#شامل "mounts.h"
#شامل "restore.h"

/* crun exec exits with the exit code of the executed process, so use this
   code for its own errors, to tell those apart (like runc and ssh do).  */
#define EXIT_EXEC_FAILURE 255

ایستا ساختار crun_global_arguments arguments;

ساختار commands_s
{
  صحیح مقدار;
  ثابت نویسه *name;
  صحیح (*handler) (ساختار crun_global_arguments *, صحیح, نویسه **, libcrun_error_t *);
};

صحیح
init_libcrun_context (libcrun_context_t *con, ثابت نویسه *id, ساختار crun_global_arguments *glob, libcrun_error_t *err)
{
  صحیح ret;

  libcrun_context_set_id (con, id);
  libcrun_context_set_state_ریشه (con, glob->ریشه);
  libcrun_context_set_systemd_cgroup (con, glob->option_systemd_cgroup);
  libcrun_context_set_force_no_cgroup (con, glob->option_force_no_cgroup);
  libcrun_context_set_notify_socket (con, getenv ("NOTIFY_SOCKET"));
  libcrun_context_set_args (con, glob->argc, glob->argv);

  /* Check if global handler is configured and pass it down to crun context */
  libcrun_context_set_handler (con, glob->handler);

  ret = libcrun_context_init_logging (con, glob->log, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (glob->log_format)
    {
      ret = libcrun_set_log_format (glob->log_format, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  libcrun_set_verbosity (glob->verbosity);
  libcrun_debug ("Using debug verbosity");

  بازگشت 0;
}

/* Allocate a public libcrun context (with handlers loaded) for a subcommand.
   Terminates the process on failure.  */
libcrun_context_t *
new_libcrun_context (ساختار crun_global_arguments *glob)
{
  libcrun_error_t err = پوچ;
  libcrun_context_t *con;

  con = libcrun_context_new (پوچ, glob->ریشه, &err);
  اگر (UNLIKELY (con == پوچ))
    libcrun_fail_with_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));

  بازگشت con;
}

شمارش
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

ساختار commands_s commands[] = { { COMMAND_CREATE, "create", crun_command_create },
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
#اگر HAVE_CRIU && HAVE_DLOPEN
                                 { COMMAND_CHECKPOINT, "checkpoint", crun_command_checkpoint },
                                 { COMMAND_RESTORE, "restore", crun_command_restore },
#endif
                                 { COMMAND_MOUNTS, "mounts", crun_command_mounts },
                                 {
                                     0,
                                 } };

ایستا نویسه doc[] = "\nCOMMANDS:\n"
#اگر HAVE_CRIU && HAVE_DLOPEN
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
#اگر HAVE_CRIU && HAVE_DLOPEN
                    "\trestore     - restore a container\n"
#endif
                    "\trun         - run a container\n"
                    "\tspec        - generate a configuration file\n"
                    "\tstart       - start a container\n"
                    "\tstate       - output the state of a container\n"
                    "\tpause       - pause all the processes in the container\n"
                    "\tresume      - unpause the processes in the container\n"
                    "\tupdate      - update container resource constraints\n";

ایستا نویسه args_doc[] = "COMMAND [OPTION...]";

ایستا ساختار commands_s *
get_command (ثابت نویسه *arg)
{
  ساختار commands_s *it;
  برای (it = commands; it->مقدار; it++)
    اگر (strcmp (it->name, arg) == 0)
      بازگشت it;
  بازگشت پوچ;
}

شمارش
{
  OPTION_VERSION = 'v',
  OPTION_VERSION_CAP = 'V',
  OPTION_DEBUG = 1000,
  OPTION_SYSTEMD_CGROUP,
  OPTION_CGROUP_MANAGER,
  OPTION_LOG,
  OPTION_LOG_FORMAT,
  OPTION_LOG_LEVEL,
  OPTION_ریشه,
  OPTION_ROOTLESS
};

ثابت نویسه *argp_program_bug_address = "https://github.com/containers/crun/issues";

ایستا ساختار argp_option options[] = { { "debug", OPTION_DEBUG, 0, 0, "produce verbose output, similar to --log-level=debug", 0 },
                                        { "cgroup-manager", OPTION_CGROUP_MANAGER, "MANAGER", 0, "cgroup manager", 0 },
                                        { "systemd-cgroup", OPTION_SYSTEMD_CGROUP, 0, 0, "use systemd cgroups", 0 },
                                        { "log", OPTION_LOG, "FILE", 0, "log destination: '[file:]PATH', 'journald:ID' or 'syslog:ID' (defaults to stderr)", 0 },
                                        { "log-format", OPTION_LOG_FORMAT, "FORMAT", 0, "log format: 'text' (default) or 'json'", 0 },
                                        { "log-level", OPTION_LOG_LEVEL, "LEVEL", 0, "log level to use: 'error' (default), 'warning' or 'debug'", 0 },
                                        { "root", OPTION_ریشه, "DIR", 0, پوچ, 0 },
                                        { "rootless", OPTION_ROOTLESS, "VALUE", 0, پوچ, 0 },
                                        { "version", OPTION_VERSION, 0, 0, پوچ, 0 },
                                        // alias OPTION_VERSION_CAP with OPTION_VERSION
                                        { پوچ, OPTION_VERSION_CAP, 0, OPTION_ALIAS, پوچ, 0 },
                                        {
                                            0,
                                        } };

ایستا تهی
print_version (FILE *stream, ساختار argp_state *state arg_unused)
{
  libcrun_error_t err = پوچ;
  cleanup_free نویسه *rundir = پوچ;
  libcrun_context_t *ctx;
  نویسه **tags;
  صحیح ret;

  fprintf (stream, "%s version %s\n", PACKAGE_NAME, PACKAGE_VERSION);
  fprintf (stream, "commit: %s\n", GIT_VERSION);

  ret = libcrun_get_state_dir (&rundir, arguments.ریشه, پوچ, &err);
  اگر (LIKELY (ret == 0))
    fprintf (stream, "rundir: %s\n", rundir);
  وگرنه
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
#اگر HAVE_CRIU && HAVE_DLOPEN
  fprintf (stream, "+CRIU ");
#endif

  ctx = libcrun_context_new (پوچ, arguments.ریشه, &err);
  اگر (ctx)
    {
      tags = libcrun_context_get_handler_feature_tags (ctx, &err);
      اگر (tags)
        {
          برای (size_t i = 0; tags[i]; i++)
            {
              fprintf (stream, "+%s ", tags[i]);
              آزاد (tags[i]);
            }
          آزاد (tags);
        }
      وگرنه
        libcrun_error_release (&err);
      libcrun_context_free (ctx);
    }
  وگرنه
    libcrun_error_release (&err);

  fprintf (stream, "+JSON_C\n");
}

ایستا error_t
parse_opt (صحیح کلید, نویسه *arg, ساختار argp_state *state)
{
  ثابت نویسه *tmp;

  انتخاب (کلید)
    {
    حالت OPTION_DEBUG:
      arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
      بشکن;

    حالت OPTION_CGROUP_MANAGER:
      tmp = argp_mandatory_argument (arg, state);
      اگر (strcmp (tmp, "systemd") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = true;
        }
      وگرنه اگر (strcmp (tmp, "cgroupfs") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = false;
        }
      وگرنه اگر (strcmp (tmp, "disabled") == 0)
        {
          arguments.option_systemd_cgroup = false;
          arguments.option_force_no_cgroup = true;
        }
      وگرنه
        {
          libcrun_fail_with_error (0, "unknown cgroup manager specified");
        }
      بشکن;

    حالت OPTION_SYSTEMD_CGROUP:
      arguments.option_force_no_cgroup = false;
      arguments.option_systemd_cgroup = true;
      بشکن;

    حالت OPTION_LOG:
      arguments.log = argp_mandatory_argument (arg, state);
      بشکن;

    حالت OPTION_LOG_FORMAT:
      arguments.log_format = argp_mandatory_argument (arg, state);
      بشکن;

    حالت OPTION_LOG_LEVEL:
      tmp = argp_mandatory_argument (arg, state);
      اگر (strcmp (tmp, "error") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_ERROR;
        }
      وگرنه اگر (strcmp (tmp, "warning") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_WARNING;
        }
      وگرنه اگر (strcmp (tmp, "debug") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
        }
      وگرنه
        {
          libcrun_fail_with_error (0, "unknown verbosity `%s` specified", arg);
        }
      بشکن;

    حالت OPTION_ریشه:
      arguments.ریشه = argp_mandatory_argument (arg, state);
      بشکن;

    حالت OPTION_ROOTLESS:
      /* Ignored.  So that a runc command line won't fail.  */
      بشکن;

    حالت ARGP_کلید_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a command");

    حالت OPTION_VERSION:
    حالت OPTION_VERSION_CAP:
      print_version (stdout, state);
      exit (EXIT_SUCCESS);
    پیشفرض:
      بازگشت ARGP_ERR_UNKNOWN;
    }

  بازگشت 0;
}

ایستا ساختار commands_s *command;

تهی
crun_assert_n_args (صحیح n, صحیح min, صحیح max)
{
  اگر (min >= 0 && n < min)
    libcrun_fail_with_error (0, "`%s` requires a minimum of %d arguments", command->name, min);
  اگر (max >= 0 && n > max)
    libcrun_fail_with_error (0, "`%s` requires a maximum of %d arguments", command->name, max);
}

نویسه *
argp_mandatory_argument (نویسه *arg, ساختار argp_state *state)
{
  اگر (arg)
    بازگشت arg;
  بازگشت state->argv[state->next++];
}

/* Parse a numeric value from STR, aborting on an out-of-range or malformed
   value.  KIND names the field for error messages.  If ENDPTR is not NULL it
   is set past the parsed number (so the caller can continue parsing, e.g. a
   "UID:GID" pair); otherwise the whole string must be a valid number.  */
صحیح
parse_id_or_fail (ثابت نویسه *str, نویسه **endptr, ثابت نویسه *kind)
{
  نویسه *end = پوچ;
  long long l;

  errno = 0;
  l = strtoll (str, &end, 10);
  اگر (end == str)
    libcrun_fail_with_error (0, "invalid %s specified", kind);
  اگر (errno == ERANGE)
    libcrun_fail_with_error (0, "invalid %s specified", kind);
  اگر (l < INT_MIN || l > INT_MAX)
    libcrun_fail_with_error (0, "invalid %s specified", kind);

  اگر (endptr)
    *endptr = end;
  وگرنه اگر (*end != '\0')
    libcrun_fail_with_error (0, "invalid %s specified", kind);

  بازگشت (صحیح) l;
}

ایستا ساختار argp argp = { options, parse_opt, args_doc, doc, پوچ, پوچ, پوچ };

صحیح ensure_cloned_دودویی (تهی);

ایستا تهی
fill_handler_from_argv0 (نویسه *argv0, ساختار crun_global_arguments *args)
{
  ثابت نویسه *b = basename (argv0);
#ifdef HAVE_LIBKRUN
  اگر (strcmp (b, "krun") == 0)
    {
      args->handler = "krun";
      بازگشت;
    }
#endif
  اگر (strncmp (b, "crun-", 5) == 0 && b[5] != '\0')
    args->handler = b + 5;
}

ایستا نویسه **
copy_args (نویسه **argv, صحیح argc)
{
  نویسه **buff = xmalloc0 ((argc + 1) * اندازه (نویسه *));
  برای (صحیح i = 0; i < argc; i++)
    buff[i] = argv[i];

  بازگشت buff;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  libcrun_error_t err = پوچ;
  صحیح ret, first_argument = 0;

  arguments.argc = argc;
  arguments.argv = argv;

#ifdef DYNLOAD_LIBCRUN
  اگر (ensure_cloned_دودویی () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      exit (EXIT_FAILURE);
    }
  /* Resolve all libcrun weak dependencies.  */
  اگر (dlopen ("libcrun.so", RTLD_GLOBAL | RTLD_DEEPBIND | RTLD_LAZY) == پوچ)
    libcrun_fail_with_error (0, "could not load `libcrun.so`: `%s`", dlerror ());
#endif

  fill_handler_from_argv0 (argv[0], &arguments);
  argp_parse (&argp, argc, argv, ARGP_IN_ORDER, &first_argument, &arguments);

  command = get_command (argv[first_argument]);
  اگر (command == پوچ)
    libcrun_fail_with_error (0, "unknown command %s", argv[first_argument]);

  صحیح command_argc = argc - first_argument;
  cleanup_free نویسه **command_argv = copy_args (argv + first_argument, command_argc);
  command_argv[0] = argv[0];

  ret = command->handler (&arguments, command_argc, command_argv, &err);
  اگر (ret && err)
    {
      اگر (command->مقدار == COMMAND_EXEC)
        {
          libcrun_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));
          exit (EXIT_EXEC_FAILURE);
        }
      libcrun_fail_with_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));
    }

  بازگشت ret;
}
