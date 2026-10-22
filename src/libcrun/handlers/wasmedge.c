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
#ಅನ್ತರ್ಭಾವ "handler-utils.h"
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMEDGE
#  include <wasmedge/wasmedge.h>
#endif

#ಯದಿ HAVE_DLOPEN && HAVE_WASMEDGE
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libwasmedge_load (ಶೂನ್ಯ **cookie, libcrun_error_t *err)
{
  ಶೂನ್ಯ *handle;

  handle = dlopen ("libwasmedge.so.0", RTLD_NOW);
  ಯದಿ (handle == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not load `libwasmedge.so.0`: `%s`", dlerror ());
  *cookie = handle;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libwasmedge_unload (ಶೂನ್ಯ *cookie, libcrun_error_t *err)
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
libwasmedge_exec (ಶೂನ್ಯ *cookie, __attribute__ ((unused)) libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *pathname, ವರ್ಣ *ಧ್ರುವ argv[])
{
  WasmEdge_ConfigureContext *(*WasmEdge_ConfigureCreate) (ಶೂನ್ಯ);
  ಶೂನ್ಯ (*WasmEdge_ConfigureDelete) (WasmEdge_ConfigureContext *Cxt);
  ಶೂನ್ಯ (*WasmEdge_ConfigureAddProposal) (WasmEdge_ConfigureContext *Cxt, ಧ್ರುವ ಗಣನಾ WasmEdge_Proposal Prop);
  ಶೂನ್ಯ (*WasmEdge_ConfigureAddHostRegistration) (WasmEdge_ConfigureContext *Cxt, ಗಣನಾ WasmEdge_HostRegistration Host);
  WasmEdge_VMContext *(*WasmEdge_VMCreate) (ಧ್ರುವ WasmEdge_ConfigureContext *ConfCxt, WasmEdge_StoreContext *StoreCxt);
  ಶೂನ್ಯ (*WasmEdge_VMDelete) (WasmEdge_VMContext *Cxt);
  WasmEdge_Result (*WasmEdge_VMRegisterModuleFromFile) (WasmEdge_VMContext *Cxt, WasmEdge_String ModuleName, ಧ್ರುವ ವರ್ಣ *Path);
  WasmEdge_Result (*WasmEdge_VMRunWasmFromFile) (WasmEdge_VMContext *Cxt, ಧ್ರುವ ವರ್ಣ *Path, ಧ್ರುವ WasmEdge_String FuncName, ಧ್ರುವ Wasm_Edge_ಮೌಲ್ಯ *Params, ಧ್ರುವ uint32_t ParamLen, Wasm_Edge_ಮೌಲ್ಯ *Returns, ಧ್ರುವ uint32_t ReturnLen);
  ಶೂನ್ಯ (*WasmEdge_PluginLoadFromPath) (ಧ್ರುವ ವರ್ಣ *Path);
  ಶೂನ್ಯ (*WasmEdge_PluginInitWASINN) (ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ *NNPreloads, ಧ್ರುವ uint32_t PreloadsLen);
  bool (*WasmEdge_ResultOK) (ಧ್ರುವ WasmEdge_Result Res);
  WasmEdge_String (*WasmEdge_StringCreateByCString) (ಧ್ರುವ ವರ್ಣ *Str);
  uint32_t argn = 0;
  uint32_t envn = 0;
  ಧ್ರುವ ವರ್ಣ *dirs[2] = { "/:/", ".:." };
  WasmEdge_ConfigureContext *configure;
  WasmEdge_VMContext *vm;
  WasmEdge_Result result;

  WasmEdge_ModuleInstanceContext *wasi_module;
  WasmEdge_ModuleInstanceContext *(*WasmEdge_VMGetImportModuleContext) (WasmEdge_VMContext *Cxt, ಧ್ರುವ ಗಣನಾ WasmEdge_HostRegistration Reg);
  ಶೂನ್ಯ (*WasmEdge_ModuleInstanceInitWASI) (WasmEdge_ModuleInstanceContext *Cxt, ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ *Args, ಧ್ರುವ uint32_t ArgLen, ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ *Envs, ಧ್ರುವ uint32_t EnvLen, ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ *Dirs, ಧ್ರುವ uint32_t DirLen, ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ *Preopens, ಧ್ರುವ uint32_t PreopenLen);
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

  ಯದಿ (WasmEdge_ConfigureCreate == ರಿಕ್ತ || WasmEdge_ConfigureDelete == ರಿಕ್ತ || WasmEdge_ConfigureAddProposal == ರಿಕ್ತ
      || WasmEdge_ConfigureAddHostRegistration == ರಿಕ್ತ || WasmEdge_VMCreate == ರಿಕ್ತ || WasmEdge_VMDelete == ರಿಕ್ತ
      || WasmEdge_VMRegisterModuleFromFile == ರಿಕ್ತ || WasmEdge_VMGetImportModuleContext == ರಿಕ್ತ
      || WasmEdge_ModuleInstanceInitWASI == ರಿಕ್ತ || WasmEdge_VMRunWasmFromFile == ರಿಕ್ತ
      || WasmEdge_ResultOK == ರಿಕ್ತ || WasmEdge_StringCreateByCString == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmedge.so.0`");

  configure = WasmEdge_ConfigureCreate ();
  ಯದಿ (UNLIKELY (configure == ರಿಕ್ತ))
    error (EXIT_FAILURE, 0, "could not create wasmedge configure");

  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_BulkMemoryOperations);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_ReferenceTypes);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_SIMD);
  WasmEdge_ConfigureAddHostRegistration (configure, WasmEdge_HostRegistration_Wasi);
  // Check if the necessary environment variables are set
  ಧ್ರುವ ವರ್ಣ *plugin_path_env = getenv ("WASMEDGE_PLUGIN_PATH");
  ಯದಿ (plugin_path_env != ರಿಕ್ತ)
    WasmEdge_PluginLoadFromPath (plugin_path_env);

  ಧ್ರುವ ವರ್ಣ *nnpreload_env = getenv ("WASMEDGE_WASINN_PRELOAD");
  ಯದಿ (nnpreload_env != ರಿಕ್ತ)
    WasmEdge_PluginInitWASINN (&nnpreload_env, 1);

  vm = WasmEdge_VMCreate (configure, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (vm == ರಿಕ್ತ))
    {
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not create wasmedge vm");
    }

  wasi_module = WasmEdge_VMGetImportModuleContext (vm, WasmEdge_HostRegistration_Wasi);
  ಯದಿ (UNLIKELY (wasi_module == ರಿಕ್ತ))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge wasi module context");
    }

  ಪ್ರತಿ (ವರ್ಣ *ಧ್ರುವ *arg = argv; *arg != ರಿಕ್ತ; ++arg, ++argn)
    ;
  ಬಾಹ್ಯ ವರ್ಣ **environ;
  ಪ್ರತಿ (ವರ್ಣ *ಧ್ರುವ *env = environ; *env != ರಿಕ್ತ; ++env, ++envn)
    ;

  WasmEdge_ModuleInstanceInitWASI (wasi_module, (ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ *) &argv[0], argn, (ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ *) &environ[0], envn, dirs, 1, ರಿಕ್ತ, 0);

  result = WasmEdge_VMRunWasmFromFile (vm, pathname, WasmEdge_StringCreateByCString ("_start"), ರಿಕ್ತ, 0, ರಿಕ್ತ, 0);

  ಯದಿ (UNLIKELY (! WasmEdge_ResultOK (result)))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge result from VM");
    }

  WasmEdge_VMDelete (vm);
  WasmEdge_ConfigureDelete (configure);
  exit (EXIT_SUCCESS);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
wasmedge_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ wasm_can_handle_container (container, err);
}

// This works only when the plugin is present in /usr/lib/wasmedge
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libwasmedge_configure_container (ಶೂನ್ಯ *cookie arg_unused, ಗಣನಾ handler_configure_phase phase,
                                 libcrun_context_t *context arg_unused, libcrun_container_t *container,
                                 ಧ್ರುವ ವರ್ಣ *rootfs arg_unused, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  ವರ್ಣ **container_env = def->process->env;
  bool has_plugin_path = false, has_preload = false;

  ಪ್ರತಿ (ವರ್ಣ **env = container_env; env && *env; env++)
    {
      ಯದಿ (strncmp (*env, "WASMEDGE_PLUGIN_PATH=", 21) == 0)
        has_plugin_path = true;
      ಅನ್ಯಥಾ ಯದಿ (strncmp (*env, "WASMEDGE_WASINN_PRELOAD=", 24) == 0)
        has_preload = true;
    }

  ಯದಿ (! has_plugin_path && ! has_preload)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    ಪ್ರತ್ಯಾಗಮ 0;

  // Check if /usr/lib/wasmedge is already present in spec
  ಯದಿ (def->linux && def->mounts)
    {
      ಪ್ರತಿ (size_t i = 0; i < def->mounts_len; i++)
        {
          ಯದಿ (strcmp (def->mounts[i]->destination, "/usr/lib/wasmedge") == 0)
            ಪ್ರತ್ಯಾಗಮ 0;
        }
    }

  // Mount the plugin folder to /usr/lib/wasmedge with specific options
  ವರ್ಣ *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/wasmedge ", "/usr/lib/wasmedge", options, 5, err);
  ಯದಿ (ret < 0)
    {
      ಯದಿ (crun_error_get_errno (err) != ENOENT)
        ಪ್ರತ್ಯಾಗಮ ret;
      crun_error_release (err);
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ custom_handler_s handler_wasmedge = {
  .name = "wasmedge",
  .alias = "wasm",
  .feature_string = "WASM:wasmedge",
  .supports_open_ವೃಕ್ಷ_namespace = false,
  .load = libwasmedge_load,
  .unload = libwasmedge_unload,
  .run_func = libwasmedge_exec,
  .can_handle_container = wasmedge_can_handle_container,
  .configure_container = libwasmedge_configure_container,
};

#endif
