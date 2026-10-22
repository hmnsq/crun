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

#اگر_تعریف_شدہ HAVE_DLOPEN
#  include <dlfcn.h>
#اگر_ختم

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
#تعریف_کرو EXIT_EXEC_FAILURE 255

ساکن ساخت crun_global_arguments arguments;

ساخت commands_s
{
  صحیح قدر;
  ثابت حرف *name;
  صحیح (*handler) (ساخت crun_global_arguments *, صحیح, حرف **, libcrun_error_t *);
};

صحیح
init_libcrun_context (libcrun_context_t *con, ثابت حرف *id, ساخت crun_global_arguments *glob, libcrun_error_t *err)
{
  صحیح ret;

  libcrun_context_سیٹ_id (con, id);
  libcrun_context_سیٹ_state_جڑ (con, glob->جڑ);
  libcrun_context_سیٹ_systemd_cgroup (con, glob->option_systemd_cgroup);
  libcrun_context_سیٹ_force_no_cgroup (con, glob->option_force_no_cgroup);
  libcrun_context_سیٹ_notify_socket (con, getenv ("NOTIFY_SOCKET"));
  libcrun_context_سیٹ_دلائل (con, glob->argc, glob->argv);

  /* Check if global handler is configured and pass it down to crun context */
  libcrun_context_سیٹ_handler (con, glob->handler);

  ret = libcrun_context_init_logging (con, glob->لوگارتھم, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (glob->log_format)
    {
      ret = libcrun_سیٹ_log_format (glob->log_format, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  libcrun_سیٹ_verbosity (glob->verbosity);
  libcrun_debug ("Using debug verbosity");

  واپس 0;
}

/* Allocate a public libcrun context (with handlers loaded) for a subcommand.
   Terminates the process on failure.  */
libcrun_context_t *
new_libcrun_context (ساخت crun_global_arguments *glob)
{
  libcrun_error_t err = تہی;
  libcrun_context_t *con;

  con = libcrun_context_new (تہی, glob->جڑ, &err);
  اگر (UNLIKELY (con == تہی))
    libcrun_fail_with_error (libcrun_error_get_حالت (err), "%s", libcrun_error_get_پیغام (err));

  واپس con;
}

شمار
{
  COMMAND_CREATE = 1000,
  COMMAND_DELETE,
  COMMAND_EXEC,
  COMMAND_LIST,
  COMMAND_KILL,
  COMMAND_تسلسل,
  COMMAND_SPEC,
  COMMAND_آغاز,
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

ساخت commands_s commands[] = { { COMMAND_CREATE, "create", crun_command_create },
                                 { COMMAND_DELETE, "delete", crun_command_delete },
                                 { COMMAND_EXEC, "exec", crun_command_exec },
                                 { COMMAND_LIST, "list", crun_command_list },
                                 { COMMAND_KILL, "kill", crun_command_kill },
                                 { COMMAND_PS, "ps", crun_command_ps },
                                 { COMMAND_تسلسل, "run", crun_command_تسلسل },
                                 { COMMAND_SPEC, "spec", crun_command_spec },
                                 { COMMAND_آغاز, "start", crun_command_آغاز },
                                 { COMMAND_STATE, "state", crun_command_state },
                                 { COMMAND_UPDATE, "update", crun_command_update },
                                 { COMMAND_PAUSE, "pause", crun_command_pause },
                                 { COMMAND_UNPAUSE, "resume", crun_command_unpause },
                                 { COMMAND_FEATURES, "features", crun_command_features },
#اگر HAVE_CRIU && HAVE_DLOPEN
                                 { COMMAND_CHECKPOINT, "checkpoint", crun_command_checkpoint },
                                 { COMMAND_RESTORE, "restore", crun_command_restore },
#اگر_ختم
                                 { COMMAND_MOUNTS, "mounts", crun_command_mounts },
                                 {
                                     0,
                                 } };

ساکن حرف doc[] = "\nCOMMANDS:\n"
#اگر HAVE_CRIU && HAVE_DLOPEN
                    "\tcheckpoint  - checkpoint a container\n"
#اگر_ختم
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
#اگر_ختم
                    "\trun         - run a container\n"
                    "\tspec        - generate a configuration file\n"
                    "\tstart       - start a container\n"
                    "\tstate       - output the state of a container\n"
                    "\tpause       - pause all the processes in the container\n"
                    "\tresume      - unpause the processes in the container\n"
                    "\tupdate      - update container resource constraints\n";

ساکن حرف دلائل_doc[] = "COMMAND [OPTION...]";

ساکن ساخت commands_s *
get_command (ثابت حرف *arg)
{
  ساخت commands_s *it;
  برائے (it = commands; it->قدر; it++)
    اگر (strcmp (it->name, arg) == 0)
      واپس it;
  واپس تہی;
}

شمار
{
  OPTION_نسخہ = 'v',
  OPTION_نسخہ_CAP = 'V',
  OPTION_DEBUG = 1000,
  OPTION_SYSTEMD_CGROUP,
  OPTION_CGROUP_MANAGER,
  OPTION_LOG,
  OPTION_LOG_FORMAT,
  OPTION_LOG_LEVEL,
  OPTION_جڑ,
  OPTION_ROOTLESS
};

ثابت حرف *argp_پروگرام_bug_پتہ = "https://github.com/containers/crun/issues";

ساکن ساخت argp_option options[] = { { "debug", OPTION_DEBUG, 0, 0, "produce verbose output, similar to --log-level=debug", 0 },
                                        { "cgroup-manager", OPTION_CGROUP_MANAGER, "MANAGER", 0, "cgroup manager", 0 },
                                        { "systemd-cgroup", OPTION_SYSTEMD_CGROUP, 0, 0, "use systemd cgroups", 0 },
                                        { "log", OPTION_LOG, "FILE", 0, "log destination: '[file:]PATH', 'journald:ID' or 'syslog:ID' (defaults to stderr)", 0 },
                                        { "log-format", OPTION_LOG_FORMAT, "FORMAT", 0, "log format: 'text' (default) or 'json'", 0 },
                                        { "log-level", OPTION_LOG_LEVEL, "LEVEL", 0, "log level to use: 'error' (default), 'warning' or 'debug'", 0 },
                                        { "root", OPTION_جڑ, "DIR", 0, تہی, 0 },
                                        { "rootless", OPTION_ROOTLESS, "VALUE", 0, تہی, 0 },
                                        { "version", OPTION_نسخہ, 0, 0, تہی, 0 },
                                        // alias OPTION_VERSION_CAP with OPTION_VERSION
                                        { تہی, OPTION_نسخہ_CAP, 0, OPTION_ALIAS, تہی, 0 },
                                        {
                                            0,
                                        } };

ساکن خالی
print_نسخہ (FILE *دھارا, ساخت argp_state *state arg_غیر_مستعمل)
{
  libcrun_error_t err = تہی;
  cleanup_free حرف *rundir = تہی;
  libcrun_context_t *ctx;
  حرف **لیبل_فہرست;
  صحیح ret;

  fprintf (دھارا, "%s version %s\n", PACKAGE_NAME, PACKAGE_نسخہ);
  fprintf (دھارا, "commit: %s\n", GIT_نسخہ);

  ret = libcrun_get_state_dir (&rundir, arguments.جڑ, تہی, &err);
  اگر (LIKELY (ret == 0))
    fprintf (دھارا, "rundir: %s\n", rundir);
  ورنہ
    libcrun_error_release (&err);

  fprintf (دھارا, "spec: 1.0.0\n");

#اگر_تعریف_شدہ HAVE_SYSTEMD
  fprintf (دھارا, "+SYSTEMD ");
#اگر_ختم
  fprintf (دھارا, "+SELINUX ");
  fprintf (دھارا, "+APPARMOR ");
#اگر_تعریف_شدہ HAVE_CAP
  fprintf (دھارا, "+CAP ");
#اگر_ختم
#اگر_تعریف_شدہ HAVE_SECCOMP
  fprintf (دھارا, "+SECCOMP ");
#اگر_ختم
#اگر_تعریف_شدہ HAVE_EBPF
  fprintf (دھارا, "+EBPF ");
#اگر_ختم
#اگر HAVE_CRIU && HAVE_DLOPEN
  fprintf (دھارا, "+CRIU ");
#اگر_ختم

  ctx = libcrun_context_new (تہی, arguments.جڑ, &err);
  اگر (ctx)
    {
      لیبل_فہرست = libcrun_context_get_handler_feature_لیبل_فہرست (ctx, &err);
      اگر (لیبل_فہرست)
        {
          برائے (size_t i = 0; لیبل_فہرست[i]; i++)
            {
              fprintf (دھارا, "+%s ", لیبل_فہرست[i]);
              آزاد (لیبل_فہرست[i]);
            }
          آزاد (لیبل_فہرست);
        }
      ورنہ
        libcrun_error_release (&err);
      libcrun_context_free (ctx);
    }
  ورنہ
    libcrun_error_release (&err);

  fprintf (دھارا, "+JSON_C\n");
}

ساکن error_t
parse_opt (صحیح کلید, حرف *arg, ساخت argp_state *state)
{
  ثابت حرف *tmp;

  انتخاب (کلید)
    {
    حالت OPTION_DEBUG:
      arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
      توڑو;

    حالت OPTION_CGROUP_MANAGER:
      tmp = argp_mandatory_دلیل (arg, state);
      اگر (strcmp (tmp, "systemd") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = true;
        }
      ورنہ اگر (strcmp (tmp, "cgroupfs") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = false;
        }
      ورنہ اگر (strcmp (tmp, "disabled") == 0)
        {
          arguments.option_systemd_cgroup = false;
          arguments.option_force_no_cgroup = true;
        }
      ورنہ
        {
          libcrun_fail_with_error (0, "unknown cgroup manager specified");
        }
      توڑو;

    حالت OPTION_SYSTEMD_CGROUP:
      arguments.option_force_no_cgroup = false;
      arguments.option_systemd_cgroup = true;
      توڑو;

    حالت OPTION_LOG:
      arguments.لوگارتھم = argp_mandatory_دلیل (arg, state);
      توڑو;

    حالت OPTION_LOG_FORMAT:
      arguments.log_format = argp_mandatory_دلیل (arg, state);
      توڑو;

    حالت OPTION_LOG_LEVEL:
      tmp = argp_mandatory_دلیل (arg, state);
      اگر (strcmp (tmp, "error") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_ERROR;
        }
      ورنہ اگر (strcmp (tmp, "warning") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_WARNING;
        }
      ورنہ اگر (strcmp (tmp, "debug") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
        }
      ورنہ
        {
          libcrun_fail_with_error (0, "unknown verbosity `%s` specified", arg);
        }
      توڑو;

    حالت OPTION_جڑ:
      arguments.جڑ = argp_mandatory_دلیل (arg, state);
      توڑو;

    حالت OPTION_ROOTLESS:
      /* Ignored.  So that a runc command line won't fail.  */
      توڑو;

    حالت ARGP_کلید_NO_دلائل:
      libcrun_fail_with_error (0, "please specify a command");

    حالت OPTION_نسخہ:
    حالت OPTION_نسخہ_CAP:
      print_نسخہ (stdout, state);
      باہر_نکلو (EXIT_SUCCESS);
    طےشدہ:
      واپس ARGP_ERR_UNKNOWN;
    }

  واپس 0;
}

ساکن ساخت commands_s *command;

خالی
crun_assert_n_دلائل (صحیح n, صحیح min, صحیح max)
{
  اگر (min >= 0 && n < min)
    libcrun_fail_with_error (0, "`%s` requires a minimum of %d arguments", command->name, min);
  اگر (max >= 0 && n > max)
    libcrun_fail_with_error (0, "`%s` requires a maximum of %d arguments", command->name, max);
}

حرف *
argp_mandatory_دلیل (حرف *arg, ساخت argp_state *state)
{
  اگر (arg)
    واپس arg;
  واپس state->argv[state->اگلا++];
}

/* Parse a numeric value from STR, aborting on an out-of-range or malformed
   value.  KIND names the field for error messages.  If ENDPTR is not NULL it
   is set past the parsed number (so the caller can continue parsing, e.g. a
   "UID:GID" pair); otherwise the whole string must be a valid number.  */
صحیح
parse_id_or_fail (ثابت حرف *str, حرف **endptr, ثابت حرف *قسم)
{
  حرف *انتہا = تہی;
  طویل طویل l;

  errno = 0;
  l = strtoll (str, &انتہا, 10);
  اگر (انتہا == str)
    libcrun_fail_with_error (0, "invalid %s specified", قسم);
  اگر (errno == ERANGE)
    libcrun_fail_with_error (0, "invalid %s specified", قسم);
  اگر (l < INT_MIN || l > زیادہ_سے_زیادہ_عدد)
    libcrun_fail_with_error (0, "invalid %s specified", قسم);

  اگر (endptr)
    *endptr = انتہا;
  ورنہ اگر (*انتہا != '\0')
    libcrun_fail_with_error (0, "invalid %s specified", قسم);

  واپس (صحیح) l;
}

ساکن ساخت argp argp = { options, parse_opt, دلائل_doc, doc, تہی, تہی, تہی };

صحیح ensure_cloned_ثنائی (خالی);

ساکن خالی
fill_handler_from_argv0 (حرف *argv0, ساخت crun_global_arguments *دلائل)
{
  ثابت حرف *b = basename (argv0);
#اگر_تعریف_شدہ HAVE_LIBKRUN
  اگر (strcmp (b, "krun") == 0)
    {
      دلائل->handler = "krun";
      واپس;
    }
#اگر_ختم
  اگر (لڑی_موازنہ (b, "crun-", 5) == 0 && b[5] != '\0')
    دلائل->handler = b + 5;
}

ساکن حرف **
copy_دلائل (حرف **argv, صحیح argc)
{
  حرف **buff = xmalloc0 ((argc + 1) * حجم (حرف *));
  برائے (صحیح i = 0; i < argc; i++)
    buff[i] = argv[i];

  واپس buff;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  libcrun_error_t err = تہی;
  صحیح ret, پہلا_دلیل = 0;

  arguments.argc = argc;
  arguments.argv = argv;

#اگر_تعریف_شدہ DYNLOAD_LIBCRUN
  اگر (ensure_cloned_ثنائی () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      باہر_نکلو (EXIT_FAILURE);
    }
  /* Resolve all libcrun weak dependencies.  */
  اگر (dlopen ("libcrun.so", RTLD_GLOBAL | RTLD_DEEPBIND | RTLD_LAZY) == تہی)
    libcrun_fail_with_error (0, "could not load `libcrun.so`: `%s`", dlerror ());
#اگر_ختم

  fill_handler_from_argv0 (argv[0], &arguments);
  argp_parse (&argp, argc, argv, ARGP_اندر_ترتیب, &پہلا_دلیل, &arguments);

  command = get_command (argv[پہلا_دلیل]);
  اگر (command == تہی)
    libcrun_fail_with_error (0, "unknown command %s", argv[پہلا_دلیل]);

  صحیح command_argc = argc - پہلا_دلیل;
  cleanup_free حرف **command_argv = copy_دلائل (argv + پہلا_دلیل, command_argc);
  command_argv[0] = argv[0];

  ret = command->handler (&arguments, command_argc, command_argv, &err);
  اگر (ret && err)
    {
      اگر (command->قدر == COMMAND_EXEC)
        {
          libcrun_error (libcrun_error_get_حالت (err), "%s", libcrun_error_get_پیغام (err));
          باہر_نکلو (EXIT_EXEC_FAILURE);
        }
      libcrun_fail_with_error (libcrun_error_get_حالت (err), "%s", libcrun_error_get_پیغام (err));
    }

  واپس ret;
}
