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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DLOPEN
#  include <dlfcn.h>
#ಆದರೆ_ಅಂತ್ಯ

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
#ವ್ಯಾಖ್ಯಾನಿಸು EXIT_EXEC_FAILURE 255

ಸ್ಥಿರ ರಚನೆ crun_global_arguments arguments;

ರಚನೆ commands_s
{
  ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
  ಪೂರ್ಣಾಂಕ (*handler) (ರಚನೆ crun_global_arguments *, ಪೂರ್ಣಾಂಕ, ಅಕ್ಷರ **, libcrun_error_t *);
};

ಪೂರ್ಣಾಂಕ
init_libcrun_context (libcrun_context_t *con, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ರಚನೆ crun_global_arguments *glob, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  libcrun_context_ಗಣ_id (con, id);
  libcrun_context_ಗಣ_state_ಬೇರು (con, glob->ಬೇರು);
  libcrun_context_ಗಣ_systemd_cgroup (con, glob->option_systemd_cgroup);
  libcrun_context_ಗಣ_force_no_cgroup (con, glob->option_force_no_cgroup);
  libcrun_context_ಗಣ_notify_socket (con, getenv ("NOTIFY_SOCKET"));
  libcrun_context_ಗಣ_ವಾದಗಳು (con, glob->argc, glob->argv);

  /* Check if global handler is configured and pass it down to crun context */
  libcrun_context_ಗಣ_handler (con, glob->handler);

  ret = libcrun_context_init_logging (con, glob->ಲಘುಗಣಕ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (glob->log_format)
    {
      ret = libcrun_ಗಣ_log_format (glob->log_format, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  libcrun_ಗಣ_verbosity (glob->verbosity);
  libcrun_debug ("Using debug verbosity");

  ಹಿಂತಿರುಗಿ 0;
}

/* Allocate a public libcrun context (with handlers loaded) for a subcommand.
   Terminates the process on failure.  */
libcrun_context_t *
new_libcrun_context (ರಚನೆ crun_global_arguments *glob)
{
  libcrun_error_t err = ಖಾಲಿ;
  libcrun_context_t *con;

  con = libcrun_context_new (ಖಾಲಿ, glob->ಬೇರು, &err);
  ಆದರೆ (UNLIKELY (con == ಖಾಲಿ))
    libcrun_fail_with_error (libcrun_error_get_ಸ್ಥಿತಿ (err), "%s", libcrun_error_get_ಸಂದೇಶ (err));

  ಹಿಂತಿರುಗಿ con;
}

ಎಣಿಕೆ
{
  COMMAND_CREATE = 1000,
  COMMAND_DELETE,
  COMMAND_EXEC,
  COMMAND_LIST,
  COMMAND_KILL,
  COMMAND_ಸತತ,
  COMMAND_SPEC,
  COMMAND_ಆರಂಭ,
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

ರಚನೆ commands_s commands[] = { { COMMAND_CREATE, "create", crun_command_create },
                                 { COMMAND_DELETE, "delete", crun_command_delete },
                                 { COMMAND_EXEC, "exec", crun_command_exec },
                                 { COMMAND_LIST, "list", crun_command_list },
                                 { COMMAND_KILL, "kill", crun_command_kill },
                                 { COMMAND_PS, "ps", crun_command_ps },
                                 { COMMAND_ಸತತ, "run", crun_command_ಸತತ },
                                 { COMMAND_SPEC, "spec", crun_command_spec },
                                 { COMMAND_ಆರಂಭ, "start", crun_command_ಆರಂಭ },
                                 { COMMAND_STATE, "state", crun_command_state },
                                 { COMMAND_UPDATE, "update", crun_command_update },
                                 { COMMAND_PAUSE, "pause", crun_command_pause },
                                 { COMMAND_UNPAUSE, "resume", crun_command_unpause },
                                 { COMMAND_FEATURES, "features", crun_command_features },
#ಆದರೆ HAVE_CRIU && HAVE_DLOPEN
                                 { COMMAND_CHECKPOINT, "checkpoint", crun_command_checkpoint },
                                 { COMMAND_RESTORE, "restore", crun_command_restore },
#ಆದರೆ_ಅಂತ್ಯ
                                 { COMMAND_MOUNTS, "mounts", crun_command_mounts },
                                 {
                                     0,
                                 } };

ಸ್ಥಿರ ಅಕ್ಷರ doc[] = "\nCOMMANDS:\n"
#ಆದರೆ HAVE_CRIU && HAVE_DLOPEN
                    "\tcheckpoint  - checkpoint a container\n"
#ಆದರೆ_ಅಂತ್ಯ
                    "\tcreate      - create a container\n"
                    "\tdelete      - remove definition for a container\n"
                    "\texec        - exec a command in a running container\n"
                    "\tfeatures    - show the enabled features\n"
                    "\tlist        - list known containers\n"
                    "\tmounts      - add or remove mounts from a running container\n"
                    "\tkill        - send a signal to the container init process\n"
                    "\tps          - show the processes in the container\n"
#ಆದರೆ HAVE_CRIU && HAVE_DLOPEN
                    "\trestore     - restore a container\n"
#ಆದರೆ_ಅಂತ್ಯ
                    "\trun         - run a container\n"
                    "\tspec        - generate a configuration file\n"
                    "\tstart       - start a container\n"
                    "\tstate       - output the state of a container\n"
                    "\tpause       - pause all the processes in the container\n"
                    "\tresume      - unpause the processes in the container\n"
                    "\tupdate      - update container resource constraints\n";

ಸ್ಥಿರ ಅಕ್ಷರ ವಾದಗಳು_doc[] = "COMMAND [OPTION...]";

ಸ್ಥಿರ ರಚನೆ commands_s *
get_command (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
  ರಚನೆ commands_s *it;
  ಗಾಗಿ (it = commands; it->ಮೌಲ್ಯ; it++)
    ಆದರೆ (strcmp (it->name, arg) == 0)
      ಹಿಂತಿರುಗಿ it;
  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಎಣಿಕೆ
{
  OPTION_ಆವೃತ್ತಿ = 'v',
  OPTION_ಆವೃತ್ತಿ_CAP = 'V',
  OPTION_DEBUG = 1000,
  OPTION_SYSTEMD_CGROUP,
  OPTION_CGROUP_MANAGER,
  OPTION_LOG,
  OPTION_LOG_FORMAT,
  OPTION_LOG_LEVEL,
  OPTION_ಬೇರು,
  OPTION_ROOTLESS
};

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *argp_ಕಾರ್ಯಕ್ರಮ_bug_ವಿಳಾಸ = "https://github.com/containers/crun/issues";

ಸ್ಥಿರ ರಚನೆ argp_option options[] = { { "debug", OPTION_DEBUG, 0, 0, "produce verbose output, similar to --log-level=debug", 0 },
                                        { "cgroup-manager", OPTION_CGROUP_MANAGER, "MANAGER", 0, "cgroup manager", 0 },
                                        { "systemd-cgroup", OPTION_SYSTEMD_CGROUP, 0, 0, "use systemd cgroups", 0 },
                                        { "log", OPTION_LOG, "FILE", 0, "log destination: '[file:]PATH', 'journald:ID' or 'syslog:ID' (defaults to stderr)", 0 },
                                        { "log-format", OPTION_LOG_FORMAT, "FORMAT", 0, "log format: 'text' (default) or 'json'", 0 },
                                        { "log-level", OPTION_LOG_LEVEL, "LEVEL", 0, "log level to use: 'error' (default), 'warning' or 'debug'", 0 },
                                        { "root", OPTION_ಬೇರು, "DIR", 0, ಖಾಲಿ, 0 },
                                        { "rootless", OPTION_ROOTLESS, "VALUE", 0, ಖಾಲಿ, 0 },
                                        { "version", OPTION_ಆವೃತ್ತಿ, 0, 0, ಖಾಲಿ, 0 },
                                        // alias OPTION_VERSION_CAP with OPTION_VERSION
                                        { ಖಾಲಿ, OPTION_ಆವೃತ್ತಿ_CAP, 0, OPTION_ALIAS, ಖಾಲಿ, 0 },
                                        {
                                            0,
                                        } };

ಸ್ಥಿರ ಶೂನ್ಯ
print_ಆವೃತ್ತಿ (FILE *ಹರಿವು, ರಚನೆ argp_state *state arg_ಬಳಸದ)
{
  libcrun_error_t err = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *rundir = ಖಾಲಿ;
  libcrun_context_t *ctx;
  ಅಕ್ಷರ **ಟ್ಯಾಗುಗಳು;
  ಪೂರ್ಣಾಂಕ ret;

  fprintf (ಹರಿವು, "%s version %s\n", PACKAGE_NAME, PACKAGE_ಆವೃತ್ತಿ);
  fprintf (ಹರಿವು, "commit: %s\n", GIT_ಆವೃತ್ತಿ);

  ret = libcrun_get_state_dir (&rundir, arguments.ಬೇರು, ಖಾಲಿ, &err);
  ಆದರೆ (LIKELY (ret == 0))
    fprintf (ಹರಿವು, "rundir: %s\n", rundir);
  ಇಲ್ಲದಿದ್ದರೆ
    libcrun_error_release (&err);

  fprintf (ಹರಿವು, "spec: 1.0.0\n");

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYSTEMD
  fprintf (ಹರಿವು, "+SYSTEMD ");
#ಆದರೆ_ಅಂತ್ಯ
  fprintf (ಹರಿವು, "+SELINUX ");
  fprintf (ಹರಿವು, "+APPARMOR ");
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CAP
  fprintf (ಹರಿವು, "+CAP ");
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SECCOMP
  fprintf (ಹರಿವು, "+SECCOMP ");
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_EBPF
  fprintf (ಹರಿವು, "+EBPF ");
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_CRIU && HAVE_DLOPEN
  fprintf (ಹರಿವು, "+CRIU ");
#ಆದರೆ_ಅಂತ್ಯ

  ctx = libcrun_context_new (ಖಾಲಿ, arguments.ಬೇರು, &err);
  ಆದರೆ (ctx)
    {
      ಟ್ಯಾಗುಗಳು = libcrun_context_get_handler_feature_ಟ್ಯಾಗುಗಳು (ctx, &err);
      ಆದರೆ (ಟ್ಯಾಗುಗಳು)
        {
          ಗಾಗಿ (size_t i = 0; ಟ್ಯಾಗುಗಳು[i]; i++)
            {
              fprintf (ಹರಿವು, "+%s ", ಟ್ಯಾಗುಗಳು[i]);
              ಬಿಡುಗಡೆ (ಟ್ಯಾಗುಗಳು[i]);
            }
          ಬಿಡುಗಡೆ (ಟ್ಯಾಗುಗಳು);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        libcrun_error_release (&err);
      libcrun_context_free (ctx);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    libcrun_error_release (&err);

  fprintf (ಹರಿವು, "+JSON_C\n");
}

ಸ್ಥಿರ error_t
parse_opt (ಪೂರ್ಣಾಂಕ ಕೀಲಿ, ಅಕ್ಷರ *arg, ರಚನೆ argp_state *state)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *tmp;

  ಆಯ್ಕೆ (ಕೀಲಿ)
    {
    ಸ್ಥಿತಿ OPTION_DEBUG:
      arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_CGROUP_MANAGER:
      tmp = argp_mandatory_ವಾದ (arg, state);
      ಆದರೆ (strcmp (tmp, "systemd") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = true;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (tmp, "cgroupfs") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = false;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (tmp, "disabled") == 0)
        {
          arguments.option_systemd_cgroup = false;
          arguments.option_force_no_cgroup = true;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          libcrun_fail_with_error (0, "unknown cgroup manager specified");
        }
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_SYSTEMD_CGROUP:
      arguments.option_force_no_cgroup = false;
      arguments.option_systemd_cgroup = true;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_LOG:
      arguments.ಲಘುಗಣಕ = argp_mandatory_ವಾದ (arg, state);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_LOG_FORMAT:
      arguments.log_format = argp_mandatory_ವಾದ (arg, state);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_LOG_LEVEL:
      tmp = argp_mandatory_ವಾದ (arg, state);
      ಆದರೆ (strcmp (tmp, "error") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_ERROR;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (tmp, "warning") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_WARNING;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (tmp, "debug") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          libcrun_fail_with_error (0, "unknown verbosity `%s` specified", arg);
        }
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_ಬೇರು:
      arguments.ಬೇರು = argp_mandatory_ವಾದ (arg, state);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_ROOTLESS:
      /* Ignored.  So that a runc command line won't fail.  */
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ARGP_ಕೀಲಿ_NO_ವಾದಗಳು:
      libcrun_fail_with_error (0, "please specify a command");

    ಸ್ಥಿತಿ OPTION_ಆವೃತ್ತಿ:
    ಸ್ಥಿತಿ OPTION_ಆವೃತ್ತಿ_CAP:
      print_ಆವೃತ್ತಿ (stdout, state);
      ನಿರ್ಗಮಿಸು (EXIT_SUCCESS);
    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ ARGP_ERR_UNKNOWN;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ರಚನೆ commands_s *command;

ಶೂನ್ಯ
crun_assert_n_ವಾದಗಳು (ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ min, ಪೂರ್ಣಾಂಕ max)
{
  ಆದರೆ (min >= 0 && n < min)
    libcrun_fail_with_error (0, "`%s` requires a minimum of %d arguments", command->name, min);
  ಆದರೆ (max >= 0 && n > max)
    libcrun_fail_with_error (0, "`%s` requires a maximum of %d arguments", command->name, max);
}

ಅಕ್ಷರ *
argp_mandatory_ವಾದ (ಅಕ್ಷರ *arg, ರಚನೆ argp_state *state)
{
  ಆದರೆ (arg)
    ಹಿಂತಿರುಗಿ arg;
  ಹಿಂತಿರುಗಿ state->argv[state->ಮುಂದಿನ++];
}

/* Parse a numeric value from STR, aborting on an out-of-range or malformed
   value.  KIND names the field for error messages.  If ENDPTR is not NULL it
   is set past the parsed number (so the caller can continue parsing, e.g. a
   "UID:GID" pair); otherwise the whole string must be a valid number.  */
ಪೂರ್ಣಾಂಕ
parse_id_or_fail (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str, ಅಕ್ಷರ **endptr, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಗೆ)
{
  ಅಕ್ಷರ *ಕೊನೆ = ಖಾಲಿ;
  ದೀರ್ಘ ದೀರ್ಘ l;

  errno = 0;
  l = strtoll (str, &ಕೊನೆ, 10);
  ಆದರೆ (ಕೊನೆ == str)
    libcrun_fail_with_error (0, "invalid %s specified", ಬಗೆ);
  ಆದರೆ (errno == ERANGE)
    libcrun_fail_with_error (0, "invalid %s specified", ಬಗೆ);
  ಆದರೆ (l < INT_MIN || l > ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
    libcrun_fail_with_error (0, "invalid %s specified", ಬಗೆ);

  ಆದರೆ (endptr)
    *endptr = ಕೊನೆ;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*ಕೊನೆ != '\0')
    libcrun_fail_with_error (0, "invalid %s specified", ಬಗೆ);

  ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) l;
}

ಸ್ಥಿರ ರಚನೆ argp argp = { options, parse_opt, ವಾದಗಳು_doc, doc, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ };

ಪೂರ್ಣಾಂಕ ensure_cloned_ದ್ವಿಮಾನ (ಶೂನ್ಯ);

ಸ್ಥಿರ ಶೂನ್ಯ
fill_handler_from_argv0 (ಅಕ್ಷರ *argv0, ರಚನೆ crun_global_arguments *ವಾದಗಳು)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *b = basename (argv0);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_LIBKRUN
  ಆದರೆ (strcmp (b, "krun") == 0)
    {
      ವಾದಗಳು->handler = "krun";
      ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ
  ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ (b, "crun-", 5) == 0 && b[5] != '\0')
    ವಾದಗಳು->handler = b + 5;
}

ಸ್ಥಿರ ಅಕ್ಷರ **
copy_ವಾದಗಳು (ಅಕ್ಷರ **argv, ಪೂರ್ಣಾಂಕ argc)
{
  ಅಕ್ಷರ **buff = xmalloc0 ((argc + 1) * ಗಾತ್ರ (ಅಕ್ಷರ *));
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < argc; i++)
    buff[i] = argv[i];

  ಹಿಂತಿರುಗಿ buff;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  libcrun_error_t err = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret, ಮೊದಲ_ವಾದ = 0;

  arguments.argc = argc;
  arguments.argv = argv;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DYNLOAD_LIBCRUN
  ಆದರೆ (ensure_cloned_ದ್ವಿಮಾನ () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      ನಿರ್ಗಮಿಸು (EXIT_FAILURE);
    }
  /* Resolve all libcrun weak dependencies.  */
  ಆದರೆ (dlopen ("libcrun.so", RTLD_GLOBAL | RTLD_DEEPBIND | RTLD_LAZY) == ಖಾಲಿ)
    libcrun_fail_with_error (0, "could not load `libcrun.so`: `%s`", dlerror ());
#ಆದರೆ_ಅಂತ್ಯ

  fill_handler_from_argv0 (argv[0], &arguments);
  argp_parse (&argp, argc, argv, ARGP_ಒಳ_ಕ್ರಮ, &ಮೊದಲ_ವಾದ, &arguments);

  command = get_command (argv[ಮೊದಲ_ವಾದ]);
  ಆದರೆ (command == ಖಾಲಿ)
    libcrun_fail_with_error (0, "unknown command %s", argv[ಮೊದಲ_ವಾದ]);

  ಪೂರ್ಣಾಂಕ command_argc = argc - ಮೊದಲ_ವಾದ;
  cleanup_free ಅಕ್ಷರ **command_argv = copy_ವಾದಗಳು (argv + ಮೊದಲ_ವಾದ, command_argc);
  command_argv[0] = argv[0];

  ret = command->handler (&arguments, command_argc, command_argv, &err);
  ಆದರೆ (ret && err)
    {
      ಆದರೆ (command->ಮೌಲ್ಯ == COMMAND_EXEC)
        {
          libcrun_error (libcrun_error_get_ಸ್ಥಿತಿ (err), "%s", libcrun_error_get_ಸಂದೇಶ (err));
          ನಿರ್ಗಮಿಸು (EXIT_EXEC_FAILURE);
        }
      libcrun_fail_with_error (libcrun_error_get_ಸ್ಥಿತಿ (err), "%s", libcrun_error_get_ಸಂದೇಶ (err));
    }

  ಹಿಂತಿರುಗಿ ret;
}
