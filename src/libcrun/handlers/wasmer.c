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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "../custom-handler.h"
#ಅನ್ತರ್ಭಾವ "../container.h"
#ಅನ್ತರ್ಭಾವ "../utils.h"
#ಅನ್ತರ್ಭಾವ "../linux.h"
#ಅನ್ತರ್ಭಾವ "handler-utils.h"
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMER
#  include <wasmer.h>
#endif

#ಯದಿ HAVE_DLOPEN && HAVE_WASMER
#  define WASMER_BUF_SIZE 128
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libwasmer_exec (ಶೂನ್ಯ *cookie, libcrun_container_t *container arg_unused,
                ಧ್ರುವ ವರ್ಣ *pathname, ವರ್ಣ *ಧ್ರುವ argv[])
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  ವರ್ಣ buffer[WASMER_BUF_SIZE] = { 0 };
  size_t data_read_size = WASMER_BUF_SIZE;
  ಧ್ರುವ wasm_func_t *core_func;
  FILE *wat_wasm_file;
  size_t file_size;
  wasm_byte_vec_t wat;
  wasm_byte_vec_t ದ್ವಿಮಾನ_bytes;
  wasm_byte_vec_t wasm_bytes;
  wasm_engine_t *engine;
  wasm_val_t results_val[1] = { WASM_INIT_VAL };
  wasm_store_t *store;
  wasm_module_t *module;
  wasm_instance_t *instance;
  wasm_extern_vec_t exports;
  size_t args_size = 0;
  cleanup_free ವರ್ಣ *wasi_args = ರಿಕ್ತ;
  wasi_config_t *config;
  ವರ್ಣ *ಧ್ರುವ *arg;
  wasi_env_t *wasi_env;
  wasm_importtype_vec_t import_types;
  wasm_extern_vec_t imports;
  wasm_func_t *run_func;
  wasm_val_vec_t args = WASM_EMPTY_VEC;
  wasm_val_vec_t res = WASM_EMPTY_VEC;

  wasm_engine_t *(*wasm_engine_new) ();
  ಶೂನ್ಯ (*wat2wasm) (ಧ್ರುವ wasm_byte_vec_t *wat, wasm_byte_vec_t *out);
  wasm_module_t *(*wasm_module_new) (wasm_store_t *, ಧ್ರುವ wasm_byte_vec_t *ದ್ವಿಮಾನ);
  wasm_store_t *(*wasm_store_new) (wasm_engine_t *);
  wasm_instance_t *(*wasm_instance_new) (wasm_store_t *, ಧ್ರುವ wasm_module_t *, ಧ್ರುವ wasm_extern_vec_t *imports, wasm_trap_t **);
  ಶೂನ್ಯ (*wasm_instance_exports) (ಧ್ರುವ wasm_instance_t *, wasm_extern_vec_t *out);
  wasm_func_t *(*wasm_extern_as_func) (wasm_extern_t *);
  ಶೂನ್ಯ (*wasm_module_delete) (wasm_module_t *);
  ಶೂನ್ಯ (*wasm_instance_delete) (wasm_instance_t *);
  ಶೂನ್ಯ (*wasm_store_delete) (wasm_store_t *);
  ಶೂನ್ಯ (*wasm_engine_delete) (wasm_engine_t *);
  ಶೂನ್ಯ (*wasm_byte_vec_new) (wasm_byte_vec_t *, size_t, ಧ್ರುವ ವರ್ಣ *);
  ಶೂನ್ಯ (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  ಶೂನ್ಯ (*wasm_importtype_vec_delete) (wasm_importtype_vec_t *);
  ಶೂನ್ಯ (*wasm_extern_vec_delete) (wasm_extern_vec_t *);
  ಶೂನ್ಯ (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  ಶೂನ್ಯ (*wasm_extern_vec_new_uninitialized) (wasm_extern_vec_t *, size_t);
  ಶೂನ್ಯ (*wasi_config_capture_stdout) (ಸಂರಚನಾ wasi_config_t *);
  ಶೂನ್ಯ (*wasi_config_inherit_stdout) (ಸಂರಚನಾ wasi_config_t *);
  ಶೂನ್ಯ (*wasm_module_imports) (ಧ್ರುವ wasm_module_t *, wasm_importtype_vec_t *);
  ಶೂನ್ಯ (*wasm_func_delete) (wasm_func_t *);
  wasm_trap_t *(*wasm_func_call) (ಧ್ರುವ wasm_func_t *, ಧ್ರುವ wasm_val_vec_t *args, wasm_val_vec_t *results);
  wasi_config_t *(*wasi_config_new) (ಧ್ರುವ ವರ್ಣ *);
  wasi_env_t *(*wasi_env_new) (wasm_store_t *, ಸಂರಚನಾ wasi_config_t *);
  bool (*wasi_get_imports) (ಧ್ರುವ wasm_store_t *, ಧ್ರುವ ಸಂರಚನಾ wasi_env_t *, ಧ್ರುವ wasm_module_t *, wasm_extern_vec_t *);
  wasm_func_t *(*wasi_get_start_function) (wasm_instance_t *);
  intptr_t (*wasi_env_read_stdout) (ಸಂರಚನಾ wasi_env_t *, ವರ್ಣ *, uintptr_t);
  ಶೂನ್ಯ (*wasi_env_delete) (ಸಂರಚನಾ wasi_env_t *);
  ಶೂನ್ಯ (*wasi_config_arg) (ಸಂರಚನಾ wasi_config_t *config, ಧ್ರುವ ವರ್ಣ *arg);
  bool (*wasi_env_initialize_instance) (ಸಂರಚನಾ wasi_env_t *, wasm_store_t *, wasm_instance_t *);

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

  ಯದಿ (wat2wasm == ರಿಕ್ತ || wasm_module_delete == ರಿಕ್ತ || wasm_instance_delete == ರಿಕ್ತ
      || wasm_engine_delete == ರಿಕ್ತ || wasm_store_delete == ರಿಕ್ತ || wasm_func_call == ರಿಕ್ತ
      || wasm_extern_as_func == ರಿಕ್ತ || wasm_instance_exports == ರಿಕ್ತ || wasm_instance_new == ರಿಕ್ತ
      || wasm_store_new == ರಿಕ್ತ || wasm_engine_new == ರಿಕ್ತ || wasm_byte_vec_new == ರಿಕ್ತ
      || wasm_byte_vec_delete == ರಿಕ್ತ || wasm_extern_vec_delete == ರಿಕ್ತ
      || wasm_byte_vec_new_uninitialized == ರಿಕ್ತ || wasi_config_new == ರಿಕ್ತ
      || wasi_config_capture_stdout == ರಿಕ್ತ || wasi_env_new == ರಿಕ್ತ || wasm_module_imports == ರಿಕ್ತ
      || wasi_env_read_stdout == ರಿಕ್ತ || wasi_env_delete == ರಿಕ್ತ || wasm_func_delete == ರಿಕ್ತ
      || wasm_importtype_vec_delete == ರಿಕ್ತ || wasm_extern_vec_new_uninitialized == ರಿಕ್ತ
      || wasi_get_imports == ರಿಕ್ತ || wasi_get_start_function == ರಿಕ್ತ || wasi_config_inherit_stdout == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmer.so`");

  wat_wasm_file = fopen (pathname, "rbe");

  ಯದಿ (! wat_wasm_file)
    error (EXIT_FAILURE, errno, "error opening wat/wasm module");

  fseek (wat_wasm_file, 0L, SEEK_END);
  file_size = ftell (wat_wasm_file);
  fseek (wat_wasm_file, 0L, SEEK_SET);

  wasm_byte_vec_new_uninitialized (&ದ್ವಿಮಾನ_bytes, file_size);

  ಯದಿ (fread (ದ್ವಿಮಾನ_bytes.data, file_size, 1, wat_wasm_file) != 1)
    error (EXIT_FAILURE, errno, "error loading wat/wasm module");

  /* We can close entrypoint file.   */
  fclose (wat_wasm_file);

  /* We have received a wat file: convert wat to wasm.   */
  ಯದಿ (has_suffix (pathname, "wat") > 0)
    {
      wat2wasm (&ದ್ವಿಮಾನ_bytes, &wasm_bytes);
      ದ್ವಿಮಾನ_bytes = wasm_bytes;
    }

  engine = wasm_engine_new ();
  store = wasm_store_new (engine);

  module = wasm_module_new (store, &ದ್ವಿಮಾನ_bytes);

  ಯದಿ (! module)
    error (EXIT_FAILURE, 0, "error compiling wasm module");

  config = wasi_config_new ("crun_wasi_program");

  /* Count number of external arguments given.  */
  ಪ್ರತಿ (arg = argv; *arg != ರಿಕ್ತ; ++arg)
    args_size++;

  ಯದಿ (args_size > 1)
    {
      wasi_args = str_join_array (1, args_size, argv, " ");
      wasi_config_arg (config, wasi_args);
    }

  wasi_config_inherit_stdout (config);
  wasi_env = wasi_env_new (store, config);
  ಯದಿ (! wasi_env)
    {
      error (EXIT_FAILURE, 0, "error building wasi env");
    }

  /* Instantiate.  */
  ಯದಿ (! wasi_get_imports (store, wasi_env, module, &imports))
    error (EXIT_FAILURE, 0, "error getting WASI imports");

  instance = wasm_instance_new (store, module, &imports, ರಿಕ್ತ);

  ಯದಿ (! instance)
    error (EXIT_FAILURE, 0, "error instantiating module");

  ಯದಿ (! wasi_env_initialize_instance (wasi_env, store, instance))
    error (EXIT_FAILURE, 0, "error init wasi env");

  /* Extract export.  */
  wasm_instance_exports (instance, &exports);
  ಯದಿ (exports.size == 0)
    error (EXIT_FAILURE, 0, "error getting instance exports");

  run_func = wasi_get_start_function (instance);
  ಯದಿ (run_func == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "error accessing export");

  ಯದಿ (wasm_func_call (run_func, &args, &res))
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

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libwasmer_load (ಶೂನ್ಯ **cookie, libcrun_error_t *err)
{
  ಶೂನ್ಯ *handle;

  handle = dlopen ("libwasmer.so", RTLD_NOW);
  ಯದಿ (handle == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not load `libwasmer.so`: %s", dlerror ());
  *cookie = handle;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libwasmer_unload (ಶೂನ್ಯ *cookie, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ r;

  ಯದಿ (cookie)
    {
      r = dlclose (cookie);
      ಯದಿ (UNLIKELY (r < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libwasmer_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ wasm_can_handle_container (container, err);
}

ಸಂರಚನಾ custom_handler_s handler_wasmer = {
  .name = "wasmer",
  .alias = "wasm",
  .feature_string = "WASM:wasmer",
  .supports_open_ವೃಕ್ಷ_namespace = false,
  .load = libwasmer_load,
  .unload = libwasmer_unload,
  .run_func = libwasmer_exec,
  .can_handle_container = libwasmer_can_handle_container,
};

#endif
