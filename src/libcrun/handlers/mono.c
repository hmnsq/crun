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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "../custom-handler.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../container.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../linux.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sched.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DLOPEN
#  include <dlfcn.h>
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_MONO
#  include <mono/metadata/environment.h>
#  include <mono/utils/mono-publib.h>
#  include <mono/metadata/mono-config.h>
#  include <mono/jit/jit.h>
#ਜੇ_ਅੰਤ

#ਜੇ HAVE_DLOPEN && HAVE_MONO
ਸਥਿਰ ਪੂਰਨ
mono_exec (ਖਾਲੀ *cookie arg_ਅਣਵਰਤਿਆ, libcrun_container_t *container arg_ਅਣਵਰਤਿਆ,
           ਅਚਲ ਅੱਖਰ *pathname, ਅੱਖਰ *ਅਚਲ argv[] arg_ਅਣਵਰਤਿਆ)
{
  MonoDomain *domain;
  ਅੱਖਰ *ਰਸਤਾ = (ਅੱਖਰ *) pathname;
  ਪੂਰਨ argc = 2;
  ਅੱਖਰ *argv_mono[] = {
    ਰਸਤਾ,
    ਰਸਤਾ,
    ਨਿਰਾ
  };
  ਅਚਲ ਅੱਖਰ *file;
  ਪੂਰਨ retval;

  file = argv_mono[1];

  MonoAllocatorVTable mem_vtable = { MONO_ALLOCATOR_VTABLE_ਸੰਸਕਰਣ, xmalloc, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ };
  mono_ਸੈੱਟ_allocator_vtable (&mem_vtable);

  /*
   * Load the default Mono configuration file, this is needed
   * if you are planning on using the dllmaps defined on the
   * system configuration
   */
  mono_config_parse (ਨਿਰਾ);
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
  ਜੇ (! assembly)
    ਬਾਹਰ_ਨਿਕਲੋ (EXIT_FAILURE);
  /*
   * mono_jit_exec() will run the Main() method in the assembly.
   * The return value needs to be looked up from
   * System.Environment.ExitCode.
   */
  mono_jit_exec (domain, assembly, argc - 1, argv_mono + 1);
  retval = mono_environment_exitcode_get ();
  mono_jit_cleanup (domain);
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
mono_ਲੱਦੋ (ਖਾਲੀ **cookie, libcrun_error_t *err)
{
  ਖਾਲੀ *handle;

  handle = dlopen ("libmono-native.so", RTLD_NOW);
  ਜੇ (handle == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, 0, "could not load `libmono-native.so`: `%s`", dlerror ());
  *cookie = handle;

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
mono_configure_container (ਖਾਲੀ *cookie arg_ਅਣਵਰਤਿਆ, ਗਿਣਤੀ handler_configure_phase phase,
                          libcrun_context_t *context arg_ਅਣਵਰਤਿਆ, libcrun_container_t *container,
                          ਅਚਲ ਅੱਖਰ *rootfs arg_ਅਣਵਰਤਿਆ, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  ਜੇ (phase != HANDLER_CONFIGURE_MOUNTS)
    ਵਾਪਸ 0;

  ਅੱਖਰ *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/etc/mono", "/etc/mono", options, 5, err);
  ਜੇ (ret != 0)
    ਵਾਪਸ ret;

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/mono", "/usr/lib/mono", options, 5, err);
  ਜੇ (ret != 0)
    ਵਾਪਸ ret;

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
mono_unload (ਖਾਲੀ *cookie, libcrun_error_t *err)
{
  ਪੂਰਨ r;

  ਜੇ (cookie)
    {
      r = dlclose (cookie);
      ਜੇ (UNLIKELY (r < 0))
        ਵਾਪਸ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
mono_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_ਅਣਵਰਤਿਆ)
{
  ਅਚਲ ਅੱਖਰ *annotation;

  annotation = ਲੱਭੋ_annotation (container, "run.oci.handler");
  ਜੇ (annotation)
    ਵਾਪਸ strcmp (annotation, "dotnet") == 0 ? 1 : 0;

  ਵਾਪਸ 0;
}

ਬਣਤਰ custom_handler_s handler_mono = {
  .name = "dotnet",
  .alias = ਨਿਰਾ,
  .feature_string = ".NET:mono",
  .supports_open_ਰੁੱਖ_namespace = false,
  .ਲੱਦੋ = mono_ਲੱਦੋ,
  .unload = mono_unload,
  .ਲੜੀਵਾਰ_func = mono_exec,
  .can_handle_container = mono_can_handle_container,
  .configure_container = mono_configure_container,
};

#ਜੇ_ਅੰਤ
