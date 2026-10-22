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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "../custom-handler.h"
#அந்தர்பாவ "../container.h"
#அந்தர்பாவ "../utils.h"
#அந்தர்பாவ "../linux.h"
#அந்தர்பாவ "handler-utils.h"
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <sched.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WAMR
#  include <wasm_export.h>
#endif

#எனில் HAVE_DLOPEN && HAVE_WAMR

நிலையான முழு
libwamr_load (வெறுமை **cookie, libcrun_error_t *err)
{
  வெறுமை *handle;

  handle = dlopen ("libiwasm.so", RTLD_NOW);
  எனில் (handle == வெற்று)
    திரும்பு crun_make_error (err, 0, "could not load `libiwasm.so`: `%s`", dlerror ());
  *cookie = handle;

  திரும்பு 0;
}

நிலையான முழு
libwamr_unload (வெறுமை *cookie, libcrun_error_t *err)
{
  முழு r;

  எனில் (cookie)
    {
      r = dlclose (cookie);
      எனில் (UNLIKELY (r < 0))
        திரும்பு crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  திரும்பு 0;
}

நிலையான முழு
libwamr_exec (வெறுமை *cookie, __attribute__ ((unused)) libcrun_container_t *container, மாறிலி எழுத்து *pathname, எழுத்து *மாறிலி argv[])
{
  // load symbols from the shared library libiwasm.so
  bool (*wasm_runtime_init) ();
  RuntimeInitArgs init_args;
  bool (*wasm_runtime_full_init) (RuntimeInitArgs *init_args);
  wasm_module_t module;
  wasm_module_t (*wasm_runtime_load) (uint8_t *buf, uint32_t size, எழுத்து *error_buf, uint32_t error_buf_size);
  wasm_module_inst_t module_inst;
  wasm_module_inst_t (*wasm_runtime_instantiate) (மாறிலி wasm_module_t module, uint32_t default_அடுக்கு_size, uint32_t host_managed_heap_size, எழுத்து *error_buf, uint32_t error_buf_size);
  wasm_function_inst_t func;
  wasm_function_inst_t (*wasm_runtime_lookup_function) (wasm_module_inst_t மாறிலி module_inst, மாறிலி எழுத்து *name);
  wasm_exec_env_t exec_env;
  wasm_exec_env_t (*wasm_runtime_create_exec_env) (wasm_module_inst_t module_inst, uint32_t அடுக்கு_size);
  bool (*wasm_runtime_call_wasm) (wasm_exec_env_t exec_env, wasm_function_inst_t function, uint32_t argc, uint32_t argv[]);
  மாறிலி எழுத்து *(*wasm_runtime_get_exception) (wasm_module_inst_t module_inst);
  வெறுமை (*wasm_runtime_set_exception) (wasm_module_inst_t module_inst, மாறிலி எழுத்து *exception);
  வெறுமை (*wasm_runtime_clear_exception) (wasm_module_inst_t module_inst);
  வெறுமை (*wasm_runtime_destroy_exec_env) (wasm_exec_env_t exec_env);
  வெறுமை (*wasm_runtime_deinstantiate) (wasm_module_inst_t module_inst);
  வெறுமை (*wasm_runtime_unload) (wasm_module_t module);
  வெறுமை (*wasm_runtime_destroy) ();
  uint32_t (*wasm_runtime_get_wasi_exit_code) (wasm_module_inst_t module_inst);
  bool (*wasm_application_execute_main) (wasm_module_inst_t module_inst, int32_t argc, எழுத்து *argv[]);
  வெறுமை (*wasm_runtime_set_wasi_args) (wasm_module_t module, மாறிலி எழுத்து *dir_list[], uint32_t dir_மொத்தம், மாறிலி எழுத்து *map_dir_list[], uint32_t map_dir_மொத்தம், மாறிலி எழுத்து *env[], uint32_t env_மொத்தம், எழுத்து *argv[], முழு argc);
  வெறுமை (*wasm_runtime_set_wasi_addr_pool) (wasm_module_t module, மாறிலி எழுத்து *addr_pool[], uint32_t pool_size);
  வெறுமை (*wasm_runtime_set_wasi_ns_lookup_pool) (wasm_module_t module, மாறிலி எழுத்து *ns_lookup_pool[], uint32_t pool_size);

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

  எனில் (wasm_runtime_init == வெற்று)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_init symbol in `libiwasm.so`");
  எனில் (wasm_runtime_full_init == வெற்று)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_full_init symbol in `libiwasm.so`");
  எனில் (wasm_runtime_load == வெற்று)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_load symbol in `libiwasm.so`");
  எனில் (wasm_runtime_instantiate == வெற்று)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_instantiate symbol in `libiwasm.so`");
  எனில் (wasm_runtime_lookup_function == வெற்று)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_lookup_function symbol in `libiwasm.so`");
  எனில் (wasm_runtime_create_exec_env == வெற்று)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_create_exec_env symbol in `libiwasm.so`");
  எனில் (wasm_runtime_call_wasm == வெற்று)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_call_wasm symbol in `libiwasm.so`");
  எனில் (wasm_runtime_get_exception == வெற்று)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_exception symbol in `libiwasm.so`");
  எனில் (wasm_runtime_set_exception == வெற்று)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_exception symbol in `libiwasm.so`");
  எனில் (wasm_runtime_clear_exception == வெற்று)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_clear_exception symbol in `libiwasm.so`");
  எனில் (wasm_runtime_destroy_exec_env == வெற்று)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy_exec_env symbol in `libiwasm.so`");
  எனில் (wasm_runtime_deinstantiate == வெற்று)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_deinstantiate symbol in `libiwasm.so`");
  எனில் (wasm_runtime_unload == வெற்று)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_unload symbol in `libiwasm.so`");
  எனில் (wasm_runtime_destroy == வெற்று)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy symbol in `libiwasm.so`");
  எனில் (wasm_runtime_get_wasi_exit_code == வெற்று)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_wasi_exit_code symbol in `libiwasm.so`");
  எனில் (wasm_application_execute_main == வெற்று)
    error (EXIT_FAILURE, 0, "could not find wasm_application_execute_main symbol in `libiwasm.so`");
  எனில் (wasm_runtime_set_wasi_args == வெற்று)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_args symbol in `libiwasm.so`");
  எனில் (wasm_runtime_set_wasi_addr_pool == வெற்று)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_addr_pool symbol in `libiwasm.so`");
  எனில் (wasm_runtime_set_wasi_ns_lookup_pool == வெற்று)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_ns_lookup_pool symbol in `libiwasm.so`");

  முழு ret;
  மாறிலி எழுத்து *exception;
  cleanup_free எழுத்து *buffer = வெற்று;
  எழுத்து error_buf[128];
  size_t buffer_size;
  uint32_t அடுக்கு_size = 8096, heap_size = 0;
  libcrun_error_t tmp_err = வெற்று;
  மாறிலி எழுத்து *wasi_proc_exit_exception = "wasi proc exit";
  மாறிலி எழுத்து *wasi_addr_pool[2] = { "0.0.0.0/0", "::/0" };
  மாறிலி எழுத்து *wasi_ns_lookup_pool[1] = { "*" };

  மாறிலி எழுத்து *dirs[2] = { "/", "." };
  எழுத்து **container_env = container->container_def->process->env;
  size_t env_மொத்தம் = container->container_def->process->env_len;

  முழு arg_மொத்தம் = 0;
  எழுத்து *மாறிலி *arg;
  க்கு (arg = argv; *arg != வெற்று; ++arg)
    arg_மொத்தம்++;

  // initialize the wasm runtime by default configurations
  எனில் (! wasm_runtime_init ())
    error (EXIT_FAILURE, 0, "Failed to initialize the wasm runtime");

  // read WASM file into a memory buffer
  ret = read_all_file (pathname, &buffer, &buffer_size, &tmp_err);
  எனில் (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      error (EXIT_FAILURE, 0, "Failed to read file");
    }

  எனில் (UNLIKELY (buffer_size > UINT32_MAX))
    error (EXIT_FAILURE, 0, "File size is too large");

  // parse the WASM file from buffer and create a WASM module
  module = wasm_runtime_load (buffer, buffer_size, error_buf, அளவு (error_buf));
  எனில் (! module)
    error (EXIT_FAILURE, 0, "Failed to load WASM file");

  // instantiate the WASI environment
  wasm_runtime_set_wasi_args (module, dirs, 1, வெற்று, 0, (மாறிலி எழுத்து **) container_env, env_மொத்தம், (எழுத்து **) argv, arg_மொத்தம்);

  // enable the WASI socket api
  wasm_runtime_set_wasi_addr_pool (module, wasi_addr_pool, 2);
  wasm_runtime_set_wasi_ns_lookup_pool (module, wasi_ns_lookup_pool, 1);

  // create an instance of the WASM module (WASM linear memory is ready)
  module_inst = wasm_runtime_instantiate (module, அடுக்கு_size, heap_size, error_buf, அளவு (error_buf));
  எனில் (! module_inst)
    error (EXIT_FAILURE, 0, "Failed to instantiate the WASM module");

  // look up a WASM function by its name (The function signature can NULL here)
  func = wasm_runtime_lookup_function (module_inst, "_start");
  எனில் (! func)
    error (EXIT_FAILURE, 0, "Failed to look up the WASM function");

  // create an execution environment to execute the WASM functions
  exec_env = wasm_runtime_create_exec_env (module_inst, அடுக்கு_size);
  எனில் (! exec_env)
    error (EXIT_FAILURE, 0, "Failed to create the execution environment");

  // call the WASM function
  ret = wasm_runtime_call_wasm (exec_env, func, 0, வெற்று);
  எனில் (ret)
    wasm_runtime_set_exception (module_inst, wasi_proc_exit_exception);
  exception = wasm_runtime_get_exception (module_inst);
  எனில் (! strstr (exception, wasi_proc_exit_exception))
    error (EXIT_FAILURE, 0, "Failed to call the WASM function");
  wasm_runtime_clear_exception (module_inst);

  wasm_runtime_destroy_exec_env (exec_env);
  wasm_runtime_deinstantiate (module_inst);
  wasm_runtime_unload (module);
  wasm_runtime_destroy ();

  exit (EXIT_SUCCESS);
}

நிலையான முழு
libwamr_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  திரும்பு wasm_can_handle_container (container, err);
}

கட்டமைப்பு custom_handler_s handler_wamr = {
  .name = "wamr",
  .alias = "wasm",
  .feature_string = "WASM:wamr",
  .supports_open_மரம்_namespace = false,
  .load = libwamr_load,
  .unload = libwamr_unload,
  .run_func = libwamr_exec,
  .can_handle_container = libwamr_can_handle_container,
};

#endif
