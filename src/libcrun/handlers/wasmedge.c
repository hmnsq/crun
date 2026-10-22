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

#ضمن <config.h>
#ضمن "../custom-handler.h"
#ضمن "../container.h"
#ضمن "../utils.h"
#ضمن "../linux.h"
#ضمن "handler-utils.h"
#ضمن <unistd.h>
#ضمن <sys/stat.h>
#ضمن <errno.h>
#ضمن <sys/types.h>
#ضمن <fcntl.h>
#ضمن <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMEDGE
#  include <wasmedge/wasmedge.h>
#endif

#إذا HAVE_DLOPEN && HAVE_WASMEDGE
ساكن صحيح
libwasmedge_load (فراغ **cookie, libcrun_error_t *err)
{
  فراغ *handle;

  handle = dlopen ("libwasmedge.so.0", RTLD_NOW);
  إذا (handle == عدم)
    أرجع crun_make_error (err, 0, "could not load `libwasmedge.so.0`: `%s`", dlerror ());
  *cookie = handle;

  أرجع 0;
}

ساكن صحيح
libwasmedge_unload (فراغ *cookie, libcrun_error_t *err)
{
  صحيح r;

  إذا (cookie)
    {
      r = dlclose (cookie);
      إذا (UNLIKELY (r < 0))
        أرجع crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  أرجع 0;
}

ساكن صحيح
libwasmedge_exec (فراغ *cookie, __attribute__ ((unused)) libcrun_container_t *container, ثابت حرف *pathname, حرف *ثابت argv[])
{
  WasmEdge_ConfigureContext *(*WasmEdge_ConfigureCreate) (فراغ);
  فراغ (*WasmEdge_ConfigureDelete) (WasmEdge_ConfigureContext *Cxt);
  فراغ (*WasmEdge_ConfigureAddProposal) (WasmEdge_ConfigureContext *Cxt, ثابت تعداد WasmEdge_Proposal Prop);
  فراغ (*WasmEdge_ConfigureAddHostRegistration) (WasmEdge_ConfigureContext *Cxt, تعداد WasmEdge_HostRegistration Host);
  WasmEdge_VMContext *(*WasmEdge_VMCreate) (ثابت WasmEdge_ConfigureContext *ConfCxt, WasmEdge_StoreContext *StoreCxt);
  فراغ (*WasmEdge_VMDelete) (WasmEdge_VMContext *Cxt);
  WasmEdge_Result (*WasmEdge_VMRegisterModuleFromFile) (WasmEdge_VMContext *Cxt, WasmEdge_String ModuleName, ثابت حرف *Path);
  WasmEdge_Result (*WasmEdge_VMRunWasmFromFile) (WasmEdge_VMContext *Cxt, ثابت حرف *Path, ثابت WasmEdge_String FuncName, ثابت Wasm_Edge_بەها *Params, ثابت uint32_t ParamLen, Wasm_Edge_بەها *Returns, ثابت uint32_t ReturnLen);
  فراغ (*WasmEdge_PluginLoadFromPath) (ثابت حرف *Path);
  فراغ (*WasmEdge_PluginInitWASINN) (ثابت حرف *ثابت *NNPreloads, ثابت uint32_t PreloadsLen);
  bool (*WasmEdge_ResultOK) (ثابت WasmEdge_Result Res);
  WasmEdge_String (*WasmEdge_StringCreateByCString) (ثابت حرف *Str);
  uint32_t argn = 0;
  uint32_t envn = 0;
  ثابت حرف *dirs[2] = { "/:/", ".:." };
  WasmEdge_ConfigureContext *configure;
  WasmEdge_VMContext *vm;
  WasmEdge_Result result;

  WasmEdge_ModuleInstanceContext *wasi_module;
  WasmEdge_ModuleInstanceContext *(*WasmEdge_VMGetImportModuleContext) (WasmEdge_VMContext *Cxt, ثابت تعداد WasmEdge_HostRegistration Reg);
  فراغ (*WasmEdge_ModuleInstanceInitWASI) (WasmEdge_ModuleInstanceContext *Cxt, ثابت حرف *ثابت *Args, ثابت uint32_t ArgLen, ثابت حرف *ثابت *Envs, ثابت uint32_t EnvLen, ثابت حرف *ثابت *Dirs, ثابت uint32_t DirLen, ثابت حرف *ثابت *Preopens, ثابت uint32_t PreopenLen);
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

  إذا (WasmEdge_ConfigureCreate == عدم || WasmEdge_ConfigureDelete == عدم || WasmEdge_ConfigureAddProposal == عدم
      || WasmEdge_ConfigureAddHostRegistration == عدم || WasmEdge_VMCreate == عدم || WasmEdge_VMDelete == عدم
      || WasmEdge_VMRegisterModuleFromFile == عدم || WasmEdge_VMGetImportModuleContext == عدم
      || WasmEdge_ModuleInstanceInitWASI == عدم || WasmEdge_VMRunWasmFromFile == عدم
      || WasmEdge_ResultOK == عدم || WasmEdge_StringCreateByCString == عدم)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmedge.so.0`");

  configure = WasmEdge_ConfigureCreate ();
  إذا (UNLIKELY (configure == عدم))
    error (EXIT_FAILURE, 0, "could not create wasmedge configure");

  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_BulkMemoryOperations);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_ReferenceTypes);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_SIMD);
  WasmEdge_ConfigureAddHostRegistration (configure, WasmEdge_HostRegistration_Wasi);
  // Check if the necessary environment variables are set
  ثابت حرف *plugin_path_env = getenv ("WASMEDGE_PLUGIN_PATH");
  إذا (plugin_path_env != عدم)
    WasmEdge_PluginLoadFromPath (plugin_path_env);

  ثابت حرف *nnpreload_env = getenv ("WASMEDGE_WASINN_PRELOAD");
  إذا (nnpreload_env != عدم)
    WasmEdge_PluginInitWASINN (&nnpreload_env, 1);

  vm = WasmEdge_VMCreate (configure, عدم);
  إذا (UNLIKELY (vm == عدم))
    {
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not create wasmedge vm");
    }

  wasi_module = WasmEdge_VMGetImportModuleContext (vm, WasmEdge_HostRegistration_Wasi);
  إذا (UNLIKELY (wasi_module == عدم))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge wasi module context");
    }

  لكل (حرف *ثابت *arg = argv; *arg != عدم; ++arg, ++argn)
    ;
  خارجي حرف **environ;
  لكل (حرف *ثابت *env = environ; *env != عدم; ++env, ++envn)
    ;

  WasmEdge_ModuleInstanceInitWASI (wasi_module, (ثابت حرف *ثابت *) &argv[0], argn, (ثابت حرف *ثابت *) &environ[0], envn, dirs, 1, عدم, 0);

  result = WasmEdge_VMRunWasmFromFile (vm, pathname, WasmEdge_StringCreateByCString ("_start"), عدم, 0, عدم, 0);

  إذا (UNLIKELY (! WasmEdge_ResultOK (result)))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge result from VM");
    }

  WasmEdge_VMDelete (vm);
  WasmEdge_ConfigureDelete (configure);
  exit (EXIT_SUCCESS);
}

ساكن صحيح
wasmedge_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  أرجع wasm_can_handle_container (container, err);
}

// This works only when the plugin is present in /usr/lib/wasmedge
ساكن صحيح
libwasmedge_configure_container (فراغ *cookie arg_unused, تعداد handler_configure_phase phase,
                                 libcrun_context_t *context arg_unused, libcrun_container_t *container,
                                 ثابت حرف *rootfs arg_unused, libcrun_error_t *err)
{
  صحيح ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  حرف **container_env = def->process->env;
  bool has_plugin_path = false, has_preload = false;

  لكل (حرف **env = container_env; env && *env; env++)
    {
      إذا (strncmp (*env, "WASMEDGE_PLUGIN_PATH=", 21) == 0)
        has_plugin_path = true;
      وإلا إذا (strncmp (*env, "WASMEDGE_WASINN_PRELOAD=", 24) == 0)
        has_preload = true;
    }

  إذا (! has_plugin_path && ! has_preload)
    أرجع 0;

  إذا (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    أرجع 0;

  // Check if /usr/lib/wasmedge is already present in spec
  إذا (def->linux && def->mounts)
    {
      لكل (size_t i = 0; i < def->mounts_len; i++)
        {
          إذا (strcmp (def->mounts[i]->destination, "/usr/lib/wasmedge") == 0)
            أرجع 0;
        }
    }

  // Mount the plugin folder to /usr/lib/wasmedge with specific options
  حرف *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/wasmedge ", "/usr/lib/wasmedge", options, 5, err);
  إذا (ret < 0)
    {
      إذا (crun_error_get_errno (err) != ENOENT)
        أرجع ret;
      crun_error_release (err);
    }

  أرجع 0;
}

بنية custom_handler_s handler_wasmedge = {
  .name = "wasmedge",
  .alias = "wasm",
  .feature_string = "WASM:wasmedge",
  .supports_open_درەخت_namespace = false,
  .load = libwasmedge_load,
  .unload = libwasmedge_unload,
  .run_func = libwasmedge_exec,
  .can_handle_container = wasmedge_can_handle_container,
  .configure_container = libwasmedge_configure_container,
};

#endif
