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

#अन्तर्भाव <config.h>
#अन्तर्भाव "../custom-handler.h"
#अन्तर्भाव "../linux.h"
#अन्तर्भाव <sys/stat.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_SPIN

#endif

#जइ HAVE_DLOPEN && HAVE_SPIN
स्थिर पूर्णाङ्क
spin_exec (शून्य *cookie arg_unused, libcrun_container_t *container arg_unused,
           ध्रुव वर्ण *pathname arg_unused, वर्ण *ध्रुव argv[] arg_unused)
{
  // wasmtime fails to determine default config path if $HOME is not set
  वर्ण *newenviron[] = { "HOME=/root", रिक्त };
  वर्ण *newargv[] = { "/bin/spin", "up", "--listen", "0.0.0.0:80", रिक्त };

  // spin up needs a /tmp folder
  पूर्णाङ्क dir_result = mkdir ("/tmp", 0777);
  जइ (dir_result != 0 && errno != EEXIST)
    {
      error (EXIT_FAILURE, errno, "failed to execute mkdir `/tmp`");
    }

  execve (newargv[0], newargv, newenviron);
  perror ("execve");
  exit (EXIT_FAILURE);
}

स्थिर पूर्णाङ्क
spin_load (शून्य **cookie arg_unused, libcrun_error_t *err)
{
  संरचना stat st = { 0 };
  जइ (stat ("/usr/local/bin/spin", &st) == -1)
    {
      प्रत्यागम crun_make_error (err, 0, "Could not find statically linked spin cli at `/usr/local/bin/spin` on host file system");
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
spin_configure_container (शून्य *cookie arg_unused, गणना handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          ध्रुव वर्ण *rootfs arg_unused, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  जइ (phase != HANDLER_CONFIGURE_MOUNTS)
    प्रत्यागम 0;

  वर्ण *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/local/bin/spin", "/bin/spin", options, 5, err);
  जइ (ret != 0)
    प्रत्यागम ret;

  /* release any error if set since we are going to be returning from here */
  crun_error_release (err);

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
spin_unload (शून्य *cookie arg_unused, libcrun_error_t *err arg_unused)
{
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
spin_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  ध्रुव वर्ण *entrypoint_executable;

  जइ (container->container_def->process == रिक्त || container->container_def->process->args == रिक्त)
    प्रत्यागम 0;

  entrypoint_executable = container->container_def->process->args[0];
  प्रत्यागम strcmp (entrypoint_executable, "/") ? 0 : 1;
}

संरचना custom_handler_s handler_spin = {
  .name = "spin",
  .alias = रिक्त,
  .feature_string = "WASM:spin",
  .supports_open_वृक्षः_namespace = false,
  .load = spin_load,
  .unload = spin_unload,
  .run_func = spin_exec,
  .can_handle_container = spin_can_handle_container,
  .configure_container = spin_configure_container,
};

#endif
