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

#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <argp.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <errno.h>
#అన్తర్భావ <regex.h>

#అన్తర్భావ "crun.h"

గణన
{
  OPTION_IMAGE_మార్గం = 1000,
  OPTION_WORK_మార్గం,
  OPTION_LEAVE_RUNNING,
  OPTION_TCP_ESTABLISHED,
  OPTION_SHELL_పని,
  OPTION_EXT_UNIX_SK,
  OPTION_FILE_LOCKS,
  OPTION_జాలం_తాళం_పద్ధతి,
  OPTION_మాతృ_మార్గం,
  OPTION_PRE_DUMP,
  OPTION_MANAGE_CGROUPS_బహుళకం,
};

స్థిర అక్షరం doc[] = "OCI runtime";

స్థిర నిర్మాణం libcrun_checkpoint_restore_options_s cr_options;

స్థిర నిర్మాణం argp_option options[]
    = { { "image-path", OPTION_IMAGE_మార్గం, "DIR", 0, "path for saving criu image files", 0 },
        { "work-path", OPTION_WORK_మార్గం, "DIR", 0, "path for saving work files and logs", 0 },
        { "leave-running", OPTION_LEAVE_RUNNING, 0, 0, "leave the process running after checkpointing", 0 },
        { "tcp-established", OPTION_TCP_ESTABLISHED, 0, 0, "allow open tcp connections", 0 },
        { "ext-unix-sk", OPTION_EXT_UNIX_SK, 0, 0, "allow external unix sockets", 0 },
        { "shell-job", OPTION_SHELL_పని, 0, 0, "allow shell jobs", 0 },
        { "file-locks", OPTION_FILE_LOCKS, 0, 0, "allow file locks", 0 },
        { "network-lock", OPTION_జాలం_తాళం_పద్ధతి, "METHOD", 0, "set network lock method", 0 },
#నిర్వచితమైతే CRIU_PRE_DUMP_SUPPORT
        { "parent-path", OPTION_మాతృ_మార్గం, "DIR", 0, "path for previous criu image files in pre-dump", 0 },
        { "pre-dump", OPTION_PRE_DUMP, 0, 0, "dump container's memory information only, leave the container running after this", 0 },
#అయితే_ముగింపు
        { "manage-cgroups-mode", OPTION_MANAGE_CGROUPS_బహుళకం, "MODE", 0, "cgroups mode: 'soft' (default), 'ignore', 'full' and 'strict'", 0 },
        {
            0,
        } };

స్థిర అక్షరం వాదనలు_doc[] = "checkpoint CONTAINER";

/* Map the CLI names onto the public LIBCRUN_CR_* enums; libcrun translates
   these to CRIU's own values internally so CRIU headers never reach here.  */
పూర్ణాంకం
crun_parse_జాలం_తాళం_పద్ధతి (అక్షరం *param)
{
  ఒకవేళ (strcmp (param, "iptables") == 0)
    తిరిగి LIBCRUN_CR_జాలం_తాళం_IPTABLES;
  లేకపోతే ఒకవేళ (strcmp (param, "nftables") == 0)
    తిరిగి LIBCRUN_CR_జాలం_తాళం_NFTABLES;
  లేకపోతే ఒకవేళ (strcmp (param, "skip") == 0)
    తిరిగి LIBCRUN_CR_జాలం_తాళం_SKIP;
  లేకపోతే
    libcrun_fail_with_error (0, "unknown network lock method specified");
}

పూర్ణాంకం
crun_parse_manage_cgroups_బహుళకం (అక్షరం *param)
{
  ఒకవేళ (strcmp (param, "soft") == 0)
    తిరిగి LIBCRUN_CR_CG_బహుళకం_SOFT;
  లేకపోతే ఒకవేళ (strcmp (param, "ignore") == 0)
    తిరిగి LIBCRUN_CR_CG_బహుళకం_IGNORE;
  లేకపోతే ఒకవేళ (strcmp (param, "full") == 0)
    తిరిగి LIBCRUN_CR_CG_బహుళకం_నిండిన;
  లేకపోతే ఒకవేళ (strcmp (param, "strict") == 0)
    తిరిగి LIBCRUN_CR_CG_బహుళకం_STRICT;
  లేకపోతే
    libcrun_fail_with_error (0, "unknown cgroup mode specified");
}

