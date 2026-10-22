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

#만약 HAVE_DLOPEN && HAVE_SPIN
static 정수
spin_exec (void *cookie arg_unused, libcrun_container_t *container arg_unused,
           const char *pathname arg_unused, char *const argv[] arg_unused)
{
  // wasmtime fails to determine default config path if $HOME is not set
  char *newenviron[] = { "HOME=/root", NULL };
  char *newargv[] = { "/bin/spin", "up", "--listen", "0.0.0.0:80", NULL };

  // spin up needs a /tmp folder
  정수 dir_result = mkdir ("/tmp", 0777);
  만약 (dir_result != 0 && errno != EEXIST)
    {
      error (EXIT_FAILURE, errno, "failed to execute mkdir `/tmp`");
    }

  execve (newargv[0], newargv, newenviron);
  perror ("execve");
  exit (EXIT_FAILURE);
}

static 정수
spin_load (void **cookie arg_unused, libcrun_error_t *err)
{
  struct stat st = { 0 };
  만약 (stat ("/usr/local/bin/spin", &st) == -1)
    {
      반환 crun_make_error (err, 0, "Could not find statically linked spin cli at `/usr/local/bin/spin` on host file system");
    }
  반환 0;
}

static 정수
spin_configure_container (void *cookie arg_unused, enum handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          const char *rootfs arg_unused, libcrun_error_t *err)
{
  정수 ret;
  만약 (phase != HANDLER_CONFIGURE_MOUNTS)
    반환 0;

  char *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/local/bin/spin", "/bin/spin", options, 5, err);
  만약 (ret != 0)
    반환 ret;

  /* release any error if set since we are going to be returning from here */
  crun_error_release (err);

  반환 0;
}

static 정수
spin_unload (void *cookie arg_unused, libcrun_error_t *err arg_unused)
{
  반환 0;
}

static 정수
spin_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  const char *entrypoint_executable;

  만약 (container->container_def->process == NULL || container->container_def->process->args == NULL)
    반환 0;

  entrypoint_executable = container->container_def->process->args[0];
  반환 strcmp (entrypoint_executable, "/") ? 0 : 1;
}

struct custom_handler_s handler_spin = {
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
