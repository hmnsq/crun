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

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DLOPEN
#  include <dlfcn.h>
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_WASMER
#  include <wasmer.h>
#ਜੇ_ਅੰਤ

#ਜੇ HAVE_DLOPEN && HAVE_WASMER
#  define WASMER_BUF_ਮਾਪ 128
ਸਥਿਰ ਪੂਰਨ
libwasmer_exec (ਖਾਲੀ *cookie, libcrun_container_t *container arg_ਅਣਵਰਤਿਆ,
                ਅਚਲ ਅੱਖਰ *pathname, ਅੱਖਰ *ਅਚਲ argv[])
{
  ਪੂਰਨ ret;
  ਅੱਖਰ ਬਫਰ[WASMER_BUF_ਮਾਪ] = { 0 };
  size_t ਅੰਕੜੇ_read_ਮਾਪ = WASMER_BUF_ਮਾਪ;
  ਅਚਲ wasm_func_t *core_func;
  FILE *wat_wasm_file;
  size_t file_ਮਾਪ;
  wasm_byte_vec_t wat;
  wasm_byte_vec_t ਦੁਆਧਾਰੀ_ਬਾਈਟਾਂ;
  wasm_byte_vec_t wasm_ਬਾਈਟਾਂ;
  wasm_engine_t *engine;
  wasm_val_t results_val[1] = { WASM_INIT_VAL };
  wasm_store_t *store;
  wasm_module_t *module;
  wasm_instance_t *instance;
  wasm_extern_vec_t exports;
  size_t ਦਲੀਲਾਂ_ਮਾਪ = 0;
  cleanup_free ਅੱਖਰ *wasi_ਦਲੀਲਾਂ = ਨਿਰਾ;
  wasi_config_t *config;
  ਅੱਖਰ *ਅਚਲ *arg;
  wasi_env_t *wasi_env;
  wasm_importtype_vec_t import_types;
  wasm_extern_vec_t imports;
  wasm_func_t *ਲੜੀਵਾਰ_func;
  wasm_val_vec_t ਦਲੀਲਾਂ = WASM_ਖਾਲੀ_ਥਾਵਾਂ_VEC;
  wasm_val_vec_t res = WASM_ਖਾਲੀ_ਥਾਵਾਂ_VEC;

  wasm_engine_t *(*wasm_engine_new) ();
  ਖਾਲੀ (*wat2wasm) (ਅਚਲ wasm_byte_vec_t *wat, wasm_byte_vec_t *ਬਾਹਰਲਾ);
  wasm_module_t *(*wasm_module_new) (wasm_store_t *, ਅਚਲ wasm_byte_vec_t *ਦੁਆਧਾਰੀ);
  wasm_store_t *(*wasm_store_new) (wasm_engine_t *);
  wasm_instance_t *(*wasm_instance_new) (wasm_store_t *, ਅਚਲ wasm_module_t *, ਅਚਲ wasm_extern_vec_t *imports, wasm_trap_t **);
  ਖਾਲੀ (*wasm_instance_exports) (ਅਚਲ wasm_instance_t *, wasm_extern_vec_t *ਬਾਹਰਲਾ);
  wasm_func_t *(*wasm_extern_as_func) (wasm_extern_t *);
  ਖਾਲੀ (*wasm_module_delete) (wasm_module_t *);
  ਖਾਲੀ (*wasm_instance_delete) (wasm_instance_t *);
  ਖਾਲੀ (*wasm_store_delete) (wasm_store_t *);
  ਖਾਲੀ (*wasm_engine_delete) (wasm_engine_t *);
  ਖਾਲੀ (*wasm_byte_vec_new) (wasm_byte_vec_t *, size_t, ਅਚਲ ਅੱਖਰ *);
  ਖਾਲੀ (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  ਖਾਲੀ (*wasm_importtype_vec_delete) (wasm_importtype_vec_t *);
  ਖਾਲੀ (*wasm_extern_vec_delete) (wasm_extern_vec_t *);
  ਖਾਲੀ (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  ਖਾਲੀ (*wasm_extern_vec_new_uninitialized) (wasm_extern_vec_t *, size_t);
  ਖਾਲੀ (*wasi_config_capture_stdout) (ਬਣਤਰ wasi_config_t *);
  ਖਾਲੀ (*wasi_config_inherit_stdout) (ਬਣਤਰ wasi_config_t *);
  ਖਾਲੀ (*wasm_module_imports) (ਅਚਲ wasm_module_t *, wasm_importtype_vec_t *);
  ਖਾਲੀ (*wasm_func_delete) (wasm_func_t *);
  wasm_trap_t *(*wasm_func_call) (ਅਚਲ wasm_func_t *, ਅਚਲ wasm_val_vec_t *ਦਲੀਲਾਂ, wasm_val_vec_t *results);
  wasi_config_t *(*wasi_config_new) (ਅਚਲ ਅੱਖਰ *);
  wasi_env_t *(*wasi_env_new) (wasm_store_t *, ਬਣਤਰ wasi_config_t *);
  bool (*wasi_get_imports) (ਅਚਲ wasm_store_t *, ਅਚਲ ਬਣਤਰ wasi_env_t *, ਅਚਲ wasm_module_t *, wasm_extern_vec_t *);
  wasm_func_t *(*wasi_get_ਸ਼ੁਰੂ_function) (wasm_instance_t *);
  intptr_t (*wasi_env_read_stdout) (ਬਣਤਰ wasi_env_t *, ਅੱਖਰ *, uintptr_t);
  ਖਾਲੀ (*wasi_env_delete) (ਬਣਤਰ wasi_env_t *);
  ਖਾਲੀ (*wasi_config_arg) (ਬਣਤਰ wasi_config_t *config, ਅਚਲ ਅੱਖਰ *arg);
  bool (*wasi_env_initialize_instance) (ਬਣਤਰ wasi_env_t *, wasm_store_t *, wasm_instance_t *);

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
  wasi_get_ਸ਼ੁਰੂ_function = dlsym (cookie, "wasi_get_start_function");
  wasi_env_read_stdout = dlsym (cookie, "wasi_env_read_stdout");
  wasi_env_delete = dlsym (cookie, "wasi_env_delete");
  wasm_func_delete = dlsym (cookie, "wasm_func_delete");
  wasi_env_initialize_instance = dlsym (cookie, "wasi_env_initialize_instance");

  ਜੇ (wat2wasm == ਨਿਰਾ || wasm_module_delete == ਨਿਰਾ || wasm_instance_delete == ਨਿਰਾ
      || wasm_engine_delete == ਨਿਰਾ || wasm_store_delete == ਨਿਰਾ || wasm_func_call == ਨਿਰਾ
      || wasm_extern_as_func == ਨਿਰਾ || wasm_instance_exports == ਨਿਰਾ || wasm_instance_new == ਨਿਰਾ
      || wasm_store_new == ਨਿਰਾ || wasm_engine_new == ਨਿਰਾ || wasm_byte_vec_new == ਨਿਰਾ
      || wasm_byte_vec_delete == ਨਿਰਾ || wasm_extern_vec_delete == ਨਿਰਾ
      || wasm_byte_vec_new_uninitialized == ਨਿਰਾ || wasi_config_new == ਨਿਰਾ
      || wasi_config_capture_stdout == ਨਿਰਾ || wasi_env_new == ਨਿਰਾ || wasm_module_imports == ਨਿਰਾ
      || wasi_env_read_stdout == ਨਿਰਾ || wasi_env_delete == ਨਿਰਾ || wasm_func_delete == ਨਿਰਾ
      || wasm_importtype_vec_delete == ਨਿਰਾ || wasm_extern_vec_new_uninitialized == ਨਿਰਾ
      || wasi_get_imports == ਨਿਰਾ || wasi_get_ਸ਼ੁਰੂ_function == ਨਿਰਾ || wasi_config_inherit_stdout == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmer.so`");

  wat_wasm_file = fopen (pathname, "rbe");

  ਜੇ (! wat_wasm_file)
    error (EXIT_FAILURE, errno, "error opening wat/wasm module");

  fseek (wat_wasm_file, 0L, SEEK_END);
  file_ਮਾਪ = ftell (wat_wasm_file);
  fseek (wat_wasm_file, 0L, SEEK_SET);

  wasm_byte_vec_new_uninitialized (&ਦੁਆਧਾਰੀ_ਬਾਈਟਾਂ, file_ਮਾਪ);

  ਜੇ (fread (ਦੁਆਧਾਰੀ_ਬਾਈਟਾਂ.ਅੰਕੜੇ, file_ਮਾਪ, 1, wat_wasm_file) != 1)
    error (EXIT_FAILURE, errno, "error loading wat/wasm module");

  /* We can close entrypoint file.   */
  fclose (wat_wasm_file);

  /* We have received a wat file: convert wat to wasm.   */
  ਜੇ (has_suffix (pathname, "wat") > 0)
    {
      wat2wasm (&ਦੁਆਧਾਰੀ_ਬਾਈਟਾਂ, &wasm_ਬਾਈਟਾਂ);
      ਦੁਆਧਾਰੀ_ਬਾਈਟਾਂ = wasm_ਬਾਈਟਾਂ;
    }

  engine = wasm_engine_new ();
  store = wasm_store_new (engine);

  module = wasm_module_new (store, &ਦੁਆਧਾਰੀ_ਬਾਈਟਾਂ);

  ਜੇ (! module)
    error (EXIT_FAILURE, 0, "error compiling wasm module");

  config = wasi_config_new ("crun_wasi_program");

  /* Count number of external arguments given.  */
  ਲਈ (arg = argv; *arg != ਨਿਰਾ; ++arg)
    ਦਲੀਲਾਂ_ਮਾਪ++;

  ਜੇ (ਦਲੀਲਾਂ_ਮਾਪ > 1)
    {
      wasi_ਦਲੀਲਾਂ = str_join_array (1, ਦਲੀਲਾਂ_ਮਾਪ, argv, " ");
      wasi_config_arg (config, wasi_ਦਲੀਲਾਂ);
    }

  wasi_config_inherit_stdout (config);
  wasi_env = wasi_env_new (store, config);
  ਜੇ (! wasi_env)
    {
      error (EXIT_FAILURE, 0, "error building wasi env");
    }

  /* Instantiate.  */
  ਜੇ (! wasi_get_imports (store, wasi_env, module, &imports))
    error (EXIT_FAILURE, 0, "error getting WASI imports");

  instance = wasm_instance_new (store, module, &imports, ਨਿਰਾ);

  ਜੇ (! instance)
    error (EXIT_FAILURE, 0, "error instantiating module");

  ਜੇ (! wasi_env_initialize_instance (wasi_env, store, instance))
    error (EXIT_FAILURE, 0, "error init wasi env");

  /* Extract export.  */
  wasm_instance_exports (instance, &exports);
  ਜੇ (exports.ਮਾਪ == 0)
    error (EXIT_FAILURE, 0, "error getting instance exports");

  ਲੜੀਵਾਰ_func = wasi_get_ਸ਼ੁਰੂ_function (instance);
  ਜੇ (ਲੜੀਵਾਰ_func == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "error accessing export");

  ਜੇ (wasm_func_call (ਲੜੀਵਾਰ_func, &ਦਲੀਲਾਂ, &res))
    error (EXIT_FAILURE, 0, "error calling wasm function");

  wasm_extern_vec_delete (&exports);
  wasm_extern_vec_delete (&imports);

  /* Shut down.  */
  wasm_module_delete (module);
  wasm_instance_delete (instance);
  wasm_func_delete (ਲੜੀਵਾਰ_func);
  wasi_env_delete (wasi_env);
  wasm_store_delete (store);
  wasm_engine_delete (engine);

  ਬਾਹਰ_ਨਿਕਲੋ (EXIT_SUCCESS);
}

ਸਥਿਰ ਪੂਰਨ
libwasmer_ਲੱਦੋ (ਖਾਲੀ **cookie, libcrun_error_t *err)
{
  ਖਾਲੀ *handle;

  handle = dlopen ("libwasmer.so", RTLD_NOW);
  ਜੇ (handle == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, 0, "could not load `libwasmer.so`: %s", dlerror ());
  *cookie = handle;

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
libwasmer_unload (ਖਾਲੀ *cookie, libcrun_error_t *err)
{
  ਪੂਰਨ r;

  ਜੇ (cookie)
    {
      r = dlclose (cookie);
      ਜੇ (UNLIKELY (r < 0))
        ਵਾਪਸ crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
libwasmer_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  ਵਾਪਸ wasm_can_handle_container (container, err);
}

ਬਣਤਰ custom_handler_s handler_wasmer = {
  .name = "wasmer",
  .alias = "wasm",
  .feature_string = "WASM:wasmer",
  .supports_open_ਰੁੱਖ_namespace = false,
  .ਲੱਦੋ = libwasmer_ਲੱਦੋ,
  .unload = libwasmer_unload,
  .ਲੜੀਵਾਰ_func = libwasmer_exec,
  .can_handle_container = libwasmer_can_handle_container,
};

#ਜੇ_ਅੰਤ
