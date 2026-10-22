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

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMEDGE
#  include <wasmedge/wasmedge.h>
#endif

#जर HAVE_DLOPEN && HAVE_WASMEDGE
स्थिर पूर्णांक
libwasmedge_load (शून्य **cookie, libcrun_error_t *err)
{
  शून्य *handle;

  handle = dlopen ("libwasmedge.so.0", RTLD_NOW);
  जर (handle == रिक्त)
    परत crun_make_error (err, 0, "could not load `libwasmedge.so.0`: `%s`", dlerror ());
  *cookie = handle;

  परत 0;
}

स्थिर पूर्णांक
libwasmedge_unload (शून्य *cookie, libcrun_error_t *err)
{
  पूर्णांक r;

  जर (cookie)
    {
      r = dlclose (cookie);
      जर (UNLIKELY (r < 0))
        परत crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  परत 0;
}

स्थिर पूर्णांक
libwasmedge_exec (शून्य *cookie, __attribute__ ((unused)) libcrun_container_t *container, स्थिरांक अक्षर *pathname, अक्षर *स्थिरांक argv[])
{
  WasmEdge_ConfigureContext *(*WasmEdge_ConfigureCreate) (शून्य);
  शून्य (*WasmEdge_ConfigureDelete) (WasmEdge_ConfigureContext *Cxt);
  शून्य (*WasmEdge_ConfigureAddProposal) (WasmEdge_ConfigureContext *Cxt, स्थिरांक गणना WasmEdge_Proposal Prop);
  शून्य (*WasmEdge_ConfigureAddHostRegistration) (WasmEdge_ConfigureContext *Cxt, गणना WasmEdge_HostRegistration Host);
  WasmEdge_VMContext *(*WasmEdge_VMCreate) (स्थिरांक WasmEdge_ConfigureContext *ConfCxt, WasmEdge_StoreContext *StoreCxt);
  शून्य (*WasmEdge_VMDelete) (WasmEdge_VMContext *Cxt);
  WasmEdge_Result (*WasmEdge_VMRegisterModuleFromFile) (WasmEdge_VMContext *Cxt, WasmEdge_String ModuleName, स्थिरांक अक्षर *Path);
  WasmEdge_Result (*WasmEdge_VMRunWasmFromFile) (WasmEdge_VMContext *Cxt, स्थिरांक अक्षर *Path, स्थिरांक WasmEdge_String FuncName, स्थिरांक Wasm_Edge_मूल्य *Params, स्थिरांक uint32_t ParamLen, Wasm_Edge_मूल्य *Returns, स्थिरांक uint32_t ReturnLen);
  शून्य (*WasmEdge_PluginLoadFromPath) (स्थिरांक अक्षर *Path);
  शून्य (*WasmEdge_PluginInitWASINN) (स्थिरांक अक्षर *स्थिरांक *NNPreloads, स्थिरांक uint32_t PreloadsLen);
  bool (*WasmEdge_ResultOK) (स्थिरांक WasmEdge_Result Res);
  WasmEdge_String (*WasmEdge_StringCreateByCString) (स्थिरांक अक्षर *Str);
  uint32_t argn = 0;
  uint32_t envn = 0;
  स्थिरांक अक्षर *dirs[2] = { "/:/", ".:." };
  WasmEdge_ConfigureContext *configure;
  WasmEdge_VMContext *vm;
  WasmEdge_Result result;

  WasmEdge_ModuleInstanceContext *wasi_module;
  WasmEdge_ModuleInstanceContext *(*WasmEdge_VMGetImportModuleContext) (WasmEdge_VMContext *Cxt, स्थिरांक गणना WasmEdge_HostRegistration Reg);
  शून्य (*WasmEdge_ModuleInstanceInitWASI) (WasmEdge_ModuleInstanceContext *Cxt, स्थिरांक अक्षर *स्थिरांक *Args, स्थिरांक uint32_t ArgLen, स्थिरांक अक्षर *स्थिरांक *Envs, स्थिरांक uint32_t EnvLen, स्थिरांक अक्षर *स्थिरांक *Dirs, स्थिरांक uint32_t DirLen, स्थिरांक अक्षर *स्थिरांक *Preopens, स्थिरांक uint32_t PreopenLen);
  WasmEdge_ModuleInstanceInitWASI = dlsym (cookie, "WasmEdge_ModuleInstanceInitWASI");

  WasmEdge_ConfigureCreate = dlsym (cookie, "WasmEdge_ConfigureCreate");
  WasmEdge_ConfigureDelete = dlsym (cookie, "WasmEdge_ConfigureDelete");
  WasmEdge_ConfigureAddProposal = dlsym (cookie, "WasmEdge_ConfigureAddProposal");
  WasmEdge_ConfigureAddHostRegistration = dlsym (cookie, "WasmEdge_ConfigureAddHostRegistration");
  WasmEdge_VMCreate = dlsym (cookie, "WasmEdge_VMCreate");
  WasmEdge_VMDelete = dlsym (cookie, "WasmEdge_VMDelete");
  WasmEdge_VMRegisterModuleFromFile = dlsym (cookie, "WasmEdge_VMRegisterModuleFromFile");
  WasmEdge_VMGetImportModuleContext = dlsym (cookie, "WasmEdge_VMGetImportModuleContext");
  WasmEdge_VMRunWasmFromFile = dlsym (cookie, "WasmEdge_VMRunWasmFromFile");
  WasmEdge_PluginLoadFromPath = dlsym (cookie, "WasmEdge_PluginLoadFromPath");
  WasmEdge_PluginInitWASINN = dlsym (cookie, "WasmEdge_PluginInitWASINN");
  WasmEdge_ResultOK = dlsym (cookie, "WasmEdge_ResultOK");
  WasmEdge_StringCreateByCString = dlsym (cookie, "WasmEdge_StringCreateByCString");

  जर (WasmEdge_ConfigureCreate == रिक्त || WasmEdge_ConfigureDelete == रिक्त || WasmEdge_ConfigureAddProposal == रिक्त
      || WasmEdge_ConfigureAddHostRegistration == रिक्त || WasmEdge_VMCreate == रिक्त || WasmEdge_VMDelete == रिक्त
      || WasmEdge_VMRegisterModuleFromFile == रिक्त || WasmEdge_VMGetImportModuleContext == रिक्त
      || WasmEdge_ModuleInstanceInitWASI == रिक्त || WasmEdge_VMRunWasmFromFile == रिक्त
      || WasmEdge_ResultOK == रिक्त || WasmEdge_StringCreateByCString == रिक्त)
    error (निकास_त्रुटि, 0, "could not find symbol in `libwasmedge.so.0`");

  configure = WasmEdge_ConfigureCreate ();
  जर (UNLIKELY (configure == रिक्त))
    error (निकास_त्रुटि, 0, "could not create wasmedge configure");

  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_BulkMemoryOperations);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_ReferenceTypes);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_SIMD);
  WasmEdge_ConfigureAddHostRegistration (configure, WasmEdge_HostRegistration_Wasi);
  // Check if the necessary environment variables are set
  स्थिरांक अक्षर *plugin_path_env = दो_पर्या ("WASMEDGE_PLUGIN_PATH");
  जर (plugin_path_env != रिक्त)
    WasmEdge_PluginLoadFromPath (plugin_path_env);

  स्थिरांक अक्षर *nnpreload_env = दो_पर्या ("WASMEDGE_WASINN_PRELOAD");
  जर (nnpreload_env != रिक्त)
    WasmEdge_PluginInitWASINN (&nnpreload_env, 1);

  vm = WasmEdge_VMCreate (configure, रिक्त);
  जर (UNLIKELY (vm == रिक्त))
    {
      WasmEdge_ConfigureDelete (configure);
      error (निकास_त्रुटि, 0, "could not create wasmedge vm");
    }

  wasi_module = WasmEdge_VMGetImportModuleContext (vm, WasmEdge_HostRegistration_Wasi);
  जर (UNLIKELY (wasi_module == रिक्त))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (निकास_त्रुटि, 0, "could not get wasmedge wasi module context");
    }

  साठी (अक्षर *स्थिरांक *arg = argv; *arg != रिक्त; ++arg, ++argn)
    ;
  बाह्य अक्षर **environ;
  साठी (अक्षर *स्थिरांक *env = environ; *env != रिक्त; ++env, ++envn)
    ;

  WasmEdge_ModuleInstanceInitWASI (wasi_module, (स्थिरांक अक्षर *स्थिरांक *) &argv[0], argn, (स्थिरांक अक्षर *स्थिरांक *) &environ[0], envn, dirs, 1, रिक्त, 0);

  result = WasmEdge_VMRunWasmFromFile (vm, pathname, WasmEdge_StringCreateByCString ("_start"), रिक्त, 0, रिक्त, 0);

  जर (UNLIKELY (! WasmEdge_ResultOK (result)))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (निकास_त्रुटि, 0, "could not get wasmedge result from VM");
    }

  WasmEdge_VMDelete (vm);
  WasmEdge_ConfigureDelete (configure);
  निकास (निकास_सफल);
}

