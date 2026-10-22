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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "../custom-handler.h"
#ଅନ୍ତର୍ଭାଵ "../linux.h"
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_SPIN

#endif

#ଯଦି HAVE_DLOPEN && HAVE_SPIN
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
spin_exec (ଶୂନ୍ୟ *cookie arg_unused, libcrun_container_t *container arg_unused,
           ଅଚଳ ଅକ୍ଷର *pathname arg_unused, ଅକ୍ଷର *ଅଚଳ argv[] arg_unused)
{
  // wasmtime fails to determine default config path if $HOME is not set
  ଅକ୍ଷର *newenviron[] = { "HOME=/root", ଖାଲି };
  ଅକ୍ଷର *newargv[] = { "/bin/spin", "up", "--listen", "0.0.0.0:80", ଖାଲି };

  // spin up needs a /tmp folder
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା dir_result = mkdir ("/tmp", 0777);
  ଯଦି (dir_result != 0 && errno != EEXIST)
    {
      error (EXIT_FAILURE, errno, "failed to execute mkdir `/tmp`");
    }

  execve (newargv[0], newargv, newenviron);
  perror ("execve");
  exit (EXIT_FAILURE);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
spin_load (ଶୂନ୍ୟ **cookie arg_unused, libcrun_error_t *err)
{
  ସଂରଚନା stat st = { 0 };
  ଯଦି (stat ("/usr/local/bin/spin", &st) == -1)
    {
      ଫେରନ୍ତୁ crun_make_error (err, 0, "Could not find statically linked spin cli at `/usr/local/bin/spin` on host file system");
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
spin_configure_container (ଶୂନ୍ୟ *cookie arg_unused, ଗଣନା handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          ଅଚଳ ଅକ୍ଷର *rootfs arg_unused, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ଯଦି (phase != HANDLER_CONFIGURE_MOUNTS)
    ଫେରନ୍ତୁ 0;

  ଅକ୍ଷର *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/local/bin/spin", "/bin/spin", options, 5, err);
  ଯଦି (ret != 0)
    ଫେରନ୍ତୁ ret;

  /* release any error if set since we are going to be returning from here */
  crun_error_release (err);

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
spin_unload (ଶୂନ୍ୟ *cookie arg_unused, libcrun_error_t *err arg_unused)
{
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
spin_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  ଅଚଳ ଅକ୍ଷର *entrypoint_executable;

  ଯଦି (container->container_def->process == ଖାଲି || container->container_def->process->args == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  entrypoint_executable = container->container_def->process->args[0];
  ଫେରନ୍ତୁ strcmp (entrypoint_executable, "/") ? 0 : 1;
}

ସଂରଚନା custom_handler_s handler_spin = {
  .name = "spin",
  .alias = ଖାଲି,
  .feature_string = "WASM:spin",
  .supports_open_ବୃକ୍ଷ_namespace = false,
  .load = spin_load,
  .unload = spin_unload,
  .run_func = spin_exec,
  .can_handle_container = spin_can_handle_container,
  .configure_container = spin_configure_container,
};

#endif
