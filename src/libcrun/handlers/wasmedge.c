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

#यदि HAVE_DLOPEN && HAVE_WASMEDGE
स्थिर पूर्णाङ्क
libwasmedge_load (शून्य **cookie, libcrun_error_t *err)
{
  शून्य *handle;

  handle = dlopen ("libwasmedge.so.0", RTLD_NOW);
  यदि (handle == रिक्त)
    फर्काऊ crun_make_error (err, 0, "could not load `libwasmedge.so.0`: `%s`", dlerror ());
  *cookie = handle;

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
libwasmedge_unload (शून्य *cookie, libcrun_error_t *err)
{
  पूर्णाङ्क r;

  यदि (cookie)
    {
      r = dlclose (cookie);
      यदि (UNLIKELY (r < 0))
        फर्काऊ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
libwasmedge_exec (शून्य *cookie, __attribute__ ((unused)) libcrun_container_t *container, अचर वर्ण *pathname, वर्ण *अचर argv[])
{
  WasmEdge_ConfigureContext *(*WasmEdge_ConfigureCreate) (शून्य);
  शून्य (*WasmEdge_ConfigureDelete) (WasmEdge_ConfigureContext *Cxt);
  शून्य (*WasmEdge_ConfigureAddProposal) (WasmEdge_ConfigureContext *Cxt, अचर गणना WasmEdge_Proposal Prop);
  शून्य (*WasmEdge_ConfigureAddHostRegistration) (WasmEdge_ConfigureContext *Cxt, गणना WasmEdge_HostRegistration Host);
  WasmEdge_VMContext *(*WasmEdge_VMCreate) (अचर WasmEdge_ConfigureContext *ConfCxt, WasmEdge_StoreContext *StoreCxt);
  शून्य (*WasmEdge_VMDelete) (WasmEdge_VMContext *Cxt);
  WasmEdge_Result (*WasmEdge_VMRegisterModuleFromFile) (WasmEdge_VMContext *Cxt, WasmEdge_String ModuleName, अचर वर्ण *Path);
  WasmEdge_Result (*WasmEdge_VMRunWasmFromFile) (WasmEdge_VMContext *Cxt, अचर वर्ण *Path, अचर WasmEdge_String FuncName, अचर Wasm_Edge_मान *Params, अचर uint32_t ParamLen, Wasm_Edge_मान *Returns, अचर uint32_t ReturnLen);
  शून्य (*WasmEdge_PluginLoadFromPath) (अचर वर्ण *Path);
  शून्य (*WasmEdge_PluginInitWASINN) (अचर वर्ण *अचर *NNPreloads, अचर uint32_t PreloadsLen);
  bool (*WasmEdge_ResultOK) (अचर WasmEdge_Result Res);
  WasmEdge_String (*WasmEdge_StringCreateByCString) (अचर वर्ण *Str);
  uint32_t argn = 0;
  uint32_t envn = 0;
  अचर वर्ण *dirs[2] = { "/:/", ".:." };
  WasmEdge_ConfigureContext *configure;
  WasmEdge_VMContext *vm;
  WasmEdge_Result result;

  WasmEdge_ModuleInstanceContext *wasi_module;
  WasmEdge_ModuleInstanceContext *(*WasmEdge_VMGetImportModuleContext) (WasmEdge_VMContext *Cxt, अचर गणना WasmEdge_HostRegistration Reg);
  शून्य (*WasmEdge_ModuleInstanceInitWASI) (WasmEdge_ModuleInstanceContext *Cxt, अचर वर्ण *अचर *Args, अचर uint32_t ArgLen, अचर वर्ण *अचर *Envs, अचर uint32_t EnvLen, अचर वर्ण *अचर *Dirs, अचर uint32_t DirLen, अचर वर्ण *अचर *Preopens, अचर uint32_t PreopenLen);
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

  यदि (WasmEdge_ConfigureCreate == रिक्त || WasmEdge_ConfigureDelete == रिक्त || WasmEdge_ConfigureAddProposal == रिक्त
      || WasmEdge_ConfigureAddHostRegistration == रिक्त || WasmEdge_VMCreate == रिक्त || WasmEdge_VMDelete == रिक्त
      || WasmEdge_VMRegisterModuleFromFile == रिक्त || WasmEdge_VMGetImportModuleContext == रिक्त
      || WasmEdge_ModuleInstanceInitWASI == रिक्त || WasmEdge_VMRunWasmFromFile == रिक्त
      || WasmEdge_ResultOK == रिक्त || WasmEdge_StringCreateByCString == रिक्त)
    error (निकास_त्रुटि, 0, "could not find symbol in `libwasmedge.so.0`");

  configure = WasmEdge_ConfigureCreate ();
  यदि (UNLIKELY (configure == रिक्त))
    error (निकास_त्रुटि, 0, "could not create wasmedge configure");

  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_BulkMemoryOperations);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_ReferenceTypes);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_SIMD);
  WasmEdge_ConfigureAddHostRegistration (configure, WasmEdge_HostRegistration_Wasi);
  // Check if the necessary environment variables are set
  अचर वर्ण *plugin_path_env = दो_पर्या ("WASMEDGE_PLUGIN_PATH");
  यदि (plugin_path_env != रिक्त)
    WasmEdge_PluginLoadFromPath (plugin_path_env);

  अचर वर्ण *nnpreload_env = दो_पर्या ("WASMEDGE_WASINN_PRELOAD");
  यदि (nnpreload_env != रिक्त)
    WasmEdge_PluginInitWASINN (&nnpreload_env, 1);

  vm = WasmEdge_VMCreate (configure, रिक्त);
  यदि (UNLIKELY (vm == रिक्त))
    {
      WasmEdge_ConfigureDelete (configure);
      error (निकास_त्रुटि, 0, "could not create wasmedge vm");
    }

  wasi_module = WasmEdge_VMGetImportModuleContext (vm, WasmEdge_HostRegistration_Wasi);
  यदि (UNLIKELY (wasi_module == रिक्त))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (निकास_त्रुटि, 0, "could not get wasmedge wasi module context");
    }

  लागि (वर्ण *अचर *arg = argv; *arg != रिक्त; ++arg, ++argn)
    ;
  बाह्य वर्ण **environ;
  लागि (वर्ण *अचर *env = environ; *env != रिक्त; ++env, ++envn)
    ;

  WasmEdge_ModuleInstanceInitWASI (wasi_module, (अचर वर्ण *अचर *) &argv[0], argn, (अचर वर्ण *अचर *) &environ[0], envn, dirs, 1, रिक्त, 0);

  result = WasmEdge_VMRunWasmFromFile (vm, pathname, WasmEdge_StringCreateByCString ("_start"), रिक्त, 0, रिक्त, 0);

  यदि (UNLIKELY (! WasmEdge_ResultOK (result)))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (निकास_त्रुटि, 0, "could not get wasmedge result from VM");
    }

  WasmEdge_VMDelete (vm);
  WasmEdge_ConfigureDelete (configure);
  निकास (निकास_सफल);
}

