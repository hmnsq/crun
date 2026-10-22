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
#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश "../custom-handler.h"
#समावेश "../container.h"
#समावेश "../utils.h"
#समावेश "../linux.h"
#समावेश "handler-utils.h"
#समावेश <unistd.h>
#समावेश <sys/stat.h>
#समावेश <errno.h>
#समावेश <sys/types.h>
#समावेश <fcntl.h>
#समावेश <sched.h>

#अगर_घोषित HAVE_DLOPEN
#  include <dlfcn.h>
#पूर्ण_अगर

#अगर_घोषित HAVE_WASMEDGE
#  include <wasmedge/wasmedge.h>
#पूर्ण_अगर

#अगर HAVE_DLOPEN && HAVE_WASMEDGE
अटल पूर्णांक
libwasmedge_load (व्योम **cookie, libcrun_error_t *err)
{
  व्योम *handle;

  handle = dlopen ("libwasmedge.so.0", RTLD_NOW);
  अगर (handle == शून्य)
    वापस crun_make_error (err, 0, "could not load `libwasmedge.so.0`: `%s`", dlerror ());
  *cookie = handle;

  वापस 0;
}

अटल पूर्णांक
libwasmedge_unload (व्योम *cookie, libcrun_error_t *err)
{
  पूर्णांक r;

  अगर (cookie)
    {
      r = dlclose (cookie);
      अगर (UNLIKELY (r < 0))
        वापस crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  वापस 0;
}

अटल पूर्णांक
libwasmedge_exec (व्योम *cookie, __attribute__ ((unused)) libcrun_container_t *container, स्थिर अक्षर *pathname, अक्षर *स्थिर argv[])
{
  WasmEdge_ConfigureContext *(*WasmEdge_ConfigureCreate) (व्योम);
  व्योम (*WasmEdge_ConfigureDelete) (WasmEdge_ConfigureContext *Cxt);
  व्योम (*WasmEdge_ConfigureAddProposal) (WasmEdge_ConfigureContext *Cxt, स्थिर क्रमागत WasmEdge_Proposal Prop);
  व्योम (*WasmEdge_ConfigureAddHostRegistration) (WasmEdge_ConfigureContext *Cxt, क्रमागत WasmEdge_HostRegistration Host);
  WasmEdge_VMContext *(*WasmEdge_VMCreate) (स्थिर WasmEdge_ConfigureContext *ConfCxt, WasmEdge_StoreContext *StoreCxt);
  व्योम (*WasmEdge_VMDelete) (WasmEdge_VMContext *Cxt);
  Wasm_Edge_परिणाम (*WasmEdge_VMRegisterModuleFromFile) (WasmEdge_VMContext *Cxt, WasmEdge_String ModuleName, स्थिर अक्षर *Path);
  Wasm_Edge_परिणाम (*WasmEdge_VMRunWasmFromFile) (WasmEdge_VMContext *Cxt, स्थिर अक्षर *Path, स्थिर WasmEdge_String FuncName, स्थिर Wasm_Edge_मूल्य *Params, स्थिर uint32_t ParamLen, Wasm_Edge_मूल्य *Returns, स्थिर uint32_t ReturnLen);
  व्योम (*WasmEdge_PluginLoadFromPath) (स्थिर अक्षर *Path);
  व्योम (*WasmEdge_PluginInitWASINN) (स्थिर अक्षर *स्थिर *NNPreloads, स्थिर uint32_t PreloadsLen);
  bool (*Wasm_Edge_परिणाम_OK) (स्थिर Wasm_Edge_परिणाम Res);
  WasmEdge_String (*WasmEdge_StringCreateByCString) (स्थिर अक्षर *Str);
  uint32_t argn = 0;
  uint32_t envn = 0;
  स्थिर अक्षर *dirs[2] = { "/:/", ".:." };
  WasmEdge_ConfigureContext *configure;
  WasmEdge_VMContext *vm;
  Wasm_Edge_परिणाम परिणाम;

  WasmEdge_ModuleInstanceContext *wasi_module;
  WasmEdge_ModuleInstanceContext *(*WasmEdge_VMGetImportModuleContext) (WasmEdge_VMContext *Cxt, स्थिर क्रमागत WasmEdge_HostRegistration Reg);
  व्योम (*WasmEdge_ModuleInstanceInitWASI) (WasmEdge_ModuleInstanceContext *Cxt, स्थिर अक्षर *स्थिर *Args, स्थिर uint32_t ArgLen, स्थिर अक्षर *स्थिर *Envs, स्थिर uint32_t EnvLen, स्थिर अक्षर *स्थिर *Dirs, स्थिर uint32_t DirLen, स्थिर अक्षर *स्थिर *Preopens, स्थिर uint32_t PreopenLen);
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
  Wasm_Edge_परिणाम_OK = dlsym (cookie, "WasmEdge_ResultOK");
  WasmEdge_StringCreateByCString = dlsym (cookie, "WasmEdge_StringCreateByCString");

  अगर (WasmEdge_ConfigureCreate == शून्य || WasmEdge_ConfigureDelete == शून्य || WasmEdge_ConfigureAddProposal == शून्य
      || WasmEdge_ConfigureAddHostRegistration == शून्य || WasmEdge_VMCreate == शून्य || WasmEdge_VMDelete == शून्य
      || WasmEdge_VMRegisterModuleFromFile == शून्य || WasmEdge_VMGetImportModuleContext == शून्य
      || WasmEdge_ModuleInstanceInitWASI == शून्य || WasmEdge_VMRunWasmFromFile == शून्य
      || Wasm_Edge_परिणाम_OK == शून्य || WasmEdge_StringCreateByCString == शून्य)
    error (निकास_त्रुटि, 0, "could not find symbol in `libwasmedge.so.0`");

  configure = WasmEdge_ConfigureCreate ();
  अगर (UNLIKELY (configure == शून्य))
    error (निकास_त्रुटि, 0, "could not create wasmedge configure");

  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_BulkMemoryOperations);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_ReferenceTypes);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_SIMD);
  WasmEdge_ConfigureAddHostRegistration (configure, WasmEdge_HostRegistration_Wasi);
  // Check if the necessary environment variables are set
  स्थिर अक्षर *plugin_path_env = दो_पर्या ("WASMEDGE_PLUGIN_PATH");
  अगर (plugin_path_env != शून्य)
    WasmEdge_PluginLoadFromPath (plugin_path_env);

  स्थिर अक्षर *nnpreload_env = दो_पर्या ("WASMEDGE_WASINN_PRELOAD");
  अगर (nnpreload_env != शून्य)
    WasmEdge_PluginInitWASINN (&nnpreload_env, 1);

  vm = WasmEdge_VMCreate (configure, शून्य);
  अगर (UNLIKELY (vm == शून्य))
    {
      WasmEdge_ConfigureDelete (configure);
      error (निकास_त्रुटि, 0, "could not create wasmedge vm");
    }

  wasi_module = WasmEdge_VMGetImportModuleContext (vm, WasmEdge_HostRegistration_Wasi);
  अगर (UNLIKELY (wasi_module == शून्य))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (निकास_त्रुटि, 0, "could not get wasmedge wasi module context");
    }

  क्रम (अक्षर *स्थिर *arg = argv; *arg != शून्य; ++arg, ++argn)
    ;
  बाह्य अक्षर **environ;
  क्रम (अक्षर *स्थिर *env = environ; *env != शून्य; ++env, ++envn)
    ;

  WasmEdge_ModuleInstanceInitWASI (wasi_module, (स्थिर अक्षर *स्थिर *) &argv[0], argn, (स्थिर अक्षर *स्थिर *) &environ[0], envn, dirs, 1, शून्य, 0);

  परिणाम = WasmEdge_VMRunWasmFromFile (vm, pathname, WasmEdge_StringCreateByCString ("_start"), शून्य, 0, शून्य, 0);

  अगर (UNLIKELY (! Wasm_Edge_परिणाम_OK (परिणाम)))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (निकास_त्रुटि, 0, "could not get wasmedge result from VM");
    }

  WasmEdge_VMDelete (vm);
  WasmEdge_ConfigureDelete (configure);
  निकास (निकास_सफल);
}

