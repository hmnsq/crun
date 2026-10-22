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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "../custom-handler.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../container.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../linux.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "handler-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sched.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DLOPEN
#  include <dlfcn.h>
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_WAMR
#  include <wasm_export.h>
#ਜੇ_ਅੰਤ

#ਜੇ HAVE_DLOPEN && HAVE_WAMR

ਸਥਿਰ ਪੂਰਨ
libwamr_ਲੱਦੋ (ਖਾਲੀ **cookie, libcrun_error_t *err)
{
  ਖਾਲੀ *handle;

  handle = dlopen ("libiwasm.so", RTLD_NOW);
  ਜੇ (handle == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, 0, "could not load `libiwasm.so`: `%s`", dlerror ());
  *cookie = handle;

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
libwamr_unload (ਖਾਲੀ *cookie, libcrun_error_t *err)
{
  ਪੂਰਨ r;

  ਜੇ (cookie)
    {
      r = dlclose (cookie);
      ਜੇ (UNLIKELY (r < 0))
        ਵਾਪਸ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
libwamr_exec (ਖਾਲੀ *cookie, __attribute__ ((ਅਣਵਰਤਿਆ)) libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *pathname, ਅੱਖਰ *ਅਚਲ argv[])
{
  // load symbols from the shared library libiwasm.so
  bool (*wasm_runtime_init) ();
  Runtime_Init_ਦਲੀਲਾਂ init_ਦਲੀਲਾਂ;
  bool (*wasm_runtime_ਭਰਿਆ_init) (Runtime_Init_ਦਲੀਲਾਂ *init_ਦਲੀਲਾਂ);
  wasm_module_t module;
  wasm_module_t (*wasm_runtime_ਲੱਦੋ) (uint8_t *buf, uint32_t ਮਾਪ, ਅੱਖਰ *error_buf, uint32_t error_buf_ਮਾਪ);
  wasm_module_inst_t module_inst;
  wasm_module_inst_t (*wasm_runtime_instantiate) (ਅਚਲ wasm_module_t module, uint32_t default_ਢੇਰ_ਮਾਪ, uint32_t host_managed_ਢੇਰ_ਮਾਪ, ਅੱਖਰ *error_buf, uint32_t error_buf_ਮਾਪ);
  wasm_function_inst_t func;
  wasm_function_inst_t (*wasm_runtime_lookup_function) (wasm_module_inst_t ਅਚਲ module_inst, ਅਚਲ ਅੱਖਰ *name);
  wasm_exec_env_t exec_env;
  wasm_exec_env_t (*wasm_runtime_create_exec_env) (wasm_module_inst_t module_inst, uint32_t ਢੇਰ_ਮਾਪ);
  bool (*wasm_runtime_call_wasm) (wasm_exec_env_t exec_env, wasm_function_inst_t function, uint32_t argc, uint32_t argv[]);
  ਅਚਲ ਅੱਖਰ *(*wasm_runtime_get_exception) (wasm_module_inst_t module_inst);
  ਖਾਲੀ (*wasm_runtime_ਸੈੱਟ_exception) (wasm_module_inst_t module_inst, ਅਚਲ ਅੱਖਰ *exception);
  ਖਾਲੀ (*wasm_runtime_clear_exception) (wasm_module_inst_t module_inst);
  ਖਾਲੀ (*wasm_runtime_destroy_exec_env) (wasm_exec_env_t exec_env);
  ਖਾਲੀ (*wasm_runtime_deinstantiate) (wasm_module_inst_t module_inst);
  ਖਾਲੀ (*wasm_runtime_unload) (wasm_module_t module);
  ਖਾਲੀ (*wasm_runtime_destroy) ();
  uint32_t (*wasm_runtime_get_wasi_exit_ਕੋਡ) (wasm_module_inst_t module_inst);
  bool (*wasm_application_execute_main) (wasm_module_inst_t module_inst, int32_t argc, ਅੱਖਰ *argv[]);
  ਖਾਲੀ (*wasm_runtime_ਸੈੱਟ_wasi_ਦਲੀਲਾਂ) (wasm_module_t module, ਅਚਲ ਅੱਖਰ *dir_list[], uint32_t dir_ਗਿਣਤੀ, ਅਚਲ ਅੱਖਰ *map_dir_list[], uint32_t map_dir_ਗਿਣਤੀ, ਅਚਲ ਅੱਖਰ *env[], uint32_t env_ਗਿਣਤੀ, ਅੱਖਰ *argv[], ਪੂਰਨ argc);
  ਖਾਲੀ (*wasm_runtime_ਸੈੱਟ_wasi_addr_pool) (wasm_module_t module, ਅਚਲ ਅੱਖਰ *addr_pool[], uint32_t pool_ਮਾਪ);
  ਖਾਲੀ (*wasm_runtime_ਸੈੱਟ_wasi_ns_lookup_pool) (wasm_module_t module, ਅਚਲ ਅੱਖਰ *ns_lookup_pool[], uint32_t pool_ਮਾਪ);

  wasm_runtime_init = dlsym (cookie, "wasm_runtime_init");
  wasm_runtime_ਭਰਿਆ_init = dlsym (cookie, "wasm_runtime_full_init");
  wasm_runtime_ਲੱਦੋ = dlsym (cookie, "wasm_runtime_load");
  wasm_runtime_instantiate = dlsym (cookie, "wasm_runtime_instantiate");
  wasm_runtime_lookup_function = dlsym (cookie, "wasm_runtime_lookup_function");
  wasm_runtime_create_exec_env = dlsym (cookie, "wasm_runtime_create_exec_env");
  wasm_runtime_call_wasm = dlsym (cookie, "wasm_runtime_call_wasm");
  wasm_runtime_get_exception = dlsym (cookie, "wasm_runtime_get_exception");
  wasm_runtime_ਸੈੱਟ_exception = dlsym (cookie, "wasm_runtime_set_exception");
  wasm_runtime_clear_exception = dlsym (cookie, "wasm_runtime_clear_exception");
  wasm_runtime_destroy_exec_env = dlsym (cookie, "wasm_runtime_destroy_exec_env");
  wasm_runtime_deinstantiate = dlsym (cookie, "wasm_runtime_deinstantiate");
  wasm_runtime_unload = dlsym (cookie, "wasm_runtime_unload");
  wasm_runtime_destroy = dlsym (cookie, "wasm_runtime_destroy");
  wasm_runtime_get_wasi_exit_ਕੋਡ = dlsym (cookie, "wasm_runtime_get_wasi_exit_code");
  wasm_application_execute_main = dlsym (cookie, "wasm_application_execute_main");
  wasm_runtime_ਸੈੱਟ_wasi_ਦਲੀਲਾਂ = dlsym (cookie, "wasm_runtime_set_wasi_args");
  wasm_runtime_ਸੈੱਟ_wasi_addr_pool = dlsym (cookie, "wasm_runtime_set_wasi_addr_pool");
  wasm_runtime_ਸੈੱਟ_wasi_ns_lookup_pool = dlsym (cookie, "wasm_runtime_set_wasi_ns_lookup_pool");

  ਜੇ (wasm_runtime_init == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_init symbol in `libiwasm.so`");
  ਜੇ (wasm_runtime_ਭਰਿਆ_init == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_full_init symbol in `libiwasm.so`");
  ਜੇ (wasm_runtime_ਲੱਦੋ == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_load symbol in `libiwasm.so`");
  ਜੇ (wasm_runtime_instantiate == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_instantiate symbol in `libiwasm.so`");
  ਜੇ (wasm_runtime_lookup_function == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_lookup_function symbol in `libiwasm.so`");
  ਜੇ (wasm_runtime_create_exec_env == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_create_exec_env symbol in `libiwasm.so`");
  ਜੇ (wasm_runtime_call_wasm == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_call_wasm symbol in `libiwasm.so`");
  ਜੇ (wasm_runtime_get_exception == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_exception symbol in `libiwasm.so`");
  ਜੇ (wasm_runtime_ਸੈੱਟ_exception == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_exception symbol in `libiwasm.so`");
  ਜੇ (wasm_runtime_clear_exception == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_clear_exception symbol in `libiwasm.so`");
  ਜੇ (wasm_runtime_destroy_exec_env == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy_exec_env symbol in `libiwasm.so`");
  ਜੇ (wasm_runtime_deinstantiate == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_deinstantiate symbol in `libiwasm.so`");
  ਜੇ (wasm_runtime_unload == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_unload symbol in `libiwasm.so`");
  ਜੇ (wasm_runtime_destroy == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy symbol in `libiwasm.so`");
  ਜੇ (wasm_runtime_get_wasi_exit_ਕੋਡ == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_wasi_exit_code symbol in `libiwasm.so`");
  ਜੇ (wasm_application_execute_main == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find wasm_application_execute_main symbol in `libiwasm.so`");
  ਜੇ (wasm_runtime_ਸੈੱਟ_wasi_ਦਲੀਲਾਂ == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_args symbol in `libiwasm.so`");
  ਜੇ (wasm_runtime_ਸੈੱਟ_wasi_addr_pool == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_addr_pool symbol in `libiwasm.so`");
  ਜੇ (wasm_runtime_ਸੈੱਟ_wasi_ns_lookup_pool == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_ns_lookup_pool symbol in `libiwasm.so`");

  ਪੂਰਨ ret;
  ਅਚਲ ਅੱਖਰ *exception;
  cleanup_free ਅੱਖਰ *ਬਫਰ = ਨਿਰਾ;
  ਅੱਖਰ error_buf[128];
  size_t ਬਫਰ_ਮਾਪ;
  uint32_t ਢੇਰ_ਮਾਪ = 8096, ਢੇਰ_ਮਾਪ_2 = 0;
  libcrun_error_t tmp_err = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *wasi_proc_exit_exception = "wasi proc exit";
  ਅਚਲ ਅੱਖਰ *wasi_addr_pool[2] = { "0.0.0.0/0", "::/0" };
  ਅਚਲ ਅੱਖਰ *wasi_ns_lookup_pool[1] = { "*" };

  ਅਚਲ ਅੱਖਰ *dirs[2] = { "/", "." };
  ਅੱਖਰ **container_env = container->container_def->process->env;
  size_t env_ਗਿਣਤੀ = container->container_def->process->env_len;

  ਪੂਰਨ arg_ਗਿਣਤੀ = 0;
  ਅੱਖਰ *ਅਚਲ *arg;
  ਲਈ (arg = argv; *arg != ਨਿਰਾ; ++arg)
    arg_ਗਿਣਤੀ++;

  // initialize the wasm runtime by default configurations
  ਜੇ (! wasm_runtime_init ())
    error (EXIT_FAILURE, 0, "Failed to initialize the wasm runtime");

  // read WASM file into a memory buffer
  ret = read_all_file (pathname, &ਬਫਰ, &ਬਫਰ_ਮਾਪ, &tmp_err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      error (EXIT_FAILURE, 0, "Failed to read file");
    }

  ਜੇ (UNLIKELY (ਬਫਰ_ਮਾਪ > UINT32_MAX))
    error (EXIT_FAILURE, 0, "File size is too large");

  // parse the WASM file from buffer and create a WASM module
  module = wasm_runtime_ਲੱਦੋ (ਬਫਰ, ਬਫਰ_ਮਾਪ, error_buf, ਆਕਾਰ (error_buf));
  ਜੇ (! module)
    error (EXIT_FAILURE, 0, "Failed to load WASM file");

  // instantiate the WASI environment
  wasm_runtime_ਸੈੱਟ_wasi_ਦਲੀਲਾਂ (module, dirs, 1, ਨਿਰਾ, 0, (ਅਚਲ ਅੱਖਰ **) container_env, env_ਗਿਣਤੀ, (ਅੱਖਰ **) argv, arg_ਗਿਣਤੀ);

  // enable the WASI socket api
  wasm_runtime_ਸੈੱਟ_wasi_addr_pool (module, wasi_addr_pool, 2);
  wasm_runtime_ਸੈੱਟ_wasi_ns_lookup_pool (module, wasi_ns_lookup_pool, 1);

  // create an instance of the WASM module (WASM linear memory is ready)
  module_inst = wasm_runtime_instantiate (module, ਢੇਰ_ਮਾਪ, ਢੇਰ_ਮਾਪ_2, error_buf, ਆਕਾਰ (error_buf));
  ਜੇ (! module_inst)
    error (EXIT_FAILURE, 0, "Failed to instantiate the WASM module");

  // look up a WASM function by its name (The function signature can NULL here)
  func = wasm_runtime_lookup_function (module_inst, "_start");
  ਜੇ (! func)
    error (EXIT_FAILURE, 0, "Failed to look up the WASM function");

  // create an execution environment to execute the WASM functions
  exec_env = wasm_runtime_create_exec_env (module_inst, ਢੇਰ_ਮਾਪ);
  ਜੇ (! exec_env)
    error (EXIT_FAILURE, 0, "Failed to create the execution environment");

  // call the WASM function
  ret = wasm_runtime_call_wasm (exec_env, func, 0, ਨਿਰਾ);
  ਜੇ (ret)
    wasm_runtime_ਸੈੱਟ_exception (module_inst, wasi_proc_exit_exception);
  exception = wasm_runtime_get_exception (module_inst);
  ਜੇ (! ਲੜੀ_ਖੋਜ (exception, wasi_proc_exit_exception))
    error (EXIT_FAILURE, 0, "Failed to call the WASM function");
  wasm_runtime_clear_exception (module_inst);

  wasm_runtime_destroy_exec_env (exec_env);
  wasm_runtime_deinstantiate (module_inst);
  wasm_runtime_unload (module);
  wasm_runtime_destroy ();

  ਬਾਹਰ_ਨਿਕਲੋ (EXIT_SUCCESS);
}

ਸਥਿਰ ਪੂਰਨ
libwamr_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  ਵਾਪਸ wasm_can_handle_container (container, err);
}

ਬਣਤਰ custom_handler_s handler_wamr = {
  .name = "wamr",
  .alias = "wasm",
  .feature_string = "WASM:wamr",
  .supports_open_ਰੁੱਖ_namespace = false,
  .ਲੱਦੋ = libwamr_ਲੱਦੋ,
  .unload = libwamr_unload,
  .ਲੜੀਵਾਰ_func = libwamr_exec,
  .can_handle_container = libwamr_can_handle_container,
};

#ਜੇ_ਅੰਤ
