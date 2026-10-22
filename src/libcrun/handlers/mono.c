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
#تعریف_کرو GNU_ماخذ

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

#اگر_تعریف_شدہ HAVE_DLOPEN
#  include <dlfcn.h>
#اگر_ختم

#اگر_تعریف_شدہ HAVE_MONO
#  include <mono/metadata/environment.h>
#  include <mono/utils/mono-publib.h>
#  include <mono/metadata/mono-config.h>
#  include <mono/jit/jit.h>
#اگر_ختم

#اگر HAVE_DLOPEN && HAVE_MONO
ساکن صحیح
mono_exec (خالی *cookie arg_غیر_مستعمل, libcrun_container_t *container arg_غیر_مستعمل,
           ثابت حرف *pathname, حرف *ثابت argv[] arg_غیر_مستعمل)
{
  MonoDomain *domain;
  حرف *راستہ = (حرف *) pathname;
  صحیح argc = 2;
  حرف *argv_mono[] = {
    راستہ,
    راستہ,
    تہی
  };
  ثابت حرف *file;
  صحیح retval;

  file = argv_mono[1];

  MonoAllocatorVTable mem_vtable = { MONO_ALLOCATOR_VTABLE_نسخہ, xmalloc, تہی, تہی, تہی };
  mono_سیٹ_allocator_vtable (&mem_vtable);

  /*
   * Load the default Mono configuration file, this is needed
   * if you are planning on using the dllmaps defined on the
   * system configuration
   */
  mono_config_parse (تہی);
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
    باہر_نکلو (EXIT_FAILURE);
  /*
   * mono_jit_exec() will run the Main() method in the assembly.
   * The return value needs to be looked up from
   * System.Environment.ExitCode.
   */
  mono_jit_exec (domain, assembly, argc - 1, argv_mono + 1);
  retval = mono_environment_exitcode_get ();
  mono_jit_cleanup (domain);
  واپس 0;
}

ساکن صحیح
mono_بھرو (خالی **cookie, libcrun_error_t *err)
{
  خالی *handle;

  handle = dlopen ("libmono-native.so", RTLD_NOW);
  اگر (handle == تہی)
    واپس crun_make_error (err, 0, "could not load `libmono-native.so`: `%s`", dlerror ());
  *cookie = handle;

  واپس 0;
}

ساکن صحیح
mono_configure_container (خالی *cookie arg_غیر_مستعمل, شمار handler_configure_phase phase,
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

  ret = libcrun_container_do_bind_mount (container, "/etc/mono", "/etc/mono", options, 5, err);
  اگر (ret != 0)
    واپس ret;

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/mono", "/usr/lib/mono", options, 5, err);
  اگر (ret != 0)
    واپس ret;

  واپس 0;
}

ساکن صحیح
mono_unload (خالی *cookie, libcrun_error_t *err)
{
  صحیح r;

  اگر (cookie)
    {
      r = dlclose (cookie);
      اگر (UNLIKELY (r < 0))
        واپس crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  واپس 0;
}

ساکن صحیح
mono_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_غیر_مستعمل)
{
  ثابت حرف *annotation;

  annotation = ڈھونڈو_annotation (container, "run.oci.handler");
  اگر (annotation)
    واپس strcmp (annotation, "dotnet") == 0 ? 1 : 0;

  واپس 0;
}

ساخت custom_handler_s handler_mono = {
  .name = "dotnet",
  .alias = تہی,
  .feature_string = ".NET:mono",
  .supports_open_درخت_namespace = false,
  .بھرو = mono_بھرو,
  .unload = mono_unload,
  .تسلسل_func = mono_exec,
  .can_handle_container = mono_can_handle_container,
  .configure_container = mono_configure_container,
};

#اگر_ختم
