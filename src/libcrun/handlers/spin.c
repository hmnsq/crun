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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "../custom-handler.h"
#അന്തര്ഭാവ "../linux.h"
#അന്തര്ഭാവ <sys/stat.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_SPIN

#endif

#എങ്കിൽ HAVE_DLOPEN && HAVE_SPIN
സ്ഥിതം പൂർണ്ണസംഖ്യ
spin_exec (ശൂന്യം *cookie arg_unused, libcrun_container_t *container arg_unused,
           സ്ഥിരം അക്ഷരം *pathname arg_unused, അക്ഷരം *സ്ഥിരം argv[] arg_unused)
{
  // wasmtime fails to determine default config path if $HOME is not set
  അക്ഷരം *newenviron[] = { "HOME=/root", ശൂന്യ };
  അക്ഷരം *newargv[] = { "/bin/spin", "up", "--listen", "0.0.0.0:80", ശൂന്യ };

  // spin up needs a /tmp folder
  പൂർണ്ണസംഖ്യ dir_result = mkdir ("/tmp", 0777);
  എങ്കിൽ (dir_result != 0 && errno != EEXIST)
    {
      error (EXIT_FAILURE, errno, "failed to execute mkdir `/tmp`");
    }

  execve (newargv[0], newargv, newenviron);
  perror ("execve");
  exit (EXIT_FAILURE);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
spin_load (ശൂന്യം **cookie arg_unused, libcrun_error_t *err)
{
  ഘടന stat st = { 0 };
  എങ്കിൽ (stat ("/usr/local/bin/spin", &st) == -1)
    {
      തിരികെ crun_make_error (err, 0, "Could not find statically linked spin cli at `/usr/local/bin/spin` on host file system");
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
spin_configure_container (ശൂന്യം *cookie arg_unused, എണ്ണൽ handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          സ്ഥിരം അക്ഷരം *rootfs arg_unused, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  എങ്കിൽ (phase != HANDLER_CONFIGURE_MOUNTS)
    തിരികെ 0;

  അക്ഷരം *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/local/bin/spin", "/bin/spin", options, 5, err);
  എങ്കിൽ (ret != 0)
    തിരികെ ret;

  /* release any error if set since we are going to be returning from here */
  crun_error_release (err);

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
spin_unload (ശൂന്യം *cookie arg_unused, libcrun_error_t *err arg_unused)
{
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
spin_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  സ്ഥിരം അക്ഷരം *entrypoint_executable;

  എങ്കിൽ (container->container_def->process == ശൂന്യ || container->container_def->process->args == ശൂന്യ)
    തിരികെ 0;

  entrypoint_executable = container->container_def->process->args[0];
  തിരികെ strcmp (entrypoint_executable, "/") ? 0 : 1;
}

ഘടന custom_handler_s handler_spin = {
  .name = "spin",
  .alias = ശൂന്യ,
  .feature_string = "WASM:spin",
  .supports_open_വൃക്ഷം_namespace = false,
  .load = spin_load,
  .unload = spin_unload,
  .run_func = spin_exec,
  .can_handle_container = spin_can_handle_container,
  .configure_container = spin_configure_container,
};

#endif
