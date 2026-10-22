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
স্থির পূর্ণাঙ্ক
spin_exec (শূন্য *cookie arg_unused, libcrun_container_t *container arg_unused,
           ধ্রুব বর্ণ *pathname arg_unused, বর্ণ *ধ্রুব argv[] arg_unused)
{
  // wasmtime fails to determine default config path if $HOME is not set
  বর্ণ *newenviron[] = { "HOME=/root", রিক্ত };
  বর্ণ *newargv[] = { "/bin/spin", "up", "--listen", "0.0.0.0:80", রিক্ত };

  // spin up needs a /tmp folder
  পূর্ণাঙ্ক dir_result = mkdir ("/tmp", 0777);
  যদি (dir_result != 0 && errno != EEXIST)
    {
      error (EXIT_FAILURE, errno, "failed to execute mkdir `/tmp`");
    }

  execve (newargv[0], newargv, newenviron);
  perror ("execve");
  exit (EXIT_FAILURE);
}

স্থির পূর্ণাঙ্ক
spin_load (শূন্য **cookie arg_unused, libcrun_error_t *err)
{
  সংরচনা stat st = { 0 };
  যদি (stat ("/usr/local/bin/spin", &st) == -1)
    {
      প্রত্যাগম crun_make_error (err, 0, "Could not find statically linked spin cli at `/usr/local/bin/spin` on host file system");
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
spin_configure_container (শূন্য *cookie arg_unused, গণনা handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          ধ্রুব বর্ণ *rootfs arg_unused, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  যদি (phase != HANDLER_CONFIGURE_MOUNTS)
    প্রত্যাগম 0;

  বর্ণ *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/local/bin/spin", "/bin/spin", options, 5, err);
  যদি (ret != 0)
    প্রত্যাগম ret;

  /* release any error if set since we are going to be returning from here */
  crun_error_release (err);

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
spin_unload (শূন্য *cookie arg_unused, libcrun_error_t *err arg_unused)
{
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
spin_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  ধ্রুব বর্ণ *entrypoint_executable;

  যদি (container->container_def->process == রিক্ত || container->container_def->process->args == রিক্ত)
    প্রত্যাগম 0;

  entrypoint_executable = container->container_def->process->args[0];
  প্রত্যাগম strcmp (entrypoint_executable, "/") ? 0 : 1;
}

সংরচনা custom_handler_s handler_spin = {
  .name = "spin",
  .alias = রিক্ত,
  .feature_string = "WASM:spin",
  .supports_open_বৃক্ষ_namespace = false,
  .load = spin_load,
  .unload = spin_unload,
  .run_func = spin_exec,
  .can_handle_container = spin_can_handle_container,
  .configure_container = spin_configure_container,
};

#endif
