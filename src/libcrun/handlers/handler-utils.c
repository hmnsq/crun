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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "../container.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "handler-utils.h"

ਪੂਰਨ
wasm_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ ਅੱਖਰ *annotation;
  ਅਚਲ ਅੱਖਰ *entrypoint_executable;

  ਜੇ (container->container_def->process == ਨਿਰਾ || container->container_def->process->ਦਲੀਲਾਂ == ਨਿਰਾ)
    ਵਾਪਸ 0;

  entrypoint_executable = container->container_def->process->ਦਲੀਲਾਂ[0];

  annotation = ਲੱਭੋ_annotation (container, "run.oci.handler");
  ਜੇ (annotation)
    {

      /* wasm-smart: annotation is a smart switch which only toggles wasm if it's necessary,
         following annotation is very useful for cases where users intend to run wasm workload on
         kubernetes cluster but workload also contains side-cars which could execute non-wasm workload.
         Example: Kubernetes clusters with service-mesh such as istio, linkerd etc
      */
      ਜੇ (strcmp (annotation, "wasm-smart") == 0)
        {
          ਵਾਪਸ ((has_suffix (entrypoint_executable, ".wat") > 0) || (has_suffix (entrypoint_executable, ".wasm") > 0)) ? 1 : 0;
        }
      ਵਾਪਸ strcmp (annotation, "wasm") == 0 ? 1 : 0;
    }

  annotation = ਲੱਭੋ_annotation (container, "module.wasm.image/variant");
  ਜੇ (annotation)
    {

      /* compat-smart: annotation is a smart switch which only toggles wasm if it's necessary,
         following annotation is very useful for cases where users intend to run wasm workload on
         kubernetes cluster but workload also contains side-cars which could execute non-wasm workload.
         Example: Kubernetes clusters with service-mesh such as istio, linkerd etc
      */
      ਜੇ (strcmp (annotation, "compat-smart") == 0)
        {
          ਵਾਪਸ ((has_suffix (entrypoint_executable, ".wat") > 0) || (has_suffix (entrypoint_executable, ".wasm") > 0)) ? 1 : 0;
        }

      ਵਾਪਸ strcmp (annotation, "compat") == 0 ? 1 : 0;
    }

  ਵਾਪਸ 0;
}
