/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2020 Adrian Reber <areber@redhat.com>
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

#define _GNU_SOURCE

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <argp.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <regex.h>

#অন্তর্ভাব "crun.h"
#অন্তর্ভাব "checkpoint.h"

গণনা
{
  OPTION_IMAGE_PATH = 1000,
  OPTION_WORK_PATH,
  OPTION_TCP_ESTABLISHED,
  OPTION_TCP_CLOSE,
  OPTION_SHELL_JOB,
  OPTION_EXT_UNIX_SK,
  OPTION_PID_FILE,
  OPTION_CONSOLE_SOCKET,
  OPTION_FILE_LOCKS,
  OPTION_MANAGE_CGROUPS_MODE,
  OPTION_NETWORK_LOCK_METHOD,
  OPTION_LSM_PROFILE,
  OPTION_LSM_MOUNT_CONTEXT,
  OPTION_NO_NEW_KEYRING,
};

স্থির অক্ষর doc[] = "OCI runtime";

স্থির ধ্রুবক অক্ষর *bundle = ফাঁকা;

স্থির libcrun_context_t *crun_context;

স্থির কাঠামো libcrun_checkpoint_restore_options_s cr_options;

স্থির কাঠামো argp_option options[]
    = { { "bundle", 'b', "DIR", 0, "container bundle (default \".\")", 0 },
        { "image-path", OPTION_IMAGE_PATH, "DIR", 0, "path for saving criu image files", 0 },
        { "work-path", OPTION_WORK_PATH, "DIR", 0, "path for saving work files and logs", 0 },
        { "tcp-established", OPTION_TCP_ESTABLISHED, 0, 0, "allow open tcp connections", 0 },
        { "tcp-close", OPTION_TCP_CLOSE, 0, 0, "allow closed tcp connections", 0 },
        { "ext-unix-sk", OPTION_EXT_UNIX_SK, 0, 0, "allow external unix sockets", 0 },
        { "shell-job", OPTION_SHELL_JOB, 0, 0, "allow shell jobs", 0 },
        { "detach", 'd', 0, 0, "detach from the container's process", 0 },
        { "pid-file", OPTION_PID_FILE, "FILE", 0, "where to write the PID of the container", 0 },
        { "console-socket", OPTION_CONSOLE_SOCKET, "SOCKET", 0,
          "path to a socket that will receive the ptmx end of the tty", 0 },
        { "file-locks", OPTION_FILE_LOCKS, 0, 0, "allow file locks", 0 },
        { "manage-cgroups-mode", OPTION_MANAGE_CGROUPS_MODE, "MODE", 0, "cgroups mode: 'soft' (default), 'ignore', 'full' and 'strict'", 0 },
        { "network-lock", OPTION_NETWORK_LOCK_METHOD, "METHOD", 0, "set network lock method", 0 },
        { "lsm-profile", OPTION_LSM_PROFILE, "VALUE", 0, "Specify an LSM profile to be used during restore in the form of TYPE:NAME", 0 },
        { "lsm-mount-context", OPTION_LSM_MOUNT_CONTEXT, "VALUE", 0, "Specify an LSM mount context to be used during restore", 0 },
        { "no-new-keyring", OPTION_NO_NEW_KEYRING, 0, 0, "keep the same session key", 0 },
        {
            0,
        } };

স্থির অক্ষর args_doc[] = "restore CONTAINER";

