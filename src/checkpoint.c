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

#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <argp.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <regex.h>

#ಅನ್ತರ್ಭಾವ "crun.h"

ಎಣಿಕೆ
{
  OPTION_IMAGE_ಮಾರ್ಗ = 1000,
  OPTION_WORK_ಮಾರ್ಗ,
  OPTION_LEAVE_RUNNING,
  OPTION_TCP_ESTABLISHED,
  OPTION_SHELL_ಕೆಲಸ,
  OPTION_EXT_UNIX_SK,
  OPTION_FILE_LOCKS,
  OPTION_ಜಾಲ_ಬೀಗ_ವಿಧಾನ,
  OPTION_ಪೋಷಕ_ಮಾರ್ಗ,
  OPTION_PRE_DUMP,
  OPTION_MANAGE_CGROUPS_ಬಹುಲಕ,
};

ಸ್ಥಿರ ಅಕ್ಷರ doc[] = "OCI runtime";

ಸ್ಥಿರ ರಚನೆ libcrun_checkpoint_restore_options_s cr_options;

ಸ್ಥಿರ ರಚನೆ argp_option options[]
    = { { "image-path", OPTION_IMAGE_ಮಾರ್ಗ, "DIR", 0, "path for saving criu image files", 0 },
        { "work-path", OPTION_WORK_ಮಾರ್ಗ, "DIR", 0, "path for saving work files and logs", 0 },
        { "leave-running", OPTION_LEAVE_RUNNING, 0, 0, "leave the process running after checkpointing", 0 },
        { "tcp-established", OPTION_TCP_ESTABLISHED, 0, 0, "allow open tcp connections", 0 },
        { "ext-unix-sk", OPTION_EXT_UNIX_SK, 0, 0, "allow external unix sockets", 0 },
        { "shell-job", OPTION_SHELL_ಕೆಲಸ, 0, 0, "allow shell jobs", 0 },
        { "file-locks", OPTION_FILE_LOCKS, 0, 0, "allow file locks", 0 },
        { "network-lock", OPTION_ಜಾಲ_ಬೀಗ_ವಿಧಾನ, "METHOD", 0, "set network lock method", 0 },
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CRIU_PRE_DUMP_SUPPORT
        { "parent-path", OPTION_ಪೋಷಕ_ಮಾರ್ಗ, "DIR", 0, "path for previous criu image files in pre-dump", 0 },
        { "pre-dump", OPTION_PRE_DUMP, 0, 0, "dump container's memory information only, leave the container running after this", 0 },
#ಆದರೆ_ಅಂತ್ಯ
        { "manage-cgroups-mode", OPTION_MANAGE_CGROUPS_ಬಹುಲಕ, "MODE", 0, "cgroups mode: 'soft' (default), 'ignore', 'full' and 'strict'", 0 },
        {
            0,
        } };

ಸ್ಥಿರ ಅಕ್ಷರ ವಾದಗಳು_doc[] = "checkpoint CONTAINER";

/* Map the CLI names onto the public LIBCRUN_CR_* enums; libcrun translates
   these to CRIU's own values internally so CRIU headers never reach here.  */
ಪೂರ್ಣಾಂಕ
crun_parse_ಜಾಲ_ಬೀಗ_ವಿಧಾನ (ಅಕ್ಷರ *param)
{
  ಆದರೆ (strcmp (param, "iptables") == 0)
    ಹಿಂತಿರುಗಿ LIBCRUN_CR_ಜಾಲ_ಬೀಗ_IPTABLES;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (param, "nftables") == 0)
    ಹಿಂತಿರುಗಿ LIBCRUN_CR_ಜಾಲ_ಬೀಗ_NFTABLES;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (param, "skip") == 0)
    ಹಿಂತಿರುಗಿ LIBCRUN_CR_ಜಾಲ_ಬೀಗ_SKIP;
  ಇಲ್ಲದಿದ್ದರೆ
    libcrun_fail_with_error (0, "unknown network lock method specified");
}

ಪೂರ್ಣಾಂಕ
crun_parse_manage_cgroups_ಬಹುಲಕ (ಅಕ್ಷರ *param)
{
  ಆದರೆ (strcmp (param, "soft") == 0)
    ಹಿಂತಿರುಗಿ LIBCRUN_CR_CG_ಬಹುಲಕ_SOFT;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (param, "ignore") == 0)
    ಹಿಂತಿರುಗಿ LIBCRUN_CR_CG_ಬಹುಲಕ_IGNORE;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (param, "full") == 0)
    ಹಿಂತಿರುಗಿ LIBCRUN_CR_CG_ಬಹುಲಕ_ತುಂಬಿದ;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (param, "strict") == 0)
    ಹಿಂತಿರುಗಿ LIBCRUN_CR_CG_ಬಹುಲಕ_STRICT;
  ಇಲ್ಲದಿದ್ದರೆ
    libcrun_fail_with_error (0, "unknown cgroup mode specified");
}

