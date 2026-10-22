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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "../custom-handler.h"
#অন্তর্ভাব "../container.h"
#অন্তর্ভাব "../utils.h"
#অন্তর্ভাব "../linux.h"
#অন্তর্ভাব "handler-utils.h"
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMEDGE
#  include <wasmedge/wasmedge.h>
#endif

#যদি HAVE_DLOPEN && HAVE_WASMEDGE
স্থির পূর্ণসংখ্যা
libwasmedge_load (শূন্য **cookie, libcrun_error_t *err)
{
  শূন্য *handle;

  handle = dlopen ("libwasmedge.so.0", RTLD_NOW);
  যদি (handle == ফাঁকা)
    ফেরত crun_make_error (err, 0, "could not load `libwasmedge.so.0`: `%s`", dlerror ());
  *cookie = handle;

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
libwasmedge_unload (শূন্য *cookie, libcrun_error_t *err)
{
  পূর্ণসংখ্যা r;

  যদি (cookie)
    {
      r = dlclose (cookie);
      যদি (UNLIKELY (r < 0))
        ফেরত crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
libwasmedge_exec (শূন্য *cookie, __attribute__ ((unused)) libcrun_container_t *container, ধ্রুবক অক্ষর *pathname, অক্ষর *ধ্রুবক argv[])
{
  WasmEdge_ConfigureContext *(*WasmEdge_ConfigureCreate) (শূন্য);
  শূন্য (*WasmEdge_ConfigureDelete) (WasmEdge_ConfigureContext *Cxt);
  শূন্য (*WasmEdge_ConfigureAddProposal) (WasmEdge_ConfigureContext *Cxt, ধ্রুবক গণনা WasmEdge_Proposal Prop);
  শূন্য (*WasmEdge_ConfigureAddHostRegistration) (WasmEdge_ConfigureContext *Cxt, গণনা WasmEdge_HostRegistration Host);
  WasmEdge_VMContext *(*WasmEdge_VMCreate) (ধ্রুবক WasmEdge_ConfigureContext *ConfCxt, WasmEdge_StoreContext *StoreCxt);
  শূন্য (*WasmEdge_VMDelete) (WasmEdge_VMContext *Cxt);
  Wasm_Edge_ফলাফল (*WasmEdge_VMRegisterModuleFromFile) (WasmEdge_VMContext *Cxt, WasmEdge_String ModuleName, ধ্রুবক অক্ষর *Path);
  Wasm_Edge_ফলাফল (*WasmEdge_VMRunWasmFromFile) (WasmEdge_VMContext *Cxt, ধ্রুবক অক্ষর *Path, ধ্রুবক WasmEdge_String FuncName, ধ্রুবক Wasm_Edge_মান *Params, ধ্রুবক uint32_t ParamLen, Wasm_Edge_মান *Returns, ধ্রুবক uint32_t ReturnLen);
  শূন্য (*WasmEdge_PluginLoadFromPath) (ধ্রুবক অক্ষর *Path);
  শূন্য (*WasmEdge_PluginInitWASINN) (ধ্রুবক অক্ষর *ধ্রুবক *NNPreloads, ধ্রুবক uint32_t PreloadsLen);
  bool (*Wasm_Edge_ফলাফল_OK) (ধ্রুবক Wasm_Edge_ফলাফল Res);
  WasmEdge_String (*WasmEdge_StringCreateByCString) (ধ্রুবক অক্ষর *Str);
  uint32_t argn = 0;
  uint32_t envn = 0;
  ধ্রুবক অক্ষর *dirs[2] = { "/:/", ".:." };
  WasmEdge_ConfigureContext *configure;
  WasmEdge_VMContext *vm;
  Wasm_Edge_ফলাফল ফলাফল;

  WasmEdge_ModuleInstanceContext *wasi_module;
  WasmEdge_ModuleInstanceContext *(*WasmEdge_VMGetImportModuleContext) (WasmEdge_VMContext *Cxt, ধ্রুবক গণনা WasmEdge_HostRegistration Reg);
  শূন্য (*WasmEdge_ModuleInstanceInitWASI) (WasmEdge_ModuleInstanceContext *Cxt, ধ্রুবক অক্ষর *ধ্রুবক *Args, ধ্রুবক uint32_t ArgLen, ধ্রুবক অক্ষর *ধ্রুবক *Envs, ধ্রুবক uint32_t EnvLen, ধ্রুবক অক্ষর *ধ্রুবক *Dirs, ধ্রুবক uint32_t DirLen, ধ্রুবক অক্ষর *ধ্রুবক *Preopens, ধ্রুবক uint32_t PreopenLen);
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
  Wasm_Edge_ফলাফল_OK = dlsym (cookie, "WasmEdge_ResultOK");
  WasmEdge_StringCreateByCString = dlsym (cookie, "WasmEdge_StringCreateByCString");

  যদি (WasmEdge_ConfigureCreate == ফাঁকা || WasmEdge_ConfigureDelete == ফাঁকা || WasmEdge_ConfigureAddProposal == ফাঁকা
      || WasmEdge_ConfigureAddHostRegistration == ফাঁকা || WasmEdge_VMCreate == ফাঁকা || WasmEdge_VMDelete == ফাঁকা
      || WasmEdge_VMRegisterModuleFromFile == ফাঁকা || WasmEdge_VMGetImportModuleContext == ফাঁকা
      || WasmEdge_ModuleInstanceInitWASI == ফাঁকা || WasmEdge_VMRunWasmFromFile == ফাঁকা
      || Wasm_Edge_ফলাফল_OK == ফাঁকা || WasmEdge_StringCreateByCString == ফাঁকা)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmedge.so.0`");

  configure = WasmEdge_ConfigureCreate ();
  যদি (UNLIKELY (configure == ফাঁকা))
    error (EXIT_FAILURE, 0, "could not create wasmedge configure");

  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_BulkMemoryOperations);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_ReferenceTypes);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_SIMD);
  WasmEdge_ConfigureAddHostRegistration (configure, WasmEdge_HostRegistration_Wasi);
  // Check if the necessary environment variables are set
  ধ্রুবক অক্ষর *plugin_path_env = getenv ("WASMEDGE_PLUGIN_PATH");
  যদি (plugin_path_env != ফাঁকা)
    WasmEdge_PluginLoadFromPath (plugin_path_env);

  ধ্রুবক অক্ষর *nnpreload_env = getenv ("WASMEDGE_WASINN_PRELOAD");
  যদি (nnpreload_env != ফাঁকা)
    WasmEdge_PluginInitWASINN (&nnpreload_env, 1);

  vm = WasmEdge_VMCreate (configure, ফাঁকা);
  যদি (UNLIKELY (vm == ফাঁকা))
    {
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not create wasmedge vm");
    }

  wasi_module = WasmEdge_VMGetImportModuleContext (vm, WasmEdge_HostRegistration_Wasi);
  যদি (UNLIKELY (wasi_module == ফাঁকা))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge wasi module context");
    }

  জন্য (অক্ষর *ধ্রুবক *arg = argv; *arg != ফাঁকা; ++arg, ++argn)
    ;
  বাহ্যিক অক্ষর **environ;
  জন্য (অক্ষর *ধ্রুবক *env = environ; *env != ফাঁকা; ++env, ++envn)
    ;

  WasmEdge_ModuleInstanceInitWASI (wasi_module, (ধ্রুবক অক্ষর *ধ্রুবক *) &argv[0], argn, (ধ্রুবক অক্ষর *ধ্রুবক *) &environ[0], envn, dirs, 1, ফাঁকা, 0);

  ফলাফল = WasmEdge_VMRunWasmFromFile (vm, pathname, WasmEdge_StringCreateByCString ("_start"), ফাঁকা, 0, ফাঁকা, 0);

  যদি (UNLIKELY (! Wasm_Edge_ফলাফল_OK (ফলাফল)))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge result from VM");
    }

  WasmEdge_VMDelete (vm);
  WasmEdge_ConfigureDelete (configure);
  exit (EXIT_SUCCESS);
}

স্থির পূর্ণসংখ্যা
wasmedge_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  ফেরত wasm_can_handle_container (container, err);
}

// This works only when the plugin is present in /usr/lib/wasmedge
স্থির পূর্ণসংখ্যা
libwasmedge_configure_container (শূন্য *cookie arg_unused, গণনা handler_configure_phase phase,
                                 libcrun_context_t *context arg_unused, libcrun_container_t *container,
                                 ধ্রুবক অক্ষর *rootfs arg_unused, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  অক্ষর **container_env = def->process->env;
  bool has_plugin_path = false, has_preload = false;

  জন্য (অক্ষর **env = container_env; env && *env; env++)
    {
      যদি (strncmp (*env, "WASMEDGE_PLUGIN_PATH=", 21) == 0)
        has_plugin_path = true;
      অন্যথা যদি (strncmp (*env, "WASMEDGE_WASINN_PRELOAD=", 24) == 0)
        has_preload = true;
    }

  যদি (! has_plugin_path && ! has_preload)
    ফেরত 0;

  যদি (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    ফেরত 0;

  // Check if /usr/lib/wasmedge is already present in spec
  যদি (def->linux && def->mounts)
    {
      জন্য (size_t i = 0; i < def->mounts_len; i++)
        {
          যদি (strcmp (def->mounts[i]->destination, "/usr/lib/wasmedge") == 0)
            ফেরত 0;
        }
    }

  // Mount the plugin folder to /usr/lib/wasmedge with specific options
  অক্ষর *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/wasmedge ", "/usr/lib/wasmedge", options, 5, err);
  যদি (ret < 0)
    {
      যদি (crun_error_get_errno (err) != ENOENT)
        ফেরত ret;
      crun_error_release (err);
    }

  ফেরত 0;
}

কাঠামো custom_handler_s handler_wasmedge = {
  .name = "wasmedge",
  .alias = "wasm",
  .feature_string = "WASM:wasmedge",
  .supports_open_বৃক্ষ_namespace = false,
  .load = libwasmedge_load,
  .unload = libwasmedge_unload,
  .run_func = libwasmedge_exec,
  .can_handle_container = wasmedge_can_handle_container,
  .configure_container = libwasmedge_configure_container,
};

#endif
