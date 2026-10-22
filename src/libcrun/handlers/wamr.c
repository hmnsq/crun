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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ "../custom-handler.h"
#অন্তর্ভাৱ "../container.h"
#অন্তর্ভাৱ "../utils.h"
#অন্তর্ভাৱ "../linux.h"
#অন্তর্ভাৱ "handler-utils.h"
#অন্তর্ভাৱ <unistd.h>
#অন্তর্ভাৱ <sys/stat.h>
#অন্তর্ভাৱ <errno.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <sched.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <stdlib.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WAMR
#  include <wasm_export.h>
#endif

#যদি HAVE_DLOPEN && HAVE_WAMR

স্থির পূর্ণাঙ্ক
libwamr_load (শূন্য **cookie, libcrun_error_t *err)
{
  শূন্য *handle;

  handle = dlopen ("libiwasm.so", RTLD_NOW);
  যদি (handle == রিক্ত)
    ঘূৰাই crun_make_error (err, 0, "could not load `libiwasm.so`: `%s`", dlerror ());
  *cookie = handle;

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
libwamr_unload (শূন্য *cookie, libcrun_error_t *err)
{
  পূর্ণাঙ্ক r;

  যদি (cookie)
    {
      r = dlclose (cookie);
      যদি (UNLIKELY (r < 0))
        ঘূৰাই crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
libwamr_exec (শূন্য *cookie, __attribute__ ((unused)) libcrun_container_t *container, ধ্রুৱ ৱর্ণ *pathname, ৱর্ণ *ধ্রুৱ argv[])
{
  // load symbols from the shared library libiwasm.so
  bool (*wasm_runtime_init) ();
  RuntimeInitArgs init_args;
  bool (*wasm_runtime_full_init) (RuntimeInitArgs *init_args);
  wasm_module_t module;
  wasm_module_t (*wasm_runtime_load) (uint8_t *buf, uint32_t size, ৱর্ণ *error_buf, uint32_t error_buf_size);
  wasm_module_inst_t module_inst;
  wasm_module_inst_t (*wasm_runtime_instantiate) (ধ্রুৱ wasm_module_t module, uint32_t default_স্তূপ_size, uint32_t host_managed_heap_size, ৱর্ণ *error_buf, uint32_t error_buf_size);
  wasm_function_inst_t func;
  wasm_function_inst_t (*wasm_runtime_lookup_function) (wasm_module_inst_t ধ্রুৱ module_inst, ধ্রুৱ ৱর্ণ *name);
  wasm_exec_env_t exec_env;
  wasm_exec_env_t (*wasm_runtime_create_exec_env) (wasm_module_inst_t module_inst, uint32_t স্তূপ_size);
  bool (*wasm_runtime_call_wasm) (wasm_exec_env_t exec_env, wasm_function_inst_t function, uint32_t argc, uint32_t argv[]);
  ধ্রুৱ ৱর্ণ *(*wasm_runtime_get_exception) (wasm_module_inst_t module_inst);
  শূন্য (*wasm_runtime_set_exception) (wasm_module_inst_t module_inst, ধ্রুৱ ৱর্ণ *exception);
  শূন্য (*wasm_runtime_clear_exception) (wasm_module_inst_t module_inst);
  শূন্য (*wasm_runtime_destroy_exec_env) (wasm_exec_env_t exec_env);
  শূন্য (*wasm_runtime_deinstantiate) (wasm_module_inst_t module_inst);
  শূন্য (*wasm_runtime_unload) (wasm_module_t module);
  শূন্য (*wasm_runtime_destroy) ();
  uint32_t (*wasm_runtime_get_wasi_exit_code) (wasm_module_inst_t module_inst);
  bool (*wasm_application_execute_main) (wasm_module_inst_t module_inst, int32_t argc, ৱর্ণ *argv[]);
  শূন্য (*wasm_runtime_set_wasi_args) (wasm_module_t module, ধ্রুৱ ৱর্ণ *dir_list[], uint32_t dir_গুনতি, ধ্রুৱ ৱর্ণ *map_dir_list[], uint32_t map_dir_গুনতি, ধ্রুৱ ৱর্ণ *env[], uint32_t env_গুনতি, ৱর্ণ *argv[], পূর্ণাঙ্ক argc);
  শূন্য (*wasm_runtime_set_wasi_addr_pool) (wasm_module_t module, ধ্রুৱ ৱর্ণ *addr_pool[], uint32_t pool_size);
  শূন্য (*wasm_runtime_set_wasi_ns_lookup_pool) (wasm_module_t module, ধ্রুৱ ৱর্ণ *ns_lookup_pool[], uint32_t pool_size);

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

  যদি (wasm_runtime_init == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_init symbol in `libiwasm.so`");
  যদি (wasm_runtime_full_init == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_full_init symbol in `libiwasm.so`");
  যদি (wasm_runtime_load == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_load symbol in `libiwasm.so`");
  যদি (wasm_runtime_instantiate == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_instantiate symbol in `libiwasm.so`");
  যদি (wasm_runtime_lookup_function == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_lookup_function symbol in `libiwasm.so`");
  যদি (wasm_runtime_create_exec_env == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_create_exec_env symbol in `libiwasm.so`");
  যদি (wasm_runtime_call_wasm == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_call_wasm symbol in `libiwasm.so`");
  যদি (wasm_runtime_get_exception == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_exception symbol in `libiwasm.so`");
  যদি (wasm_runtime_set_exception == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_exception symbol in `libiwasm.so`");
  যদি (wasm_runtime_clear_exception == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_clear_exception symbol in `libiwasm.so`");
  যদি (wasm_runtime_destroy_exec_env == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy_exec_env symbol in `libiwasm.so`");
  যদি (wasm_runtime_deinstantiate == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_deinstantiate symbol in `libiwasm.so`");
  যদি (wasm_runtime_unload == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_unload symbol in `libiwasm.so`");
  যদি (wasm_runtime_destroy == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy symbol in `libiwasm.so`");
  যদি (wasm_runtime_get_wasi_exit_code == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_wasi_exit_code symbol in `libiwasm.so`");
  যদি (wasm_application_execute_main == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find wasm_application_execute_main symbol in `libiwasm.so`");
  যদি (wasm_runtime_set_wasi_args == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_args symbol in `libiwasm.so`");
  যদি (wasm_runtime_set_wasi_addr_pool == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_addr_pool symbol in `libiwasm.so`");
  যদি (wasm_runtime_set_wasi_ns_lookup_pool == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_ns_lookup_pool symbol in `libiwasm.so`");

  পূর্ণাঙ্ক ret;
  ধ্রুৱ ৱর্ণ *exception;
  cleanup_free ৱর্ণ *buffer = রিক্ত;
  ৱর্ণ error_buf[128];
  size_t buffer_size;
  uint32_t স্তূপ_size = 8096, heap_size = 0;
  libcrun_error_t tmp_err = রিক্ত;
  ধ্রুৱ ৱর্ণ *wasi_proc_exit_exception = "wasi proc exit";
  ধ্রুৱ ৱর্ণ *wasi_addr_pool[2] = { "0.0.0.0/0", "::/0" };
  ধ্রুৱ ৱর্ণ *wasi_ns_lookup_pool[1] = { "*" };

  ধ্রুৱ ৱর্ণ *dirs[2] = { "/", "." };
  ৱর্ণ **container_env = container->container_def->process->env;
  size_t env_গুনতি = container->container_def->process->env_len;

  পূর্ণাঙ্ক arg_গুনতি = 0;
  ৱর্ণ *ধ্রুৱ *arg;
  বাবে (arg = argv; *arg != রিক্ত; ++arg)
    arg_গুনতি++;

  // initialize the wasm runtime by default configurations
  যদি (! wasm_runtime_init ())
    error (EXIT_FAILURE, 0, "Failed to initialize the wasm runtime");

  // read WASM file into a memory buffer
  ret = read_all_file (pathname, &buffer, &buffer_size, &tmp_err);
  যদি (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      error (EXIT_FAILURE, 0, "Failed to read file");
    }

  যদি (UNLIKELY (buffer_size > UINT32_MAX))
    error (EXIT_FAILURE, 0, "File size is too large");

  // parse the WASM file from buffer and create a WASM module
  module = wasm_runtime_load (buffer, buffer_size, error_buf, পরিমাণ (error_buf));
  যদি (! module)
    error (EXIT_FAILURE, 0, "Failed to load WASM file");

  // instantiate the WASI environment
  wasm_runtime_set_wasi_args (module, dirs, 1, রিক্ত, 0, (ধ্রুৱ ৱর্ণ **) container_env, env_গুনতি, (ৱর্ণ **) argv, arg_গুনতি);

  // enable the WASI socket api
  wasm_runtime_set_wasi_addr_pool (module, wasi_addr_pool, 2);
  wasm_runtime_set_wasi_ns_lookup_pool (module, wasi_ns_lookup_pool, 1);

  // create an instance of the WASM module (WASM linear memory is ready)
  module_inst = wasm_runtime_instantiate (module, স্তূপ_size, heap_size, error_buf, পরিমাণ (error_buf));
  যদি (! module_inst)
    error (EXIT_FAILURE, 0, "Failed to instantiate the WASM module");

  // look up a WASM function by its name (The function signature can NULL here)
  func = wasm_runtime_lookup_function (module_inst, "_start");
  যদি (! func)
    error (EXIT_FAILURE, 0, "Failed to look up the WASM function");

  // create an execution environment to execute the WASM functions
  exec_env = wasm_runtime_create_exec_env (module_inst, স্তূপ_size);
  যদি (! exec_env)
    error (EXIT_FAILURE, 0, "Failed to create the execution environment");

  // call the WASM function
  ret = wasm_runtime_call_wasm (exec_env, func, 0, রিক্ত);
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

স্থির পূর্ণাঙ্ক
libwamr_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  ঘূৰাই wasm_can_handle_container (container, err);
}

সংরচনা custom_handler_s handler_wamr = {
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
