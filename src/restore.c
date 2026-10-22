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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <argp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <regex.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "crun.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "checkpoint.h"

ਗਿਣਤੀ
{
  OPTION_IMAGE_ਰਸਤਾ = 1000,
  OPTION_WORK_ਰਸਤਾ,
  OPTION_TCP_ESTABLISHED,
  OPTION_TCP_CLOSE,
  OPTION_SHELL_ਕੰਮ,
  OPTION_EXT_UNIX_SK,
  OPTION_PID_FILE,
  OPTION_CONSOLE_SOCKET,
  OPTION_FILE_LOCKS,
  OPTION_MANAGE_CGROUPS_ਬਹੁਲਕ,
  OPTION_ਜਾਲ_ਤਾਲਾ_ਵਿਧੀ,
  OPTION_LSM_PROFILE,
  OPTION_LSM_MOUNT_CONTEXT,
  OPTION_NO_NEW_KEYRING,
};

ਸਥਿਰ ਅੱਖਰ doc[] = "OCI runtime";

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *bundle = ਨਿਰਾ;

ਸਥਿਰ libcrun_context_t *crun_context;

ਸਥਿਰ ਬਣਤਰ libcrun_checkpoint_restore_options_s cr_options;

ਸਥਿਰ ਬਣਤਰ argp_option options[]
    = { { "bundle", 'b', "DIR", 0, "container bundle (default \".\")", 0 },
        { "image-path", OPTION_IMAGE_ਰਸਤਾ, "DIR", 0, "path for saving criu image files", 0 },
        { "work-path", OPTION_WORK_ਰਸਤਾ, "DIR", 0, "path for saving work files and logs", 0 },
        { "tcp-established", OPTION_TCP_ESTABLISHED, 0, 0, "allow open tcp connections", 0 },
        { "tcp-close", OPTION_TCP_CLOSE, 0, 0, "allow closed tcp connections", 0 },
        { "ext-unix-sk", OPTION_EXT_UNIX_SK, 0, 0, "allow external unix sockets", 0 },
        { "shell-job", OPTION_SHELL_ਕੰਮ, 0, 0, "allow shell jobs", 0 },
        { "detach", 'd', 0, 0, "detach from the container's process", 0 },
        { "pid-file", OPTION_PID_FILE, "FILE", 0, "where to write the PID of the container", 0 },
        { "console-socket", OPTION_CONSOLE_SOCKET, "SOCKET", 0,
          "path to a socket that will receive the ptmx end of the tty", 0 },
        { "file-locks", OPTION_FILE_LOCKS, 0, 0, "allow file locks", 0 },
        { "manage-cgroups-mode", OPTION_MANAGE_CGROUPS_ਬਹੁਲਕ, "MODE", 0, "cgroups mode: 'soft' (default), 'ignore', 'full' and 'strict'", 0 },
        { "network-lock", OPTION_ਜਾਲ_ਤਾਲਾ_ਵਿਧੀ, "METHOD", 0, "set network lock method", 0 },
        { "lsm-profile", OPTION_LSM_PROFILE, "VALUE", 0, "Specify an LSM profile to be used during restore in the form of TYPE:NAME", 0 },
        { "lsm-mount-context", OPTION_LSM_MOUNT_CONTEXT, "VALUE", 0, "Specify an LSM mount context to be used during restore", 0 },
        { "no-new-keyring", OPTION_NO_NEW_KEYRING, 0, 0, "keep the same session key", 0 },
        {
            0,
        } };

ਸਥਿਰ ਅੱਖਰ ਦਲੀਲਾਂ_doc[] = "restore CONTAINER";

