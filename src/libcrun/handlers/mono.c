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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "../custom-handler.h"
#অন্তর্ভাব "../container.h"
#অন্তর্ভাব "../utils.h"
#অন্তর্ভাব "../linux.h"
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_MONO
#  include <mono/metadata/environment.h>
#  include <mono/utils/mono-publib.h>
#  include <mono/metadata/mono-config.h>
#  include <mono/jit/jit.h>
#endif

#যদি HAVE_DLOPEN && HAVE_MONO
স্থির পূর্ণসংখ্যা
mono_exec (শূন্য *cookie arg_unused, libcrun_container_t *container arg_unused,
           ধ্রুবক অক্ষর *pathname, অক্ষর *ধ্রুবক argv[] arg_unused)
{
  MonoDomain *domain;
  অক্ষর *path = (অক্ষর *) pathname;
  পূর্ণসংখ্যা argc = 2;
  অক্ষর *argv_mono[] = {
    path,
    path,
    ফাঁকা
  };
  ধ্রুবক অক্ষর *file;
  পূর্ণসংখ্যা retval;

  file = argv_mono[1];

  MonoAllocatorVTable mem_vtable = { MONO_ALLOCATOR_VTABLE_VERSION, xmalloc, ফাঁকা, ফাঁকা, ফাঁকা };
  mono_set_allocator_vtable (&mem_vtable);

  /*
   * Load the default Mono configuration file, this is needed
   * if you are planning on using the dllmaps defined on the
   * system configuration
   */
  mono_config_parse (ফাঁকা);
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
  যদি (! assembly)
    exit (EXIT_FAILURE);
  /*
   * mono_jit_exec() will run the Main() method in the assembly.
   * The return value needs to be looked up from
   * System.Environment.ExitCode.
   */
  mono_jit_exec (domain, assembly, argc - 1, argv_mono + 1);
  retval = mono_environment_exitcode_get ();
  mono_jit_cleanup (domain);
  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
mono_load (শূন্য **cookie, libcrun_error_t *err)
{
  শূন্য *handle;

  handle = dlopen ("libmono-native.so", RTLD_NOW);
  যদি (handle == ফাঁকা)
    ফেরত crun_make_error (err, 0, "could not load `libmono-native.so`: `%s`", dlerror ());
  *cookie = handle;

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
mono_configure_container (শূন্য *cookie arg_unused, গণনা handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          ধ্রুবক অক্ষর *rootfs arg_unused, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;
  যদি (phase != HANDLER_CONFIGURE_MOUNTS)
    ফেরত 0;

  অক্ষর *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/etc/mono", "/etc/mono", options, 5, err);
  যদি (ret != 0)
    ফেরত ret;

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/mono", "/usr/lib/mono", options, 5, err);
  যদি (ret != 0)
    ফেরত ret;

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
mono_unload (শূন্য *cookie, libcrun_error_t *err)
{
  পূর্ণসংখ্যা r;

  যদি (cookie)
    {
      r = dlclose (cookie);
      যদি (UNLIKELY (r < 0))
        ফেরত crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
mono_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  ধ্রুবক অক্ষর *annotation;

  annotation = find_annotation (container, "run.oci.handler");
  যদি (annotation)
    ফেরত strcmp (annotation, "dotnet") == 0 ? 1 : 0;

  ফেরত 0;
}

কাঠামো custom_handler_s handler_mono = {
  .name = "dotnet",
  .alias = ফাঁকা,
  .feature_string = ".NET:mono",
  .supports_open_বৃক্ষ_namespace = false,
  .load = mono_load,
  .unload = mono_unload,
  .run_func = mono_exec,
  .can_handle_container = mono_can_handle_container,
  .configure_container = mono_configure_container,
};

#endif
