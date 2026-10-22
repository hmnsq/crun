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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "../custom-handler.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../container.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../linux.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "handler-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sched.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DLOPEN
#  include <dlfcn.h>
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_WASMEDGE
#  include <wasmedge/wasmedge.h>
#ਜੇ_ਅੰਤ

#ਜੇ HAVE_DLOPEN && HAVE_WASMEDGE
ਸਥਿਰ ਪੂਰਨ
libwasmedge_ਲੱਦੋ (ਖਾਲੀ **cookie, libcrun_error_t *err)
{
  ਖਾਲੀ *handle;

  handle = dlopen ("libwasmedge.so.0", RTLD_NOW);
  ਜੇ (handle == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, 0, "could not load `libwasmedge.so.0`: `%s`", dlerror ());
  *cookie = handle;

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
libwasmedge_unload (ਖਾਲੀ *cookie, libcrun_error_t *err)
{
  ਪੂਰਨ r;

  ਜੇ (cookie)
    {
      r = dlclose (cookie);
      ਜੇ (UNLIKELY (r < 0))
        ਵਾਪਸ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
libwasmedge_exec (ਖਾਲੀ *cookie, __attribute__ ((ਅਣਵਰਤਿਆ)) libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *pathname, ਅੱਖਰ *ਅਚਲ argv[])
{
  Wasm_ਕਿਨਾਰਾ_Configure_Context *(*Wasm_ਕਿਨਾਰਾ_Configure_Create) (ਖਾਲੀ);
  ਖਾਲੀ (*Wasm_ਕਿਨਾਰਾ_Configure_Delete) (Wasm_ਕਿਨਾਰਾ_Configure_Context *Cxt);
  ਖਾਲੀ (*Wasm_ਕਿਨਾਰਾ_Configure_ਜੋੜੋ_Proposal) (Wasm_ਕਿਨਾਰਾ_Configure_Context *Cxt, ਅਚਲ ਗਿਣਤੀ Wasm_ਕਿਨਾਰਾ_Proposal Prop);
  ਖਾਲੀ (*Wasm_ਕਿਨਾਰਾ_Configure_ਜੋੜੋ_Host_Registration) (Wasm_ਕਿਨਾਰਾ_Configure_Context *Cxt, ਗਿਣਤੀ Wasm_ਕਿਨਾਰਾ_Host_Registration Host);
  Wasm_ਕਿਨਾਰਾ_VMContext *(*Wasm_ਕਿਨਾਰਾ_VMCreate) (ਅਚਲ Wasm_ਕਿਨਾਰਾ_Configure_Context *ConfCxt, Wasm_ਕਿਨਾਰਾ_Store_Context *StoreCxt);
  ਖਾਲੀ (*Wasm_ਕਿਨਾਰਾ_VMDelete) (Wasm_ਕਿਨਾਰਾ_VMContext *Cxt);
  Wasm_ਕਿਨਾਰਾ_ਨਤੀਜਾ (*Wasm_ਕਿਨਾਰਾ_VMRegister_Module_From_File) (Wasm_ਕਿਨਾਰਾ_VMContext *Cxt, Wasm_ਕਿਨਾਰਾ_String ModuleName, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ_2);
  Wasm_ਕਿਨਾਰਾ_ਨਤੀਜਾ (*Wasm_ਕਿਨਾਰਾ_VMRun_Wasm_From_File) (Wasm_ਕਿਨਾਰਾ_VMContext *Cxt, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ_2, ਅਚਲ Wasm_ਕਿਨਾਰਾ_String FuncName, ਅਚਲ Wasm_ਕਿਨਾਰਾ_ਮੁੱਲ *Params, ਅਚਲ uint32_t ParamLen, Wasm_ਕਿਨਾਰਾ_ਮੁੱਲ *Returns, ਅਚਲ uint32_t ReturnLen);
  ਖਾਲੀ (*Wasm_ਕਿਨਾਰਾ_Plugin_ਲੱਦੋ_From_ਰਸਤਾ) (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ_2);
  ਖਾਲੀ (*Wasm_ਕਿਨਾਰਾ_Plugin_Init_WASINN) (ਅਚਲ ਅੱਖਰ *ਅਚਲ *NNPreloads, ਅਚਲ uint32_t PreloadsLen);
  bool (*Wasm_ਕਿਨਾਰਾ_ਨਤੀਜਾ_OK) (ਅਚਲ Wasm_ਕਿਨਾਰਾ_ਨਤੀਜਾ Res);
  Wasm_ਕਿਨਾਰਾ_String (*Wasm_ਕਿਨਾਰਾ_String_Create_By_CString) (ਅਚਲ ਅੱਖਰ *Str);
  uint32_t argn = 0;
  uint32_t envn = 0;
  ਅਚਲ ਅੱਖਰ *dirs[2] = { "/:/", ".:." };
  Wasm_ਕਿਨਾਰਾ_Configure_Context *configure;
  Wasm_ਕਿਨਾਰਾ_VMContext *vm;
  Wasm_ਕਿਨਾਰਾ_ਨਤੀਜਾ ਨਤੀਜਾ;

  Wasm_ਕਿਨਾਰਾ_Module_Instance_Context *wasi_module;
  Wasm_ਕਿਨਾਰਾ_Module_Instance_Context *(*Wasm_ਕਿਨਾਰਾ_VMGet_Import_Module_Context) (Wasm_ਕਿਨਾਰਾ_VMContext *Cxt, ਅਚਲ ਗਿਣਤੀ Wasm_ਕਿਨਾਰਾ_Host_Registration Reg);
  ਖਾਲੀ (*Wasm_ਕਿਨਾਰਾ_Module_Instance_Init_WASI) (Wasm_ਕਿਨਾਰਾ_Module_Instance_Context *Cxt, ਅਚਲ ਅੱਖਰ *ਅਚਲ *ਦਲੀਲਾਂ_2, ਅਚਲ uint32_t ArgLen, ਅਚਲ ਅੱਖਰ *ਅਚਲ *Envs, ਅਚਲ uint32_t EnvLen, ਅਚਲ ਅੱਖਰ *ਅਚਲ *Dirs, ਅਚਲ uint32_t DirLen, ਅਚਲ ਅੱਖਰ *ਅਚਲ *Preopens, ਅਚਲ uint32_t PreopenLen);
  Wasm_ਕਿਨਾਰਾ_Module_Instance_Init_WASI = dlsym (cookie, "WasmEdge_ModuleInstanceInitWASI");

  Wasm_ਕਿਨਾਰਾ_Configure_Create = dlsym (cookie, "WasmEdge_ConfigureCreate");
  Wasm_ਕਿਨਾਰਾ_Configure_Delete = dlsym (cookie, "WasmEdge_ConfigureDelete");
  Wasm_ਕਿਨਾਰਾ_Configure_ਜੋੜੋ_Proposal = dlsym (cookie, "WasmEdge_ConfigureAddProposal");
  Wasm_ਕਿਨਾਰਾ_Configure_ਜੋੜੋ_Host_Registration = dlsym (cookie, "WasmEdge_ConfigureAddHostRegistration");
  Wasm_ਕਿਨਾਰਾ_VMCreate = dlsym (cookie, "WasmEdge_VMCreate");
  Wasm_ਕਿਨਾਰਾ_VMDelete = dlsym (cookie, "WasmEdge_VMDelete");
  Wasm_ਕਿਨਾਰਾ_VMRegister_Module_From_File = dlsym (cookie, "WasmEdge_VMRegisterModuleFromFile");
  Wasm_ਕਿਨਾਰਾ_VMGet_Import_Module_Context = dlsym (cookie, "WasmEdge_VMGetImportModuleContext");
  Wasm_ਕਿਨਾਰਾ_VMRun_Wasm_From_File = dlsym (cookie, "WasmEdge_VMRunWasmFromFile");
  Wasm_ਕਿਨਾਰਾ_Plugin_ਲੱਦੋ_From_ਰਸਤਾ = dlsym (cookie, "WasmEdge_PluginLoadFromPath");
  Wasm_ਕਿਨਾਰਾ_Plugin_Init_WASINN = dlsym (cookie, "WasmEdge_PluginInitWASINN");
  Wasm_ਕਿਨਾਰਾ_ਨਤੀਜਾ_OK = dlsym (cookie, "WasmEdge_ResultOK");
  Wasm_ਕਿਨਾਰਾ_String_Create_By_CString = dlsym (cookie, "WasmEdge_StringCreateByCString");

  ਜੇ (Wasm_ਕਿਨਾਰਾ_Configure_Create == ਨਿਰਾ || Wasm_ਕਿਨਾਰਾ_Configure_Delete == ਨਿਰਾ || Wasm_ਕਿਨਾਰਾ_Configure_ਜੋੜੋ_Proposal == ਨਿਰਾ
      || Wasm_ਕਿਨਾਰਾ_Configure_ਜੋੜੋ_Host_Registration == ਨਿਰਾ || Wasm_ਕਿਨਾਰਾ_VMCreate == ਨਿਰਾ || Wasm_ਕਿਨਾਰਾ_VMDelete == ਨਿਰਾ
      || Wasm_ਕਿਨਾਰਾ_VMRegister_Module_From_File == ਨਿਰਾ || Wasm_ਕਿਨਾਰਾ_VMGet_Import_Module_Context == ਨਿਰਾ
      || Wasm_ਕਿਨਾਰਾ_Module_Instance_Init_WASI == ਨਿਰਾ || Wasm_ਕਿਨਾਰਾ_VMRun_Wasm_From_File == ਨਿਰਾ
      || Wasm_ਕਿਨਾਰਾ_ਨਤੀਜਾ_OK == ਨਿਰਾ || Wasm_ਕਿਨਾਰਾ_String_Create_By_CString == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmedge.so.0`");

  configure = Wasm_ਕਿਨਾਰਾ_Configure_Create ();
  ਜੇ (UNLIKELY (configure == ਨਿਰਾ))
    error (EXIT_FAILURE, 0, "could not create wasmedge configure");

  Wasm_ਕਿਨਾਰਾ_Configure_ਜੋੜੋ_Proposal (configure, Wasm_ਕਿਨਾਰਾ_Proposal_Bulk_ਸਮ੍ਰਿਤੀ_Operations);
  Wasm_ਕਿਨਾਰਾ_Configure_ਜੋੜੋ_Proposal (configure, Wasm_ਕਿਨਾਰਾ_Proposal_Reference_Types);
  Wasm_ਕਿਨਾਰਾ_Configure_ਜੋੜੋ_Proposal (configure, Wasm_ਕਿਨਾਰਾ_Proposal_SIMD);
  Wasm_ਕਿਨਾਰਾ_Configure_ਜੋੜੋ_Host_Registration (configure, Wasm_ਕਿਨਾਰਾ_Host_Registration_Wasi);
  // Check if the necessary environment variables are set
  ਅਚਲ ਅੱਖਰ *plugin_ਰਸਤਾ_env = getenv ("WASMEDGE_PLUGIN_PATH");
  ਜੇ (plugin_ਰਸਤਾ_env != ਨਿਰਾ)
    Wasm_ਕਿਨਾਰਾ_Plugin_ਲੱਦੋ_From_ਰਸਤਾ (plugin_ਰਸਤਾ_env);

  ਅਚਲ ਅੱਖਰ *nnpreload_env = getenv ("WASMEDGE_WASINN_PRELOAD");
  ਜੇ (nnpreload_env != ਨਿਰਾ)
    Wasm_ਕਿਨਾਰਾ_Plugin_Init_WASINN (&nnpreload_env, 1);

  vm = Wasm_ਕਿਨਾਰਾ_VMCreate (configure, ਨਿਰਾ);
  ਜੇ (UNLIKELY (vm == ਨਿਰਾ))
    {
      Wasm_ਕਿਨਾਰਾ_Configure_Delete (configure);
      error (EXIT_FAILURE, 0, "could not create wasmedge vm");
    }

  wasi_module = Wasm_ਕਿਨਾਰਾ_VMGet_Import_Module_Context (vm, Wasm_ਕਿਨਾਰਾ_Host_Registration_Wasi);
  ਜੇ (UNLIKELY (wasi_module == ਨਿਰਾ))
    {
      Wasm_ਕਿਨਾਰਾ_VMDelete (vm);
      Wasm_ਕਿਨਾਰਾ_Configure_Delete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge wasi module context");
    }

  ਲਈ (ਅੱਖਰ *ਅਚਲ *arg = argv; *arg != ਨਿਰਾ; ++arg, ++argn)
    ;
  ਬਾਹਰੀ ਅੱਖਰ **environ;
  ਲਈ (ਅੱਖਰ *ਅਚਲ *env = environ; *env != ਨਿਰਾ; ++env, ++envn)
    ;

  Wasm_ਕਿਨਾਰਾ_Module_Instance_Init_WASI (wasi_module, (ਅਚਲ ਅੱਖਰ *ਅਚਲ *) &argv[0], argn, (ਅਚਲ ਅੱਖਰ *ਅਚਲ *) &environ[0], envn, dirs, 1, ਨਿਰਾ, 0);

  ਨਤੀਜਾ = Wasm_ਕਿਨਾਰਾ_VMRun_Wasm_From_File (vm, pathname, Wasm_ਕਿਨਾਰਾ_String_Create_By_CString ("_start"), ਨਿਰਾ, 0, ਨਿਰਾ, 0);

  ਜੇ (UNLIKELY (! Wasm_ਕਿਨਾਰਾ_ਨਤੀਜਾ_OK (ਨਤੀਜਾ)))
    {
      Wasm_ਕਿਨਾਰਾ_VMDelete (vm);
      Wasm_ਕਿਨਾਰਾ_Configure_Delete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge result from VM");
    }

  Wasm_ਕਿਨਾਰਾ_VMDelete (vm);
  Wasm_ਕਿਨਾਰਾ_Configure_Delete (configure);
  ਬਾਹਰ_ਨਿਕਲੋ (EXIT_SUCCESS);
}

ਸਥਿਰ ਪੂਰਨ
wasmedge_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  ਵਾਪਸ wasm_can_handle_container (container, err);
}

// This works only when the plugin is present in /usr/lib/wasmedge
ਸਥਿਰ ਪੂਰਨ
libwasmedge_configure_container (ਖਾਲੀ *cookie arg_ਅਣਵਰਤਿਆ, ਗਿਣਤੀ handler_configure_phase phase,
                                 libcrun_context_t *context arg_ਅਣਵਰਤਿਆ, libcrun_container_t *container,
                                 ਅਚਲ ਅੱਖਰ *rootfs arg_ਅਣਵਰਤਿਆ, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  ਅੱਖਰ **container_env = def->process->env;
  bool has_plugin_ਰਸਤਾ = false, has_preload = false;

  ਲਈ (ਅੱਖਰ **env = container_env; env && *env; env++)
    {
      ਜੇ (ਲੜੀ_ਤੁਲਨਾ (*env, "WASMEDGE_PLUGIN_PATH=", 21) == 0)
        has_plugin_ਰਸਤਾ = true;
      ਨਹੀਂਤਾਂ ਜੇ (ਲੜੀ_ਤੁਲਨਾ (*env, "WASMEDGE_WASINN_PRELOAD=", 24) == 0)
        has_preload = true;
    }

  ਜੇ (! has_plugin_ਰਸਤਾ && ! has_preload)
    ਵਾਪਸ 0;

  ਜੇ (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    ਵਾਪਸ 0;

  // Check if /usr/lib/wasmedge is already present in spec
  ਜੇ (def->linux && def->mounts)
    {
      ਲਈ (size_t i = 0; i < def->mounts_len; i++)
        {
          ਜੇ (strcmp (def->mounts[i]->destination, "/usr/lib/wasmedge") == 0)
            ਵਾਪਸ 0;
        }
    }

  // Mount the plugin folder to /usr/lib/wasmedge with specific options
  ਅੱਖਰ *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/wasmedge ", "/usr/lib/wasmedge", options, 5, err);
  ਜੇ (ret < 0)
    {
      ਜੇ (crun_error_get_errno (err) != ENOENT)
        ਵਾਪਸ ret;
      crun_error_release (err);
    }

  ਵਾਪਸ 0;
}

ਬਣਤਰ custom_handler_s handler_wasmedge = {
  .name = "wasmedge",
  .alias = "wasm",
  .feature_string = "WASM:wasmedge",
  .supports_open_ਰੁੱਖ_namespace = false,
  .ਲੱਦੋ = libwasmedge_ਲੱਦੋ,
  .unload = libwasmedge_unload,
  .ਲੜੀਵਾਰ_func = libwasmedge_exec,
  .can_handle_container = wasmedge_can_handle_container,
  .configure_container = libwasmedge_configure_container,
};

#ਜੇ_ਅੰਤ