ಸ್ಥಿರ error_t
parse_opt (ಪೂರ್ಣಾಂಕ ಕೀಲಿ, ಅಕ್ಷರ *arg, ರಚನೆ argp_state *state)
{
  ಆಯ್ಕೆ (ಕೀಲಿ)
    {
    ಸ್ಥಿತಿ ARGP_ಕೀಲಿ_NO_ವಾದಗಳು:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ಸ್ಥಿತಿ OPTION_IMAGE_ಮಾರ್ಗ:
      cr_options.image_ಮಾರ್ಗ = argp_mandatory_ವಾದ (arg, state);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_WORK_ಮಾರ್ಗ:
      cr_options.work_ಮಾರ್ಗ = argp_mandatory_ವಾದ (arg, state);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_ಪೋಷಕ_ಮಾರ್ಗ:
      cr_options.ಪೋಷಕ_ಮಾರ್ಗ = argp_mandatory_ವಾದ (arg, state);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_PRE_DUMP:
      cr_options.pre_dump = true;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_LEAVE_RUNNING:
      cr_options.leave_running = true;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_TCP_ESTABLISHED:
      cr_options.tcp_established = true;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_EXT_UNIX_SK:
      cr_options.ext_unix_sk = true;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_SHELL_ಕೆಲಸ:
      cr_options.shell_ಕೆಲಸ = true;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_FILE_LOCKS:
      cr_options.file_locks = true;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_MANAGE_CGROUPS_ಬಹುಲಕ:
      cr_options.manage_cgroups_ಬಹುಲಕ = crun_parse_manage_cgroups_ಬಹುಲಕ (argp_mandatory_ವಾದ (arg, state));
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_ಜಾಲ_ಬೀಗ_ವಿಧಾನ:
      cr_options.ಜಾಲ_ಬೀಗ_ವಿಧಾನ = crun_parse_ಜಾಲ_ಬೀಗ_ವಿಧಾನ (argp_mandatory_ವಾದ (arg, state));
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ ARGP_ERR_UNKNOWN;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ರಚನೆ argp ಸತತ_argp = { options, parse_opt, ವಾದಗಳು_doc, doc, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ };

ಪೂರ್ಣಾಂಕ
crun_command_checkpoint (ರಚನೆ crun_global_arguments *global_ವಾದಗಳು, ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *cr_ಮಾರ್ಗ = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ಮೊದಲ_arg;
  ಪೂರ್ಣಾಂಕ ret;
  cleanup_context libcrun_context_t *crun_context = ಖಾಲಿ;

  cr_options.struct_ಗಾತ್ರ = ಗಾತ್ರ (cr_options);

  argp_parse (&ಸತತ_argp, argc, argv, ARGP_ಒಳ_ಕ್ರಮ, &ಮೊದಲ_arg, &cr_options);
  crun_assert_n_ವಾದಗಳು (argc - ಮೊದಲ_arg, 1, 2);

  crun_context = new_libcrun_context (global_ವಾದಗಳು);

  ret = init_libcrun_context (crun_context, argv[ಮೊದಲ_arg], global_ವಾದಗಳು, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (cr_options.image_ಮಾರ್ಗ == ಖಾಲಿ)
    {
      cleanup_free ಅಕ್ಷರ *ಮಾರ್ಗ = ಖಾಲಿ;

      ಮಾರ್ಗ = getcwd (ಖಾಲಿ, 0);
      ಆದರೆ (UNLIKELY (ಮಾರ್ಗ == ಖಾಲಿ))
        libcrun_fail_with_error (errno, "getcwd failed");

      ret = asprintf (&cr_ಮಾರ್ಗ, "%s/checkpoint", ಮಾರ್ಗ);
      ಆದರೆ (UNLIKELY (ret < 0))
        OOM ();
      cr_options.image_ಮಾರ್ಗ = cr_ಮಾರ್ಗ;
    }

  ಹಿಂತಿರುಗಿ libcrun_container_checkpoint (crun_context, argv[ಮೊದಲ_arg], &cr_options, err);
}
