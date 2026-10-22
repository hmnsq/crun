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
#تعریف_کرو GNU_ماخذ

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

#اگر_تعریف_شدہ HAVE_DLOPEN
#  include <dlfcn.h>
#اگر_ختم

#اگر_تعریف_شدہ HAVE_WASMEDGE
#  include <wasmedge/wasmedge.h>
#اگر_ختم

#اگر HAVE_DLOPEN && HAVE_WASMEDGE
ساکن صحیح
libwasmedge_بھرو (خالی **cookie, libcrun_error_t *err)
{
  خالی *handle;

  handle = dlopen ("libwasmedge.so.0", RTLD_NOW);
  اگر (handle == تہی)
    واپس crun_make_error (err, 0, "could not load `libwasmedge.so.0`: `%s`", dlerror ());
  *cookie = handle;

  واپس 0;
}

ساکن صحیح
libwasmedge_unload (خالی *cookie, libcrun_error_t *err)
{
  صحیح r;

  اگر (cookie)
    {
      r = dlclose (cookie);
      اگر (UNLIKELY (r < 0))
        واپس crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  واپس 0;
}

ساکن صحیح
libwasmedge_exec (خالی *cookie, __attribute__ ((غیر_مستعمل)) libcrun_container_t *container, ثابت حرف *pathname, حرف *ثابت argv[])
{
  Wasm_کنارہ_Configure_Context *(*Wasm_کنارہ_Configure_Create) (خالی);
  خالی (*Wasm_کنارہ_Configure_Delete) (Wasm_کنارہ_Configure_Context *Cxt);
  خالی (*Wasm_کنارہ_Configure_جوڑو_Proposal) (Wasm_کنارہ_Configure_Context *Cxt, ثابت شمار Wasm_کنارہ_Proposal Prop);
  خالی (*Wasm_کنارہ_Configure_جوڑو_Host_Registration) (Wasm_کنارہ_Configure_Context *Cxt, شمار Wasm_کنارہ_Host_Registration Host);
  Wasm_کنارہ_VMContext *(*Wasm_کنارہ_VMCreate) (ثابت Wasm_کنارہ_Configure_Context *ConfCxt, Wasm_کنارہ_Store_Context *StoreCxt);
  خالی (*Wasm_کنارہ_VMDelete) (Wasm_کنارہ_VMContext *Cxt);
  Wasm_کنارہ_نتیجہ (*Wasm_کنارہ_VMRegister_Module_From_File) (Wasm_کنارہ_VMContext *Cxt, Wasm_کنارہ_String ModuleName, ثابت حرف *راستہ_2);
  Wasm_کنارہ_نتیجہ (*Wasm_کنارہ_VMRun_Wasm_From_File) (Wasm_کنارہ_VMContext *Cxt, ثابت حرف *راستہ_2, ثابت Wasm_کنارہ_String FuncName, ثابت Wasm_کنارہ_قدر *Params, ثابت uint32_t ParamLen, Wasm_کنارہ_قدر *Returns, ثابت uint32_t ReturnLen);
  خالی (*Wasm_کنارہ_Plugin_بھرو_From_راستہ) (ثابت حرف *راستہ_2);
  خالی (*Wasm_کنارہ_Plugin_Init_WASINN) (ثابت حرف *ثابت *NNPreloads, ثابت uint32_t PreloadsLen);
  bool (*Wasm_کنارہ_نتیجہ_OK) (ثابت Wasm_کنارہ_نتیجہ Res);
  Wasm_کنارہ_String (*Wasm_کنارہ_String_Create_By_CString) (ثابت حرف *Str);
  uint32_t argn = 0;
  uint32_t envn = 0;
  ثابت حرف *dirs[2] = { "/:/", ".:." };
  Wasm_کنارہ_Configure_Context *configure;
  Wasm_کنارہ_VMContext *vm;
  Wasm_کنارہ_نتیجہ نتیجہ;

  Wasm_کنارہ_Module_Instance_Context *wasi_module;
  Wasm_کنارہ_Module_Instance_Context *(*Wasm_کنارہ_VMGet_Import_Module_Context) (Wasm_کنارہ_VMContext *Cxt, ثابت شمار Wasm_کنارہ_Host_Registration Reg);
  خالی (*Wasm_کنارہ_Module_Instance_Init_WASI) (Wasm_کنارہ_Module_Instance_Context *Cxt, ثابت حرف *ثابت *دلائل_2, ثابت uint32_t ArgLen, ثابت حرف *ثابت *Envs, ثابت uint32_t EnvLen, ثابت حرف *ثابت *Dirs, ثابت uint32_t DirLen, ثابت حرف *ثابت *Preopens, ثابت uint32_t PreopenLen);
  Wasm_کنارہ_Module_Instance_Init_WASI = dlsym (cookie, "WasmEdge_ModuleInstanceInitWASI");

  Wasm_کنارہ_Configure_Create = dlsym (cookie, "WasmEdge_ConfigureCreate");
  Wasm_کنارہ_Configure_Delete = dlsym (cookie, "WasmEdge_ConfigureDelete");
  Wasm_کنارہ_Configure_جوڑو_Proposal = dlsym (cookie, "WasmEdge_ConfigureAddProposal");
  Wasm_کنارہ_Configure_جوڑو_Host_Registration = dlsym (cookie, "WasmEdge_ConfigureAddHostRegistration");
  Wasm_کنارہ_VMCreate = dlsym (cookie, "WasmEdge_VMCreate");
  Wasm_کنارہ_VMDelete = dlsym (cookie, "WasmEdge_VMDelete");
  Wasm_کنارہ_VMRegister_Module_From_File = dlsym (cookie, "WasmEdge_VMRegisterModuleFromFile");
  Wasm_کنارہ_VMGet_Import_Module_Context = dlsym (cookie, "WasmEdge_VMGetImportModuleContext");
  Wasm_کنارہ_VMRun_Wasm_From_File = dlsym (cookie, "WasmEdge_VMRunWasmFromFile");
  Wasm_کنارہ_Plugin_بھرو_From_راستہ = dlsym (cookie, "WasmEdge_PluginLoadFromPath");
  Wasm_کنارہ_Plugin_Init_WASINN = dlsym (cookie, "WasmEdge_PluginInitWASINN");
  Wasm_کنارہ_نتیجہ_OK = dlsym (cookie, "WasmEdge_ResultOK");
  Wasm_کنارہ_String_Create_By_CString = dlsym (cookie, "WasmEdge_StringCreateByCString");

  اگر (Wasm_کنارہ_Configure_Create == تہی || Wasm_کنارہ_Configure_Delete == تہی || Wasm_کنارہ_Configure_جوڑو_Proposal == تہی
      || Wasm_کنارہ_Configure_جوڑو_Host_Registration == تہی || Wasm_کنارہ_VMCreate == تہی || Wasm_کنارہ_VMDelete == تہی
      || Wasm_کنارہ_VMRegister_Module_From_File == تہی || Wasm_کنارہ_VMGet_Import_Module_Context == تہی
      || Wasm_کنارہ_Module_Instance_Init_WASI == تہی || Wasm_کنارہ_VMRun_Wasm_From_File == تہی
      || Wasm_کنارہ_نتیجہ_OK == تہی || Wasm_کنارہ_String_Create_By_CString == تہی)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmedge.so.0`");

  configure = Wasm_کنارہ_Configure_Create ();
  اگر (UNLIKELY (configure == تہی))
    error (EXIT_FAILURE, 0, "could not create wasmedge configure");

  Wasm_کنارہ_Configure_جوڑو_Proposal (configure, Wasm_کنارہ_Proposal_Bulk_حافظہ_Operations);
  Wasm_کنارہ_Configure_جوڑو_Proposal (configure, Wasm_کنارہ_Proposal_Reference_Types);
  Wasm_کنارہ_Configure_جوڑو_Proposal (configure, Wasm_کنارہ_Proposal_SIMD);
  Wasm_کنارہ_Configure_جوڑو_Host_Registration (configure, Wasm_کنارہ_Host_Registration_Wasi);
  // Check if the necessary environment variables are set
  ثابت حرف *plugin_راستہ_env = getenv ("WASMEDGE_PLUGIN_PATH");
  اگر (plugin_راستہ_env != تہی)
    Wasm_کنارہ_Plugin_بھرو_From_راستہ (plugin_راستہ_env);

  ثابت حرف *nnpreload_env = getenv ("WASMEDGE_WASINN_PRELOAD");
  اگر (nnpreload_env != تہی)
    Wasm_کنارہ_Plugin_Init_WASINN (&nnpreload_env, 1);

  vm = Wasm_کنارہ_VMCreate (configure, تہی);
  اگر (UNLIKELY (vm == تہی))
    {
      Wasm_کنارہ_Configure_Delete (configure);
      error (EXIT_FAILURE, 0, "could not create wasmedge vm");
    }

  wasi_module = Wasm_کنارہ_VMGet_Import_Module_Context (vm, Wasm_کنارہ_Host_Registration_Wasi);
  اگر (UNLIKELY (wasi_module == تہی))
    {
      Wasm_کنارہ_VMDelete (vm);
      Wasm_کنارہ_Configure_Delete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge wasi module context");
    }

  برائے (حرف *ثابت *arg = argv; *arg != تہی; ++arg, ++argn)
    ;
  خارجی حرف **environ;
  برائے (حرف *ثابت *env = environ; *env != تہی; ++env, ++envn)
    ;

  Wasm_کنارہ_Module_Instance_Init_WASI (wasi_module, (ثابت حرف *ثابت *) &argv[0], argn, (ثابت حرف *ثابت *) &environ[0], envn, dirs, 1, تہی, 0);

  نتیجہ = Wasm_کنارہ_VMRun_Wasm_From_File (vm, pathname, Wasm_کنارہ_String_Create_By_CString ("_start"), تہی, 0, تہی, 0);

  اگر (UNLIKELY (! Wasm_کنارہ_نتیجہ_OK (نتیجہ)))
    {
      Wasm_کنارہ_VMDelete (vm);
      Wasm_کنارہ_Configure_Delete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge result from VM");
    }

  Wasm_کنارہ_VMDelete (vm);
  Wasm_کنارہ_Configure_Delete (configure);
  باہر_نکلو (EXIT_SUCCESS);
}

ساکن صحیح
wasmedge_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  واپس wasm_can_handle_container (container, err);
}

// This works only when the plugin is present in /usr/lib/wasmedge
ساکن صحیح
libwasmedge_configure_container (خالی *cookie arg_غیر_مستعمل, شمار handler_configure_phase phase,
                                 libcrun_context_t *context arg_غیر_مستعمل, libcrun_container_t *container,
                                 ثابت حرف *rootfs arg_غیر_مستعمل, libcrun_error_t *err)
{
  صحیح ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  حرف **container_env = def->process->env;
  bool has_plugin_راستہ = false, has_preload = false;

  برائے (حرف **env = container_env; env && *env; env++)
    {
      اگر (لڑی_موازنہ (*env, "WASMEDGE_PLUGIN_PATH=", 21) == 0)
        has_plugin_راستہ = true;
      ورنہ اگر (لڑی_موازنہ (*env, "WASMEDGE_WASINN_PRELOAD=", 24) == 0)
        has_preload = true;
    }

  اگر (! has_plugin_راستہ && ! has_preload)
    واپس 0;

  اگر (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    واپس 0;

  // Check if /usr/lib/wasmedge is already present in spec
  اگر (def->linux && def->mounts)
    {
      برائے (size_t i = 0; i < def->mounts_len; i++)
        {
          اگر (strcmp (def->mounts[i]->destination, "/usr/lib/wasmedge") == 0)
            واپس 0;
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
  اگر (ret < 0)
    {
      اگر (crun_error_get_errno (err) != ENOENT)
        واپس ret;
      crun_error_release (err);
    }

  واپس 0;
}

ساخت custom_handler_s handler_wasmedge = {
  .name = "wasmedge",
  .alias = "wasm",
  .feature_string = "WASM:wasmedge",
  .supports_open_درخت_namespace = false,
  .بھرو = libwasmedge_بھرو,
  .unload = libwasmedge_unload,
  .تسلسل_func = libwasmedge_exec,
  .can_handle_container = wasmedge_can_handle_container,
  .configure_container = libwasmedge_configure_container,
};

#اگر_ختم