ਸਥਿਰ error_t
parse_opt (ਪੂਰਨ ਕੁੰਜੀ, ਅੱਖਰ *arg, ਬਣਤਰ argp_state *state)
{
  ਚੋਣ (ਕੁੰਜੀ)
    {
    ਹਾਲਤ ARGP_ਕੁੰਜੀ_NO_ਦਲੀਲਾਂ:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ਹਾਲਤ OPTION_IMAGE_ਰਸਤਾ:
      cr_options.image_ਰਸਤਾ = argp_mandatory_ਦਲੀਲ (arg, state);
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_WORK_ਰਸਤਾ:
      cr_options.work_ਰਸਤਾ = argp_mandatory_ਦਲੀਲ (arg, state);
      ਰੋਕੋ;

    ਹਾਲਤ 'b':
      bundle = argp_mandatory_ਦਲੀਲ (arg, state);
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_TCP_ESTABLISHED:
      cr_options.tcp_established = true;
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_TCP_CLOSE:
      cr_options.tcp_close = true;
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_EXT_UNIX_SK:
      cr_options.ext_unix_sk = true;
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_SHELL_ਕੰਮ:
      cr_options.shell_ਕੰਮ = true;
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_FILE_LOCKS:
      cr_options.file_locks = true;
      ਰੋਕੋ;

    ਹਾਲਤ 'd':
      cr_options.detach = true;
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_CONSOLE_SOCKET:
      cr_options.console_socket = argp_mandatory_ਦਲੀਲ (arg, state);
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_PID_FILE:
      libcrun_context_ਸੈੱਟ_pid_file (crun_context, argp_mandatory_ਦਲੀਲ (arg, state));
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_MANAGE_CGROUPS_ਬਹੁਲਕ:
      cr_options.manage_cgroups_ਬਹੁਲਕ = crun_parse_manage_cgroups_ਬਹੁਲਕ (argp_mandatory_ਦਲੀਲ (arg, state));
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_ਜਾਲ_ਤਾਲਾ_ਵਿਧੀ:
      cr_options.ਜਾਲ_ਤਾਲਾ_ਵਿਧੀ = crun_parse_ਜਾਲ_ਤਾਲਾ_ਵਿਧੀ (argp_mandatory_ਦਲੀਲ (arg, state));
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_LSM_PROFILE:
      cr_options.lsm_profile = argp_mandatory_ਦਲੀਲ (arg, state);
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_LSM_MOUNT_CONTEXT:
      cr_options.lsm_mount_context = argp_mandatory_ਦਲੀਲ (arg, state);
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_NO_NEW_KEYRING:
      libcrun_context_ਸੈੱਟ_no_new_keyring (crun_context, true);
      ਰੋਕੋ;

    ਮੂਲ:
      ਵਾਪਸ ARGP_ERR_UNKNOWN;
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਣਤਰ argp ਲੜੀਵਾਰ_argp = { options, parse_opt, ਦਲੀਲਾਂ_doc, doc, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ };

ਪੂਰਨ
crun_command_restore (ਬਣਤਰ crun_global_arguments *global_ਦਲੀਲਾਂ, ਪੂਰਨ argc, ਅੱਖਰ **argv, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *bundle_cleanup = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *cr_ਰਸਤਾ = ਨਿਰਾ;
  ਪੂਰਨ ਪਹਿਲਾ_arg;
  ਪੂਰਨ ret;

  crun_context = new_libcrun_context (global_ਦਲੀਲਾਂ);
  cr_options.struct_ਮਾਪ = ਆਕਾਰ (cr_options);
  cr_options.manage_cgroups_ਬਹੁਲਕ = -1;

  argp_parse (&ਲੜੀਵਾਰ_argp, argc, argv, ARGP_ਵਿੱਚ_ਕ੍ਰਮ, &ਪਹਿਲਾ_arg, &cr_options);
  crun_assert_n_ਦਲੀਲਾਂ (argc - ਪਹਿਲਾ_arg, 1, 2);

  /* Make sure the bundle is an absolute path.  */

  ਜੇ (bundle == ਨਿਰਾ)
    {
      bundle_cleanup = realpath (".", ਨਿਰਾ);
      ਜੇ (bundle_cleanup == ਨਿਰਾ)
        libcrun_fail_with_error (errno, "realpath `.` failed");
      bundle = bundle_cleanup;
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, ਨਿਰਾ);
          ਜੇ (bundle_cleanup == ਨਿਰਾ)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      ਜੇ (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  ret = init_libcrun_context (crun_context, argv[ਪਹਿਲਾ_arg], global_ਦਲੀਲਾਂ, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      libcrun_context_free (crun_context);
      ਵਾਪਸ ret;
    }

  ਜੇ (cr_options.image_ਰਸਤਾ == ਨਿਰਾ)
    {
      cleanup_free ਅੱਖਰ *ਰਸਤਾ = ਨਿਰਾ;

      ਰਸਤਾ = getcwd (ਨਿਰਾ, 0);
      ਜੇ (UNLIKELY (ਰਸਤਾ == ਨਿਰਾ))
        libcrun_fail_with_error (errno, "getcwd failed");

      ret = asprintf (&cr_ਰਸਤਾ, "%s/checkpoint", ਰਸਤਾ);
      ਜੇ (UNLIKELY (ret < 0))
        OOM ();
      cr_options.image_ਰਸਤਾ = cr_ਰਸਤਾ;
    }

  libcrun_context_ਸੈੱਟ_bundle (crun_context, bundle);
  ret = libcrun_container_restore (crun_context, argv[ਪਹਿਲਾ_arg], &cr_options, err);
  libcrun_context_free (crun_context);
  ਵਾਪਸ ret;
}
