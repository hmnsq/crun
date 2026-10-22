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
#تعریف_کړه GNU_سرچینه

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

#که_تعریف_شوی HAVE_DLOPEN
#  include <dlfcn.h>
#د_که_پای

#که_تعریف_شوی HAVE_WASMEDGE
#  include <wasmedge/wasmedge.h>
#د_که_پای

#إذا HAVE_DLOPEN && HAVE_WASMEDGE
ساكن صحيح
libwasmedge_بارول (فراغ **cookie, libcrun_error_t *err)
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
libwasmedge_exec (فراغ *cookie, __attribute__ ((ناکارول_شوی)) libcrun_container_t *container, ثابت حرف *pathname, حرف *ثابت argv[])
{
  Wasm_څنډه_Configure_Context *(*Wasm_څنډه_Configure_Create) (فراغ);
  فراغ (*Wasm_څنډه_Configure_Delete) (Wasm_څنډه_Configure_Context *Cxt);
  فراغ (*Wasm_څنډه_Configure_جمع_کړه_Proposal) (Wasm_څنډه_Configure_Context *Cxt, ثابت تعداد Wasm_څنډه_Proposal Prop);
  فراغ (*Wasm_څنډه_Configure_جمع_کړه_Host_Registration) (Wasm_څنډه_Configure_Context *Cxt, تعداد Wasm_څنډه_Host_Registration Host);
  Wasm_څنډه_VMContext *(*Wasm_څنډه_VMCreate) (ثابت Wasm_څنډه_Configure_Context *ConfCxt, Wasm_څنډه_Store_Context *StoreCxt);
  فراغ (*Wasm_څنډه_VMDelete) (Wasm_څنډه_VMContext *Cxt);
  Wasm_څنډه_پایله (*Wasm_څنډه_VMRegister_Module_From_File) (Wasm_څنډه_VMContext *Cxt, Wasm_څنډه_String ModuleName, ثابت حرف *لار_2);
  Wasm_څنډه_پایله (*Wasm_څنډه_VMRun_Wasm_From_File) (Wasm_څنډه_VMContext *Cxt, ثابت حرف *لار_2, ثابت Wasm_څنډه_String FuncName, ثابت Wasm_څنډه_ارزښت *Params, ثابت uint32_t ParamLen, Wasm_څنډه_ارزښت *Returns, ثابت uint32_t ReturnLen);
  فراغ (*Wasm_څنډه_Plugin_بارول_From_لار) (ثابت حرف *لار_2);
  فراغ (*Wasm_څنډه_Plugin_Init_WASINN) (ثابت حرف *ثابت *NNPreloads, ثابت uint32_t PreloadsLen);
  bool (*Wasm_څنډه_پایله_OK) (ثابت Wasm_څنډه_پایله Res);
  Wasm_څنډه_String (*Wasm_څنډه_String_Create_By_CString) (ثابت حرف *Str);
  uint32_t argn = 0;
  uint32_t envn = 0;
  ثابت حرف *dirs[2] = { "/:/", ".:." };
  Wasm_څنډه_Configure_Context *configure;
  Wasm_څنډه_VMContext *vm;
  Wasm_څنډه_پایله پایله;

  Wasm_څنډه_Module_Instance_Context *wasi_module;
  Wasm_څنډه_Module_Instance_Context *(*Wasm_څنډه_VMGet_Import_Module_Context) (Wasm_څنډه_VMContext *Cxt, ثابت تعداد Wasm_څنډه_Host_Registration Reg);
  فراغ (*Wasm_څنډه_Module_Instance_Init_WASI) (Wasm_څنډه_Module_Instance_Context *Cxt, ثابت حرف *ثابت *دلیلونه_2, ثابت uint32_t ArgLen, ثابت حرف *ثابت *Envs, ثابت uint32_t EnvLen, ثابت حرف *ثابت *Dirs, ثابت uint32_t DirLen, ثابت حرف *ثابت *Preopens, ثابت uint32_t PreopenLen);
  Wasm_څنډه_Module_Instance_Init_WASI = dlsym (cookie, "WasmEdge_ModuleInstanceInitWASI");

  Wasm_څنډه_Configure_Create = dlsym (cookie, "WasmEdge_ConfigureCreate");
  Wasm_څنډه_Configure_Delete = dlsym (cookie, "WasmEdge_ConfigureDelete");
  Wasm_څنډه_Configure_جمع_کړه_Proposal = dlsym (cookie, "WasmEdge_ConfigureAddProposal");
  Wasm_څنډه_Configure_جمع_کړه_Host_Registration = dlsym (cookie, "WasmEdge_ConfigureAddHostRegistration");
  Wasm_څنډه_VMCreate = dlsym (cookie, "WasmEdge_VMCreate");
  Wasm_څنډه_VMDelete = dlsym (cookie, "WasmEdge_VMDelete");
  Wasm_څنډه_VMRegister_Module_From_File = dlsym (cookie, "WasmEdge_VMRegisterModuleFromFile");
  Wasm_څنډه_VMGet_Import_Module_Context = dlsym (cookie, "WasmEdge_VMGetImportModuleContext");
  Wasm_څنډه_VMRun_Wasm_From_File = dlsym (cookie, "WasmEdge_VMRunWasmFromFile");
  Wasm_څنډه_Plugin_بارول_From_لار = dlsym (cookie, "WasmEdge_PluginLoadFromPath");
  Wasm_څنډه_Plugin_Init_WASINN = dlsym (cookie, "WasmEdge_PluginInitWASINN");
  Wasm_څنډه_پایله_OK = dlsym (cookie, "WasmEdge_ResultOK");
  Wasm_څنډه_String_Create_By_CString = dlsym (cookie, "WasmEdge_StringCreateByCString");

  إذا (Wasm_څنډه_Configure_Create == عدم || Wasm_څنډه_Configure_Delete == عدم || Wasm_څنډه_Configure_جمع_کړه_Proposal == عدم
      || Wasm_څنډه_Configure_جمع_کړه_Host_Registration == عدم || Wasm_څنډه_VMCreate == عدم || Wasm_څنډه_VMDelete == عدم
      || Wasm_څنډه_VMRegister_Module_From_File == عدم || Wasm_څنډه_VMGet_Import_Module_Context == عدم
      || Wasm_څنډه_Module_Instance_Init_WASI == عدم || Wasm_څنډه_VMRun_Wasm_From_File == عدم
      || Wasm_څنډه_پایله_OK == عدم || Wasm_څنډه_String_Create_By_CString == عدم)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmedge.so.0`");

  configure = Wasm_څنډه_Configure_Create ();
  إذا (UNLIKELY (configure == عدم))
    error (EXIT_FAILURE, 0, "could not create wasmedge configure");

  Wasm_څنډه_Configure_جمع_کړه_Proposal (configure, Wasm_څنډه_Proposal_Bulk_حافظه_Operations);
  Wasm_څنډه_Configure_جمع_کړه_Proposal (configure, Wasm_څنډه_Proposal_Reference_Types);
  Wasm_څنډه_Configure_جمع_کړه_Proposal (configure, Wasm_څنډه_Proposal_SIMD);
  Wasm_څنډه_Configure_جمع_کړه_Host_Registration (configure, Wasm_څنډه_Host_Registration_Wasi);
  // Check if the necessary environment variables are set
  ثابت حرف *plugin_لار_env = getenv ("WASMEDGE_PLUGIN_PATH");
  إذا (plugin_لار_env != عدم)
    Wasm_څنډه_Plugin_بارول_From_لار (plugin_لار_env);

  ثابت حرف *nnpreload_env = getenv ("WASMEDGE_WASINN_PRELOAD");
  إذا (nnpreload_env != عدم)
    Wasm_څنډه_Plugin_Init_WASINN (&nnpreload_env, 1);

  vm = Wasm_څنډه_VMCreate (configure, عدم);
  إذا (UNLIKELY (vm == عدم))
    {
      Wasm_څنډه_Configure_Delete (configure);
      error (EXIT_FAILURE, 0, "could not create wasmedge vm");
    }

  wasi_module = Wasm_څنډه_VMGet_Import_Module_Context (vm, Wasm_څنډه_Host_Registration_Wasi);
  إذا (UNLIKELY (wasi_module == عدم))
    {
      Wasm_څنډه_VMDelete (vm);
      Wasm_څنډه_Configure_Delete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge wasi module context");
    }

  لكل (حرف *ثابت *arg = argv; *arg != عدم; ++arg, ++argn)
    ;
  خارجي حرف **environ;
  لكل (حرف *ثابت *env = environ; *env != عدم; ++env, ++envn)
    ;

  Wasm_څنډه_Module_Instance_Init_WASI (wasi_module, (ثابت حرف *ثابت *) &argv[0], argn, (ثابت حرف *ثابت *) &environ[0], envn, dirs, 1, عدم, 0);

  پایله = Wasm_څنډه_VMRun_Wasm_From_File (vm, pathname, Wasm_څنډه_String_Create_By_CString ("_start"), عدم, 0, عدم, 0);

  إذا (UNLIKELY (! Wasm_څنډه_پایله_OK (پایله)))
    {
      Wasm_څنډه_VMDelete (vm);
      Wasm_څنډه_Configure_Delete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge result from VM");
    }

  Wasm_څنډه_VMDelete (vm);
  Wasm_څنډه_Configure_Delete (configure);
  وتل (EXIT_SUCCESS);
}

ساكن صحيح
wasmedge_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  أرجع wasm_can_handle_container (container, err);
}

// This works only when the plugin is present in /usr/lib/wasmedge
ساكن صحيح
libwasmedge_configure_container (فراغ *cookie arg_ناکارول_شوی, تعداد handler_configure_phase phase,
                                 libcrun_context_t *context arg_ناکارول_شوی, libcrun_container_t *container,
                                 ثابت حرف *rootfs arg_ناکارول_شوی, libcrun_error_t *err)
{
  صحيح ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  حرف **container_env = def->process->env;
  bool has_plugin_لار = false, has_preload = false;

  لكل (حرف **env = container_env; env && *env; env++)
    {
      إذا (مزي_پرتله (*env, "WASMEDGE_PLUGIN_PATH=", 21) == 0)
        has_plugin_لار = true;
      وإلا إذا (مزي_پرتله (*env, "WASMEDGE_WASINN_PRELOAD=", 24) == 0)
        has_preload = true;
    }

  إذا (! has_plugin_لار && ! has_preload)
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
  .supports_open_ونه_namespace = false,
  .بارول = libwasmedge_بارول,
  .unload = libwasmedge_unload,
  .پرلپسې_func = libwasmedge_exec,
  .can_handle_container = wasmedge_can_handle_container,
  .configure_container = libwasmedge_configure_container,
};

#د_که_پای
