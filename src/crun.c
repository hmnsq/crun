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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <git-version.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <argp.h>
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <libgen.h>
#অন্তর্ভাৱ <errno.h>
#অন্তর্ভাৱ <limits.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#অন্তর্ভাৱ "crun.h"

/* Commands.  */
#অন্তর্ভাৱ "run.h"
#অন্তর্ভাৱ "delete.h"
#অন্তর্ভাৱ "kill.h"
#অন্তর্ভাৱ "list.h"
#অন্তর্ভাৱ "start.h"
#অন্তর্ভাৱ "create.h"
#অন্তর্ভাৱ "exec.h"
#অন্তর্ভাৱ "state.h"
#অন্তর্ভাৱ "update.h"
#অন্তর্ভাৱ "spec.h"
#অন্তর্ভাৱ "pause.h"
#অন্তর্ভাৱ "unpause.h"
#অন্তর্ভাৱ "oci_features.h"
#অন্তর্ভাৱ "ps.h"
#অন্তর্ভাৱ "checkpoint.h"
#অন্তর্ভাৱ "mounts.h"
#অন্তর্ভাৱ "restore.h"

/* crun exec exits with the exit code of the executed process, so use this
   code for its own errors, to tell those apart (like runc and ssh do).  */
#define EXIT_EXEC_FAILURE 255

স্থির সংরচনা crun_global_arguments arguments;

সংরচনা commands_s
{
  পূর্ণাঙ্ক মান;
  ধ্রুৱ ৱর্ণ *name;
  পূর্ণাঙ্ক (*handler) (সংরচনা crun_global_arguments *, পূর্ণাঙ্ক, ৱর্ণ **, libcrun_error_t *);
};

