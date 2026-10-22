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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "../custom-handler.h"
#அந்தர்பாவ "../container.h"
#அந்தர்பாவ "../utils.h"
#அந்தர்பாவ "../linux.h"
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_MONO
#  include <mono/metadata/environment.h>
#  include <mono/utils/mono-publib.h>
#  include <mono/metadata/mono-config.h>
#  include <mono/jit/jit.h>
#endif

#எனில் HAVE_DLOPEN && HAVE_MONO
நிலையான முழு
mono_exec (வெறுமை *cookie arg_unused, libcrun_container_t *container arg_unused,
           மாறிலி எழுத்து *pathname, எழுத்து *மாறிலி argv[] arg_unused)
{
  MonoDomain *domain;
  எழுத்து *path = (எழுத்து *) pathname;
  முழு argc = 2;
  எழுத்து *argv_mono[] = {
    path,
    path,
    வெற்று
  };
  மாறிலி எழுத்து *file;
  முழு retval;

  file = argv_mono[1];

  MonoAllocatorVTable mem_vtable = { MONO_ALLOCATOR_VTABLE_VERSION, xmalloc, வெற்று, வெற்று, வெற்று };
  mono_set_allocator_vtable (&mem_vtable);

  /*
   * Load the default Mono configuration file, this is needed
   * if you are planning on using the dllmaps defined on the
   * system configuration
   */
  mono_config_parse (வெற்று);
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
  எனில் (! assembly)
    exit (EXIT_FAILURE);
  /*
   * mono_jit_exec() will run the Main() method in the assembly.
   * The return value needs to be looked up from
   * System.Environment.ExitCode.
   */
  mono_jit_exec (domain, assembly, argc - 1, argv_mono + 1);
  retval = mono_environment_exitcode_get ();
  mono_jit_cleanup (domain);
  திரும்பு 0;
}

நிலையான முழு
mono_load (வெறுமை **cookie, libcrun_error_t *err)
{
  வெறுமை *handle;

  handle = dlopen ("libmono-native.so", RTLD_NOW);
  எனில் (handle == வெற்று)
    திரும்பு crun_make_error (err, 0, "could not load `libmono-native.so`: `%s`", dlerror ());
  *cookie = handle;

  திரும்பு 0;
}

நிலையான முழு
mono_configure_container (வெறுமை *cookie arg_unused, எண்ணிக்கை handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          மாறிலி எழுத்து *rootfs arg_unused, libcrun_error_t *err)
{
  முழு ret;
  எனில் (phase != HANDLER_CONFIGURE_MOUNTS)
    திரும்பு 0;

  எழுத்து *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/etc/mono", "/etc/mono", options, 5, err);
  எனில் (ret != 0)
    திரும்பு ret;

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/mono", "/usr/lib/mono", options, 5, err);
  எனில் (ret != 0)
    திரும்பு ret;

  திரும்பு 0;
}

நிலையான முழு
mono_unload (வெறுமை *cookie, libcrun_error_t *err)
{
  முழு r;

  எனில் (cookie)
    {
      r = dlclose (cookie);
      எனில் (UNLIKELY (r < 0))
        திரும்பு crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  திரும்பு 0;
}

நிலையான முழு
mono_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  மாறிலி எழுத்து *annotation;

  annotation = find_annotation (container, "run.oci.handler");
  எனில் (annotation)
    திரும்பு strcmp (annotation, "dotnet") == 0 ? 1 : 0;

  திரும்பு 0;
}

கட்டமைப்பு custom_handler_s handler_mono = {
  .name = "dotnet",
  .alias = வெற்று,
  .feature_string = ".NET:mono",
  .supports_open_மரம்_namespace = false,
  .load = mono_load,
  .unload = mono_unload,
  .run_func = mono_exec,
  .can_handle_container = mono_can_handle_container,
  .configure_container = mono_configure_container,
};

#endif
