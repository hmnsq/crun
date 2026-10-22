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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "../custom-handler.h"
#ଅନ୍ତର୍ଭାଵ "../container.h"
#ଅନ୍ତର୍ଭାଵ "../utils.h"
#ଅନ୍ତର୍ଭାଵ "../linux.h"
#ଅନ୍ତର୍ଭାଵ "handler-utils.h"
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <sched.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WAMR
#  include <wasm_export.h>
#endif

#ଯଦି HAVE_DLOPEN && HAVE_WAMR

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libwamr_load (ଶୂନ୍ୟ **cookie, libcrun_error_t *err)
{
  ଶୂନ୍ୟ *handle;

  handle = dlopen ("libiwasm.so", RTLD_NOW);
  ଯଦି (handle == ଖାଲି)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "could not load `libiwasm.so`: `%s`", dlerror ());
  *cookie = handle;

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libwamr_unload (ଶୂନ୍ୟ *cookie, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା r;

  ଯଦି (cookie)
    {
      r = dlclose (cookie);
      ଯଦି (UNLIKELY (r < 0))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libwamr_exec (ଶୂନ୍ୟ *cookie, __attribute__ ((unused)) libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *pathname, ଅକ୍ଷର *ଅଚଳ argv[])
{
  // load symbols from the shared library libiwasm.so
  bool (*wasm_runtime_init) ();
  RuntimeInitArgs init_args;
  bool (*wasm_runtime_full_init) (RuntimeInitArgs *init_args);
  wasm_module_t module;
  wasm_module_t (*wasm_runtime_load) (uint8_t *buf, uint32_t size, ଅକ୍ଷର *error_buf, uint32_t error_buf_size);
  wasm_module_inst_t module_inst;
  wasm_module_inst_t (*wasm_runtime_instantiate) (ଅଚଳ wasm_module_t module, uint32_t default_ଗଦା_size, uint32_t host_managed_heap_size, ଅକ୍ଷର *error_buf, uint32_t error_buf_size);
  wasm_function_inst_t func;
  wasm_function_inst_t (*wasm_runtime_lookup_function) (wasm_module_inst_t ଅଚଳ module_inst, ଅଚଳ ଅକ୍ଷର *name);
  wasm_exec_env_t exec_env;
  wasm_exec_env_t (*wasm_runtime_create_exec_env) (wasm_module_inst_t module_inst, uint32_t ଗଦା_size);
  bool (*wasm_runtime_call_wasm) (wasm_exec_env_t exec_env, wasm_function_inst_t function, uint32_t argc, uint32_t argv[]);
  ଅଚଳ ଅକ୍ଷର *(*wasm_runtime_get_exception) (wasm_module_inst_t module_inst);
  ଶୂନ୍ୟ (*wasm_runtime_set_exception) (wasm_module_inst_t module_inst, ଅଚଳ ଅକ୍ଷର *exception);
  ଶୂନ୍ୟ (*wasm_runtime_clear_exception) (wasm_module_inst_t module_inst);
  ଶୂନ୍ୟ (*wasm_runtime_destroy_exec_env) (wasm_exec_env_t exec_env);
  ଶୂନ୍ୟ (*wasm_runtime_deinstantiate) (wasm_module_inst_t module_inst);
  ଶୂନ୍ୟ (*wasm_runtime_unload) (wasm_module_t module);
  ଶୂନ୍ୟ (*wasm_runtime_destroy) ();
  uint32_t (*wasm_runtime_get_wasi_exit_code) (wasm_module_inst_t module_inst);
  bool (*wasm_application_execute_main) (wasm_module_inst_t module_inst, int32_t argc, ଅକ୍ଷର *argv[]);
  ଶୂନ୍ୟ (*wasm_runtime_set_wasi_args) (wasm_module_t module, ଅଚଳ ଅକ୍ଷର *dir_list[], uint32_t dir_ଗଣନା, ଅଚଳ ଅକ୍ଷର *map_dir_list[], uint32_t map_dir_ଗଣନା, ଅଚଳ ଅକ୍ଷର *env[], uint32_t env_ଗଣନା, ଅକ୍ଷର *argv[], ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc);
  ଶୂନ୍ୟ (*wasm_runtime_set_wasi_addr_pool) (wasm_module_t module, ଅଚଳ ଅକ୍ଷର *addr_pool[], uint32_t pool_size);
  ଶୂନ୍ୟ (*wasm_runtime_set_wasi_ns_lookup_pool) (wasm_module_t module, ଅଚଳ ଅକ୍ଷର *ns_lookup_pool[], uint32_t pool_size);

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

  ଯଦି (wasm_runtime_init == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_init symbol in `libiwasm.so`");
  ଯଦି (wasm_runtime_full_init == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_full_init symbol in `libiwasm.so`");
  ଯଦି (wasm_runtime_load == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_load symbol in `libiwasm.so`");
  ଯଦି (wasm_runtime_instantiate == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_instantiate symbol in `libiwasm.so`");
  ଯଦି (wasm_runtime_lookup_function == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_lookup_function symbol in `libiwasm.so`");
  ଯଦି (wasm_runtime_create_exec_env == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_create_exec_env symbol in `libiwasm.so`");
  ଯଦି (wasm_runtime_call_wasm == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_call_wasm symbol in `libiwasm.so`");
  ଯଦି (wasm_runtime_get_exception == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_exception symbol in `libiwasm.so`");
  ଯଦି (wasm_runtime_set_exception == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_exception symbol in `libiwasm.so`");
  ଯଦି (wasm_runtime_clear_exception == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_clear_exception symbol in `libiwasm.so`");
  ଯଦି (wasm_runtime_destroy_exec_env == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy_exec_env symbol in `libiwasm.so`");
  ଯଦି (wasm_runtime_deinstantiate == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_deinstantiate symbol in `libiwasm.so`");
  ଯଦି (wasm_runtime_unload == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_unload symbol in `libiwasm.so`");
  ଯଦି (wasm_runtime_destroy == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy symbol in `libiwasm.so`");
  ଯଦି (wasm_runtime_get_wasi_exit_code == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_wasi_exit_code symbol in `libiwasm.so`");
  ଯଦି (wasm_application_execute_main == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find wasm_application_execute_main symbol in `libiwasm.so`");
  ଯଦି (wasm_runtime_set_wasi_args == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_args symbol in `libiwasm.so`");
  ଯଦି (wasm_runtime_set_wasi_addr_pool == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_addr_pool symbol in `libiwasm.so`");
  ଯଦି (wasm_runtime_set_wasi_ns_lookup_pool == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_ns_lookup_pool symbol in `libiwasm.so`");

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ଅଚଳ ଅକ୍ଷର *exception;
  cleanup_free ଅକ୍ଷର *buffer = ଖାଲି;
  ଅକ୍ଷର error_buf[128];
  size_t buffer_size;
  uint32_t ଗଦା_size = 8096, heap_size = 0;
  libcrun_error_t tmp_err = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *wasi_proc_exit_exception = "wasi proc exit";
  ଅଚଳ ଅକ୍ଷର *wasi_addr_pool[2] = { "0.0.0.0/0", "::/0" };
  ଅଚଳ ଅକ୍ଷର *wasi_ns_lookup_pool[1] = { "*" };

  ଅଚଳ ଅକ୍ଷର *dirs[2] = { "/", "." };
  ଅକ୍ଷର **container_env = container->container_def->process->env;
  size_t env_ଗଣନା = container->container_def->process->env_len;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା arg_ଗଣନା = 0;
  ଅକ୍ଷର *ଅଚଳ *arg;
  ପାଇଁ (arg = argv; *arg != ଖାଲି; ++arg)
    arg_ଗଣନା++;

  // initialize the wasm runtime by default configurations
  ଯଦି (! wasm_runtime_init ())
    error (EXIT_FAILURE, 0, "Failed to initialize the wasm runtime");

  // read WASM file into a memory buffer
  ret = read_all_file (pathname, &buffer, &buffer_size, &tmp_err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      error (EXIT_FAILURE, 0, "Failed to read file");
    }

  ଯଦି (UNLIKELY (buffer_size > UINT32_MAX))
    error (EXIT_FAILURE, 0, "File size is too large");

  // parse the WASM file from buffer and create a WASM module
  module = wasm_runtime_load (buffer, buffer_size, error_buf, ଆକାର (error_buf));
  ଯଦି (! module)
    error (EXIT_FAILURE, 0, "Failed to load WASM file");

  // instantiate the WASI environment
  wasm_runtime_set_wasi_args (module, dirs, 1, ଖାଲି, 0, (ଅଚଳ ଅକ୍ଷର **) container_env, env_ଗଣନା, (ଅକ୍ଷର **) argv, arg_ଗଣନା);

  // enable the WASI socket api
  wasm_runtime_set_wasi_addr_pool (module, wasi_addr_pool, 2);
  wasm_runtime_set_wasi_ns_lookup_pool (module, wasi_ns_lookup_pool, 1);

  // create an instance of the WASM module (WASM linear memory is ready)
  module_inst = wasm_runtime_instantiate (module, ଗଦା_size, heap_size, error_buf, ଆକାର (error_buf));
  ଯଦି (! module_inst)
    error (EXIT_FAILURE, 0, "Failed to instantiate the WASM module");

  // look up a WASM function by its name (The function signature can NULL here)
  func = wasm_runtime_lookup_function (module_inst, "_start");
  ଯଦି (! func)
    error (EXIT_FAILURE, 0, "Failed to look up the WASM function");

  // create an execution environment to execute the WASM functions
  exec_env = wasm_runtime_create_exec_env (module_inst, ଗଦା_size);
  ଯଦି (! exec_env)
    error (EXIT_FAILURE, 0, "Failed to create the execution environment");

  // call the WASM function
  ret = wasm_runtime_call_wasm (exec_env, func, 0, ଖାଲି);
  ଯଦି (ret)
    wasm_runtime_set_exception (module_inst, wasi_proc_exit_exception);
  exception = wasm_runtime_get_exception (module_inst);
  ଯଦି (! strstr (exception, wasi_proc_exit_exception))
    error (EXIT_FAILURE, 0, "Failed to call the WASM function");
  wasm_runtime_clear_exception (module_inst);

  wasm_runtime_destroy_exec_env (exec_env);
  wasm_runtime_deinstantiate (module_inst);
  wasm_runtime_unload (module);
  wasm_runtime_destroy ();

  exit (EXIT_SUCCESS);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libwamr_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ wasm_can_handle_container (container, err);
}

ସଂରଚନା custom_handler_s handler_wamr = {
  .name = "wamr",
  .alias = "wasm",
  .feature_string = "WASM:wamr",
  .supports_open_ବୃକ୍ଷ_namespace = false,
  .load = libwamr_load,
  .unload = libwamr_unload,
  .run_func = libwamr_exec,
  .can_handle_container = libwamr_can_handle_container,
};

#endif
