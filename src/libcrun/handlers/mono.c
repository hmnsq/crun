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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ "../custom-handler.h"
#অন্তর্ভাৱ "../container.h"
#অন্তর্ভাৱ "../utils.h"
#অন্তর্ভাৱ "../linux.h"
#অন্তর্ভাৱ <unistd.h>
#অন্তর্ভাৱ <sys/stat.h>
#অন্তর্ভাৱ <errno.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <sched.h>

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
স্থির পূর্ণাঙ্ক
mono_exec (শূন্য *cookie arg_unused, libcrun_container_t *container arg_unused,
           ধ্রুৱ ৱর্ণ *pathname, ৱর্ণ *ধ্রুৱ argv[] arg_unused)
{
  MonoDomain *domain;
  ৱর্ণ *path = (ৱর্ণ *) pathname;
  পূর্ণাঙ্ক argc = 2;
  ৱর্ণ *argv_mono[] = {
    path,
    path,
    রিক্ত
  };
  ধ্রুৱ ৱর্ণ *file;
  পূর্ণাঙ্ক retval;

  file = argv_mono[1];

  MonoAllocatorVTable mem_vtable = { MONO_ALLOCATOR_VTABLE_VERSION, xmalloc, রিক্ত, রিক্ত, রিক্ত };
  mono_set_allocator_vtable (&mem_vtable);

  /*
   * Load the default Mono configuration file, this is needed
   * if you are planning on using the dllmaps defined on the
   * system configuration
   */
  mono_config_parse (রিক্ত);
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
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
mono_load (শূন্য **cookie, libcrun_error_t *err)
{
  শূন্য *handle;

  handle = dlopen ("libmono-native.so", RTLD_NOW);
  যদি (handle == রিক্ত)
    ঘূৰাই crun_make_error (err, 0, "could not load `libmono-native.so`: `%s`", dlerror ());
  *cookie = handle;

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
mono_configure_container (শূন্য *cookie arg_unused, গণনা handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          ধ্রুৱ ৱর্ণ *rootfs arg_unused, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  যদি (phase != HANDLER_CONFIGURE_MOUNTS)
    ঘূৰাই 0;

  ৱর্ণ *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/etc/mono", "/etc/mono", options, 5, err);
  যদি (ret != 0)
    ঘূৰাই ret;

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/mono", "/usr/lib/mono", options, 5, err);
  যদি (ret != 0)
    ঘূৰাই ret;

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
mono_unload (শূন্য *cookie, libcrun_error_t *err)
{
  পূর্ণাঙ্ক r;

  যদি (cookie)
    {
      r = dlclose (cookie);
      যদি (UNLIKELY (r < 0))
        ঘূৰাই crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
mono_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  ধ্রুৱ ৱর্ণ *annotation;

  annotation = find_annotation (container, "run.oci.handler");
  যদি (annotation)
    ঘূৰাই strcmp (annotation, "dotnet") == 0 ? 1 : 0;

  ঘূৰাই 0;
}

সংরচনা custom_handler_s handler_mono = {
  .name = "dotnet",
  .alias = রিক্ত,
  .feature_string = ".NET:mono",
  .supports_open_বৃক্ষ_namespace = false,
  .load = mono_load,
  .unload = mono_unload,
  .run_func = mono_exec,
  .can_handle_container = mono_can_handle_container,
  .configure_container = mono_configure_container,
};

#endif
