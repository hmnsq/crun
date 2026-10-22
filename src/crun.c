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

#అన్తర్భావ <config.h>
#అన్తర్భావ <git-version.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <argp.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <libgen.h>
#అన్తర్భావ <errno.h>
#అన్తర్భావ <limits.h>

#నిర్వచితమైతే HAVE_DLOPEN
#  include <dlfcn.h>
#అయితే_ముగింపు

#అన్తర్భావ "crun.h"

/* Commands.  */
#అన్తర్భావ "run.h"
#అన్తర్భావ "delete.h"
#అన్తర్భావ "kill.h"
#అన్తర్భావ "list.h"
#అన్తర్భావ "start.h"
#అన్తర్భావ "create.h"
#అన్తర్భావ "exec.h"
#అన్తర్భావ "state.h"
#అన్తర్భావ "update.h"
#అన్తర్భావ "spec.h"
#అన్తర్భావ "pause.h"
#అన్తర్భావ "unpause.h"
#అన్తర్భావ "oci_features.h"
#అన్తర్భావ "ps.h"
#అన్తర్భావ "checkpoint.h"
#అన్తర్భావ "mounts.h"
#అన్తర్భావ "restore.h"

/* crun exec exits with the exit code of the executed process, so use this
   code for its own errors, to tell those apart (like runc and ssh do).  */
#నిర్వచించు EXIT_EXEC_FAILURE 255

స్థిర నిర్మాణం crun_global_arguments arguments;

నిర్మాణం commands_s
{
  పూర్ణాంకం విలువ;
  స్థిరరాశి అక్షరం *name;
  పూర్ణాంకం (*handler) (నిర్మాణం crun_global_arguments *, పూర్ణాంకం, అక్షరం **, libcrun_error_t *);
};

