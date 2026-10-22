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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <argp.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <regex.h>

#અન્તર્ભાવ "crun.h"

ગણના
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

સ્થિર અક્ષર doc[] = "OCI runtime";

સ્થિર સંરચના libcrun_checkpoint_restore_options_s cr_options;

સ્થિર સંરચના argp_option options[]
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

સ્થિર અક્ષર args_doc[] = "checkpoint CONTAINER";

/* Map the CLI names onto the public LIBCRUN_CR_* enums; libcrun translates
   these to CRIU's own values internally so CRIU headers never reach here.  */
પૂર્ણાંક
crun_parse_network_lock_method (અક્ષર *param)
{
  જો (strcmp (param, "iptables") == 0)
    પાછા LIBCRUN_CR_NETWORK_LOCK_IPTABLES;
  નહીંતર જો (strcmp (param, "nftables") == 0)
    પાછા LIBCRUN_CR_NETWORK_LOCK_NFTABLES;
  નહીંતર જો (strcmp (param, "skip") == 0)
    પાછા LIBCRUN_CR_NETWORK_LOCK_SKIP;
  નહીંતર
    libcrun_fail_with_error (0, "unknown network lock method specified");
}

પૂર્ણાંક
crun_parse_manage_cgroups_mode (અક્ષર *param)
{
  જો (strcmp (param, "soft") == 0)
    પાછા LIBCRUN_CR_CG_MODE_SOFT;
  નહીંતર જો (strcmp (param, "ignore") == 0)
    પાછા LIBCRUN_CR_CG_MODE_IGNORE;
  નહીંતર જો (strcmp (param, "full") == 0)
    પાછા LIBCRUN_CR_CG_MODE_FULL;
  નહીંતર જો (strcmp (param, "strict") == 0)
    પાછા LIBCRUN_CR_CG_MODE_STRICT;
  નહીંતર
    libcrun_fail_with_error (0, "unknown cgroup mode specified");
}

સ્થિર error_t
parse_opt (પૂર્ણાંક ચાવી, અક્ષર *arg, સંરચના argp_state *state)
{
  પસંદગી (ચાવી)
    {
    સ્થિતિ ARGP_ચાવી_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    સ્થિતિ OPTION_IMAGE_PATH:
      cr_options.image_path = argp_mandatory_argument (arg, state);
      તોડો;

    સ્થિતિ OPTION_WORK_PATH:
      cr_options.work_path = argp_mandatory_argument (arg, state);
      તોડો;

    સ્થિતિ OPTION_PARENT_PATH:
      cr_options.parent_path = argp_mandatory_argument (arg, state);
      તોડો;

    સ્થિતિ OPTION_PRE_DUMP:
      cr_options.pre_dump = true;
      તોડો;

    સ્થિતિ OPTION_LEAVE_RUNNING:
      cr_options.leave_running = true;
      તોડો;

    સ્થિતિ OPTION_TCP_ESTABLISHED:
      cr_options.tcp_established = true;
      તોડો;

    સ્થિતિ OPTION_EXT_UNIX_SK:
      cr_options.ext_unix_sk = true;
      તોડો;

    સ્થિતિ OPTION_SHELL_JOB:
      cr_options.shell_job = true;
      તોડો;

    સ્થિતિ OPTION_FILE_LOCKS:
      cr_options.file_locks = true;
      તોડો;

    સ્થિતિ OPTION_MANAGE_CGROUPS_MODE:
      cr_options.manage_cgroups_mode = crun_parse_manage_cgroups_mode (argp_mandatory_argument (arg, state));
      તોડો;

    સ્થિતિ OPTION_NETWORK_LOCK_METHOD:
      cr_options.network_lock_method = crun_parse_network_lock_method (argp_mandatory_argument (arg, state));
      તોડો;

    મૂળભૂત:
      પાછા ARGP_ERR_UNKNOWN;
    }

  પાછા 0;
}

સ્થિર સંરચના argp run_argp = { options, parse_opt, args_doc, doc, ખાલી, ખાલી, ખાલી };

પૂર્ણાંક
crun_command_checkpoint (સંરચના crun_global_arguments *global_args, પૂર્ણાંક argc, અક્ષર **argv, libcrun_error_t *err)
{
  cleanup_free અક્ષર *cr_path = ખાલી;
  પૂર્ણાંક first_arg;
  પૂર્ણાંક ret;
  cleanup_context libcrun_context_t *crun_context = ખાલી;

  cr_options.struct_size = કદ (cr_options);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &cr_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (cr_options.image_path == ખાલી)
    {
      cleanup_free અક્ષર *path = ખાલી;

      path = getcwd (ખાલી, 0);
      જો (UNLIKELY (path == ખાલી))
        libcrun_fail_with_error (errno, "getcwd failed");

      ret = asprintf (&cr_path, "%s/checkpoint", path);
      જો (UNLIKELY (ret < 0))
        OOM ();
      cr_options.image_path = cr_path;
    }

  પાછા libcrun_container_checkpoint (crun_context, argv[first_arg], &cr_options, err);
}
