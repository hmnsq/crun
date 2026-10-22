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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "../custom-handler.h"
#ಅನ್ತರ್ಭಾವ "../container.h"
#ಅನ್ತರ್ಭಾವ "../utils.h"
#ಅನ್ತರ್ಭಾವ "../linux.h"
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_MONO
#  include <mono/metadata/environment.h>
#  include <mono/utils/mono-publib.h>
#  include <mono/metadata/mono-config.h>
#  include <mono/jit/jit.h>
#endif

#ಯದಿ HAVE_DLOPEN && HAVE_MONO
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
mono_exec (ಶೂನ್ಯ *cookie arg_unused, libcrun_container_t *container arg_unused,
           ಧ್ರುವ ವರ್ಣ *pathname, ವರ್ಣ *ಧ್ರುವ argv[] arg_unused)
{
  MonoDomain *domain;
  ವರ್ಣ *path = (ವರ್ಣ *) pathname;
  ಪೂರ್ಣಾಙ್ಕ argc = 2;
  ವರ್ಣ *argv_mono[] = {
    path,
    path,
    ರಿಕ್ತ
  };
  ಧ್ರುವ ವರ್ಣ *file;
  ಪೂರ್ಣಾಙ್ಕ retval;

  file = argv_mono[1];

  MonoAllocatorVTable mem_vtable = { MONO_ALLOCATOR_VTABLE_VERSION, xmalloc, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ };
  mono_set_allocator_vtable (&mem_vtable);

  /*
   * Load the default Mono configuration file, this is needed
   * if you are planning on using the dllmaps defined on the
   * system configuration
   */
  mono_config_parse (ರಿಕ್ತ);
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
  ಯದಿ (! assembly)
    exit (EXIT_FAILURE);
  /*
   * mono_jit_exec() will run the Main() method in the assembly.
   * The return value needs to be looked up from
   * System.Environment.ExitCode.
   */
  mono_jit_exec (domain, assembly, argc - 1, argv_mono + 1);
  retval = mono_environment_exitcode_get ();
  mono_jit_cleanup (domain);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
mono_load (ಶೂನ್ಯ **cookie, libcrun_error_t *err)
{
  ಶೂನ್ಯ *handle;

  handle = dlopen ("libmono-native.so", RTLD_NOW);
  ಯದಿ (handle == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not load `libmono-native.so`: `%s`", dlerror ());
  *cookie = handle;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
mono_configure_container (ಶೂನ್ಯ *cookie arg_unused, ಗಣನಾ handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          ಧ್ರುವ ವರ್ಣ *rootfs arg_unused, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಯದಿ (phase != HANDLER_CONFIGURE_MOUNTS)
    ಪ್ರತ್ಯಾಗಮ 0;

  ವರ್ಣ *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/etc/mono", "/etc/mono", options, 5, err);
  ಯದಿ (ret != 0)
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/mono", "/usr/lib/mono", options, 5, err);
  ಯದಿ (ret != 0)
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
mono_unload (ಶೂನ್ಯ *cookie, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ r;

  ಯದಿ (cookie)
    {
      r = dlclose (cookie);
      ಯದಿ (UNLIKELY (r < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
mono_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  ಧ್ರುವ ವರ್ಣ *annotation;

  annotation = find_annotation (container, "run.oci.handler");
  ಯದಿ (annotation)
    ಪ್ರತ್ಯಾಗಮ strcmp (annotation, "dotnet") == 0 ? 1 : 0;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ custom_handler_s handler_mono = {
  .name = "dotnet",
  .alias = ರಿಕ್ತ,
  .feature_string = ".NET:mono",
  .supports_open_ವೃಕ್ಷ_namespace = false,
  .load = mono_load,
  .unload = mono_unload,
  .run_func = mono_exec,
  .can_handle_container = mono_can_handle_container,
  .configure_container = mono_configure_container,
};

#endif
