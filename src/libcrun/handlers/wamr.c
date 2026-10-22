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
#تعریف_کرو GNU_ماخذ

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

#اگر_تعریف_شدہ HAVE_DLOPEN
#  include <dlfcn.h>
#اگر_ختم

#اگر_تعریف_شدہ HAVE_WAMR
#  include <wasm_export.h>
#اگر_ختم

#اگر HAVE_DLOPEN && HAVE_WAMR

ساکن صحیح
libwamr_بھرو (خالی **cookie, libcrun_error_t *err)
{
  خالی *handle;

  handle = dlopen ("libiwasm.so", RTLD_NOW);
  اگر (handle == تہی)
    واپس crun_make_error (err, 0, "could not load `libiwasm.so`: `%s`", dlerror ());
  *cookie = handle;

  واپس 0;
}

ساکن صحیح
libwamr_unload (خالی *cookie, libcrun_error_t *err)
{
  صحیح r;

  اگر (cookie)
    {
      r = dlclose (cookie);
      اگر (UNLIKELY (r < 0))
        واپس crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  واپس 0;
}

ساکن صحیح
libwamr_exec (خالی *cookie, __attribute__ ((غیر_مستعمل)) libcrun_container_t *container, ثابت حرف *pathname, حرف *ثابت argv[])
{
  // load symbols from the shared library libiwasm.so
  bool (*wasm_runtime_init) ();
  Runtime_Init_دلائل init_دلائل;
  bool (*wasm_runtime_بھرا_init) (Runtime_Init_دلائل *init_دلائل);
  wasm_module_t module;
  wasm_module_t (*wasm_runtime_بھرو) (uint8_t *buf, uint32_t سائز, حرف *error_buf, uint32_t error_buf_سائز);
  wasm_module_inst_t module_inst;
  wasm_module_inst_t (*wasm_runtime_instantiate) (ثابت wasm_module_t module, uint32_t default_ڈھیر_سائز, uint32_t host_managed_ڈھیر_سائز, حرف *error_buf, uint32_t error_buf_سائز);
  wasm_function_inst_t func;
  wasm_function_inst_t (*wasm_runtime_lookup_function) (wasm_module_inst_t ثابت module_inst, ثابت حرف *name);
  wasm_exec_env_t exec_env;
  wasm_exec_env_t (*wasm_runtime_create_exec_env) (wasm_module_inst_t module_inst, uint32_t ڈھیر_سائز);
  bool (*wasm_runtime_call_wasm) (wasm_exec_env_t exec_env, wasm_function_inst_t function, uint32_t argc, uint32_t argv[]);
  ثابت حرف *(*wasm_runtime_get_exception) (wasm_module_inst_t module_inst);
  خالی (*wasm_runtime_سیٹ_exception) (wasm_module_inst_t module_inst, ثابت حرف *exception);
  خالی (*wasm_runtime_clear_exception) (wasm_module_inst_t module_inst);
  خالی (*wasm_runtime_destroy_exec_env) (wasm_exec_env_t exec_env);
  خالی (*wasm_runtime_deinstantiate) (wasm_module_inst_t module_inst);
  خالی (*wasm_runtime_unload) (wasm_module_t module);
  خالی (*wasm_runtime_destroy) ();
  uint32_t (*wasm_runtime_get_wasi_exit_رمز) (wasm_module_inst_t module_inst);
  bool (*wasm_application_execute_main) (wasm_module_inst_t module_inst, int32_t argc, حرف *argv[]);
  خالی (*wasm_runtime_سیٹ_wasi_دلائل) (wasm_module_t module, ثابت حرف *dir_list[], uint32_t dir_گنتی, ثابت حرف *map_dir_list[], uint32_t map_dir_گنتی, ثابت حرف *env[], uint32_t env_گنتی, حرف *argv[], صحیح argc);
  خالی (*wasm_runtime_سیٹ_wasi_addr_pool) (wasm_module_t module, ثابت حرف *addr_pool[], uint32_t pool_سائز);
  خالی (*wasm_runtime_سیٹ_wasi_ns_lookup_pool) (wasm_module_t module, ثابت حرف *ns_lookup_pool[], uint32_t pool_سائز);

  wasm_runtime_init = dlsym (cookie, "wasm_runtime_init");
  wasm_runtime_بھرا_init = dlsym (cookie, "wasm_runtime_full_init");
  wasm_runtime_بھرو = dlsym (cookie, "wasm_runtime_load");
  wasm_runtime_instantiate = dlsym (cookie, "wasm_runtime_instantiate");
  wasm_runtime_lookup_function = dlsym (cookie, "wasm_runtime_lookup_function");
  wasm_runtime_create_exec_env = dlsym (cookie, "wasm_runtime_create_exec_env");
  wasm_runtime_call_wasm = dlsym (cookie, "wasm_runtime_call_wasm");
  wasm_runtime_get_exception = dlsym (cookie, "wasm_runtime_get_exception");
  wasm_runtime_سیٹ_exception = dlsym (cookie, "wasm_runtime_set_exception");
  wasm_runtime_clear_exception = dlsym (cookie, "wasm_runtime_clear_exception");
  wasm_runtime_destroy_exec_env = dlsym (cookie, "wasm_runtime_destroy_exec_env");
  wasm_runtime_deinstantiate = dlsym (cookie, "wasm_runtime_deinstantiate");
  wasm_runtime_unload = dlsym (cookie, "wasm_runtime_unload");
  wasm_runtime_destroy = dlsym (cookie, "wasm_runtime_destroy");
  wasm_runtime_get_wasi_exit_رمز = dlsym (cookie, "wasm_runtime_get_wasi_exit_code");
  wasm_application_execute_main = dlsym (cookie, "wasm_application_execute_main");
  wasm_runtime_سیٹ_wasi_دلائل = dlsym (cookie, "wasm_runtime_set_wasi_args");
  wasm_runtime_سیٹ_wasi_addr_pool = dlsym (cookie, "wasm_runtime_set_wasi_addr_pool");
  wasm_runtime_سیٹ_wasi_ns_lookup_pool = dlsym (cookie, "wasm_runtime_set_wasi_ns_lookup_pool");

  اگر (wasm_runtime_init == تہی)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_init symbol in `libiwasm.so`");
  اگر (wasm_runtime_بھرا_init == تہی)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_full_init symbol in `libiwasm.so`");
  اگر (wasm_runtime_بھرو == تہی)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_load symbol in `libiwasm.so`");
  اگر (wasm_runtime_instantiate == تہی)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_instantiate symbol in `libiwasm.so`");
  اگر (wasm_runtime_lookup_function == تہی)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_lookup_function symbol in `libiwasm.so`");
  اگر (wasm_runtime_create_exec_env == تہی)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_create_exec_env symbol in `libiwasm.so`");
  اگر (wasm_runtime_call_wasm == تہی)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_call_wasm symbol in `libiwasm.so`");
  اگر (wasm_runtime_get_exception == تہی)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_exception symbol in `libiwasm.so`");
  اگر (wasm_runtime_سیٹ_exception == تہی)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_exception symbol in `libiwasm.so`");
  اگر (wasm_runtime_clear_exception == تہی)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_clear_exception symbol in `libiwasm.so`");
  اگر (wasm_runtime_destroy_exec_env == تہی)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy_exec_env symbol in `libiwasm.so`");
  اگر (wasm_runtime_deinstantiate == تہی)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_deinstantiate symbol in `libiwasm.so`");
  اگر (wasm_runtime_unload == تہی)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_unload symbol in `libiwasm.so`");
  اگر (wasm_runtime_destroy == تہی)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_destroy symbol in `libiwasm.so`");
  اگر (wasm_runtime_get_wasi_exit_رمز == تہی)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_get_wasi_exit_code symbol in `libiwasm.so`");
  اگر (wasm_application_execute_main == تہی)
    error (EXIT_FAILURE, 0, "could not find wasm_application_execute_main symbol in `libiwasm.so`");
  اگر (wasm_runtime_سیٹ_wasi_دلائل == تہی)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_args symbol in `libiwasm.so`");
  اگر (wasm_runtime_سیٹ_wasi_addr_pool == تہی)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_addr_pool symbol in `libiwasm.so`");
  اگر (wasm_runtime_سیٹ_wasi_ns_lookup_pool == تہی)
    error (EXIT_FAILURE, 0, "could not find wasm_runtime_set_wasi_ns_lookup_pool symbol in `libiwasm.so`");

  صحیح ret;
  ثابت حرف *exception;
  cleanup_free حرف *بفر = تہی;
  حرف error_buf[128];
  size_t بفر_سائز;
  uint32_t ڈھیر_سائز = 8096, ڈھیر_سائز_2 = 0;
  libcrun_error_t tmp_err = تہی;
  ثابت حرف *wasi_proc_exit_exception = "wasi proc exit";
  ثابت حرف *wasi_addr_pool[2] = { "0.0.0.0/0", "::/0" };
  ثابت حرف *wasi_ns_lookup_pool[1] = { "*" };

  ثابت حرف *dirs[2] = { "/", "." };
  حرف **container_env = container->container_def->process->env;
  size_t env_گنتی = container->container_def->process->env_len;

  صحیح arg_گنتی = 0;
  حرف *ثابت *arg;
  برائے (arg = argv; *arg != تہی; ++arg)
    arg_گنتی++;

  // initialize the wasm runtime by default configurations
  اگر (! wasm_runtime_init ())
    error (EXIT_FAILURE, 0, "Failed to initialize the wasm runtime");

  // read WASM file into a memory buffer
  ret = read_all_file (pathname, &بفر, &بفر_سائز, &tmp_err);
  اگر (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      error (EXIT_FAILURE, 0, "Failed to read file");
    }

  اگر (UNLIKELY (بفر_سائز > UINT32_MAX))
    error (EXIT_FAILURE, 0, "File size is too large");

  // parse the WASM file from buffer and create a WASM module
  module = wasm_runtime_بھرو (بفر, بفر_سائز, error_buf, حجم (error_buf));
  اگر (! module)
    error (EXIT_FAILURE, 0, "Failed to load WASM file");

  // instantiate the WASI environment
  wasm_runtime_سیٹ_wasi_دلائل (module, dirs, 1, تہی, 0, (ثابت حرف **) container_env, env_گنتی, (حرف **) argv, arg_گنتی);

  // enable the WASI socket api
  wasm_runtime_سیٹ_wasi_addr_pool (module, wasi_addr_pool, 2);
  wasm_runtime_سیٹ_wasi_ns_lookup_pool (module, wasi_ns_lookup_pool, 1);

  // create an instance of the WASM module (WASM linear memory is ready)
  module_inst = wasm_runtime_instantiate (module, ڈھیر_سائز, ڈھیر_سائز_2, error_buf, حجم (error_buf));
  اگر (! module_inst)
    error (EXIT_FAILURE, 0, "Failed to instantiate the WASM module");

  // look up a WASM function by its name (The function signature can NULL here)
  func = wasm_runtime_lookup_function (module_inst, "_start");
  اگر (! func)
    error (EXIT_FAILURE, 0, "Failed to look up the WASM function");

  // create an execution environment to execute the WASM functions
  exec_env = wasm_runtime_create_exec_env (module_inst, ڈھیر_سائز);
  اگر (! exec_env)
    error (EXIT_FAILURE, 0, "Failed to create the execution environment");

  // call the WASM function
  ret = wasm_runtime_call_wasm (exec_env, func, 0, تہی);
  اگر (ret)
    wasm_runtime_سیٹ_exception (module_inst, wasi_proc_exit_exception);
  exception = wasm_runtime_get_exception (module_inst);
  اگر (! لڑی_تلاش (exception, wasi_proc_exit_exception))
    error (EXIT_FAILURE, 0, "Failed to call the WASM function");
  wasm_runtime_clear_exception (module_inst);

  wasm_runtime_destroy_exec_env (exec_env);
  wasm_runtime_deinstantiate (module_inst);
  wasm_runtime_unload (module);
  wasm_runtime_destroy ();

  باہر_نکلو (EXIT_SUCCESS);
}

ساکن صحیح
libwamr_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  واپس wasm_can_handle_container (container, err);
}

ساخت custom_handler_s handler_wamr = {
  .name = "wamr",
  .alias = "wasm",
  .feature_string = "WASM:wamr",
  .supports_open_درخت_namespace = false,
  .بھرو = libwamr_بھرو,
  .unload = libwamr_unload,
  .تسلسل_func = libwamr_exec,
  .can_handle_container = libwamr_can_handle_container,
};

#اگر_ختم