स्थिर पूर्णाङ्क
wasmedge_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  फर्काऊ wasm_can_handle_container (container, err);
}

// This works only when the plugin is present in /usr/lib/wasmedge
स्थिर पूर्णाङ्क
libwasmedge_configure_container (शून्य *cookie arg_unused, गणना handler_configure_phase phase,
                                 libcrun_context_t *context arg_unused, libcrun_container_t *container,
                                 अचर वर्ण *rootfs arg_unused, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  वर्ण **container_env = def->process->env;
  bool has_plugin_path = false, has_preload = false;

  लागि (वर्ण **env = container_env; env && *env; env++)
    {
      यदि (म_भेदन (*env, "WASMEDGE_PLUGIN_PATH=", 21) == 0)
        has_plugin_path = true;
      अन्यथा यदि (म_भेदन (*env, "WASMEDGE_WASINN_PRELOAD=", 24) == 0)
        has_preload = true;
    }

  यदि (! has_plugin_path && ! has_preload)
    फर्काऊ 0;

  यदि (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    फर्काऊ 0;

  // Check if /usr/lib/wasmedge is already present in spec
  यदि (def->linux && def->mounts)
    {
      लागि (माप_प्रकार i = 0; i < def->mounts_len; i++)
        {
          यदि (म_भेद (def->mounts[i]->destination, "/usr/lib/wasmedge") == 0)
            फर्काऊ 0;
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
        फर्काऊ ret;
      crun_error_release (err);
    }

  फर्काऊ 0;
}

संरचना custom_handler_s handler_wasmedge = {
  .name = "wasmedge",
  .alias = "wasm",
  .feature_string = "WASM:wasmedge",
  .supports_open_रूख_namespace = false,
  .load = libwasmedge_load,
  .unload = libwasmedge_unload,
  .run_func = libwasmedge_exec,
  .can_handle_container = wasmedge_can_handle_container,
  .configure_container = libwasmedge_configure_container,
};

#endif
