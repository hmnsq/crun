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

#अन्तर्भाव <config.h>
#अन्तर्भाव <git-version.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <argp.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <libgen.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <limits.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#अन्तर्भाव "crun.h"

/* Commands.  */
#अन्तर्भाव "run.h"
#अन्तर्भाव "delete.h"
#अन्तर्भाव "kill.h"
#अन्तर्भाव "list.h"
#अन्तर्भाव "start.h"
#अन्तर्भाव "create.h"
#अन्तर्भाव "exec.h"
#अन्तर्भाव "state.h"
#अन्तर्भाव "update.h"
#अन्तर्भाव "spec.h"
#अन्तर्भाव "pause.h"
#अन्तर्भाव "unpause.h"
#अन्तर्भाव "oci_features.h"
#अन्तर्भाव "ps.h"
#अन्तर्भाव "checkpoint.h"
#अन्तर्भाव "mounts.h"
#अन्तर्भाव "restore.h"

/* crun exec exits with the exit code of the executed process, so use this
   code for its own errors, to tell those apart (like runc and ssh do).  */
#define EXIT_EXEC_FAILURE 255

स्थिर संरचना crun_global_arguments arguments;

संरचना commands_s
{
  पूर्णाङ्क मूल्य;
  ध्रुव वर्ण *name;
  पूर्णाङ्क (*handler) (संरचना crun_global_arguments *, पूर्णाङ्क, वर्ण **, libcrun_error_t *);
};

