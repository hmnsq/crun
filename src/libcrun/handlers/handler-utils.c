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

#define _GNU_SOURCE

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "../container.h"
#അന്തര്ഭാവ "../utils.h"
#അന്തര്ഭാവ "handler-utils.h"

പൂർണ്ണസംഖ്യ
wasm_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  സ്ഥിരം അക്ഷരം *annotation;
  സ്ഥിരം അക്ഷരം *entrypoint_executable;

  എങ്കിൽ (container->container_def->process == ശൂന്യ || container->container_def->process->args == ശൂന്യ)
    തിരികെ 0;

  entrypoint_executable = container->container_def->process->args[0];

  annotation = find_annotation (container, "run.oci.handler");
  എങ്കിൽ (annotation)
    {

      /* wasm-smart: annotation is a smart switch which only toggles wasm if it's necessary,
         following annotation is very useful for cases where users intend to run wasm workload on
         kubernetes cluster but workload also contains side-cars which could execute non-wasm workload.
         Example: Kubernetes clusters with service-mesh such as istio, linkerd etc
      */
      എങ്കിൽ (strcmp (annotation, "wasm-smart") == 0)
        {
          തിരികെ ((has_suffix (entrypoint_executable, ".wat") > 0) || (has_suffix (entrypoint_executable, ".wasm") > 0)) ? 1 : 0;
        }
      തിരികെ strcmp (annotation, "wasm") == 0 ? 1 : 0;
    }

  annotation = find_annotation (container, "module.wasm.image/variant");
  എങ്കിൽ (annotation)
    {

      /* compat-smart: annotation is a smart switch which only toggles wasm if it's necessary,
         following annotation is very useful for cases where users intend to run wasm workload on
         kubernetes cluster but workload also contains side-cars which could execute non-wasm workload.
         Example: Kubernetes clusters with service-mesh such as istio, linkerd etc
      */
      എങ്കിൽ (strcmp (annotation, "compat-smart") == 0)
        {
          തിരികെ ((has_suffix (entrypoint_executable, ".wat") > 0) || (has_suffix (entrypoint_executable, ".wasm") > 0)) ? 1 : 0;
        }

      തിരികെ strcmp (annotation, "compat") == 0 ? 1 : 0;
    }

  തിരികെ 0;
}
