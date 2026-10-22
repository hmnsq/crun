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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "../custom-handler.h"
#ଅନ୍ତର୍ଭାଵ "../container.h"
#ଅନ୍ତର୍ଭାଵ "../utils.h"
#ଅନ୍ତର୍ଭାଵ "../linux.h"
#ଅନ୍ତର୍ଭାଵ "handler-utils.h"
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMEDGE
#  include <wasmedge/wasmedge.h>
#endif

#ଯଦି HAVE_DLOPEN && HAVE_WASMEDGE
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libwasmedge_load (ଶୂନ୍ୟ **cookie, libcrun_error_t *err)
{
  ଶୂନ୍ୟ *handle;

  handle = dlopen ("libwasmedge.so.0", RTLD_NOW);
  ଯଦି (handle == ଖାଲି)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "could not load `libwasmedge.so.0`: `%s`", dlerror ());
  *cookie = handle;

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libwasmedge_unload (ଶୂନ୍ୟ *cookie, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା r;

  ଯଦି (cookie)
    {
      r = dlclose (cookie);
      ଯଦି (UNLIKELY (r < 0))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libwasmedge_exec (ଶୂନ୍ୟ *cookie, __attribute__ ((unused)) libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *pathname, ଅକ୍ଷର *ଅଚଳ argv[])
{
  WasmEdge_ConfigureContext *(*WasmEdge_ConfigureCreate) (ଶୂନ୍ୟ);
  ଶୂନ୍ୟ (*WasmEdge_ConfigureDelete) (WasmEdge_ConfigureContext *Cxt);
  ଶୂନ୍ୟ (*WasmEdge_ConfigureAddProposal) (WasmEdge_ConfigureContext *Cxt, ଅଚଳ ଗଣନା WasmEdge_Proposal Prop);
  ଶୂନ୍ୟ (*WasmEdge_ConfigureAddHostRegistration) (WasmEdge_ConfigureContext *Cxt, ଗଣନା WasmEdge_HostRegistration Host);
  WasmEdge_VMContext *(*WasmEdge_VMCreate) (ଅଚଳ WasmEdge_ConfigureContext *ConfCxt, WasmEdge_StoreContext *StoreCxt);
  ଶୂନ୍ୟ (*WasmEdge_VMDelete) (WasmEdge_VMContext *Cxt);
  WasmEdge_Result (*WasmEdge_VMRegisterModuleFromFile) (WasmEdge_VMContext *Cxt, WasmEdge_String ModuleName, ଅଚଳ ଅକ୍ଷର *Path);
  WasmEdge_Result (*WasmEdge_VMRunWasmFromFile) (WasmEdge_VMContext *Cxt, ଅଚଳ ଅକ୍ଷର *Path, ଅଚଳ WasmEdge_String FuncName, ଅଚଳ Wasm_Edge_ମୂଲ୍ୟ *Params, ଅଚଳ uint32_t ParamLen, Wasm_Edge_ମୂଲ୍ୟ *Returns, ଅଚଳ uint32_t ReturnLen);
  ଶୂନ୍ୟ (*WasmEdge_PluginLoadFromPath) (ଅଚଳ ଅକ୍ଷର *Path);
  ଶୂନ୍ୟ (*WasmEdge_PluginInitWASINN) (ଅଚଳ ଅକ୍ଷର *ଅଚଳ *NNPreloads, ଅଚଳ uint32_t PreloadsLen);
  bool (*WasmEdge_ResultOK) (ଅଚଳ WasmEdge_Result Res);
  WasmEdge_String (*WasmEdge_StringCreateByCString) (ଅଚଳ ଅକ୍ଷର *Str);
  uint32_t argn = 0;
  uint32_t envn = 0;
  ଅଚଳ ଅକ୍ଷର *dirs[2] = { "/:/", ".:." };
  WasmEdge_ConfigureContext *configure;
  WasmEdge_VMContext *vm;
  WasmEdge_Result result;

  WasmEdge_ModuleInstanceContext *wasi_module;
  WasmEdge_ModuleInstanceContext *(*WasmEdge_VMGetImportModuleContext) (WasmEdge_VMContext *Cxt, ଅଚଳ ଗଣନା WasmEdge_HostRegistration Reg);
  ଶୂନ୍ୟ (*WasmEdge_ModuleInstanceInitWASI) (WasmEdge_ModuleInstanceContext *Cxt, ଅଚଳ ଅକ୍ଷର *ଅଚଳ *Args, ଅଚଳ uint32_t ArgLen, ଅଚଳ ଅକ୍ଷର *ଅଚଳ *Envs, ଅଚଳ uint32_t EnvLen, ଅଚଳ ଅକ୍ଷର *ଅଚଳ *Dirs, ଅଚଳ uint32_t DirLen, ଅଚଳ ଅକ୍ଷର *ଅଚଳ *Preopens, ଅଚଳ uint32_t PreopenLen);
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

  ଯଦି (WasmEdge_ConfigureCreate == ଖାଲି || WasmEdge_ConfigureDelete == ଖାଲି || WasmEdge_ConfigureAddProposal == ଖାଲି
      || WasmEdge_ConfigureAddHostRegistration == ଖାଲି || WasmEdge_VMCreate == ଖାଲି || WasmEdge_VMDelete == ଖାଲି
      || WasmEdge_VMRegisterModuleFromFile == ଖାଲି || WasmEdge_VMGetImportModuleContext == ଖାଲି
      || WasmEdge_ModuleInstanceInitWASI == ଖାଲି || WasmEdge_VMRunWasmFromFile == ଖାଲି
      || WasmEdge_ResultOK == ଖାଲି || WasmEdge_StringCreateByCString == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmedge.so.0`");

  configure = WasmEdge_ConfigureCreate ();
  ଯଦି (UNLIKELY (configure == ଖାଲି))
    error (EXIT_FAILURE, 0, "could not create wasmedge configure");

  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_BulkMemoryOperations);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_ReferenceTypes);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_SIMD);
  WasmEdge_ConfigureAddHostRegistration (configure, WasmEdge_HostRegistration_Wasi);
  // Check if the necessary environment variables are set
  ଅଚଳ ଅକ୍ଷର *plugin_path_env = getenv ("WASMEDGE_PLUGIN_PATH");
  ଯଦି (plugin_path_env != ଖାଲି)
    WasmEdge_PluginLoadFromPath (plugin_path_env);

  ଅଚଳ ଅକ୍ଷର *nnpreload_env = getenv ("WASMEDGE_WASINN_PRELOAD");
  ଯଦି (nnpreload_env != ଖାଲି)
    WasmEdge_PluginInitWASINN (&nnpreload_env, 1);

  vm = WasmEdge_VMCreate (configure, ଖାଲି);
  ଯଦି (UNLIKELY (vm == ଖାଲି))
    {
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not create wasmedge vm");
    }

  wasi_module = WasmEdge_VMGetImportModuleContext (vm, WasmEdge_HostRegistration_Wasi);
  ଯଦି (UNLIKELY (wasi_module == ଖାଲି))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge wasi module context");
    }

  ପାଇଁ (ଅକ୍ଷର *ଅଚଳ *arg = argv; *arg != ଖାଲି; ++arg, ++argn)
    ;
  ବାହ୍ୟ ଅକ୍ଷର **environ;
  ପାଇଁ (ଅକ୍ଷର *ଅଚଳ *env = environ; *env != ଖାଲି; ++env, ++envn)
    ;

  WasmEdge_ModuleInstanceInitWASI (wasi_module, (ଅଚଳ ଅକ୍ଷର *ଅଚଳ *) &argv[0], argn, (ଅଚଳ ଅକ୍ଷର *ଅଚଳ *) &environ[0], envn, dirs, 1, ଖାଲି, 0);

  result = WasmEdge_VMRunWasmFromFile (vm, pathname, WasmEdge_StringCreateByCString ("_start"), ଖାଲି, 0, ଖାଲି, 0);

  ଯଦି (UNLIKELY (! WasmEdge_ResultOK (result)))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge result from VM");
    }

  WasmEdge_VMDelete (vm);
  WasmEdge_ConfigureDelete (configure);
  exit (EXIT_SUCCESS);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
wasmedge_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ wasm_can_handle_container (container, err);
}

// This works only when the plugin is present in /usr/lib/wasmedge
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libwasmedge_configure_container (ଶୂନ୍ୟ *cookie arg_unused, ଗଣନା handler_configure_phase phase,
                                 libcrun_context_t *context arg_unused, libcrun_container_t *container,
                                 ଅଚଳ ଅକ୍ଷର *rootfs arg_unused, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  ଅକ୍ଷର **container_env = def->process->env;
  bool has_plugin_path = false, has_preload = false;

  ପାଇଁ (ଅକ୍ଷର **env = container_env; env && *env; env++)
    {
      ଯଦି (strncmp (*env, "WASMEDGE_PLUGIN_PATH=", 21) == 0)
        has_plugin_path = true;
      ଅନ୍ୟଥା ଯଦି (strncmp (*env, "WASMEDGE_WASINN_PRELOAD=", 24) == 0)
        has_preload = true;
    }

  ଯଦି (! has_plugin_path && ! has_preload)
    ଫେରନ୍ତୁ 0;

  ଯଦି (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    ଫେରନ୍ତୁ 0;

  // Check if /usr/lib/wasmedge is already present in spec
  ଯଦି (def->linux && def->mounts)
    {
      ପାଇଁ (size_t i = 0; i < def->mounts_len; i++)
        {
          ଯଦି (strcmp (def->mounts[i]->destination, "/usr/lib/wasmedge") == 0)
            ଫେରନ୍ତୁ 0;
        }
    }

  // Mount the plugin folder to /usr/lib/wasmedge with specific options
  ଅକ୍ଷର *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/wasmedge ", "/usr/lib/wasmedge", options, 5, err);
  ଯଦି (ret < 0)
    {
      ଯଦି (crun_error_get_errno (err) != ENOENT)
        ଫେରନ୍ତୁ ret;
      crun_error_release (err);
    }

  ଫେରନ୍ତୁ 0;
}

ସଂରଚନା custom_handler_s handler_wasmedge = {
  .name = "wasmedge",
  .alias = "wasm",
  .feature_string = "WASM:wasmedge",
  .supports_open_ବୃକ୍ଷ_namespace = false,
  .load = libwasmedge_load,
  .unload = libwasmedge_unload,
  .run_func = libwasmedge_exec,
  .can_handle_container = wasmedge_can_handle_container,
  .configure_container = libwasmedge_configure_container,
};

#endif
