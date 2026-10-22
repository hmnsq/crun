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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "../custom-handler.h"
#அந்தர்பாவ "../container.h"
#அந்தர்பாவ "../utils.h"
#அந்தர்பாவ "../linux.h"
#அந்தர்பாவ "handler-utils.h"
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMER
#  include <wasmer.h>
#endif

#எனில் HAVE_DLOPEN && HAVE_WASMER
#  define WASMER_BUF_SIZE 128
நிலையான முழு
libwasmer_exec (வெறுமை *cookie, libcrun_container_t *container arg_unused,
                மாறிலி எழுத்து *pathname, எழுத்து *மாறிலி argv[])
{
  முழு ret;
  எழுத்து buffer[WASMER_BUF_SIZE] = { 0 };
  size_t data_read_size = WASMER_BUF_SIZE;
  மாறிலி wasm_func_t *core_func;
  FILE *wat_wasm_file;
  size_t file_size;
  wasm_byte_vec_t wat;
  wasm_byte_vec_t இருமம்_bytes;
  wasm_byte_vec_t wasm_bytes;
  wasm_engine_t *engine;
  wasm_val_t results_val[1] = { WASM_INIT_VAL };
  wasm_store_t *store;
  wasm_module_t *module;
  wasm_instance_t *instance;
  wasm_extern_vec_t exports;
  size_t args_size = 0;
  cleanup_free எழுத்து *wasi_args = வெற்று;
  wasi_config_t *config;
  எழுத்து *மாறிலி *arg;
  wasi_env_t *wasi_env;
  wasm_importtype_vec_t import_types;
  wasm_extern_vec_t imports;
  wasm_func_t *run_func;
  wasm_val_vec_t args = WASM_EMPTY_VEC;
  wasm_val_vec_t res = WASM_EMPTY_VEC;

  wasm_engine_t *(*wasm_engine_new) ();
  வெறுமை (*wat2wasm) (மாறிலி wasm_byte_vec_t *wat, wasm_byte_vec_t *out);
  wasm_module_t *(*wasm_module_new) (wasm_store_t *, மாறிலி wasm_byte_vec_t *இருமம்);
  wasm_store_t *(*wasm_store_new) (wasm_engine_t *);
  wasm_instance_t *(*wasm_instance_new) (wasm_store_t *, மாறிலி wasm_module_t *, மாறிலி wasm_extern_vec_t *imports, wasm_trap_t **);
  வெறுமை (*wasm_instance_exports) (மாறிலி wasm_instance_t *, wasm_extern_vec_t *out);
  wasm_func_t *(*wasm_extern_as_func) (wasm_extern_t *);
  வெறுமை (*wasm_module_delete) (wasm_module_t *);
  வெறுமை (*wasm_instance_delete) (wasm_instance_t *);
  வெறுமை (*wasm_store_delete) (wasm_store_t *);
  வெறுமை (*wasm_engine_delete) (wasm_engine_t *);
  வெறுமை (*wasm_byte_vec_new) (wasm_byte_vec_t *, size_t, மாறிலி எழுத்து *);
  வெறுமை (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  வெறுமை (*wasm_importtype_vec_delete) (wasm_importtype_vec_t *);
  வெறுமை (*wasm_extern_vec_delete) (wasm_extern_vec_t *);
  வெறுமை (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  வெறுமை (*wasm_extern_vec_new_uninitialized) (wasm_extern_vec_t *, size_t);
  வெறுமை (*wasi_config_capture_stdout) (கட்டமைப்பு wasi_config_t *);
  வெறுமை (*wasi_config_inherit_stdout) (கட்டமைப்பு wasi_config_t *);
  வெறுமை (*wasm_module_imports) (மாறிலி wasm_module_t *, wasm_importtype_vec_t *);
  வெறுமை (*wasm_func_delete) (wasm_func_t *);
  wasm_trap_t *(*wasm_func_call) (மாறிலி wasm_func_t *, மாறிலி wasm_val_vec_t *args, wasm_val_vec_t *results);
  wasi_config_t *(*wasi_config_new) (மாறிலி எழுத்து *);
  wasi_env_t *(*wasi_env_new) (wasm_store_t *, கட்டமைப்பு wasi_config_t *);
  bool (*wasi_get_imports) (மாறிலி wasm_store_t *, மாறிலி கட்டமைப்பு wasi_env_t *, மாறிலி wasm_module_t *, wasm_extern_vec_t *);
  wasm_func_t *(*wasi_get_start_function) (wasm_instance_t *);
  intptr_t (*wasi_env_read_stdout) (கட்டமைப்பு wasi_env_t *, எழுத்து *, uintptr_t);
  வெறுமை (*wasi_env_delete) (கட்டமைப்பு wasi_env_t *);
  வெறுமை (*wasi_config_arg) (கட்டமைப்பு wasi_config_t *config, மாறிலி எழுத்து *arg);
  bool (*wasi_env_initialize_instance) (கட்டமைப்பு wasi_env_t *, wasm_store_t *, wasm_instance_t *);

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

  எனில் (wat2wasm == வெற்று || wasm_module_delete == வெற்று || wasm_instance_delete == வெற்று
      || wasm_engine_delete == வெற்று || wasm_store_delete == வெற்று || wasm_func_call == வெற்று
      || wasm_extern_as_func == வெற்று || wasm_instance_exports == வெற்று || wasm_instance_new == வெற்று
      || wasm_store_new == வெற்று || wasm_engine_new == வெற்று || wasm_byte_vec_new == வெற்று
      || wasm_byte_vec_delete == வெற்று || wasm_extern_vec_delete == வெற்று
      || wasm_byte_vec_new_uninitialized == வெற்று || wasi_config_new == வெற்று
      || wasi_config_capture_stdout == வெற்று || wasi_env_new == வெற்று || wasm_module_imports == வெற்று
      || wasi_env_read_stdout == வெற்று || wasi_env_delete == வெற்று || wasm_func_delete == வெற்று
      || wasm_importtype_vec_delete == வெற்று || wasm_extern_vec_new_uninitialized == வெற்று
      || wasi_get_imports == வெற்று || wasi_get_start_function == வெற்று || wasi_config_inherit_stdout == வெற்று)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmer.so`");

  wat_wasm_file = fopen (pathname, "rbe");

  எனில் (! wat_wasm_file)
    error (EXIT_FAILURE, errno, "error opening wat/wasm module");

  fseek (wat_wasm_file, 0L, SEEK_END);
  file_size = ftell (wat_wasm_file);
  fseek (wat_wasm_file, 0L, SEEK_SET);

  wasm_byte_vec_new_uninitialized (&இருமம்_bytes, file_size);

  எனில் (fread (இருமம்_bytes.data, file_size, 1, wat_wasm_file) != 1)
    error (EXIT_FAILURE, errno, "error loading wat/wasm module");

  /* We can close entrypoint file.   */
  fclose (wat_wasm_file);

  /* We have received a wat file: convert wat to wasm.   */
  எனில் (has_suffix (pathname, "wat") > 0)
    {
      wat2wasm (&இருமம்_bytes, &wasm_bytes);
      இருமம்_bytes = wasm_bytes;
    }

  engine = wasm_engine_new ();
  store = wasm_store_new (engine);

  module = wasm_module_new (store, &இருமம்_bytes);

  எனில் (! module)
    error (EXIT_FAILURE, 0, "error compiling wasm module");

  config = wasi_config_new ("crun_wasi_program");

  /* Count number of external arguments given.  */
  க்கு (arg = argv; *arg != வெற்று; ++arg)
    args_size++;

  எனில் (args_size > 1)
    {
      wasi_args = str_join_array (1, args_size, argv, " ");
      wasi_config_arg (config, wasi_args);
    }

  wasi_config_inherit_stdout (config);
  wasi_env = wasi_env_new (store, config);
  எனில் (! wasi_env)
    {
      error (EXIT_FAILURE, 0, "error building wasi env");
    }

  /* Instantiate.  */
  எனில் (! wasi_get_imports (store, wasi_env, module, &imports))
    error (EXIT_FAILURE, 0, "error getting WASI imports");

  instance = wasm_instance_new (store, module, &imports, வெற்று);

  எனில் (! instance)
    error (EXIT_FAILURE, 0, "error instantiating module");

  எனில் (! wasi_env_initialize_instance (wasi_env, store, instance))
    error (EXIT_FAILURE, 0, "error init wasi env");

  /* Extract export.  */
  wasm_instance_exports (instance, &exports);
  எனில் (exports.size == 0)
    error (EXIT_FAILURE, 0, "error getting instance exports");

  run_func = wasi_get_start_function (instance);
  எனில் (run_func == வெற்று)
    error (EXIT_FAILURE, 0, "error accessing export");

  எனில் (wasm_func_call (run_func, &args, &res))
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

நிலையான முழு
libwasmer_load (வெறுமை **cookie, libcrun_error_t *err)
{
  வெறுமை *handle;

  handle = dlopen ("libwasmer.so", RTLD_NOW);
  எனில் (handle == வெற்று)
    திரும்பு crun_make_error (err, 0, "could not load `libwasmer.so`: %s", dlerror ());
  *cookie = handle;

  திரும்பு 0;
}

நிலையான முழு
libwasmer_unload (வெறுமை *cookie, libcrun_error_t *err)
{
  முழு r;

  எனில் (cookie)
    {
      r = dlclose (cookie);
      எனில் (UNLIKELY (r < 0))
        திரும்பு crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  திரும்பு 0;
}

நிலையான முழு
libwasmer_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  திரும்பு wasm_can_handle_container (container, err);
}

கட்டமைப்பு custom_handler_s handler_wasmer = {
  .name = "wasmer",
  .alias = "wasm",
  .feature_string = "WASM:wasmer",
  .supports_open_மரம்_namespace = false,
  .load = libwasmer_load,
  .unload = libwasmer_unload,
  .run_func = libwasmer_exec,
  .can_handle_container = libwasmer_can_handle_container,
};

#endif
