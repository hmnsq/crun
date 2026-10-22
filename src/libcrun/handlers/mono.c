/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019, 2020, 2021 Giuseppe Scrivano <giuseppe@scrivano.org>
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
#समावेश "../container.h"
#समावेश "../utils.h"
#समावेश "../linux.h"
#समावेश <unistd.h>
#समावेश <sys/stat.h>
#समावेश <errno.h>
#समावेश <sys/types.h>
#समावेश <fcntl.h>
#समावेश <sched.h>

#अगर_घोषित HAVE_DLOPEN
#  include <dlfcn.h>
#पूर्ण_अगर

#अगर_घोषित HAVE_MONO
#  include <mono/metadata/environment.h>
#  include <mono/utils/mono-publib.h>
#  include <mono/metadata/mono-config.h>
#  include <mono/jit/jit.h>
#पूर्ण_अगर

#अगर HAVE_DLOPEN && HAVE_MONO
अटल पूर्णांक
mono_exec (व्योम *cookie arg_unused, libcrun_container_t *container arg_unused,
           स्थिर अक्षर *pathname, अक्षर *स्थिर argv[] arg_unused)
{
  MonoDomain *domain;
  अक्षर *path = (अक्षर *) pathname;
  पूर्णांक argc = 2;
  अक्षर *argv_mono[] = {
    path,
    path,
    शून्य
  };
  स्थिर अक्षर *file;
  पूर्णांक retval;

  file = argv_mono[1];

  MonoAllocatorVTable mem_vtable = { MONO_ALLOCATOR_VTABLE_VERSION, xmalloc, शून्य, शून्य, शून्य };
  mono_set_allocator_vtable (&mem_vtable);

  /*
   * Load the default Mono configuration file, this is needed
   * if you are planning on using the dllmaps defined on the
   * system configuration
   */
  mono_config_parse (शून्य);
  /*
   * mono_jit_init() creates a domain: each assembly is
   * loaded and run in a MonoDomain.
   */
  domain = mono_jit_init (file);

  /*
   * We add our special internal call, so that C# code
   * can call us back.
   */

  MonoAssembly *assembly;
  assembly = mono_domain_assembly_open (domain, file);
  अगर (! assembly)
    निकास (निकास_त्रुटि);
  /*
   * mono_jit_exec() will run the Main() method in the assembly.
   * The return value needs to be looked up from
   * System.Environment.ExitCode.
   */
  mono_jit_exec (domain, assembly, argc - 1, argv_mono + 1);
  retval = mono_environment_exitcode_get ();
  mono_jit_cleanup (domain);
  वापस 0;
}

अटल पूर्णांक
mono_load (व्योम **cookie, libcrun_error_t *err)
{
  व्योम *handle;

  handle = dlopen ("libmono-native.so", RTLD_NOW);
  अगर (handle == शून्य)
    वापस crun_make_error (err, 0, "could not load `libmono-native.so`: `%s`", dlerror ());
  *cookie = handle;

  वापस 0;
}

अटल पूर्णांक
mono_configure_container (व्योम *cookie arg_unused, क्रमागत handler_configure_phase phase,
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

  ret = libcrun_container_do_bind_mount (container, "/etc/mono", "/etc/mono", options, 5, err);
  अगर (ret != 0)
    वापस ret;

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/mono", "/usr/lib/mono", options, 5, err);
  अगर (ret != 0)
    वापस ret;

  वापस 0;
}

अटल पूर्णांक
mono_unload (व्योम *cookie, libcrun_error_t *err)
{
  पूर्णांक r;

  अगर (cookie)
    {
      r = dlclose (cookie);
      अगर (UNLIKELY (r < 0))
        वापस crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  वापस 0;
}

अटल पूर्णांक
mono_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  स्थिर अक्षर *annotation;

  annotation = find_annotation (container, "run.oci.handler");
  अगर (annotation)
    वापस म_भेद (annotation, "dotnet") == 0 ? 1 : 0;

  वापस 0;
}

काष्ठा custom_handler_s handler_mono = {
  .name = "dotnet",
  .alias = शून्य,
  .feature_string = ".NET:mono",
  .supports_open_वृक्ष_namespace = false,
  .load = mono_load,
  .unload = mono_unload,
  .run_func = mono_exec,
  .can_handle_container = mono_can_handle_container,
  .configure_container = mono_configure_container,
};

#पूर्ण_अगर
