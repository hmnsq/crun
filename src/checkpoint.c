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

#تعریف_کړه GNU_سرچینه

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <argp.h>
#ضمن <string.h>
#ضمن <unistd.h>
#ضمن <errno.h>
#ضمن <regex.h>

#ضمن "crun.h"

تعداد
{
  OPTION_IMAGE_لار = 1000,
  OPTION_WORK_لار,
  OPTION_LEAVE_RUNNING,
  OPTION_TCP_ESTABLISHED,
  OPTION_SHELL_دنده,
  OPTION_EXT_UNIX_SK,
  OPTION_FILE_LOCKS,
  OPTION_شبکه_قلف_لاره,
  OPTION_پلار_لار,
  OPTION_PRE_DUMP,
  OPTION_MANAGE_CGROUPS_ډېرپېښ,
};

ساكن حرف doc[] = "OCI runtime";

ساكن بنية libcrun_checkpoint_restore_options_s cr_options;

ساكن بنية argp_option options[]
    = { { "image-path", OPTION_IMAGE_لار, "DIR", 0, "path for saving criu image files", 0 },
        { "work-path", OPTION_WORK_لار, "DIR", 0, "path for saving work files and logs", 0 },
        { "leave-running", OPTION_LEAVE_RUNNING, 0, 0, "leave the process running after checkpointing", 0 },
        { "tcp-established", OPTION_TCP_ESTABLISHED, 0, 0, "allow open tcp connections", 0 },
        { "ext-unix-sk", OPTION_EXT_UNIX_SK, 0, 0, "allow external unix sockets", 0 },
        { "shell-job", OPTION_SHELL_دنده, 0, 0, "allow shell jobs", 0 },
        { "file-locks", OPTION_FILE_LOCKS, 0, 0, "allow file locks", 0 },
        { "network-lock", OPTION_شبکه_قلف_لاره, "METHOD", 0, "set network lock method", 0 },
#که_تعریف_شوی CRIU_PRE_DUMP_SUPPORT
        { "parent-path", OPTION_پلار_لار, "DIR", 0, "path for previous criu image files in pre-dump", 0 },
        { "pre-dump", OPTION_PRE_DUMP, 0, 0, "dump container's memory information only, leave the container running after this", 0 },
#د_که_پای
        { "manage-cgroups-mode", OPTION_MANAGE_CGROUPS_ډېرپېښ, "MODE", 0, "cgroups mode: 'soft' (default), 'ignore', 'full' and 'strict'", 0 },
        {
            0,
        } };

ساكن حرف دلیلونه_doc[] = "checkpoint CONTAINER";

/* Map the CLI names onto the public LIBCRUN_CR_* enums; libcrun translates
   these to CRIU's own values internally so CRIU headers never reach here.  */
صحيح
crun_parse_شبکه_قلف_لاره (حرف *param)
{
  إذا (strcmp (param, "iptables") == 0)
    أرجع LIBCRUN_CR_شبکه_قلف_IPTABLES;
  وإلا إذا (strcmp (param, "nftables") == 0)
    أرجع LIBCRUN_CR_شبکه_قلف_NFTABLES;
  وإلا إذا (strcmp (param, "skip") == 0)
    أرجع LIBCRUN_CR_شبکه_قلف_SKIP;
  وإلا
    libcrun_fail_with_error (0, "unknown network lock method specified");
}

صحيح
crun_parse_manage_cgroups_ډېرپېښ (حرف *param)
{
  إذا (strcmp (param, "soft") == 0)
    أرجع LIBCRUN_CR_CG_ډېرپېښ_SOFT;
  وإلا إذا (strcmp (param, "ignore") == 0)
    أرجع LIBCRUN_CR_CG_ډېرپېښ_IGNORE;
  وإلا إذا (strcmp (param, "full") == 0)
    أرجع LIBCRUN_CR_CG_ډېرپېښ_ډک;
  وإلا إذا (strcmp (param, "strict") == 0)
    أرجع LIBCRUN_CR_CG_ډېرپېښ_STRICT;
  وإلا
    libcrun_fail_with_error (0, "unknown cgroup mode specified");
}

ساكن error_t
parse_opt (صحيح کلی, حرف *arg, بنية argp_state *state)
{
  بدل (کلی)
    {
    حالة ARGP_کلی_NO_دلیلونه:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    حالة OPTION_IMAGE_لار:
      cr_options.image_لار = argp_mandatory_دلیل (arg, state);
      اكسر;

    حالة OPTION_WORK_لار:
      cr_options.work_لار = argp_mandatory_دلیل (arg, state);
      اكسر;

    حالة OPTION_پلار_لار:
      cr_options.پلار_لار = argp_mandatory_دلیل (arg, state);
      اكسر;

    حالة OPTION_PRE_DUMP:
      cr_options.pre_dump = true;
      اكسر;

    حالة OPTION_LEAVE_RUNNING:
      cr_options.leave_running = true;
      اكسر;

    حالة OPTION_TCP_ESTABLISHED:
      cr_options.tcp_established = true;
      اكسر;

    حالة OPTION_EXT_UNIX_SK:
      cr_options.ext_unix_sk = true;
      اكسر;

    حالة OPTION_SHELL_دنده:
      cr_options.shell_دنده = true;
      اكسر;

    حالة OPTION_FILE_LOCKS:
      cr_options.file_locks = true;
      اكسر;

    حالة OPTION_MANAGE_CGROUPS_ډېرپېښ:
      cr_options.manage_cgroups_ډېرپېښ = crun_parse_manage_cgroups_ډېرپېښ (argp_mandatory_دلیل (arg, state));
      اكسر;

    حالة OPTION_شبکه_قلف_لاره:
      cr_options.شبکه_قلف_لاره = crun_parse_شبکه_قلف_لاره (argp_mandatory_دلیل (arg, state));
      اكسر;

    افتراضي:
      أرجع ARGP_ERR_UNKNOWN;
    }

  أرجع 0;
}

ساكن بنية argp پرلپسې_argp = { options, parse_opt, دلیلونه_doc, doc, عدم, عدم, عدم };

صحيح
crun_command_checkpoint (بنية crun_global_arguments *global_دلیلونه, صحيح argc, حرف **argv, libcrun_error_t *err)
{
  cleanup_free حرف *cr_لار = عدم;
  صحيح لومړی_arg;
  صحيح ret;
  cleanup_context libcrun_context_t *crun_context = عدم;

  cr_options.struct_اندازه = حجم (cr_options);

  argp_parse (&پرلپسې_argp, argc, argv, ARGP_دننه_ترتیب, &لومړی_arg, &cr_options);
  crun_assert_n_دلیلونه (argc - لومړی_arg, 1, 2);

  crun_context = new_libcrun_context (global_دلیلونه);

  ret = init_libcrun_context (crun_context, argv[لومړی_arg], global_دلیلونه, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (cr_options.image_لار == عدم)
    {
      cleanup_free حرف *لار = عدم;

      لار = getcwd (عدم, 0);
      إذا (UNLIKELY (لار == عدم))
        libcrun_fail_with_error (errno, "getcwd failed");

      ret = asprintf (&cr_لار, "%s/checkpoint", لار);
      إذا (UNLIKELY (ret < 0))
        OOM ();
      cr_options.image_لار = cr_لار;
    }

  أرجع libcrun_container_checkpoint (crun_context, argv[لومړی_arg], &cr_options, err);
}
