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

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMER
#  include <wasmer.h>
#endif

#यदि HAVE_DLOPEN && HAVE_WASMER
#  define WASMER_BUF_SIZE 128
स्थिर पूर्णाङ्क
libwasmer_exec (शून्य *cookie, libcrun_container_t *container arg_unused,
                अचर वर्ण *pathname, वर्ण *अचर argv[])
{
  पूर्णाङ्क ret;
  वर्ण buffer[WASMER_BUF_SIZE] = { 0 };
  माप_प्रकार data_read_size = WASMER_BUF_SIZE;
  अचर wasm_func_t *core_func;
  खाता *wat_wasm_file;
  माप_प्रकार file_size;
  wasm_byte_vec_t wat;
  wasm_byte_vec_t द्विआधारी_bytes;
  wasm_byte_vec_t wasm_bytes;
  wasm_engine_t *engine;
  wasm_val_t results_val[1] = { WASM_INIT_VAL };
  wasm_store_t *store;
  wasm_module_t *module;
  wasm_instance_t *instance;
  wasm_extern_vec_t exports;
  माप_प्रकार args_size = 0;
  cleanup_free वर्ण *wasi_args = रिक्त;
  wasi_config_t *config;
  वर्ण *अचर *arg;
  wasi_env_t *wasi_env;
  wasm_importtype_vec_t import_types;
  wasm_extern_vec_t imports;
  wasm_func_t *run_func;
  wasm_val_vec_t args = WASM_EMPTY_VEC;
  wasm_val_vec_t res = WASM_EMPTY_VEC;

  wasm_engine_t *(*wasm_engine_new) ();
  शून्य (*wat2wasm) (अचर wasm_byte_vec_t *wat, wasm_byte_vec_t *out);
  wasm_module_t *(*wasm_module_new) (wasm_store_t *, अचर wasm_byte_vec_t *द्विआधारी);
  wasm_store_t *(*wasm_store_new) (wasm_engine_t *);
  wasm_instance_t *(*wasm_instance_new) (wasm_store_t *, अचर wasm_module_t *, अचर wasm_extern_vec_t *imports, wasm_trap_t **);
  शून्य (*wasm_instance_exports) (अचर wasm_instance_t *, wasm_extern_vec_t *out);
  wasm_func_t *(*wasm_extern_as_func) (wasm_extern_t *);
  शून्य (*wasm_module_delete) (wasm_module_t *);
  शून्य (*wasm_instance_delete) (wasm_instance_t *);
  शून्य (*wasm_store_delete) (wasm_store_t *);
  शून्य (*wasm_engine_delete) (wasm_engine_t *);
  शून्य (*wasm_byte_vec_new) (wasm_byte_vec_t *, माप_प्रकार, अचर वर्ण *);
  शून्य (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  शून्य (*wasm_importtype_vec_delete) (wasm_importtype_vec_t *);
  शून्य (*wasm_extern_vec_delete) (wasm_extern_vec_t *);
  शून्य (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, माप_प्रकार);
  शून्य (*wasm_extern_vec_new_uninitialized) (wasm_extern_vec_t *, माप_प्रकार);
  शून्य (*wasi_config_capture_stdout) (संरचना wasi_config_t *);
  शून्य (*wasi_config_inherit_stdout) (संरचना wasi_config_t *);
  शून्य (*wasm_module_imports) (अचर wasm_module_t *, wasm_importtype_vec_t *);
  शून्य (*wasm_func_delete) (wasm_func_t *);
  wasm_trap_t *(*wasm_func_call) (अचर wasm_func_t *, अचर wasm_val_vec_t *args, wasm_val_vec_t *results);
  wasi_config_t *(*wasi_config_new) (अचर वर्ण *);
  wasi_env_t *(*wasi_env_new) (wasm_store_t *, संरचना wasi_config_t *);
  bool (*wasi_get_imports) (अचर wasm_store_t *, अचर संरचना wasi_env_t *, अचर wasm_module_t *, wasm_extern_vec_t *);
  wasm_func_t *(*wasi_get_start_function) (wasm_instance_t *);
  intptr_t (*wasi_env_read_stdout) (संरचना wasi_env_t *, वर्ण *, uintptr_t);
  शून्य (*wasi_env_delete) (संरचना wasi_env_t *);
  शून्य (*wasi_config_arg) (संरचना wasi_config_t *config, अचर वर्ण *arg);
  bool (*wasi_env_initialize_instance) (संरचना wasi_env_t *, wasm_store_t *, wasm_instance_t *);

  wat2wasm = dlsym (cookie, "wat2wasm");
  wasm_module_delete = dlsym (cookie, "wasm_module_delete");
  wasm_instance_delete = dlsym (cookie, "wasm_instance_delete");
  wasm_engine_delete = dlsym (cookie, "wasm_engine_delete");
  wasm_store_delete = dlsym (cookie, "wasm_store_delete");
  wasm_func_call = dlsym (cookie, "wasm_func_call");
  wasm_extern_as_func = dlsym (cookie, "wasm_extern_as_func");
  wasm_instance_exports = dlsym (cookie, "wasm_instance_exports");
  wasm_instance_new = dlsym (cookie, "wasm_instance_new");
  wasm_store_new = dlsym (cookie, "wasm_store_new");
  wasm_module_new = dlsym (cookie, "wasm_module_new");
  wasm_engine_new = dlsym (cookie, "wasm_engine_new");
  wasm_byte_vec_new = dlsym (cookie, "wasm_byte_vec_new");
  wasm_byte_vec_delete = dlsym (cookie, "wasm_byte_vec_delete");
  wasm_extern_vec_delete = dlsym (cookie, "wasm_extern_vec_delete");
  wasm_importtype_vec_delete = dlsym (cookie, "wasm_importtype_vec_delete");
  wasm_byte_vec_new_uninitialized = dlsym (cookie, "wasm_byte_vec_new_uninitialized");
  wasi_config_new = dlsym (cookie, "wasi_config_new");
  wasi_config_arg = dlsym (cookie, "wasi_config_arg");
  wasi_config_capture_stdout = dlsym (cookie, "wasi_config_capture_stdout");
  wasi_config_inherit_stdout = dlsym (cookie, "wasi_config_inherit_stdout");
  wasi_env_new = dlsym (cookie, "wasi_env_new");
  wasm_module_imports = dlsym (cookie, "wasm_module_imports");
  wasm_extern_vec_new_uninitialized = dlsym (cookie, "wasm_extern_vec_new_uninitialized");
  wasi_get_imports = dlsym (cookie, "wasi_get_imports");
  wasi_get_start_function = dlsym (cookie, "wasi_get_start_function");
  wasi_env_read_stdout = dlsym (cookie, "wasi_env_read_stdout");
  wasi_env_delete = dlsym (cookie, "wasi_env_delete");
  wasm_func_delete = dlsym (cookie, "wasm_func_delete");
  wasi_env_initialize_instance = dlsym (cookie, "wasi_env_initialize_instance");

  यदि (wat2wasm == रिक्त || wasm_module_delete == रिक्त || wasm_instance_delete == रिक्त
      || wasm_engine_delete == रिक्त || wasm_store_delete == रिक्त || wasm_func_call == रिक्त
      || wasm_extern_as_func == रिक्त || wasm_instance_exports == रिक्त || wasm_instance_new == रिक्त
      || wasm_store_new == रिक्त || wasm_engine_new == रिक्त || wasm_byte_vec_new == रिक्त
      || wasm_byte_vec_delete == रिक्त || wasm_extern_vec_delete == रिक्त
      || wasm_byte_vec_new_uninitialized == रिक्त || wasi_config_new == रिक्त
      || wasi_config_capture_stdout == रिक्त || wasi_env_new == रिक्त || wasm_module_imports == रिक्त
      || wasi_env_read_stdout == रिक्त || wasi_env_delete == रिक्त || wasm_func_delete == रिक्त
      || wasm_importtype_vec_delete == रिक्त || wasm_extern_vec_new_uninitialized == रिक्त
      || wasi_get_imports == रिक्त || wasi_get_start_function == रिक्त || wasi_config_inherit_stdout == रिक्त)
    error (निकास_त्रुटि, 0, "could not find symbol in `libwasmer.so`");

  wat_wasm_file = ख_खोलो (pathname, "rbe");

  यदि (! wat_wasm_file)
    error (निकास_त्रुटि, त्रुटि_सं, "error opening wat/wasm module");

  ख_जाओ (wat_wasm_file, 0L, अंत_से);
  file_size = ख_बताओ (wat_wasm_file);
  ख_जाओ (wat_wasm_file, 0L, शुरू_से);

  wasm_byte_vec_new_uninitialized (&द्विआधारी_bytes, file_size);

  यदि (ख_पढ़ो (द्विआधारी_bytes.data, file_size, 1, wat_wasm_file) != 1)
    error (निकास_त्रुटि, त्रुटि_सं, "error loading wat/wasm module");

  /* We can close entrypoint file.   */
  ख_बंद (wat_wasm_file);

  /* We have received a wat file: convert wat to wasm.   */
  यदि (has_suffix (pathname, "wat") > 0)
    {
      wat2wasm (&द्विआधारी_bytes, &wasm_bytes);
      द्विआधारी_bytes = wasm_bytes;
    }

  engine = wasm_engine_new ();
  store = wasm_store_new (engine);

  module = wasm_module_new (store, &द्विआधारी_bytes);

  यदि (! module)
    error (निकास_त्रुटि, 0, "error compiling wasm module");

  config = wasi_config_new ("crun_wasi_program");

  /* Count number of external arguments given.  */
  लागि (arg = argv; *arg != रिक्त; ++arg)
    args_size++;

  यदि (args_size > 1)
    {
      wasi_args = str_join_array (1, args_size, argv, " ");
      wasi_config_arg (config, wasi_args);
    }

  wasi_config_inherit_stdout (config);
  wasi_env = wasi_env_new (store, config);
  यदि (! wasi_env)
    {
      error (निकास_त्रुटि, 0, "error building wasi env");
    }

  /* Instantiate.  */
  यदि (! wasi_get_imports (store, wasi_env, module, &imports))
    error (निकास_त्रुटि, 0, "error getting WASI imports");

  instance = wasm_instance_new (store, module, &imports, रिक्त);

  यदि (! instance)
    error (निकास_त्रुटि, 0, "error instantiating module");

  यदि (! wasi_env_initialize_instance (wasi_env, store, instance))
    error (निकास_त्रुटि, 0, "error init wasi env");

  /* Extract export.  */
  wasm_instance_exports (instance, &exports);
  यदि (exports.size == 0)
    error (निकास_त्रुटि, 0, "error getting instance exports");

  run_func = wasi_get_start_function (instance);
  यदि (run_func == रिक्त)
    error (निकास_त्रुटि, 0, "error accessing export");

  यदि (wasm_func_call (run_func, &args, &res))
    error (निकास_त्रुटि, 0, "error calling wasm function");

  wasm_extern_vec_delete (&exports);
  wasm_extern_vec_delete (&imports);

  /* Shut down.  */
  wasm_module_delete (module);
  wasm_instance_delete (instance);
  wasm_func_delete (run_func);
  wasi_env_delete (wasi_env);
  wasm_store_delete (store);
  wasm_engine_delete (engine);

  निकास (निकास_सफल);
}

स्थिर पूर्णाङ्क
libwasmer_load (शून्य **cookie, libcrun_error_t *err)
{
  शून्य *handle;

  handle = dlopen ("libwasmer.so", RTLD_NOW);
  यदि (handle == रिक्त)
    फर्काऊ crun_make_error (err, 0, "could not load `libwasmer.so`: %s", dlerror ());
  *cookie = handle;

  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
libwasmer_unload (शून्य *cookie, libcrun_error_t *err)
{
  पूर्णाङ्क r;

  यदि (cookie)
    {
      r = dlclose (cookie);
      यदि (UNLIKELY (r < 0))
        फर्काऊ crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
libwasmer_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  फर्काऊ wasm_can_handle_container (container, err);
}

संरचना custom_handler_s handler_wasmer = {
  .name = "wasmer",
  .alias = "wasm",
  .feature_string = "WASM:wasmer",
  .supports_open_रूख_namespace = false,
  .load = libwasmer_load,
  .unload = libwasmer_unload,
  .run_func = libwasmer_exec,
  .can_handle_container = libwasmer_can_handle_container,
};

#endif
