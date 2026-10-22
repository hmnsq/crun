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

#अन्तर्भाव <config.h>
#अन्तर्भाव "../custom-handler.h"
#अन्तर्भाव "../container.h"
#अन्तर्भाव "../utils.h"
#अन्तर्भाव "../linux.h"
#अन्तर्भाव "handler-utils.h"
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <sched.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WAMR
#  include <wasm_export.h>
#endif

#जर HAVE_DLOPEN && HAVE_WAMR

स्थिर पूर्णांक
libwamr_load (शून्य **cookie, libcrun_error_t *err)
{
  शून्य *handle;

  handle = dlopen ("libiwasm.so", RTLD_NOW);
  जर (handle == रिक्त)
    परत crun_make_error (err, 0, "could not load `libiwasm.so`: `%s`", dlerror ());
  *cookie = handle;

  परत 0;
}

स्थिर पूर्णांक
libwamr_unload (शून्य *cookie, libcrun_error_t *err)
{
  पूर्णांक r;

  जर (cookie)
    {
      r = dlclose (cookie);
      जर (UNLIKELY (r < 0))
        परत crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }
  परत 0;
}

स्थिर पूर्णांक
libwamr_exec (शून्य *cookie, __attribute__ ((unused)) libcrun_container_t *container, स्थिरांक अक्षर *pathname, अक्षर *स्थिरांक argv[])
{
  // load symbols from the shared library libiwasm.so
  bool (*wasm_runtime_init) ();
  RuntimeInitArgs init_args;
  bool (*wasm_runtime_full_init) (RuntimeInitArgs *init_args);
  wasm_module_t module;
  wasm_module_t (*wasm_runtime_load) (uint8_t *buf, uint32_t size, अक्षर *error_buf, uint32_t error_buf_size);
  wasm_module_inst_t module_inst;
  wasm_module_inst_t (*wasm_runtime_instantiate) (स्थिरांक wasm_module_t module, uint32_t default_चळत_size, uint32_t host_managed_heap_size, अक्षर *error_buf, uint32_t error_buf_size);
  wasm_function_inst_t func;
  wasm_function_inst_t (*wasm_runtime_lookup_function) (wasm_module_inst_t स्थिरांक module_inst, स्थिरांक अक्षर *name);
  wasm_exec_env_t exec_env;
  wasm_exec_env_t (*wasm_runtime_create_exec_env) (wasm_module_inst_t module_inst, uint32_t चळत_size);
  bool (*wasm_runtime_call_wasm) (wasm_exec_env_t exec_env, wasm_function_inst_t function, uint32_t argc, uint32_t argv[]);
  स्थिरांक अक्षर *(*wasm_runtime_get_exception) (wasm_module_inst_t module_inst);
  शून्य (*wasm_runtime_set_exception) (wasm_module_inst_t module_inst, स्थिरांक अक्षर *exception);
  शून्य (*wasm_runtime_clear_exception) (wasm_module_inst_t module_inst);
  शून्य (*wasm_runtime_destroy_exec_env) (wasm_exec_env_t exec_env);
  शून्य (*wasm_runtime_deinstantiate) (wasm_module_inst_t module_inst);
  शून्य (*wasm_runtime_unload) (wasm_module_t module);
  शून्य (*wasm_runtime_destroy) ();
  uint32_t (*wasm_runtime_get_wasi_exit_code) (wasm_module_inst_t module_inst);
  bool (*wasm_application_execute_main) (wasm_module_inst_t module_inst, int32_t argc, अक्षर *argv[]);
  शून्य (*wasm_runtime_set_wasi_args) (wasm_module_t module, स्थिरांक अक्षर *dir_list[], uint32_t dir_मोजणी, स्थिरांक अक्षर *map_dir_list[], uint32_t map_dir_मोजणी, स्थिरांक अक्षर *env[], uint32_t env_मोजणी, अक्षर *argv[], पूर्णांक argc);
  शून्य (*wasm_runtime_set_wasi_addr_pool) (wasm_module_t module, स्थिरांक अक्षर *addr_pool[], uint32_t pool_size);
  शून्य (*wasm_runtime_set_wasi_ns_lookup_pool) (wasm_module_t module, स्थिरांक अक्षर *ns_lookup_pool[], uint32_t pool_size);

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

  जर (wasm_runtime_init == रिक्त)
    error (निकास_त्रुटि, 0, "could not find wasm_runtime_init symbol in `libiwasm.so`");
  जर (wasm_runtime_full_init == रिक्त)
    error (निकास_त्रुटि, 0, "could not find wasm_runtime_full_init symbol in `libiwasm.so`");
  जर (wasm_runtime_load == रिक्त)
    error (निकास_त्रुटि, 0, "could not find wasm_runtime_load symbol in `libiwasm.so`");
  जर (wasm_runtime_instantiate == रिक्त)
    error (निकास_त्रुटि, 0, "could not find wasm_runtime_instantiate symbol in `libiwasm.so`");
  जर (wasm_runtime_lookup_function == रिक्त)
    error (निकास_त्रुटि, 0, "could not find wasm_runtime_lookup_function symbol in `libiwasm.so`");
  जर (wasm_runtime_create_exec_env == रिक्त)
    error (निकास_त्रुटि, 0, "could not find wasm_runtime_create_exec_env symbol in `libiwasm.so`");
  जर (wasm_runtime_call_wasm == रिक्त)
    error (निकास_त्रुटि, 0, "could not find wasm_runtime_call_wasm symbol in `libiwasm.so`");
  जर (wasm_runtime_get_exception == रिक्त)
    error (निकास_त्रुटि, 0, "could not find wasm_runtime_get_exception symbol in `libiwasm.so`");
  जर (wasm_runtime_set_exception == रिक्त)
    error (निकास_त्रुटि, 0, "could not find wasm_runtime_set_exception symbol in `libiwasm.so`");
  जर (wasm_runtime_clear_exception == रिक्त)
    error (निकास_त्रुटि, 0, "could not find wasm_runtime_clear_exception symbol in `libiwasm.so`");
  जर (wasm_runtime_destroy_exec_env == रिक्त)
    error (निकास_त्रुटि, 0, "could not find wasm_runtime_destroy_exec_env symbol in `libiwasm.so`");
  जर (wasm_runtime_deinstantiate == रिक्त)
    error (निकास_त्रुटि, 0, "could not find wasm_runtime_deinstantiate symbol in `libiwasm.so`");
  जर (wasm_runtime_unload == रिक्त)
    error (निकास_त्रुटि, 0, "could not find wasm_runtime_unload symbol in `libiwasm.so`");
  जर (wasm_runtime_destroy == रिक्त)
    error (निकास_त्रुटि, 0, "could not find wasm_runtime_destroy symbol in `libiwasm.so`");
  जर (wasm_runtime_get_wasi_exit_code == रिक्त)
    error (निकास_त्रुटि, 0, "could not find wasm_runtime_get_wasi_exit_code symbol in `libiwasm.so`");
  जर (wasm_application_execute_main == रिक्त)
    error (निकास_त्रुटि, 0, "could not find wasm_application_execute_main symbol in `libiwasm.so`");
  जर (wasm_runtime_set_wasi_args == रिक्त)
    error (निकास_त्रुटि, 0, "could not find wasm_runtime_set_wasi_args symbol in `libiwasm.so`");
  जर (wasm_runtime_set_wasi_addr_pool == रिक्त)
    error (निकास_त्रुटि, 0, "could not find wasm_runtime_set_wasi_addr_pool symbol in `libiwasm.so`");
  जर (wasm_runtime_set_wasi_ns_lookup_pool == रिक्त)
    error (निकास_त्रुटि, 0, "could not find wasm_runtime_set_wasi_ns_lookup_pool symbol in `libiwasm.so`");

  पूर्णांक ret;
  स्थिरांक अक्षर *exception;
  cleanup_free अक्षर *buffer = रिक्त;
  अक्षर error_buf[128];
  माप_प्रकार buffer_size;
  uint32_t चळत_size = 8096, heap_size = 0;
  libcrun_error_t tmp_err = रिक्त;
  स्थिरांक अक्षर *wasi_proc_exit_exception = "wasi proc exit";
  स्थिरांक अक्षर *wasi_addr_pool[2] = { "0.0.0.0/0", "::/0" };
  स्थिरांक अक्षर *wasi_ns_lookup_pool[1] = { "*" };

  स्थिरांक अक्षर *dirs[2] = { "/", "." };
  अक्षर **container_env = container->container_def->process->env;
  माप_प्रकार env_मोजणी = container->container_def->process->env_len;

  पूर्णांक arg_मोजणी = 0;
  अक्षर *स्थिरांक *arg;
  साठी (arg = argv; *arg != रिक्त; ++arg)
    arg_मोजणी++;

  // initialize the wasm runtime by default configurations
  जर (! wasm_runtime_init ())
    error (निकास_त्रुटि, 0, "Failed to initialize the wasm runtime");

  // read WASM file into a memory buffer
  ret = read_all_file (pathname, &buffer, &buffer_size, &tmp_err);
  जर (UNLIKELY (ret < 0))
    {
      crun_error_release (&tmp_err);
      error (निकास_त्रुटि, 0, "Failed to read file");
    }

  जर (UNLIKELY (buffer_size > UINT32_MAX))
    error (निकास_त्रुटि, 0, "File size is too large");

  // parse the WASM file from buffer and create a WASM module
  module = wasm_runtime_load (buffer, buffer_size, error_buf, आकार (error_buf));
  जर (! module)
    error (निकास_त्रुटि, 0, "Failed to load WASM file");

  // instantiate the WASI environment
  wasm_runtime_set_wasi_args (module, dirs, 1, रिक्त, 0, (स्थिरांक अक्षर **) container_env, env_मोजणी, (अक्षर **) argv, arg_मोजणी);

  // enable the WASI socket api
  wasm_runtime_set_wasi_addr_pool (module, wasi_addr_pool, 2);
  wasm_runtime_set_wasi_ns_lookup_pool (module, wasi_ns_lookup_pool, 1);

  // create an instance of the WASM module (WASM linear memory is ready)
  module_inst = wasm_runtime_instantiate (module, चळत_size, heap_size, error_buf, आकार (error_buf));
  जर (! module_inst)
    error (निकास_त्रुटि, 0, "Failed to instantiate the WASM module");

  // look up a WASM function by its name (The function signature can NULL here)
  func = wasm_runtime_lookup_function (module_inst, "_start");
  जर (! func)
    error (निकास_त्रुटि, 0, "Failed to look up the WASM function");

  // create an execution environment to execute the WASM functions
  exec_env = wasm_runtime_create_exec_env (module_inst, चळत_size);
  जर (! exec_env)
    error (निकास_त्रुटि, 0, "Failed to create the execution environment");

  // call the WASM function
  ret = wasm_runtime_call_wasm (exec_env, func, 0, रिक्त);
  जर (ret)
    wasm_runtime_set_exception (module_inst, wasi_proc_exit_exception);
  exception = wasm_runtime_get_exception (module_inst);
  जर (! म_माला (exception, wasi_proc_exit_exception))
    error (निकास_त्रुटि, 0, "Failed to call the WASM function");
  wasm_runtime_clear_exception (module_inst);

  wasm_runtime_destroy_exec_env (exec_env);
  wasm_runtime_deinstantiate (module_inst);
  wasm_runtime_unload (module);
  wasm_runtime_destroy ();

  निकास (निकास_सफल);
}

स्थिर पूर्णांक
libwamr_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  परत wasm_can_handle_container (container, err);
}

संरचना custom_handler_s handler_wamr = {
  .name = "wamr",
  .alias = "wasm",
  .feature_string = "WASM:wamr",
  .supports_open_वृक्ष_namespace = false,
  .load = libwamr_load,
  .unload = libwamr_unload,
  .run_func = libwamr_exec,
  .can_handle_container = libwamr_can_handle_container,
};

#endif
