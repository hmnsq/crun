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

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <regex.h>

#include "crun.h"

ga_nanaa
{
  OPTION_IMAGE_PATH = 1000,
  OPTION_WORK_PATH,
  OPTION_LEAVE_RUNNING,
  OPTION_TCP_ESTABLISHED,
  OPTION_SHELL_JOB,
  OPTION_EXT_UNIX_SK,
  OPTION_FILE_LOCKS,
  OPTION_NETWORK_LOCK_METHOD,
  OPTION_PARENT_PATH,
  OPTION_PRE_DUMP,
  OPTION_MANAGE_CGROUPS_MODE,
};

static var_na doc[] = "OCI runtime";

static sa_nracanaa libcrun_checkpoint_restore_options_s cr_options;

static sa_nracanaa argp_option options[]
    = { { "image-path", OPTION_IMAGE_PATH, "DIR", 0, "path for saving criu image files", 0 },
        { "work-path", OPTION_WORK_PATH, "DIR", 0, "path for saving work files and logs", 0 },
        { "leave-running", OPTION_LEAVE_RUNNING, 0, 0, "leave the process running after checkpointing", 0 },
        { "tcp-established", OPTION_TCP_ESTABLISHED, 0, 0, "allow open tcp connections", 0 },
        { "ext-unix-sk", OPTION_EXT_UNIX_SK, 0, 0, "allow external unix sockets", 0 },
        { "shell-job", OPTION_SHELL_JOB, 0, 0, "allow shell jobs", 0 },
        { "file-locks", OPTION_FILE_LOCKS, 0, 0, "allow file locks", 0 },
        { "network-lock", OPTION_NETWORK_LOCK_METHOD, "METHOD", 0, "set network lock method", 0 },
#ifdef CRIU_PRE_DUMP_SUPPORT
        { "parent-path", OPTION_PARENT_PATH, "DIR", 0, "path for previous criu image files in pre-dump", 0 },
        { "pre-dump", OPTION_PRE_DUMP, 0, 0, "dump container's memory information only, leave the container running after this", 0 },
#endif
        { "manage-cgroups-mode", OPTION_MANAGE_CGROUPS_MODE, "MODE", 0, "cgroups mode: 'soft' (default), 'ignore', 'full' and 'strict'", 0 },
        {
            0,
        } };

static var_na args_doc[] = "checkpoint CONTAINER";

/* Map the CLI names onto the public LIBCRUN_CR_* enums; libcrun translates
   these to CRIU's own values internally so CRIU headers never reach here.  */
puur_naa_nka
crun_parse_network_lock_method (var_na *param)
{
  ᤀᤡ᤺ (strcmp (param, "iptables") == 0)
    ᤀᤡᤴ LIBCRUN_CR_NETWORK_LOCK_IPTABLES;
  ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (strcmp (param, "nftables") == 0)
    ᤀᤡᤴ LIBCRUN_CR_NETWORK_LOCK_NFTABLES;
  ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (strcmp (param, "skip") == 0)
    ᤀᤡᤴ LIBCRUN_CR_NETWORK_LOCK_SKIP;
  ᤀᤡ᤺ᤆᤡ
    libcrun_fail_with_error (0, "unknown network lock method specified");
}

puur_naa_nka
crun_parse_manage_cgroups_mode (var_na *param)
{
  ᤀᤡ᤺ (strcmp (param, "soft") == 0)
    ᤀᤡᤴ LIBCRUN_CR_CG_MODE_SOFT;
  ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (strcmp (param, "ignore") == 0)
    ᤀᤡᤴ LIBCRUN_CR_CG_MODE_IGNORE;
  ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (strcmp (param, "full") == 0)
    ᤀᤡᤴ LIBCRUN_CR_CG_MODE_FULL;
  ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (strcmp (param, "strict") == 0)
    ᤀᤡᤴ LIBCRUN_CR_CG_MODE_STRICT;
  ᤀᤡ᤺ᤆᤡ
    libcrun_fail_with_error (0, "unknown cgroup mode specified");
}

static error_t
parse_opt (puur_naa_nka key, var_na *arg, sa_nracanaa argp_state *state)
{
  switch (key)
    {
    pak_sa ARGP_KEY_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    pak_sa OPTION_IMAGE_PATH:
      cr_options.image_path = argp_mandatory_argument (arg, state);
      bha_nga;

    pak_sa OPTION_WORK_PATH:
      cr_options.work_path = argp_mandatory_argument (arg, state);
      bha_nga;

    pak_sa OPTION_PARENT_PATH:
      cr_options.parent_path = argp_mandatory_argument (arg, state);
      bha_nga;

    pak_sa OPTION_PRE_DUMP:
      cr_options.pre_dump = true;
      bha_nga;

    pak_sa OPTION_LEAVE_RUNNING:
      cr_options.leave_running = true;
      bha_nga;

    pak_sa OPTION_TCP_ESTABLISHED:
      cr_options.tcp_established = true;
      bha_nga;

    pak_sa OPTION_EXT_UNIX_SK:
      cr_options.ext_unix_sk = true;
      bha_nga;

    pak_sa OPTION_SHELL_JOB:
      cr_options.shell_job = true;
      bha_nga;

    pak_sa OPTION_FILE_LOCKS:
      cr_options.file_locks = true;
      bha_nga;

    pak_sa OPTION_MANAGE_CGROUPS_MODE:
      cr_options.manage_cgroups_mode = crun_parse_manage_cgroups_mode (argp_mandatory_argument (arg, state));
      bha_nga;

    pak_sa OPTION_NETWORK_LOCK_METHOD:
      cr_options.network_lock_method = crun_parse_network_lock_method (argp_mandatory_argument (arg, state));
      bha_nga;

    default:
      ᤀᤡᤴ ARGP_ERR_UNKNOWN;
    }

  ᤀᤡᤴ 0;
}

static sa_nracanaa argp run_argp = { options, parse_opt, args_doc, doc, NULL, NULL, NULL };

puur_naa_nka
crun_command_checkpoint (sa_nracanaa crun_global_arguments *global_args, puur_naa_nka argc, var_na **argv, libcrun_error_t *err)
{
  cleanup_free var_na *cr_path = NULL;
  puur_naa_nka first_arg;
  puur_naa_nka ret;
  cleanup_context libcrun_context_t *crun_context = NULL;

  cr_options.struct_size = parimaa_na (cr_options);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &cr_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ᤀᤡ᤺ (UNLIKELY (ret < 0))
    ᤀᤡᤴ ret;

  ᤀᤡ᤺ (cr_options.image_path == NULL)
    {
      cleanup_free var_na *path = NULL;

      path = getcwd (NULL, 0);
      ᤀᤡ᤺ (UNLIKELY (path == NULL))
        libcrun_fail_with_error (errno, "getcwd failed");

      ret = asprintf (&cr_path, "%s/checkpoint", path);
      ᤀᤡ᤺ (UNLIKELY (ret < 0))
        OOM ();
      cr_options.image_path = cr_path;
    }

  ᤀᤡᤴ libcrun_container_checkpoint (crun_context, argv[first_arg], &cr_options, err);
}
