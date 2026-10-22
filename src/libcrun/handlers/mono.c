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

#شامل <config.h>
#شامل "../custom-handler.h"
#شامل "../container.h"
#شامل "../utils.h"
#شامل "../linux.h"
#شامل <unistd.h>
#شامل <sys/stat.h>
#شامل <errno.h>
#شامل <sys/types.h>
#شامل <fcntl.h>
#شامل <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_MONO
#  include <mono/metadata/environment.h>
#  include <mono/utils/mono-publib.h>
#  include <mono/metadata/mono-config.h>
#  include <mono/jit/jit.h>
#endif

#اگر HAVE_DLOPEN && HAVE_MONO
ایستا صحیح
mono_exec (تهی *cookie arg_unused, libcrun_container_t *container arg_unused,
           ثابت نویسه *pathname, نویسه *ثابت argv[] arg_unused)
{
  MonoDomain *domain;
  نویسه *path = (نویسه *) pathname;
  صحیح argc = 2;
  نویسه *argv_mono[] = {
    path,
    path,
    پوچ
  };
  ثابت نویسه *file;
  صحیح retval;

  file = argv_mono[1];

  MonoAllocatorVTable mem_vtable = { MONO_ALLOCATOR_VTABLE_VERSION, xmalloc, پوچ, پوچ, پوچ };
  mono_set_allocator_vtable (&mem_vtable);

  /*
   * Load the default Mono configuration file, this is needed
   * if you are planning on using the dllmaps defined on the
   * system configuration
   */
  mono_config_parse (پوچ);
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
  اگر (! assembly)
    exit (EXIT_FAILURE);
  /*
   * mono_jit_exec() will run the Main() method in the assembly.
   * The return value needs to be looked up from
   * System.Environment.ExitCode.
   */
  mono_jit_exec (domain, assembly, argc - 1, argv_mono + 1);
  retval = mono_environment_exitcode_get ();
  mono_jit_cleanup (domain);
  بازگشت 0;
}

ایستا صحیح
mono_load (تهی **cookie, libcrun_error_t *err)
{
  تهی *handle;

  handle = dlopen ("libmono-native.so", RTLD_NOW);
  اگر (handle == پوچ)
    بازگشت crun_make_error (err, 0, "could not load `libmono-native.so`: `%s`", dlerror ());
  *cookie = handle;

  بازگشت 0;
}

ایستا صحیح
mono_configure_container (تهی *cookie arg_unused, شمارش handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          ثابت نویسه *rootfs arg_unused, libcrun_error_t *err)
{
  صحیح ret;
  اگر (phase != HANDLER_CONFIGURE_MOUNTS)
    بازگشت 0;

  نویسه *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/etc/mono", "/etc/mono", options, 5, err);
  اگر (ret != 0)
    بازگشت ret;

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/mono", "/usr/lib/mono", options, 5, err);
  اگر (ret != 0)
    بازگشت ret;

  بازگشت 0;
}

ایستا صحیح
mono_unload (تهی *cookie, libcrun_error_t *err)
{
  صحیح r;

  اگر (cookie)
    {
      r = dlclose (cookie);
      اگر (UNLIKELY (r < 0))
        بازگشت crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  بازگشت 0;
}

ایستا صحیح
mono_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  ثابت نویسه *annotation;

  annotation = find_annotation (container, "run.oci.handler");
  اگر (annotation)
    بازگشت strcmp (annotation, "dotnet") == 0 ? 1 : 0;

  بازگشت 0;
}

ساختار custom_handler_s handler_mono = {
  .name = "dotnet",
  .alias = پوچ,
  .feature_string = ".NET:mono",
  .supports_open_درخت_namespace = false,
  .load = mono_load,
  .unload = mono_unload,
  .run_func = mono_exec,
  .can_handle_container = mono_can_handle_container,
  .configure_container = mono_configure_container,
};

#endif
