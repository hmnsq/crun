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
#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "../custom-handler.h"
#ಅನ್ತರ್ಭಾವ "../linux.h"
#ಅನ್ತರ್ಭಾವ <sys/stat.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DLOPEN
#  include <dlfcn.h>
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SPIN

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ HAVE_DLOPEN && HAVE_SPIN
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
spin_exec (ಶೂನ್ಯ *cookie arg_ಬಳಸದ, libcrun_container_t *container arg_ಬಳಸದ,
           ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname arg_ಬಳಸದ, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ argv[] arg_ಬಳಸದ)
{
  // wasmtime fails to determine default config path if $HOME is not set
  ಅಕ್ಷರ *newenviron[] = { "HOME=/root", ಖಾಲಿ };
  ಅಕ್ಷರ *newargv[] = { "/bin/spin", "up", "--listen", "0.0.0.0:80", ಖಾಲಿ };

  // spin up needs a /tmp folder
  ಪೂರ್ಣಾಂಕ dir_ಫಲಿತಾಂಶ = mkdir ("/tmp", 0777);
  ಆದರೆ (dir_ಫಲಿತಾಂಶ != 0 && errno != EEXIST)
    {
      error (EXIT_FAILURE, errno, "failed to execute mkdir `/tmp`");
    }

  execve (newargv[0], newargv, newenviron);
  perror ("execve");
  ನಿರ್ಗಮಿಸು (EXIT_FAILURE);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
spin_ತುಂಬು (ಶೂನ್ಯ **cookie arg_ಬಳಸದ, libcrun_error_t *err)
{
  ರಚನೆ stat st = { 0 };
  ಆದರೆ (stat ("/usr/local/bin/spin", &st) == -1)
    {
      ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "Could not find statically linked spin cli at `/usr/local/bin/spin` on host file system");
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
spin_configure_container (ಶೂನ್ಯ *cookie arg_ಬಳಸದ, ಎಣಿಕೆ handler_configure_phase phase,
                          libcrun_context_t *context arg_ಬಳಸದ, libcrun_container_t *container,
                          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs arg_ಬಳಸದ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  ಆದರೆ (phase != HANDLER_CONFIGURE_MOUNTS)
    ಹಿಂತಿರುಗಿ 0;

  ಅಕ್ಷರ *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/local/bin/spin", "/bin/spin", options, 5, err);
  ಆದರೆ (ret != 0)
    ಹಿಂತಿರುಗಿ ret;

  /* release any error if set since we are going to be returning from here */
  crun_error_release (err);

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
spin_unload (ಶೂನ್ಯ *cookie arg_ಬಳಸದ, libcrun_error_t *err arg_ಬಳಸದ)
{
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
spin_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_ಬಳಸದ)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *entrypoint_executable;

  ಆದರೆ (container->container_def->process == ಖಾಲಿ || container->container_def->process->ವಾದಗಳು == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  entrypoint_executable = container->container_def->process->ವಾದಗಳು[0];
  ಹಿಂತಿರುಗಿ strcmp (entrypoint_executable, "/") ? 0 : 1;
}

ರಚನೆ custom_handler_s handler_spin = {
  .name = "spin",
  .alias = ಖಾಲಿ,
  .feature_string = "WASM:spin",
  .supports_open_ವೃಕ್ಷ_namespace = false,
  .ತುಂಬು = spin_ತುಂಬು,
  .unload = spin_unload,
  .ಸತತ_func = spin_exec,
  .can_handle_container = spin_can_handle_container,
  .configure_container = spin_configure_container,
};

#ಆದರೆ_ಅಂತ್ಯ
