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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "../custom-handler.h"
#அந்தர்பாவ "../linux.h"
#அந்தர்பாவ <sys/stat.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_SPIN

#endif

#எனில் HAVE_DLOPEN && HAVE_SPIN
நிலையான முழு
spin_exec (வெறுமை *cookie arg_unused, libcrun_container_t *container arg_unused,
           மாறிலி எழுத்து *pathname arg_unused, எழுத்து *மாறிலி argv[] arg_unused)
{
  // wasmtime fails to determine default config path if $HOME is not set
  எழுத்து *newenviron[] = { "HOME=/root", வெற்று };
  எழுத்து *newargv[] = { "/bin/spin", "up", "--listen", "0.0.0.0:80", வெற்று };

  // spin up needs a /tmp folder
  முழு dir_result = mkdir ("/tmp", 0777);
  எனில் (dir_result != 0 && errno != EEXIST)
    {
      error (EXIT_FAILURE, errno, "failed to execute mkdir `/tmp`");
    }

  execve (newargv[0], newargv, newenviron);
  perror ("execve");
  exit (EXIT_FAILURE);
}

நிலையான முழு
spin_load (வெறுமை **cookie arg_unused, libcrun_error_t *err)
{
  கட்டமைப்பு stat st = { 0 };
  எனில் (stat ("/usr/local/bin/spin", &st) == -1)
    {
      திரும்பு crun_make_error (err, 0, "Could not find statically linked spin cli at `/usr/local/bin/spin` on host file system");
    }
  திரும்பு 0;
}

நிலையான முழு
spin_configure_container (வெறுமை *cookie arg_unused, எண்ணிக்கை handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          மாறிலி எழுத்து *rootfs arg_unused, libcrun_error_t *err)
{
  முழு ret;
  எனில் (phase != HANDLER_CONFIGURE_MOUNTS)
    திரும்பு 0;

  எழுத்து *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/local/bin/spin", "/bin/spin", options, 5, err);
  எனில் (ret != 0)
    திரும்பு ret;

  /* release any error if set since we are going to be returning from here */
  crun_error_release (err);

  திரும்பு 0;
}

நிலையான முழு
spin_unload (வெறுமை *cookie arg_unused, libcrun_error_t *err arg_unused)
{
  திரும்பு 0;
}

நிலையான முழு
spin_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  மாறிலி எழுத்து *entrypoint_executable;

  எனில் (container->container_def->process == வெற்று || container->container_def->process->args == வெற்று)
    திரும்பு 0;

  entrypoint_executable = container->container_def->process->args[0];
  திரும்பு strcmp (entrypoint_executable, "/") ? 0 : 1;
}

கட்டமைப்பு custom_handler_s handler_spin = {
  .name = "spin",
  .alias = வெற்று,
  .feature_string = "WASM:spin",
  .supports_open_மரம்_namespace = false,
  .load = spin_load,
  .unload = spin_unload,
  .run_func = spin_exec,
  .can_handle_container = spin_can_handle_container,
  .configure_container = spin_configure_container,
};

#endif
