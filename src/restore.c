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

#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <argp.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <errno.h>
#అన్తర్భావ <regex.h>

#అన్తర్భావ "crun.h"
#అన్తర్భావ "checkpoint.h"

గణన
{
  OPTION_IMAGE_మార్గం = 1000,
  OPTION_WORK_మార్గం,
  OPTION_TCP_ESTABLISHED,
  OPTION_TCP_CLOSE,
  OPTION_SHELL_పని,
  OPTION_EXT_UNIX_SK,
  OPTION_PID_FILE,
  OPTION_CONSOLE_SOCKET,
  OPTION_FILE_LOCKS,
  OPTION_MANAGE_CGROUPS_బహుళకం,
  OPTION_జాలం_తాళం_పద్ధతి,
  OPTION_LSM_PROFILE,
  OPTION_LSM_MOUNT_CONTEXT,
  OPTION_NO_NEW_KEYRING,
};

స్థిర అక్షరం doc[] = "OCI runtime";

స్థిర స్థిరరాశి అక్షరం *bundle = ఖాళీ;

స్థిర libcrun_context_t *crun_context;

స్థిర నిర్మాణం libcrun_checkpoint_restore_options_s cr_options;

స్థిర నిర్మాణం argp_option options[]
    = { { "bundle", 'b', "DIR", 0, "container bundle (default \".\")", 0 },
        { "image-path", OPTION_IMAGE_మార్గం, "DIR", 0, "path for saving criu image files", 0 },
        { "work-path", OPTION_WORK_మార్గం, "DIR", 0, "path for saving work files and logs", 0 },
        { "tcp-established", OPTION_TCP_ESTABLISHED, 0, 0, "allow open tcp connections", 0 },
        { "tcp-close", OPTION_TCP_CLOSE, 0, 0, "allow closed tcp connections", 0 },
        { "ext-unix-sk", OPTION_EXT_UNIX_SK, 0, 0, "allow external unix sockets", 0 },
        { "shell-job", OPTION_SHELL_పని, 0, 0, "allow shell jobs", 0 },
        { "detach", 'd', 0, 0, "detach from the container's process", 0 },
        { "pid-file", OPTION_PID_FILE, "FILE", 0, "where to write the PID of the container", 0 },
        { "console-socket", OPTION_CONSOLE_SOCKET, "SOCKET", 0,
          "path to a socket that will receive the ptmx end of the tty", 0 },
        { "file-locks", OPTION_FILE_LOCKS, 0, 0, "allow file locks", 0 },
        { "manage-cgroups-mode", OPTION_MANAGE_CGROUPS_బహుళకం, "MODE", 0, "cgroups mode: 'soft' (default), 'ignore', 'full' and 'strict'", 0 },
        { "network-lock", OPTION_జాలం_తాళం_పద్ధతి, "METHOD", 0, "set network lock method", 0 },
        { "lsm-profile", OPTION_LSM_PROFILE, "VALUE", 0, "Specify an LSM profile to be used during restore in the form of TYPE:NAME", 0 },
        { "lsm-mount-context", OPTION_LSM_MOUNT_CONTEXT, "VALUE", 0, "Specify an LSM mount context to be used during restore", 0 },
        { "no-new-keyring", OPTION_NO_NEW_KEYRING, 0, 0, "keep the same session key", 0 },
        {
            0,
        } };

స్థిర అక్షరం వాదనలు_doc[] = "restore CONTAINER";