పూర్ణాంకం
init_libcrun_context (libcrun_context_t *con, స్థిరరాశి అక్షరం *id, నిర్మాణం crun_global_arguments *glob, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  libcrun_context_సమితి_id (con, id);
  libcrun_context_సమితి_state_మూలం (con, glob->మూలం);
  libcrun_context_సమితి_systemd_cgroup (con, glob->option_systemd_cgroup);
  libcrun_context_సమితి_force_no_cgroup (con, glob->option_force_no_cgroup);
  libcrun_context_సమితి_notify_socket (con, getenv ("NOTIFY_SOCKET"));
  libcrun_context_సమితి_వాదనలు (con, glob->argc, glob->argv);

  /* Check if global handler is configured and pass it down to crun context */
  libcrun_context_సమితి_handler (con, glob->handler);

  ret = libcrun_context_init_logging (con, glob->సంవర్గమానం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (glob->log_format)
    {
      ret = libcrun_సమితి_log_format (glob->log_format, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  libcrun_సమితి_verbosity (glob->verbosity);
  libcrun_debug ("Using debug verbosity");

  తిరిగి 0;
}

/* Allocate a public libcrun context (with handlers loaded) for a subcommand.
   Terminates the process on failure.  */
libcrun_context_t *
new_libcrun_context (నిర్మాణం crun_global_arguments *glob)
{
  libcrun_error_t err = ఖాళీ;
  libcrun_context_t *con;

  con = libcrun_context_new (ఖాళీ, glob->మూలం, &err);
  ఒకవేళ (UNLIKELY (con == ఖాళీ))
    libcrun_fail_with_error (libcrun_error_get_స్థితి (err), "%s", libcrun_error_get_సందేశం (err));

  తిరిగి con;
}

గణన
{
  COMMAND_CREATE = 1000,
  COMMAND_DELETE,
  COMMAND_EXEC,
  COMMAND_LIST,
  COMMAND_KILL,
  COMMAND_నిరంతరం,
  COMMAND_SPEC,
  COMMAND_ప్రారంభం,
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

నిర్మాణం commands_s commands[] = { { COMMAND_CREATE, "create", crun_command_create },
                                 { COMMAND_DELETE, "delete", crun_command_delete },
                                 { COMMAND_EXEC, "exec", crun_command_exec },
                                 { COMMAND_LIST, "list", crun_command_list },
                                 { COMMAND_KILL, "kill", crun_command_kill },
                                 { COMMAND_PS, "ps", crun_command_ps },
                                 { COMMAND_నిరంతరం, "run", crun_command_నిరంతరం },
                                 { COMMAND_SPEC, "spec", crun_command_spec },
                                 { COMMAND_ప్రారంభం, "start", crun_command_ప్రారంభం },
                                 { COMMAND_STATE, "state", crun_command_state },
                                 { COMMAND_UPDATE, "update", crun_command_update },
                                 { COMMAND_PAUSE, "pause", crun_command_pause },
                                 { COMMAND_UNPAUSE, "resume", crun_command_unpause },
                                 { COMMAND_FEATURES, "features", crun_command_features },
#ఒకవేళ HAVE_CRIU && HAVE_DLOPEN
                                 { COMMAND_CHECKPOINT, "checkpoint", crun_command_checkpoint },
                                 { COMMAND_RESTORE, "restore", crun_command_restore },
#అయితే_ముగింపు
                                 { COMMAND_MOUNTS, "mounts", crun_command_mounts },
                                 {
                                     0,
                                 } };

స్థిర అక్షరం doc[] = "\nCOMMANDS:\n"
#ఒకవేళ HAVE_CRIU && HAVE_DLOPEN
                    "\tcheckpoint  - checkpoint a container\n"
#అయితే_ముగింపు
                    "\tcreate      - create a container\n"
                    "\tdelete      - remove definition for a container\n"
                    "\texec        - exec a command in a running container\n"
                    "\tfeatures    - show the enabled features\n"
                    "\tlist        - list known containers\n"
                    "\tmounts      - add or remove mounts from a running container\n"
                    "\tkill        - send a signal to the container init process\n"
                    "\tps          - show the processes in the container\n"
#ఒకవేళ HAVE_CRIU && HAVE_DLOPEN
                    "\trestore     - restore a container\n"
#అయితే_ముగింపు
                    "\trun         - run a container\n"
                    "\tspec        - generate a configuration file\n"
                    "\tstart       - start a container\n"
                    "\tstate       - output the state of a container\n"
                    "\tpause       - pause all the processes in the container\n"
                    "\tresume      - unpause the processes in the container\n"
                    "\tupdate      - update container resource constraints\n";

స్థిర అక్షరం వాదనలు_doc[] = "COMMAND [OPTION...]";

స్థిర నిర్మాణం commands_s *
get_command (స్థిరరాశి అక్షరం *arg)
{
  నిర్మాణం commands_s *it;
  కొరకు (it = commands; it->విలువ; it++)
    ఒకవేళ (strcmp (it->name, arg) == 0)
      తిరిగి it;
  తిరిగి ఖాళీ;
}

గణన
{
  OPTION_సంస్కరణ = 'v',
  OPTION_సంస్కరణ_CAP = 'V',
  OPTION_DEBUG = 1000,
  OPTION_SYSTEMD_CGROUP,
  OPTION_CGROUP_MANAGER,
  OPTION_LOG,
  OPTION_LOG_FORMAT,
  OPTION_LOG_LEVEL,
  OPTION_మూలం,
  OPTION_ROOTLESS
};

స్థిరరాశి అక్షరం *argp_కార్యక్రమం_bug_చిరునామా = "https://github.com/containers/crun/issues";

స్థిర నిర్మాణం argp_option options[] = { { "debug", OPTION_DEBUG, 0, 0, "produce verbose output, similar to --log-level=debug", 0 },
                                        { "cgroup-manager", OPTION_CGROUP_MANAGER, "MANAGER", 0, "cgroup manager", 0 },
                                        { "systemd-cgroup", OPTION_SYSTEMD_CGROUP, 0, 0, "use systemd cgroups", 0 },
                                        { "log", OPTION_LOG, "FILE", 0, "log destination: '[file:]PATH', 'journald:ID' or 'syslog:ID' (defaults to stderr)", 0 },
                                        { "log-format", OPTION_LOG_FORMAT, "FORMAT", 0, "log format: 'text' (default) or 'json'", 0 },
                                        { "log-level", OPTION_LOG_LEVEL, "LEVEL", 0, "log level to use: 'error' (default), 'warning' or 'debug'", 0 },
                                        { "root", OPTION_మూలం, "DIR", 0, ఖాళీ, 0 },
                                        { "rootless", OPTION_ROOTLESS, "VALUE", 0, ఖాళీ, 0 },
                                        { "version", OPTION_సంస్కరణ, 0, 0, ఖాళీ, 0 },
                                        // alias OPTION_VERSION_CAP with OPTION_VERSION
                                        { ఖాళీ, OPTION_సంస్కరణ_CAP, 0, OPTION_ALIAS, ఖాళీ, 0 },
                                        {
                                            0,
                                        } };

స్థిర శూన్య
print_సంస్కరణ (FILE *ప్రవాహం, నిర్మాణం argp_state *state arg_వాడని)
{
  libcrun_error_t err = ఖాళీ;
  cleanup_free అక్షరం *rundir = ఖాళీ;
  libcrun_context_t *ctx;
  అక్షరం **ట్యాగులు;
  పూర్ణాంకం ret;

  fprintf (ప్రవాహం, "%s version %s\n", PACKAGE_NAME, PACKAGE_సంస్కరణ);
  fprintf (ప్రవాహం, "commit: %s\n", GIT_సంస్కరణ);

  ret = libcrun_get_state_dir (&rundir, arguments.మూలం, ఖాళీ, &err);
  ఒకవేళ (LIKELY (ret == 0))
    fprintf (ప్రవాహం, "rundir: %s\n", rundir);
  లేకపోతే
    libcrun_error_release (&err);

  fprintf (ప్రవాహం, "spec: 1.0.0\n");

#నిర్వచితమైతే HAVE_SYSTEMD
  fprintf (ప్రవాహం, "+SYSTEMD ");
#అయితే_ముగింపు
  fprintf (ప్రవాహం, "+SELINUX ");
  fprintf (ప్రవాహం, "+APPARMOR ");
#నిర్వచితమైతే HAVE_CAP
  fprintf (ప్రవాహం, "+CAP ");
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_SECCOMP
  fprintf (ప్రవాహం, "+SECCOMP ");
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_EBPF
  fprintf (ప్రవాహం, "+EBPF ");
#అయితే_ముగింపు
#ఒకవేళ HAVE_CRIU && HAVE_DLOPEN
  fprintf (ప్రవాహం, "+CRIU ");
#అయితే_ముగింపు

  ctx = libcrun_context_new (ఖాళీ, arguments.మూలం, &err);
  ఒకవేళ (ctx)
    {
      ట్యాగులు = libcrun_context_get_handler_feature_ట్యాగులు (ctx, &err);
      ఒకవేళ (ట్యాగులు)
        {
          కొరకు (size_t i = 0; ట్యాగులు[i]; i++)
            {
              fprintf (ప్రవాహం, "+%s ", ట్యాగులు[i]);
              విడుదల (ట్యాగులు[i]);
            }
          విడుదల (ట్యాగులు);
        }
      లేకపోతే
        libcrun_error_release (&err);
      libcrun_context_free (ctx);
    }
  లేకపోతే
    libcrun_error_release (&err);

  fprintf (ప్రవాహం, "+JSON_C\n");
}

స్థిర error_t
parse_opt (పూర్ణాంకం కీలకం, అక్షరం *arg, నిర్మాణం argp_state *state)
{
  స్థిరరాశి అక్షరం *tmp;

  ఎంపిక (కీలకం)
    {
    స్థితి OPTION_DEBUG:
      arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
      ఆపు;

    స్థితి OPTION_CGROUP_MANAGER:
      tmp = argp_mandatory_వాదం (arg, state);
      ఒకవేళ (strcmp (tmp, "systemd") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = true;
        }
      లేకపోతే ఒకవేళ (strcmp (tmp, "cgroupfs") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = false;
        }
      లేకపోతే ఒకవేళ (strcmp (tmp, "disabled") == 0)
        {
          arguments.option_systemd_cgroup = false;
          arguments.option_force_no_cgroup = true;
        }
      లేకపోతే
        {
          libcrun_fail_with_error (0, "unknown cgroup manager specified");
        }
      ఆపు;

    స్థితి OPTION_SYSTEMD_CGROUP:
      arguments.option_force_no_cgroup = false;
      arguments.option_systemd_cgroup = true;
      ఆపు;

    స్థితి OPTION_LOG:
      arguments.సంవర్గమానం = argp_mandatory_వాదం (arg, state);
      ఆపు;

    స్థితి OPTION_LOG_FORMAT:
      arguments.log_format = argp_mandatory_వాదం (arg, state);
      ఆపు;

    స్థితి OPTION_LOG_LEVEL:
      tmp = argp_mandatory_వాదం (arg, state);
      ఒకవేళ (strcmp (tmp, "error") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_ERROR;
        }
      లేకపోతే ఒకవేళ (strcmp (tmp, "warning") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_WARNING;
        }
      లేకపోతే ఒకవేళ (strcmp (tmp, "debug") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
        }
      లేకపోతే
        {
          libcrun_fail_with_error (0, "unknown verbosity `%s` specified", arg);
        }
      ఆపు;

    స్థితి OPTION_మూలం:
      arguments.మూలం = argp_mandatory_వాదం (arg, state);
      ఆపు;

    స్థితి OPTION_ROOTLESS:
      /* Ignored.  So that a runc command line won't fail.  */
      ఆపు;

    స్థితి ARGP_కీలకం_NO_వాదనలు:
      libcrun_fail_with_error (0, "please specify a command");

    స్థితి OPTION_సంస్కరణ:
    స్థితి OPTION_సంస్కరణ_CAP:
      print_సంస్కరణ (stdout, state);
      నిష్క్రమించు (EXIT_SUCCESS);
    అప్రమేయ:
      తిరిగి ARGP_ERR_UNKNOWN;
    }

  తిరిగి 0;
}

స్థిర నిర్మాణం commands_s *command;

శూన్య
crun_assert_n_వాదనలు (పూర్ణాంకం n, పూర్ణాంకం min, పూర్ణాంకం max)
{
  ఒకవేళ (min >= 0 && n < min)
    libcrun_fail_with_error (0, "`%s` requires a minimum of %d arguments", command->name, min);
  ఒకవేళ (max >= 0 && n > max)
    libcrun_fail_with_error (0, "`%s` requires a maximum of %d arguments", command->name, max);
}

అక్షరం *
argp_mandatory_వాదం (అక్షరం *arg, నిర్మాణం argp_state *state)
{
  ఒకవేళ (arg)
    తిరిగి arg;
  తిరిగి state->argv[state->తదుపరి++];
}

/* Parse a numeric value from STR, aborting on an out-of-range or malformed
   value.  KIND names the field for error messages.  If ENDPTR is not NULL it
   is set past the parsed number (so the caller can continue parsing, e.g. a
   "UID:GID" pair); otherwise the whole string must be a valid number.  */
పూర్ణాంకం
parse_id_or_fail (స్థిరరాశి అక్షరం *str, అక్షరం **endptr, స్థిరరాశి అక్షరం *రకం)
{
  అక్షరం *చివర = ఖాళీ;
  దీర్ఘ దీర్ఘ l;

  errno = 0;
  l = strtoll (str, &చివర, 10);
  ఒకవేళ (చివర == str)
    libcrun_fail_with_error (0, "invalid %s specified", రకం);
  ఒకవేళ (errno == ERANGE)
    libcrun_fail_with_error (0, "invalid %s specified", రకం);
  ఒకవేళ (l < INT_MIN || l > గరిష్ఠ_పూర్ణాంకం)
    libcrun_fail_with_error (0, "invalid %s specified", రకం);

  ఒకవేళ (endptr)
    *endptr = చివర;
  లేకపోతే ఒకవేళ (*చివర != '\0')
    libcrun_fail_with_error (0, "invalid %s specified", రకం);

  తిరిగి (పూర్ణాంకం) l;
}

స్థిర నిర్మాణం argp argp = { options, parse_opt, వాదనలు_doc, doc, ఖాళీ, ఖాళీ, ఖాళీ };

పూర్ణాంకం ensure_cloned_ద్విమాన (శూన్య);

స్థిర శూన్య
fill_handler_from_argv0 (అక్షరం *argv0, నిర్మాణం crun_global_arguments *వాదనలు)
{
  స్థిరరాశి అక్షరం *b = basename (argv0);
#నిర్వచితమైతే HAVE_LIBKRUN
  ఒకవేళ (strcmp (b, "krun") == 0)
    {
      వాదనలు->handler = "krun";
      తిరిగి;
    }
#అయితే_ముగింపు
  ఒకవేళ (అక్షరశ్రేణి_పోలిక (b, "crun-", 5) == 0 && b[5] != '\0')
    వాదనలు->handler = b + 5;
}

స్థిర అక్షరం **
copy_వాదనలు (అక్షరం **argv, పూర్ణాంకం argc)
{
  అక్షరం **buff = xmalloc0 ((argc + 1) * పరిమాణం (అక్షరం *));
  కొరకు (పూర్ణాంకం i = 0; i < argc; i++)
    buff[i] = argv[i];

  తిరిగి buff;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  libcrun_error_t err = ఖాళీ;
  పూర్ణాంకం ret, మొదటి_వాదం = 0;

  arguments.argc = argc;
  arguments.argv = argv;

#నిర్వచితమైతే DYNLOAD_LIBCRUN
  ఒకవేళ (ensure_cloned_ద్విమాన () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      నిష్క్రమించు (EXIT_FAILURE);
    }
  /* Resolve all libcrun weak dependencies.  */
  ఒకవేళ (dlopen ("libcrun.so", RTLD_GLOBAL | RTLD_DEEPBIND | RTLD_LAZY) == ఖాళీ)
    libcrun_fail_with_error (0, "could not load `libcrun.so`: `%s`", dlerror ());
#అయితే_ముగింపు

  fill_handler_from_argv0 (argv[0], &arguments);
  argp_parse (&argp, argc, argv, ARGP_లోపల_క్రమం, &మొదటి_వాదం, &arguments);

  command = get_command (argv[మొదటి_వాదం]);
  ఒకవేళ (command == ఖాళీ)
    libcrun_fail_with_error (0, "unknown command %s", argv[మొదటి_వాదం]);

  పూర్ణాంకం command_argc = argc - మొదటి_వాదం;
  cleanup_free అక్షరం **command_argv = copy_వాదనలు (argv + మొదటి_వాదం, command_argc);
  command_argv[0] = argv[0];

  ret = command->handler (&arguments, command_argc, command_argv, &err);
  ఒకవేళ (ret && err)
    {
      ఒకవేళ (command->విలువ == COMMAND_EXEC)
        {
          libcrun_error (libcrun_error_get_స్థితి (err), "%s", libcrun_error_get_సందేశం (err));
          నిష్క్రమించు (EXIT_EXEC_FAILURE);
        }
      libcrun_fail_with_error (libcrun_error_get_స్థితి (err), "%s", libcrun_error_get_సందేశం (err));
    }

  తిరిగి ret;
}
