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

#समावेश <config.h>
#समावेश <git-version.h>
#समावेश <stdlib.h>
#समावेश <argp.h>
#समावेश <string.h>
#समावेश <libgen.h>
#समावेश <errno.h>
#समावेश <limits.h>

#अगर_घोषित HAVE_DLOPEN
#  include <dlfcn.h>
#पूर्ण_अगर

#समावेश "crun.h"

/* Commands.  */
#समावेश "run.h"
#समावेश "delete.h"
#समावेश "kill.h"
#समावेश "list.h"
#समावेश "start.h"
#समावेश "create.h"
#समावेश "exec.h"
#समावेश "state.h"
#समावेश "update.h"
#समावेश "spec.h"
#समावेश "pause.h"
#समावेश "unpause.h"
#समावेश "oci_features.h"
#समावेश "ps.h"
#समावेश "checkpoint.h"
#समावेश "mounts.h"
#समावेश "restore.h"

/* crun exec exits with the exit code of the executed process, so use this
   code for its own errors, to tell those apart (like runc and ssh do).  */
#घोषणा EXIT_EXEC_FAILURE 255

अटल काष्ठा crun_global_arguments arguments;

काष्ठा commands_s
{
  पूर्णांक मूल्य;
  स्थिर अक्षर *name;
  पूर्णांक (*handler) (काष्ठा crun_global_arguments *, पूर्णांक, अक्षर **, libcrun_error_t *);
};

