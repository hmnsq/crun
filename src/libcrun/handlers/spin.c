/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2023 Sven Pfennig <s.pfennig@reply.de>
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

#include <config.h>
#include "../custom-handler.h"
#include "../linux.h"
#include <sys/stat.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_SPIN

#endif

#නම් HAVE_DLOPEN && HAVE_SPIN
static පූර්ණ_සංඛ්‍යා
spin_exec (void *cookie arg_unused, libcrun_container_t *container arg_unused,
           const var_na *pathname arg_unused, var_na *const argv[] arg_unused)
{
  // wasmtime fails to determine default config path if $HOME is not set
  var_na *newenviron[] = { "HOME=/root", NULL };
  var_na *newargv[] = { "/bin/spin", "up", "--listen", "0.0.0.0:80", NULL };

  // spin up needs a /tmp folder
  පූර්ණ_සංඛ්‍යා dir_result = mkdir ("/tmp", 0777);
  නම් (dir_result != 0 && errno != EEXIST)
    {
      error (EXIT_FAILURE, errno, "failed to execute mkdir `/tmp`");
    }

  execve (newargv[0], newargv, newenviron);
  perror ("execve");
  exit (EXIT_FAILURE);
}

static පූර්ණ_සංඛ්‍යා
spin_load (void **cookie arg_unused, libcrun_error_t *err)
{
  sa_nracanaa stat st = { 0 };
  නම් (stat ("/usr/local/bin/spin", &st) == -1)
    {
      ආපසු crun_make_error (err, 0, "Could not find statically linked spin cli at `/usr/local/bin/spin` on host file system");
    }
  ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා
spin_configure_container (void *cookie arg_unused, ga_nanaa handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          const var_na *rootfs arg_unused, libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා ret;
  නම් (phase != HANDLER_CONFIGURE_MOUNTS)
    ආපසු 0;

  var_na *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/local/bin/spin", "/bin/spin", options, 5, err);
  නම් (ret != 0)
    ආපසු ret;

  /* release any error if set since we are going to be returning from here */
  crun_error_release (err);

  ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා
spin_unload (void *cookie arg_unused, libcrun_error_t *err arg_unused)
{
  ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා
spin_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  const var_na *entrypoint_executable;

  නම් (container->container_def->process == NULL || container->container_def->process->args == NULL)
    ආපසු 0;

  entrypoint_executable = container->container_def->process->args[0];
  ආපසු strcmp (entrypoint_executable, "/") ? 0 : 1;
}

sa_nracanaa custom_handler_s handler_spin = {
  .name = "spin",
  .alias = NULL,
  .feature_string = "WASM:spin",
  .supports_open_tree_namespace = false,
  .load = spin_load,
  .unload = spin_unload,
  .run_func = spin_exec,
  .can_handle_container = spin_can_handle_container,
  .configure_container = spin_configure_container,
};

#endif
