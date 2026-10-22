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
#include <git-version.h>
#include <stdlib.h>
#include <argp.h>
#include <string.h>
#include <libgen.h>
#include <errno.h>
#include <limits.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#include "crun.h"

/* Commands.  */
#include "run.h"
#include "delete.h"
#include "kill.h"
#include "list.h"
#include "start.h"
#include "create.h"
#include "exec.h"
#include "state.h"
#include "update.h"
#include "spec.h"
#include "pause.h"
#include "unpause.h"
#include "oci_features.h"
#include "ps.h"
#include "checkpoint.h"
#include "mounts.h"
#include "restore.h"

/* crun exec exits with the exit code of the executed process, so use this
   code for its own errors, to tell those apart (like runc and ssh do).  */
#define EXIT_EXEC_FAILURE 255

static sa_nracanaa crun_global_arguments arguments;

sa_nracanaa commands_s
{
  puur_naa_nka value;
  const var_na *name;
  puur_naa_nka (*handler) (sa_nracanaa crun_global_arguments *, puur_naa_nka, var_na **, libcrun_error_t *);
};

puur_naa_nka
init_libcrun_context (libcrun_context_t *con, const var_na *id, sa_nracanaa crun_global_arguments *glob, libcrun_error_t *err)
{
  puur_naa_nka ret;

  libcrun_context_set_id (con, id);
  libcrun_context_set_state_root (con, glob->root);
  libcrun_context_set_systemd_cgroup (con, glob->option_systemd_cgroup);
  libcrun_context_set_force_no_cgroup (con, glob->option_force_no_cgroup);
  libcrun_context_set_notify_socket (con, getenv ("NOTIFY_SOCKET"));
  libcrun_context_set_args (con, glob->argc, glob->argv);

  /* Check if global handler is configured and pass it down to crun context */
  libcrun_context_set_handler (con, glob->handler);

  ret = libcrun_context_init_logging (con, glob->log, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  ꦪꦼꦤ꧀ (glob->log_format)
    {
      ret = libcrun_set_log_format (glob->log_format, err);
      ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
        ꦮꦭꦶ ret;
    }

  libcrun_set_verbosity (glob->verbosity);
  libcrun_debug ("Using debug verbosity");

  ꦮꦭꦶ 0;
}

/* Allocate a public libcrun context (with handlers loaded) for a subcommand.
   Terminates the process on failure.  */
libcrun_context_t *
new_libcrun_context (sa_nracanaa crun_global_arguments *glob)
{
  libcrun_error_t err = NULL;
  libcrun_context_t *con;

  con = libcrun_context_new (NULL, glob->root, &err);
  ꦪꦼꦤ꧀ (UNLIKELY (con == NULL))
    libcrun_fail_with_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));

  ꦮꦭꦶ con;
}

ga_nanaa
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

sa_nracanaa commands_s commands[] = { { COMMAND_CREATE, "create", crun_command_create },
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
#ꦪꦼꦤ꧀ HAVE_CRIU && HAVE_DLOPEN
                                 { COMMAND_CHECKPOINT, "checkpoint", crun_command_checkpoint },
                                 { COMMAND_RESTORE, "restore", crun_command_restore },
#endif
                                 { COMMAND_MOUNTS, "mounts", crun_command_mounts },
                                 {
                                     0,
                                 } };

static var_na doc[] = "\nCOMMANDS:\n"
#ꦪꦼꦤ꧀ HAVE_CRIU && HAVE_DLOPEN
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
#ꦪꦼꦤ꧀ HAVE_CRIU && HAVE_DLOPEN
                    "\trestore     - restore a container\n"
#endif
                    "\trun         - run a container\n"
                    "\tspec        - generate a configuration file\n"
                    "\tstart       - start a container\n"
                    "\tstate       - output the state of a container\n"
                    "\tpause       - pause all the processes in the container\n"
                    "\tresume      - unpause the processes in the container\n"
                    "\tupdate      - update container resource constraints\n";

static var_na args_doc[] = "COMMAND [OPTION...]";

static sa_nracanaa commands_s *
get_command (const var_na *arg)
{
  sa_nracanaa commands_s *it;
  ꦏꦁꦒꦺꦴ (it = commands; it->value; it++)
    ꦪꦼꦤ꧀ (strcmp (it->name, arg) == 0)
      ꦮꦭꦶ it;
  ꦮꦭꦶ NULL;
}

ga_nanaa
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

const var_na *argp_program_bug_address = "https://github.com/containers/crun/issues";

