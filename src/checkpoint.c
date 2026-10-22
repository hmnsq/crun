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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <argp.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <regex.h>

#அந்தர்பாவ "crun.h"

எண்ணிக்கை
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

நிலையான எழுத்து doc[] = "OCI runtime";

நிலையான கட்டமைப்பு libcrun_checkpoint_restore_options_s cr_options;

நிலையான கட்டமைப்பு argp_option options[]
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

நிலையான எழுத்து args_doc[] = "checkpoint CONTAINER";

/* Map the CLI names onto the public LIBCRUN_CR_* enums; libcrun translates
   these to CRIU's own values internally so CRIU headers never reach here.  */
முழு
crun_parse_network_lock_method (எழுத்து *param)
{
  எனில் (strcmp (param, "iptables") == 0)
    திரும்பு LIBCRUN_CR_NETWORK_LOCK_IPTABLES;
  இல்லையெனில் எனில் (strcmp (param, "nftables") == 0)
    திரும்பு LIBCRUN_CR_NETWORK_LOCK_NFTABLES;
  இல்லையெனில் எனில் (strcmp (param, "skip") == 0)
    திரும்பு LIBCRUN_CR_NETWORK_LOCK_SKIP;
  இல்லையெனில்
    libcrun_fail_with_error (0, "unknown network lock method specified");
}

முழு
crun_parse_manage_cgroups_mode (எழுத்து *param)
{
  எனில் (strcmp (param, "soft") == 0)
    திரும்பு LIBCRUN_CR_CG_MODE_SOFT;
  இல்லையெனில் எனில் (strcmp (param, "ignore") == 0)
    திரும்பு LIBCRUN_CR_CG_MODE_IGNORE;
  இல்லையெனில் எனில் (strcmp (param, "full") == 0)
    திரும்பு LIBCRUN_CR_CG_MODE_FULL;
  இல்லையெனில் எனில் (strcmp (param, "strict") == 0)
    திரும்பு LIBCRUN_CR_CG_MODE_STRICT;
  இல்லையெனில்
    libcrun_fail_with_error (0, "unknown cgroup mode specified");
}

நிலையான error_t
parse_opt (முழு சாவி, எழுத்து *arg, கட்டமைப்பு argp_state *state)
{
  தேர்வு (சாவி)
    {
    நிலை ARGP_சாவி_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    நிலை OPTION_IMAGE_PATH:
      cr_options.image_path = argp_mandatory_argument (arg, state);
      நிறுத்து;

    நிலை OPTION_WORK_PATH:
      cr_options.work_path = argp_mandatory_argument (arg, state);
      நிறுத்து;

    நிலை OPTION_PARENT_PATH:
      cr_options.parent_path = argp_mandatory_argument (arg, state);
      நிறுத்து;

    நிலை OPTION_PRE_DUMP:
      cr_options.pre_dump = true;
      நிறுத்து;

    நிலை OPTION_LEAVE_RUNNING:
      cr_options.leave_running = true;
      நிறுத்து;

    நிலை OPTION_TCP_ESTABLISHED:
      cr_options.tcp_established = true;
      நிறுத்து;

    நிலை OPTION_EXT_UNIX_SK:
      cr_options.ext_unix_sk = true;
      நிறுத்து;

    நிலை OPTION_SHELL_JOB:
      cr_options.shell_job = true;
      நிறுத்து;

    நிலை OPTION_FILE_LOCKS:
      cr_options.file_locks = true;
      நிறுத்து;

    நிலை OPTION_MANAGE_CGROUPS_MODE:
      cr_options.manage_cgroups_mode = crun_parse_manage_cgroups_mode (argp_mandatory_argument (arg, state));
      நிறுத்து;

    நிலை OPTION_NETWORK_LOCK_METHOD:
      cr_options.network_lock_method = crun_parse_network_lock_method (argp_mandatory_argument (arg, state));
      நிறுத்து;

    இயல்பு:
      திரும்பு ARGP_ERR_UNKNOWN;
    }

  திரும்பு 0;
}

நிலையான கட்டமைப்பு argp run_argp = { options, parse_opt, args_doc, doc, வெற்று, வெற்று, வெற்று };

முழு
crun_command_checkpoint (கட்டமைப்பு crun_global_arguments *global_args, முழு argc, எழுத்து **argv, libcrun_error_t *err)
{
  cleanup_free எழுத்து *cr_path = வெற்று;
  முழு first_arg;
  முழு ret;
  cleanup_context libcrun_context_t *crun_context = வெற்று;

  cr_options.struct_size = அளவு (cr_options);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &cr_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (cr_options.image_path == வெற்று)
    {
      cleanup_free எழுத்து *path = வெற்று;

      path = getcwd (வெற்று, 0);
      எனில் (UNLIKELY (path == வெற்று))
        libcrun_fail_with_error (errno, "getcwd failed");

      ret = asprintf (&cr_path, "%s/checkpoint", path);
      எனில் (UNLIKELY (ret < 0))
        OOM ();
      cr_options.image_path = cr_path;
    }

  திரும்பு libcrun_container_checkpoint (crun_context, argv[first_arg], &cr_options, err);
}
