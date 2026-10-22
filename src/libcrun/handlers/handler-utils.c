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

#परिभाषय _GNU_SOURCE

#अन्तर्भाव <config.h>
#अन्तर्भाव "../container.h"
#अन्तर्भाव "../utils.h"
#अन्तर्भाव "handler-utils.h"

पूर्णाङ्क
wasm_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_अप्रयुक्तम्)
{
  ध्रुव वर्ण *annotation;
  ध्रुव वर्ण *entrypoint_executable;

  यदि (container->container_def->process == रिक्त || container->container_def->process->तर्काः == रिक्त)
    प्रत्यागम 0;

  entrypoint_executable = container->container_def->process->तर्काः[0];

  annotation = find_annotation (container, "run.oci.handler");
  यदि (annotation)
    {

      /* wasm-smart: annotation is a smart switch which only toggles wasm if it's necessary,
         following annotation is very useful for cases where users intend to run wasm workload on
         kubernetes cluster but workload also contains side-cars which could execute non-wasm workload.
         Example: Kubernetes clusters with service-mesh such as istio, linkerd etc
      */
      यदि (वाक्यं_तुलय (annotation, "wasm-smart") == 0)
        {
          प्रत्यागम ((has_suffix (entrypoint_executable, ".wat") > 0) || (has_suffix (entrypoint_executable, ".wasm") > 0)) ? 1 : 0;
        }
      प्रत्यागम वाक्यं_तुलय (annotation, "wasm") == 0 ? 1 : 0;
    }

  annotation = find_annotation (container, "module.wasm.image/variant");
  यदि (annotation)
    {

      /* compat-smart: annotation is a smart switch which only toggles wasm if it's necessary,
         following annotation is very useful for cases where users intend to run wasm workload on
         kubernetes cluster but workload also contains side-cars which could execute non-wasm workload.
         Example: Kubernetes clusters with service-mesh such as istio, linkerd etc
      */
      यदि (वाक्यं_तुलय (annotation, "compat-smart") == 0)
        {
          प्रत्यागम ((has_suffix (entrypoint_executable, ".wat") > 0) || (has_suffix (entrypoint_executable, ".wasm") > 0)) ? 1 : 0;
        }

      प्रत्यागम वाक्यं_तुलय (annotation, "compat") == 0 ? 1 : 0;
    }

  प्रत्यागम 0;
}