পূর্ণাঙ্ক
init_libcrun_context (libcrun_context_t *con, ধ্রুৱ ৱর্ণ *id, সংরচনা crun_global_arguments *glob, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  libcrun_context_set_id (con, id);
  libcrun_context_set_state_মূল (con, glob->মূল);
  libcrun_context_set_systemd_cgroup (con, glob->option_systemd_cgroup);
  libcrun_context_set_force_no_cgroup (con, glob->option_force_no_cgroup);
  libcrun_context_set_notify_socket (con, getenv ("NOTIFY_SOCKET"));
  libcrun_context_set_args (con, glob->argc, glob->argv);

  /* Check if global handler is configured and pass it down to crun context */
  libcrun_context_set_handler (con, glob->handler);

  ret = libcrun_context_init_logging (con, glob->log, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (glob->log_format)
    {
      ret = libcrun_set_log_format (glob->log_format, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  libcrun_set_verbosity (glob->verbosity);
  libcrun_debug ("Using debug verbosity");

  ঘূৰাই 0;
}

/* Allocate a public libcrun context (with handlers loaded) for a subcommand.
   Terminates the process on failure.  */
libcrun_context_t *
new_libcrun_context (সংরচনা crun_global_arguments *glob)
{
  libcrun_error_t err = রিক্ত;
  libcrun_context_t *con;

  con = libcrun_context_new (রিক্ত, glob->মূল, &err);
  যদি (UNLIKELY (con == রিক্ত))
    libcrun_fail_with_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));

  ঘূৰাই con;
}

গণনা
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

সংরচনা commands_s commands[] = { { COMMAND_CREATE, "create", crun_command_create },
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
#যদি HAVE_CRIU && HAVE_DLOPEN
                                 { COMMAND_CHECKPOINT, "checkpoint", crun_command_checkpoint },
                                 { COMMAND_RESTORE, "restore", crun_command_restore },
#endif
                                 { COMMAND_MOUNTS, "mounts", crun_command_mounts },
                                 {
                                     0,
                                 } };

স্থির ৱর্ণ doc[] = "\nCOMMANDS:\n"
#যদি HAVE_CRIU && HAVE_DLOPEN
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
#যদি HAVE_CRIU && HAVE_DLOPEN
                    "\trestore     - restore a container\n"
#endif
                    "\trun         - run a container\n"
                    "\tspec        - generate a configuration file\n"
                    "\tstart       - start a container\n"
                    "\tstate       - output the state of a container\n"
                    "\tpause       - pause all the processes in the container\n"
                    "\tresume      - unpause the processes in the container\n"
                    "\tupdate      - update container resource constraints\n";

স্থির ৱর্ণ args_doc[] = "COMMAND [OPTION...]";

স্থির সংরচনা commands_s *
get_command (ধ্রুৱ ৱর্ণ *arg)
{
  সংরচনা commands_s *it;
  বাবে (it = commands; it->মান; it++)
    যদি (strcmp (it->name, arg) == 0)
      ঘূৰাই it;
  ঘূৰাই রিক্ত;
}

গণনা
{
  OPTION_VERSION = 'v',
  OPTION_VERSION_CAP = 'V',
  OPTION_DEBUG = 1000,
  OPTION_SYSTEMD_CGROUP,
  OPTION_CGROUP_MANAGER,
  OPTION_LOG,
  OPTION_LOG_FORMAT,
  OPTION_LOG_LEVEL,
  OPTION_মূল,
  OPTION_ROOTLESS
};

ধ্রুৱ ৱর্ণ *argp_program_bug_address = "https://github.com/containers/crun/issues";

স্থির সংরচনা argp_option options[] = { { "debug", OPTION_DEBUG, 0, 0, "produce verbose output, similar to --log-level=debug", 0 },
                                        { "cgroup-manager", OPTION_CGROUP_MANAGER, "MANAGER", 0, "cgroup manager", 0 },
                                        { "systemd-cgroup", OPTION_SYSTEMD_CGROUP, 0, 0, "use systemd cgroups", 0 },
                                        { "log", OPTION_LOG, "FILE", 0, "log destination: '[file:]PATH', 'journald:ID' or 'syslog:ID' (defaults to stderr)", 0 },
                                        { "log-format", OPTION_LOG_FORMAT, "FORMAT", 0, "log format: 'text' (default) or 'json'", 0 },
                                        { "log-level", OPTION_LOG_LEVEL, "LEVEL", 0, "log level to use: 'error' (default), 'warning' or 'debug'", 0 },
                                        { "root", OPTION_মূল, "DIR", 0, রিক্ত, 0 },
                                        { "rootless", OPTION_ROOTLESS, "VALUE", 0, রিক্ত, 0 },
                                        { "version", OPTION_VERSION, 0, 0, রিক্ত, 0 },
                                        // alias OPTION_VERSION_CAP with OPTION_VERSION
                                        { রিক্ত, OPTION_VERSION_CAP, 0, OPTION_ALIAS, রিক্ত, 0 },
                                        {
                                            0,
                                        } };

স্থির শূন্য
print_version (FILE *stream, সংরচনা argp_state *state arg_unused)
{
  libcrun_error_t err = রিক্ত;
  cleanup_free ৱর্ণ *rundir = রিক্ত;
  libcrun_context_t *ctx;
  ৱর্ণ **tags;
  পূর্ণাঙ্ক ret;

  fprintf (stream, "%s version %s\n", PACKAGE_NAME, PACKAGE_VERSION);
  fprintf (stream, "commit: %s\n", GIT_VERSION);

  ret = libcrun_get_state_dir (&rundir, arguments.মূল, রিক্ত, &err);
  যদি (LIKELY (ret == 0))
    fprintf (stream, "rundir: %s\n", rundir);
  অন্যথা
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
#যদি HAVE_CRIU && HAVE_DLOPEN
  fprintf (stream, "+CRIU ");
#endif

  ctx = libcrun_context_new (রিক্ত, arguments.মূল, &err);
  যদি (ctx)
    {
      tags = libcrun_context_get_handler_feature_tags (ctx, &err);
      যদি (tags)
        {
          বাবে (size_t i = 0; tags[i]; i++)
            {
              fprintf (stream, "+%s ", tags[i]);
              মুক্ত (tags[i]);
            }
          মুক্ত (tags);
        }
      অন্যথা
        libcrun_error_release (&err);
      libcrun_context_free (ctx);
    }
  অন্যথা
    libcrun_error_release (&err);

  fprintf (stream, "+JSON_C\n");
}

স্থির error_t
parse_opt (পূর্ণাঙ্ক চাবি, ৱর্ণ *arg, সংরচনা argp_state *state)
{
  ধ্রুৱ ৱর্ণ *tmp;

  ৱিকল্প (চাবি)
    {
    পক্ষ OPTION_DEBUG:
      arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
      ভঙ্গ;

    পক্ষ OPTION_CGROUP_MANAGER:
      tmp = argp_mandatory_argument (arg, state);
      যদি (strcmp (tmp, "systemd") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = true;
        }
      অন্যথা যদি (strcmp (tmp, "cgroupfs") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = false;
        }
      অন্যথা যদি (strcmp (tmp, "disabled") == 0)
        {
          arguments.option_systemd_cgroup = false;
          arguments.option_force_no_cgroup = true;
        }
      অন্যথা
        {
          libcrun_fail_with_error (0, "unknown cgroup manager specified");
        }
      ভঙ্গ;

    পক্ষ OPTION_SYSTEMD_CGROUP:
      arguments.option_force_no_cgroup = false;
      arguments.option_systemd_cgroup = true;
      ভঙ্গ;

    পক্ষ OPTION_LOG:
      arguments.log = argp_mandatory_argument (arg, state);
      ভঙ্গ;

    পক্ষ OPTION_LOG_FORMAT:
      arguments.log_format = argp_mandatory_argument (arg, state);
      ভঙ্গ;

    পক্ষ OPTION_LOG_LEVEL:
      tmp = argp_mandatory_argument (arg, state);
      যদি (strcmp (tmp, "error") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_ERROR;
        }
      অন্যথা যদি (strcmp (tmp, "warning") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_WARNING;
        }
      অন্যথা যদি (strcmp (tmp, "debug") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
        }
      অন্যথা
        {
          libcrun_fail_with_error (0, "unknown verbosity `%s` specified", arg);
        }
      ভঙ্গ;

    পক্ষ OPTION_মূল:
      arguments.মূল = argp_mandatory_argument (arg, state);
      ভঙ্গ;

    পক্ষ OPTION_ROOTLESS:
      /* Ignored.  So that a runc command line won't fail.  */
      ভঙ্গ;

    পক্ষ ARGP_চাবি_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a command");

    পক্ষ OPTION_VERSION:
    পক্ষ OPTION_VERSION_CAP:
      print_version (stdout, state);
      exit (EXIT_SUCCESS);
    স্ৱভাৱ:
      ঘূৰাই ARGP_ERR_UNKNOWN;
    }

  ঘূৰাই 0;
}

স্থির সংরচনা commands_s *command;

শূন্য
crun_assert_n_args (পূর্ণাঙ্ক n, পূর্ণাঙ্ক min, পূর্ণাঙ্ক max)
{
  যদি (min >= 0 && n < min)
    libcrun_fail_with_error (0, "`%s` requires a minimum of %d arguments", command->name, min);
  যদি (max >= 0 && n > max)
    libcrun_fail_with_error (0, "`%s` requires a maximum of %d arguments", command->name, max);
}

ৱর্ণ *
argp_mandatory_argument (ৱর্ণ *arg, সংরচনা argp_state *state)
{
  যদি (arg)
    ঘূৰাই arg;
  ঘূৰাই state->argv[state->next++];
}

/* Parse a numeric value from STR, aborting on an out-of-range or malformed
   value.  KIND names the field for error messages.  If ENDPTR is not NULL it
   is set past the parsed number (so the caller can continue parsing, e.g. a
   "UID:GID" pair); otherwise the whole string must be a valid number.  */
পূর্ণাঙ্ক
parse_id_or_fail (ধ্রুৱ ৱর্ণ *str, ৱর্ণ **endptr, ধ্রুৱ ৱর্ণ *kind)
{
  ৱর্ণ *end = রিক্ত;
  long long l;

  errno = 0;
  l = strtoll (str, &end, 10);
  যদি (end == str)
    libcrun_fail_with_error (0, "invalid %s specified", kind);
  যদি (errno == ERANGE)
    libcrun_fail_with_error (0, "invalid %s specified", kind);
  যদি (l < INT_MIN || l > INT_MAX)
    libcrun_fail_with_error (0, "invalid %s specified", kind);

  যদি (endptr)
    *endptr = end;
  অন্যথা যদি (*end != '\0')
    libcrun_fail_with_error (0, "invalid %s specified", kind);

  ঘূৰাই (পূর্ণাঙ্ক) l;
}

স্থির সংরচনা argp argp = { options, parse_opt, args_doc, doc, রিক্ত, রিক্ত, রিক্ত };

পূর্ণাঙ্ক ensure_cloned_দ্বিমিক (শূন্য);

স্থির শূন্য
fill_handler_from_argv0 (ৱর্ণ *argv0, সংরচনা crun_global_arguments *args)
{
  ধ্রুৱ ৱর্ণ *b = basename (argv0);
#ifdef HAVE_LIBKRUN
  যদি (strcmp (b, "krun") == 0)
    {
      args->handler = "krun";
      ঘূৰাই;
    }
#endif
  যদি (strncmp (b, "crun-", 5) == 0 && b[5] != '\0')
    args->handler = b + 5;
}

স্থির ৱর্ণ **
copy_args (ৱর্ণ **argv, পূর্ণাঙ্ক argc)
{
  ৱর্ণ **buff = xmalloc0 ((argc + 1) * পরিমাণ (ৱর্ণ *));
  বাবে (পূর্ণাঙ্ক i = 0; i < argc; i++)
    buff[i] = argv[i];

  ঘূৰাই buff;
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  libcrun_error_t err = রিক্ত;
  পূর্ণাঙ্ক ret, first_argument = 0;

  arguments.argc = argc;
  arguments.argv = argv;

#ifdef DYNLOAD_LIBCRUN
  যদি (ensure_cloned_দ্বিমিক () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      exit (EXIT_FAILURE);
    }
  /* Resolve all libcrun weak dependencies.  */
  যদি (dlopen ("libcrun.so", RTLD_GLOBAL | RTLD_DEEPBIND | RTLD_LAZY) == রিক্ত)
    libcrun_fail_with_error (0, "could not load `libcrun.so`: `%s`", dlerror ());
#endif

  fill_handler_from_argv0 (argv[0], &arguments);
  argp_parse (&argp, argc, argv, ARGP_IN_ORDER, &first_argument, &arguments);

  command = get_command (argv[first_argument]);
  যদি (command == রিক্ত)
    libcrun_fail_with_error (0, "unknown command %s", argv[first_argument]);

  পূর্ণাঙ্ক command_argc = argc - first_argument;
  cleanup_free ৱর্ণ **command_argv = copy_args (argv + first_argument, command_argc);
  command_argv[0] = argv[0];

  ret = command->handler (&arguments, command_argc, command_argv, &err);
  যদি (ret && err)
    {
      যদি (command->মান == COMMAND_EXEC)
        {
          libcrun_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));
          exit (EXIT_EXEC_FAILURE);
        }
      libcrun_fail_with_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));
    }

  ঘূৰাই ret;
}
