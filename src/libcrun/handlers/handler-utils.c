/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
 * crun is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * crun is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with crun.  If not, see <http://www.gnu.org/licenses/>.
 */

#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ "../container.h"
#అన్తర్భావ "../utils.h"
#అన్తర్భావ "handler-utils.h"

పూర్ణాంకం
wasm_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_వాడని)
{
  స్థిరరాశి అక్షరం *annotation;
  స్థిరరాశి అక్షరం *entrypoint_executable;

  ఒకవేళ (container->container_def->process == ఖాళీ || container->container_def->process->వాదనలు == ఖాళీ)
    తిరిగి 0;

  entrypoint_executable = container->container_def->process->వాదనలు[0];

  annotation = కనుగొను_annotation (container, "run.oci.handler");
  ఒకవేళ (annotation)
    {

      /* wasm-smart: annotation is a smart switch which only toggles wasm if it's necessary,
         following annotation is very useful for cases where users intend to run wasm workload on
         kubernetes cluster but workload also contains side-cars which could execute non-wasm workload.
         Example: Kubernetes clusters with service-mesh such as istio, linkerd etc
      */
      ఒకవేళ (strcmp (annotation, "wasm-smart") == 0)
        {
          తిరిగి ((has_suffix (entrypoint_executable, ".wat") > 0) || (has_suffix (entrypoint_executable, ".wasm") > 0)) ? 1 : 0;
        }
      తిరిగి strcmp (annotation, "wasm") == 0 ? 1 : 0;
    }

  annotation = కనుగొను_annotation (container, "module.wasm.image/variant");
  ఒకవేళ (annotation)
    {

      /* compat-smart: annotation is a smart switch which only toggles wasm if it's necessary,
         following annotation is very useful for cases where users intend to run wasm workload on
         kubernetes cluster but workload also contains side-cars which could execute non-wasm workload.
         Example: Kubernetes clusters with service-mesh such as istio, linkerd etc
      */
      ఒకవేళ (strcmp (annotation, "compat-smart") == 0)
        {
          తిరిగి ((has_suffix (entrypoint_executable, ".wat") > 0) || (has_suffix (entrypoint_executable, ".wasm") > 0)) ? 1 : 0;
        }

      తిరిగి strcmp (annotation, "compat") == 0 ? 1 : 0;
    }

  తిరిగి 0;
}
