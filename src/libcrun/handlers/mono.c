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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "../custom-handler.h"
#അന്തര്ഭാവ "../container.h"
#അന്തര്ഭാവ "../utils.h"
#അന്തര്ഭാവ "../linux.h"
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_MONO
#  include <mono/metadata/environment.h>
#  include <mono/utils/mono-publib.h>
#  include <mono/metadata/mono-config.h>
#  include <mono/jit/jit.h>
#endif

#എങ്കിൽ HAVE_DLOPEN && HAVE_MONO
സ്ഥിതം പൂർണ്ണസംഖ്യ
mono_exec (ശൂന്യം *cookie arg_unused, libcrun_container_t *container arg_unused,
           സ്ഥിരം അക്ഷരം *pathname, അക്ഷരം *സ്ഥിരം argv[] arg_unused)
{
  MonoDomain *domain;
  അക്ഷരം *path = (അക്ഷരം *) pathname;
  പൂർണ്ണസംഖ്യ argc = 2;
  അക്ഷരം *argv_mono[] = {
    path,
    path,
    ശൂന്യ
  };
  സ്ഥിരം അക്ഷരം *file;
  പൂർണ്ണസംഖ്യ retval;

  file = argv_mono[1];

  MonoAllocatorVTable mem_vtable = { MONO_ALLOCATOR_VTABLE_VERSION, xmalloc, ശൂന്യ, ശൂന്യ, ശൂന്യ };
  mono_set_allocator_vtable (&mem_vtable);

  /*
   * Load the default Mono configuration file, this is needed
   * if you are planning on using the dllmaps defined on the
   * system configuration
   */
  mono_config_parse (ശൂന്യ);
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
  എങ്കിൽ (! assembly)
    exit (EXIT_FAILURE);
  /*
   * mono_jit_exec() will run the Main() method in the assembly.
   * The return value needs to be looked up from
   * System.Environment.ExitCode.
   */
  mono_jit_exec (domain, assembly, argc - 1, argv_mono + 1);
  retval = mono_environment_exitcode_get ();
  mono_jit_cleanup (domain);
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
mono_load (ശൂന്യം **cookie, libcrun_error_t *err)
{
  ശൂന്യം *handle;

  handle = dlopen ("libmono-native.so", RTLD_NOW);
  എങ്കിൽ (handle == ശൂന്യ)
    തിരികെ crun_make_error (err, 0, "could not load `libmono-native.so`: `%s`", dlerror ());
  *cookie = handle;

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
mono_configure_container (ശൂന്യം *cookie arg_unused, എണ്ണൽ handler_configure_phase phase,
                          libcrun_context_t *context arg_unused, libcrun_container_t *container,
                          സ്ഥിരം അക്ഷരം *rootfs arg_unused, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  എങ്കിൽ (phase != HANDLER_CONFIGURE_MOUNTS)
    തിരികെ 0;

  അക്ഷരം *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/etc/mono", "/etc/mono", options, 5, err);
  എങ്കിൽ (ret != 0)
    തിരികെ ret;

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/mono", "/usr/lib/mono", options, 5, err);
  എങ്കിൽ (ret != 0)
    തിരികെ ret;

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
mono_unload (ശൂന്യം *cookie, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ r;

  എങ്കിൽ (cookie)
    {
      r = dlclose (cookie);
      എങ്കിൽ (UNLIKELY (r < 0))
        തിരികെ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
mono_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_unused)
{
  സ്ഥിരം അക്ഷരം *annotation;

  annotation = find_annotation (container, "run.oci.handler");
  എങ്കിൽ (annotation)
    തിരികെ strcmp (annotation, "dotnet") == 0 ? 1 : 0;

  തിരികെ 0;
}

ഘടന custom_handler_s handler_mono = {
  .name = "dotnet",
  .alias = ശൂന്യ,
  .feature_string = ".NET:mono",
  .supports_open_വൃക്ഷം_namespace = false,
  .load = mono_load,
  .unload = mono_unload,
  .run_func = mono_exec,
  .can_handle_container = mono_can_handle_container,
  .configure_container = mono_configure_container,
};

#endif