पूर्णाङ्क
init_libcrun_context (libcrun_context_t *con, ध्रुव वर्ण *id, संरचना crun_global_arguments *glob, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  libcrun_context_set_id (con, id);
  libcrun_context_set_state_मूल (con, glob->मूल);
  libcrun_context_set_systemd_cgroup (con, glob->option_systemd_cgroup);
  libcrun_context_set_force_no_cgroup (con, glob->option_force_no_cgroup);
  libcrun_context_set_notify_socket (con, दो_पर्या ("NOTIFY_SOCKET"));
  libcrun_context_set_args (con, glob->argc, glob->argv);

  /* Check if global handler is configured and pass it down to crun context */
  libcrun_context_set_handler (con, glob->handler);

  ret = libcrun_context_init_logging (con, glob->log, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (glob->log_format)
    {
      ret = libcrun_set_log_format (glob->log_format, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  libcrun_set_verbosity (glob->verbosity);
  libcrun_debug ("Using debug verbosity");

  प्रत्यागम 0;
}

/* Allocate a public libcrun context (with handlers loaded) for a subcommand.
   Terminates the process on failure.  */
libcrun_context_t *
new_libcrun_context (संरचना crun_global_arguments *glob)
{
  libcrun_error_t err = रिक्त;
  libcrun_context_t *con;

  con = libcrun_context_new (रिक्त, glob->मूल, &err);
  यदि (UNLIKELY (con == रिक्त))
    libcrun_fail_with_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));

  प्रत्यागम con;
}

गणना
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

संरचना commands_s commands[] = { { COMMAND_CREATE, "create", crun_command_create },
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
#यदि HAVE_CRIU && HAVE_DLOPEN
                                 { COMMAND_CHECKPOINT, "checkpoint", crun_command_checkpoint },
                                 { COMMAND_RESTORE, "restore", crun_command_restore },
#endif
                                 { COMMAND_MOUNTS, "mounts", crun_command_mounts },
                                 {
                                     0,
                                 } };

स्थिर वर्ण doc[] = "\nCOMMANDS:\n"
#यदि HAVE_CRIU && HAVE_DLOPEN
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
#यदि HAVE_CRIU && HAVE_DLOPEN
                    "\trestore     - restore a container\n"
#endif
                    "\trun         - run a container\n"
                    "\tspec        - generate a configuration file\n"
                    "\tstart       - start a container\n"
                    "\tstate       - output the state of a container\n"
                    "\tpause       - pause all the processes in the container\n"
                    "\tresume      - unpause the processes in the container\n"
                    "\tupdate      - update container resource constraints\n";

स्थिर वर्ण args_doc[] = "COMMAND [OPTION...]";

स्थिर संरचना commands_s *
get_command (ध्रुव वर्ण *arg)
{
  संरचना commands_s *it;
  प्रति (it = commands; it->मूल्य; it++)
    यदि (म_भेद (it->name, arg) == 0)
      प्रत्यागम it;
  प्रत्यागम रिक्त;
}

गणना
{
  OPTION_VERSION = 'v',
  OPTION_VERSION_CAP = 'V',
  OPTION_DEBUG = 1000,
  OPTION_SYSTEMD_CGROUP,
  OPTION_CGROUP_MANAGER,
  OPTION_LOG,
  OPTION_LOG_FORMAT,
  OPTION_LOG_LEVEL,
  OPTION_मूल,
  OPTION_ROOTLESS
};

ध्रुव वर्ण *argp_program_bug_address = "https://github.com/containers/crun/issues";

स्थिर संरचना argp_option options[] = { { "debug", OPTION_DEBUG, 0, 0, "produce verbose output, similar to --log-level=debug", 0 },
                                        { "cgroup-manager", OPTION_CGROUP_MANAGER, "MANAGER", 0, "cgroup manager", 0 },
                                        { "systemd-cgroup", OPTION_SYSTEMD_CGROUP, 0, 0, "use systemd cgroups", 0 },
                                        { "log", OPTION_LOG, "FILE", 0, "log destination: '[file:]PATH', 'journald:ID' or 'syslog:ID' (defaults to stderr)", 0 },
                                        { "log-format", OPTION_LOG_FORMAT, "FORMAT", 0, "log format: 'text' (default) or 'json'", 0 },
                                        { "log-level", OPTION_LOG_LEVEL, "LEVEL", 0, "log level to use: 'error' (default), 'warning' or 'debug'", 0 },
                                        { "root", OPTION_मूल, "DIR", 0, रिक्त, 0 },
                                        { "rootless", OPTION_ROOTLESS, "VALUE", 0, रिक्त, 0 },
                                        { "version", OPTION_VERSION, 0, 0, रिक्त, 0 },
                                        // alias OPTION_VERSION_CAP with OPTION_VERSION
                                        { रिक्त, OPTION_VERSION_CAP, 0, OPTION_ALIAS, रिक्त, 0 },
                                        {
                                            0,
                                        } };

स्थिर शून्य
print_version (खाता *stream, संरचना argp_state *state arg_unused)
{
  libcrun_error_t err = रिक्त;
  cleanup_free वर्ण *rundir = रिक्त;
  libcrun_context_t *ctx;
  वर्ण **tags;
  पूर्णाङ्क ret;

  ख_लिखो (stream, "%s version %s\n", PACKAGE_NAME, PACKAGE_VERSION);
  ख_लिखो (stream, "commit: %s\n", GIT_VERSION);

  ret = libcrun_get_state_dir (&rundir, arguments.मूल, रिक्त, &err);
  यदि (LIKELY (ret == 0))
    ख_लिखो (stream, "rundir: %s\n", rundir);
  अन्यथा
    libcrun_error_release (&err);

  ख_लिखो (stream, "spec: 1.0.0\n");

#ifdef HAVE_SYSTEMD
  ख_लिखो (stream, "+SYSTEMD ");
#endif
  ख_लिखो (stream, "+SELINUX ");
  ख_लिखो (stream, "+APPARMOR ");
#ifdef HAVE_CAP
  ख_लिखो (stream, "+CAP ");
#endif
#ifdef HAVE_SECCOMP
  ख_लिखो (stream, "+SECCOMP ");
#endif
#ifdef HAVE_EBPF
  ख_लिखो (stream, "+EBPF ");
#endif
#यदि HAVE_CRIU && HAVE_DLOPEN
  ख_लिखो (stream, "+CRIU ");
#endif

  ctx = libcrun_context_new (रिक्त, arguments.मूल, &err);
  यदि (ctx)
    {
      tags = libcrun_context_get_handler_feature_tags (ctx, &err);
      यदि (tags)
        {
          प्रति (माप_प्रकार i = 0; tags[i]; i++)
            {
              ख_लिखो (stream, "+%s ", tags[i]);
              मुक्त (tags[i]);
            }
          मुक्त (tags);
        }
      अन्यथा
        libcrun_error_release (&err);
      libcrun_context_free (ctx);
    }
  अन्यथा
    libcrun_error_release (&err);

  ख_लिखो (stream, "+JSON_C\n");
}

स्थिर error_t
parse_opt (पूर्णाङ्क कुंजी, वर्ण *arg, संरचना argp_state *state)
{
  ध्रुव वर्ण *tmp;

  विकल्प (कुंजी)
    {
    पक्ष OPTION_DEBUG:
      arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
      भङ्ग;

    पक्ष OPTION_CGROUP_MANAGER:
      tmp = argp_mandatory_argument (arg, state);
      यदि (म_भेद (tmp, "systemd") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = true;
        }
      अन्यथा यदि (म_भेद (tmp, "cgroupfs") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = false;
        }
      अन्यथा यदि (म_भेद (tmp, "disabled") == 0)
        {
          arguments.option_systemd_cgroup = false;
          arguments.option_force_no_cgroup = true;
        }
      अन्यथा
        {
          libcrun_fail_with_error (0, "unknown cgroup manager specified");
        }
      भङ्ग;

    पक्ष OPTION_SYSTEMD_CGROUP:
      arguments.option_force_no_cgroup = false;
      arguments.option_systemd_cgroup = true;
      भङ्ग;

    पक्ष OPTION_LOG:
      arguments.log = argp_mandatory_argument (arg, state);
      भङ्ग;

    पक्ष OPTION_LOG_FORMAT:
      arguments.log_format = argp_mandatory_argument (arg, state);
      भङ्ग;

    पक्ष OPTION_LOG_LEVEL:
      tmp = argp_mandatory_argument (arg, state);
      यदि (म_भेद (tmp, "error") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_ERROR;
        }
      अन्यथा यदि (म_भेद (tmp, "warning") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_WARNING;
        }
      अन्यथा यदि (म_भेद (tmp, "debug") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
        }
      अन्यथा
        {
          libcrun_fail_with_error (0, "unknown verbosity `%s` specified", arg);
        }
      भङ्ग;

    पक्ष OPTION_मूल:
      arguments.मूल = argp_mandatory_argument (arg, state);
      भङ्ग;

    पक्ष OPTION_ROOTLESS:
      /* Ignored.  So that a runc command line won't fail.  */
      भङ्ग;

    पक्ष ARGP_कुंजी_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a command");

    पक्ष OPTION_VERSION:
    पक्ष OPTION_VERSION_CAP:
      print_version (मानक_निकास, state);
      निकास (निकास_सफल);
    स्वभाव:
      प्रत्यागम ARGP_ERR_UNKNOWN;
    }

  प्रत्यागम 0;
}

स्थिर संरचना commands_s *command;

शून्य
crun_assert_n_args (पूर्णाङ्क n, पूर्णाङ्क min, पूर्णाङ्क max)
{
  यदि (min >= 0 && n < min)
    libcrun_fail_with_error (0, "`%s` requires a minimum of %d arguments", command->name, min);
  यदि (max >= 0 && n > max)
    libcrun_fail_with_error (0, "`%s` requires a maximum of %d arguments", command->name, max);
}

वर्ण *
argp_mandatory_argument (वर्ण *arg, संरचना argp_state *state)
{
  यदि (arg)
    प्रत्यागम arg;
  प्रत्यागम state->argv[state->next++];
}

/* Parse a numeric value from STR, aborting on an out-of-range or malformed
   value.  KIND names the field for error messages.  If ENDPTR is not NULL it
   is set past the parsed number (so the caller can continue parsing, e.g. a
   "UID:GID" pair); otherwise the whole string must be a valid number.  */
पूर्णाङ्क
parse_id_or_fail (ध्रुव वर्ण *str, वर्ण **endptr, ध्रुव वर्ण *kind)
{
  वर्ण *end = रिक्त;
  दीर्घ दीर्घ l;

  त्रुटि_सं = 0;
  l = strtoll (str, &end, 10);
  यदि (end == str)
    libcrun_fail_with_error (0, "invalid %s specified", kind);
  यदि (त्रुटि_सं == दुस्फल)
    libcrun_fail_with_error (0, "invalid %s specified", kind);
  यदि (l < पूर्णांक_न्यून || l > पूर्णांक_उच्च)
    libcrun_fail_with_error (0, "invalid %s specified", kind);

  यदि (endptr)
    *endptr = end;
  अन्यथा यदि (*end != '\0')
    libcrun_fail_with_error (0, "invalid %s specified", kind);

  प्रत्यागम (पूर्णाङ्क) l;
}

स्थिर संरचना argp argp = { options, parse_opt, args_doc, doc, रिक्त, रिक्त, रिक्त };

पूर्णाङ्क ensure_cloned_द्विआधारी (शून्य);

स्थिर शून्य
fill_handler_from_argv0 (वर्ण *argv0, संरचना crun_global_arguments *args)
{
  ध्रुव वर्ण *b = basename (argv0);
#ifdef HAVE_LIBKRUN
  यदि (म_भेद (b, "krun") == 0)
    {
      args->handler = "krun";
      प्रत्यागम;
    }
#endif
  यदि (म_भेदन (b, "crun-", 5) == 0 && b[5] != '\0')
    args->handler = b + 5;
}

स्थिर वर्ण **
copy_args (वर्ण **argv, पूर्णाङ्क argc)
{
  वर्ण **buff = xmalloc0 ((argc + 1) * परिमाण (वर्ण *));
  प्रति (पूर्णाङ्क i = 0; i < argc; i++)
    buff[i] = argv[i];

  प्रत्यागम buff;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  libcrun_error_t err = रिक्त;
  पूर्णाङ्क ret, first_argument = 0;

  arguments.argc = argc;
  arguments.argv = argv;

#ifdef DYNLOAD_LIBCRUN
  यदि (ensure_cloned_द्विआधारी () < 0)
    {
      ख_लिखो (मानक_त्रुटि, "Failed to re-execute libcrun via memory file descriptor\n");
      निकास (निकास_त्रुटि);
    }
  /* Resolve all libcrun weak dependencies.  */
  यदि (dlopen ("libcrun.so", RTLD_GLOBAL | RTLD_DEEPBIND | RTLD_LAZY) == रिक्त)
    libcrun_fail_with_error (0, "could not load `libcrun.so`: `%s`", dlerror ());
#endif

  fill_handler_from_argv0 (argv[0], &arguments);
  argp_parse (&argp, argc, argv, ARGP_IN_ORDER, &first_argument, &arguments);

  command = get_command (argv[first_argument]);
  यदि (command == रिक्त)
    libcrun_fail_with_error (0, "unknown command %s", argv[first_argument]);

  पूर्णाङ्क command_argc = argc - first_argument;
  cleanup_free वर्ण **command_argv = copy_args (argv + first_argument, command_argc);
  command_argv[0] = argv[0];

  ret = command->handler (&arguments, command_argc, command_argv, &err);
  यदि (ret && err)
    {
      यदि (command->मूल्य == COMMAND_EXEC)
        {
          libcrun_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));
          निकास (EXIT_EXEC_FAILURE);
        }
      libcrun_fail_with_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));
    }

  प्रत्यागम ret;
}
