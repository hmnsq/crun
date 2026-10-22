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

ساكن حرف doc[] = "OCI runtime";

ساكن بنية libcrun_checkpoint_restore_options_s cr_options;

ساكن بنية argp_option options[]
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

ساكن حرف وسائط_doc[] = "checkpoint CONTAINER";

/* Map the CLI names onto the public LIBCRUN_CR_* enums; libcrun translates
   these to CRIU's own values internally so CRIU headers never reach here.  */
صحيح
crun_parse_network_lock_method (حرف *param)
{
  إذا (strcmp (param, "iptables") == 0)
    أرجع LIBCRUN_CR_NETWORK_LOCK_IPTABLES;
  وإلا إذا (strcmp (param, "nftables") == 0)
    أرجع LIBCRUN_CR_NETWORK_LOCK_NFTABLES;
  وإلا إذا (strcmp (param, "skip") == 0)
    أرجع LIBCRUN_CR_NETWORK_LOCK_SKIP;
  وإلا
    libcrun_fail_with_error (0, "unknown network lock method specified");
}

صحيح
crun_parse_manage_cgroups_mode (حرف *param)
{
  إذا (strcmp (param, "soft") == 0)
    أرجع LIBCRUN_CR_CG_MODE_SOFT;
  وإلا إذا (strcmp (param, "ignore") == 0)
    أرجع LIBCRUN_CR_CG_MODE_IGNORE;
  وإلا إذا (strcmp (param, "full") == 0)
    أرجع LIBCRUN_CR_CG_MODE_FULL;
  وإلا إذا (strcmp (param, "strict") == 0)
    أرجع LIBCRUN_CR_CG_MODE_STRICT;
  وإلا
    libcrun_fail_with_error (0, "unknown cgroup mode specified");
}

ساكن error_t
parse_opt (صحيح مفتاح, حرف *arg, بنية argp_state *state)
{
  بدل (مفتاح)
    {
    حالة ARGP_مفتاح_NO_وسائط:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    حالة OPTION_IMAGE_PATH:
      cr_options.image_path = argp_mandatory_argument (arg, state);
      اكسر;

    حالة OPTION_WORK_PATH:
      cr_options.work_path = argp_mandatory_argument (arg, state);
      اكسر;

    حالة OPTION_PARENT_PATH:
      cr_options.parent_path = argp_mandatory_argument (arg, state);
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

    حالة OPTION_SHELL_JOB:
      cr_options.shell_job = true;
      اكسر;

    حالة OPTION_FILE_LOCKS:
      cr_options.file_locks = true;
      اكسر;

    حالة OPTION_MANAGE_CGROUPS_MODE:
      cr_options.manage_cgroups_mode = crun_parse_manage_cgroups_mode (argp_mandatory_argument (arg, state));
      اكسر;

    حالة OPTION_NETWORK_LOCK_METHOD:
      cr_options.network_lock_method = crun_parse_network_lock_method (argp_mandatory_argument (arg, state));
      اكسر;

    افتراضي:
      أرجع ARGP_ERR_UNKNOWN;
    }

  أرجع 0;
}

ساكن بنية argp run_argp = { options, parse_opt, وسائط_doc, doc, عدم, عدم, عدم };

صحيح
crun_command_checkpoint (بنية crun_global_arguments *global_وسائط, صحيح argc, حرف **argv, libcrun_error_t *err)
{
  cleanup_free حرف *cr_path = عدم;
  صحيح first_arg;
  صحيح ret;
  cleanup_context libcrun_context_t *crun_context = عدم;

  cr_options.struct_size = حجم (cr_options);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &cr_options);
  crun_assert_n_وسائط (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_وسائط);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_وسائط, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (cr_options.image_path == عدم)
    {
      cleanup_free حرف *path = عدم;

      path = getcwd (عدم, 0);
      إذا (UNLIKELY (path == عدم))
        libcrun_fail_with_error (errno, "getcwd failed");

      ret = asprintf (&cr_path, "%s/checkpoint", path);
      إذا (UNLIKELY (ret < 0))
        OOM ();
      cr_options.image_path = cr_path;
    }

  أرجع libcrun_container_checkpoint (crun_context, argv[first_arg], &cr_options, err);
}
