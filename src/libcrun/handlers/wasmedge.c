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
#وصف_ڪريو GNU_ذريعو

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

#جيڪڏهن_وصف_ٿيل HAVE_DLOPEN
#  include <dlfcn.h>
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HAVE_WASMEDGE
#  include <wasmedge/wasmedge.h>
#جيڪڏهن_پورو

#إذا HAVE_DLOPEN && HAVE_WASMEDGE
ساكن صحيح
libwasmedge_ڀريو (فراغ **cookie, libcrun_error_t *err)
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
libwasmedge_exec (فراغ *cookie, __attribute__ ((اڻ_استعمال_ٿيل)) libcrun_container_t *container, ثابت حرف *pathname, حرف *ثابت argv[])
{
  Wasm_ڪنارو_Configure_Context *(*Wasm_ڪنارو_Configure_Create) (فراغ);
  فراغ (*Wasm_ڪنارو_Configure_Delete) (Wasm_ڪنارو_Configure_Context *Cxt);
  فراغ (*Wasm_ڪنارو_Configure_جوڙيو_Proposal) (Wasm_ڪنارو_Configure_Context *Cxt, ثابت تعداد Wasm_ڪنارو_Proposal Prop);
  فراغ (*Wasm_ڪنارو_Configure_جوڙيو_Host_Registration) (Wasm_ڪنارو_Configure_Context *Cxt, تعداد Wasm_ڪنارو_Host_Registration Host);
  Wasm_ڪنارو_VMContext *(*Wasm_ڪنارو_VMCreate) (ثابت Wasm_ڪنارو_Configure_Context *ConfCxt, Wasm_ڪنارو_Store_Context *StoreCxt);
  فراغ (*Wasm_ڪنارو_VMDelete) (Wasm_ڪنارو_VMContext *Cxt);
  Wasm_ڪنارو_نتيجو (*Wasm_ڪنارو_VMRegister_Module_From_File) (Wasm_ڪنارو_VMContext *Cxt, Wasm_ڪنارو_String ModuleName, ثابت حرف *رستو_2);
  Wasm_ڪنارو_نتيجو (*Wasm_ڪنارو_VMRun_Wasm_From_File) (Wasm_ڪنارو_VMContext *Cxt, ثابت حرف *رستو_2, ثابت Wasm_ڪنارو_String FuncName, ثابت Wasm_ڪنارو_قدر *Params, ثابت uint32_t ParamLen, Wasm_ڪنارو_قدر *Returns, ثابت uint32_t ReturnLen);
  فراغ (*Wasm_ڪنارو_Plugin_ڀريو_From_رستو) (ثابت حرف *رستو_2);
  فراغ (*Wasm_ڪنارو_Plugin_Init_WASINN) (ثابت حرف *ثابت *NNPreloads, ثابت uint32_t PreloadsLen);
  bool (*Wasm_ڪنارو_نتيجو_OK) (ثابت Wasm_ڪنارو_نتيجو Res);
  Wasm_ڪنارو_String (*Wasm_ڪنارو_String_Create_By_CString) (ثابت حرف *Str);
  uint32_t argn = 0;
  uint32_t envn = 0;
  ثابت حرف *dirs[2] = { "/:/", ".:." };
  Wasm_ڪنارو_Configure_Context *configure;
  Wasm_ڪنارو_VMContext *vm;
  Wasm_ڪنارو_نتيجو نتيجو;

  Wasm_ڪنارو_Module_Instance_Context *wasi_module;
  Wasm_ڪنارو_Module_Instance_Context *(*Wasm_ڪنارو_VMGet_Import_Module_Context) (Wasm_ڪنارو_VMContext *Cxt, ثابت تعداد Wasm_ڪنارو_Host_Registration Reg);
  فراغ (*Wasm_ڪنارو_Module_Instance_Init_WASI) (Wasm_ڪنارو_Module_Instance_Context *Cxt, ثابت حرف *ثابت *دليلون_2, ثابت uint32_t ArgLen, ثابت حرف *ثابت *Envs, ثابت uint32_t EnvLen, ثابت حرف *ثابت *Dirs, ثابت uint32_t DirLen, ثابت حرف *ثابت *Preopens, ثابت uint32_t PreopenLen);
  Wasm_ڪنارو_Module_Instance_Init_WASI = dlsym (cookie, "WasmEdge_ModuleInstanceInitWASI");

  Wasm_ڪنارو_Configure_Create = dlsym (cookie, "WasmEdge_ConfigureCreate");
  Wasm_ڪنارو_Configure_Delete = dlsym (cookie, "WasmEdge_ConfigureDelete");
  Wasm_ڪنارو_Configure_جوڙيو_Proposal = dlsym (cookie, "WasmEdge_ConfigureAddProposal");
  Wasm_ڪنارو_Configure_جوڙيو_Host_Registration = dlsym (cookie, "WasmEdge_ConfigureAddHostRegistration");
  Wasm_ڪنارو_VMCreate = dlsym (cookie, "WasmEdge_VMCreate");
  Wasm_ڪنارو_VMDelete = dlsym (cookie, "WasmEdge_VMDelete");
  Wasm_ڪنارو_VMRegister_Module_From_File = dlsym (cookie, "WasmEdge_VMRegisterModuleFromFile");
  Wasm_ڪنارو_VMGet_Import_Module_Context = dlsym (cookie, "WasmEdge_VMGetImportModuleContext");
  Wasm_ڪنارو_VMRun_Wasm_From_File = dlsym (cookie, "WasmEdge_VMRunWasmFromFile");
  Wasm_ڪنارو_Plugin_ڀريو_From_رستو = dlsym (cookie, "WasmEdge_PluginLoadFromPath");
  Wasm_ڪنارو_Plugin_Init_WASINN = dlsym (cookie, "WasmEdge_PluginInitWASINN");
  Wasm_ڪنارو_نتيجو_OK = dlsym (cookie, "WasmEdge_ResultOK");
  Wasm_ڪنارو_String_Create_By_CString = dlsym (cookie, "WasmEdge_StringCreateByCString");

  إذا (Wasm_ڪنارو_Configure_Create == عدم || Wasm_ڪنارو_Configure_Delete == عدم || Wasm_ڪنارو_Configure_جوڙيو_Proposal == عدم
      || Wasm_ڪنارو_Configure_جوڙيو_Host_Registration == عدم || Wasm_ڪنارو_VMCreate == عدم || Wasm_ڪنارو_VMDelete == عدم
      || Wasm_ڪنارو_VMRegister_Module_From_File == عدم || Wasm_ڪنارو_VMGet_Import_Module_Context == عدم
      || Wasm_ڪنارو_Module_Instance_Init_WASI == عدم || Wasm_ڪنارو_VMRun_Wasm_From_File == عدم
      || Wasm_ڪنارو_نتيجو_OK == عدم || Wasm_ڪنارو_String_Create_By_CString == عدم)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmedge.so.0`");

  configure = Wasm_ڪنارو_Configure_Create ();
  إذا (UNLIKELY (configure == عدم))
    error (EXIT_FAILURE, 0, "could not create wasmedge configure");

  Wasm_ڪنارو_Configure_جوڙيو_Proposal (configure, Wasm_ڪنارو_Proposal_Bulk_يادگيري_Operations);
  Wasm_ڪنارو_Configure_جوڙيو_Proposal (configure, Wasm_ڪنارو_Proposal_Reference_Types);
  Wasm_ڪنارو_Configure_جوڙيو_Proposal (configure, Wasm_ڪنارو_Proposal_SIMD);
  Wasm_ڪنارو_Configure_جوڙيو_Host_Registration (configure, Wasm_ڪنارو_Host_Registration_Wasi);
  // Check if the necessary environment variables are set
  ثابت حرف *plugin_رستو_env = getenv ("WASMEDGE_PLUGIN_PATH");
  إذا (plugin_رستو_env != عدم)
    Wasm_ڪنارو_Plugin_ڀريو_From_رستو (plugin_رستو_env);

  ثابت حرف *nnpreload_env = getenv ("WASMEDGE_WASINN_PRELOAD");
  إذا (nnpreload_env != عدم)
    Wasm_ڪنارو_Plugin_Init_WASINN (&nnpreload_env, 1);

  vm = Wasm_ڪنارو_VMCreate (configure, عدم);
  إذا (UNLIKELY (vm == عدم))
    {
      Wasm_ڪنارو_Configure_Delete (configure);
      error (EXIT_FAILURE, 0, "could not create wasmedge vm");
    }

  wasi_module = Wasm_ڪنارو_VMGet_Import_Module_Context (vm, Wasm_ڪنارو_Host_Registration_Wasi);
  إذا (UNLIKELY (wasi_module == عدم))
    {
      Wasm_ڪنارو_VMDelete (vm);
      Wasm_ڪنارو_Configure_Delete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge wasi module context");
    }

  لكل (حرف *ثابت *arg = argv; *arg != عدم; ++arg, ++argn)
    ;
  خارجي حرف **environ;
  لكل (حرف *ثابت *env = environ; *env != عدم; ++env, ++envn)
    ;

  Wasm_ڪنارو_Module_Instance_Init_WASI (wasi_module, (ثابت حرف *ثابت *) &argv[0], argn, (ثابت حرف *ثابت *) &environ[0], envn, dirs, 1, عدم, 0);

  نتيجو = Wasm_ڪنارو_VMRun_Wasm_From_File (vm, pathname, Wasm_ڪنارو_String_Create_By_CString ("_start"), عدم, 0, عدم, 0);

  إذا (UNLIKELY (! Wasm_ڪنارو_نتيجو_OK (نتيجو)))
    {
      Wasm_ڪنارو_VMDelete (vm);
      Wasm_ڪنارو_Configure_Delete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge result from VM");
    }

  Wasm_ڪنارو_VMDelete (vm);
  Wasm_ڪنارو_Configure_Delete (configure);
  ٻاهر_نڪرو (EXIT_SUCCESS);
}

ساكن صحيح
wasmedge_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  أرجع wasm_can_handle_container (container, err);
}

// This works only when the plugin is present in /usr/lib/wasmedge
ساكن صحيح
libwasmedge_configure_container (فراغ *cookie arg_اڻ_استعمال_ٿيل, تعداد handler_configure_phase phase,
                                 libcrun_context_t *context arg_اڻ_استعمال_ٿيل, libcrun_container_t *container,
                                 ثابت حرف *rootfs arg_اڻ_استعمال_ٿيل, libcrun_error_t *err)
{
  صحيح ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  حرف **container_env = def->process->env;
  bool has_plugin_رستو = false, has_preload = false;

  لكل (حرف **env = container_env; env && *env; env++)
    {
      إذا (لڙي_ڀيٽ (*env, "WASMEDGE_PLUGIN_PATH=", 21) == 0)
        has_plugin_رستو = true;
      وإلا إذا (لڙي_ڀيٽ (*env, "WASMEDGE_WASINN_PRELOAD=", 24) == 0)
        has_preload = true;
    }

  إذا (! has_plugin_رستو && ! has_preload)
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
  .supports_open_وڻ_namespace = false,
  .ڀريو = libwasmedge_ڀريو,
  .unload = libwasmedge_unload,
  .لاڳيتو_func = libwasmedge_exec,
  .can_handle_container = wasmedge_can_handle_container,
  .configure_container = libwasmedge_configure_container,
};

#جيڪڏهن_پورو
