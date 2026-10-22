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

#परिभाषय _GNU_SOURCE

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <argp.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <regex.h>

#अन्तर्भाव "crun.h"

गणना
{
  OPTION_IMAGE_मार्गः = 1000,
  OPTION_WORK_मार्गः,
  OPTION_LEAVE_RUNNING,
  OPTION_TCP_ESTABLISHED,
  OPTION_SHELL_JOB,
  OPTION_EXT_UNIX_SK,
  OPTION_FILE_LOCKS,
  OPTION_जालम्_तालकम्_विधानम्,
  OPTION_PARENT_मार्गः,
  OPTION_PRE_DUMP,
  OPTION_MANAGE_CGROUPS_बहुलकम्,
};

स्थिर वर्ण doc[] = "OCI runtime";

स्थिर संरचना libcrun_checkpoint_restore_options_s cr_options;

स्थिर संरचना argp_option options[]
    = { { "image-path", OPTION_IMAGE_मार्गः, "DIR", 0, "path for saving criu image files", 0 },
        { "work-path", OPTION_WORK_मार्गः, "DIR", 0, "path for saving work files and logs", 0 },
        { "leave-running", OPTION_LEAVE_RUNNING, 0, 0, "leave the process running after checkpointing", 0 },
        { "tcp-established", OPTION_TCP_ESTABLISHED, 0, 0, "allow open tcp connections", 0 },
        { "ext-unix-sk", OPTION_EXT_UNIX_SK, 0, 0, "allow external unix sockets", 0 },
        { "shell-job", OPTION_SHELL_JOB, 0, 0, "allow shell jobs", 0 },
        { "file-locks", OPTION_FILE_LOCKS, 0, 0, "allow file locks", 0 },
        { "network-lock", OPTION_जालम्_तालकम्_विधानम्, "METHOD", 0, "set network lock method", 0 },
#यदि_परिभाषितम् CRIU_PRE_DUMP_SUPPORT
        { "parent-path", OPTION_PARENT_मार्गः, "DIR", 0, "path for previous criu image files in pre-dump", 0 },
        { "pre-dump", OPTION_PRE_DUMP, 0, 0, "dump container's memory information only, leave the container running after this", 0 },
#यदि_अन्तः
        { "manage-cgroups-mode", OPTION_MANAGE_CGROUPS_बहुलकम्, "MODE", 0, "cgroups mode: 'soft' (default), 'ignore', 'full' and 'strict'", 0 },
        {
            0,
        } };

स्थिर वर्ण तर्काः_doc[] = "checkpoint CONTAINER";

/* Map the CLI names onto the public LIBCRUN_CR_* enums; libcrun translates
   these to CRIU's own values internally so CRIU headers never reach here.  */
पूर्णाङ्क
crun_parse_जालम्_तालकम्_विधानम् (वर्ण *param)
{
  यदि (वाक्यं_तुलय (param, "iptables") == 0)
    प्रत्यागम LIBCRUN_CR_जालम्_तालकम्_IPTABLES;
  अन्यथा यदि (वाक्यं_तुलय (param, "nftables") == 0)
    प्रत्यागम LIBCRUN_CR_जालम्_तालकम्_NFTABLES;
  अन्यथा यदि (वाक्यं_तुलय (param, "skip") == 0)
    प्रत्यागम LIBCRUN_CR_जालम्_तालकम्_SKIP;
  अन्यथा
    libcrun_fail_with_error (0, "unknown network lock method specified");
}

पूर्णाङ्क
crun_parse_manage_cgroups_बहुलकम् (वर्ण *param)
{
  यदि (वाक्यं_तुलय (param, "soft") == 0)
    प्रत्यागम LIBCRUN_CR_CG_बहुलकम्_SOFT;
  अन्यथा यदि (वाक्यं_तुलय (param, "ignore") == 0)
    प्रत्यागम LIBCRUN_CR_CG_बहुलकम्_IGNORE;
  अन्यथा यदि (वाक्यं_तुलय (param, "full") == 0)
    प्रत्यागम LIBCRUN_CR_CG_बहुलकम्_पूर्णम्;
  अन्यथा यदि (वाक्यं_तुलय (param, "strict") == 0)
    प्रत्यागम LIBCRUN_CR_CG_बहुलकम्_STRICT;
  अन्यथा
    libcrun_fail_with_error (0, "unknown cgroup mode specified");
}

