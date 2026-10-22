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
#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DLOPEN
#  include <dlfcn.h>
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_WASMEDGE
#  include <wasmedge/wasmedge.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ HAVE_DLOPEN && HAVE_WASMEDGE
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libwasmedge_ತುಂಬು (ಶೂನ್ಯ **cookie, libcrun_error_t *err)
{
  ಶೂನ್ಯ *handle;

  handle = dlopen ("libwasmedge.so.0", RTLD_NOW);
  ಆದರೆ (handle == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not load `libwasmedge.so.0`: `%s`", dlerror ());
  *cookie = handle;

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libwasmedge_unload (ಶೂನ್ಯ *cookie, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ r;

  ಆದರೆ (cookie)
    {
      r = dlclose (cookie);
      ಆದರೆ (UNLIKELY (r < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libwasmedge_exec (ಶೂನ್ಯ *cookie, __attribute__ ((ಬಳಸದ)) libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ argv[])
{
  Wasm_ಅಂಚು_Configure_Context *(*Wasm_ಅಂಚು_Configure_Create) (ಶೂನ್ಯ);
  ಶೂನ್ಯ (*Wasm_ಅಂಚು_Configure_Delete) (Wasm_ಅಂಚು_Configure_Context *Cxt);
  ಶೂನ್ಯ (*Wasm_ಅಂಚು_Configure_ಕೂಡಿಸು_Proposal) (Wasm_ಅಂಚು_Configure_Context *Cxt, ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ Wasm_ಅಂಚು_Proposal Prop);
  ಶೂನ್ಯ (*Wasm_ಅಂಚು_Configure_ಕೂಡಿಸು_Host_Registration) (Wasm_ಅಂಚು_Configure_Context *Cxt, ಎಣಿಕೆ Wasm_ಅಂಚು_Host_Registration Host);
  Wasm_ಅಂಚು_VMContext *(*Wasm_ಅಂಚು_VMCreate) (ಸ್ಥಿರಾಂಕ Wasm_ಅಂಚು_Configure_Context *ConfCxt, Wasm_ಅಂಚು_Store_Context *StoreCxt);
  ಶೂನ್ಯ (*Wasm_ಅಂಚು_VMDelete) (Wasm_ಅಂಚು_VMContext *Cxt);
  Wasm_ಅಂಚು_ಫಲಿತಾಂಶ (*Wasm_ಅಂಚು_VMRegister_Module_From_File) (Wasm_ಅಂಚು_VMContext *Cxt, Wasm_ಅಂಚು_String ModuleName, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ_2);
  Wasm_ಅಂಚು_ಫಲಿತಾಂಶ (*Wasm_ಅಂಚು_VMRun_Wasm_From_File) (Wasm_ಅಂಚು_VMContext *Cxt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ_2, ಸ್ಥಿರಾಂಕ Wasm_ಅಂಚು_String FuncName, ಸ್ಥಿರಾಂಕ Wasm_ಅಂಚು_ಮೌಲ್ಯ *Params, ಸ್ಥಿರಾಂಕ uint32_t ParamLen, Wasm_ಅಂಚು_ಮೌಲ್ಯ *Returns, ಸ್ಥಿರಾಂಕ uint32_t ReturnLen);
  ಶೂನ್ಯ (*Wasm_ಅಂಚು_Plugin_ತುಂಬು_From_ಮಾರ್ಗ) (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ_2);
  ಶೂನ್ಯ (*Wasm_ಅಂಚು_Plugin_Init_WASINN) (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *NNPreloads, ಸ್ಥಿರಾಂಕ uint32_t PreloadsLen);
  bool (*Wasm_ಅಂಚು_ಫಲಿತಾಂಶ_OK) (ಸ್ಥಿರಾಂಕ Wasm_ಅಂಚು_ಫಲಿತಾಂಶ Res);
  Wasm_ಅಂಚು_String (*Wasm_ಅಂಚು_String_Create_By_CString) (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *Str);
  uint32_t argn = 0;
  uint32_t envn = 0;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dirs[2] = { "/:/", ".:." };
  Wasm_ಅಂಚು_Configure_Context *configure;
  Wasm_ಅಂಚು_VMContext *vm;
  Wasm_ಅಂಚು_ಫಲಿತಾಂಶ ಫಲಿತಾಂಶ;

  Wasm_ಅಂಚು_Module_Instance_Context *wasi_module;
  Wasm_ಅಂಚು_Module_Instance_Context *(*Wasm_ಅಂಚು_VMGet_Import_Module_Context) (Wasm_ಅಂಚು_VMContext *Cxt, ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ Wasm_ಅಂಚು_Host_Registration Reg);
  ಶೂನ್ಯ (*Wasm_ಅಂಚು_Module_Instance_Init_WASI) (Wasm_ಅಂಚು_Module_Instance_Context *Cxt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *ವಾದಗಳು_2, ಸ್ಥಿರಾಂಕ uint32_t ArgLen, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *Envs, ಸ್ಥಿರಾಂಕ uint32_t EnvLen, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *Dirs, ಸ್ಥಿರಾಂಕ uint32_t DirLen, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *Preopens, ಸ್ಥಿರಾಂಕ uint32_t PreopenLen);
  Wasm_ಅಂಚು_Module_Instance_Init_WASI = dlsym (cookie, "WasmEdge_ModuleInstanceInitWASI");

  Wasm_ಅಂಚು_Configure_Create = dlsym (cookie, "WasmEdge_ConfigureCreate");
  Wasm_ಅಂಚು_Configure_Delete = dlsym (cookie, "WasmEdge_ConfigureDelete");
  Wasm_ಅಂಚು_Configure_ಕೂಡಿಸು_Proposal = dlsym (cookie, "WasmEdge_ConfigureAddProposal");
  Wasm_ಅಂಚು_Configure_ಕೂಡಿಸು_Host_Registration = dlsym (cookie, "WasmEdge_ConfigureAddHostRegistration");
  Wasm_ಅಂಚು_VMCreate = dlsym (cookie, "WasmEdge_VMCreate");
  Wasm_ಅಂಚು_VMDelete = dlsym (cookie, "WasmEdge_VMDelete");
  Wasm_ಅಂಚು_VMRegister_Module_From_File = dlsym (cookie, "WasmEdge_VMRegisterModuleFromFile");
  Wasm_ಅಂಚು_VMGet_Import_Module_Context = dlsym (cookie, "WasmEdge_VMGetImportModuleContext");
  Wasm_ಅಂಚು_VMRun_Wasm_From_File = dlsym (cookie, "WasmEdge_VMRunWasmFromFile");
  Wasm_ಅಂಚು_Plugin_ತುಂಬು_From_ಮಾರ್ಗ = dlsym (cookie, "WasmEdge_PluginLoadFromPath");
  Wasm_ಅಂಚು_Plugin_Init_WASINN = dlsym (cookie, "WasmEdge_PluginInitWASINN");
  Wasm_ಅಂಚು_ಫಲಿತಾಂಶ_OK = dlsym (cookie, "WasmEdge_ResultOK");
  Wasm_ಅಂಚು_String_Create_By_CString = dlsym (cookie, "WasmEdge_StringCreateByCString");

  ಆದರೆ (Wasm_ಅಂಚು_Configure_Create == ಖಾಲಿ || Wasm_ಅಂಚು_Configure_Delete == ಖಾಲಿ || Wasm_ಅಂಚು_Configure_ಕೂಡಿಸು_Proposal == ಖಾಲಿ
      || Wasm_ಅಂಚು_Configure_ಕೂಡಿಸು_Host_Registration == ಖಾಲಿ || Wasm_ಅಂಚು_VMCreate == ಖಾಲಿ || Wasm_ಅಂಚು_VMDelete == ಖಾಲಿ
      || Wasm_ಅಂಚು_VMRegister_Module_From_File == ಖಾಲಿ || Wasm_ಅಂಚು_VMGet_Import_Module_Context == ಖಾಲಿ
      || Wasm_ಅಂಚು_Module_Instance_Init_WASI == ಖಾಲಿ || Wasm_ಅಂಚು_VMRun_Wasm_From_File == ಖಾಲಿ
      || Wasm_ಅಂಚು_ಫಲಿತಾಂಶ_OK == ಖಾಲಿ || Wasm_ಅಂಚು_String_Create_By_CString == ಖಾಲಿ)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmedge.so.0`");

  configure = Wasm_ಅಂಚು_Configure_Create ();
  ಆದರೆ (UNLIKELY (configure == ಖಾಲಿ))
    error (EXIT_FAILURE, 0, "could not create wasmedge configure");

  Wasm_ಅಂಚು_Configure_ಕೂಡಿಸು_Proposal (configure, Wasm_ಅಂಚು_Proposal_Bulk_ಸ್ಮರಣೆ_Operations);
  Wasm_ಅಂಚು_Configure_ಕೂಡಿಸು_Proposal (configure, Wasm_ಅಂಚು_Proposal_Reference_Types);
  Wasm_ಅಂಚು_Configure_ಕೂಡಿಸು_Proposal (configure, Wasm_ಅಂಚು_Proposal_SIMD);
  Wasm_ಅಂಚು_Configure_ಕೂಡಿಸು_Host_Registration (configure, Wasm_ಅಂಚು_Host_Registration_Wasi);
  // Check if the necessary environment variables are set
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *plugin_ಮಾರ್ಗ_env = getenv ("WASMEDGE_PLUGIN_PATH");
  ಆದರೆ (plugin_ಮಾರ್ಗ_env != ಖಾಲಿ)
    Wasm_ಅಂಚು_Plugin_ತುಂಬು_From_ಮಾರ್ಗ (plugin_ಮಾರ್ಗ_env);

  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *nnpreload_env = getenv ("WASMEDGE_WASINN_PRELOAD");
  ಆದರೆ (nnpreload_env != ಖಾಲಿ)
    Wasm_ಅಂಚು_Plugin_Init_WASINN (&nnpreload_env, 1);

  vm = Wasm_ಅಂಚು_VMCreate (configure, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (vm == ಖಾಲಿ))
    {
      Wasm_ಅಂಚು_Configure_Delete (configure);
      error (EXIT_FAILURE, 0, "could not create wasmedge vm");
    }

  wasi_module = Wasm_ಅಂಚು_VMGet_Import_Module_Context (vm, Wasm_ಅಂಚು_Host_Registration_Wasi);
  ಆದರೆ (UNLIKELY (wasi_module == ಖಾಲಿ))
    {
      Wasm_ಅಂಚು_VMDelete (vm);
      Wasm_ಅಂಚು_Configure_Delete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge wasi module context");
    }

  ಗಾಗಿ (ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *arg = argv; *arg != ಖಾಲಿ; ++arg, ++argn)
    ;
  ಬಾಹ್ಯ ಅಕ್ಷರ **environ;
  ಗಾಗಿ (ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *env = environ; *env != ಖಾಲಿ; ++env, ++envn)
    ;

  Wasm_ಅಂಚು_Module_Instance_Init_WASI (wasi_module, (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *) &argv[0], argn, (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *) &environ[0], envn, dirs, 1, ಖಾಲಿ, 0);

  ಫಲಿತಾಂಶ = Wasm_ಅಂಚು_VMRun_Wasm_From_File (vm, pathname, Wasm_ಅಂಚು_String_Create_By_CString ("_start"), ಖಾಲಿ, 0, ಖಾಲಿ, 0);

  ಆದರೆ (UNLIKELY (! Wasm_ಅಂಚು_ಫಲಿತಾಂಶ_OK (ಫಲಿತಾಂಶ)))
    {
      Wasm_ಅಂಚು_VMDelete (vm);
      Wasm_ಅಂಚು_Configure_Delete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge result from VM");
    }

  Wasm_ಅಂಚು_VMDelete (vm);
  Wasm_ಅಂಚು_Configure_Delete (configure);
  ನಿರ್ಗಮಿಸು (EXIT_SUCCESS);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
wasmedge_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ wasm_can_handle_container (container, err);
}

// This works only when the plugin is present in /usr/lib/wasmedge
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libwasmedge_configure_container (ಶೂನ್ಯ *cookie arg_ಬಳಸದ, ಎಣಿಕೆ handler_configure_phase phase,
                                 libcrun_context_t *context arg_ಬಳಸದ, libcrun_container_t *container,
                                 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs arg_ಬಳಸದ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  ಅಕ್ಷರ **container_env = def->process->env;
  bool has_plugin_ಮಾರ್ಗ = false, has_preload = false;

  ಗಾಗಿ (ಅಕ್ಷರ **env = container_env; env && *env; env++)
    {
      ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ (*env, "WASMEDGE_PLUGIN_PATH=", 21) == 0)
        has_plugin_ಮಾರ್ಗ = true;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ (*env, "WASMEDGE_WASINN_PRELOAD=", 24) == 0)
        has_preload = true;
    }

  ಆದರೆ (! has_plugin_ಮಾರ್ಗ && ! has_preload)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    ಹಿಂತಿರುಗಿ 0;

  // Check if /usr/lib/wasmedge is already present in spec
  ಆದರೆ (def->linux && def->mounts)
    {
      ಗಾಗಿ (size_t i = 0; i < def->mounts_len; i++)
        {
          ಆದರೆ (strcmp (def->mounts[i]->destination, "/usr/lib/wasmedge") == 0)
            ಹಿಂತಿರುಗಿ 0;
        }
    }

  // Mount the plugin folder to /usr/lib/wasmedge with specific options
  ಅಕ್ಷರ *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/wasmedge ", "/usr/lib/wasmedge", options, 5, err);
  ಆದರೆ (ret < 0)
    {
      ಆದರೆ (crun_error_get_errno (err) != ENOENT)
        ಹಿಂತಿರುಗಿ ret;
      crun_error_release (err);
    }

  ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ custom_handler_s handler_wasmedge = {
  .name = "wasmedge",
  .alias = "wasm",
  .feature_string = "WASM:wasmedge",
  .supports_open_ವೃಕ್ಷ_namespace = false,
  .ತುಂಬು = libwasmedge_ತುಂಬು,
  .unload = libwasmedge_unload,
  .ಸತತ_func = libwasmedge_exec,
  .can_handle_container = wasmedge_can_handle_container,
  .configure_container = libwasmedge_configure_container,
};

#ಆದರೆ_ಅಂತ್ಯ
