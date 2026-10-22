/*
 * crun - OCI runtime written in C
 *
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

స్థిర నిర్మాణం argp_option options[] = { { 0 } };

స్థిర అక్షరం వాదనలు_doc[] = "features";

స్థిర error_t
parse_opt (పూర్ణాంకం కీలకం, అక్షరం *arg arg_వాడని, నిర్మాణం argp_state *state arg_వాడని)
{
  ఒకవేళ (కీలకం != ARGP_కీలకం_NO_వాదనలు)
    {
      తిరిగి ARGP_ERR_UNKNOWN;
    }

  తిరిగి 0;
}

స్థిర నిర్మాణం argp నిరంతరం_argp = { options, parse_opt, వాదనలు_doc, doc, ఖాళీ, ఖాళీ, ఖాళీ };

పూర్ణాంకం
crun_command_features (నిర్మాణం crun_global_arguments *global_వాదనలు, పూర్ణాంకం argc, అక్షరం **argv, libcrun_error_t *err)
{
  cleanup_free అక్షరం *json = ఖాళీ;
  పూర్ణాంకం మొదటి_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = ఖాళీ;

  argp_parse (&నిరంతరం_argp, argc, argv, 0, 0, &options);

  crun_context = new_libcrun_context (global_వాదనలు);

  ret = init_libcrun_context (crun_context, argv[మొదటి_arg], global_వాదనలు, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_container_get_features_json (crun_context, &json, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  రాయి ("%s", json);

  తిరిగి 0;
}
