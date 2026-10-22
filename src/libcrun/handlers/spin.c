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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "../custom-handler.h"
#ಅನ್ತರ್ಭಾವ "../linux.h"
#ಅನ್ತರ್ಭಾವ <sys/stat.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_SPIN

#endif

#ಯದಿ HAVE_DLOPEN && HAVE_SPIN
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
spin_exec (ಶೂನ್ಯ *cookie arg_unused, libcrun_container_t *container arg_unused,
           ಧ್ರುವ ವರ್ಣ *pathname arg_unused, ವರ್ಣ *ಧ್ರುವ argv[] arg_unused)
{
  // wasmtime fails to determine default config path if $HOME is not set
  ವರ್ಣ *newenviron[] = { "HOME=/root", ರಿಕ್ತ };
  ವರ್ಣ *newargv[] = { "/bin/spin", "up", "--listen", "0.0.0.0:80", ರಿಕ್ತ };

  // spin up needs a /tmp folder
  ಪೂರ್ಣಾಙ್ಕ dir_result = mkdir ("/tmp", 0777);
  ಯದಿ (dir_result != 0 && errno != EEXIST)
    {
      error (EXIT_FAILURE, errno, "failed to execute mkdir `/tmp`");
    }

  execve (newargv[0], newargv, newenviron);
  perror ("execve");
  exit (EXIT_FAILURE);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
spin_load (ಶೂನ್ಯ **cookie arg_unused, libcrun_error_t *err)
{
  ಸಂರಚನಾ stat st = { 0 };
  ಯದಿ (stat ("/usr/local/bin/spin", &st) == -1)
    {
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "Could not find statically linked spin cli at `/usr/local/bin/spin` on host file system");
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
spin_configure_container (ಶೂನ್ಯ *cookie arg_unused, ಗಣನಾ handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          ಧ್ರುವ ವರ್ಣ *rootfs arg_unused, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಯದಿ (phase != HANDLER_CONFIGURE_MOUNTS)
    ಪ್ರತ್ಯಾಗಮ 0;

  ವರ್ಣ *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/local/bin/spin", "/bin/spin", options, 5, err);
  ಯದಿ (ret != 0)
    ಪ್ರತ್ಯಾಗಮ ret;

  /* release any error if set since we are going to be returning from here */
  crun_error_release (err);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
spin_unload (ಶೂನ್ಯ *cookie arg_unused, libcrun_error_t *err arg_unused)
{
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
spin_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  ಧ್ರುವ ವರ್ಣ *entrypoint_executable;

  ಯದಿ (container->container_def->process == ರಿಕ್ತ || container->container_def->process->args == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  entrypoint_executable = container->container_def->process->args[0];
  ಪ್ರತ್ಯಾಗಮ strcmp (entrypoint_executable, "/") ? 0 : 1;
}

ಸಂರಚನಾ custom_handler_s handler_spin = {
  .name = "spin",
  .alias = ರಿಕ್ತ,
  .feature_string = "WASM:spin",
  .supports_open_ವೃಕ್ಷ_namespace = false,
  .load = spin_load,
  .unload = spin_unload,
  .run_func = spin_exec,
  .can_handle_container = spin_can_handle_container,
  .configure_container = spin_configure_container,
};

#endif
