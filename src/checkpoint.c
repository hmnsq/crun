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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <argp.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <regex.h>

#അന്തര്ഭാവ "crun.h"

എണ്ണൽ
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

സ്ഥിതം അക്ഷരം doc[] = "OCI runtime";

സ്ഥിതം ഘടന libcrun_checkpoint_restore_options_s cr_options;

സ്ഥിതം ഘടന argp_option options[]
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

സ്ഥിതം അക്ഷരം args_doc[] = "checkpoint CONTAINER";

/* Map the CLI names onto the public LIBCRUN_CR_* enums; libcrun translates
   these to CRIU's own values internally so CRIU headers never reach here.  */
പൂർണ്ണസംഖ്യ
crun_parse_network_lock_method (അക്ഷരം *param)
{
  എങ്കിൽ (strcmp (param, "iptables") == 0)
    തിരികെ LIBCRUN_CR_NETWORK_LOCK_IPTABLES;
  അല്ലെങ്കിൽ എങ്കിൽ (strcmp (param, "nftables") == 0)
    തിരികെ LIBCRUN_CR_NETWORK_LOCK_NFTABLES;
  അല്ലെങ്കിൽ എങ്കിൽ (strcmp (param, "skip") == 0)
    തിരികെ LIBCRUN_CR_NETWORK_LOCK_SKIP;
  അല്ലെങ്കിൽ
    libcrun_fail_with_error (0, "unknown network lock method specified");
}

പൂർണ്ണസംഖ്യ
crun_parse_manage_cgroups_mode (അക്ഷരം *param)
{
  എങ്കിൽ (strcmp (param, "soft") == 0)
    തിരികെ LIBCRUN_CR_CG_MODE_SOFT;
  അല്ലെങ്കിൽ എങ്കിൽ (strcmp (param, "ignore") == 0)
    തിരികെ LIBCRUN_CR_CG_MODE_IGNORE;
  അല്ലെങ്കിൽ എങ്കിൽ (strcmp (param, "full") == 0)
    തിരികെ LIBCRUN_CR_CG_MODE_FULL;
  അല്ലെങ്കിൽ എങ്കിൽ (strcmp (param, "strict") == 0)
    തിരികെ LIBCRUN_CR_CG_MODE_STRICT;
  അല്ലെങ്കിൽ
    libcrun_fail_with_error (0, "unknown cgroup mode specified");
}

സ്ഥിതം error_t
parse_opt (പൂർണ്ണസംഖ്യ താക്കോൽ, അക്ഷരം *arg, ഘടന argp_state *state)
{
  തിരഞ്ഞെടുപ്പ് (താക്കോൽ)
    {
    അവസ്ഥ ARGP_താക്കോൽ_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    അവസ്ഥ OPTION_IMAGE_PATH:
      cr_options.image_path = argp_mandatory_argument (arg, state);
      നിർത്തുക;

    അവസ്ഥ OPTION_WORK_PATH:
      cr_options.work_path = argp_mandatory_argument (arg, state);
      നിർത്തുക;

    അവസ്ഥ OPTION_PARENT_PATH:
      cr_options.parent_path = argp_mandatory_argument (arg, state);
      നിർത്തുക;

    അവസ്ഥ OPTION_PRE_DUMP:
      cr_options.pre_dump = true;
      നിർത്തുക;

    അവസ്ഥ OPTION_LEAVE_RUNNING:
      cr_options.leave_running = true;
      നിർത്തുക;

    അവസ്ഥ OPTION_TCP_ESTABLISHED:
      cr_options.tcp_established = true;
      നിർത്തുക;

    അവസ്ഥ OPTION_EXT_UNIX_SK:
      cr_options.ext_unix_sk = true;
      നിർത്തുക;

    അവസ്ഥ OPTION_SHELL_JOB:
      cr_options.shell_job = true;
      നിർത്തുക;

    അവസ്ഥ OPTION_FILE_LOCKS:
      cr_options.file_locks = true;
      നിർത്തുക;

    അവസ്ഥ OPTION_MANAGE_CGROUPS_MODE:
      cr_options.manage_cgroups_mode = crun_parse_manage_cgroups_mode (argp_mandatory_argument (arg, state));
      നിർത്തുക;

    അവസ്ഥ OPTION_NETWORK_LOCK_METHOD:
      cr_options.network_lock_method = crun_parse_network_lock_method (argp_mandatory_argument (arg, state));
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
      തിരികെ ARGP_ERR_UNKNOWN;
    }

  തിരികെ 0;
}

സ്ഥിതം ഘടന argp run_argp = { options, parse_opt, args_doc, doc, ശൂന്യ, ശൂന്യ, ശൂന്യ };

പൂർണ്ണസംഖ്യ
crun_command_checkpoint (ഘടന crun_global_arguments *global_args, പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *cr_path = ശൂന്യ;
  പൂർണ്ണസംഖ്യ first_arg;
  പൂർണ്ണസംഖ്യ ret;
  cleanup_context libcrun_context_t *crun_context = ശൂന്യ;

  cr_options.struct_size = വലുപ്പം (cr_options);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &cr_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (cr_options.image_path == ശൂന്യ)
    {
      cleanup_free അക്ഷരം *path = ശൂന്യ;

      path = getcwd (ശൂന്യ, 0);
      എങ്കിൽ (UNLIKELY (path == ശൂന്യ))
        libcrun_fail_with_error (errno, "getcwd failed");

      ret = asprintf (&cr_path, "%s/checkpoint", path);
      എങ്കിൽ (UNLIKELY (ret < 0))
        OOM ();
      cr_options.image_path = cr_path;
    }

  തിരികെ libcrun_container_checkpoint (crun_context, argv[first_arg], &cr_options, err);
}
