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

#تعریف_کرو GNU_ماخذ

#شامل <config.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <argp.h>
#شامل <string.h>
#شامل <unistd.h>
#شامل <errno.h>
#شامل <regex.h>

#شامل "crun.h"

شمار
{
  OPTION_IMAGE_راستہ = 1000,
  OPTION_WORK_راستہ,
  OPTION_LEAVE_RUNNING,
  OPTION_TCP_ESTABLISHED,
  OPTION_SHELL_کام,
  OPTION_EXT_UNIX_SK,
  OPTION_FILE_LOCKS,
  OPTION_جال_تالا_طریقہ,
  OPTION_والد_راستہ,
  OPTION_PRE_DUMP,
  OPTION_MANAGE_CGROUPS_منوال,
};

ساکن حرف doc[] = "OCI runtime";

ساکن ساخت libcrun_checkpoint_restore_options_s cr_options;

ساکن ساخت argp_option options[]
    = { { "image-path", OPTION_IMAGE_راستہ, "DIR", 0, "path for saving criu image files", 0 },
        { "work-path", OPTION_WORK_راستہ, "DIR", 0, "path for saving work files and logs", 0 },
        { "leave-running", OPTION_LEAVE_RUNNING, 0, 0, "leave the process running after checkpointing", 0 },
        { "tcp-established", OPTION_TCP_ESTABLISHED, 0, 0, "allow open tcp connections", 0 },
        { "ext-unix-sk", OPTION_EXT_UNIX_SK, 0, 0, "allow external unix sockets", 0 },
        { "shell-job", OPTION_SHELL_کام, 0, 0, "allow shell jobs", 0 },
        { "file-locks", OPTION_FILE_LOCKS, 0, 0, "allow file locks", 0 },
        { "network-lock", OPTION_جال_تالا_طریقہ, "METHOD", 0, "set network lock method", 0 },
#اگر_تعریف_شدہ CRIU_PRE_DUMP_SUPPORT
        { "parent-path", OPTION_والد_راستہ, "DIR", 0, "path for previous criu image files in pre-dump", 0 },
        { "pre-dump", OPTION_PRE_DUMP, 0, 0, "dump container's memory information only, leave the container running after this", 0 },
#اگر_ختم
        { "manage-cgroups-mode", OPTION_MANAGE_CGROUPS_منوال, "MODE", 0, "cgroups mode: 'soft' (default), 'ignore', 'full' and 'strict'", 0 },
        {
            0,
        } };

ساکن حرف دلائل_doc[] = "checkpoint CONTAINER";

/* Map the CLI names onto the public LIBCRUN_CR_* enums; libcrun translates
   these to CRIU's own values internally so CRIU headers never reach here.  */
صحیح
crun_parse_جال_تالا_طریقہ (حرف *param)
{
  اگر (strcmp (param, "iptables") == 0)
    واپس LIBCRUN_CR_جال_تالا_IPTABLES;
  ورنہ اگر (strcmp (param, "nftables") == 0)
    واپس LIBCRUN_CR_جال_تالا_NFTABLES;
  ورنہ اگر (strcmp (param, "skip") == 0)
    واپس LIBCRUN_CR_جال_تالا_SKIP;
  ورنہ
    libcrun_fail_with_error (0, "unknown network lock method specified");
}

صحیح
crun_parse_manage_cgroups_منوال (حرف *param)
{
  اگر (strcmp (param, "soft") == 0)
    واپس LIBCRUN_CR_CG_منوال_SOFT;
  ورنہ اگر (strcmp (param, "ignore") == 0)
    واپس LIBCRUN_CR_CG_منوال_IGNORE;
  ورنہ اگر (strcmp (param, "full") == 0)
    واپس LIBCRUN_CR_CG_منوال_بھرا;
  ورنہ اگر (strcmp (param, "strict") == 0)
    واپس LIBCRUN_CR_CG_منوال_STRICT;
  ورنہ
    libcrun_fail_with_error (0, "unknown cgroup mode specified");
}

ساکن error_t
parse_opt (صحیح کلید, حرف *arg, ساخت argp_state *state)
{
  انتخاب (کلید)
    {
    حالت ARGP_کلید_NO_دلائل:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    حالت OPTION_IMAGE_راستہ:
      cr_options.image_راستہ = argp_mandatory_دلیل (arg, state);
      توڑو;

    حالت OPTION_WORK_راستہ:
      cr_options.work_راستہ = argp_mandatory_دلیل (arg, state);
      توڑو;

    حالت OPTION_والد_راستہ:
      cr_options.والد_راستہ = argp_mandatory_دلیل (arg, state);
      توڑو;

    حالت OPTION_PRE_DUMP:
      cr_options.pre_dump = true;
      توڑو;

    حالت OPTION_LEAVE_RUNNING:
      cr_options.leave_running = true;
      توڑو;

    حالت OPTION_TCP_ESTABLISHED:
      cr_options.tcp_established = true;
      توڑو;

    حالت OPTION_EXT_UNIX_SK:
      cr_options.ext_unix_sk = true;
      توڑو;

    حالت OPTION_SHELL_کام:
      cr_options.shell_کام = true;
      توڑو;

    حالت OPTION_FILE_LOCKS:
      cr_options.file_locks = true;
      توڑو;

    حالت OPTION_MANAGE_CGROUPS_منوال:
      cr_options.manage_cgroups_منوال = crun_parse_manage_cgroups_منوال (argp_mandatory_دلیل (arg, state));
      توڑو;

    حالت OPTION_جال_تالا_طریقہ:
      cr_options.جال_تالا_طریقہ = crun_parse_جال_تالا_طریقہ (argp_mandatory_دلیل (arg, state));
      توڑو;

    طےشدہ:
      واپس ARGP_ERR_UNKNOWN;
    }

  واپس 0;
}

ساکن ساخت argp تسلسل_argp = { options, parse_opt, دلائل_doc, doc, تہی, تہی, تہی };

صحیح
crun_command_checkpoint (ساخت crun_global_arguments *global_دلائل, صحیح argc, حرف **argv, libcrun_error_t *err)
{
  cleanup_free حرف *cr_راستہ = تہی;
  صحیح پہلا_arg;
  صحیح ret;
  cleanup_context libcrun_context_t *crun_context = تہی;

  cr_options.struct_سائز = حجم (cr_options);

  argp_parse (&تسلسل_argp, argc, argv, ARGP_اندر_ترتیب, &پہلا_arg, &cr_options);
  crun_assert_n_دلائل (argc - پہلا_arg, 1, 2);

  crun_context = new_libcrun_context (global_دلائل);

  ret = init_libcrun_context (crun_context, argv[پہلا_arg], global_دلائل, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (cr_options.image_راستہ == تہی)
    {
      cleanup_free حرف *راستہ = تہی;

      راستہ = getcwd (تہی, 0);
      اگر (UNLIKELY (راستہ == تہی))
        libcrun_fail_with_error (errno, "getcwd failed");

      ret = asprintf (&cr_راستہ, "%s/checkpoint", راستہ);
      اگر (UNLIKELY (ret < 0))
        OOM ();
      cr_options.image_راستہ = cr_راستہ;
    }

  واپس libcrun_container_checkpoint (crun_context, argv[پہلا_arg], &cr_options, err);
}
