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
#परिभाषय _GNU_SOURCE

#अन्तर्भाव <config.h>
#अन्तर्भाव "../custom-handler.h"
#अन्तर्भाव "../container.h"
#अन्तर्भाव "../utils.h"
#अन्तर्भाव "../linux.h"
#अन्तर्भाव "handler-utils.h"
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <sched.h>

#यदि_परिभाषितम् HAVE_DLOPEN
#  include <dlfcn.h>
#यदि_अन्तः

#यदि_परिभाषितम् HAVE_WASMEDGE
#  include <wasmedge/wasmedge.h>
#यदि_अन्तः

#यदि HAVE_DLOPEN && HAVE_WASMEDGE
स्थिर पूर्णाङ्क
libwasmedge_आरोपय (शून्य **cookie, libcrun_error_t *err)
{
  शून्य *handle;

  handle = dlopen ("libwasmedge.so.0", RTLD_NOW);
  यदि (handle == रिक्त)
    प्रत्यागम crun_make_error (err, 0, "could not load `libwasmedge.so.0`: `%s`", dlerror ());
  *cookie = handle;

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
libwasmedge_unload (शून्य *cookie, libcrun_error_t *err)
{
  पूर्णाङ्क r;

  यदि (cookie)
    {
      r = dlclose (cookie);
      यदि (UNLIKELY (r < 0))
        प्रत्यागम crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
libwasmedge_exec (शून्य *cookie, __attribute__ ((अप्रयुक्तम्)) libcrun_container_t *container, ध्रुव वर्ण *pathname, वर्ण *ध्रुव argv[])
{
  WasmEdge_ConfigureContext *(*WasmEdge_ConfigureCreate) (शून्य);
  शून्य (*WasmEdge_ConfigureDelete) (WasmEdge_ConfigureContext *Cxt);
  शून्य (*Wasm_Edge_Configure_सङ्कलय_Proposal) (WasmEdge_ConfigureContext *Cxt, ध्रुव गणना WasmEdge_Proposal Prop);
  शून्य (*Wasm_Edge_Configure_सङ्कलय_Host_Registration) (WasmEdge_ConfigureContext *Cxt, गणना WasmEdge_HostRegistration Host);
  WasmEdge_VMContext *(*WasmEdge_VMCreate) (ध्रुव WasmEdge_ConfigureContext *ConfCxt, WasmEdge_StoreContext *StoreCxt);
  शून्य (*WasmEdge_VMDelete) (WasmEdge_VMContext *Cxt);
  Wasm_Edge_परिणाम (*WasmEdge_VMRegisterModuleFromFile) (WasmEdge_VMContext *Cxt, WasmEdge_String ModuleName, ध्रुव वर्ण *मार्गः_2);
  Wasm_Edge_परिणाम (*WasmEdge_VMRunWasmFromFile) (WasmEdge_VMContext *Cxt, ध्रुव वर्ण *मार्गः_2, ध्रुव WasmEdge_String FuncName, ध्रुव Wasm_Edge_मानम् *Params, ध्रुव uint32_t ParamLen, Wasm_Edge_मानम् *Returns, ध्रुव uint32_t ReturnLen);
  शून्य (*Wasm_Edge_Plugin_आरोपय_From_मार्गः) (ध्रुव वर्ण *मार्गः_2);
  शून्य (*WasmEdge_PluginInitWASINN) (ध्रुव वर्ण *ध्रुव *NNPreloads, ध्रुव uint32_t PreloadsLen);
  bool (*Wasm_Edge_परिणाम_OK) (ध्रुव Wasm_Edge_परिणाम Res);
  WasmEdge_String (*WasmEdge_StringCreateByCString) (ध्रुव वर्ण *Str);
  uint32_t argn = 0;
  uint32_t envn = 0;
  ध्रुव वर्ण *dirs[2] = { "/:/", ".:." };
  WasmEdge_ConfigureContext *configure;
  WasmEdge_VMContext *vm;
  Wasm_Edge_परिणाम परिणाम;

  WasmEdge_ModuleInstanceContext *wasi_module;
  WasmEdge_ModuleInstanceContext *(*WasmEdge_VMGetImportModuleContext) (WasmEdge_VMContext *Cxt, ध्रुव गणना WasmEdge_HostRegistration Reg);
  शून्य (*WasmEdge_ModuleInstanceInitWASI) (WasmEdge_ModuleInstanceContext *Cxt, ध्रुव वर्ण *ध्रुव *तर्काः_2, ध्रुव uint32_t ArgLen, ध्रुव वर्ण *ध्रुव *Envs, ध्रुव uint32_t EnvLen, ध्रुव वर्ण *ध्रुव *Dirs, ध्रुव uint32_t DirLen, ध्रुव वर्ण *ध्रुव *Preopens, ध्रुव uint32_t PreopenLen);
  WasmEdge_ModuleInstanceInitWASI = dlsym (cookie, "WasmEdge_ModuleInstanceInitWASI");

  WasmEdge_ConfigureCreate = dlsym (cookie, "WasmEdge_ConfigureCreate");
  WasmEdge_ConfigureDelete = dlsym (cookie, "WasmEdge_ConfigureDelete");
  Wasm_Edge_Configure_सङ्कलय_Proposal = dlsym (cookie, "WasmEdge_ConfigureAddProposal");
  Wasm_Edge_Configure_सङ्कलय_Host_Registration = dlsym (cookie, "WasmEdge_ConfigureAddHostRegistration");
  WasmEdge_VMCreate = dlsym (cookie, "WasmEdge_VMCreate");
  WasmEdge_VMDelete = dlsym (cookie, "WasmEdge_VMDelete");
  WasmEdge_VMRegisterModuleFromFile = dlsym (cookie, "WasmEdge_VMRegisterModuleFromFile");
  WasmEdge_VMGetImportModuleContext = dlsym (cookie, "WasmEdge_VMGetImportModuleContext");
  WasmEdge_VMRunWasmFromFile = dlsym (cookie, "WasmEdge_VMRunWasmFromFile");
  Wasm_Edge_Plugin_आरोपय_From_मार्गः = dlsym (cookie, "WasmEdge_PluginLoadFromPath");
  WasmEdge_PluginInitWASINN = dlsym (cookie, "WasmEdge_PluginInitWASINN");
  Wasm_Edge_परिणाम_OK = dlsym (cookie, "WasmEdge_ResultOK");
  WasmEdge_StringCreateByCString = dlsym (cookie, "WasmEdge_StringCreateByCString");

  यदि (WasmEdge_ConfigureCreate == रिक्त || WasmEdge_ConfigureDelete == रिक्त || Wasm_Edge_Configure_सङ्कलय_Proposal == रिक्त
      || Wasm_Edge_Configure_सङ्कलय_Host_Registration == रिक्त || WasmEdge_VMCreate == रिक्त || WasmEdge_VMDelete == रिक्त
      || WasmEdge_VMRegisterModuleFromFile == रिक्त || WasmEdge_VMGetImportModuleContext == रिक्त
      || WasmEdge_ModuleInstanceInitWASI == रिक्त || WasmEdge_VMRunWasmFromFile == रिक्त
      || Wasm_Edge_परिणाम_OK == रिक्त || WasmEdge_StringCreateByCString == रिक्त)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmedge.so.0`");

  configure = WasmEdge_ConfigureCreate ();
  यदि (UNLIKELY (configure == रिक्त))
    error (EXIT_FAILURE, 0, "could not create wasmedge configure");

  Wasm_Edge_Configure_सङ्कलय_Proposal (configure, Wasm_Edge_Proposal_Bulk_स्मृतिः_Operations);
  Wasm_Edge_Configure_सङ्कलय_Proposal (configure, WasmEdge_Proposal_ReferenceTypes);
  Wasm_Edge_Configure_सङ्कलय_Proposal (configure, WasmEdge_Proposal_SIMD);
  Wasm_Edge_Configure_सङ्कलय_Host_Registration (configure, WasmEdge_HostRegistration_Wasi);
  // Check if the necessary environment variables are set
  ध्रुव वर्ण *plugin_मार्गः_env = getenv ("WASMEDGE_PLUGIN_PATH");
  यदि (plugin_मार्गः_env != रिक्त)
    Wasm_Edge_Plugin_आरोपय_From_मार्गः (plugin_मार्गः_env);

  ध्रुव वर्ण *nnpreload_env = getenv ("WASMEDGE_WASINN_PRELOAD");
  यदि (nnpreload_env != रिक्त)
    WasmEdge_PluginInitWASINN (&nnpreload_env, 1);

  vm = WasmEdge_VMCreate (configure, रिक्त);
  यदि (UNLIKELY (vm == रिक्त))
    {
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not create wasmedge vm");
    }

  wasi_module = WasmEdge_VMGetImportModuleContext (vm, WasmEdge_HostRegistration_Wasi);
  यदि (UNLIKELY (wasi_module == रिक्त))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge wasi module context");
    }

  प्रति (वर्ण *ध्रुव *arg = argv; *arg != रिक्त; ++arg, ++argn)
    ;
  बाह्य वर्ण **environ;
  प्रति (वर्ण *ध्रुव *env = environ; *env != रिक्त; ++env, ++envn)
    ;

  WasmEdge_ModuleInstanceInitWASI (wasi_module, (ध्रुव वर्ण *ध्रुव *) &argv[0], argn, (ध्रुव वर्ण *ध्रुव *) &environ[0], envn, dirs, 1, रिक्त, 0);

  परिणाम = WasmEdge_VMRunWasmFromFile (vm, pathname, WasmEdge_StringCreateByCString ("_start"), रिक्त, 0, रिक्त, 0);

  यदि (UNLIKELY (! Wasm_Edge_परिणाम_OK (परिणाम)))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge result from VM");
    }

  WasmEdge_VMDelete (vm);
  WasmEdge_ConfigureDelete (configure);
  निर्गच्छ (EXIT_SUCCESS);
}

स्थिर पूर्णाङ्क
wasmedge_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  प्रत्यागम wasm_can_handle_container (container, err);
}

// This works only when the plugin is present in /usr/lib/wasmedge
स्थिर पूर्णाङ्क
libwasmedge_configure_container (शून्य *cookie arg_अप्रयुक्तम्, गणना handler_configure_phase phase,
                                 libcrun_context_t *context arg_अप्रयुक्तम्, libcrun_container_t *container,
                                 ध्रुव वर्ण *rootfs arg_अप्रयुक्तम्, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  वर्ण **container_env = def->process->env;
  bool has_plugin_मार्गः = false, has_preload = false;

  प्रति (वर्ण **env = container_env; env && *env; env++)
    {
      यदि (वाक्ये_तुलय (*env, "WASMEDGE_PLUGIN_PATH=", 21) == 0)
        has_plugin_मार्गः = true;
      अन्यथा यदि (वाक्ये_तुलय (*env, "WASMEDGE_WASINN_PRELOAD=", 24) == 0)
        has_preload = true;
    }

  यदि (! has_plugin_मार्गः && ! has_preload)
    प्रत्यागम 0;

  यदि (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    प्रत्यागम 0;

  // Check if /usr/lib/wasmedge is already present in spec
  यदि (def->linux && def->mounts)
    {
      प्रति (size_t i = 0; i < def->mounts_len; i++)
        {
          यदि (वाक्यं_तुलय (def->mounts[i]->destination, "/usr/lib/wasmedge") == 0)
            प्रत्यागम 0;
        }
    }

  // Mount the plugin folder to /usr/lib/wasmedge with specific options
  वर्ण *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/wasmedge ", "/usr/lib/wasmedge", options, 5, err);
  यदि (ret < 0)
    {
      यदि (crun_error_get_errno (err) != ENOENT)
        प्रत्यागम ret;
      crun_error_release (err);
    }

  प्रत्यागम 0;
}

संरचना custom_handler_s handler_wasmedge = {
  .name = "wasmedge",
  .alias = "wasm",
  .feature_string = "WASM:wasmedge",
  .supports_open_वृक्षः_namespace = false,
  .आरोपय = libwasmedge_आरोपय,
  .unload = libwasmedge_unload,
  .सततम्_func = libwasmedge_exec,
  .can_handle_container = wasmedge_can_handle_container,
  .configure_container = libwasmedge_configure_container,
};

#यदि_अन्तः
