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
#define _GNU_SOURCE

#अन्तर्भाव <config.h>
#अन्तर्भाव "../custom-handler.h"
#अन्तर्भाव "../container.h"
#अन्तर्भाव "../utils.h"
#अन्तर्भाव "../linux.h"
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_MONO
#  include <mono/metadata/environment.h>
#  include <mono/utils/mono-publib.h>
#  include <mono/metadata/mono-config.h>
#  include <mono/jit/jit.h>
#endif

#यदि HAVE_DLOPEN && HAVE_MONO
स्थिर पूर्णाङ्क
mono_exec (शून्य *cookie arg_unused, libcrun_container_t *container arg_unused,
           अचर वर्ण *pathname, वर्ण *अचर argv[] arg_unused)
{
  MonoDomain *domain;
  वर्ण *path = (वर्ण *) pathname;
  पूर्णाङ्क argc = 2;
  वर्ण *argv_mono[] = {
    path,
    path,
    रिक्त
  };
  अचर वर्ण *file;
  पूर्णाङ्क retval;

  file = argv_mono[1];

  MonoAllocatorVTable mem_vtable = { MONO_ALLOCATOR_VTABLE_VERSION, xmalloc, रिक्त, रिक्त, रिक्त };
  mono_set_allocator_vtable (&mem_vtable);

  /*
   * Load the default Mono configuration file, this is needed
   * if you are planning on using the dllmaps defined on the
   * system configuration
   */
  mono_config_parse (रिक्त);
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
  यदि (! assembly)
    निकास (निकास_त्रुटि);
  /*
   * mono_jit_exec() will run the Main() method in the assembly.
   * The return value needs to be looked up from
   * System.Environment.ExitCode.
   */
  mono_jit_exec (domain, assembly, argc - 1, argv_mono + 1);
  retval = mono_environment_exitcode_get ();
  mono_jit_cleanup (domain);
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
mono_load (शून्य **cookie, libcrun_error_t *err)
{
  शून्य *handle;

  handle = dlopen ("libmono-native.so", RTLD_NOW);
  यदि (handle == रिक्त)
    फर्काऊ crun_make_error (err, 0, "could not load `libmono-native.so`: `%s`", dlerror ());
  *cookie = handle;

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
mono_configure_container (शून्य *cookie arg_unused, गणना handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          अचर वर्ण *rootfs arg_unused, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  यदि (phase != HANDLER_CONFIGURE_MOUNTS)
    फर्काऊ 0;

  वर्ण *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/etc/mono", "/etc/mono", options, 5, err);
  यदि (ret != 0)
    फर्काऊ ret;

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/mono", "/usr/lib/mono", options, 5, err);
  यदि (ret != 0)
    फर्काऊ ret;

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
mono_unload (शून्य *cookie, libcrun_error_t *err)
{
  पूर्णाङ्क r;

  यदि (cookie)
    {
      r = dlclose (cookie);
      यदि (UNLIKELY (r < 0))
        फर्काऊ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
mono_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  अचर वर्ण *annotation;

  annotation = find_annotation (container, "run.oci.handler");
  यदि (annotation)
    फर्काऊ म_भेद (annotation, "dotnet") == 0 ? 1 : 0;

  फर्काऊ 0;
}

संरचना custom_handler_s handler_mono = {
  .name = "dotnet",
  .alias = रिक्त,
  .feature_string = ".NET:mono",
  .supports_open_रूख_namespace = false,
  .load = mono_load,
  .unload = mono_unload,
  .run_func = mono_exec,
  .can_handle_container = mono_can_handle_container,
  .configure_container = mono_configure_container,
};

#endif
