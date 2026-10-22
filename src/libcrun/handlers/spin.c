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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "../custom-handler.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../linux.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DLOPEN
#  include <dlfcn.h>
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SPIN

#ਜੇ_ਅੰਤ

#ਜੇ HAVE_DLOPEN && HAVE_SPIN
ਸਥਿਰ ਪੂਰਨ
spin_exec (ਖਾਲੀ *cookie arg_ਅਣਵਰਤਿਆ, libcrun_container_t *container arg_ਅਣਵਰਤਿਆ,
           ਅਚਲ ਅੱਖਰ *pathname arg_ਅਣਵਰਤਿਆ, ਅੱਖਰ *ਅਚਲ argv[] arg_ਅਣਵਰਤਿਆ)
{
  // wasmtime fails to determine default config path if $HOME is not set
  ਅੱਖਰ *newenviron[] = { "HOME=/root", ਨਿਰਾ };
  ਅੱਖਰ *newargv[] = { "/bin/spin", "up", "--listen", "0.0.0.0:80", ਨਿਰਾ };

  // spin up needs a /tmp folder
  ਪੂਰਨ dir_ਨਤੀਜਾ = mkdir ("/tmp", 0777);
  ਜੇ (dir_ਨਤੀਜਾ != 0 && errno != EEXIST)
    {
      error (EXIT_FAILURE, errno, "failed to execute mkdir `/tmp`");
    }

  execve (newargv[0], newargv, newenviron);
  perror ("execve");
  ਬਾਹਰ_ਨਿਕਲੋ (EXIT_FAILURE);
}

ਸਥਿਰ ਪੂਰਨ
spin_ਲੱਦੋ (ਖਾਲੀ **cookie arg_ਅਣਵਰਤਿਆ, libcrun_error_t *err)
{
  ਬਣਤਰ stat st = { 0 };
  ਜੇ (stat ("/usr/local/bin/spin", &st) == -1)
    {
      ਵਾਪਸ crun_make_error (err, 0, "Could not find statically linked spin cli at `/usr/local/bin/spin` on host file system");
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
spin_configure_container (ਖਾਲੀ *cookie arg_ਅਣਵਰਤਿਆ, ਗਿਣਤੀ handler_configure_phase phase,
                          libcrun_context_t *context arg_ਅਣਵਰਤਿਆ, libcrun_container_t *container,
                          ਅਚਲ ਅੱਖਰ *rootfs arg_ਅਣਵਰਤਿਆ, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  ਜੇ (phase != HANDLER_CONFIGURE_MOUNTS)
    ਵਾਪਸ 0;

  ਅੱਖਰ *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/local/bin/spin", "/bin/spin", options, 5, err);
  ਜੇ (ret != 0)
    ਵਾਪਸ ret;

  /* release any error if set since we are going to be returning from here */
  crun_error_release (err);

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
spin_unload (ਖਾਲੀ *cookie arg_ਅਣਵਰਤਿਆ, libcrun_error_t *err arg_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
spin_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ ਅੱਖਰ *entrypoint_executable;

  ਜੇ (container->container_def->process == ਨਿਰਾ || container->container_def->process->ਦਲੀਲਾਂ == ਨਿਰਾ)
    ਵਾਪਸ 0;

  entrypoint_executable = container->container_def->process->ਦਲੀਲਾਂ[0];
  ਵਾਪਸ strcmp (entrypoint_executable, "/") ? 0 : 1;
}

ਬਣਤਰ custom_handler_s handler_spin = {
  .name = "spin",
  .alias = ਨਿਰਾ,
  .feature_string = "WASM:spin",
  .supports_open_ਰੁੱਖ_namespace = false,
  .ਲੱਦੋ = spin_ਲੱਦੋ,
  .unload = spin_unload,
  .ਲੜੀਵਾਰ_func = spin_exec,
  .can_handle_container = spin_can_handle_container,
  .configure_container = spin_configure_container,
};

#ਜੇ_ਅੰਤ