స్థిర error_t
parse_opt (పూర్ణాంకం కీలకం, అక్షరం *arg, నిర్మాణం argp_state *state)
{
  ఎంపిక (కీలకం)
    {
    స్థితి ARGP_కీలకం_NO_వాదనలు:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    స్థితి OPTION_IMAGE_మార్గం:
      cr_options.image_మార్గం = argp_mandatory_వాదం (arg, state);
      ఆపు;

    స్థితి OPTION_WORK_మార్గం:
      cr_options.work_మార్గం = argp_mandatory_వాదం (arg, state);
      ఆపు;

    స్థితి OPTION_మాతృ_మార్గం:
      cr_options.మాతృ_మార్గం = argp_mandatory_వాదం (arg, state);
      ఆపు;

    స్థితి OPTION_PRE_DUMP:
      cr_options.pre_dump = true;
      ఆపు;

    స్థితి OPTION_LEAVE_RUNNING:
      cr_options.leave_running = true;
      ఆపు;

    స్థితి OPTION_TCP_ESTABLISHED:
      cr_options.tcp_established = true;
      ఆపు;

    స్థితి OPTION_EXT_UNIX_SK:
      cr_options.ext_unix_sk = true;
      ఆపు;

    స్థితి OPTION_SHELL_పని:
      cr_options.shell_పని = true;
      ఆపు;

    స్థితి OPTION_FILE_LOCKS:
      cr_options.file_locks = true;
      ఆపు;

    స్థితి OPTION_MANAGE_CGROUPS_బహుళకం:
      cr_options.manage_cgroups_బహుళకం = crun_parse_manage_cgroups_బహుళకం (argp_mandatory_వాదం (arg, state));
      ఆపు;

    స్థితి OPTION_జాలం_తాళం_పద్ధతి:
      cr_options.జాలం_తాళం_పద్ధతి = crun_parse_జాలం_తాళం_పద్ధతి (argp_mandatory_వాదం (arg, state));
      ఆపు;

    అప్రమేయ:
      తిరిగి ARGP_ERR_UNKNOWN;
    }

  తిరిగి 0;
}

స్థిర నిర్మాణం argp నిరంతరం_argp = { options, parse_opt, వాదనలు_doc, doc, ఖాళీ, ఖాళీ, ఖాళీ };

పూర్ణాంకం
crun_command_checkpoint (నిర్మాణం crun_global_arguments *global_వాదనలు, పూర్ణాంకం argc, అక్షరం **argv, libcrun_error_t *err)
{
  cleanup_free అక్షరం *cr_మార్గం = ఖాళీ;
  పూర్ణాంకం మొదటి_arg;
  పూర్ణాంకం ret;
  cleanup_context libcrun_context_t *crun_context = ఖాళీ;

  cr_options.struct_కొలత = పరిమాణం (cr_options);

  argp_parse (&నిరంతరం_argp, argc, argv, ARGP_లోపల_క్రమం, &మొదటి_arg, &cr_options);
  crun_assert_n_వాదనలు (argc - మొదటి_arg, 1, 2);

  crun_context = new_libcrun_context (global_వాదనలు);

  ret = init_libcrun_context (crun_context, argv[మొదటి_arg], global_వాదనలు, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (cr_options.image_మార్గం == ఖాళీ)
    {
      cleanup_free అక్షరం *మార్గం = ఖాళీ;

      మార్గం = getcwd (ఖాళీ, 0);
      ఒకవేళ (UNLIKELY (మార్గం == ఖాళీ))
        libcrun_fail_with_error (errno, "getcwd failed");

      ret = asprintf (&cr_మార్గం, "%s/checkpoint", మార్గం);
      ఒకవేళ (UNLIKELY (ret < 0))
        OOM ();
      cr_options.image_మార్గం = cr_మార్గం;
    }

  తిరిగి libcrun_container_checkpoint (crun_context, argv[మొదటి_arg], &cr_options, err);
}