static sa_nracanaa argp_option options[] = { { "debug", OPTION_DEBUG, 0, 0, "produce verbose output, similar to --log-level=debug", 0 },
                                        { "cgroup-manager", OPTION_CGROUP_MANAGER, "MANAGER", 0, "cgroup manager", 0 },
                                        { "systemd-cgroup", OPTION_SYSTEMD_CGROUP, 0, 0, "use systemd cgroups", 0 },
                                        { "log", OPTION_LOG, "FILE", 0, "log destination: '[file:]PATH', 'journald:ID' or 'syslog:ID' (defaults to stderr)", 0 },
                                        { "log-format", OPTION_LOG_FORMAT, "FORMAT", 0, "log format: 'text' (default) or 'json'", 0 },
                                        { "log-level", OPTION_LOG_LEVEL, "LEVEL", 0, "log level to use: 'error' (default), 'warning' or 'debug'", 0 },
                                        { "root", OPTION_ROOT, "DIR", 0, NULL, 0 },
                                        { "rootless", OPTION_ROOTLESS, "VALUE", 0, NULL, 0 },
                                        { "version", OPTION_VERSION, 0, 0, NULL, 0 },
                                        // alias OPTION_VERSION_CAP with OPTION_VERSION
                                        { NULL, OPTION_VERSION_CAP, 0, OPTION_ALIAS, NULL, 0 },
                                        {
                                            0,
                                        } };

static void
print_version (FILE *stream, sa_nracanaa argp_state *state arg_unused)
{
  libcrun_error_t err = NULL;
  cleanup_free var_na *rundir = NULL;
  libcrun_context_t *ctx;
  var_na **tags;
  puur_naa_nka ret;

  fprintf (stream, "%s version %s\n", PACKAGE_NAME, PACKAGE_VERSION);
  fprintf (stream, "commit: %s\n", GIT_VERSION);

  ret = libcrun_get_state_dir (&rundir, arguments.root, NULL, &err);
  ꦪꦼꦤ꧀ (LIKELY (ret == 0))
    fprintf (stream, "rundir: %s\n", rundir);
  ꦭꦶꦪ
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
#ꦪꦼꦤ꧀ HAVE_CRIU && HAVE_DLOPEN
  fprintf (stream, "+CRIU ");
#endif

  ctx = libcrun_context_new (NULL, arguments.root, &err);
  ꦪꦼꦤ꧀ (ctx)
    {
      tags = libcrun_context_get_handler_feature_tags (ctx, &err);
      ꦪꦼꦤ꧀ (tags)
        {
          ꦏꦁꦒꦺꦴ (size_t i = 0; tags[i]; i++)
            {
              fprintf (stream, "+%s ", tags[i]);
              free (tags[i]);
            }
          free (tags);
        }
      ꦭꦶꦪ
        libcrun_error_release (&err);
      libcrun_context_free (ctx);
    }
  ꦭꦶꦪ
    libcrun_error_release (&err);

  fprintf (stream, "+JSON_C\n");
}

static error_t
parse_opt (puur_naa_nka key, var_na *arg, sa_nracanaa argp_state *state)
{
  const var_na *tmp;

  switch (key)
    {
    pak_sa OPTION_DEBUG:
      arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
      bha_nga;

    pak_sa OPTION_CGROUP_MANAGER:
      tmp = argp_mandatory_argument (arg, state);
      ꦪꦼꦤ꧀ (strcmp (tmp, "systemd") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = true;
        }
      ꦭꦶꦪ ꦪꦼꦤ꧀ (strcmp (tmp, "cgroupfs") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = false;
        }
      ꦭꦶꦪ ꦪꦼꦤ꧀ (strcmp (tmp, "disabled") == 0)
        {
          arguments.option_systemd_cgroup = false;
          arguments.option_force_no_cgroup = true;
        }
      ꦭꦶꦪ
        {
          libcrun_fail_with_error (0, "unknown cgroup manager specified");
        }
      bha_nga;

    pak_sa OPTION_SYSTEMD_CGROUP:
      arguments.option_force_no_cgroup = false;
      arguments.option_systemd_cgroup = true;
      bha_nga;

    pak_sa OPTION_LOG:
      arguments.log = argp_mandatory_argument (arg, state);
      bha_nga;

    pak_sa OPTION_LOG_FORMAT:
      arguments.log_format = argp_mandatory_argument (arg, state);
      bha_nga;

    pak_sa OPTION_LOG_LEVEL:
      tmp = argp_mandatory_argument (arg, state);
      ꦪꦼꦤ꧀ (strcmp (tmp, "error") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_ERROR;
        }
      ꦭꦶꦪ ꦪꦼꦤ꧀ (strcmp (tmp, "warning") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_WARNING;
        }
      ꦭꦶꦪ ꦪꦼꦤ꧀ (strcmp (tmp, "debug") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
        }
      ꦭꦶꦪ
        {
          libcrun_fail_with_error (0, "unknown verbosity `%s` specified", arg);
        }
      bha_nga;

    pak_sa OPTION_ROOT:
      arguments.root = argp_mandatory_argument (arg, state);
      bha_nga;

    pak_sa OPTION_ROOTLESS:
      /* Ignored.  So that a runc command line won't fail.  */
      bha_nga;

    pak_sa ARGP_KEY_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a command");

    pak_sa OPTION_VERSION:
    pak_sa OPTION_VERSION_CAP:
      print_version (stdout, state);
      exit (EXIT_SUCCESS);
    default:
      ꦮꦭꦶ ARGP_ERR_UNKNOWN;
    }

  ꦮꦭꦶ 0;
}

