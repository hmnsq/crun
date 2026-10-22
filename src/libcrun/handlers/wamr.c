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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "../custom-handler.h"
#ಅನ್ತರ್ಭಾವ "../container.h"
#ಅನ್ತರ್ಭಾವ "../utils.h"
#ಅನ್ತರ್ಭಾವ "../linux.h"
#ಅನ್ತರ್ಭಾವ "handler-utils.h"
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <sched.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WAMR
#  include <wasm_export.h>
#endif

#ಯದಿ HAVE_DLOPEN && HAVE_WAMR

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libwamr_load (ಶೂನ್ಯ **cookie, libcrun_error_t *err)
{
  ಶೂನ್ಯ *handle;

  handle = dlopen ("libiwasm.so", RTLD_NOW);
  ಯದಿ (handle == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not load `libiwasm.so`: `%s`", dlerror ());
  *cookie = handle;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libwamr_unload (ಶೂನ್ಯ *cookie, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ r;

  ಯದಿ (cookie)
    {
      r = dlclose (cookie);
      ಯದಿ (UNLIKELY (r < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libwamr_exec (ಶೂನ್ಯ *cookie, __attribute__ ((unused)) libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *pathname, ವರ್ಣ *ಧ್ರುವ argv[])
{
  // load symbols from the shared library libiwasm.so
  bool (*wasm_runtime_init) ();
  RuntimeInitArgs init_args;
  bool (*wasm_runtime_full_init) (RuntimeInitArgs *init_args);
  wasm_module_t module;
  wasm_module_t (*wasm_runtime_load) (uint8_t *buf, uint32_t size, ವರ್ಣ *error_buf, uint32_t error_buf_size);
  wasm_module_inst_t module_inst;
  wasm_module_inst_t (*wasm_runtime_instantiate) (ಧ್ರುವ wasm_module_t module, uint32_t default_ರಾಶಿ_size, uint32_t host_managed_heap_size, ವರ್ಣ *error_buf, uint32_t error_buf_size);
  wasm_function_inst_t func;
  wasm_function_inst_t (*wasm_runtime_lookup_function) (wasm_module_inst_t ಧ್ರುವ module_inst, ಧ್ರುವ ವರ್ಣ *name);
  wasm_exec_env_t exec_env;
  wasm_exec_env_t (*wasm_runtime_create_exec_env) (wasm_module_inst_t module_inst, uint32_t ರಾಶಿ_size);
  bool (*wasm_runtime_call_wasm) (wasm_exec_env_t exec_env, wasm_function_inst_t function, uint32_t argc, uint32_t argv[]);
  ಧ್ರುವ ವರ್ಣ *(*wasm_runtime_get_exception) (wasm_module_inst_t module_inst);
  ಶೂನ್ಯ (*wasm_runtime_set_exception) (wasm_module_inst_t module_inst, ಧ್ರುವ ವರ್ಣ *exception);
  ಶೂನ್ಯ (*wasm_runtime_clear_exception) (wasm_module_inst_t module_inst);
  ಶೂನ್ಯ (*wasm_runtime_destroy_exec_env) (wasm_exec_env_t exec_env);
  ಶೂನ್ಯ (*wasm_runtime_deinstantiate) (wasm_module_inst_t module_inst);
  ಶೂನ್ಯ (*wasm_runtime_unload) (wasm_module_t module);
  ಶೂನ್ಯ (*wasm_runtime_destroy) ();
  uint32_t (*wasm_runtime_get_wasi_exit_code) (wasm_module_inst_t module_inst);
  bool (*wasm_application_execute_main) (wasm_module_inst_t module_inst, int32_t argc, ವರ್ಣ *argv[]);
  ಶೂನ್ಯ (*wasm_runtime_set_wasi_args) (wasm_module_t module, ಧ್ರುವ ವರ್ಣ *dir_list[], uint32_t dir_ಎಣಿಕೆ, ಧ್ರುವ ವರ್ಣ *map_dir_list[], uint32_t map_dir_ಎಣಿಕೆ, ಧ್ರುವ ವರ್ಣ *env[], uint32_t env_ಎಣಿಕೆ, ವರ್ಣ *argv[], ಪೂರ್ಣಾಙ್ಕ argc);
  ಶೂನ್ಯ (*wasm_runtime_set_wasi_addr_pool) (wasm_module_t module, ಧ್ರುವ ವರ್ಣ *addr_pool[], uint32_t pool_size);
  ಶೂನ್ಯ (*wasm_runtime_set_wasi_ns_lookup_pool) (wasm_module_t module, ಧ್ರುವ ವರ್ಣ *ns_lookup_pool[], uint32_t pool_size);

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

  ಯದಿ (wasm_runtime_init == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_init symbol in `libiwasm.so`");
  ಯದಿ (wasm_runtime_full_init == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_full_init symbol in `libiwasm.so`");
  ಯದಿ (wasm_runtime_load == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_load symbol in `libiwasm.so`");
  ಯದಿ (wasm_runtime_instantiate == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_instantiate symbol in `libiwasm.so`");
  ಯದಿ (wasm_runtime_lookup_function == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_lookup_function symbol in `libiwasm.so`");
  ಯದಿ (wasm_runtime_create_exec_env == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_create_exec_env symbol in `libiwasm.so`");
  ಯದಿ (wasm_runtime_call_wasm == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_call_wasm symbol in `libiwasm.so`");
  ಯದಿ (wasm_runtime_get_exception == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_exception symbol in `libiwasm.so`");
  ಯದಿ (wasm_runtime_set_exception == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_exception symbol in `libiwasm.so`");
  ಯದಿ (wasm_runtime_clear_exception == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_clear_exception symbol in `libiwasm.so`");
  ಯದಿ (wasm_runtime_destroy_exec_env == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy_exec_env symbol in `libiwasm.so`");
  ಯದಿ (wasm_runtime_deinstantiate == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_deinstantiate symbol in `libiwasm.so`");
  ಯದಿ (wasm_runtime_unload == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_unload symbol in `libiwasm.so`");
  ಯದಿ (wasm_runtime_destroy == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy symbol in `libiwasm.so`");
  ಯದಿ (wasm_runtime_get_wasi_exit_code == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_wasi_exit_code symbol in `libiwasm.so`");
  ಯದಿ (wasm_application_execute_main == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find wasm_application_execute_main symbol in `libiwasm.so`");
  ಯದಿ (wasm_runtime_set_wasi_args == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_args symbol in `libiwasm.so`");
  ಯದಿ (wasm_runtime_set_wasi_addr_pool == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_addr_pool symbol in `libiwasm.so`");
  ಯದಿ (wasm_runtime_set_wasi_ns_lookup_pool == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_ns_lookup_pool symbol in `libiwasm.so`");

  ಪೂರ್ಣಾಙ್ಕ ret;
  ಧ್ರುವ ವರ್ಣ *exception;
  cleanup_free ವರ್ಣ *buffer = ರಿಕ್ತ;
  ವರ್ಣ error_buf[128];
  size_t buffer_size;
  uint32_t ರಾಶಿ_size = 8096, heap_size = 0;
  libcrun_error_t tmp_err = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *wasi_proc_exit_exception = "wasi proc exit";
  ಧ್ರುವ ವರ್ಣ *wasi_addr_pool[2] = { "0.0.0.0/0", "::/0" };
  ಧ್ರುವ ವರ್ಣ *wasi_ns_lookup_pool[1] = { "*" };

  ಧ್ರುವ ವರ್ಣ *dirs[2] = { "/", "." };
  ವರ್ಣ **container_env = container->container_def->process->env;
  size_t env_ಎಣಿಕೆ = container->container_def->process->env_len;

  ಪೂರ್ಣಾಙ್ಕ arg_ಎಣಿಕೆ = 0;
  ವರ್ಣ *ಧ್ರುವ *arg;
  ಪ್ರತಿ (arg = argv; *arg != ರಿಕ್ತ; ++arg)
    arg_ಎಣಿಕೆ++;

  // initialize the wasm runtime by default configurations
  ಯದಿ (! wasm_runtime_init ())
    error (EXIT_FAILURE, 0, "Failed to initialize the wasm runtime");

  // read WASM file into a memory buffer
  ret = read_all_file (pathname, &buffer, &buffer_size, &tmp_err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      error (EXIT_FAILURE, 0, "Failed to read file");
    }

  ಯದಿ (UNLIKELY (buffer_size > UINT32_MAX))
    error (EXIT_FAILURE, 0, "File size is too large");

  // parse the WASM file from buffer and create a WASM module
  module = wasm_runtime_load (buffer, buffer_size, error_buf, ಪರಿಮಾಣ (error_buf));
  ಯದಿ (! module)
    error (EXIT_FAILURE, 0, "Failed to load WASM file");

  // instantiate the WASI environment
  wasm_runtime_set_wasi_args (module, dirs, 1, ರಿಕ್ತ, 0, (ಧ್ರುವ ವರ್ಣ **) container_env, env_ಎಣಿಕೆ, (ವರ್ಣ **) argv, arg_ಎಣಿಕೆ);

  // enable the WASI socket api
  wasm_runtime_set_wasi_addr_pool (module, wasi_addr_pool, 2);
  wasm_runtime_set_wasi_ns_lookup_pool (module, wasi_ns_lookup_pool, 1);

  // create an instance of the WASM module (WASM linear memory is ready)
  module_inst = wasm_runtime_instantiate (module, ರಾಶಿ_size, heap_size, error_buf, ಪರಿಮಾಣ (error_buf));
  ಯದಿ (! module_inst)
    error (EXIT_FAILURE, 0, "Failed to instantiate the WASM module");

  // look up a WASM function by its name (The function signature can NULL here)
  func = wasm_runtime_lookup_function (module_inst, "_start");
  ಯದಿ (! func)
    error (EXIT_FAILURE, 0, "Failed to look up the WASM function");

  // create an execution environment to execute the WASM functions
  exec_env = wasm_runtime_create_exec_env (module_inst, ರಾಶಿ_size);
  ಯದಿ (! exec_env)
    error (EXIT_FAILURE, 0, "Failed to create the execution environment");

  // call the WASM function
  ret = wasm_runtime_call_wasm (exec_env, func, 0, ರಿಕ್ತ);
  ಯದಿ (ret)
    wasm_runtime_set_exception (module_inst, wasi_proc_exit_exception);
  exception = wasm_runtime_get_exception (module_inst);
  ಯದಿ (! strstr (exception, wasi_proc_exit_exception))
    error (EXIT_FAILURE, 0, "Failed to call the WASM function");
  wasm_runtime_clear_exception (module_inst);

  wasm_runtime_destroy_exec_env (exec_env);
  wasm_runtime_deinstantiate (module_inst);
  wasm_runtime_unload (module);
  wasm_runtime_destroy ();

  exit (EXIT_SUCCESS);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libwamr_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ wasm_can_handle_container (container, err);
}

ಸಂರಚನಾ custom_handler_s handler_wamr = {
  .name = "wamr",
  .alias = "wasm",
  .feature_string = "WASM:wamr",
  .supports_open_ವೃಕ್ಷ_namespace = false,
  .load = libwamr_load,
  .unload = libwamr_unload,
  .run_func = libwamr_exec,
  .can_handle_container = libwamr_can_handle_container,
};

#endif
