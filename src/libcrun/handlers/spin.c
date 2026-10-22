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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "../custom-handler.h"
#অন্তর্ভাব "../linux.h"
#অন্তর্ভাব <sys/stat.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_SPIN

#endif

#যদি HAVE_DLOPEN && HAVE_SPIN
স্থির পূর্ণসংখ্যা
spin_exec (শূন্য *cookie arg_unused, libcrun_container_t *container arg_unused,
           ধ্রুবক অক্ষর *pathname arg_unused, অক্ষর *ধ্রুবক argv[] arg_unused)
{
  // wasmtime fails to determine default config path if $HOME is not set
  অক্ষর *newenviron[] = { "HOME=/root", ফাঁকা };
  অক্ষর *newargv[] = { "/bin/spin", "up", "--listen", "0.0.0.0:80", ফাঁকা };

  // spin up needs a /tmp folder
  পূর্ণসংখ্যা dir_ফলাফল = mkdir ("/tmp", 0777);
  যদি (dir_ফলাফল != 0 && errno != EEXIST)
    {
      error (EXIT_FAILURE, errno, "failed to execute mkdir `/tmp`");
    }

  execve (newargv[0], newargv, newenviron);
  perror ("execve");
  exit (EXIT_FAILURE);
}

স্থির পূর্ণসংখ্যা
spin_load (শূন্য **cookie arg_unused, libcrun_error_t *err)
{
  কাঠামো stat st = { 0 };
  যদি (stat ("/usr/local/bin/spin", &st) == -1)
    {
      ফেরত crun_make_error (err, 0, "Could not find statically linked spin cli at `/usr/local/bin/spin` on host file system");
    }
  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
spin_configure_container (শূন্য *cookie arg_unused, গণনা handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          ধ্রুবক অক্ষর *rootfs arg_unused, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;
  যদি (phase != HANDLER_CONFIGURE_MOUNTS)
    ফেরত 0;

  অক্ষর *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/local/bin/spin", "/bin/spin", options, 5, err);
  যদি (ret != 0)
    ফেরত ret;

  /* release any error if set since we are going to be returning from here */
  crun_error_release (err);

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
spin_unload (শূন্য *cookie arg_unused, libcrun_error_t *err arg_unused)
{
  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
spin_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  ধ্রুবক অক্ষর *entrypoint_executable;

  যদি (container->container_def->process == ফাঁকা || container->container_def->process->args == ফাঁকা)
    ফেরত 0;

  entrypoint_executable = container->container_def->process->args[0];
  ফেরত strcmp (entrypoint_executable, "/") ? 0 : 1;
}

কাঠামো custom_handler_s handler_spin = {
  .name = "spin",
  .alias = ফাঁকা,
  .feature_string = "WASM:spin",
  .supports_open_বৃক্ষ_namespace = false,
  .load = spin_load,
  .unload = spin_unload,
  .run_func = spin_exec,
  .can_handle_container = spin_can_handle_container,
  .configure_container = spin_configure_container,
};

#endif
