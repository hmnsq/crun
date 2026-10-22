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
স্থির পূর্ণাঙ্ক
libwasmedge_load (শূন্য **cookie, libcrun_error_t *err)
{
  শূন্য *handle;

  handle = dlopen ("libwasmedge.so.0", RTLD_NOW);
  যদি (handle == রিক্ত)
    প্রত্যাগম crun_make_error (err, 0, "could not load `libwasmedge.so.0`: `%s`", dlerror ());
  *cookie = handle;

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
libwasmedge_unload (শূন্য *cookie, libcrun_error_t *err)
{
  পূর্ণাঙ্ক r;

  যদি (cookie)
    {
      r = dlclose (cookie);
      যদি (UNLIKELY (r < 0))
        প্রত্যাগম crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
libwasmedge_exec (শূন্য *cookie, __attribute__ ((unused)) libcrun_container_t *container, ধ্রুব বর্ণ *pathname, বর্ণ *ধ্রুব argv[])
{
  WasmEdge_ConfigureContext *(*WasmEdge_ConfigureCreate) (শূন্য);
  শূন্য (*WasmEdge_ConfigureDelete) (WasmEdge_ConfigureContext *Cxt);
  শূন্য (*WasmEdge_ConfigureAddProposal) (WasmEdge_ConfigureContext *Cxt, ধ্রুব গণনা WasmEdge_Proposal Prop);
  শূন্য (*WasmEdge_ConfigureAddHostRegistration) (WasmEdge_ConfigureContext *Cxt, গণনা WasmEdge_HostRegistration Host);
  WasmEdge_VMContext *(*WasmEdge_VMCreate) (ধ্রুব WasmEdge_ConfigureContext *ConfCxt, WasmEdge_StoreContext *StoreCxt);
  শূন্য (*WasmEdge_VMDelete) (WasmEdge_VMContext *Cxt);
  WasmEdge_Result (*WasmEdge_VMRegisterModuleFromFile) (WasmEdge_VMContext *Cxt, WasmEdge_String ModuleName, ধ্রুব বর্ণ *Path);
  WasmEdge_Result (*WasmEdge_VMRunWasmFromFile) (WasmEdge_VMContext *Cxt, ধ্রুব বর্ণ *Path, ধ্রুব WasmEdge_String FuncName, ধ্রুব Wasm_Edge_মান *Params, ধ্রুব uint32_t ParamLen, Wasm_Edge_মান *Returns, ধ্রুব uint32_t ReturnLen);
  শূন্য (*WasmEdge_PluginLoadFromPath) (ধ্রুব বর্ণ *Path);
  শূন্য (*WasmEdge_PluginInitWASINN) (ধ্রুব বর্ণ *ধ্রুব *NNPreloads, ধ্রুব uint32_t PreloadsLen);
  bool (*WasmEdge_ResultOK) (ধ্রুব WasmEdge_Result Res);
  WasmEdge_String (*WasmEdge_StringCreateByCString) (ধ্রুব বর্ণ *Str);
  uint32_t argn = 0;
  uint32_t envn = 0;
  ধ্রুব বর্ণ *dirs[2] = { "/:/", ".:." };
  WasmEdge_ConfigureContext *configure;
  WasmEdge_VMContext *vm;
  WasmEdge_Result result;

  WasmEdge_ModuleInstanceContext *wasi_module;
  WasmEdge_ModuleInstanceContext *(*WasmEdge_VMGetImportModuleContext) (WasmEdge_VMContext *Cxt, ধ্রুব গণনা WasmEdge_HostRegistration Reg);
  শূন্য (*WasmEdge_ModuleInstanceInitWASI) (WasmEdge_ModuleInstanceContext *Cxt, ধ্রুব বর্ণ *ধ্রুব *Args, ধ্রুব uint32_t ArgLen, ধ্রুব বর্ণ *ধ্রুব *Envs, ধ্রুব uint32_t EnvLen, ধ্রুব বর্ণ *ধ্রুব *Dirs, ধ্রুব uint32_t DirLen, ধ্রুব বর্ণ *ধ্রুব *Preopens, ধ্রুব uint32_t PreopenLen);
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

  যদি (WasmEdge_ConfigureCreate == রিক্ত || WasmEdge_ConfigureDelete == রিক্ত || WasmEdge_ConfigureAddProposal == রিক্ত
      || WasmEdge_ConfigureAddHostRegistration == রিক্ত || WasmEdge_VMCreate == রিক্ত || WasmEdge_VMDelete == রিক্ত
      || WasmEdge_VMRegisterModuleFromFile == রিক্ত || WasmEdge_VMGetImportModuleContext == রিক্ত
      || WasmEdge_ModuleInstanceInitWASI == রিক্ত || WasmEdge_VMRunWasmFromFile == রিক্ত
      || WasmEdge_ResultOK == রিক্ত || WasmEdge_StringCreateByCString == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmedge.so.0`");

  configure = WasmEdge_ConfigureCreate ();
  যদি (UNLIKELY (configure == রিক্ত))
    error (EXIT_FAILURE, 0, "could not create wasmedge configure");

  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_BulkMemoryOperations);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_ReferenceTypes);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_SIMD);
  WasmEdge_ConfigureAddHostRegistration (configure, WasmEdge_HostRegistration_Wasi);
  // Check if the necessary environment variables are set
  ধ্রুব বর্ণ *plugin_path_env = getenv ("WASMEDGE_PLUGIN_PATH");
  যদি (plugin_path_env != রিক্ত)
    WasmEdge_PluginLoadFromPath (plugin_path_env);

  ধ্রুব বর্ণ *nnpreload_env = getenv ("WASMEDGE_WASINN_PRELOAD");
  যদি (nnpreload_env != রিক্ত)
    WasmEdge_PluginInitWASINN (&nnpreload_env, 1);

  vm = WasmEdge_VMCreate (configure, রিক্ত);
  যদি (UNLIKELY (vm == রিক্ত))
    {
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not create wasmedge vm");
    }

  wasi_module = WasmEdge_VMGetImportModuleContext (vm, WasmEdge_HostRegistration_Wasi);
  যদি (UNLIKELY (wasi_module == রিক্ত))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge wasi module context");
    }

  প্রতি (বর্ণ *ধ্রুব *arg = argv; *arg != রিক্ত; ++arg, ++argn)
    ;
  বাহ্য বর্ণ **environ;
  প্রতি (বর্ণ *ধ্রুব *env = environ; *env != রিক্ত; ++env, ++envn)
    ;

  WasmEdge_ModuleInstanceInitWASI (wasi_module, (ধ্রুব বর্ণ *ধ্রুব *) &argv[0], argn, (ধ্রুব বর্ণ *ধ্রুব *) &environ[0], envn, dirs, 1, রিক্ত, 0);

  result = WasmEdge_VMRunWasmFromFile (vm, pathname, WasmEdge_StringCreateByCString ("_start"), রিক্ত, 0, রিক্ত, 0);

  যদি (UNLIKELY (! WasmEdge_ResultOK (result)))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge result from VM");
    }

  WasmEdge_VMDelete (vm);
  WasmEdge_ConfigureDelete (configure);
  exit (EXIT_SUCCESS);
}

স্থির পূর্ণাঙ্ক
wasmedge_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  প্রত্যাগম wasm_can_handle_container (container, err);
}

// This works only when the plugin is present in /usr/lib/wasmedge
স্থির পূর্ণাঙ্ক
libwasmedge_configure_container (শূন্য *cookie arg_unused, গণনা handler_configure_phase phase,
                                 libcrun_context_t *context arg_unused, libcrun_container_t *container,
                                 ধ্রুব বর্ণ *rootfs arg_unused, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  বর্ণ **container_env = def->process->env;
  bool has_plugin_path = false, has_preload = false;

  প্রতি (বর্ণ **env = container_env; env && *env; env++)
    {
      যদি (strncmp (*env, "WASMEDGE_PLUGIN_PATH=", 21) == 0)
        has_plugin_path = true;
      অন্যথা যদি (strncmp (*env, "WASMEDGE_WASINN_PRELOAD=", 24) == 0)
        has_preload = true;
    }

  যদি (! has_plugin_path && ! has_preload)
    প্রত্যাগম 0;

  যদি (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    প্রত্যাগম 0;

  // Check if /usr/lib/wasmedge is already present in spec
  যদি (def->linux && def->mounts)
    {
      প্রতি (size_t i = 0; i < def->mounts_len; i++)
        {
          যদি (strcmp (def->mounts[i]->destination, "/usr/lib/wasmedge") == 0)
            প্রত্যাগম 0;
        }
    }

  // Mount the plugin folder to /usr/lib/wasmedge with specific options
  বর্ণ *options[] = {
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
        প্রত্যাগম ret;
      crun_error_release (err);
    }

  প্রত্যাগম 0;
}

সংরচনা custom_handler_s handler_wasmedge = {
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
