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
#تعریف_کن GNU_منبع

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

#اگر_تعریف_شده HAVE_DLOPEN
#  include <dlfcn.h>
#پایان_اگر

#اگر_تعریف_شده HAVE_WASMEDGE
#  include <wasmedge/wasmedge.h>
#پایان_اگر

#إذا HAVE_DLOPEN && HAVE_WASMEDGE
ساكن صحيح
libwasmedge_بارگیری (فراغ **cookie, libcrun_error_t *err)
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
libwasmedge_exec (فراغ *cookie, __attribute__ ((استفادهنشده)) libcrun_container_t *container, ثابت حرف *pathname, حرف *ثابت argv[])
{
  Wasm_یال_Configure_Context *(*Wasm_یال_Configure_Create) (فراغ);
  فراغ (*Wasm_یال_Configure_Delete) (Wasm_یال_Configure_Context *Cxt);
  فراغ (*Wasm_یال_Configure_جمع_کن_Proposal) (Wasm_یال_Configure_Context *Cxt, ثابت تعداد Wasm_یال_Proposal Prop);
  فراغ (*Wasm_یال_Configure_جمع_کن_Host_Registration) (Wasm_یال_Configure_Context *Cxt, تعداد Wasm_یال_Host_Registration Host);
  Wasm_یال_VMContext *(*Wasm_یال_VMCreate) (ثابت Wasm_یال_Configure_Context *ConfCxt, Wasm_یال_Store_Context *StoreCxt);
  فراغ (*Wasm_یال_VMDelete) (Wasm_یال_VMContext *Cxt);
  Wasm_یال_نتیجه (*Wasm_یال_VMRegister_Module_From_File) (Wasm_یال_VMContext *Cxt, Wasm_یال_String ModuleName, ثابت حرف *مسیر_2);
  Wasm_یال_نتیجه (*Wasm_یال_VMRun_Wasm_From_File) (Wasm_یال_VMContext *Cxt, ثابت حرف *مسیر_2, ثابت Wasm_یال_String FuncName, ثابت Wasm_یال_مقدار *Params, ثابت uint32_t ParamLen, Wasm_یال_مقدار *Returns, ثابت uint32_t ReturnLen);
  فراغ (*Wasm_یال_Plugin_بارگیری_From_مسیر) (ثابت حرف *مسیر_2);
  فراغ (*Wasm_یال_Plugin_Init_WASINN) (ثابت حرف *ثابت *NNPreloads, ثابت uint32_t PreloadsLen);
  bool (*Wasm_یال_نتیجه_OK) (ثابت Wasm_یال_نتیجه Res);
  Wasm_یال_String (*Wasm_یال_String_Create_By_CString) (ثابت حرف *Str);
  uint32_t argn = 0;
  uint32_t envn = 0;
  ثابت حرف *dirs[2] = { "/:/", ".:." };
  Wasm_یال_Configure_Context *configure;
  Wasm_یال_VMContext *vm;
  Wasm_یال_نتیجه نتیجه;

  Wasm_یال_Module_Instance_Context *wasi_module;
  Wasm_یال_Module_Instance_Context *(*Wasm_یال_VMGet_Import_Module_Context) (Wasm_یال_VMContext *Cxt, ثابت تعداد Wasm_یال_Host_Registration Reg);
  فراغ (*Wasm_یال_Module_Instance_Init_WASI) (Wasm_یال_Module_Instance_Context *Cxt, ثابت حرف *ثابت *آرگومانها_2, ثابت uint32_t ArgLen, ثابت حرف *ثابت *Envs, ثابت uint32_t EnvLen, ثابت حرف *ثابت *Dirs, ثابت uint32_t DirLen, ثابت حرف *ثابت *Preopens, ثابت uint32_t PreopenLen);
  Wasm_یال_Module_Instance_Init_WASI = dlsym (cookie, "WasmEdge_ModuleInstanceInitWASI");

  Wasm_یال_Configure_Create = dlsym (cookie, "WasmEdge_ConfigureCreate");
  Wasm_یال_Configure_Delete = dlsym (cookie, "WasmEdge_ConfigureDelete");
  Wasm_یال_Configure_جمع_کن_Proposal = dlsym (cookie, "WasmEdge_ConfigureAddProposal");
  Wasm_یال_Configure_جمع_کن_Host_Registration = dlsym (cookie, "WasmEdge_ConfigureAddHostRegistration");
  Wasm_یال_VMCreate = dlsym (cookie, "WasmEdge_VMCreate");
  Wasm_یال_VMDelete = dlsym (cookie, "WasmEdge_VMDelete");
  Wasm_یال_VMRegister_Module_From_File = dlsym (cookie, "WasmEdge_VMRegisterModuleFromFile");
  Wasm_یال_VMGet_Import_Module_Context = dlsym (cookie, "WasmEdge_VMGetImportModuleContext");
  Wasm_یال_VMRun_Wasm_From_File = dlsym (cookie, "WasmEdge_VMRunWasmFromFile");
  Wasm_یال_Plugin_بارگیری_From_مسیر = dlsym (cookie, "WasmEdge_PluginLoadFromPath");
  Wasm_یال_Plugin_Init_WASINN = dlsym (cookie, "WasmEdge_PluginInitWASINN");
  Wasm_یال_نتیجه_OK = dlsym (cookie, "WasmEdge_ResultOK");
  Wasm_یال_String_Create_By_CString = dlsym (cookie, "WasmEdge_StringCreateByCString");

  إذا (Wasm_یال_Configure_Create == عدم || Wasm_یال_Configure_Delete == عدم || Wasm_یال_Configure_جمع_کن_Proposal == عدم
      || Wasm_یال_Configure_جمع_کن_Host_Registration == عدم || Wasm_یال_VMCreate == عدم || Wasm_یال_VMDelete == عدم
      || Wasm_یال_VMRegister_Module_From_File == عدم || Wasm_یال_VMGet_Import_Module_Context == عدم
      || Wasm_یال_Module_Instance_Init_WASI == عدم || Wasm_یال_VMRun_Wasm_From_File == عدم
      || Wasm_یال_نتیجه_OK == عدم || Wasm_یال_String_Create_By_CString == عدم)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmedge.so.0`");

  configure = Wasm_یال_Configure_Create ();
  إذا (UNLIKELY (configure == عدم))
    error (EXIT_FAILURE, 0, "could not create wasmedge configure");

  Wasm_یال_Configure_جمع_کن_Proposal (configure, Wasm_یال_Proposal_Bulk_حافظه_Operations);
  Wasm_یال_Configure_جمع_کن_Proposal (configure, Wasm_یال_Proposal_Reference_Types);
  Wasm_یال_Configure_جمع_کن_Proposal (configure, Wasm_یال_Proposal_SIMD);
  Wasm_یال_Configure_جمع_کن_Host_Registration (configure, Wasm_یال_Host_Registration_Wasi);
  // Check if the necessary environment variables are set
  ثابت حرف *plugin_مسیر_env = getenv ("WASMEDGE_PLUGIN_PATH");
  إذا (plugin_مسیر_env != عدم)
    Wasm_یال_Plugin_بارگیری_From_مسیر (plugin_مسیر_env);

  ثابت حرف *nnpreload_env = getenv ("WASMEDGE_WASINN_PRELOAD");
  إذا (nnpreload_env != عدم)
    Wasm_یال_Plugin_Init_WASINN (&nnpreload_env, 1);

  vm = Wasm_یال_VMCreate (configure, عدم);
  إذا (UNLIKELY (vm == عدم))
    {
      Wasm_یال_Configure_Delete (configure);
      error (EXIT_FAILURE, 0, "could not create wasmedge vm");
    }

  wasi_module = Wasm_یال_VMGet_Import_Module_Context (vm, Wasm_یال_Host_Registration_Wasi);
  إذا (UNLIKELY (wasi_module == عدم))
    {
      Wasm_یال_VMDelete (vm);
      Wasm_یال_Configure_Delete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge wasi module context");
    }

  لكل (حرف *ثابت *arg = argv; *arg != عدم; ++arg, ++argn)
    ;
  خارجي حرف **environ;
  لكل (حرف *ثابت *env = environ; *env != عدم; ++env, ++envn)
    ;

  Wasm_یال_Module_Instance_Init_WASI (wasi_module, (ثابت حرف *ثابت *) &argv[0], argn, (ثابت حرف *ثابت *) &environ[0], envn, dirs, 1, عدم, 0);

  نتیجه = Wasm_یال_VMRun_Wasm_From_File (vm, pathname, Wasm_یال_String_Create_By_CString ("_start"), عدم, 0, عدم, 0);

  إذا (UNLIKELY (! Wasm_یال_نتیجه_OK (نتیجه)))
    {
      Wasm_یال_VMDelete (vm);
      Wasm_یال_Configure_Delete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge result from VM");
    }

  Wasm_یال_VMDelete (vm);
  Wasm_یال_Configure_Delete (configure);
  خروج (EXIT_SUCCESS);
}

ساكن صحيح
wasmedge_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  أرجع wasm_can_handle_container (container, err);
}

// This works only when the plugin is present in /usr/lib/wasmedge
ساكن صحيح
libwasmedge_configure_container (فراغ *cookie arg_استفادهنشده, تعداد handler_configure_phase phase,
                                 libcrun_context_t *context arg_استفادهنشده, libcrun_container_t *container,
                                 ثابت حرف *rootfs arg_استفادهنشده, libcrun_error_t *err)
{
  صحيح ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  حرف **container_env = def->process->env;
  bool has_plugin_مسیر = false, has_preload = false;

  لكل (حرف **env = container_env; env && *env; env++)
    {
      إذا (مقایسه_رشته (*env, "WASMEDGE_PLUGIN_PATH=", 21) == 0)
        has_plugin_مسیر = true;
      وإلا إذا (مقایسه_رشته (*env, "WASMEDGE_WASINN_PRELOAD=", 24) == 0)
        has_preload = true;
    }

  إذا (! has_plugin_مسیر && ! has_preload)
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
  .supports_open_درخت_namespace = false,
  .بارگیری = libwasmedge_بارگیری,
  .unload = libwasmedge_unload,
  .پیاپی_func = libwasmedge_exec,
  .can_handle_container = wasmedge_can_handle_container,
  .configure_container = libwasmedge_configure_container,
};

#پایان_اگر
