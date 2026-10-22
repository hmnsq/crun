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
#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ "../custom-handler.h"
#అన్తర్భావ "../container.h"
#అన్తర్భావ "../utils.h"
#అన్తర్భావ "../linux.h"
#అన్తర్భావ "handler-utils.h"
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <errno.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <sched.h>

#నిర్వచితమైతే HAVE_DLOPEN
#  include <dlfcn.h>
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_WASMEDGE
#  include <wasmedge/wasmedge.h>
#అయితే_ముగింపు

#ఒకవేళ HAVE_DLOPEN && HAVE_WASMEDGE
స్థిర పూర్ణాంకం
libwasmedge_ఎక్కించు (శూన్య **cookie, libcrun_error_t *err)
{
  శూన్య *handle;

  handle = dlopen ("libwasmedge.so.0", RTLD_NOW);
  ఒకవేళ (handle == ఖాళీ)
    తిరిగి crun_make_error (err, 0, "could not load `libwasmedge.so.0`: `%s`", dlerror ());
  *cookie = handle;

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
libwasmedge_unload (శూన్య *cookie, libcrun_error_t *err)
{
  పూర్ణాంకం r;

  ఒకవేళ (cookie)
    {
      r = dlclose (cookie);
      ఒకవేళ (UNLIKELY (r < 0))
        తిరిగి crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
libwasmedge_exec (శూన్య *cookie, __attribute__ ((వాడని)) libcrun_container_t *container, స్థిరరాశి అక్షరం *pathname, అక్షరం *స్థిరరాశి argv[])
{
  Wasm_అంచు_Configure_Context *(*Wasm_అంచు_Configure_Create) (శూన్య);
  శూన్య (*Wasm_అంచు_Configure_Delete) (Wasm_అంచు_Configure_Context *Cxt);
  శూన్య (*Wasm_అంచు_Configure_కూడు_Proposal) (Wasm_అంచు_Configure_Context *Cxt, స్థిరరాశి గణన Wasm_అంచు_Proposal Prop);
  శూన్య (*Wasm_అంచు_Configure_కూడు_Host_Registration) (Wasm_అంచు_Configure_Context *Cxt, గణన Wasm_అంచు_Host_Registration Host);
  Wasm_అంచు_VMContext *(*Wasm_అంచు_VMCreate) (స్థిరరాశి Wasm_అంచు_Configure_Context *ConfCxt, Wasm_అంచు_Store_Context *StoreCxt);
  శూన్య (*Wasm_అంచు_VMDelete) (Wasm_అంచు_VMContext *Cxt);
  Wasm_అంచు_ఫలితం (*Wasm_అంచు_VMRegister_Module_From_File) (Wasm_అంచు_VMContext *Cxt, Wasm_అంచు_String ModuleName, స్థిరరాశి అక్షరం *మార్గం_2);
  Wasm_అంచు_ఫలితం (*Wasm_అంచు_VMRun_Wasm_From_File) (Wasm_అంచు_VMContext *Cxt, స్థిరరాశి అక్షరం *మార్గం_2, స్థిరరాశి Wasm_అంచు_String FuncName, స్థిరరాశి Wasm_అంచు_విలువ *Params, స్థిరరాశి uint32_t ParamLen, Wasm_అంచు_విలువ *Returns, స్థిరరాశి uint32_t ReturnLen);
  శూన్య (*Wasm_అంచు_Plugin_ఎక్కించు_From_మార్గం) (స్థిరరాశి అక్షరం *మార్గం_2);
  శూన్య (*Wasm_అంచు_Plugin_Init_WASINN) (స్థిరరాశి అక్షరం *స్థిరరాశి *NNPreloads, స్థిరరాశి uint32_t PreloadsLen);
  bool (*Wasm_అంచు_ఫలితం_OK) (స్థిరరాశి Wasm_అంచు_ఫలితం Res);
  Wasm_అంచు_String (*Wasm_అంచు_String_Create_By_CString) (స్థిరరాశి అక్షరం *Str);
  uint32_t argn = 0;
  uint32_t envn = 0;
  స్థిరరాశి అక్షరం *dirs[2] = { "/:/", ".:." };
  Wasm_అంచు_Configure_Context *configure;
  Wasm_అంచు_VMContext *vm;
  Wasm_అంచు_ఫలితం ఫలితం;

  Wasm_అంచు_Module_Instance_Context *wasi_module;
  Wasm_అంచు_Module_Instance_Context *(*Wasm_అంచు_VMGet_Import_Module_Context) (Wasm_అంచు_VMContext *Cxt, స్థిరరాశి గణన Wasm_అంచు_Host_Registration Reg);
  శూన్య (*Wasm_అంచు_Module_Instance_Init_WASI) (Wasm_అంచు_Module_Instance_Context *Cxt, స్థిరరాశి అక్షరం *స్థిరరాశి *వాదనలు_2, స్థిరరాశి uint32_t ArgLen, స్థిరరాశి అక్షరం *స్థిరరాశి *Envs, స్థిరరాశి uint32_t EnvLen, స్థిరరాశి అక్షరం *స్థిరరాశి *Dirs, స్థిరరాశి uint32_t DirLen, స్థిరరాశి అక్షరం *స్థిరరాశి *Preopens, స్థిరరాశి uint32_t PreopenLen);
  Wasm_అంచు_Module_Instance_Init_WASI = dlsym (cookie, "WasmEdge_ModuleInstanceInitWASI");

  Wasm_అంచు_Configure_Create = dlsym (cookie, "WasmEdge_ConfigureCreate");
  Wasm_అంచు_Configure_Delete = dlsym (cookie, "WasmEdge_ConfigureDelete");
  Wasm_అంచు_Configure_కూడు_Proposal = dlsym (cookie, "WasmEdge_ConfigureAddProposal");
  Wasm_అంచు_Configure_కూడు_Host_Registration = dlsym (cookie, "WasmEdge_ConfigureAddHostRegistration");
  Wasm_అంచు_VMCreate = dlsym (cookie, "WasmEdge_VMCreate");
  Wasm_అంచు_VMDelete = dlsym (cookie, "WasmEdge_VMDelete");
  Wasm_అంచు_VMRegister_Module_From_File = dlsym (cookie, "WasmEdge_VMRegisterModuleFromFile");
  Wasm_అంచు_VMGet_Import_Module_Context = dlsym (cookie, "WasmEdge_VMGetImportModuleContext");
  Wasm_అంచు_VMRun_Wasm_From_File = dlsym (cookie, "WasmEdge_VMRunWasmFromFile");
  Wasm_అంచు_Plugin_ఎక్కించు_From_మార్గం = dlsym (cookie, "WasmEdge_PluginLoadFromPath");
  Wasm_అంచు_Plugin_Init_WASINN = dlsym (cookie, "WasmEdge_PluginInitWASINN");
  Wasm_అంచు_ఫలితం_OK = dlsym (cookie, "WasmEdge_ResultOK");
  Wasm_అంచు_String_Create_By_CString = dlsym (cookie, "WasmEdge_StringCreateByCString");

  ఒకవేళ (Wasm_అంచు_Configure_Create == ఖాళీ || Wasm_అంచు_Configure_Delete == ఖాళీ || Wasm_అంచు_Configure_కూడు_Proposal == ఖాళీ
      || Wasm_అంచు_Configure_కూడు_Host_Registration == ఖాళీ || Wasm_అంచు_VMCreate == ఖాళీ || Wasm_అంచు_VMDelete == ఖాళీ
      || Wasm_అంచు_VMRegister_Module_From_File == ఖాళీ || Wasm_అంచు_VMGet_Import_Module_Context == ఖాళీ
      || Wasm_అంచు_Module_Instance_Init_WASI == ఖాళీ || Wasm_అంచు_VMRun_Wasm_From_File == ఖాళీ
      || Wasm_అంచు_ఫలితం_OK == ఖాళీ || Wasm_అంచు_String_Create_By_CString == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmedge.so.0`");

  configure = Wasm_అంచు_Configure_Create ();
  ఒకవేళ (UNLIKELY (configure == ఖాళీ))
    error (EXIT_FAILURE, 0, "could not create wasmedge configure");

  Wasm_అంచు_Configure_కూడు_Proposal (configure, Wasm_అంచు_Proposal_Bulk_జ్ఞాపకం_Operations);
  Wasm_అంచు_Configure_కూడు_Proposal (configure, Wasm_అంచు_Proposal_Reference_Types);
  Wasm_అంచు_Configure_కూడు_Proposal (configure, Wasm_అంచు_Proposal_SIMD);
  Wasm_అంచు_Configure_కూడు_Host_Registration (configure, Wasm_అంచు_Host_Registration_Wasi);
  // Check if the necessary environment variables are set
  స్థిరరాశి అక్షరం *plugin_మార్గం_env = getenv ("WASMEDGE_PLUGIN_PATH");
  ఒకవేళ (plugin_మార్గం_env != ఖాళీ)
    Wasm_అంచు_Plugin_ఎక్కించు_From_మార్గం (plugin_మార్గం_env);

  స్థిరరాశి అక్షరం *nnpreload_env = getenv ("WASMEDGE_WASINN_PRELOAD");
  ఒకవేళ (nnpreload_env != ఖాళీ)
    Wasm_అంచు_Plugin_Init_WASINN (&nnpreload_env, 1);

  vm = Wasm_అంచు_VMCreate (configure, ఖాళీ);
  ఒకవేళ (UNLIKELY (vm == ఖాళీ))
    {
      Wasm_అంచు_Configure_Delete (configure);
      error (EXIT_FAILURE, 0, "could not create wasmedge vm");
    }

  wasi_module = Wasm_అంచు_VMGet_Import_Module_Context (vm, Wasm_అంచు_Host_Registration_Wasi);
  ఒకవేళ (UNLIKELY (wasi_module == ఖాళీ))
    {
      Wasm_అంచు_VMDelete (vm);
      Wasm_అంచు_Configure_Delete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge wasi module context");
    }

  కొరకు (అక్షరం *స్థిరరాశి *arg = argv; *arg != ఖాళీ; ++arg, ++argn)
    ;
  బాహ్య అక్షరం **environ;
  కొరకు (అక్షరం *స్థిరరాశి *env = environ; *env != ఖాళీ; ++env, ++envn)
    ;

  Wasm_అంచు_Module_Instance_Init_WASI (wasi_module, (స్థిరరాశి అక్షరం *స్థిరరాశి *) &argv[0], argn, (స్థిరరాశి అక్షరం *స్థిరరాశి *) &environ[0], envn, dirs, 1, ఖాళీ, 0);

  ఫలితం = Wasm_అంచు_VMRun_Wasm_From_File (vm, pathname, Wasm_అంచు_String_Create_By_CString ("_start"), ఖాళీ, 0, ఖాళీ, 0);

  ఒకవేళ (UNLIKELY (! Wasm_అంచు_ఫలితం_OK (ఫలితం)))
    {
      Wasm_అంచు_VMDelete (vm);
      Wasm_అంచు_Configure_Delete (configure);
      error (EXIT_FAILURE, 0, "could not get wasmedge result from VM");
    }

  Wasm_అంచు_VMDelete (vm);
  Wasm_అంచు_Configure_Delete (configure);
  నిష్క్రమించు (EXIT_SUCCESS);
}

స్థిర పూర్ణాంకం
wasmedge_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  తిరిగి wasm_can_handle_container (container, err);
}

// This works only when the plugin is present in /usr/lib/wasmedge
స్థిర పూర్ణాంకం
libwasmedge_configure_container (శూన్య *cookie arg_వాడని, గణన handler_configure_phase phase,
                                 libcrun_context_t *context arg_వాడని, libcrun_container_t *container,
                                 స్థిరరాశి అక్షరం *rootfs arg_వాడని, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  అక్షరం **container_env = def->process->env;
  bool has_plugin_మార్గం = false, has_preload = false;

  కొరకు (అక్షరం **env = container_env; env && *env; env++)
    {
      ఒకవేళ (అక్షరశ్రేణి_పోలిక (*env, "WASMEDGE_PLUGIN_PATH=", 21) == 0)
        has_plugin_మార్గం = true;
      లేకపోతే ఒకవేళ (అక్షరశ్రేణి_పోలిక (*env, "WASMEDGE_WASINN_PRELOAD=", 24) == 0)
        has_preload = true;
    }

  ఒకవేళ (! has_plugin_మార్గం && ! has_preload)
    తిరిగి 0;

  ఒకవేళ (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    తిరిగి 0;

  // Check if /usr/lib/wasmedge is already present in spec
  ఒకవేళ (def->linux && def->mounts)
    {
      కొరకు (size_t i = 0; i < def->mounts_len; i++)
        {
          ఒకవేళ (strcmp (def->mounts[i]->destination, "/usr/lib/wasmedge") == 0)
            తిరిగి 0;
        }
    }

  // Mount the plugin folder to /usr/lib/wasmedge with specific options
  అక్షరం *options[] = {
    "ro",
    "rprivate",
    "nosuid",
    "nodev",
    "rbind"
  };

  ret = libcrun_container_do_bind_mount (container, "/usr/lib/wasmedge ", "/usr/lib/wasmedge", options, 5, err);
  ఒకవేళ (ret < 0)
    {
      ఒకవేళ (crun_error_get_errno (err) != ENOENT)
        తిరిగి ret;
      crun_error_release (err);
    }

  తిరిగి 0;
}

నిర్మాణం custom_handler_s handler_wasmedge = {
  .name = "wasmedge",
  .alias = "wasm",
  .feature_string = "WASM:wasmedge",
  .supports_open_వృక్షం_namespace = false,
  .ఎక్కించు = libwasmedge_ఎక్కించు,
  .unload = libwasmedge_unload,
  .నిరంతరం_func = libwasmedge_exec,
  .can_handle_container = wasmedge_can_handle_container,
  .configure_container = libwasmedge_configure_container,
};

#అయితే_ముగింపు
