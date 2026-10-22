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
#അന്തര്ഭാവ "handler-utils.h"
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMEDGE
#  include <wasmedge/wasmedge.h>
#endif

#എങ്കിൽ HAVE_DLOPEN && HAVE_WASMEDGE
സ്ഥിതം പൂർണ്ണസംഖ്യ
libwasmedge_load (ശൂന്യം **cookie, libcrun_error_t *err)
{
  ശൂന്യം *handle;

  handle = dlopen ("libwasmedge.so.0", RTLD_NOW);
  എങ്കിൽ (handle == ശൂന്യ)
    തിരികെ crun_make_error (err, 0, "could not load `libwasmedge.so.0`: `%s`", dlerror ());
  *cookie = handle;

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libwasmedge_unload (ശൂന്യം *cookie, libcrun_error_t *err)
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
libwasmedge_exec (ശൂന്യം *cookie, __attribute__ ((unused)) libcrun_container_t *container, സ്ഥിരം അക്ഷരം *pathname, അക്ഷരം *സ്ഥിരം argv[])
{
  WasmEdge_ConfigureContext *(*WasmEdge_ConfigureCreate) (ശൂന്യം);
  ശൂന്യം (*WasmEdge_ConfigureDelete) (WasmEdge_ConfigureContext *Cxt);
  ശൂന്യം (*WasmEdge_ConfigureAddProposal) (WasmEdge_ConfigureContext *Cxt, സ്ഥിരം എണ്ണൽ WasmEdge_Proposal Prop);
  ശൂന്യം (*WasmEdge_ConfigureAddHostRegistration) (WasmEdge_ConfigureContext *Cxt, എണ്ണൽ WasmEdge_HostRegistration Host);
  WasmEdge_VMContext *(*WasmEdge_VMCreate) (സ്ഥിരം WasmEdge_ConfigureContext *ConfCxt, WasmEdge_StoreContext *StoreCxt);
  ശൂന്യം (*WasmEdge_VMDelete) (WasmEdge_VMContext *Cxt);
  WasmEdge_Result (*WasmEdge_VMRegisterModuleFromFile) (WasmEdge_VMContext *Cxt, WasmEdge_String ModuleName, സ്ഥിരം അക്ഷരം *Path);
  WasmEdge_Result (*WasmEdge_VMRunWasmFromFile) (WasmEdge_VMContext *Cxt, സ്ഥിരം അക്ഷരം *Path, സ്ഥിരം WasmEdge_String FuncName, സ്ഥിരം Wasm_Edge_മൂല്യം *Params, സ്ഥിരം uint32_t ParamLen, Wasm_Edge_മൂല്യം *Returns, സ്ഥിരം uint32_t ReturnLen);
  ശൂന്യം (*WasmEdge_PluginLoadFromPath) (സ്ഥിരം അക്ഷരം *Path);
  ശൂന്യം (*WasmEdge_PluginInitWASINN) (സ്ഥിരം അക്ഷരം *സ്ഥിരം *NNPreloads, സ്ഥിരം uint32_t PreloadsLen);
  bool (*WasmEdge_ResultOK) (സ്ഥിരം WasmEdge_Result Res);
  WasmEdge_String (*WasmEdge_StringCreateByCString) (സ്ഥിരം അക്ഷരം *Str);
  uint32_t argn = 0;
  uint32_t envn = 0;
  സ്ഥിരം അക്ഷരം *dirs[2] = { "/:/", ".:." };
  WasmEdge_ConfigureContext *configure;
  WasmEdge_VMContext *vm;
  WasmEdge_Result result;

  WasmEdge_ModuleInstanceContext *wasi_module;
  WasmEdge_ModuleInstanceContext *(*WasmEdge_VMGetImportModuleContext) (WasmEdge_VMContext *Cxt, സ്ഥിരം എണ്ണൽ WasmEdge_HostRegistration Reg);
  ശൂന്യം (*WasmEdge_ModuleInstanceInitWASI) (WasmEdge_ModuleInstanceContext *Cxt, സ്ഥിരം അക്ഷരം *സ്ഥിരം *Args, സ്ഥിരം uint32_t ArgLen, സ്ഥിരം അക്ഷരം *സ്ഥിരം *Envs, സ്ഥിരം uint32_t EnvLen, സ്ഥിരം അക്ഷരം *സ്ഥിരം *Dirs, സ്ഥിരം uint32_t DirLen, സ്ഥിരം അക്ഷരം *സ്ഥിരം *Preopens, സ്ഥിരം uint32_t PreopenLen);
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

  എങ്കിൽ (WasmEdge_ConfigureCreate == ശൂന്യ || WasmEdge_ConfigureDelete == ശൂന്യ || WasmEdge_ConfigureAddProposal == ശൂന്യ
      || WasmEdge_ConfigureAddHostRegistration == ശൂന്യ || WasmEdge_VMCreate == ശൂന്യ || WasmEdge_VMDelete == ശൂന്യ
      || WasmEdge_VMRegisterModuleFromFile == ശൂന്യ || WasmEdge_VMGetImportModuleContext == ശൂന്യ
      || WasmEdge_ModuleInstanceInitWASI == ശൂന്യ || WasmEdge_VMRunWasmFromFile == ശൂന്യ
      || WasmEdge_ResultOK == ശൂന്യ || WasmEdge_StringCreateByCString == ശൂന്യ)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmedge.so.0`");

  configure = WasmEdge_ConfigureCreate ();
  എങ്കിൽ (UNLIKELY (configure == ശൂന്യ))
    error (EXIT_FAILURE, 0, "could not create wasmedge configure");

  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_BulkMemoryOperations);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_ReferenceTypes);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_SIMD);
  WasmEdge_ConfigureAddHostRegistration (configure, WasmEdge_HostRegistration_Wasi);
  // Check if the necessary environment variables are set
  സ്ഥിരം അക്ഷരം *plugin_path_env = getenv ("WASMEDGE_PLUGIN_PATH");
  എങ്കിൽ (plugin_path_env != ശൂന്യ)
    WasmEdge_PluginLoadFromPath (plugin_path_env);

  സ്ഥിരം അക്ഷരം *nnpreload_env = getenv ("WASMEDGE_WASINN_PRELOAD");
  എങ്കിൽ (nnpreload_env != ശൂന്യ)
    WasmEdge_PluginInitWASINN (&nnpreload_env, 1);

  vm = WasmEdge_VMCreate (configure, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (vm == ശൂന്യ))
    {
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not create wasmedge vm");
    }

  wasi_module = WasmEdge_VMGetImportModuleContext (vm, WasmEdge_HostRegistration_Wasi);
  എങ്കിൽ (UNLIKELY (wasi_module == ശൂന്യ))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge wasi module context");
    }

  വേണ്ടി (അക്ഷരം *സ്ഥിരം *arg = argv; *arg != ശൂന്യ; ++arg, ++argn)
    ;
  ബാഹ്യ അക്ഷരം **environ;
  വേണ്ടി (അക്ഷരം *സ്ഥിരം *env = environ; *env != ശൂന്യ; ++env, ++envn)
    ;

  WasmEdge_ModuleInstanceInitWASI (wasi_module, (സ്ഥിരം അക്ഷരം *സ്ഥിരം *) &argv[0], argn, (സ്ഥിരം അക്ഷരം *സ്ഥിരം *) &environ[0], envn, dirs, 1, ശൂന്യ, 0);

  result = WasmEdge_VMRunWasmFromFile (vm, pathname, WasmEdge_StringCreateByCString ("_start"), ശൂന്യ, 0, ശൂന്യ, 0);

  എങ്കിൽ (UNLIKELY (! WasmEdge_ResultOK (result)))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge result from VM");
    }

  WasmEdge_VMDelete (vm);
  WasmEdge_ConfigureDelete (configure);
  exit (EXIT_SUCCESS);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
wasmedge_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  തിരികെ wasm_can_handle_container (container, err);
}

// This works only when the plugin is present in /usr/lib/wasmedge
സ്ഥിതം പൂർണ്ണസംഖ്യ
libwasmedge_configure_container (ശൂന്യം *cookie arg_unused, എണ്ണൽ handler_configure_phase phase,
                                 libcrun_context_t *context arg_unused, libcrun_container_t *container,
                                 സ്ഥിരം അക്ഷരം *rootfs arg_unused, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  അക്ഷരം **container_env = def->process->env;
  bool has_plugin_path = false, has_preload = false;

  വേണ്ടി (അക്ഷരം **env = container_env; env && *env; env++)
    {
      എങ്കിൽ (strncmp (*env, "WASMEDGE_PLUGIN_PATH=", 21) == 0)
        has_plugin_path = true;
      അല്ലെങ്കിൽ എങ്കിൽ (strncmp (*env, "WASMEDGE_WASINN_PRELOAD=", 24) == 0)
        has_preload = true;
    }

  എങ്കിൽ (! has_plugin_path && ! has_preload)
    തിരികെ 0;

  എങ്കിൽ (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    തിരികെ 0;

  // Check if /usr/lib/wasmedge is already present in spec
  എങ്കിൽ (def->linux && def->mounts)
    {
      വേണ്ടി (size_t i = 0; i < def->mounts_len; i++)
        {
          എങ്കിൽ (strcmp (def->mounts[i]->destination, "/usr/lib/wasmedge") == 0)
            തിരികെ 0;
        }
    }

  // Mount the plugin folder to /usr/lib/wasmedge with specific options
  അക്ഷരം *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/wasmedge ", "/usr/lib/wasmedge", options, 5, err);
  എങ്കിൽ (ret < 0)
    {
      എങ്കിൽ (crun_error_get_errno (err) != ENOENT)
        തിരികെ ret;
      crun_error_release (err);
    }

  തിരികെ 0;
}

ഘടന custom_handler_s handler_wasmedge = {
  .name = "wasmedge",
  .alias = "wasm",
  .feature_string = "WASM:wasmedge",
  .supports_open_വൃക്ഷം_namespace = false,
  .load = libwasmedge_load,
  .unload = libwasmedge_unload,
  .run_func = libwasmedge_exec,
  .can_handle_container = wasmedge_can_handle_container,
  .configure_container = libwasmedge_configure_container,
};

#endif