स्थिर error_t
parse_opt (पूर्णाङ्क कुञ्चिका, वर्ण *arg, संरचना argp_state *state)
{
  विकल्प (कुञ्चिका)
    {
    पक्ष ARGP_कुञ्चिका_NO_तर्काः:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    पक्ष OPTION_IMAGE_मार्गः:
      cr_options.image_मार्गः = argp_mandatory_तर्कः (arg, state);
      भङ्ग;

    पक्ष OPTION_WORK_मार्गः:
      cr_options.work_मार्गः = argp_mandatory_तर्कः (arg, state);
      भङ्ग;

    पक्ष OPTION_PARENT_मार्गः:
      cr_options.parent_मार्गः = argp_mandatory_तर्कः (arg, state);
      भङ्ग;

    पक्ष OPTION_PRE_DUMP:
      cr_options.pre_dump = true;
      भङ्ग;

    पक्ष OPTION_LEAVE_RUNNING:
      cr_options.leave_running = true;
      भङ्ग;

    पक्ष OPTION_TCP_ESTABLISHED:
      cr_options.tcp_established = true;
      भङ्ग;

    पक्ष OPTION_EXT_UNIX_SK:
      cr_options.ext_unix_sk = true;
      भङ्ग;

    पक्ष OPTION_SHELL_JOB:
      cr_options.shell_job = true;
      भङ्ग;

    पक्ष OPTION_FILE_LOCKS:
      cr_options.file_locks = true;
      भङ्ग;

    पक्ष OPTION_MANAGE_CGROUPS_बहुलकम्:
      cr_options.manage_cgroups_बहुलकम् = crun_parse_manage_cgroups_बहुलकम् (argp_mandatory_तर्कः (arg, state));
      भङ्ग;

    पक्ष OPTION_जालम्_तालकम्_विधानम्:
      cr_options.जालम्_तालकम्_विधानम् = crun_parse_जालम्_तालकम्_विधानम् (argp_mandatory_तर्कः (arg, state));
      भङ्ग;

    स्वभाव:
      प्रत्यागम ARGP_ERR_UNKNOWN;
    }

  प्रत्यागम 0;
}

स्थिर संरचना argp सततम्_argp = { options, parse_opt, तर्काः_doc, doc, रिक्त, रिक्त, रिक्त };

पूर्णाङ्क
crun_command_checkpoint (संरचना crun_global_arguments *global_तर्काः, पूर्णाङ्क argc, वर्ण **argv, libcrun_error_t *err)
{
  cleanup_free वर्ण *cr_मार्गः = रिक्त;
  पूर्णाङ्क first_arg;
  पूर्णाङ्क ret;
  cleanup_context libcrun_context_t *crun_context = रिक्त;

  cr_options.struct_आकार = परिमाण (cr_options);

  argp_parse (&सततम्_argp, argc, argv, ARGP_अन्तर्_क्रमः, &first_arg, &cr_options);
  crun_assert_n_तर्काः (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_तर्काः);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_तर्काः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (cr_options.image_मार्गः == रिक्त)
    {
      cleanup_free वर्ण *मार्गः = रिक्त;

      मार्गः = getcwd (रिक्त, 0);
      यदि (UNLIKELY (मार्गः == रिक्त))
        libcrun_fail_with_error (errno, "getcwd failed");

      ret = asprintf (&cr_मार्गः, "%s/checkpoint", मार्गः);
      यदि (UNLIKELY (ret < 0))
        OOM ();
      cr_options.image_मार्गः = cr_मार्गः;
    }

  प्रत्यागम libcrun_container_checkpoint (crun_context, argv[first_arg], &cr_options, err);
}