स्थिर पूर्णांक
wasmedge_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  परत wasm_can_handle_container (container, err);
}

// This works only when the plugin is present in /usr/lib/wasmedge
स्थिर पूर्णांक
libwasmedge_configure_container (शून्य *cookie arg_unused, गणना handler_configure_phase phase,
                                 libcrun_context_t *context arg_unused, libcrun_container_t *container,
                                 स्थिरांक अक्षर *rootfs arg_unused, libcrun_error_t *err)
{
  पूर्णांक ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  अक्षर **container_env = def->process->env;
  bool has_plugin_path = false, has_preload = false;

  साठी (अक्षर **env = container_env; env && *env; env++)
    {
      जर (म_भेदन (*env, "WASMEDGE_PLUGIN_PATH=", 21) == 0)
        has_plugin_path = true;
      अन्यथा जर (म_भेदन (*env, "WASMEDGE_WASINN_PRELOAD=", 24) == 0)
        has_preload = true;
    }

  जर (! has_plugin_path && ! has_preload)
    परत 0;

  जर (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    परत 0;

  // Check if /usr/lib/wasmedge is already present in spec
  जर (def->linux && def->mounts)
    {
      साठी (माप_प्रकार i = 0; i < def->mounts_len; i++)
        {
          जर (म_भेद (def->mounts[i]->destination, "/usr/lib/wasmedge") == 0)
            परत 0;
        }
    }

  // Mount the plugin folder to /usr/lib/wasmedge with specific options
  अक्षर *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/wasmedge ", "/usr/lib/wasmedge", options, 5, err);
  जर (ret < 0)
    {
      जर (crun_error_get_errno (err) != ENOENT)
        परत ret;
      crun_error_release (err);
    }

  परत 0;
}

संरचना custom_handler_s handler_wasmedge = {
  .name = "wasmedge",
  .alias = "wasm",
  .feature_string = "WASM:wasmedge",
  .supports_open_वृक्ष_namespace = false,
  .load = libwasmedge_load,
  .unload = libwasmedge_unload,
  .run_func = libwasmedge_exec,
  .can_handle_container = wasmedge_can_handle_container,
  .configure_container = libwasmedge_configure_container,
};

#endif
