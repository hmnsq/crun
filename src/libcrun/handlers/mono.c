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
#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DLOPEN
#  include <dlfcn.h>
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_MONO
#  include <mono/metadata/environment.h>
#  include <mono/utils/mono-publib.h>
#  include <mono/metadata/mono-config.h>
#  include <mono/jit/jit.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ HAVE_DLOPEN && HAVE_MONO
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
mono_exec (ಶೂನ್ಯ *cookie arg_ಬಳಸದ, libcrun_container_t *container arg_ಬಳಸದ,
           ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ argv[] arg_ಬಳಸದ)
{
  MonoDomain *domain;
  ಅಕ್ಷರ *ಮಾರ್ಗ = (ಅಕ್ಷರ *) pathname;
  ಪೂರ್ಣಾಂಕ argc = 2;
  ಅಕ್ಷರ *argv_mono[] = {
    ಮಾರ್ಗ,
    ಮಾರ್ಗ,
    ಖಾಲಿ
  };
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file;
  ಪೂರ್ಣಾಂಕ retval;

  file = argv_mono[1];

  MonoAllocatorVTable mem_vtable = { MONO_ALLOCATOR_VTABLE_ಆವೃತ್ತಿ, xmalloc, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ };
  mono_ಗಣ_allocator_vtable (&mem_vtable);

  /*
   * Load the default Mono configuration file, this is needed
   * if you are planning on using the dllmaps defined on the
   * system configuration
   */
  mono_config_parse (ಖಾಲಿ);
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
  ಆದರೆ (! assembly)
    ನಿರ್ಗಮಿಸು (EXIT_FAILURE);
  /*
   * mono_jit_exec() will run the Main() method in the assembly.
   * The return value needs to be looked up from
   * System.Environment.ExitCode.
   */
  mono_jit_exec (domain, assembly, argc - 1, argv_mono + 1);
  retval = mono_environment_exitcode_get ();
  mono_jit_cleanup (domain);
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
mono_ತುಂಬು (ಶೂನ್ಯ **cookie, libcrun_error_t *err)
{
  ಶೂನ್ಯ *handle;

  handle = dlopen ("libmono-native.so", RTLD_NOW);
  ಆದರೆ (handle == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not load `libmono-native.so`: `%s`", dlerror ());
  *cookie = handle;

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
mono_configure_container (ಶೂನ್ಯ *cookie arg_ಬಳಸದ, ಎಣಿಕೆ handler_configure_phase phase,
                          libcrun_context_t *context arg_ಬಳಸದ, libcrun_container_t *container,
                          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs arg_ಬಳಸದ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  ಆದರೆ (phase != HANDLER_CONFIGURE_MOUNTS)
    ಹಿಂತಿರುಗಿ 0;

  ಅಕ್ಷರ *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/etc/mono", "/etc/mono", options, 5, err);
  ಆದರೆ (ret != 0)
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/mono", "/usr/lib/mono", options, 5, err);
  ಆದರೆ (ret != 0)
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
mono_unload (ಶೂನ್ಯ *cookie, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ r;

  ಆದರೆ (cookie)
    {
      r = dlclose (cookie);
      ಆದರೆ (UNLIKELY (r < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
mono_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_ಬಳಸದ)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *annotation;

  annotation = ಕಂಡುಹಿಡಿ_annotation (container, "run.oci.handler");
  ಆದರೆ (annotation)
    ಹಿಂತಿರುಗಿ strcmp (annotation, "dotnet") == 0 ? 1 : 0;

  ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ custom_handler_s handler_mono = {
  .name = "dotnet",
  .alias = ಖಾಲಿ,
  .feature_string = ".NET:mono",
  .supports_open_ವೃಕ್ಷ_namespace = false,
  .ತುಂಬು = mono_ತುಂಬು,
  .unload = mono_unload,
  .ಸತತ_func = mono_exec,
  .can_handle_container = mono_can_handle_container,
  .configure_container = mono_configure_container,
};

#ಆದರೆ_ಅಂತ್ಯ
