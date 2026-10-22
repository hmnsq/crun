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
#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ "../custom-handler.h"
#అన్తర్భావ "../linux.h"
#అన్తర్భావ <sys/stat.h>

#నిర్వచితమైతే HAVE_DLOPEN
#  include <dlfcn.h>
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_SPIN

#అయితే_ముగింపు

#ఒకవేళ HAVE_DLOPEN && HAVE_SPIN
స్థిర పూర్ణాంకం
spin_exec (శూన్య *cookie arg_వాడని, libcrun_container_t *container arg_వాడని,
           స్థిరరాశి అక్షరం *pathname arg_వాడని, అక్షరం *స్థిరరాశి argv[] arg_వాడని)
{
  // wasmtime fails to determine default config path if $HOME is not set
  అక్షరం *newenviron[] = { "HOME=/root", ఖాళీ };
  అక్షరం *newargv[] = { "/bin/spin", "up", "--listen", "0.0.0.0:80", ఖాళీ };

  // spin up needs a /tmp folder
  పూర్ణాంకం dir_ఫలితం = mkdir ("/tmp", 0777);
  ఒకవేళ (dir_ఫలితం != 0 && errno != EEXIST)
    {
      error (EXIT_FAILURE, errno, "failed to execute mkdir `/tmp`");
    }

  execve (newargv[0], newargv, newenviron);
  perror ("execve");
  నిష్క్రమించు (EXIT_FAILURE);
}

స్థిర పూర్ణాంకం
spin_ఎక్కించు (శూన్య **cookie arg_వాడని, libcrun_error_t *err)
{
  నిర్మాణం stat st = { 0 };
  ఒకవేళ (stat ("/usr/local/bin/spin", &st) == -1)
    {
      తిరిగి crun_make_error (err, 0, "Could not find statically linked spin cli at `/usr/local/bin/spin` on host file system");
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
spin_configure_container (శూన్య *cookie arg_వాడని, గణన handler_configure_phase phase,
                          libcrun_context_t *context arg_వాడని, libcrun_container_t *container,
                          స్థిరరాశి అక్షరం *rootfs arg_వాడని, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  ఒకవేళ (phase != HANDLER_CONFIGURE_MOUNTS)
    తిరిగి 0;

  అక్షరం *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/local/bin/spin", "/bin/spin", options, 5, err);
  ఒకవేళ (ret != 0)
    తిరిగి ret;

  /* release any error if set since we are going to be returning from here */
  crun_error_release (err);

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
spin_unload (శూన్య *cookie arg_వాడని, libcrun_error_t *err arg_వాడని)
{
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
spin_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_వాడని)
{
  స్థిరరాశి అక్షరం *entrypoint_executable;

  ఒకవేళ (container->container_def->process == ఖాళీ || container->container_def->process->వాదనలు == ఖాళీ)
    తిరిగి 0;

  entrypoint_executable = container->container_def->process->వాదనలు[0];
  తిరిగి strcmp (entrypoint_executable, "/") ? 0 : 1;
}

నిర్మాణం custom_handler_s handler_spin = {
  .name = "spin",
  .alias = ఖాళీ,
  .feature_string = "WASM:spin",
  .supports_open_వృక్షం_namespace = false,
  .ఎక్కించు = spin_ఎక్కించు,
  .unload = spin_unload,
  .నిరంతరం_func = spin_exec,
  .can_handle_container = spin_can_handle_container,
  .configure_container = spin_configure_container,
};

#అయితే_ముగింపు
