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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "../custom-handler.h"
#અન્તર્ભાવ "../container.h"
#અન્તર્ભાવ "../utils.h"
#અન્તર્ભાવ "../linux.h"
#અન્તર્ભાવ "handler-utils.h"
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMEDGE
#  include <wasmedge/wasmedge.h>
#endif

#જો HAVE_DLOPEN && HAVE_WASMEDGE
સ્થિર પૂર્ણાંક
libwasmedge_load (શૂન્ય **cookie, libcrun_error_t *err)
{
  શૂન્ય *handle;

  handle = dlopen ("libwasmedge.so.0", RTLD_NOW);
  જો (handle == ખાલી)
    પાછા crun_make_error (err, 0, "could not load `libwasmedge.so.0`: `%s`", dlerror ());
  *cookie = handle;

  પાછા 0;
}

સ્થિર પૂર્ણાંક
libwasmedge_unload (શૂન્ય *cookie, libcrun_error_t *err)
{
  પૂર્ણાંક r;

  જો (cookie)
    {
      r = dlclose (cookie);
      જો (UNLIKELY (r < 0))
        પાછા crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
libwasmedge_exec (શૂન્ય *cookie, __attribute__ ((unused)) libcrun_container_t *container, અચલ અક્ષર *pathname, અક્ષર *અચલ argv[])
{
  WasmEdge_ConfigureContext *(*WasmEdge_ConfigureCreate) (શૂન્ય);
  શૂન્ય (*WasmEdge_ConfigureDelete) (WasmEdge_ConfigureContext *Cxt);
  શૂન્ય (*WasmEdge_ConfigureAddProposal) (WasmEdge_ConfigureContext *Cxt, અચલ ગણના WasmEdge_Proposal Prop);
  શૂન્ય (*WasmEdge_ConfigureAddHostRegistration) (WasmEdge_ConfigureContext *Cxt, ગણના WasmEdge_HostRegistration Host);
  WasmEdge_VMContext *(*WasmEdge_VMCreate) (અચલ WasmEdge_ConfigureContext *ConfCxt, WasmEdge_StoreContext *StoreCxt);
  શૂન્ય (*WasmEdge_VMDelete) (WasmEdge_VMContext *Cxt);
  WasmEdge_Result (*WasmEdge_VMRegisterModuleFromFile) (WasmEdge_VMContext *Cxt, WasmEdge_String ModuleName, અચલ અક્ષર *Path);
  WasmEdge_Result (*WasmEdge_VMRunWasmFromFile) (WasmEdge_VMContext *Cxt, અચલ અક્ષર *Path, અચલ WasmEdge_String FuncName, અચલ Wasm_Edge_કિંમત *Params, અચલ uint32_t ParamLen, Wasm_Edge_કિંમત *Returns, અચલ uint32_t ReturnLen);
  શૂન્ય (*WasmEdge_PluginLoadFromPath) (અચલ અક્ષર *Path);
  શૂન્ય (*WasmEdge_PluginInitWASINN) (અચલ અક્ષર *અચલ *NNPreloads, અચલ uint32_t PreloadsLen);
  bool (*WasmEdge_ResultOK) (અચલ WasmEdge_Result Res);
  WasmEdge_String (*WasmEdge_StringCreateByCString) (અચલ અક્ષર *Str);
  uint32_t argn = 0;
  uint32_t envn = 0;
  અચલ અક્ષર *dirs[2] = { "/:/", ".:." };
  WasmEdge_ConfigureContext *configure;
  WasmEdge_VMContext *vm;
  WasmEdge_Result result;

  WasmEdge_ModuleInstanceContext *wasi_module;
  WasmEdge_ModuleInstanceContext *(*WasmEdge_VMGetImportModuleContext) (WasmEdge_VMContext *Cxt, અચલ ગણના WasmEdge_HostRegistration Reg);
  શૂન્ય (*WasmEdge_ModuleInstanceInitWASI) (WasmEdge_ModuleInstanceContext *Cxt, અચલ અક્ષર *અચલ *Args, અચલ uint32_t ArgLen, અચલ અક્ષર *અચલ *Envs, અચલ uint32_t EnvLen, અચલ અક્ષર *અચલ *Dirs, અચલ uint32_t DirLen, અચલ અક્ષર *અચલ *Preopens, અચલ uint32_t PreopenLen);
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

  જો (WasmEdge_ConfigureCreate == ખાલી || WasmEdge_ConfigureDelete == ખાલી || WasmEdge_ConfigureAddProposal == ખાલી
      || WasmEdge_ConfigureAddHostRegistration == ખાલી || WasmEdge_VMCreate == ખાલી || WasmEdge_VMDelete == ખાલી
      || WasmEdge_VMRegisterModuleFromFile == ખાલી || WasmEdge_VMGetImportModuleContext == ખાલી
      || WasmEdge_ModuleInstanceInitWASI == ખાલી || WasmEdge_VMRunWasmFromFile == ખાલી
      || WasmEdge_ResultOK == ખાલી || WasmEdge_StringCreateByCString == ખાલી)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmedge.so.0`");

  configure = WasmEdge_ConfigureCreate ();
  જો (UNLIKELY (configure == ખાલી))
    error (EXIT_FAILURE, 0, "could not create wasmedge configure");

  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_BulkMemoryOperations);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_ReferenceTypes);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_SIMD);
  WasmEdge_ConfigureAddHostRegistration (configure, WasmEdge_HostRegistration_Wasi);
  // Check if the necessary environment variables are set
  અચલ અક્ષર *plugin_path_env = getenv ("WASMEDGE_PLUGIN_PATH");
  જો (plugin_path_env != ખાલી)
    WasmEdge_PluginLoadFromPath (plugin_path_env);

  અચલ અક્ષર *nnpreload_env = getenv ("WASMEDGE_WASINN_PRELOAD");
  જો (nnpreload_env != ખાલી)
    WasmEdge_PluginInitWASINN (&nnpreload_env, 1);

  vm = WasmEdge_VMCreate (configure, ખાલી);
  જો (UNLIKELY (vm == ખાલી))
    {
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not create wasmedge vm");
    }

  wasi_module = WasmEdge_VMGetImportModuleContext (vm, WasmEdge_HostRegistration_Wasi);
  જો (UNLIKELY (wasi_module == ખાલી))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge wasi module context");
    }

  માટે (અક્ષર *અચલ *arg = argv; *arg != ખાલી; ++arg, ++argn)
    ;
  બાહ્ય અક્ષર **environ;
  માટે (અક્ષર *અચલ *env = environ; *env != ખાલી; ++env, ++envn)
    ;

  WasmEdge_ModuleInstanceInitWASI (wasi_module, (અચલ અક્ષર *અચલ *) &argv[0], argn, (અચલ અક્ષર *અચલ *) &environ[0], envn, dirs, 1, ખાલી, 0);

  result = WasmEdge_VMRunWasmFromFile (vm, pathname, WasmEdge_StringCreateByCString ("_start"), ખાલી, 0, ખાલી, 0);

  જો (UNLIKELY (! WasmEdge_ResultOK (result)))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge result from VM");
    }

  WasmEdge_VMDelete (vm);
  WasmEdge_ConfigureDelete (configure);
  exit (EXIT_SUCCESS);
}

સ્થિર પૂર્ણાંક
wasmedge_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  પાછા wasm_can_handle_container (container, err);
}

// This works only when the plugin is present in /usr/lib/wasmedge
સ્થિર પૂર્ણાંક
libwasmedge_configure_container (શૂન્ય *cookie arg_unused, ગણના handler_configure_phase phase,
                                 libcrun_context_t *context arg_unused, libcrun_container_t *container,
                                 અચલ અક્ષર *rootfs arg_unused, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  અક્ષર **container_env = def->process->env;
  bool has_plugin_path = false, has_preload = false;

  માટે (અક્ષર **env = container_env; env && *env; env++)
    {
      જો (strncmp (*env, "WASMEDGE_PLUGIN_PATH=", 21) == 0)
        has_plugin_path = true;
      નહીંતર જો (strncmp (*env, "WASMEDGE_WASINN_PRELOAD=", 24) == 0)
        has_preload = true;
    }

  જો (! has_plugin_path && ! has_preload)
    પાછા 0;

  જો (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    પાછા 0;

  // Check if /usr/lib/wasmedge is already present in spec
  જો (def->linux && def->mounts)
    {
      માટે (size_t i = 0; i < def->mounts_len; i++)
        {
          જો (strcmp (def->mounts[i]->destination, "/usr/lib/wasmedge") == 0)
            પાછા 0;
        }
    }

  // Mount the plugin folder to /usr/lib/wasmedge with specific options
  અક્ષર *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/wasmedge ", "/usr/lib/wasmedge", options, 5, err);
  જો (ret < 0)
    {
      જો (crun_error_get_errno (err) != ENOENT)
        પાછા ret;
      crun_error_release (err);
    }

  પાછા 0;
}

સંરચના custom_handler_s handler_wasmedge = {
  .name = "wasmedge",
  .alias = "wasm",
  .feature_string = "WASM:wasmedge",
  .supports_open_વૃક્ષ_namespace = false,
  .load = libwasmedge_load,
  .unload = libwasmedge_unload,
  .run_func = libwasmedge_exec,
  .can_handle_container = wasmedge_can_handle_container,
  .configure_container = libwasmedge_configure_container,
};

#endif
