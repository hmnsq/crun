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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "../custom-handler.h"
#અન્તર્ભાવ "../container.h"
#અન્તર્ભાવ "../utils.h"
#અન્તર્ભાવ "../linux.h"
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_MONO
#  include <mono/metadata/environment.h>
#  include <mono/utils/mono-publib.h>
#  include <mono/metadata/mono-config.h>
#  include <mono/jit/jit.h>
#endif

#જો HAVE_DLOPEN && HAVE_MONO
સ્થિર પૂર્ણાંક
mono_exec (શૂન્ય *cookie arg_unused, libcrun_container_t *container arg_unused,
           અચલ અક્ષર *pathname, અક્ષર *અચલ argv[] arg_unused)
{
  MonoDomain *domain;
  અક્ષર *path = (અક્ષર *) pathname;
  પૂર્ણાંક argc = 2;
  અક્ષર *argv_mono[] = {
    path,
    path,
    ખાલી
  };
  અચલ અક્ષર *file;
  પૂર્ણાંક retval;

  file = argv_mono[1];

  MonoAllocatorVTable mem_vtable = { MONO_ALLOCATOR_VTABLE_VERSION, xmalloc, ખાલી, ખાલી, ખાલી };
  mono_set_allocator_vtable (&mem_vtable);

  /*
   * Load the default Mono configuration file, this is needed
   * if you are planning on using the dllmaps defined on the
   * system configuration
   */
  mono_config_parse (ખાલી);
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
  જો (! assembly)
    exit (EXIT_FAILURE);
  /*
   * mono_jit_exec() will run the Main() method in the assembly.
   * The return value needs to be looked up from
   * System.Environment.ExitCode.
   */
  mono_jit_exec (domain, assembly, argc - 1, argv_mono + 1);
  retval = mono_environment_exitcode_get ();
  mono_jit_cleanup (domain);
  પાછા 0;
}

સ્થિર પૂર્ણાંક
mono_load (શૂન્ય **cookie, libcrun_error_t *err)
{
  શૂન્ય *handle;

  handle = dlopen ("libmono-native.so", RTLD_NOW);
  જો (handle == ખાલી)
    પાછા crun_make_error (err, 0, "could not load `libmono-native.so`: `%s`", dlerror ());
  *cookie = handle;

  પાછા 0;
}

સ્થિર પૂર્ણાંક
mono_configure_container (શૂન્ય *cookie arg_unused, ગણના handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          અચલ અક્ષર *rootfs arg_unused, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  જો (phase != HANDLER_CONFIGURE_MOUNTS)
    પાછા 0;

  અક્ષર *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/etc/mono", "/etc/mono", options, 5, err);
  જો (ret != 0)
    પાછા ret;

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/mono", "/usr/lib/mono", options, 5, err);
  જો (ret != 0)
    પાછા ret;

  પાછા 0;
}

સ્થિર પૂર્ણાંક
mono_unload (શૂન્ય *cookie, libcrun_error_t *err)
{
  પૂર્ણાંક r;

  જો (cookie)
    {
      r = dlclose (cookie);
      જો (UNLIKELY (r < 0))
        પાછા crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
mono_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  અચલ અક્ષર *annotation;

  annotation = find_annotation (container, "run.oci.handler");
  જો (annotation)
    પાછા strcmp (annotation, "dotnet") == 0 ? 1 : 0;

  પાછા 0;
}

સંરચના custom_handler_s handler_mono = {
  .name = "dotnet",
  .alias = ખાલી,
  .feature_string = ".NET:mono",
  .supports_open_વૃક્ષ_namespace = false,
  .load = mono_load,
  .unload = mono_unload,
  .run_func = mono_exec,
  .can_handle_container = mono_can_handle_container,
  .configure_container = mono_configure_container,
};

#endif
