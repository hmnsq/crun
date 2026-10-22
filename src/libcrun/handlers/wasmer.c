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
#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश "../custom-handler.h"
#समावेश "../container.h"
#समावेश "../utils.h"
#समावेश "../linux.h"
#समावेश "handler-utils.h"
#समावेश <unistd.h>
#समावेश <sys/stat.h>
#समावेश <errno.h>
#समावेश <sys/types.h>
#समावेश <fcntl.h>
#समावेश <sched.h>

#अगर_घोषित HAVE_DLOPEN
#  include <dlfcn.h>
#पूर्ण_अगर

#अगर_घोषित HAVE_WASMER
#  include <wasmer.h>
#पूर्ण_अगर

#अगर HAVE_DLOPEN && HAVE_WASMER
#  define WASMER_BUF_आकार 128
अटल पूर्णांक
libwasmer_exec (व्योम *cookie, libcrun_container_t *container arg_unused,
                स्थिर अक्षर *pathname, अक्षर *स्थिर argv[])
{
  पूर्णांक ret;
  अक्षर बफर[WASMER_BUF_आकार] = { 0 };
  माप_प्रकार तथ्य_read_आकार = WASMER_BUF_आकार;
  स्थिर wasm_func_t *core_func;
  खाता *wat_wasm_file;
  माप_प्रकार file_आकार;
  wasm_byte_vec_t wat;
  wasm_byte_vec_t द्विआधारी_bytes;
  wasm_byte_vec_t wasm_bytes;
  wasm_engine_t *engine;
  wasm_val_t results_val[1] = { WASM_INIT_VAL };
  wasm_store_t *store;
  wasm_module_t *module;
  wasm_instance_t *instance;
  wasm_extern_vec_t exports;
  माप_प्रकार args_आकार = 0;
  cleanup_free अक्षर *wasi_args = शून्य;
  wasi_config_t *config;
  अक्षर *स्थिर *arg;
  wasi_env_t *wasi_env;
  wasm_importtype_vec_t import_types;
  wasm_extern_vec_t imports;
  wasm_func_t *run_func;
  wasm_val_vec_t args = WASM_EMPTY_VEC;
  wasm_val_vec_t res = WASM_EMPTY_VEC;

  wasm_engine_t *(*wasm_engine_new) ();
  व्योम (*wat2wasm) (स्थिर wasm_byte_vec_t *wat, wasm_byte_vec_t *out);
  wasm_module_t *(*wasm_module_new) (wasm_store_t *, स्थिर wasm_byte_vec_t *द्विआधारी);
  wasm_store_t *(*wasm_store_new) (wasm_engine_t *);
  wasm_instance_t *(*wasm_instance_new) (wasm_store_t *, स्थिर wasm_module_t *, स्थिर wasm_extern_vec_t *imports, wasm_trap_t **);
  व्योम (*wasm_instance_exports) (स्थिर wasm_instance_t *, wasm_extern_vec_t *out);
  wasm_func_t *(*wasm_extern_as_func) (wasm_extern_t *);
  व्योम (*wasm_module_delete) (wasm_module_t *);
  व्योम (*wasm_instance_delete) (wasm_instance_t *);
  व्योम (*wasm_store_delete) (wasm_store_t *);
  व्योम (*wasm_engine_delete) (wasm_engine_t *);
  व्योम (*wasm_byte_vec_new) (wasm_byte_vec_t *, माप_प्रकार, स्थिर अक्षर *);
  व्योम (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  व्योम (*wasm_importtype_vec_delete) (wasm_importtype_vec_t *);
  व्योम (*wasm_extern_vec_delete) (wasm_extern_vec_t *);
  व्योम (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, माप_प्रकार);
  व्योम (*wasm_extern_vec_new_uninitialized) (wasm_extern_vec_t *, माप_प्रकार);
  व्योम (*wasi_config_capture_stdout) (काष्ठा wasi_config_t *);
  व्योम (*wasi_config_inherit_stdout) (काष्ठा wasi_config_t *);
  व्योम (*wasm_module_imports) (स्थिर wasm_module_t *, wasm_importtype_vec_t *);
  व्योम (*wasm_func_delete) (wasm_func_t *);
  wasm_trap_t *(*wasm_func_call) (स्थिर wasm_func_t *, स्थिर wasm_val_vec_t *args, wasm_val_vec_t *results);
  wasi_config_t *(*wasi_config_new) (स्थिर अक्षर *);
  wasi_env_t *(*wasi_env_new) (wasm_store_t *, काष्ठा wasi_config_t *);
  bool (*wasi_get_imports) (स्थिर wasm_store_t *, स्थिर काष्ठा wasi_env_t *, स्थिर wasm_module_t *, wasm_extern_vec_t *);
  wasm_func_t *(*wasi_get_start_function) (wasm_instance_t *);
  intptr_t (*wasi_env_read_stdout) (काष्ठा wasi_env_t *, अक्षर *, uintptr_t);
  व्योम (*wasi_env_delete) (काष्ठा wasi_env_t *);
  व्योम (*wasi_config_arg) (काष्ठा wasi_config_t *config, स्थिर अक्षर *arg);
  bool (*wasi_env_initialize_instance) (काष्ठा wasi_env_t *, wasm_store_t *, wasm_instance_t *);

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

  अगर (wat2wasm == शून्य || wasm_module_delete == शून्य || wasm_instance_delete == शून्य
      || wasm_engine_delete == शून्य || wasm_store_delete == शून्य || wasm_func_call == शून्य
      || wasm_extern_as_func == शून्य || wasm_instance_exports == शून्य || wasm_instance_new == शून्य
      || wasm_store_new == शून्य || wasm_engine_new == शून्य || wasm_byte_vec_new == शून्य
      || wasm_byte_vec_delete == शून्य || wasm_extern_vec_delete == शून्य
      || wasm_byte_vec_new_uninitialized == शून्य || wasi_config_new == शून्य
      || wasi_config_capture_stdout == शून्य || wasi_env_new == शून्य || wasm_module_imports == शून्य
      || wasi_env_read_stdout == शून्य || wasi_env_delete == शून्य || wasm_func_delete == शून्य
      || wasm_importtype_vec_delete == शून्य || wasm_extern_vec_new_uninitialized == शून्य
      || wasi_get_imports == शून्य || wasi_get_start_function == शून्य || wasi_config_inherit_stdout == शून्य)
    error (निकास_त्रुटि, 0, "could not find symbol in `libwasmer.so`");

  wat_wasm_file = ख_खोलो (pathname, "rbe");

  अगर (! wat_wasm_file)
    error (निकास_त्रुटि, त्रुटि_सं, "error opening wat/wasm module");

  ख_जाओ (wat_wasm_file, 0L, अंत_से);
  file_आकार = ख_बताओ (wat_wasm_file);
  ख_जाओ (wat_wasm_file, 0L, शुरू_से);

  wasm_byte_vec_new_uninitialized (&द्विआधारी_bytes, file_आकार);

  अगर (ख_पढ़ो (द्विआधारी_bytes.तथ्य, file_आकार, 1, wat_wasm_file) != 1)
    error (निकास_त्रुटि, त्रुटि_सं, "error loading wat/wasm module");

  /* We can close entrypoint file.   */
  ख_बंद (wat_wasm_file);

  /* We have received a wat file: convert wat to wasm.   */
  अगर (has_suffix (pathname, "wat") > 0)
    {
      wat2wasm (&द्विआधारी_bytes, &wasm_bytes);
      द्विआधारी_bytes = wasm_bytes;
    }

  engine = wasm_engine_new ();
  store = wasm_store_new (engine);

  module = wasm_module_new (store, &द्विआधारी_bytes);

  अगर (! module)
    error (निकास_त्रुटि, 0, "error compiling wasm module");

  config = wasi_config_new ("crun_wasi_program");

  /* Count number of external arguments given.  */
  क्रम (arg = argv; *arg != शून्य; ++arg)
    args_आकार++;

  अगर (args_आकार > 1)
    {
      wasi_args = str_join_array (1, args_आकार, argv, " ");
      wasi_config_arg (config, wasi_args);
    }

  wasi_config_inherit_stdout (config);
  wasi_env = wasi_env_new (store, config);
  अगर (! wasi_env)
    {
      error (निकास_त्रुटि, 0, "error building wasi env");
    }

  /* Instantiate.  */
  अगर (! wasi_get_imports (store, wasi_env, module, &imports))
    error (निकास_त्रुटि, 0, "error getting WASI imports");

  instance = wasm_instance_new (store, module, &imports, शून्य);

  अगर (! instance)
    error (निकास_त्रुटि, 0, "error instantiating module");

  अगर (! wasi_env_initialize_instance (wasi_env, store, instance))
    error (निकास_त्रुटि, 0, "error init wasi env");

  /* Extract export.  */
  wasm_instance_exports (instance, &exports);
  अगर (exports.आकार == 0)
    error (निकास_त्रुटि, 0, "error getting instance exports");

  run_func = wasi_get_start_function (instance);
  अगर (run_func == शून्य)
    error (निकास_त्रुटि, 0, "error accessing export");

  अगर (wasm_func_call (run_func, &args, &res))
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

अटल पूर्णांक
libwasmer_load (व्योम **cookie, libcrun_error_t *err)
{
  व्योम *handle;

  handle = dlopen ("libwasmer.so", RTLD_NOW);
  अगर (handle == शून्य)
    वापस crun_make_error (err, 0, "could not load `libwasmer.so`: %s", dlerror ());
  *cookie = handle;

  वापस 0;
}

अटल पूर्णांक
libwasmer_unload (व्योम *cookie, libcrun_error_t *err)
{
  पूर्णांक r;

  अगर (cookie)
    {
      r = dlclose (cookie);
      अगर (UNLIKELY (r < 0))
        वापस crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  वापस 0;
}

अटल पूर्णांक
libwasmer_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  वापस wasm_can_handle_container (container, err);
}

काष्ठा custom_handler_s handler_wasmer = {
  .name = "wasmer",
  .alias = "wasm",
  .feature_string = "WASM:wasmer",
  .supports_open_वृक्ष_namespace = false,
  .load = libwasmer_load,
  .unload = libwasmer_unload,
  .run_func = libwasmer_exec,
  .can_handle_container = libwasmer_can_handle_container,
};

#पूर्ण_अगर
