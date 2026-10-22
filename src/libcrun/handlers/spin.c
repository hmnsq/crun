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
#تعریف_کرو GNU_ماخذ

#شامل <config.h>
#شامل "../custom-handler.h"
#شامل "../linux.h"
#شامل <sys/stat.h>

#اگر_تعریف_شدہ HAVE_DLOPEN
#  include <dlfcn.h>
#اگر_ختم

#اگر_تعریف_شدہ HAVE_SPIN

#اگر_ختم

#اگر HAVE_DLOPEN && HAVE_SPIN
ساکن صحیح
spin_exec (خالی *cookie arg_غیر_مستعمل, libcrun_container_t *container arg_غیر_مستعمل,
           ثابت حرف *pathname arg_غیر_مستعمل, حرف *ثابت argv[] arg_غیر_مستعمل)
{
  // wasmtime fails to determine default config path if $HOME is not set
  حرف *newenviron[] = { "HOME=/root", تہی };
  حرف *newargv[] = { "/bin/spin", "up", "--listen", "0.0.0.0:80", تہی };

  // spin up needs a /tmp folder
  صحیح dir_نتیجہ = mkdir ("/tmp", 0777);
  اگر (dir_نتیجہ != 0 && errno != EEXIST)
    {
      error (EXIT_FAILURE, errno, "failed to execute mkdir `/tmp`");
    }

  execve (newargv[0], newargv, newenviron);
  perror ("execve");
  باہر_نکلو (EXIT_FAILURE);
}

ساکن صحیح
spin_بھرو (خالی **cookie arg_غیر_مستعمل, libcrun_error_t *err)
{
  ساخت stat st = { 0 };
  اگر (stat ("/usr/local/bin/spin", &st) == -1)
    {
      واپس crun_make_error (err, 0, "Could not find statically linked spin cli at `/usr/local/bin/spin` on host file system");
    }
  واپس 0;
}

ساکن صحیح
spin_configure_container (خالی *cookie arg_غیر_مستعمل, شمار handler_configure_phase phase,
                          libcrun_context_t *context arg_غیر_مستعمل, libcrun_container_t *container,
                          ثابت حرف *rootfs arg_غیر_مستعمل, libcrun_error_t *err)
{
  صحیح ret;
  اگر (phase != HANDLER_CONFIGURE_MOUNTS)
    واپس 0;

  حرف *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/local/bin/spin", "/bin/spin", options, 5, err);
  اگر (ret != 0)
    واپس ret;

  /* release any error if set since we are going to be returning from here */
  crun_error_release (err);

  واپس 0;
}

ساکن صحیح
spin_unload (خالی *cookie arg_غیر_مستعمل, libcrun_error_t *err arg_غیر_مستعمل)
{
  واپس 0;
}

ساکن صحیح
spin_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_غیر_مستعمل)
{
  ثابت حرف *entrypoint_executable;

  اگر (container->container_def->process == تہی || container->container_def->process->دلائل == تہی)
    واپس 0;

  entrypoint_executable = container->container_def->process->دلائل[0];
  واپس strcmp (entrypoint_executable, "/") ? 0 : 1;
}

ساخت custom_handler_s handler_spin = {
  .name = "spin",
  .alias = تہی,
  .feature_string = "WASM:spin",
  .supports_open_درخت_namespace = false,
  .بھرو = spin_بھرو,
  .unload = spin_unload,
  .تسلسل_func = spin_exec,
  .can_handle_container = spin_can_handle_container,
  .configure_container = spin_configure_container,
};

#اگر_ختم
