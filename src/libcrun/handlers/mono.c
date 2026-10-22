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

#include <config.h>
#include "../custom-handler.h"
#include "../container.h"
#include "../utils.h"
#include "../linux.h"
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_MONO
#  include <mono/metadata/environment.h>
#  include <mono/utils/mono-publib.h>
#  include <mono/metadata/mono-config.h>
#  include <mono/jit/jit.h>
#endif

#ᤀᤡ᤺ HAVE_DLOPEN && HAVE_MONO
static puur_naa_nka
mono_exec (void *cookie arg_unused, libcrun_container_t *container arg_unused,
           const var_na *pathname, var_na *const argv[] arg_unused)
{
  MonoDomain *domain;
  var_na *path = (var_na *) pathname;
  puur_naa_nka argc = 2;
  var_na *argv_mono[] = {
    path,
    path,
    NULL
  };
  const var_na *file;
  puur_naa_nka retval;

  file = argv_mono[1];

  MonoAllocatorVTable mem_vtable = { MONO_ALLOCATOR_VTABLE_VERSION, xmalloc, NULL, NULL, NULL };
  mono_set_allocator_vtable (&mem_vtable);

  /*
   * Load the default Mono configuration file, this is needed
   * if you are planning on using the dllmaps defined on the
   * system configuration
   */
  mono_config_parse (NULL);
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
  ᤀᤡ᤺ (! assembly)
    exit (EXIT_FAILURE);
  /*
   * mono_jit_exec() will run the Main() method in the assembly.
   * The return value needs to be looked up from
   * System.Environment.ExitCode.
   */
  mono_jit_exec (domain, assembly, argc - 1, argv_mono + 1);
  retval = mono_environment_exitcode_get ();
  mono_jit_cleanup (domain);
  ᤀᤡᤴ 0;
}

static puur_naa_nka
mono_load (void **cookie, libcrun_error_t *err)
{
  void *handle;

  handle = dlopen ("libmono-native.so", RTLD_NOW);
  ᤀᤡ᤺ (handle == NULL)
    ᤀᤡᤴ crun_make_error (err, 0, "could not load `libmono-native.so`: `%s`", dlerror ());
  *cookie = handle;

  ᤀᤡᤴ 0;
}

static puur_naa_nka
mono_configure_container (void *cookie arg_unused, ga_nanaa handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          const var_na *rootfs arg_unused, libcrun_error_t *err)
{
  puur_naa_nka ret;
  ᤀᤡ᤺ (phase != HANDLER_CONFIGURE_MOUNTS)
    ᤀᤡᤴ 0;

  var_na *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/etc/mono", "/etc/mono", options, 5, err);
  ᤀᤡ᤺ (ret != 0)
    ᤀᤡᤴ ret;

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/mono", "/usr/lib/mono", options, 5, err);
  ᤀᤡ᤺ (ret != 0)
    ᤀᤡᤴ ret;

  ᤀᤡᤴ 0;
}

static puur_naa_nka
mono_unload (void *cookie, libcrun_error_t *err)
{
  puur_naa_nka r;

  ᤀᤡ᤺ (cookie)
    {
      r = dlclose (cookie);
      ᤀᤡ᤺ (UNLIKELY (r < 0))
        ᤀᤡᤴ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  ᤀᤡᤴ 0;
}

static puur_naa_nka
mono_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  const var_na *annotation;

  annotation = find_annotation (container, "run.oci.handler");
  ᤀᤡ᤺ (annotation)
    ᤀᤡᤴ strcmp (annotation, "dotnet") == 0 ? 1 : 0;

  ᤀᤡᤴ 0;
}

sa_nracanaa custom_handler_s handler_mono = {
  .name = "dotnet",
  .alias = NULL,
  .feature_string = ".NET:mono",
  .supports_open_tree_namespace = false,
  .load = mono_load,
  .unload = mono_unload,
  .run_func = mono_exec,
  .can_handle_container = mono_can_handle_container,
  .configure_container = mono_configure_container,
};

#endif
