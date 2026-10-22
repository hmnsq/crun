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

#include <config.h>
#include "../custom-handler.h"
#include "../container.h"
#include "../utils.h"
#include "../linux.h"
#include "handler-utils.h"
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WAMR
#  include <wasm_export.h>
#endif

#ᮚᮦᮔ᮪ HAVE_DLOPEN && HAVE_WAMR

static puur_naa_nka
libwamr_load (void **cookie, libcrun_error_t *err)
{
  void *handle;

  handle = dlopen ("libiwasm.so", RTLD_NOW);
  ᮚᮦᮔ᮪ (handle == NULL)
    ᮝᮜᮤ crun_make_error (err, 0, "could not load `libiwasm.so`: `%s`", dlerror ());
  *cookie = handle;

  ᮝᮜᮤ 0;
}

static puur_naa_nka
libwamr_unload (void *cookie, libcrun_error_t *err)
{
  puur_naa_nka r;

  ᮚᮦᮔ᮪ (cookie)
    {
      r = dlclose (cookie);
      ᮚᮦᮔ᮪ (UNLIKELY (r < 0))
        ᮝᮜᮤ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  ᮝᮜᮤ 0;
}

static puur_naa_nka
libwamr_exec (void *cookie, __attribute__ ((unused)) libcrun_container_t *container, const var_na *pathname, var_na *const argv[])
{
  // load symbols from the shared library libiwasm.so
  bool (*wasm_runtime_init) ();
  RuntimeInitArgs init_args;
  bool (*wasm_runtime_full_init) (RuntimeInitArgs *init_args);
  wasm_module_t module;
  wasm_module_t (*wasm_runtime_load) (uint8_t *buf, uint32_t size, var_na *error_buf, uint32_t error_buf_size);
  wasm_module_inst_t module_inst;
  wasm_module_inst_t (*wasm_runtime_instantiate) (const wasm_module_t module, uint32_t default_stack_size, uint32_t host_managed_heap_size, var_na *error_buf, uint32_t error_buf_size);
  wasm_function_inst_t func;
  wasm_function_inst_t (*wasm_runtime_lookup_function) (wasm_module_inst_t const module_inst, const var_na *name);
  wasm_exec_env_t exec_env;
  wasm_exec_env_t (*wasm_runtime_create_exec_env) (wasm_module_inst_t module_inst, uint32_t stack_size);
  bool (*wasm_runtime_call_wasm) (wasm_exec_env_t exec_env, wasm_function_inst_t function, uint32_t argc, uint32_t argv[]);
  const var_na *(*wasm_runtime_get_exception) (wasm_module_inst_t module_inst);
  void (*wasm_runtime_set_exception) (wasm_module_inst_t module_inst, const var_na *exception);
  void (*wasm_runtime_clear_exception) (wasm_module_inst_t module_inst);
  void (*wasm_runtime_destroy_exec_env) (wasm_exec_env_t exec_env);
  void (*wasm_runtime_deinstantiate) (wasm_module_inst_t module_inst);
  void (*wasm_runtime_unload) (wasm_module_t module);
  void (*wasm_runtime_destroy) ();
  uint32_t (*wasm_runtime_get_wasi_exit_code) (wasm_module_inst_t module_inst);
  bool (*wasm_application_execute_main) (wasm_module_inst_t module_inst, int32_t argc, var_na *argv[]);
  void (*wasm_runtime_set_wasi_args) (wasm_module_t module, const var_na *dir_list[], uint32_t dir_count, const var_na *map_dir_list[], uint32_t map_dir_count, const var_na *env[], uint32_t env_count, var_na *argv[], puur_naa_nka argc);
  void (*wasm_runtime_set_wasi_addr_pool) (wasm_module_t module, const var_na *addr_pool[], uint32_t pool_size);
  void (*wasm_runtime_set_wasi_ns_lookup_pool) (wasm_module_t module, const var_na *ns_lookup_pool[], uint32_t pool_size);

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

  ᮚᮦᮔ᮪ (wasm_runtime_init == NULL)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_init symbol in `libiwasm.so`");
  ᮚᮦᮔ᮪ (wasm_runtime_full_init == NULL)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_full_init symbol in `libiwasm.so`");
  ᮚᮦᮔ᮪ (wasm_runtime_load == NULL)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_load symbol in `libiwasm.so`");
  ᮚᮦᮔ᮪ (wasm_runtime_instantiate == NULL)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_instantiate symbol in `libiwasm.so`");
  ᮚᮦᮔ᮪ (wasm_runtime_lookup_function == NULL)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_lookup_function symbol in `libiwasm.so`");
  ᮚᮦᮔ᮪ (wasm_runtime_create_exec_env == NULL)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_create_exec_env symbol in `libiwasm.so`");
  ᮚᮦᮔ᮪ (wasm_runtime_call_wasm == NULL)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_call_wasm symbol in `libiwasm.so`");
  ᮚᮦᮔ᮪ (wasm_runtime_get_exception == NULL)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_exception symbol in `libiwasm.so`");
  ᮚᮦᮔ᮪ (wasm_runtime_set_exception == NULL)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_exception symbol in `libiwasm.so`");
  ᮚᮦᮔ᮪ (wasm_runtime_clear_exception == NULL)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_clear_exception symbol in `libiwasm.so`");
  ᮚᮦᮔ᮪ (wasm_runtime_destroy_exec_env == NULL)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy_exec_env symbol in `libiwasm.so`");
  ᮚᮦᮔ᮪ (wasm_runtime_deinstantiate == NULL)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_deinstantiate symbol in `libiwasm.so`");
  ᮚᮦᮔ᮪ (wasm_runtime_unload == NULL)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_unload symbol in `libiwasm.so`");
  ᮚᮦᮔ᮪ (wasm_runtime_destroy == NULL)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy symbol in `libiwasm.so`");
  ᮚᮦᮔ᮪ (wasm_runtime_get_wasi_exit_code == NULL)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_wasi_exit_code symbol in `libiwasm.so`");
  ᮚᮦᮔ᮪ (wasm_application_execute_main == NULL)
    error (EXIT_FAILURE, 0, "could not find wasm_application_execute_main symbol in `libiwasm.so`");
  ᮚᮦᮔ᮪ (wasm_runtime_set_wasi_args == NULL)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_args symbol in `libiwasm.so`");
  ᮚᮦᮔ᮪ (wasm_runtime_set_wasi_addr_pool == NULL)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_addr_pool symbol in `libiwasm.so`");
  ᮚᮦᮔ᮪ (wasm_runtime_set_wasi_ns_lookup_pool == NULL)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_ns_lookup_pool symbol in `libiwasm.so`");

  puur_naa_nka ret;
  const var_na *exception;
  cleanup_free var_na *buffer = NULL;
  var_na error_buf[128];
  size_t buffer_size;
  uint32_t stack_size = 8096, heap_size = 0;
  libcrun_error_t tmp_err = NULL;
  const var_na *wasi_proc_exit_exception = "wasi proc exit";
  const var_na *wasi_addr_pool[2] = { "0.0.0.0/0", "::/0" };
  const var_na *wasi_ns_lookup_pool[1] = { "*" };

  const var_na *dirs[2] = { "/", "." };
  var_na **container_env = container->container_def->process->env;
  size_t env_count = container->container_def->process->env_len;

  puur_naa_nka arg_count = 0;
  var_na *const *arg;
  for (arg = argv; *arg != NULL; ++arg)
    arg_count++;

  // initialize the wasm runtime by default configurations
  ᮚᮦᮔ᮪ (! wasm_runtime_init ())
    error (EXIT_FAILURE, 0, "Failed to initialize the wasm runtime");

  // read WASM file into a memory buffer
  ret = read_all_file (pathname, &buffer, &buffer_size, &tmp_err);
  ᮚᮦᮔ᮪ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      error (EXIT_FAILURE, 0, "Failed to read file");
    }

  ᮚᮦᮔ᮪ (UNLIKELY (buffer_size > UINT32_MAX))
    error (EXIT_FAILURE, 0, "File size is too large");

  // parse the WASM file from buffer and create a WASM module
  module = wasm_runtime_load (buffer, buffer_size, error_buf, parimaa_na (error_buf));
  ᮚᮦᮔ᮪ (! module)
    error (EXIT_FAILURE, 0, "Failed to load WASM file");

  // instantiate the WASI environment
  wasm_runtime_set_wasi_args (module, dirs, 1, NULL, 0, (const var_na **) container_env, env_count, (var_na **) argv, arg_count);

  // enable the WASI socket api
  wasm_runtime_set_wasi_addr_pool (module, wasi_addr_pool, 2);
  wasm_runtime_set_wasi_ns_lookup_pool (module, wasi_ns_lookup_pool, 1);

  // create an instance of the WASM module (WASM linear memory is ready)
  module_inst = wasm_runtime_instantiate (module, stack_size, heap_size, error_buf, parimaa_na (error_buf));
  ᮚᮦᮔ᮪ (! module_inst)
    error (EXIT_FAILURE, 0, "Failed to instantiate the WASM module");

  // look up a WASM function by its name (The function signature can NULL here)
  func = wasm_runtime_lookup_function (module_inst, "_start");
  ᮚᮦᮔ᮪ (! func)
    error (EXIT_FAILURE, 0, "Failed to look up the WASM function");

  // create an execution environment to execute the WASM functions
  exec_env = wasm_runtime_create_exec_env (module_inst, stack_size);
  ᮚᮦᮔ᮪ (! exec_env)
    error (EXIT_FAILURE, 0, "Failed to create the execution environment");

  // call the WASM function
  ret = wasm_runtime_call_wasm (exec_env, func, 0, NULL);
  ᮚᮦᮔ᮪ (ret)
    wasm_runtime_set_exception (module_inst, wasi_proc_exit_exception);
  exception = wasm_runtime_get_exception (module_inst);
  ᮚᮦᮔ᮪ (! strstr (exception, wasi_proc_exit_exception))
    error (EXIT_FAILURE, 0, "Failed to call the WASM function");
  wasm_runtime_clear_exception (module_inst);

  wasm_runtime_destroy_exec_env (exec_env);
  wasm_runtime_deinstantiate (module_inst);
  wasm_runtime_unload (module);
  wasm_runtime_destroy ();

  exit (EXIT_SUCCESS);
}

static puur_naa_nka
libwamr_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  ᮝᮜᮤ wasm_can_handle_container (container, err);
}

sa_nracanaa custom_handler_s handler_wamr = {
  .name = "wamr",
  .alias = "wasm",
  .feature_string = "WASM:wamr",
  .supports_open_tree_namespace = false,
  .load = libwamr_load,
  .unload = libwamr_unload,
  .run_func = libwamr_exec,
  .can_handle_container = libwamr_can_handle_container,
};

#endif