స్థిర error_t
parse_opt (పూర్ణాంకం కీలకం, అక్షరం *arg, నిర్మాణం argp_state *state)
{
  ఎంపిక (కీలకం)
    {
    స్థితి ARGP_కీలకం_NO_వాదనలు:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    స్థితి OPTION_IMAGE_మార్గం:
      cr_options.image_మార్గం = argp_mandatory_వాదం (arg, state);
      ఆపు;

    స్థితి OPTION_WORK_మార్గం:
      cr_options.work_మార్గం = argp_mandatory_వాదం (arg, state);
      ఆపు;

    స్థితి 'b':
      bundle = argp_mandatory_వాదం (arg, state);
      ఆపు;

    స్థితి OPTION_TCP_ESTABLISHED:
      cr_options.tcp_established = true;
      ఆపు;

    స్థితి OPTION_TCP_CLOSE:
      cr_options.tcp_close = true;
      ఆపు;

    స్థితి OPTION_EXT_UNIX_SK:
      cr_options.ext_unix_sk = true;
      ఆపు;

    స్థితి OPTION_SHELL_పని:
      cr_options.shell_పని = true;
      ఆపు;

    స్థితి OPTION_FILE_LOCKS:
      cr_options.file_locks = true;
      ఆపు;

    స్థితి 'd':
      cr_options.detach = true;
      ఆపు;

    స్థితి OPTION_CONSOLE_SOCKET:
      cr_options.console_socket = argp_mandatory_వాదం (arg, state);
      ఆపు;

    స్థితి OPTION_PID_FILE:
      libcrun_context_సమితి_pid_file (crun_context, argp_mandatory_వాదం (arg, state));
      ఆపు;

    స్థితి OPTION_MANAGE_CGROUPS_బహుళకం:
      cr_options.manage_cgroups_బహుళకం = crun_parse_manage_cgroups_బహుళకం (argp_mandatory_వాదం (arg, state));
      ఆపు;

    స్థితి OPTION_జాలం_తాళం_పద్ధతి:
      cr_options.జాలం_తాళం_పద్ధతి = crun_parse_జాలం_తాళం_పద్ధతి (argp_mandatory_వాదం (arg, state));
      ఆపు;

    స్థితి OPTION_LSM_PROFILE:
      cr_options.lsm_profile = argp_mandatory_వాదం (arg, state);
      ఆపు;

    స్థితి OPTION_LSM_MOUNT_CONTEXT:
      cr_options.lsm_mount_context = argp_mandatory_వాదం (arg, state);
      ఆపు;

    స్థితి OPTION_NO_NEW_KEYRING:
      libcrun_context_సమితి_no_new_keyring (crun_context, true);
      ఆపు;

    అప్రమేయ:
      తిరిగి ARGP_ERR_UNKNOWN;
    }

  తిరిగి 0;
}

స్థిర నిర్మాణం argp నిరంతరం_argp = { options, parse_opt, వాదనలు_doc, doc, ఖాళీ, ఖాళీ, ఖాళీ };

పూర్ణాంకం
crun_command_restore (నిర్మాణం crun_global_arguments *global_వాదనలు, పూర్ణాంకం argc, అక్షరం **argv, libcrun_error_t *err)
{
  cleanup_free అక్షరం *bundle_cleanup = ఖాళీ;
  cleanup_free అక్షరం *cr_మార్గం = ఖాళీ;
  పూర్ణాంకం మొదటి_arg;
  పూర్ణాంకం ret;

  crun_context = new_libcrun_context (global_వాదనలు);
  cr_options.struct_కొలత = పరిమాణం (cr_options);
  cr_options.manage_cgroups_బహుళకం = -1;

  argp_parse (&నిరంతరం_argp, argc, argv, ARGP_లోపల_క్రమం, &మొదటి_arg, &cr_options);
  crun_assert_n_వాదనలు (argc - మొదటి_arg, 1, 2);

  /* Make sure the bundle is an absolute path.  */

  ఒకవేళ (bundle == ఖాళీ)
    {
      bundle_cleanup = realpath (".", ఖాళీ);
      ఒకవేళ (bundle_cleanup == ఖాళీ)
        libcrun_fail_with_error (errno, "realpath `.` failed");
      bundle = bundle_cleanup;
    }
  లేకపోతే
    {
      ఒకవేళ (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, ఖాళీ);
          ఒకవేళ (bundle_cleanup == ఖాళీ)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      ఒకవేళ (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  ret = init_libcrun_context (crun_context, argv[మొదటి_arg], global_వాదనలు, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      libcrun_context_free (crun_context);
      తిరిగి ret;
    }

  ఒకవేళ (cr_options.image_మార్గం == ఖాళీ)
    {
      cleanup_free అక్షరం *మార్గం = ఖాళీ;

      మార్గం = getcwd (ఖాళీ, 0);
      ఒకవేళ (UNLIKELY (మార్గం == ఖాళీ))
        libcrun_fail_with_error (errno, "getcwd failed");

      ret = asprintf (&cr_మార్గం, "%s/checkpoint", మార్గం);
      ఒకవేళ (UNLIKELY (ret < 0))
        OOM ();
      cr_options.image_మార్గం = cr_మార్గం;
    }

  libcrun_context_సమితి_bundle (crun_context, bundle);
  ret = libcrun_container_restore (crun_context, argv[మొదటి_arg], &cr_options, err);
  libcrun_context_free (crun_context);
  తిరిగి ret;
}