पूर्णांक
init_libcrun_context (libcrun_context_t *con, स्थिर अक्षर *id, काष्ठा crun_global_arguments *glob, libcrun_error_t *err)
{
  पूर्णांक ret;

  libcrun_context_set_id (con, id);
  libcrun_context_set_state_मूल (con, glob->मूल);
  libcrun_context_set_systemd_cgroup (con, glob->option_systemd_cgroup);
  libcrun_context_set_force_no_cgroup (con, glob->option_force_no_cgroup);
  libcrun_context_set_notify_socket (con, दो_पर्या ("NOTIFY_SOCKET"));
  libcrun_context_set_args (con, glob->argc, glob->argv);

  /* Check if global handler is configured and pass it down to crun context */
  libcrun_context_set_handler (con, glob->handler);

  ret = libcrun_context_init_logging (con, glob->log, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (glob->log_format)
    {
      ret = libcrun_set_log_format (glob->log_format, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  libcrun_set_verbosity (glob->verbosity);
  libcrun_debug ("Using debug verbosity");

  वापस 0;
}

/* Allocate a public libcrun context (with handlers loaded) for a subcommand.
   Terminates the process on failure.  */
libcrun_context_t *
new_libcrun_context (काष्ठा crun_global_arguments *glob)
{
  libcrun_error_t err = शून्य;
  libcrun_context_t *con;

  con = libcrun_context_new (शून्य, glob->मूल, &err);
  अगर (UNLIKELY (con == शून्य))
    libcrun_fail_with_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));

  वापस con;
}

क्रमागत
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

काष्ठा commands_s commands[] = { { COMMAND_CREATE, "create", crun_command_create },
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
#अगर HAVE_CRIU && HAVE_DLOPEN
                                 { COMMAND_CHECKPOINT, "checkpoint", crun_command_checkpoint },
                                 { COMMAND_RESTORE, "restore", crun_command_restore },
#पूर्ण_अगर
                                 { COMMAND_MOUNTS, "mounts", crun_command_mounts },
                                 {
                                     0,
                                 } };

अटल अक्षर doc[] = "\nCOMMANDS:\n"
#अगर HAVE_CRIU && HAVE_DLOPEN
                    "\tcheckpoint  - checkpoint a container\n"
#पूर्ण_अगर
                    "\tcreate      - create a container\n"
                    "\tdelete      - remove definition for a container\n"
                    "\texec        - exec a command in a running container\n"
                    "\tfeatures    - show the enabled features\n"
                    "\tlist        - list known containers\n"
                    "\tmounts      - add or remove mounts from a running container\n"
                    "\tkill        - send a signal to the container init process\n"
                    "\tps          - show the processes in the container\n"
#अगर HAVE_CRIU && HAVE_DLOPEN
                    "\trestore     - restore a container\n"
#पूर्ण_अगर
                    "\trun         - run a container\n"
                    "\tspec        - generate a configuration file\n"
                    "\tstart       - start a container\n"
                    "\tstate       - output the state of a container\n"
                    "\tpause       - pause all the processes in the container\n"
                    "\tresume      - unpause the processes in the container\n"
                    "\tupdate      - update container resource constraints\n";

अटल अक्षर args_doc[] = "COMMAND [OPTION...]";

अटल काष्ठा commands_s *
get_command (स्थिर अक्षर *arg)
{
  काष्ठा commands_s *it;
  क्रम (it = commands; it->मूल्य; it++)
    अगर (म_भेद (it->name, arg) == 0)
      वापस it;
  वापस शून्य;
}

क्रमागत
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

स्थिर अक्षर *argp_program_bug_address = "https://github.com/containers/crun/issues";

अटल काष्ठा argp_option options[] = { { "debug", OPTION_DEBUG, 0, 0, "produce verbose output, similar to --log-level=debug", 0 },
                                        { "cgroup-manager", OPTION_CGROUP_MANAGER, "MANAGER", 0, "cgroup manager", 0 },
                                        { "systemd-cgroup", OPTION_SYSTEMD_CGROUP, 0, 0, "use systemd cgroups", 0 },
                                        { "log", OPTION_LOG, "FILE", 0, "log destination: '[file:]PATH', 'journald:ID' or 'syslog:ID' (defaults to stderr)", 0 },
                                        { "log-format", OPTION_LOG_FORMAT, "FORMAT", 0, "log format: 'text' (default) or 'json'", 0 },
                                        { "log-level", OPTION_LOG_LEVEL, "LEVEL", 0, "log level to use: 'error' (default), 'warning' or 'debug'", 0 },
                                        { "root", OPTION_मूल, "DIR", 0, शून्य, 0 },
                                        { "rootless", OPTION_ROOTLESS, "VALUE", 0, शून्य, 0 },
                                        { "version", OPTION_VERSION, 0, 0, शून्य, 0 },
                                        // alias OPTION_VERSION_CAP with OPTION_VERSION
                                        { शून्य, OPTION_VERSION_CAP, 0, OPTION_ALIAS, शून्य, 0 },
                                        {
                                            0,
                                        } };

अटल व्योम
print_version (खाता *stream, काष्ठा argp_state *state arg_unused)
{
  libcrun_error_t err = शून्य;
  cleanup_free अक्षर *rundir = शून्य;
  libcrun_context_t *ctx;
  अक्षर **tags;
  पूर्णांक ret;

  ख_लिखो (stream, "%s version %s\n", PACKAGE_NAME, PACKAGE_VERSION);
  ख_लिखो (stream, "commit: %s\n", GIT_VERSION);

  ret = libcrun_get_state_dir (&rundir, arguments.मूल, शून्य, &err);
  अगर (LIKELY (ret == 0))
    ख_लिखो (stream, "rundir: %s\n", rundir);
  अन्यथा
    libcrun_error_release (&err);

  ख_लिखो (stream, "spec: 1.0.0\n");

#अगर_घोषित HAVE_SYSTEMD
  ख_लिखो (stream, "+SYSTEMD ");
#पूर्ण_अगर
  ख_लिखो (stream, "+SELINUX ");
  ख_लिखो (stream, "+APPARMOR ");
#अगर_घोषित HAVE_CAP
  ख_लिखो (stream, "+CAP ");
#पूर्ण_अगर
#अगर_घोषित HAVE_SECCOMP
  ख_लिखो (stream, "+SECCOMP ");
#पूर्ण_अगर
#अगर_घोषित HAVE_EBPF
  ख_लिखो (stream, "+EBPF ");
#पूर्ण_अगर
#अगर HAVE_CRIU && HAVE_DLOPEN
  ख_लिखो (stream, "+CRIU ");
#पूर्ण_अगर

  ctx = libcrun_context_new (शून्य, arguments.मूल, &err);
  अगर (ctx)
    {
      tags = libcrun_context_get_handler_feature_tags (ctx, &err);
      अगर (tags)
        {
          क्रम (माप_प्रकार i = 0; tags[i]; i++)
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

अटल error_t
parse_opt (पूर्णांक कुंजी, अक्षर *arg, काष्ठा argp_state *state)
{
  स्थिर अक्षर *tmp;

  चयन (कुंजी)
    {
    हाल OPTION_DEBUG:
      arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
      अवरोध;

    हाल OPTION_CGROUP_MANAGER:
      tmp = argp_mandatory_argument (arg, state);
      अगर (म_भेद (tmp, "systemd") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = true;
        }
      अन्यथा अगर (म_भेद (tmp, "cgroupfs") == 0)
        {
          arguments.option_force_no_cgroup = false;
          arguments.option_systemd_cgroup = false;
        }
      अन्यथा अगर (म_भेद (tmp, "disabled") == 0)
        {
          arguments.option_systemd_cgroup = false;
          arguments.option_force_no_cgroup = true;
        }
      अन्यथा
        {
          libcrun_fail_with_error (0, "unknown cgroup manager specified");
        }
      अवरोध;

    हाल OPTION_SYSTEMD_CGROUP:
      arguments.option_force_no_cgroup = false;
      arguments.option_systemd_cgroup = true;
      अवरोध;

    हाल OPTION_LOG:
      arguments.log = argp_mandatory_argument (arg, state);
      अवरोध;

    हाल OPTION_LOG_FORMAT:
      arguments.log_format = argp_mandatory_argument (arg, state);
      अवरोध;

    हाल OPTION_LOG_LEVEL:
      tmp = argp_mandatory_argument (arg, state);
      अगर (म_भेद (tmp, "error") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_ERROR;
        }
      अन्यथा अगर (म_भेद (tmp, "warning") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_WARNING;
        }
      अन्यथा अगर (म_भेद (tmp, "debug") == 0)
        {
          arguments.verbosity = LIBCRUN_VERBOSITY_DEBUG;
        }
      अन्यथा
        {
          libcrun_fail_with_error (0, "unknown verbosity `%s` specified", arg);
        }
      अवरोध;

    हाल OPTION_मूल:
      arguments.मूल = argp_mandatory_argument (arg, state);
      अवरोध;

    हाल OPTION_ROOTLESS:
      /* Ignored.  So that a runc command line won't fail.  */
      अवरोध;

    हाल ARGP_कुंजी_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a command");

    हाल OPTION_VERSION:
    हाल OPTION_VERSION_CAP:
      print_version (मानक_निकास, state);
      निकास (निकास_सफल);
    शेष:
      वापस ARGP_ERR_UNKNOWN;
    }

  वापस 0;
}

अटल काष्ठा commands_s *command;

व्योम
crun_assert_n_args (पूर्णांक n, पूर्णांक min, पूर्णांक max)
{
  अगर (min >= 0 && n < min)
    libcrun_fail_with_error (0, "`%s` requires a minimum of %d arguments", command->name, min);
  अगर (max >= 0 && n > max)
    libcrun_fail_with_error (0, "`%s` requires a maximum of %d arguments", command->name, max);
}

अक्षर *
argp_mandatory_argument (अक्षर *arg, काष्ठा argp_state *state)
{
  अगर (arg)
    वापस arg;
  वापस state->argv[state->अग्र++];
}

/* Parse a numeric value from STR, aborting on an out-of-range or malformed
   value.  KIND names the field for error messages.  If ENDPTR is not NULL it
   is set past the parsed number (so the caller can continue parsing, e.g. a
   "UID:GID" pair); otherwise the whole string must be a valid number.  */
पूर्णांक
parse_id_or_fail (स्थिर अक्षर *str, अक्षर **endptr, स्थिर अक्षर *kind)
{
  अक्षर *end = शून्य;
  दीर्घ दीर्घ l;

  त्रुटि_सं = 0;
  l = strtoll (str, &end, 10);
  अगर (end == str)
    libcrun_fail_with_error (0, "invalid %s specified", kind);
  अगर (त्रुटि_सं == दुस्फल)
    libcrun_fail_with_error (0, "invalid %s specified", kind);
  अगर (l < पूर्णांक_न्यून || l > पूर्णांक_उच्च)
    libcrun_fail_with_error (0, "invalid %s specified", kind);

  अगर (endptr)
    *endptr = end;
  अन्यथा अगर (*end != '\0')
    libcrun_fail_with_error (0, "invalid %s specified", kind);

  वापस (पूर्णांक) l;
}

अटल काष्ठा argp argp = { options, parse_opt, args_doc, doc, शून्य, शून्य, शून्य };

पूर्णांक ensure_cloned_द्विआधारी (व्योम);

अटल व्योम
fill_handler_from_argv0 (अक्षर *argv0, काष्ठा crun_global_arguments *args)
{
  स्थिर अक्षर *b = basename (argv0);
#अगर_घोषित HAVE_LIBKRUN
  अगर (म_भेद (b, "krun") == 0)
    {
      args->handler = "krun";
      वापस;
    }
#पूर्ण_अगर
  अगर (म_भेदन (b, "crun-", 5) == 0 && b[5] != '\0')
    args->handler = b + 5;
}

अटल अक्षर **
copy_args (अक्षर **argv, पूर्णांक argc)
{
  अक्षर **buff = xmalloc0 ((argc + 1) * माप (अक्षर *));
  क्रम (पूर्णांक i = 0; i < argc; i++)
    buff[i] = argv[i];

  वापस buff;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  libcrun_error_t err = शून्य;
  पूर्णांक ret, first_argument = 0;

  arguments.argc = argc;
  arguments.argv = argv;

#अगर_घोषित DYNLOAD_LIBCRUN
  अगर (ensure_cloned_द्विआधारी () < 0)
    {
      ख_लिखो (मानक_त्रुटि, "Failed to re-execute libcrun via memory file descriptor\n");
      निकास (निकास_त्रुटि);
    }
  /* Resolve all libcrun weak dependencies.  */
  अगर (dlopen ("libcrun.so", RTLD_GLOBAL | RTLD_DEEPBIND | RTLD_LAZY) == शून्य)
    libcrun_fail_with_error (0, "could not load `libcrun.so`: `%s`", dlerror ());
#पूर्ण_अगर

  fill_handler_from_argv0 (argv[0], &arguments);
  argp_parse (&argp, argc, argv, ARGP_IN_ORDER, &first_argument, &arguments);

  command = get_command (argv[first_argument]);
  अगर (command == शून्य)
    libcrun_fail_with_error (0, "unknown command %s", argv[first_argument]);

  पूर्णांक command_argc = argc - first_argument;
  cleanup_free अक्षर **command_argv = copy_args (argv + first_argument, command_argc);
  command_argv[0] = argv[0];

  ret = command->handler (&arguments, command_argc, command_argv, &err);
  अगर (ret && err)
    {
      अगर (command->मूल्य == COMMAND_EXEC)
        {
          libcrun_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));
          निकास (EXIT_EXEC_FAILURE);
        }
      libcrun_fail_with_error (libcrun_error_get_status (err), "%s", libcrun_error_get_message (err));
    }

  वापस ret;
}
