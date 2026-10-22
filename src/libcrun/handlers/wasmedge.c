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

#شامل <config.h>
#شامل "../custom-handler.h"
#شامل "../container.h"
#شامل "../utils.h"
#شامل "../linux.h"
#شامل "handler-utils.h"
#شامل <unistd.h>
#شامل <sys/stat.h>
#شامل <errno.h>
#شامل <sys/types.h>
#شامل <fcntl.h>
#شامل <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMEDGE
#  include <wasmedge/wasmedge.h>
#endif

#اگر HAVE_DLOPEN && HAVE_WASMEDGE
ایستا صحیح
libwasmedge_load (تهی **cookie, libcrun_error_t *err)
{
  تهی *handle;

  handle = dlopen ("libwasmedge.so.0", RTLD_NOW);
  اگر (handle == پوچ)
    بازگشت crun_make_error (err, 0, "could not load `libwasmedge.so.0`: `%s`", dlerror ());
  *cookie = handle;

  بازگشت 0;
}

ایستا صحیح
libwasmedge_unload (تهی *cookie, libcrun_error_t *err)
{
  صحیح r;

  اگر (cookie)
    {
      r = dlclose (cookie);
      اگر (UNLIKELY (r < 0))
        بازگشت crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  بازگشت 0;
}

ایستا صحیح
libwasmedge_exec (تهی *cookie, __attribute__ ((unused)) libcrun_container_t *container, ثابت نویسه *pathname, نویسه *ثابت argv[])
{
  WasmEdge_ConfigureContext *(*WasmEdge_ConfigureCreate) (تهی);
  تهی (*WasmEdge_ConfigureDelete) (WasmEdge_ConfigureContext *Cxt);
  تهی (*WasmEdge_ConfigureAddProposal) (WasmEdge_ConfigureContext *Cxt, ثابت شمارش WasmEdge_Proposal Prop);
  تهی (*WasmEdge_ConfigureAddHostRegistration) (WasmEdge_ConfigureContext *Cxt, شمارش WasmEdge_HostRegistration Host);
  WasmEdge_VMContext *(*WasmEdge_VMCreate) (ثابت WasmEdge_ConfigureContext *ConfCxt, WasmEdge_StoreContext *StoreCxt);
  تهی (*WasmEdge_VMDelete) (WasmEdge_VMContext *Cxt);
  WasmEdge_Result (*WasmEdge_VMRegisterModuleFromFile) (WasmEdge_VMContext *Cxt, WasmEdge_String ModuleName, ثابت نویسه *Path);
  WasmEdge_Result (*WasmEdge_VMRunWasmFromFile) (WasmEdge_VMContext *Cxt, ثابت نویسه *Path, ثابت WasmEdge_String FuncName, ثابت Wasm_Edge_مقدار *Params, ثابت uint32_t ParamLen, Wasm_Edge_مقدار *Returns, ثابت uint32_t ReturnLen);
  تهی (*WasmEdge_PluginLoadFromPath) (ثابت نویسه *Path);
  تهی (*WasmEdge_PluginInitWASINN) (ثابت نویسه *ثابت *NNPreloads, ثابت uint32_t PreloadsLen);
  bool (*WasmEdge_ResultOK) (ثابت WasmEdge_Result Res);
  WasmEdge_String (*WasmEdge_StringCreateByCString) (ثابت نویسه *Str);
  uint32_t argn = 0;
  uint32_t envn = 0;
  ثابت نویسه *dirs[2] = { "/:/", ".:." };
  WasmEdge_ConfigureContext *configure;
  WasmEdge_VMContext *vm;
  WasmEdge_Result result;

  WasmEdge_ModuleInstanceContext *wasi_module;
  WasmEdge_ModuleInstanceContext *(*WasmEdge_VMGetImportModuleContext) (WasmEdge_VMContext *Cxt, ثابت شمارش WasmEdge_HostRegistration Reg);
  تهی (*WasmEdge_ModuleInstanceInitWASI) (WasmEdge_ModuleInstanceContext *Cxt, ثابت نویسه *ثابت *Args, ثابت uint32_t ArgLen, ثابت نویسه *ثابت *Envs, ثابت uint32_t EnvLen, ثابت نویسه *ثابت *Dirs, ثابت uint32_t DirLen, ثابت نویسه *ثابت *Preopens, ثابت uint32_t PreopenLen);
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

  اگر (WasmEdge_ConfigureCreate == پوچ || WasmEdge_ConfigureDelete == پوچ || WasmEdge_ConfigureAddProposal == پوچ
      || WasmEdge_ConfigureAddHostRegistration == پوچ || WasmEdge_VMCreate == پوچ || WasmEdge_VMDelete == پوچ
      || WasmEdge_VMRegisterModuleFromFile == پوچ || WasmEdge_VMGetImportModuleContext == پوچ
      || WasmEdge_ModuleInstanceInitWASI == پوچ || WasmEdge_VMRunWasmFromFile == پوچ
      || WasmEdge_ResultOK == پوچ || WasmEdge_StringCreateByCString == پوچ)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmedge.so.0`");

  configure = WasmEdge_ConfigureCreate ();
  اگر (UNLIKELY (configure == پوچ))
    error (EXIT_FAILURE, 0, "could not create wasmedge configure");

  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_BulkMemoryOperations);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_ReferenceTypes);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_SIMD);
  WasmEdge_ConfigureAddHostRegistration (configure, WasmEdge_HostRegistration_Wasi);
  // Check if the necessary environment variables are set
  ثابت نویسه *plugin_path_env = getenv ("WASMEDGE_PLUGIN_PATH");
  اگر (plugin_path_env != پوچ)
    WasmEdge_PluginLoadFromPath (plugin_path_env);

  ثابت نویسه *nnpreload_env = getenv ("WASMEDGE_WASINN_PRELOAD");
  اگر (nnpreload_env != پوچ)
    WasmEdge_PluginInitWASINN (&nnpreload_env, 1);

  vm = WasmEdge_VMCreate (configure, پوچ);
  اگر (UNLIKELY (vm == پوچ))
    {
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not create wasmedge vm");
    }

  wasi_module = WasmEdge_VMGetImportModuleContext (vm, WasmEdge_HostRegistration_Wasi);
  اگر (UNLIKELY (wasi_module == پوچ))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge wasi module context");
    }

  برای (نویسه *ثابت *arg = argv; *arg != پوچ; ++arg, ++argn)
    ;
  بیرونی نویسه **environ;
  برای (نویسه *ثابت *env = environ; *env != پوچ; ++env, ++envn)
    ;

  WasmEdge_ModuleInstanceInitWASI (wasi_module, (ثابت نویسه *ثابت *) &argv[0], argn, (ثابت نویسه *ثابت *) &environ[0], envn, dirs, 1, پوچ, 0);

  result = WasmEdge_VMRunWasmFromFile (vm, pathname, WasmEdge_StringCreateByCString ("_start"), پوچ, 0, پوچ, 0);

  اگر (UNLIKELY (! WasmEdge_ResultOK (result)))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge result from VM");
    }

  WasmEdge_VMDelete (vm);
  WasmEdge_ConfigureDelete (configure);
  exit (EXIT_SUCCESS);
}

ایستا صحیح
wasmedge_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  بازگشت wasm_can_handle_container (container, err);
}

// This works only when the plugin is present in /usr/lib/wasmedge
ایستا صحیح
libwasmedge_configure_container (تهی *cookie arg_unused, شمارش handler_configure_phase phase,
                                 libcrun_context_t *context arg_unused, libcrun_container_t *container,
                                 ثابت نویسه *rootfs arg_unused, libcrun_error_t *err)
{
  صحیح ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  نویسه **container_env = def->process->env;
  bool has_plugin_path = false, has_preload = false;

  برای (نویسه **env = container_env; env && *env; env++)
    {
      اگر (strncmp (*env, "WASMEDGE_PLUGIN_PATH=", 21) == 0)
        has_plugin_path = true;
      وگرنه اگر (strncmp (*env, "WASMEDGE_WASINN_PRELOAD=", 24) == 0)
        has_preload = true;
    }

  اگر (! has_plugin_path && ! has_preload)
    بازگشت 0;

  اگر (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    بازگشت 0;

  // Check if /usr/lib/wasmedge is already present in spec
  اگر (def->linux && def->mounts)
    {
      برای (size_t i = 0; i < def->mounts_len; i++)
        {
          اگر (strcmp (def->mounts[i]->destination, "/usr/lib/wasmedge") == 0)
            بازگشت 0;
        }
    }

  // Mount the plugin folder to /usr/lib/wasmedge with specific options
  نویسه *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/wasmedge ", "/usr/lib/wasmedge", options, 5, err);
  اگر (ret < 0)
    {
      اگر (crun_error_get_errno (err) != ENOENT)
        بازگشت ret;
      crun_error_release (err);
    }

  بازگشت 0;
}

ساختار custom_handler_s handler_wasmedge = {
  .name = "wasmedge",
  .alias = "wasm",
  .feature_string = "WASM:wasmedge",
  .supports_open_درخت_namespace = false,
  .load = libwasmedge_load,
  .unload = libwasmedge_unload,
  .run_func = libwasmedge_exec,
  .can_handle_container = wasmedge_can_handle_container,
  .configure_container = libwasmedge_configure_container,
};

#endif
