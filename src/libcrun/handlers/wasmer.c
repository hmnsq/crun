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

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMER
#  include <wasmer.h>
#endif

#اگر HAVE_DLOPEN && HAVE_WASMER
#  define WASMER_BUF_SIZE 128
ایستا صحیح
libwasmer_exec (تهی *cookie, libcrun_container_t *container arg_unused,
                ثابت نویسه *pathname, نویسه *ثابت argv[])
{
  صحیح ret;
  نویسه buffer[WASMER_BUF_SIZE] = { 0 };
  size_t data_read_size = WASMER_BUF_SIZE;
  ثابت wasm_func_t *core_func;
  FILE *wat_wasm_file;
  size_t file_size;
  wasm_byte_vec_t wat;
  wasm_byte_vec_t دودویی_bytes;
  wasm_byte_vec_t wasm_bytes;
  wasm_engine_t *engine;
  wasm_val_t results_val[1] = { WASM_INIT_VAL };
  wasm_store_t *store;
  wasm_module_t *module;
  wasm_instance_t *instance;
  wasm_extern_vec_t exports;
  size_t args_size = 0;
  cleanup_free نویسه *wasi_args = پوچ;
  wasi_config_t *config;
  نویسه *ثابت *arg;
  wasi_env_t *wasi_env;
  wasm_importtype_vec_t import_types;
  wasm_extern_vec_t imports;
  wasm_func_t *run_func;
  wasm_val_vec_t args = WASM_EMPTY_VEC;
  wasm_val_vec_t res = WASM_EMPTY_VEC;

  wasm_engine_t *(*wasm_engine_new) ();
  تهی (*wat2wasm) (ثابت wasm_byte_vec_t *wat, wasm_byte_vec_t *out);
  wasm_module_t *(*wasm_module_new) (wasm_store_t *, ثابت wasm_byte_vec_t *دودویی);
  wasm_store_t *(*wasm_store_new) (wasm_engine_t *);
  wasm_instance_t *(*wasm_instance_new) (wasm_store_t *, ثابت wasm_module_t *, ثابت wasm_extern_vec_t *imports, wasm_trap_t **);
  تهی (*wasm_instance_exports) (ثابت wasm_instance_t *, wasm_extern_vec_t *out);
  wasm_func_t *(*wasm_extern_as_func) (wasm_extern_t *);
  تهی (*wasm_module_delete) (wasm_module_t *);
  تهی (*wasm_instance_delete) (wasm_instance_t *);
  تهی (*wasm_store_delete) (wasm_store_t *);
  تهی (*wasm_engine_delete) (wasm_engine_t *);
  تهی (*wasm_byte_vec_new) (wasm_byte_vec_t *, size_t, ثابت نویسه *);
  تهی (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  تهی (*wasm_importtype_vec_delete) (wasm_importtype_vec_t *);
  تهی (*wasm_extern_vec_delete) (wasm_extern_vec_t *);
  تهی (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  تهی (*wasm_extern_vec_new_uninitialized) (wasm_extern_vec_t *, size_t);
  تهی (*wasi_config_capture_stdout) (ساختار wasi_config_t *);
  تهی (*wasi_config_inherit_stdout) (ساختار wasi_config_t *);
  تهی (*wasm_module_imports) (ثابت wasm_module_t *, wasm_importtype_vec_t *);
  تهی (*wasm_func_delete) (wasm_func_t *);
  wasm_trap_t *(*wasm_func_call) (ثابت wasm_func_t *, ثابت wasm_val_vec_t *args, wasm_val_vec_t *results);
  wasi_config_t *(*wasi_config_new) (ثابت نویسه *);
  wasi_env_t *(*wasi_env_new) (wasm_store_t *, ساختار wasi_config_t *);
  bool (*wasi_get_imports) (ثابت wasm_store_t *, ثابت ساختار wasi_env_t *, ثابت wasm_module_t *, wasm_extern_vec_t *);
  wasm_func_t *(*wasi_get_start_function) (wasm_instance_t *);
  intptr_t (*wasi_env_read_stdout) (ساختار wasi_env_t *, نویسه *, uintptr_t);
  تهی (*wasi_env_delete) (ساختار wasi_env_t *);
  تهی (*wasi_config_arg) (ساختار wasi_config_t *config, ثابت نویسه *arg);
  bool (*wasi_env_initialize_instance) (ساختار wasi_env_t *, wasm_store_t *, wasm_instance_t *);

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

  اگر (wat2wasm == پوچ || wasm_module_delete == پوچ || wasm_instance_delete == پوچ
      || wasm_engine_delete == پوچ || wasm_store_delete == پوچ || wasm_func_call == پوچ
      || wasm_extern_as_func == پوچ || wasm_instance_exports == پوچ || wasm_instance_new == پوچ
      || wasm_store_new == پوچ || wasm_engine_new == پوچ || wasm_byte_vec_new == پوچ
      || wasm_byte_vec_delete == پوچ || wasm_extern_vec_delete == پوچ
      || wasm_byte_vec_new_uninitialized == پوچ || wasi_config_new == پوچ
      || wasi_config_capture_stdout == پوچ || wasi_env_new == پوچ || wasm_module_imports == پوچ
      || wasi_env_read_stdout == پوچ || wasi_env_delete == پوچ || wasm_func_delete == پوچ
      || wasm_importtype_vec_delete == پوچ || wasm_extern_vec_new_uninitialized == پوچ
      || wasi_get_imports == پوچ || wasi_get_start_function == پوچ || wasi_config_inherit_stdout == پوچ)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmer.so`");

  wat_wasm_file = fopen (pathname, "rbe");

  اگر (! wat_wasm_file)
    error (EXIT_FAILURE, errno, "error opening wat/wasm module");

  fseek (wat_wasm_file, 0L, SEEK_END);
  file_size = ftell (wat_wasm_file);
  fseek (wat_wasm_file, 0L, SEEK_SET);

  wasm_byte_vec_new_uninitialized (&دودویی_bytes, file_size);

  اگر (fread (دودویی_bytes.data, file_size, 1, wat_wasm_file) != 1)
    error (EXIT_FAILURE, errno, "error loading wat/wasm module");

  /* We can close entrypoint file.   */
  fclose (wat_wasm_file);

  /* We have received a wat file: convert wat to wasm.   */
  اگر (has_suffix (pathname, "wat") > 0)
    {
      wat2wasm (&دودویی_bytes, &wasm_bytes);
      دودویی_bytes = wasm_bytes;
    }

  engine = wasm_engine_new ();
  store = wasm_store_new (engine);

  module = wasm_module_new (store, &دودویی_bytes);

  اگر (! module)
    error (EXIT_FAILURE, 0, "error compiling wasm module");

  config = wasi_config_new ("crun_wasi_program");

  /* Count number of external arguments given.  */
  برای (arg = argv; *arg != پوچ; ++arg)
    args_size++;

  اگر (args_size > 1)
    {
      wasi_args = str_join_array (1, args_size, argv, " ");
      wasi_config_arg (config, wasi_args);
    }

  wasi_config_inherit_stdout (config);
  wasi_env = wasi_env_new (store, config);
  اگر (! wasi_env)
    {
      error (EXIT_FAILURE, 0, "error building wasi env");
    }

  /* Instantiate.  */
  اگر (! wasi_get_imports (store, wasi_env, module, &imports))
    error (EXIT_FAILURE, 0, "error getting WASI imports");

  instance = wasm_instance_new (store, module, &imports, پوچ);

  اگر (! instance)
    error (EXIT_FAILURE, 0, "error instantiating module");

  اگر (! wasi_env_initialize_instance (wasi_env, store, instance))
    error (EXIT_FAILURE, 0, "error init wasi env");

  /* Extract export.  */
  wasm_instance_exports (instance, &exports);
  اگر (exports.size == 0)
    error (EXIT_FAILURE, 0, "error getting instance exports");

  run_func = wasi_get_start_function (instance);
  اگر (run_func == پوچ)
    error (EXIT_FAILURE, 0, "error accessing export");

  اگر (wasm_func_call (run_func, &args, &res))
    error (EXIT_FAILURE, 0, "error calling wasm function");

  wasm_extern_vec_delete (&exports);
  wasm_extern_vec_delete (&imports);

  /* Shut down.  */
  wasm_module_delete (module);
  wasm_instance_delete (instance);
  wasm_func_delete (run_func);
  wasi_env_delete (wasi_env);
  wasm_store_delete (store);
  wasm_engine_delete (engine);

  exit (EXIT_SUCCESS);
}

ایستا صحیح
libwasmer_load (تهی **cookie, libcrun_error_t *err)
{
  تهی *handle;

  handle = dlopen ("libwasmer.so", RTLD_NOW);
  اگر (handle == پوچ)
    بازگشت crun_make_error (err, 0, "could not load `libwasmer.so`: %s", dlerror ());
  *cookie = handle;

  بازگشت 0;
}

ایستا صحیح
libwasmer_unload (تهی *cookie, libcrun_error_t *err)
{
  صحیح r;

  اگر (cookie)
    {
      r = dlclose (cookie);
      اگر (UNLIKELY (r < 0))
        بازگشت crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  بازگشت 0;
}

ایستا صحیح
libwasmer_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  بازگشت wasm_can_handle_container (container, err);
}

ساختار custom_handler_s handler_wasmer = {
  .name = "wasmer",
  .alias = "wasm",
  .feature_string = "WASM:wasmer",
  .supports_open_درخت_namespace = false,
  .load = libwasmer_load,
  .unload = libwasmer_unload,
  .run_func = libwasmer_exec,
  .can_handle_container = libwasmer_can_handle_container,
};

#endif
