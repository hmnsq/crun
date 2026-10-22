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

ਗਿਣਤੀ
{
  OPTION_IMAGE_ਰਸਤਾ = 1000,
  OPTION_WORK_ਰਸਤਾ,
  OPTION_LEAVE_RUNNING,
  OPTION_TCP_ESTABLISHED,
  OPTION_SHELL_ਕੰਮ,
  OPTION_EXT_UNIX_SK,
  OPTION_FILE_LOCKS,
  OPTION_ਜਾਲ_ਤਾਲਾ_ਵਿਧੀ,
  OPTION_ਮਾਪਾ_ਰਸਤਾ,
  OPTION_PRE_DUMP,
  OPTION_MANAGE_CGROUPS_ਬਹੁਲਕ,
};

ਸਥਿਰ ਅੱਖਰ doc[] = "OCI runtime";

ਸਥਿਰ ਬਣਤਰ libcrun_checkpoint_restore_options_s cr_options;

ਸਥਿਰ ਬਣਤਰ argp_option options[]
    = { { "image-path", OPTION_IMAGE_ਰਸਤਾ, "DIR", 0, "path for saving criu image files", 0 },
        { "work-path", OPTION_WORK_ਰਸਤਾ, "DIR", 0, "path for saving work files and logs", 0 },
        { "leave-running", OPTION_LEAVE_RUNNING, 0, 0, "leave the process running after checkpointing", 0 },
        { "tcp-established", OPTION_TCP_ESTABLISHED, 0, 0, "allow open tcp connections", 0 },
        { "ext-unix-sk", OPTION_EXT_UNIX_SK, 0, 0, "allow external unix sockets", 0 },
        { "shell-job", OPTION_SHELL_ਕੰਮ, 0, 0, "allow shell jobs", 0 },
        { "file-locks", OPTION_FILE_LOCKS, 0, 0, "allow file locks", 0 },
        { "network-lock", OPTION_ਜਾਲ_ਤਾਲਾ_ਵਿਧੀ, "METHOD", 0, "set network lock method", 0 },
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CRIU_PRE_DUMP_SUPPORT
        { "parent-path", OPTION_ਮਾਪਾ_ਰਸਤਾ, "DIR", 0, "path for previous criu image files in pre-dump", 0 },
        { "pre-dump", OPTION_PRE_DUMP, 0, 0, "dump container's memory information only, leave the container running after this", 0 },
#ਜੇ_ਅੰਤ
        { "manage-cgroups-mode", OPTION_MANAGE_CGROUPS_ਬਹੁਲਕ, "MODE", 0, "cgroups mode: 'soft' (default), 'ignore', 'full' and 'strict'", 0 },
        {
            0,
        } };

ਸਥਿਰ ਅੱਖਰ ਦਲੀਲਾਂ_doc[] = "checkpoint CONTAINER";

/* Map the CLI names onto the public LIBCRUN_CR_* enums; libcrun translates
   these to CRIU's own values internally so CRIU headers never reach here.  */
ਪੂਰਨ
crun_parse_ਜਾਲ_ਤਾਲਾ_ਵਿਧੀ (ਅੱਖਰ *param)
{
  ਜੇ (strcmp (param, "iptables") == 0)
    ਵਾਪਸ LIBCRUN_CR_ਜਾਲ_ਤਾਲਾ_IPTABLES;
  ਨਹੀਂਤਾਂ ਜੇ (strcmp (param, "nftables") == 0)
    ਵਾਪਸ LIBCRUN_CR_ਜਾਲ_ਤਾਲਾ_NFTABLES;
  ਨਹੀਂਤਾਂ ਜੇ (strcmp (param, "skip") == 0)
    ਵਾਪਸ LIBCRUN_CR_ਜਾਲ_ਤਾਲਾ_SKIP;
  ਨਹੀਂਤਾਂ
    libcrun_fail_with_error (0, "unknown network lock method specified");
}

ਪੂਰਨ
crun_parse_manage_cgroups_ਬਹੁਲਕ (ਅੱਖਰ *param)
{
  ਜੇ (strcmp (param, "soft") == 0)
    ਵਾਪਸ LIBCRUN_CR_CG_ਬਹੁਲਕ_SOFT;
  ਨਹੀਂਤਾਂ ਜੇ (strcmp (param, "ignore") == 0)
    ਵਾਪਸ LIBCRUN_CR_CG_ਬਹੁਲਕ_IGNORE;
  ਨਹੀਂਤਾਂ ਜੇ (strcmp (param, "full") == 0)
    ਵਾਪਸ LIBCRUN_CR_CG_ਬਹੁਲਕ_ਭਰਿਆ;
  ਨਹੀਂਤਾਂ ਜੇ (strcmp (param, "strict") == 0)
    ਵਾਪਸ LIBCRUN_CR_CG_ਬਹੁਲਕ_STRICT;
  ਨਹੀਂਤਾਂ
    libcrun_fail_with_error (0, "unknown cgroup mode specified");
}

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

    ਹਾਲਤ OPTION_ਮਾਪਾ_ਰਸਤਾ:
      cr_options.ਮਾਪਾ_ਰਸਤਾ = argp_mandatory_ਦਲੀਲ (arg, state);
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_PRE_DUMP:
      cr_options.pre_dump = true;
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_LEAVE_RUNNING:
      cr_options.leave_running = true;
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_TCP_ESTABLISHED:
      cr_options.tcp_established = true;
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

    ਹਾਲਤ OPTION_MANAGE_CGROUPS_ਬਹੁਲਕ:
      cr_options.manage_cgroups_ਬਹੁਲਕ = crun_parse_manage_cgroups_ਬਹੁਲਕ (argp_mandatory_ਦਲੀਲ (arg, state));
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_ਜਾਲ_ਤਾਲਾ_ਵਿਧੀ:
      cr_options.ਜਾਲ_ਤਾਲਾ_ਵਿਧੀ = crun_parse_ਜਾਲ_ਤਾਲਾ_ਵਿਧੀ (argp_mandatory_ਦਲੀਲ (arg, state));
      ਰੋਕੋ;

    ਮੂਲ:
      ਵਾਪਸ ARGP_ERR_UNKNOWN;
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਣਤਰ argp ਲੜੀਵਾਰ_argp = { options, parse_opt, ਦਲੀਲਾਂ_doc, doc, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ };

ਪੂਰਨ
crun_command_checkpoint (ਬਣਤਰ crun_global_arguments *global_ਦਲੀਲਾਂ, ਪੂਰਨ argc, ਅੱਖਰ **argv, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *cr_ਰਸਤਾ = ਨਿਰਾ;
  ਪੂਰਨ ਪਹਿਲਾ_arg;
  ਪੂਰਨ ret;
  cleanup_context libcrun_context_t *crun_context = ਨਿਰਾ;

  cr_options.struct_ਮਾਪ = ਆਕਾਰ (cr_options);

  argp_parse (&ਲੜੀਵਾਰ_argp, argc, argv, ARGP_ਵਿੱਚ_ਕ੍ਰਮ, &ਪਹਿਲਾ_arg, &cr_options);
  crun_assert_n_ਦਲੀਲਾਂ (argc - ਪਹਿਲਾ_arg, 1, 2);

  crun_context = new_libcrun_context (global_ਦਲੀਲਾਂ);

  ret = init_libcrun_context (crun_context, argv[ਪਹਿਲਾ_arg], global_ਦਲੀਲਾਂ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

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

  ਵਾਪਸ libcrun_container_checkpoint (crun_context, argv[ਪਹਿਲਾ_arg], &cr_options, err);
}
