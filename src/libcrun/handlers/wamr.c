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
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>

#నిర్వచితమైతే HAVE_DLOPEN
#  include <dlfcn.h>
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_WAMR
#  include <wasm_export.h>
#అయితే_ముగింపు

#ఒకవేళ HAVE_DLOPEN && HAVE_WAMR

స్థిర పూర్ణాంకం
libwamr_ఎక్కించు (శూన్య **cookie, libcrun_error_t *err)
{
  శూన్య *handle;

  handle = dlopen ("libiwasm.so", RTLD_NOW);
  ఒకవేళ (handle == ఖాళీ)
    తిరిగి crun_make_error (err, 0, "could not load `libiwasm.so`: `%s`", dlerror ());
  *cookie = handle;

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
libwamr_unload (శూన్య *cookie, libcrun_error_t *err)
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
libwamr_exec (శూన్య *cookie, __attribute__ ((వాడని)) libcrun_container_t *container, స్థిరరాశి అక్షరం *pathname, అక్షరం *స్థిరరాశి argv[])
{
  // load symbols from the shared library libiwasm.so
  bool (*wasm_runtime_init) ();
  Runtime_Init_వాదనలు init_వాదనలు;
  bool (*wasm_runtime_నిండిన_init) (Runtime_Init_వాదనలు *init_వాదనలు);
  wasm_module_t module;
  wasm_module_t (*wasm_runtime_ఎక్కించు) (uint8_t *buf, uint32_t కొలత, అక్షరం *error_buf, uint32_t error_buf_కొలత);
  wasm_module_inst_t module_inst;
  wasm_module_inst_t (*wasm_runtime_instantiate) (స్థిరరాశి wasm_module_t module, uint32_t default_దొంతర_కొలత, uint32_t host_managed_కుప్ప_కొలత, అక్షరం *error_buf, uint32_t error_buf_కొలత);
  wasm_function_inst_t func;
  wasm_function_inst_t (*wasm_runtime_lookup_function) (wasm_module_inst_t స్థిరరాశి module_inst, స్థిరరాశి అక్షరం *name);
  wasm_exec_env_t exec_env;
  wasm_exec_env_t (*wasm_runtime_create_exec_env) (wasm_module_inst_t module_inst, uint32_t దొంతర_కొలత);
  bool (*wasm_runtime_call_wasm) (wasm_exec_env_t exec_env, wasm_function_inst_t function, uint32_t argc, uint32_t argv[]);
  స్థిరరాశి అక్షరం *(*wasm_runtime_get_exception) (wasm_module_inst_t module_inst);
  శూన్య (*wasm_runtime_సమితి_exception) (wasm_module_inst_t module_inst, స్థిరరాశి అక్షరం *exception);
  శూన్య (*wasm_runtime_clear_exception) (wasm_module_inst_t module_inst);
  శూన్య (*wasm_runtime_destroy_exec_env) (wasm_exec_env_t exec_env);
  శూన్య (*wasm_runtime_deinstantiate) (wasm_module_inst_t module_inst);
  శూన్య (*wasm_runtime_unload) (wasm_module_t module);
  శూన్య (*wasm_runtime_destroy) ();
  uint32_t (*wasm_runtime_get_wasi_exit_సంకేతం) (wasm_module_inst_t module_inst);
  bool (*wasm_application_execute_main) (wasm_module_inst_t module_inst, int32_t argc, అక్షరం *argv[]);
  శూన్య (*wasm_runtime_సమితి_wasi_వాదనలు) (wasm_module_t module, స్థిరరాశి అక్షరం *dir_list[], uint32_t dir_లెక్క, స్థిరరాశి అక్షరం *map_dir_list[], uint32_t map_dir_లెక్క, స్థిరరాశి అక్షరం *env[], uint32_t env_లెక్క, అక్షరం *argv[], పూర్ణాంకం argc);
  శూన్య (*wasm_runtime_సమితి_wasi_addr_pool) (wasm_module_t module, స్థిరరాశి అక్షరం *addr_pool[], uint32_t pool_కొలత);
  శూన్య (*wasm_runtime_సమితి_wasi_ns_lookup_pool) (wasm_module_t module, స్థిరరాశి అక్షరం *ns_lookup_pool[], uint32_t pool_కొలత);

  wasm_runtime_init = dlsym (cookie, "wasm_runtime_init");
  wasm_runtime_నిండిన_init = dlsym (cookie, "wasm_runtime_full_init");
  wasm_runtime_ఎక్కించు = dlsym (cookie, "wasm_runtime_load");
  wasm_runtime_instantiate = dlsym (cookie, "wasm_runtime_instantiate");
  wasm_runtime_lookup_function = dlsym (cookie, "wasm_runtime_lookup_function");
  wasm_runtime_create_exec_env = dlsym (cookie, "wasm_runtime_create_exec_env");
  wasm_runtime_call_wasm = dlsym (cookie, "wasm_runtime_call_wasm");
  wasm_runtime_get_exception = dlsym (cookie, "wasm_runtime_get_exception");
  wasm_runtime_సమితి_exception = dlsym (cookie, "wasm_runtime_set_exception");
  wasm_runtime_clear_exception = dlsym (cookie, "wasm_runtime_clear_exception");
  wasm_runtime_destroy_exec_env = dlsym (cookie, "wasm_runtime_destroy_exec_env");
  wasm_runtime_deinstantiate = dlsym (cookie, "wasm_runtime_deinstantiate");
  wasm_runtime_unload = dlsym (cookie, "wasm_runtime_unload");
  wasm_runtime_destroy = dlsym (cookie, "wasm_runtime_destroy");
  wasm_runtime_get_wasi_exit_సంకేతం = dlsym (cookie, "wasm_runtime_get_wasi_exit_code");
  wasm_application_execute_main = dlsym (cookie, "wasm_application_execute_main");
  wasm_runtime_సమితి_wasi_వాదనలు = dlsym (cookie, "wasm_runtime_set_wasi_args");
  wasm_runtime_సమితి_wasi_addr_pool = dlsym (cookie, "wasm_runtime_set_wasi_addr_pool");
  wasm_runtime_సమితి_wasi_ns_lookup_pool = dlsym (cookie, "wasm_runtime_set_wasi_ns_lookup_pool");

  ఒకవేళ (wasm_runtime_init == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_init symbol in `libiwasm.so`");
  ఒకవేళ (wasm_runtime_నిండిన_init == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_full_init symbol in `libiwasm.so`");
  ఒకవేళ (wasm_runtime_ఎక్కించు == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_load symbol in `libiwasm.so`");
  ఒకవేళ (wasm_runtime_instantiate == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_instantiate symbol in `libiwasm.so`");
  ఒకవేళ (wasm_runtime_lookup_function == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_lookup_function symbol in `libiwasm.so`");
  ఒకవేళ (wasm_runtime_create_exec_env == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_create_exec_env symbol in `libiwasm.so`");
  ఒకవేళ (wasm_runtime_call_wasm == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_call_wasm symbol in `libiwasm.so`");
  ఒకవేళ (wasm_runtime_get_exception == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_exception symbol in `libiwasm.so`");
  ఒకవేళ (wasm_runtime_సమితి_exception == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_exception symbol in `libiwasm.so`");
  ఒకవేళ (wasm_runtime_clear_exception == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_clear_exception symbol in `libiwasm.so`");
  ఒకవేళ (wasm_runtime_destroy_exec_env == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy_exec_env symbol in `libiwasm.so`");
  ఒకవేళ (wasm_runtime_deinstantiate == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_deinstantiate symbol in `libiwasm.so`");
  ఒకవేళ (wasm_runtime_unload == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_unload symbol in `libiwasm.so`");
  ఒకవేళ (wasm_runtime_destroy == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy symbol in `libiwasm.so`");
  ఒకవేళ (wasm_runtime_get_wasi_exit_సంకేతం == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_wasi_exit_code symbol in `libiwasm.so`");
  ఒకవేళ (wasm_application_execute_main == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find wasm_application_execute_main symbol in `libiwasm.so`");
  ఒకవేళ (wasm_runtime_సమితి_wasi_వాదనలు == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_args symbol in `libiwasm.so`");
  ఒకవేళ (wasm_runtime_సమితి_wasi_addr_pool == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_addr_pool symbol in `libiwasm.so`");
  ఒకవేళ (wasm_runtime_సమితి_wasi_ns_lookup_pool == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_ns_lookup_pool symbol in `libiwasm.so`");

  పూర్ణాంకం ret;
  స్థిరరాశి అక్షరం *exception;
  cleanup_free అక్షరం *బఫరు = ఖాళీ;
  అక్షరం error_buf[128];
  size_t బఫరు_కొలత;
  uint32_t దొంతర_కొలత = 8096, కుప్ప_కొలత = 0;
  libcrun_error_t tmp_err = ఖాళీ;
  స్థిరరాశి అక్షరం *wasi_proc_exit_exception = "wasi proc exit";
  స్థిరరాశి అక్షరం *wasi_addr_pool[2] = { "0.0.0.0/0", "::/0" };
  స్థిరరాశి అక్షరం *wasi_ns_lookup_pool[1] = { "*" };

  స్థిరరాశి అక్షరం *dirs[2] = { "/", "." };
  అక్షరం **container_env = container->container_def->process->env;
  size_t env_లెక్క = container->container_def->process->env_len;

  పూర్ణాంకం arg_లెక్క = 0;
  అక్షరం *స్థిరరాశి *arg;
  కొరకు (arg = argv; *arg != ఖాళీ; ++arg)
    arg_లెక్క++;

  // initialize the wasm runtime by default configurations
  ఒకవేళ (! wasm_runtime_init ())
    error (EXIT_FAILURE, 0, "Failed to initialize the wasm runtime");

  // read WASM file into a memory buffer
  ret = read_all_file (pathname, &బఫరు, &బఫరు_కొలత, &tmp_err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      error (EXIT_FAILURE, 0, "Failed to read file");
    }

  ఒకవేళ (UNLIKELY (బఫరు_కొలత > UINT32_MAX))
    error (EXIT_FAILURE, 0, "File size is too large");

  // parse the WASM file from buffer and create a WASM module
  module = wasm_runtime_ఎక్కించు (బఫరు, బఫరు_కొలత, error_buf, పరిమాణం (error_buf));
  ఒకవేళ (! module)
    error (EXIT_FAILURE, 0, "Failed to load WASM file");

  // instantiate the WASI environment
  wasm_runtime_సమితి_wasi_వాదనలు (module, dirs, 1, ఖాళీ, 0, (స్థిరరాశి అక్షరం **) container_env, env_లెక్క, (అక్షరం **) argv, arg_లెక్క);

  // enable the WASI socket api
  wasm_runtime_సమితి_wasi_addr_pool (module, wasi_addr_pool, 2);
  wasm_runtime_సమితి_wasi_ns_lookup_pool (module, wasi_ns_lookup_pool, 1);

  // create an instance of the WASM module (WASM linear memory is ready)
  module_inst = wasm_runtime_instantiate (module, దొంతర_కొలత, కుప్ప_కొలత, error_buf, పరిమాణం (error_buf));
  ఒకవేళ (! module_inst)
    error (EXIT_FAILURE, 0, "Failed to instantiate the WASM module");

  // look up a WASM function by its name (The function signature can NULL here)
  func = wasm_runtime_lookup_function (module_inst, "_start");
  ఒకవేళ (! func)
    error (EXIT_FAILURE, 0, "Failed to look up the WASM function");

  // create an execution environment to execute the WASM functions
  exec_env = wasm_runtime_create_exec_env (module_inst, దొంతర_కొలత);
  ఒకవేళ (! exec_env)
    error (EXIT_FAILURE, 0, "Failed to create the execution environment");

  // call the WASM function
  ret = wasm_runtime_call_wasm (exec_env, func, 0, ఖాళీ);
  ఒకవేళ (ret)
    wasm_runtime_సమితి_exception (module_inst, wasi_proc_exit_exception);
  exception = wasm_runtime_get_exception (module_inst);
  ఒకవేళ (! అక్షరశ్రేణి_శోధన (exception, wasi_proc_exit_exception))
    error (EXIT_FAILURE, 0, "Failed to call the WASM function");
  wasm_runtime_clear_exception (module_inst);

  wasm_runtime_destroy_exec_env (exec_env);
  wasm_runtime_deinstantiate (module_inst);
  wasm_runtime_unload (module);
  wasm_runtime_destroy ();

  నిష్క్రమించు (EXIT_SUCCESS);
}

స్థిర పూర్ణాంకం
libwamr_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  తిరిగి wasm_can_handle_container (container, err);
}

నిర్మాణం custom_handler_s handler_wamr = {
  .name = "wamr",
  .alias = "wasm",
  .feature_string = "WASM:wamr",
  .supports_open_వృక్షం_namespace = false,
  .ఎక్కించు = libwamr_ఎక్కించు,
  .unload = libwamr_unload,
  .నిరంతరం_func = libwamr_exec,
  .can_handle_container = libwamr_can_handle_container,
};

#అయితే_ముగింపు
