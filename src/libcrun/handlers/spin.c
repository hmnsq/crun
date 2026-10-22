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

#ضمن <config.h>
#ضمن "../custom-handler.h"
#ضمن "../linux.h"
#ضمن <sys/stat.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_SPIN

#endif

#إذا HAVE_DLOPEN && HAVE_SPIN
ساكن صحيح
spin_exec (فراغ *cookie arg_unused, libcrun_container_t *container arg_unused,
           ثابت حرف *pathname arg_unused, حرف *ثابت argv[] arg_unused)
{
  // wasmtime fails to determine default config path if $HOME is not set
  حرف *newenviron[] = { "HOME=/root", عدم };
  حرف *newargv[] = { "/bin/spin", "up", "--listen", "0.0.0.0:80", عدم };

  // spin up needs a /tmp folder
  صحيح dir_result = mkdir ("/tmp", 0777);
  إذا (dir_result != 0 && errno != EEXIST)
    {
      error (EXIT_FAILURE, errno, "failed to execute mkdir `/tmp`");
    }

  execve (newargv[0], newargv, newenviron);
  perror ("execve");
  exit (EXIT_FAILURE);
}

ساكن صحيح
spin_load (فراغ **cookie arg_unused, libcrun_error_t *err)
{
  بنية stat st = { 0 };
  إذا (stat ("/usr/local/bin/spin", &st) == -1)
    {
      أرجع crun_make_error (err, 0, "Could not find statically linked spin cli at `/usr/local/bin/spin` on host file system");
    }
  أرجع 0;
}

ساكن صحيح
spin_configure_container (فراغ *cookie arg_unused, تعداد handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          ثابت حرف *rootfs arg_unused, libcrun_error_t *err)
{
  صحيح ret;
  إذا (phase != HANDLER_CONFIGURE_MOUNTS)
    أرجع 0;

  حرف *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/local/bin/spin", "/bin/spin", options, 5, err);
  إذا (ret != 0)
    أرجع ret;

  /* release any error if set since we are going to be returning from here */
  crun_error_release (err);

  أرجع 0;
}

ساكن صحيح
spin_unload (فراغ *cookie arg_unused, libcrun_error_t *err arg_unused)
{
  أرجع 0;
}

ساكن صحيح
spin_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  ثابت حرف *entrypoint_executable;

  إذا (container->container_def->process == عدم || container->container_def->process->args == عدم)
    أرجع 0;

  entrypoint_executable = container->container_def->process->args[0];
  أرجع strcmp (entrypoint_executable, "/") ? 0 : 1;
}

بنية custom_handler_s handler_spin = {
  .name = "spin",
  .alias = عدم,
  .feature_string = "WASM:spin",
  .supports_open_دەرەخ_namespace = false,
  .load = spin_load,
  .unload = spin_unload,
  .run_func = spin_exec,
  .can_handle_container = spin_can_handle_container,
  .configure_container = spin_configure_container,
};

#endif