अटल पूर्णांक
wasmedge_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  वापस wasm_can_handle_container (container, err);
}

// This works only when the plugin is present in /usr/lib/wasmedge
अटल पूर्णांक
libwasmedge_configure_container (व्योम *cookie arg_unused, क्रमागत handler_configure_phase phase,
                                 libcrun_context_t *context arg_unused, libcrun_container_t *container,
                                 स्थिर अक्षर *rootfs arg_unused, libcrun_error_t *err)
{
  पूर्णांक ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  अक्षर **container_env = def->process->env;
  bool has_plugin_path = false, has_preload = false;

  क्रम (अक्षर **env = container_env; env && *env; env++)
    {
      अगर (म_भेदन (*env, "WASMEDGE_PLUGIN_PATH=", 21) == 0)
        has_plugin_path = true;
      अन्यथा अगर (म_भेदन (*env, "WASMEDGE_WASINN_PRELOAD=", 24) == 0)
        has_preload = true;
    }

  अगर (! has_plugin_path && ! has_preload)
    वापस 0;

  अगर (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    वापस 0;

  // Check if /usr/lib/wasmedge is already present in spec
  अगर (def->linux && def->mounts)
    {
      क्रम (माप_प्रकार i = 0; i < def->mounts_len; i++)
        {
          अगर (म_भेद (def->mounts[i]->destination, "/usr/lib/wasmedge") == 0)
            वापस 0;
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
  अगर (ret < 0)
    {
      अगर (crun_error_get_errno (err) != ENOENT)
        वापस ret;
      crun_error_release (err);
    }

  वापस 0;
}

काष्ठा custom_handler_s handler_wasmedge = {
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

#पूर्ण_अगर
