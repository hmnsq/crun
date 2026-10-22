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
#شامل <stdio.h>
#شامل <stdlib.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WAMR
#  include <wasm_export.h>
#endif

#اگر HAVE_DLOPEN && HAVE_WAMR

ایستا صحیح
libwamr_load (تهی **cookie, libcrun_error_t *err)
{
  تهی *handle;

  handle = dlopen ("libiwasm.so", RTLD_NOW);
  اگر (handle == پوچ)
    بازگشت crun_make_error (err, 0, "could not load `libiwasm.so`: `%s`", dlerror ());
  *cookie = handle;

  بازگشت 0;
}

ایستا صحیح
libwamr_unload (تهی *cookie, libcrun_error_t *err)
{
  صحیح r;

  اگر (cookie)
    {
      r = dlclose (cookie);
      اگر (UNLIKELY (r < 0))
        بازگشت crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  بازگشت 0;
}

ایستا صحیح
libwamr_exec (تهی *cookie, __attribute__ ((unused)) libcrun_container_t *container, ثابت نویسه *pathname, نویسه *ثابت argv[])
{
  // load symbols from the shared library libiwasm.so
  bool (*wasm_runtime_init) ();
  RuntimeInitArgs init_args;
  bool (*wasm_runtime_full_init) (RuntimeInitArgs *init_args);
  wasm_module_t module;
  wasm_module_t (*wasm_runtime_load) (uint8_t *buf, uint32_t size, نویسه *error_buf, uint32_t error_buf_size);
  wasm_module_inst_t module_inst;
  wasm_module_inst_t (*wasm_runtime_instantiate) (ثابت wasm_module_t module, uint32_t default_پشته_size, uint32_t host_managed_heap_size, نویسه *error_buf, uint32_t error_buf_size);
  wasm_function_inst_t func;
  wasm_function_inst_t (*wasm_runtime_lookup_function) (wasm_module_inst_t ثابت module_inst, ثابت نویسه *name);
  wasm_exec_env_t exec_env;
  wasm_exec_env_t (*wasm_runtime_create_exec_env) (wasm_module_inst_t module_inst, uint32_t پشته_size);
  bool (*wasm_runtime_call_wasm) (wasm_exec_env_t exec_env, wasm_function_inst_t function, uint32_t argc, uint32_t argv[]);
  ثابت نویسه *(*wasm_runtime_get_exception) (wasm_module_inst_t module_inst);
  تهی (*wasm_runtime_set_exception) (wasm_module_inst_t module_inst, ثابت نویسه *exception);
  تهی (*wasm_runtime_clear_exception) (wasm_module_inst_t module_inst);
  تهی (*wasm_runtime_destroy_exec_env) (wasm_exec_env_t exec_env);
  تهی (*wasm_runtime_deinstantiate) (wasm_module_inst_t module_inst);
  تهی (*wasm_runtime_unload) (wasm_module_t module);
  تهی (*wasm_runtime_destroy) ();
  uint32_t (*wasm_runtime_get_wasi_exit_code) (wasm_module_inst_t module_inst);
  bool (*wasm_application_execute_main) (wasm_module_inst_t module_inst, int32_t argc, نویسه *argv[]);
  تهی (*wasm_runtime_set_wasi_args) (wasm_module_t module, ثابت نویسه *dir_list[], uint32_t dir_شمار, ثابت نویسه *map_dir_list[], uint32_t map_dir_شمار, ثابت نویسه *env[], uint32_t env_شمار, نویسه *argv[], صحیح argc);
  تهی (*wasm_runtime_set_wasi_addr_pool) (wasm_module_t module, ثابت نویسه *addr_pool[], uint32_t pool_size);
  تهی (*wasm_runtime_set_wasi_ns_lookup_pool) (wasm_module_t module, ثابت نویسه *ns_lookup_pool[], uint32_t pool_size);

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

  اگر (wasm_runtime_init == پوچ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_init symbol in `libiwasm.so`");
  اگر (wasm_runtime_full_init == پوچ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_full_init symbol in `libiwasm.so`");
  اگر (wasm_runtime_load == پوچ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_load symbol in `libiwasm.so`");
  اگر (wasm_runtime_instantiate == پوچ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_instantiate symbol in `libiwasm.so`");
  اگر (wasm_runtime_lookup_function == پوچ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_lookup_function symbol in `libiwasm.so`");
  اگر (wasm_runtime_create_exec_env == پوچ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_create_exec_env symbol in `libiwasm.so`");
  اگر (wasm_runtime_call_wasm == پوچ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_call_wasm symbol in `libiwasm.so`");
  اگر (wasm_runtime_get_exception == پوچ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_exception symbol in `libiwasm.so`");
  اگر (wasm_runtime_set_exception == پوچ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_exception symbol in `libiwasm.so`");
  اگر (wasm_runtime_clear_exception == پوچ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_clear_exception symbol in `libiwasm.so`");
  اگر (wasm_runtime_destroy_exec_env == پوچ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy_exec_env symbol in `libiwasm.so`");
  اگر (wasm_runtime_deinstantiate == پوچ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_deinstantiate symbol in `libiwasm.so`");
  اگر (wasm_runtime_unload == پوچ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_unload symbol in `libiwasm.so`");
  اگر (wasm_runtime_destroy == پوچ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy symbol in `libiwasm.so`");
  اگر (wasm_runtime_get_wasi_exit_code == پوچ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_wasi_exit_code symbol in `libiwasm.so`");
  اگر (wasm_application_execute_main == پوچ)
    error (EXIT_FAILURE, 0, "could not find wasm_application_execute_main symbol in `libiwasm.so`");
  اگر (wasm_runtime_set_wasi_args == پوچ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_args symbol in `libiwasm.so`");
  اگر (wasm_runtime_set_wasi_addr_pool == پوچ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_addr_pool symbol in `libiwasm.so`");
  اگر (wasm_runtime_set_wasi_ns_lookup_pool == پوچ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_ns_lookup_pool symbol in `libiwasm.so`");

  صحیح ret;
  ثابت نویسه *exception;
  cleanup_free نویسه *buffer = پوچ;
  نویسه error_buf[128];
  size_t buffer_size;
  uint32_t پشته_size = 8096, heap_size = 0;
  libcrun_error_t tmp_err = پوچ;
  ثابت نویسه *wasi_proc_exit_exception = "wasi proc exit";
  ثابت نویسه *wasi_addr_pool[2] = { "0.0.0.0/0", "::/0" };
  ثابت نویسه *wasi_ns_lookup_pool[1] = { "*" };

  ثابت نویسه *dirs[2] = { "/", "." };
  نویسه **container_env = container->container_def->process->env;
  size_t env_شمار = container->container_def->process->env_len;

  صحیح arg_شمار = 0;
  نویسه *ثابت *arg;
  برای (arg = argv; *arg != پوچ; ++arg)
    arg_شمار++;

  // initialize the wasm runtime by default configurations
  اگر (! wasm_runtime_init ())
    error (EXIT_FAILURE, 0, "Failed to initialize the wasm runtime");

  // read WASM file into a memory buffer
  ret = read_all_file (pathname, &buffer, &buffer_size, &tmp_err);
  اگر (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      error (EXIT_FAILURE, 0, "Failed to read file");
    }

  اگر (UNLIKELY (buffer_size > UINT32_MAX))
    error (EXIT_FAILURE, 0, "File size is too large");

  // parse the WASM file from buffer and create a WASM module
  module = wasm_runtime_load (buffer, buffer_size, error_buf, اندازه (error_buf));
  اگر (! module)
    error (EXIT_FAILURE, 0, "Failed to load WASM file");

  // instantiate the WASI environment
  wasm_runtime_set_wasi_args (module, dirs, 1, پوچ, 0, (ثابت نویسه **) container_env, env_شمار, (نویسه **) argv, arg_شمار);

  // enable the WASI socket api
  wasm_runtime_set_wasi_addr_pool (module, wasi_addr_pool, 2);
  wasm_runtime_set_wasi_ns_lookup_pool (module, wasi_ns_lookup_pool, 1);

  // create an instance of the WASM module (WASM linear memory is ready)
  module_inst = wasm_runtime_instantiate (module, پشته_size, heap_size, error_buf, اندازه (error_buf));
  اگر (! module_inst)
    error (EXIT_FAILURE, 0, "Failed to instantiate the WASM module");

  // look up a WASM function by its name (The function signature can NULL here)
  func = wasm_runtime_lookup_function (module_inst, "_start");
  اگر (! func)
    error (EXIT_FAILURE, 0, "Failed to look up the WASM function");

  // create an execution environment to execute the WASM functions
  exec_env = wasm_runtime_create_exec_env (module_inst, پشته_size);
  اگر (! exec_env)
    error (EXIT_FAILURE, 0, "Failed to create the execution environment");

  // call the WASM function
  ret = wasm_runtime_call_wasm (exec_env, func, 0, پوچ);
  اگر (ret)
    wasm_runtime_set_exception (module_inst, wasi_proc_exit_exception);
  exception = wasm_runtime_get_exception (module_inst);
  اگر (! strstr (exception, wasi_proc_exit_exception))
    error (EXIT_FAILURE, 0, "Failed to call the WASM function");
  wasm_runtime_clear_exception (module_inst);

  wasm_runtime_destroy_exec_env (exec_env);
  wasm_runtime_deinstantiate (module_inst);
  wasm_runtime_unload (module);
  wasm_runtime_destroy ();

  exit (EXIT_SUCCESS);
}

ایستا صحیح
libwamr_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  بازگشت wasm_can_handle_container (container, err);
}

ساختار custom_handler_s handler_wamr = {
  .name = "wamr",
  .alias = "wasm",
  .feature_string = "WASM:wamr",
  .supports_open_درخت_namespace = false,
  .load = libwamr_load,
  .unload = libwamr_unload,
  .run_func = libwamr_exec,
  .can_handle_container = libwamr_can_handle_container,
};

#endif
