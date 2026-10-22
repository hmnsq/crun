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

#include <config.h>
#include "../custom-handler.h"
#include "../container.h"
#include "../utils.h"
#include "../linux.h"
#include "handler-utils.h"
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMEDGE
#  include <wasmedge/wasmedge.h>
#endif

#ប្រសិនបើ HAVE_DLOPEN && HAVE_WASMEDGE
static ចំនួនគត់
libwasmedge_load (void **cookie, libcrun_error_t *err)
{
  void *handle;

  handle = dlopen ("libwasmedge.so.0", RTLD_NOW);
  ប្រសិនបើ (handle == NULL)
    ត្រឡប់ crun_make_error (err, 0, "could not load `libwasmedge.so.0`: `%s`", dlerror ());
  *cookie = handle;

  ត្រឡប់ 0;
}

static ចំនួនគត់
libwasmedge_unload (void *cookie, libcrun_error_t *err)
{
  ចំនួនគត់ r;

  ប្រសិនបើ (cookie)
    {
      r = dlclose (cookie);
      ប្រសិនបើ (UNLIKELY (r < 0))
        ត្រឡប់ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  ត្រឡប់ 0;
}

static ចំនួនគត់
libwasmedge_exec (void *cookie, __attribute__ ((unused)) libcrun_container_t *container, const var_na *pathname, var_na *const argv[])
{
  WasmEdge_ConfigureContext *(*WasmEdge_ConfigureCreate) (void);
  void (*WasmEdge_ConfigureDelete) (WasmEdge_ConfigureContext *Cxt);
  void (*WasmEdge_ConfigureAddProposal) (WasmEdge_ConfigureContext *Cxt, const ga_nanaa WasmEdge_Proposal Prop);
  void (*WasmEdge_ConfigureAddHostRegistration) (WasmEdge_ConfigureContext *Cxt, ga_nanaa WasmEdge_HostRegistration Host);
  WasmEdge_VMContext *(*WasmEdge_VMCreate) (const WasmEdge_ConfigureContext *ConfCxt, WasmEdge_StoreContext *StoreCxt);
  void (*WasmEdge_VMDelete) (WasmEdge_VMContext *Cxt);
  WasmEdge_Result (*WasmEdge_VMRegisterModuleFromFile) (WasmEdge_VMContext *Cxt, WasmEdge_String ModuleName, const var_na *Path);
  WasmEdge_Result (*WasmEdge_VMRunWasmFromFile) (WasmEdge_VMContext *Cxt, const var_na *Path, const WasmEdge_String FuncName, const WasmEdge_Value *Params, const uint32_t ParamLen, WasmEdge_Value *Returns, const uint32_t ReturnLen);
  void (*WasmEdge_PluginLoadFromPath) (const var_na *Path);
  void (*WasmEdge_PluginInitWASINN) (const var_na *const *NNPreloads, const uint32_t PreloadsLen);
  bool (*WasmEdge_ResultOK) (const WasmEdge_Result Res);
  WasmEdge_String (*WasmEdge_StringCreateByCString) (const var_na *Str);
  uint32_t argn = 0;
  uint32_t envn = 0;
  const var_na *dirs[2] = { "/:/", ".:." };
  WasmEdge_ConfigureContext *configure;
  WasmEdge_VMContext *vm;
  WasmEdge_Result result;

  WasmEdge_ModuleInstanceContext *wasi_module;
  WasmEdge_ModuleInstanceContext *(*WasmEdge_VMGetImportModuleContext) (WasmEdge_VMContext *Cxt, const ga_nanaa WasmEdge_HostRegistration Reg);
  void (*WasmEdge_ModuleInstanceInitWASI) (WasmEdge_ModuleInstanceContext *Cxt, const var_na *const *Args, const uint32_t ArgLen, const var_na *const *Envs, const uint32_t EnvLen, const var_na *const *Dirs, const uint32_t DirLen, const var_na *const *Preopens, const uint32_t PreopenLen);
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

  ប្រសិនបើ (WasmEdge_ConfigureCreate == NULL || WasmEdge_ConfigureDelete == NULL || WasmEdge_ConfigureAddProposal == NULL
      || WasmEdge_ConfigureAddHostRegistration == NULL || WasmEdge_VMCreate == NULL || WasmEdge_VMDelete == NULL
      || WasmEdge_VMRegisterModuleFromFile == NULL || WasmEdge_VMGetImportModuleContext == NULL
      || WasmEdge_ModuleInstanceInitWASI == NULL || WasmEdge_VMRunWasmFromFile == NULL
      || WasmEdge_ResultOK == NULL || WasmEdge_StringCreateByCString == NULL)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmedge.so.0`");

  configure = WasmEdge_ConfigureCreate ();
  ប្រសិនបើ (UNLIKELY (configure == NULL))
    error (EXIT_FAILURE, 0, "could not create wasmedge configure");

  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_BulkMemoryOperations);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_ReferenceTypes);
  WasmEdge_ConfigureAddProposal (configure, WasmEdge_Proposal_SIMD);
  WasmEdge_ConfigureAddHostRegistration (configure, WasmEdge_HostRegistration_Wasi);
  // Check if the necessary environment variables are set
  const var_na *plugin_path_env = getenv ("WASMEDGE_PLUGIN_PATH");
  ប្រសិនបើ (plugin_path_env != NULL)
    WasmEdge_PluginLoadFromPath (plugin_path_env);

  const var_na *nnpreload_env = getenv ("WASMEDGE_WASINN_PRELOAD");
  ប្រសិនបើ (nnpreload_env != NULL)
    WasmEdge_PluginInitWASINN (&nnpreload_env, 1);

  vm = WasmEdge_VMCreate (configure, NULL);
  ប្រសិនបើ (UNLIKELY (vm == NULL))
    {
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not create wasmedge vm");
    }

  wasi_module = WasmEdge_VMGetImportModuleContext (vm, WasmEdge_HostRegistration_Wasi);
  ប្រសិនបើ (UNLIKELY (wasi_module == NULL))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge wasi module context");
    }

  សម្រាប់ (var_na *const *arg = argv; *arg != NULL; ++arg, ++argn)
    ;
  extern var_na **environ;
  សម្រាប់ (var_na *const *env = environ; *env != NULL; ++env, ++envn)
    ;

  WasmEdge_ModuleInstanceInitWASI (wasi_module, (const var_na *const *) &argv[0], argn, (const var_na *const *) &environ[0], envn, dirs, 1, NULL, 0);

  result = WasmEdge_VMRunWasmFromFile (vm, pathname, WasmEdge_StringCreateByCString ("_start"), NULL, 0, NULL, 0);

  ប្រសិនបើ (UNLIKELY (! WasmEdge_ResultOK (result)))
    {
      WasmEdge_VMDelete (vm);
      WasmEdge_ConfigureDelete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge result from VM");
    }

  WasmEdge_VMDelete (vm);
  WasmEdge_ConfigureDelete (configure);
  exit (EXIT_SUCCESS);
}

static ចំនួនគត់
wasmedge_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  ត្រឡប់ wasm_can_handle_container (container, err);
}

// This works only when the plugin is present in /usr/lib/wasmedge
static ចំនួនគត់
libwasmedge_configure_container (void *cookie arg_unused, ga_nanaa handler_configure_phase phase,
                                 libcrun_context_t *context arg_unused, libcrun_container_t *container,
                                 const var_na *rootfs arg_unused, libcrun_error_t *err)
{
  ចំនួនគត់ ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  var_na **container_env = def->process->env;
  bool has_plugin_path = false, has_preload = false;

  សម្រាប់ (var_na **env = container_env; env && *env; env++)
    {
      ប្រសិនបើ (strncmp (*env, "WASMEDGE_PLUGIN_PATH=", 21) == 0)
        has_plugin_path = true;
      បើមិនដូច្នេះ ប្រសិនបើ (strncmp (*env, "WASMEDGE_WASINN_PRELOAD=", 24) == 0)
        has_preload = true;
    }

  ប្រសិនបើ (! has_plugin_path && ! has_preload)
    ត្រឡប់ 0;

  ប្រសិនបើ (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    ត្រឡប់ 0;

  // Check if /usr/lib/wasmedge is already present in spec
  ប្រសិនបើ (def->linux && def->mounts)
    {
      សម្រាប់ (size_t i = 0; i < def->mounts_len; i++)
        {
          ប្រសិនបើ (strcmp (def->mounts[i]->destination, "/usr/lib/wasmedge") == 0)
            ត្រឡប់ 0;
        }
    }

  // Mount the plugin folder to /usr/lib/wasmedge with specific options
  var_na *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/wasmedge ", "/usr/lib/wasmedge", options, 5, err);
  ប្រសិនបើ (ret < 0)
    {
      ប្រសិនបើ (crun_error_get_errno (err) != ENOENT)
        ត្រឡប់ ret;
      crun_error_release (err);
    }

  ត្រឡប់ 0;
}

sa_nracanaa custom_handler_s handler_wasmedge = {
  .name = "wasmedge",
  .alias = "wasm",
  .feature_string = "WASM:wasmedge",
  .supports_open_tree_namespace = false,
  .load = libwasmedge_load,
  .unload = libwasmedge_unload,
  .run_func = libwasmedge_exec,
  .can_handle_container = wasmedge_can_handle_container,
  .configure_container = libwasmedge_configure_container,
};

#endif