স্থির error_t
parse_opt (পূর্ণসংখ্যা চাবি, অক্ষর *arg, কাঠামো argp_state *state)
{
  নির্বাচন (চাবি)
    {
    ক্ষেত্র ARGP_চাবি_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ক্ষেত্র OPTION_IMAGE_PATH:
      cr_options.image_path = argp_mandatory_argument (arg, state);
      বিরতি;

    ক্ষেত্র OPTION_WORK_PATH:
      cr_options.work_path = argp_mandatory_argument (arg, state);
      বিরতি;

    ক্ষেত্র 'b':
      bundle = argp_mandatory_argument (arg, state);
      বিরতি;

    ক্ষেত্র OPTION_TCP_ESTABLISHED:
      cr_options.tcp_established = true;
      বিরতি;

    ক্ষেত্র OPTION_TCP_CLOSE:
      cr_options.tcp_close = true;
      বিরতি;

    ক্ষেত্র OPTION_EXT_UNIX_SK:
      cr_options.ext_unix_sk = true;
      বিরতি;

    ক্ষেত্র OPTION_SHELL_JOB:
      cr_options.shell_job = true;
      বিরতি;

    ক্ষেত্র OPTION_FILE_LOCKS:
      cr_options.file_locks = true;
      বিরতি;

    ক্ষেত্র 'd':
      cr_options.detach = true;
      বিরতি;

    ক্ষেত্র OPTION_CONSOLE_SOCKET:
      cr_options.console_socket = argp_mandatory_argument (arg, state);
      বিরতি;

    ক্ষেত্র OPTION_PID_FILE:
      libcrun_context_set_pid_file (crun_context, argp_mandatory_argument (arg, state));
      বিরতি;

    ক্ষেত্র OPTION_MANAGE_CGROUPS_MODE:
      cr_options.manage_cgroups_mode = crun_parse_manage_cgroups_mode (argp_mandatory_argument (arg, state));
      বিরতি;

    ক্ষেত্র OPTION_NETWORK_LOCK_METHOD:
      cr_options.network_lock_method = crun_parse_network_lock_method (argp_mandatory_argument (arg, state));
      বিরতি;

    ক্ষেত্র OPTION_LSM_PROFILE:
      cr_options.lsm_profile = argp_mandatory_argument (arg, state);
      বিরতি;

    ক্ষেত্র OPTION_LSM_MOUNT_CONTEXT:
      cr_options.lsm_mount_context = argp_mandatory_argument (arg, state);
      বিরতি;

    ক্ষেত্র OPTION_NO_NEW_KEYRING:
      libcrun_context_set_no_new_keyring (crun_context, true);
      বিরতি;

    পূর্বনির্ধারিত:
      ফেরত ARGP_ERR_UNKNOWN;
    }

  ফেরত 0;
}

স্থির কাঠামো argp run_argp = { options, parse_opt, args_doc, doc, ফাঁকা, ফাঁকা, ফাঁকা };

পূর্ণসংখ্যা
crun_command_restore (কাঠামো crun_global_arguments *global_args, পূর্ণসংখ্যা argc, অক্ষর **argv, libcrun_error_t *err)
{
  cleanup_free অক্ষর *bundle_cleanup = ফাঁকা;
  cleanup_free অক্ষর *cr_path = ফাঁকা;
  পূর্ণসংখ্যা first_arg;
  পূর্ণসংখ্যা ret;

  crun_context = new_libcrun_context (global_args);
  cr_options.struct_আকার = আকার (cr_options);
  cr_options.manage_cgroups_mode = -1;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &cr_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  /* Make sure the bundle is an absolute path.  */

  যদি (bundle == ফাঁকা)
    {
      bundle_cleanup = realpath (".", ফাঁকা);
      যদি (bundle_cleanup == ফাঁকা)
        libcrun_fail_with_error (errno, "realpath `.` failed");
      bundle = bundle_cleanup;
    }
  অন্যথা
    {
      যদি (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, ফাঁকা);
          যদি (bundle_cleanup == ফাঁকা)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      যদি (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  যদি (UNLIKELY (ret < 0))
    {
      libcrun_context_free (crun_context);
      ফেরত ret;
    }

  যদি (cr_options.image_path == ফাঁকা)
    {
      cleanup_free অক্ষর *path = ফাঁকা;

      path = getcwd (ফাঁকা, 0);
      যদি (UNLIKELY (path == ফাঁকা))
        libcrun_fail_with_error (errno, "getcwd failed");

      ret = asprintf (&cr_path, "%s/checkpoint", path);
      যদি (UNLIKELY (ret < 0))
        OOM ();
      cr_options.image_path = cr_path;
    }

  libcrun_context_set_bundle (crun_context, bundle);
  ret = libcrun_container_restore (crun_context, argv[first_arg], &cr_options, err);
  libcrun_context_free (crun_context);
  ফেরত ret;
}
