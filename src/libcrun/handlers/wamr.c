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
#تعریف_کړه GNU_سرچینه

#ضمن <config.h>
#ضمن "../custom-handler.h"
#ضمن "../container.h"
#ضمن "../utils.h"
#ضمن "../linux.h"
#ضمن "handler-utils.h"
#ضمن <unistd.h>
#ضمن <sys/stat.h>
#ضمن <errno.h>
#ضمن <sys/types.h>
#ضمن <fcntl.h>
#ضمن <sched.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>

#که_تعریف_شوی HAVE_DLOPEN
#  include <dlfcn.h>
#د_که_پای

#که_تعریف_شوی HAVE_WAMR
#  include <wasm_export.h>
#د_که_پای

#إذا HAVE_DLOPEN && HAVE_WAMR

ساكن صحيح
libwamr_بارول (فراغ **cookie, libcrun_error_t *err)
{
  فراغ *handle;

  handle = dlopen ("libiwasm.so", RTLD_NOW);
  إذا (handle == عدم)
    أرجع crun_make_error (err, 0, "could not load `libiwasm.so`: `%s`", dlerror ());
  *cookie = handle;

  أرجع 0;
}

ساكن صحيح
libwamr_unload (فراغ *cookie, libcrun_error_t *err)
{
  صحيح r;

  إذا (cookie)
    {
      r = dlclose (cookie);
      إذا (UNLIKELY (r < 0))
        أرجع crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  أرجع 0;
}

ساكن صحيح
libwamr_exec (فراغ *cookie, __attribute__ ((ناکارول_شوی)) libcrun_container_t *container, ثابت حرف *pathname, حرف *ثابت argv[])
{
  // load symbols from the shared library libiwasm.so
  bool (*wasm_runtime_init) ();
  Runtime_Init_دلیلونه init_دلیلونه;
  bool (*wasm_runtime_ډک_init) (Runtime_Init_دلیلونه *init_دلیلونه);
  wasm_module_t module;
  wasm_module_t (*wasm_runtime_بارول) (uint8_t *buf, uint32_t اندازه, حرف *error_buf, uint32_t error_buf_اندازه);
  wasm_module_inst_t module_inst;
  wasm_module_inst_t (*wasm_runtime_instantiate) (ثابت wasm_module_t module, uint32_t default_ډېری_اندازه, uint32_t host_managed_ډېرۍ_اندازه, حرف *error_buf, uint32_t error_buf_اندازه);
  wasm_function_inst_t func;
  wasm_function_inst_t (*wasm_runtime_lookup_function) (wasm_module_inst_t ثابت module_inst, ثابت حرف *name);
  wasm_exec_env_t exec_env;
  wasm_exec_env_t (*wasm_runtime_create_exec_env) (wasm_module_inst_t module_inst, uint32_t ډېری_اندازه);
  bool (*wasm_runtime_call_wasm) (wasm_exec_env_t exec_env, wasm_function_inst_t function, uint32_t argc, uint32_t argv[]);
  ثابت حرف *(*wasm_runtime_get_exception) (wasm_module_inst_t module_inst);
  فراغ (*wasm_runtime_سېټ_exception) (wasm_module_inst_t module_inst, ثابت حرف *exception);
  فراغ (*wasm_runtime_clear_exception) (wasm_module_inst_t module_inst);
  فراغ (*wasm_runtime_destroy_exec_env) (wasm_exec_env_t exec_env);
  فراغ (*wasm_runtime_deinstantiate) (wasm_module_inst_t module_inst);
  فراغ (*wasm_runtime_unload) (wasm_module_t module);
  فراغ (*wasm_runtime_destroy) ();
  uint32_t (*wasm_runtime_get_wasi_exit_کوډ) (wasm_module_inst_t module_inst);
  bool (*wasm_application_execute_main) (wasm_module_inst_t module_inst, int32_t argc, حرف *argv[]);
  فراغ (*wasm_runtime_سېټ_wasi_دلیلونه) (wasm_module_t module, ثابت حرف *dir_list[], uint32_t dir_شمېر, ثابت حرف *map_dir_list[], uint32_t map_dir_شمېر, ثابت حرف *env[], uint32_t env_شمېر, حرف *argv[], صحيح argc);
  فراغ (*wasm_runtime_سېټ_wasi_addr_pool) (wasm_module_t module, ثابت حرف *addr_pool[], uint32_t pool_اندازه);
  فراغ (*wasm_runtime_سېټ_wasi_ns_lookup_pool) (wasm_module_t module, ثابت حرف *ns_lookup_pool[], uint32_t pool_اندازه);

  wasm_runtime_init = dlsym (cookie, "wasm_runtime_init");
  wasm_runtime_ډک_init = dlsym (cookie, "wasm_runtime_full_init");
  wasm_runtime_بارول = dlsym (cookie, "wasm_runtime_load");
  wasm_runtime_instantiate = dlsym (cookie, "wasm_runtime_instantiate");
  wasm_runtime_lookup_function = dlsym (cookie, "wasm_runtime_lookup_function");
  wasm_runtime_create_exec_env = dlsym (cookie, "wasm_runtime_create_exec_env");
  wasm_runtime_call_wasm = dlsym (cookie, "wasm_runtime_call_wasm");
  wasm_runtime_get_exception = dlsym (cookie, "wasm_runtime_get_exception");
  wasm_runtime_سېټ_exception = dlsym (cookie, "wasm_runtime_set_exception");
  wasm_runtime_clear_exception = dlsym (cookie, "wasm_runtime_clear_exception");
  wasm_runtime_destroy_exec_env = dlsym (cookie, "wasm_runtime_destroy_exec_env");
  wasm_runtime_deinstantiate = dlsym (cookie, "wasm_runtime_deinstantiate");
  wasm_runtime_unload = dlsym (cookie, "wasm_runtime_unload");
  wasm_runtime_destroy = dlsym (cookie, "wasm_runtime_destroy");
  wasm_runtime_get_wasi_exit_کوډ = dlsym (cookie, "wasm_runtime_get_wasi_exit_code");
  wasm_application_execute_main = dlsym (cookie, "wasm_application_execute_main");
  wasm_runtime_سېټ_wasi_دلیلونه = dlsym (cookie, "wasm_runtime_set_wasi_args");
  wasm_runtime_سېټ_wasi_addr_pool = dlsym (cookie, "wasm_runtime_set_wasi_addr_pool");
  wasm_runtime_سېټ_wasi_ns_lookup_pool = dlsym (cookie, "wasm_runtime_set_wasi_ns_lookup_pool");

  إذا (wasm_runtime_init == عدم)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_init symbol in `libiwasm.so`");
  إذا (wasm_runtime_ډک_init == عدم)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_full_init symbol in `libiwasm.so`");
  إذا (wasm_runtime_بارول == عدم)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_load symbol in `libiwasm.so`");
  إذا (wasm_runtime_instantiate == عدم)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_instantiate symbol in `libiwasm.so`");
  إذا (wasm_runtime_lookup_function == عدم)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_lookup_function symbol in `libiwasm.so`");
  إذا (wasm_runtime_create_exec_env == عدم)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_create_exec_env symbol in `libiwasm.so`");
  إذا (wasm_runtime_call_wasm == عدم)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_call_wasm symbol in `libiwasm.so`");
  إذا (wasm_runtime_get_exception == عدم)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_exception symbol in `libiwasm.so`");
  إذا (wasm_runtime_سېټ_exception == عدم)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_exception symbol in `libiwasm.so`");
  إذا (wasm_runtime_clear_exception == عدم)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_clear_exception symbol in `libiwasm.so`");
  إذا (wasm_runtime_destroy_exec_env == عدم)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy_exec_env symbol in `libiwasm.so`");
  إذا (wasm_runtime_deinstantiate == عدم)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_deinstantiate symbol in `libiwasm.so`");
  إذا (wasm_runtime_unload == عدم)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_unload symbol in `libiwasm.so`");
  إذا (wasm_runtime_destroy == عدم)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy symbol in `libiwasm.so`");
  إذا (wasm_runtime_get_wasi_exit_کوډ == عدم)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_wasi_exit_code symbol in `libiwasm.so`");
  إذا (wasm_application_execute_main == عدم)
    error (EXIT_FAILURE, 0, "could not find wasm_application_execute_main symbol in `libiwasm.so`");
  إذا (wasm_runtime_سېټ_wasi_دلیلونه == عدم)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_args symbol in `libiwasm.so`");
  إذا (wasm_runtime_سېټ_wasi_addr_pool == عدم)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_addr_pool symbol in `libiwasm.so`");
  إذا (wasm_runtime_سېټ_wasi_ns_lookup_pool == عدم)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_ns_lookup_pool symbol in `libiwasm.so`");

  صحيح ret;
  ثابت حرف *exception;
  cleanup_free حرف *بفر = عدم;
  حرف error_buf[128];
  size_t بفر_اندازه;
  uint32_t ډېری_اندازه = 8096, ډېرۍ_اندازه = 0;
  libcrun_error_t tmp_err = عدم;
  ثابت حرف *wasi_proc_exit_exception = "wasi proc exit";
  ثابت حرف *wasi_addr_pool[2] = { "0.0.0.0/0", "::/0" };
  ثابت حرف *wasi_ns_lookup_pool[1] = { "*" };

  ثابت حرف *dirs[2] = { "/", "." };
  حرف **container_env = container->container_def->process->env;
  size_t env_شمېر = container->container_def->process->env_len;

  صحيح arg_شمېر = 0;
  حرف *ثابت *arg;
  لكل (arg = argv; *arg != عدم; ++arg)
    arg_شمېر++;

  // initialize the wasm runtime by default configurations
  إذا (! wasm_runtime_init ())
    error (EXIT_FAILURE, 0, "Failed to initialize the wasm runtime");

  // read WASM file into a memory buffer
  ret = read_all_file (pathname, &بفر, &بفر_اندازه, &tmp_err);
  إذا (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      error (EXIT_FAILURE, 0, "Failed to read file");
    }

  إذا (UNLIKELY (بفر_اندازه > UINT32_MAX))
    error (EXIT_FAILURE, 0, "File size is too large");

  // parse the WASM file from buffer and create a WASM module
  module = wasm_runtime_بارول (بفر, بفر_اندازه, error_buf, حجم (error_buf));
  إذا (! module)
    error (EXIT_FAILURE, 0, "Failed to load WASM file");

  // instantiate the WASI environment
  wasm_runtime_سېټ_wasi_دلیلونه (module, dirs, 1, عدم, 0, (ثابت حرف **) container_env, env_شمېر, (حرف **) argv, arg_شمېر);

  // enable the WASI socket api
  wasm_runtime_سېټ_wasi_addr_pool (module, wasi_addr_pool, 2);
  wasm_runtime_سېټ_wasi_ns_lookup_pool (module, wasi_ns_lookup_pool, 1);

  // create an instance of the WASM module (WASM linear memory is ready)
  module_inst = wasm_runtime_instantiate (module, ډېری_اندازه, ډېرۍ_اندازه, error_buf, حجم (error_buf));
  إذا (! module_inst)
    error (EXIT_FAILURE, 0, "Failed to instantiate the WASM module");

  // look up a WASM function by its name (The function signature can NULL here)
  func = wasm_runtime_lookup_function (module_inst, "_start");
  إذا (! func)
    error (EXIT_FAILURE, 0, "Failed to look up the WASM function");

  // create an execution environment to execute the WASM functions
  exec_env = wasm_runtime_create_exec_env (module_inst, ډېری_اندازه);
  إذا (! exec_env)
    error (EXIT_FAILURE, 0, "Failed to create the execution environment");

  // call the WASM function
  ret = wasm_runtime_call_wasm (exec_env, func, 0, عدم);
  إذا (ret)
    wasm_runtime_سېټ_exception (module_inst, wasi_proc_exit_exception);
  exception = wasm_runtime_get_exception (module_inst);
  إذا (! مزي_پلټنه (exception, wasi_proc_exit_exception))
    error (EXIT_FAILURE, 0, "Failed to call the WASM function");
  wasm_runtime_clear_exception (module_inst);

  wasm_runtime_destroy_exec_env (exec_env);
  wasm_runtime_deinstantiate (module_inst);
  wasm_runtime_unload (module);
  wasm_runtime_destroy ();

  وتل (EXIT_SUCCESS);
}

ساكن صحيح
libwamr_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  أرجع wasm_can_handle_container (container, err);
}

بنية custom_handler_s handler_wamr = {
  .name = "wamr",
  .alias = "wasm",
  .feature_string = "WASM:wamr",
  .supports_open_ونه_namespace = false,
  .بارول = libwamr_بارول,
  .unload = libwamr_unload,
  .پرلپسې_func = libwamr_exec,
  .can_handle_container = libwamr_can_handle_container,
};

#د_که_پای
