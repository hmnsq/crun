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
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WAMR
#  include <wasm_export.h>
#endif

#যদি HAVE_DLOPEN && HAVE_WAMR

স্থির পূর্ণসংখ্যা
libwamr_load (শূন্য **cookie, libcrun_error_t *err)
{
  শূন্য *handle;

  handle = dlopen ("libiwasm.so", RTLD_NOW);
  যদি (handle == ফাঁকা)
    ফেরত crun_make_error (err, 0, "could not load `libiwasm.so`: `%s`", dlerror ());
  *cookie = handle;

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
libwamr_unload (শূন্য *cookie, libcrun_error_t *err)
{
  পূর্ণসংখ্যা r;

  যদি (cookie)
    {
      r = dlclose (cookie);
      যদি (UNLIKELY (r < 0))
        ফেরত crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
libwamr_exec (শূন্য *cookie, __attribute__ ((unused)) libcrun_container_t *container, ধ্রুবক অক্ষর *pathname, অক্ষর *ধ্রুবক argv[])
{
  // load symbols from the shared library libiwasm.so
  bool (*wasm_runtime_init) ();
  RuntimeInitArgs init_args;
  bool (*wasm_runtime_full_init) (RuntimeInitArgs *init_args);
  wasm_module_t module;
  wasm_module_t (*wasm_runtime_load) (uint8_t *buf, uint32_t আকার_2, অক্ষর *error_buf, uint32_t error_buf_আকার);
  wasm_module_inst_t module_inst;
  wasm_module_inst_t (*wasm_runtime_instantiate) (ধ্রুবক wasm_module_t module, uint32_t default_স্তূপ_আকার, uint32_t host_managed_heap_আকার, অক্ষর *error_buf, uint32_t error_buf_আকার);
  wasm_function_inst_t func;
  wasm_function_inst_t (*wasm_runtime_lookup_function) (wasm_module_inst_t ধ্রুবক module_inst, ধ্রুবক অক্ষর *name);
  wasm_exec_env_t exec_env;
  wasm_exec_env_t (*wasm_runtime_create_exec_env) (wasm_module_inst_t module_inst, uint32_t স্তূপ_আকার);
  bool (*wasm_runtime_call_wasm) (wasm_exec_env_t exec_env, wasm_function_inst_t function, uint32_t argc, uint32_t argv[]);
  ধ্রুবক অক্ষর *(*wasm_runtime_get_exception) (wasm_module_inst_t module_inst);
  শূন্য (*wasm_runtime_set_exception) (wasm_module_inst_t module_inst, ধ্রুবক অক্ষর *exception);
  শূন্য (*wasm_runtime_clear_exception) (wasm_module_inst_t module_inst);
  শূন্য (*wasm_runtime_destroy_exec_env) (wasm_exec_env_t exec_env);
  শূন্য (*wasm_runtime_deinstantiate) (wasm_module_inst_t module_inst);
  শূন্য (*wasm_runtime_unload) (wasm_module_t module);
  শূন্য (*wasm_runtime_destroy) ();
  uint32_t (*wasm_runtime_get_wasi_exit_code) (wasm_module_inst_t module_inst);
  bool (*wasm_application_execute_main) (wasm_module_inst_t module_inst, int32_t argc, অক্ষর *argv[]);
  শূন্য (*wasm_runtime_set_wasi_args) (wasm_module_t module, ধ্রুবক অক্ষর *dir_list[], uint32_t dir_গুনতি, ধ্রুবক অক্ষর *map_dir_list[], uint32_t map_dir_গুনতি, ধ্রুবক অক্ষর *env[], uint32_t env_গুনতি, অক্ষর *argv[], পূর্ণসংখ্যা argc);
  শূন্য (*wasm_runtime_set_wasi_addr_pool) (wasm_module_t module, ধ্রুবক অক্ষর *addr_pool[], uint32_t pool_আকার);
  শূন্য (*wasm_runtime_set_wasi_ns_lookup_pool) (wasm_module_t module, ধ্রুবক অক্ষর *ns_lookup_pool[], uint32_t pool_আকার);

  wasm_runtime_init = dlsym (cookie, "wasm_runtime_init");
  wasm_runtime_full_init = dlsym (cookie, "wasm_runtime_full_init");
  wasm_runtime_load = dlsym (cookie, "wasm_runtime_load");
  wasm_runtime_instantiate = dlsym (cookie, "wasm_runtime_instantiate");
  wasm_runtime_lookup_function = dlsym (cookie, "wasm_runtime_lookup_function");
  wasm_runtime_create_exec_env = dlsym (cookie, "wasm_runtime_create_exec_env");
  wasm_runtime_call_wasm = dlsym (cookie, "wasm_runtime_call_wasm");
  wasm_runtime_get_exception = dlsym (cookie, "wasm_runtime_get_exception");
  wasm_runtime_set_exception = dlsym (cookie, "wasm_runtime_set_exception");
  wasm_runtime_clear_exception = dlsym (cookie, "wasm_runtime_clear_exception");
  wasm_runtime_destroy_exec_env = dlsym (cookie, "wasm_runtime_destroy_exec_env");
  wasm_runtime_deinstantiate = dlsym (cookie, "wasm_runtime_deinstantiate");
  wasm_runtime_unload = dlsym (cookie, "wasm_runtime_unload");
  wasm_runtime_destroy = dlsym (cookie, "wasm_runtime_destroy");
  wasm_runtime_get_wasi_exit_code = dlsym (cookie, "wasm_runtime_get_wasi_exit_code");
  wasm_application_execute_main = dlsym (cookie, "wasm_application_execute_main");
  wasm_runtime_set_wasi_args = dlsym (cookie, "wasm_runtime_set_wasi_args");
  wasm_runtime_set_wasi_addr_pool = dlsym (cookie, "wasm_runtime_set_wasi_addr_pool");
  wasm_runtime_set_wasi_ns_lookup_pool = dlsym (cookie, "wasm_runtime_set_wasi_ns_lookup_pool");

  যদি (wasm_runtime_init == ফাঁকা)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_init symbol in `libiwasm.so`");
  যদি (wasm_runtime_full_init == ফাঁকা)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_full_init symbol in `libiwasm.so`");
  যদি (wasm_runtime_load == ফাঁকা)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_load symbol in `libiwasm.so`");
  যদি (wasm_runtime_instantiate == ফাঁকা)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_instantiate symbol in `libiwasm.so`");
  যদি (wasm_runtime_lookup_function == ফাঁকা)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_lookup_function symbol in `libiwasm.so`");
  যদি (wasm_runtime_create_exec_env == ফাঁকা)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_create_exec_env symbol in `libiwasm.so`");
  যদি (wasm_runtime_call_wasm == ফাঁকা)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_call_wasm symbol in `libiwasm.so`");
  যদি (wasm_runtime_get_exception == ফাঁকা)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_exception symbol in `libiwasm.so`");
  যদি (wasm_runtime_set_exception == ফাঁকা)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_exception symbol in `libiwasm.so`");
  যদি (wasm_runtime_clear_exception == ফাঁকা)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_clear_exception symbol in `libiwasm.so`");
  যদি (wasm_runtime_destroy_exec_env == ফাঁকা)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy_exec_env symbol in `libiwasm.so`");
  যদি (wasm_runtime_deinstantiate == ফাঁকা)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_deinstantiate symbol in `libiwasm.so`");
  যদি (wasm_runtime_unload == ফাঁকা)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_unload symbol in `libiwasm.so`");
  যদি (wasm_runtime_destroy == ফাঁকা)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy symbol in `libiwasm.so`");
  যদি (wasm_runtime_get_wasi_exit_code == ফাঁকা)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_wasi_exit_code symbol in `libiwasm.so`");
  যদি (wasm_application_execute_main == ফাঁকা)
    error (EXIT_FAILURE, 0, "could not find wasm_application_execute_main symbol in `libiwasm.so`");
  যদি (wasm_runtime_set_wasi_args == ফাঁকা)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_args symbol in `libiwasm.so`");
  যদি (wasm_runtime_set_wasi_addr_pool == ফাঁকা)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_addr_pool symbol in `libiwasm.so`");
  যদি (wasm_runtime_set_wasi_ns_lookup_pool == ফাঁকা)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_ns_lookup_pool symbol in `libiwasm.so`");

  পূর্ণসংখ্যা ret;
  ধ্রুবক অক্ষর *exception;
  cleanup_free অক্ষর *বাফার = ফাঁকা;
  অক্ষর error_buf[128];
  size_t বাফার_আকার;
  uint32_t স্তূপ_আকার = 8096, heap_আকার = 0;
  libcrun_error_t tmp_err = ফাঁকা;
  ধ্রুবক অক্ষর *wasi_proc_exit_exception = "wasi proc exit";
  ধ্রুবক অক্ষর *wasi_addr_pool[2] = { "0.0.0.0/0", "::/0" };
  ধ্রুবক অক্ষর *wasi_ns_lookup_pool[1] = { "*" };

  ধ্রুবক অক্ষর *dirs[2] = { "/", "." };
  অক্ষর **container_env = container->container_def->process->env;
  size_t env_গুনতি = container->container_def->process->env_len;

  পূর্ণসংখ্যা arg_গুনতি = 0;
  অক্ষর *ধ্রুবক *arg;
  জন্য (arg = argv; *arg != ফাঁকা; ++arg)
    arg_গুনতি++;

  // initialize the wasm runtime by default configurations
  যদি (! wasm_runtime_init ())
    error (EXIT_FAILURE, 0, "Failed to initialize the wasm runtime");

  // read WASM file into a memory buffer
  ret = read_all_file (pathname, &বাফার, &বাফার_আকার, &tmp_err);
  যদি (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      error (EXIT_FAILURE, 0, "Failed to read file");
    }

  যদি (UNLIKELY (বাফার_আকার > UINT32_MAX))
    error (EXIT_FAILURE, 0, "File size is too large");

  // parse the WASM file from buffer and create a WASM module
  module = wasm_runtime_load (বাফার, বাফার_আকার, error_buf, আকার (error_buf));
  যদি (! module)
    error (EXIT_FAILURE, 0, "Failed to load WASM file");

  // instantiate the WASI environment
  wasm_runtime_set_wasi_args (module, dirs, 1, ফাঁকা, 0, (ধ্রুবক অক্ষর **) container_env, env_গুনতি, (অক্ষর **) argv, arg_গুনতি);

  // enable the WASI socket api
  wasm_runtime_set_wasi_addr_pool (module, wasi_addr_pool, 2);
  wasm_runtime_set_wasi_ns_lookup_pool (module, wasi_ns_lookup_pool, 1);

  // create an instance of the WASM module (WASM linear memory is ready)
  module_inst = wasm_runtime_instantiate (module, স্তূপ_আকার, heap_আকার, error_buf, আকার (error_buf));
  যদি (! module_inst)
    error (EXIT_FAILURE, 0, "Failed to instantiate the WASM module");

  // look up a WASM function by its name (The function signature can NULL here)
  func = wasm_runtime_lookup_function (module_inst, "_start");
  যদি (! func)
    error (EXIT_FAILURE, 0, "Failed to look up the WASM function");

  // create an execution environment to execute the WASM functions
  exec_env = wasm_runtime_create_exec_env (module_inst, স্তূপ_আকার);
  যদি (! exec_env)
    error (EXIT_FAILURE, 0, "Failed to create the execution environment");

  // call the WASM function
  ret = wasm_runtime_call_wasm (exec_env, func, 0, ফাঁকা);
  যদি (ret)
    wasm_runtime_set_exception (module_inst, wasi_proc_exit_exception);
  exception = wasm_runtime_get_exception (module_inst);
  যদি (! strstr (exception, wasi_proc_exit_exception))
    error (EXIT_FAILURE, 0, "Failed to call the WASM function");
  wasm_runtime_clear_exception (module_inst);

  wasm_runtime_destroy_exec_env (exec_env);
  wasm_runtime_deinstantiate (module_inst);
  wasm_runtime_unload (module);
  wasm_runtime_destroy ();

  exit (EXIT_SUCCESS);
}

স্থির পূর্ণসংখ্যা
libwamr_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  ফেরত wasm_can_handle_container (container, err);
}

কাঠামো custom_handler_s handler_wamr = {
  .name = "wamr",
  .alias = "wasm",
  .feature_string = "WASM:wamr",
  .supports_open_বৃক্ষ_namespace = false,
  .load = libwamr_load,
  .unload = libwamr_unload,
  .run_func = libwamr_exec,
  .can_handle_container = libwamr_can_handle_container,
};

#endif
