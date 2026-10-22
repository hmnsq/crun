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
#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ "../custom-handler.h"
#అన్తర్భావ "../container.h"
#అన్తర్భావ "../utils.h"
#అన్తర్భావ "../linux.h"
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <errno.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <sched.h>

#నిర్వచితమైతే HAVE_DLOPEN
#  include <dlfcn.h>
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_MONO
#  include <mono/metadata/environment.h>
#  include <mono/utils/mono-publib.h>
#  include <mono/metadata/mono-config.h>
#  include <mono/jit/jit.h>
#అయితే_ముగింపు

#ఒకవేళ HAVE_DLOPEN && HAVE_MONO
స్థిర పూర్ణాంకం
mono_exec (శూన్య *cookie arg_వాడని, libcrun_container_t *container arg_వాడని,
           స్థిరరాశి అక్షరం *pathname, అక్షరం *స్థిరరాశి argv[] arg_వాడని)
{
  MonoDomain *domain;
  అక్షరం *మార్గం = (అక్షరం *) pathname;
  పూర్ణాంకం argc = 2;
  అక్షరం *argv_mono[] = {
    మార్గం,
    మార్గం,
    ఖాళీ
  };
  స్థిరరాశి అక్షరం *file;
  పూర్ణాంకం retval;

  file = argv_mono[1];

  MonoAllocatorVTable mem_vtable = { MONO_ALLOCATOR_VTABLE_సంస్కరణ, xmalloc, ఖాళీ, ఖాళీ, ఖాళీ };
  mono_సమితి_allocator_vtable (&mem_vtable);

  /*
   * Load the default Mono configuration file, this is needed
   * if you are planning on using the dllmaps defined on the
   * system configuration
   */
  mono_config_parse (ఖాళీ);
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
  ఒకవేళ (! assembly)
    నిష్క్రమించు (EXIT_FAILURE);
  /*
   * mono_jit_exec() will run the Main() method in the assembly.
   * The return value needs to be looked up from
   * System.Environment.ExitCode.
   */
  mono_jit_exec (domain, assembly, argc - 1, argv_mono + 1);
  retval = mono_environment_exitcode_get ();
  mono_jit_cleanup (domain);
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
mono_ఎక్కించు (శూన్య **cookie, libcrun_error_t *err)
{
  శూన్య *handle;

  handle = dlopen ("libmono-native.so", RTLD_NOW);
  ఒకవేళ (handle == ఖాళీ)
    తిరిగి crun_make_error (err, 0, "could not load `libmono-native.so`: `%s`", dlerror ());
  *cookie = handle;

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
mono_configure_container (శూన్య *cookie arg_వాడని, గణన handler_configure_phase phase,
                          libcrun_context_t *context arg_వాడని, libcrun_container_t *container,
                          స్థిరరాశి అక్షరం *rootfs arg_వాడని, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  ఒకవేళ (phase != HANDLER_CONFIGURE_MOUNTS)
    తిరిగి 0;

  అక్షరం *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/etc/mono", "/etc/mono", options, 5, err);
  ఒకవేళ (ret != 0)
    తిరిగి ret;

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/mono", "/usr/lib/mono", options, 5, err);
  ఒకవేళ (ret != 0)
    తిరిగి ret;

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
mono_unload (శూన్య *cookie, libcrun_error_t *err)
{
  పూర్ణాంకం r;

  ఒకవేళ (cookie)
    {
      r = dlclose (cookie);
      ఒకవేళ (UNLIKELY (r < 0))
        తిరిగి crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
mono_can_handle_container (libcrun_container_t *container, libcrun_error_t *err arg_వాడని)
{
  స్థిరరాశి అక్షరం *annotation;

  annotation = కనుగొను_annotation (container, "run.oci.handler");
  ఒకవేళ (annotation)
    తిరిగి strcmp (annotation, "dotnet") == 0 ? 1 : 0;

  తిరిగి 0;
}

నిర్మాణం custom_handler_s handler_mono = {
  .name = "dotnet",
  .alias = ఖాళీ,
  .feature_string = ".NET:mono",
  .supports_open_వృక్షం_namespace = false,
  .ఎక్కించు = mono_ఎక్కించు,
  .unload = mono_unload,
  .నిరంతరం_func = mono_exec,
  .can_handle_container = mono_can_handle_container,
  .configure_container = mono_configure_container,
};

#అయితే_ముగింపు
