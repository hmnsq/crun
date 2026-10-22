/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <argp.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <errno.h>

#అన్తర్భావ "crun.h"

స్థిర అక్షరం doc[] = "OCI runtime";

గణన
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

నిర్మాణం list_options_s
{
  bool quiet;
  పూర్ణాంకం format;
};

గణన
{
  LIST_పట్టిక = 100,
  LIST_JSON,
};

స్థిర నిర్మాణం list_options_s list_options;

స్థిర నిర్మాణం argp_option options[]
    = { { "quiet", 'q', 0, 0, "show only IDs", 0 },
        { "format", 'f', "FORMAT", 0, "select one of: table or json (default: \"table\")", 0 },
        {
            0,
        } };

స్థిర అక్షరం వాదనలు_doc[] = "list";

స్థిర error_t
parse_opt (పూర్ణాంకం కీలకం, అక్షరం *arg, నిర్మాణం argp_state *state arg_వాడని)
{
  ఎంపిక (కీలకం)
    {
    స్థితి 'q':
      list_options.quiet = true;
      ఆపు;
    స్థితి 'f':
      ఒకవేళ (strcmp (arg, "table") == 0)
        list_options.format = LIST_పట్టిక;
      లేకపోతే ఒకవేళ (strcmp (arg, "json") == 0)
        list_options.format = LIST_JSON;
      లేకపోతే
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      ఆపు;

    అప్రమేయ:
      తిరిగి ARGP_ERR_UNKNOWN;
    }

  తిరిగి 0;
}

స్థిర నిర్మాణం argp నిరంతరం_argp = { options, parse_opt, వాదనలు_doc, doc, ఖాళీ, ఖాళీ, ఖాళీ };

పూర్ణాంకం
crun_command_list (నిర్మాణం crun_global_arguments *global_వాదనలు, పూర్ణాంకం argc, అక్షరం **argv, libcrun_error_t *err)
{
  పూర్ణాంకం మొదటి_arg;
  పూర్ణాంకం ret, max_నిడివి = 4;
  cleanup_context libcrun_context_t *crun_context = ఖాళీ;
  libcrun_container_list_t *list = ఖాళీ;
  libcrun_container_iter_t *it;
  స్థిరరాశి అక్షరం *name;

  list_options.format = LIST_పట్టిక;

  argp_parse (&నిరంతరం_argp, argc, argv, ARGP_లోపల_క్రమం, &మొదటి_arg, &list_options);
  crun_assert_n_వాదనలు (argc - మొదటి_arg, 0, 0);

  crun_context = new_libcrun_context (global_వాదనలు);

  ret = init_libcrun_context (crun_context, argv[మొదటి_arg], global_వాదనలు, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (list_options.format == LIST_JSON)
    {
      cleanup_free అక్షరం *json = ఖాళీ;

      ret = libcrun_container_list_json (crun_context, &json, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      fputs (json, stdout);
      తిరిగి 0;
    }

  ret = libcrun_container_list (crun_context, &list, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  కొరకు (it = libcrun_container_list_iter (list); libcrun_container_iter_తదుపరి (it, &name);)
    {
      పూర్ణాంకం l = అక్షరశ్రేణి_పొడవు (name);
      ఒకవేళ (l > max_నిడివి)
        max_నిడివి = l;
    }
  libcrun_container_iter_free (it);

  max_నిడివి++;

  ఒకవేళ (! list_options.quiet)
    రాయి ("%-*s%-10s%-8s %-39s %-30s %s\n", max_నిడివి, "NAME", "PID", "STATUS", "BUNDLE PATH", "CREATED", "OWNER");

  కొరకు (it = libcrun_container_list_iter (list); libcrun_container_iter_తదుపరి (it, &name);)
    {
      libcrun_స్థితి_t *స్థితి_2 = ఖాళీ;

      ret = libcrun_container_స్థితి_ఎక్కించు (crun_context, name, &స్థితి_2, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          కొనసాగు;
        }
      ఒకవేళ (list_options.quiet)
        రాయి ("%s\n", name);
      లేకపోతే
        {
          libcrun_container_state_t state;
          పూర్ణాంకం running = 0;
          పూర్ణాంకం pid = libcrun_స్థితి_get_pid (స్థితి_2);

          ret = libcrun_స్థితి_get_state (స్థితి_2, &state, &running, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            {
              libcrun_error_report_and_release (err);
              libcrun_container_స్థితి_free (స్థితి_2);
              కొనసాగు;
            }

          ఒకవేళ (! running)
            pid = 0;

          రాయి ("%-*s%-10d%-8s %-39s %-30s %s\n", max_నిడివి, name, pid,
                  libcrun_container_state_గమ్యం_string (state), libcrun_స్థితి_get_bundle (స్థితి_2),
                  libcrun_స్థితి_get_created (స్థితి_2), libcrun_స్థితి_get_owner (స్థితి_2));
        }

      libcrun_container_స్థితి_free (స్థితి_2);
    }
  libcrun_container_iter_free (it);

  libcrun_container_list_free (list);
  తిరిగి ret >= 0 ? 0 : ret;
}
