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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "../custom-handler.h"
#அந்தர்பாவ "../container.h"
#அந்தர்பாவ "../utils.h"
#அந்தர்பாவ "../linux.h"
#அந்தர்பாவ "handler-utils.h"
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMEDGE
#  include <wasmedge/wasmedge.h>
#endif

#எனில் HAVE_DLOPEN && HAVE_WASMEDGE
நிலையான முழு
libwasmedge_load (வெறுமை **cookie, libcrun_error_t *err)
{
  வெறுமை *handle;

  handle = dlopen ("libwasmedge.so.0", RTLD_NOW);
  எனில் (handle == வெற்று)
    திரும்பு crun_make_error (err, 0, "could not load `libwasmedge.so.0`: `%s`", dlerror ());
  *cookie = handle;

  திரும்பு 0;
}

நிலையான முழு
libwasmedge_unload (வெறுமை *cookie, libcrun_error_t *err)
{
  முழு r;

  எனில் (cookie)
    {
      r = dlclose (cookie);
      எனில் (UNLIKELY (r < 0))
        திரும்பு crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  திரும்பு 0;
}

நிலையான முழு
libwasmedge_exec (வெறுமை *cookie, __attribute__ ((unused)) libcrun_container_t *container, மாறிலி எழுத்து *pathname, எழுத்து *மாறிலி argv[])
{
  WasmEdge_ConfigureContext *(*WasmEdge_ConfigureCreate) (வெறுமை);
  வெறுமை (*WasmEdge_ConfigureDelete) (WasmEdge_ConfigureContext *Cxt);
  வெறுமை (*WasmEdge_ConfigureAddProposal) (WasmEdge_ConfigureContext *Cxt, மாறிலி எண்ணிக்கை WasmEdge_Proposal Prop);
  வெறுமை (*WasmEdge_ConfigureAddHostRegistration) (WasmEdge_ConfigureContext *Cxt, எண்ணிக்கை WasmEdge_HostRegistration Host);
  WasmEdge_VMContext *(*WasmEdge_VMCreate) (மாறிலி WasmEdge_ConfigureContext *ConfCxt, WasmEdge_StoreContext *StoreCxt);
  வெறுமை (*WasmEdge_VMDelete) (WasmEdge_VMContext *Cxt);
  WasmEdge_Result (*WasmEdge_VMRegisterModuleFromFile) (WasmEdge_VMContext *Cxt, WasmEdge_String ModuleName, மாறிலி எழுத்து *Path);
  WasmEdge_Result (*WasmEdge_VMRunWasmFromFile) (WasmEdge_VMContext *Cxt, மாறிலி எழுத்து *Path, மாறிலி WasmEdge_String FuncName, மாறிலி Wasm_Edge_மதிப்பு *Params, மாறிலி uint32_t ParamLen, Wasm_Edge_மதிப்பு *Returns, மாறிலி uint32_t ReturnLen);
  வெறுமை (*WasmEdge_PluginLoadFromPath) (மாறிலி எழுத்து *Path);
  வெறுமை (*WasmEdge_PluginInitWASINN) (மாறிலி எழுத்து *மாறிலி *NNPreloads, மாறிலி uint32_t PreloadsLen);
  bool (*WasmEdge_ResultOK) (மாறிலி WasmEdge_Result Res);
  WasmEdge_String (*WasmEdge_StringCreateByCString) (மாறிலி எழுத்து *Str);
  uint32_t argn = 0;
  uint32_t envn = 0;
  மாறிலி எழுத்து *dirs[2] = { "/:/", ".:." };
  WasmEdge_ConfigureContext *configure;
  WasmEdge_VMContext *vm;
  WasmEdge_Result result;

  WasmEdge_ModuleInstanceContext *wasi_module;
  WasmEdge_ModuleInstanceContext *(*WasmEdge_VMGetImportModuleContext) (WasmEdge_VMContext *Cxt, மாறிலி எண்ணிக்கை WasmEdge_HostRegistration Reg);
  வெறுமை (*WasmEdge_ModuleInstanceInitWASI) (WasmEdge_ModuleInstanceContext *Cxt, மாறிலி எழுத்து *மாறிலி *Args, மாறிலி uint32_t ArgLen, மாறிலி எழுத்து *மாறிலி *Envs, மாறிலி uint32_t EnvLen, மாறிலி எழுத்து *மாறிலி *Dirs, மாறிலி uint32_t DirLen, மாறிலி எழுத்து *மாறிலி *Preopens, மாறிலி uint32_t PreopenLen);
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

  எனில் (WasmEdge_ConfigureCreate == வெற்று || WasmEdge_ConfigureDelete == வெற்று || WasmEdge_ConfigureAddProposal == வெற்று
      || WasmEdge_ConfigureAddHostRegistration == வெற்று || WasmEdge_VMCreate == வெற்று || WasmEdge_VMDelete == வெற்று
      || WasmEdge_VMRegisterModuleFromFile == வெற்று || WasmEdge_VMGetImportModuleContext == வெற்று
      || WasmEdge_ModuleInstanceInitWASI == வெற்று || WasmEdge_VMRunWasmFromFile == வெற்று
      || WasmEdge_ResultOK == வெற்று || WasmEdge_StringCreateByCString == வெற்று)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmedge.so.0`");

  configure = WasmEdge_ConfigureCreate ();
  எனில் (UNLIKELY (configure == வெற்று))
    error (EXIT_FAILURE, 0, "could not create wasmedge configure");

  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_BulkMemoryOperations);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_ReferenceTypes);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_SIMD);
  WasmEdge_ConfigureAddHostRegistration (configure, WasmEdge_HostRegistration_Wasi);
  // Check if the necessary environment variables are set
  மாறிலி எழுத்து *plugin_path_env = getenv ("WASMEDGE_PLUGIN_PATH");
  எனில் (plugin_path_env != வெற்று)
    WasmEdge_PluginLoadFromPath (plugin_path_env);

  மாறிலி எழுத்து *nnpreload_env = getenv ("WASMEDGE_WASINN_PRELOAD");
  எனில் (nnpreload_env != வெற்று)
    WasmEdge_PluginInitWASINN (&nnpreload_env, 1);

  vm = WasmEdge_VMCreate (configure, வெற்று);
  எனில் (UNLIKELY (vm == வெற்று))
    {
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not create wasmedge vm");
    }

  wasi_module = WasmEdge_VMGetImportModuleContext (vm, WasmEdge_HostRegistration_Wasi);
  எனில் (UNLIKELY (wasi_module == வெற்று))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge wasi module context");
    }

  க்கு (எழுத்து *மாறிலி *arg = argv; *arg != வெற்று; ++arg, ++argn)
    ;
  வெளி எழுத்து **environ;
  க்கு (எழுத்து *மாறிலி *env = environ; *env != வெற்று; ++env, ++envn)
    ;

  WasmEdge_ModuleInstanceInitWASI (wasi_module, (மாறிலி எழுத்து *மாறிலி *) &argv[0], argn, (மாறிலி எழுத்து *மாறிலி *) &environ[0], envn, dirs, 1, வெற்று, 0);

  result = WasmEdge_VMRunWasmFromFile (vm, pathname, WasmEdge_StringCreateByCString ("_start"), வெற்று, 0, வெற்று, 0);

  எனில் (UNLIKELY (! WasmEdge_ResultOK (result)))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge result from VM");
    }

  WasmEdge_VMDelete (vm);
  WasmEdge_ConfigureDelete (configure);
  exit (EXIT_SUCCESS);
}

நிலையான முழு
wasmedge_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  திரும்பு wasm_can_handle_container (container, err);
}

// This works only when the plugin is present in /usr/lib/wasmedge
நிலையான முழு
libwasmedge_configure_container (வெறுமை *cookie arg_unused, எண்ணிக்கை handler_configure_phase phase,
                                 libcrun_context_t *context arg_unused, libcrun_container_t *container,
                                 மாறிலி எழுத்து *rootfs arg_unused, libcrun_error_t *err)
{
  முழு ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  எழுத்து **container_env = def->process->env;
  bool has_plugin_path = false, has_preload = false;

  க்கு (எழுத்து **env = container_env; env && *env; env++)
    {
      எனில் (strncmp (*env, "WASMEDGE_PLUGIN_PATH=", 21) == 0)
        has_plugin_path = true;
      இல்லையெனில் எனில் (strncmp (*env, "WASMEDGE_WASINN_PRELOAD=", 24) == 0)
        has_preload = true;
    }

  எனில் (! has_plugin_path && ! has_preload)
    திரும்பு 0;

  எனில் (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    திரும்பு 0;

  // Check if /usr/lib/wasmedge is already present in spec
  எனில் (def->linux && def->mounts)
    {
      க்கு (size_t i = 0; i < def->mounts_len; i++)
        {
          எனில் (strcmp (def->mounts[i]->destination, "/usr/lib/wasmedge") == 0)
            திரும்பு 0;
        }
    }

  // Mount the plugin folder to /usr/lib/wasmedge with specific options
  எழுத்து *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/wasmedge ", "/usr/lib/wasmedge", options, 5, err);
  எனில் (ret < 0)
    {
      எனில் (crun_error_get_errno (err) != ENOENT)
        திரும்பு ret;
      crun_error_release (err);
    }

  திரும்பு 0;
}

கட்டமைப்பு custom_handler_s handler_wasmedge = {
  .name = "wasmedge",
  .alias = "wasm",
  .feature_string = "WASM:wasmedge",
  .supports_open_மரம்_namespace = false,
  .load = libwasmedge_load,
  .unload = libwasmedge_unload,
  .run_func = libwasmedge_exec,
  .can_handle_container = wasmedge_can_handle_container,
  .configure_container = libwasmedge_configure_container,
};

#endif