static sa_nracanaa commands_s *command;

void
crun_assert_n_args (puur_naa_nka n, puur_naa_nka min, puur_naa_nka max)
{
  ꦪꦼꦤ꧀ (min >= 0 && n < min)
    libcrun_fail_with_error (0, "`%s` requires a minimum of %d arguments", command->name, min);
  ꦪꦼꦤ꧀ (max >= 0 && n > max)
    libcrun_fail_with_error (0, "`%s` requires a maximum of %d arguments", command->name, max);
}

var_na *
argp_mandatory_argument (var_na *arg, sa_nracanaa argp_state *state)
{
  ꦪꦼꦤ꧀ (arg)
    ꦮꦭꦶ arg;
  ꦮꦭꦶ state->argv[state->next++];
}

/* Parse a numeric value from STR, aborting on an out-of-range or malformed
   value.  KIND names the field for error messages.  If ENDPTR is not NULL it
   is set past the parsed number (so the caller can continue parsing, e.g. a
   "UID:GID" pair); otherwise the whole string must be a valid number.  */
puur_naa_nka
parse_id_or_fail (const var_na *str, var_na **endptr, const var_na *kind)
{
  var_na *end = NULL;
  long long l;

  errno = 0;
  l = strtoll (str, &end, 10);
  ꦪꦼꦤ꧀ (end == str)
    libcrun_fail_with_error (0, "invalid %s specified", kind);
  ꦪꦼꦤ꧀ (errno == ERANGE)
    libcrun_fail_with_error (0, "invalid %s specified", kind);
  ꦪꦼꦤ꧀ (l < INT_MIN || l > INT_MAX)
    libcrun_fail_with_error (0, "invalid %s specified", kind);

  ꦪꦼꦤ꧀ (endptr)
    *endptr = end;
  ꦭꦶꦪ ꦪꦼꦤ꧀ (*end != '\0')
    libcrun_fail_with_error (0, "invalid %s specified", kind);

  ꦮꦭꦶ (puur_naa_nka) l;
}

static sa_nracanaa argp argp = { options, parse_opt, args_doc, doc, NULL, NULL, NULL };

puur_naa_nka ensure_cloned_binary (void);

static void
fill_handler_from_argv0 (var_na *argv0, sa_nracanaa crun_global_arguments *args)
{
  const var_na *b = basename (argv0);
#ifdef HAVE_LIBKRUN
  ꦪꦼꦤ꧀ (strcmp (b, "krun") == 0)
    {
      args->handler = "krun";
      ꦮꦭꦶ;
    }
#endif
  ꦪꦼꦤ꧀ (strncmp (b, "crun-", 5) == 0 && b[5] != '\0')
    args->handler = b + 5;
}

static var_na **
copy_args (var_na **argv, puur_naa_nka argc)
{
  var_na **buff = xmalloc0 ((argc + 1) * parimaa_na (var_na *));
  ꦏꦁꦒꦺꦴ (puur_naa_nka i = 0; i < argc; i++)
    buff[i] = argv[i];

  ꦮꦭꦶ buff;
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  libcrun_error_t err = NULL;
  puur_naa_nka ret, first_argument = 0;

  arguments.argc = argc;
  arguments.argv = argv;

#ifdef DYNLOAD_LIBCRUN
  ꦪꦼꦤ꧀ (ensure_cloned_binary () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      exit (EXIT_FAILURE);
    }
  /* Resolve all libcrun weak dependencies.  */
  ꦪꦼꦤ꧀ (dlopen ("libcrun.so", RTLD_GLOBAL | RTLD_DEEPBIND | RTLD_LAZY) == NULL)
    libcrun_fail_with_error (0, "could not load `libcrun.so`: `%s`", dlerror ());
#endif

  fill_handler_from_argv0 (argv[0], &arguments);
  argp_parse (&argp, argc, argv, ARGP_IN_ORDER, &first_argument, &arguments);

  command = get_command (argv[first_argument]);
  ꦪꦼꦤ꧀ (command == NULL)
    libcrun_fail_with_error (0, "unknown command %s", argv[first_argument]);

  puur_naa_nka command_argc = argc - first_argument;
  cleanup_free var_na **command_argv = copy_args (argv + first_argument, command_argc);
  command_argv[0] = argv[0];

  ret = command->handler (&arguments, command_argc, command_argv, &err);
  ꦪꦼꦤ꧀ (ret && err)
    {
      ꦪꦼꦤ꧀ (command->value == COMMAND_EXEC)
        {
          libcrun_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));
          exit (EXIT_EXEC_FAILURE);
        }
      libcrun_fail_with_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));
    }

  ꦮꦭꦶ ret;
}
