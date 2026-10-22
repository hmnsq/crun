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

#ضمن <config.h>
#ضمن "../custom-handler.h"
#ضمن "../container.h"
#ضمن "../utils.h"
#ضمن "../linux.h"
#ضمن <unistd.h>
#ضمن <sys/stat.h>
#ضمن <errno.h>
#ضمن <sys/types.h>
#ضمن <fcntl.h>
#ضمن <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_MONO
#  include <mono/metadata/environment.h>
#  include <mono/utils/mono-publib.h>
#  include <mono/metadata/mono-config.h>
#  include <mono/jit/jit.h>
#endif

#إذا HAVE_DLOPEN && HAVE_MONO
ساكن صحيح
mono_exec (فراغ *cookie arg_unused, libcrun_container_t *container arg_unused,
           ثابت حرف *pathname, حرف *ثابت argv[] arg_unused)
{
  MonoDomain *domain;
  حرف *path = (حرف *) pathname;
  صحيح argc = 2;
  حرف *argv_mono[] = {
    path,
    path,
    عدم
  };
  ثابت حرف *file;
  صحيح retval;

  file = argv_mono[1];

  MonoAllocatorVTable mem_vtable = { MONO_ALLOCATOR_VTABLE_VERSION, xmalloc, عدم, عدم, عدم };
  mono_set_allocator_vtable (&mem_vtable);

  /*
   * Load the default Mono configuration file, this is needed
   * if you are planning on using the dllmaps defined on the
   * system configuration
   */
  mono_config_parse (عدم);
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
  إذا (! assembly)
    exit (EXIT_FAILURE);
  /*
   * mono_jit_exec() will run the Main() method in the assembly.
   * The return value needs to be looked up from
   * System.Environment.ExitCode.
   */
  mono_jit_exec (domain, assembly, argc - 1, argv_mono + 1);
  retval = mono_environment_exitcode_get ();
  mono_jit_cleanup (domain);
  أرجع 0;
}

ساكن صحيح
mono_load (فراغ **cookie, libcrun_error_t *err)
{
  فراغ *handle;

  handle = dlopen ("libmono-native.so", RTLD_NOW);
  إذا (handle == عدم)
    أرجع crun_make_error (err, 0, "could not load `libmono-native.so`: `%s`", dlerror ());
  *cookie = handle;

  أرجع 0;
}

ساكن صحيح
mono_configure_container (فراغ *cookie arg_unused, تعداد handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          ثابت حرف *rootfs arg_unused, libcrun_error_t *err)
{
  صحيح ret;
  إذا (phase != HANDLER_CONFIGURE_MOUNTS)
    أرجع 0;

  حرف *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/etc/mono", "/etc/mono", options, 5, err);
  إذا (ret != 0)
    أرجع ret;

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/mono", "/usr/lib/mono", options, 5, err);
  إذا (ret != 0)
    أرجع ret;

  أرجع 0;
}

ساكن صحيح
mono_unload (فراغ *cookie, libcrun_error_t *err)
{
  صحيح r;

  إذا (cookie)
    {
      r = dlclose (cookie);
      إذا (UNLIKELY (r < 0))
        أرجع crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  أرجع 0;
}

ساكن صحيح
mono_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  ثابت حرف *annotation;

  annotation = find_annotation (container, "run.oci.handler");
  إذا (annotation)
    أرجع strcmp (annotation, "dotnet") == 0 ? 1 : 0;

  أرجع 0;
}

بنية custom_handler_s handler_mono = {
  .name = "dotnet",
  .alias = عدم,
  .feature_string = ".NET:mono",
  .supports_open_درەخت_namespace = false,
  .load = mono_load,
  .unload = mono_unload,
  .run_func = mono_exec,
  .can_handle_container = mono_can_handle_container,
  .configure_container = mono_configure_container,
};

#endif
