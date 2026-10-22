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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "../custom-handler.h"
#ଅନ୍ତର୍ଭାଵ "../container.h"
#ଅନ୍ତର୍ଭାଵ "../utils.h"
#ଅନ୍ତର୍ଭାଵ "../linux.h"
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_MONO
#  include <mono/metadata/environment.h>
#  include <mono/utils/mono-publib.h>
#  include <mono/metadata/mono-config.h>
#  include <mono/jit/jit.h>
#endif

#ଯଦି HAVE_DLOPEN && HAVE_MONO
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
mono_exec (ଶୂନ୍ୟ *cookie arg_unused, libcrun_container_t *container arg_unused,
           ଅଚଳ ଅକ୍ଷର *pathname, ଅକ୍ଷର *ଅଚଳ argv[] arg_unused)
{
  MonoDomain *domain;
  ଅକ୍ଷର *path = (ଅକ୍ଷର *) pathname;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc = 2;
  ଅକ୍ଷର *argv_mono[] = {
    path,
    path,
    ଖାଲି
  };
  ଅଚଳ ଅକ୍ଷର *file;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା retval;

  file = argv_mono[1];

  MonoAllocatorVTable mem_vtable = { MONO_ALLOCATOR_VTABLE_VERSION, xmalloc, ଖାଲି, ଖାଲି, ଖାଲି };
  mono_set_allocator_vtable (&mem_vtable);

  /*
   * Load the default Mono configuration file, this is needed
   * if you are planning on using the dllmaps defined on the
   * system configuration
   */
  mono_config_parse (ଖାଲି);
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
  ଯଦି (! assembly)
    exit (EXIT_FAILURE);
  /*
   * mono_jit_exec() will run the Main() method in the assembly.
   * The return value needs to be looked up from
   * System.Environment.ExitCode.
   */
  mono_jit_exec (domain, assembly, argc - 1, argv_mono + 1);
  retval = mono_environment_exitcode_get ();
  mono_jit_cleanup (domain);
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
mono_load (ଶୂନ୍ୟ **cookie, libcrun_error_t *err)
{
  ଶୂନ୍ୟ *handle;

  handle = dlopen ("libmono-native.so", RTLD_NOW);
  ଯଦି (handle == ଖାଲି)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "could not load `libmono-native.so`: `%s`", dlerror ());
  *cookie = handle;

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
mono_configure_container (ଶୂନ୍ୟ *cookie arg_unused, ଗଣନା handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          ଅଚଳ ଅକ୍ଷର *rootfs arg_unused, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ଯଦି (phase != HANDLER_CONFIGURE_MOUNTS)
    ଫେରନ୍ତୁ 0;

  ଅକ୍ଷର *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/etc/mono", "/etc/mono", options, 5, err);
  ଯଦି (ret != 0)
    ଫେରନ୍ତୁ ret;

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/mono", "/usr/lib/mono", options, 5, err);
  ଯଦି (ret != 0)
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
mono_unload (ଶୂନ୍ୟ *cookie, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା r;

  ଯଦି (cookie)
    {
      r = dlclose (cookie);
      ଯଦି (UNLIKELY (r < 0))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
mono_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  ଅଚଳ ଅକ୍ଷର *annotation;

  annotation = find_annotation (container, "run.oci.handler");
  ଯଦି (annotation)
    ଫେରନ୍ତୁ strcmp (annotation, "dotnet") == 0 ? 1 : 0;

  ଫେରନ୍ତୁ 0;
}

ସଂରଚନା custom_handler_s handler_mono = {
  .name = "dotnet",
  .alias = ଖାଲି,
  .feature_string = ".NET:mono",
  .supports_open_ବୃକ୍ଷ_namespace = false,
  .load = mono_load,
  .unload = mono_unload,
  .run_func = mono_exec,
  .can_handle_container = mono_can_handle_container,
  .configure_container = mono_configure_container,
};

#endif
