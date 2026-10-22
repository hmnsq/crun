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
#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश "../custom-handler.h"
#समावेश "../linux.h"
#समावेश <sys/stat.h>

#अगर_घोषित HAVE_DLOPEN
#  include <dlfcn.h>
#पूर्ण_अगर

#अगर_घोषित HAVE_SPIN

#पूर्ण_अगर

#अगर HAVE_DLOPEN && HAVE_SPIN
अटल पूर्णांक
spin_exec (व्योम *cookie arg_unused, libcrun_container_t *container arg_unused,
           स्थिर अक्षर *pathname arg_unused, अक्षर *स्थिर argv[] arg_unused)
{
  // wasmtime fails to determine default config path if $HOME is not set
  अक्षर *newenviron[] = { "HOME=/root", शून्य };
  अक्षर *newargv[] = { "/bin/spin", "up", "--listen", "0.0.0.0:80", शून्य };

  // spin up needs a /tmp folder
  पूर्णांक dir_परिणाम = सूची_गढ़ो ("/tmp", 0777);
  अगर (dir_परिणाम != 0 && त्रुटि_सं != EEXIST)
    {
      error (निकास_त्रुटि, त्रुटि_सं, "failed to execute mkdir `/tmp`");
    }

  execve (newargv[0], newargv, newenviron);
  लिखो_त्रुटि ("execve");
  निकास (निकास_त्रुटि);
}

अटल पूर्णांक
spin_load (व्योम **cookie arg_unused, libcrun_error_t *err)
{
  काष्ठा stat st = { 0 };
  अगर (stat ("/usr/local/bin/spin", &st) == -1)
    {
      वापस crun_make_error (err, 0, "Could not find statically linked spin cli at `/usr/local/bin/spin` on host file system");
    }
  वापस 0;
}

अटल पूर्णांक
spin_configure_container (व्योम *cookie arg_unused, क्रमागत handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          स्थिर अक्षर *rootfs arg_unused, libcrun_error_t *err)
{
  पूर्णांक ret;
  अगर (phase != HANDLER_CONFIGURE_MOUNTS)
    वापस 0;

  अक्षर *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/local/bin/spin", "/bin/spin", options, 5, err);
  अगर (ret != 0)
    वापस ret;

  /* release any error if set since we are going to be returning from here */
  crun_error_release (err);

  वापस 0;
}

अटल पूर्णांक
spin_unload (व्योम *cookie arg_unused, libcrun_error_t *err arg_unused)
{
  वापस 0;
}

अटल पूर्णांक
spin_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  स्थिर अक्षर *entrypoint_executable;

  अगर (container->container_def->process == शून्य || container->container_def->process->args == शून्य)
    वापस 0;

  entrypoint_executable = container->container_def->process->args[0];
  वापस म_भेद (entrypoint_executable, "/") ? 0 : 1;
}

काष्ठा custom_handler_s handler_spin = {
  .name = "spin",
  .alias = शून्य,
  .feature_string = "WASM:spin",
  .supports_open_वृक्ष_namespace = false,
  .load = spin_load,
  .unload = spin_unload,
  .run_func = spin_exec,
  .can_handle_container = spin_can_handle_container,
  .configure_container = spin_configure_container,
};

#पूर्ण_अगर
