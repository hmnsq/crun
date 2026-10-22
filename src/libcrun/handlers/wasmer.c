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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "../custom-handler.h"
#അന്തര്ഭാവ "../container.h"
#അന്തര്ഭാവ "../utils.h"
#അന്തര്ഭാവ "../linux.h"
#അന്തര്ഭാവ "handler-utils.h"
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMER
#  include <wasmer.h>
#endif

#എങ്കിൽ HAVE_DLOPEN && HAVE_WASMER
#  define WASMER_BUF_SIZE 128
സ്ഥിതം പൂർണ്ണസംഖ്യ
libwasmer_exec (ശൂന്യം *cookie, libcrun_container_t *container arg_unused,
                സ്ഥിരം അക്ഷരം *pathname, അക്ഷരം *സ്ഥിരം argv[])
{
  പൂർണ്ണസംഖ്യ ret;
  അക്ഷരം buffer[WASMER_BUF_SIZE] = { 0 };
  size_t data_read_size = WASMER_BUF_SIZE;
  സ്ഥിരം wasm_func_t *core_func;
  FILE *wat_wasm_file;
  size_t file_size;
  wasm_byte_vec_t wat;
  wasm_byte_vec_t ദ്വിമാന_bytes;
  wasm_byte_vec_t wasm_bytes;
  wasm_engine_t *engine;
  wasm_val_t results_val[1] = { WASM_INIT_VAL };
  wasm_store_t *store;
  wasm_module_t *module;
  wasm_instance_t *instance;
  wasm_extern_vec_t exports;
  size_t args_size = 0;
  cleanup_free അക്ഷരം *wasi_args = ശൂന്യ;
  wasi_config_t *config;
  അക്ഷരം *സ്ഥിരം *arg;
  wasi_env_t *wasi_env;
  wasm_importtype_vec_t import_types;
  wasm_extern_vec_t imports;
  wasm_func_t *run_func;
  wasm_val_vec_t args = WASM_EMPTY_VEC;
  wasm_val_vec_t res = WASM_EMPTY_VEC;

  wasm_engine_t *(*wasm_engine_new) ();
  ശൂന്യം (*wat2wasm) (സ്ഥിരം wasm_byte_vec_t *wat, wasm_byte_vec_t *out);
  wasm_module_t *(*wasm_module_new) (wasm_store_t *, സ്ഥിരം wasm_byte_vec_t *ദ്വിമാന);
  wasm_store_t *(*wasm_store_new) (wasm_engine_t *);
  wasm_instance_t *(*wasm_instance_new) (wasm_store_t *, സ്ഥിരം wasm_module_t *, സ്ഥിരം wasm_extern_vec_t *imports, wasm_trap_t **);
  ശൂന്യം (*wasm_instance_exports) (സ്ഥിരം wasm_instance_t *, wasm_extern_vec_t *out);
  wasm_func_t *(*wasm_extern_as_func) (wasm_extern_t *);
  ശൂന്യം (*wasm_module_delete) (wasm_module_t *);
  ശൂന്യം (*wasm_instance_delete) (wasm_instance_t *);
  ശൂന്യം (*wasm_store_delete) (wasm_store_t *);
  ശൂന്യം (*wasm_engine_delete) (wasm_engine_t *);
  ശൂന്യം (*wasm_byte_vec_new) (wasm_byte_vec_t *, size_t, സ്ഥിരം അക്ഷരം *);
  ശൂന്യം (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  ശൂന്യം (*wasm_importtype_vec_delete) (wasm_importtype_vec_t *);
  ശൂന്യം (*wasm_extern_vec_delete) (wasm_extern_vec_t *);
  ശൂന്യം (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  ശൂന്യം (*wasm_extern_vec_new_uninitialized) (wasm_extern_vec_t *, size_t);
  ശൂന്യം (*wasi_config_capture_stdout) (ഘടന wasi_config_t *);
  ശൂന്യം (*wasi_config_inherit_stdout) (ഘടന wasi_config_t *);
  ശൂന്യം (*wasm_module_imports) (സ്ഥിരം wasm_module_t *, wasm_importtype_vec_t *);
  ശൂന്യം (*wasm_func_delete) (wasm_func_t *);
  wasm_trap_t *(*wasm_func_call) (സ്ഥിരം wasm_func_t *, സ്ഥിരം wasm_val_vec_t *args, wasm_val_vec_t *results);
  wasi_config_t *(*wasi_config_new) (സ്ഥിരം അക്ഷരം *);
  wasi_env_t *(*wasi_env_new) (wasm_store_t *, ഘടന wasi_config_t *);
  bool (*wasi_get_imports) (സ്ഥിരം wasm_store_t *, സ്ഥിരം ഘടന wasi_env_t *, സ്ഥിരം wasm_module_t *, wasm_extern_vec_t *);
  wasm_func_t *(*wasi_get_start_function) (wasm_instance_t *);
  intptr_t (*wasi_env_read_stdout) (ഘടന wasi_env_t *, അക്ഷരം *, uintptr_t);
  ശൂന്യം (*wasi_env_delete) (ഘടന wasi_env_t *);
  ശൂന്യം (*wasi_config_arg) (ഘടന wasi_config_t *config, സ്ഥിരം അക്ഷരം *arg);
  bool (*wasi_env_initialize_instance) (ഘടന wasi_env_t *, wasm_store_t *, wasm_instance_t *);

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

  എങ്കിൽ (wat2wasm == ശൂന്യ || wasm_module_delete == ശൂന്യ || wasm_instance_delete == ശൂന്യ
      || wasm_engine_delete == ശൂന്യ || wasm_store_delete == ശൂന്യ || wasm_func_call == ശൂന്യ
      || wasm_extern_as_func == ശൂന്യ || wasm_instance_exports == ശൂന്യ || wasm_instance_new == ശൂന്യ
      || wasm_store_new == ശൂന്യ || wasm_engine_new == ശൂന്യ || wasm_byte_vec_new == ശൂന്യ
      || wasm_byte_vec_delete == ശൂന്യ || wasm_extern_vec_delete == ശൂന്യ
      || wasm_byte_vec_new_uninitialized == ശൂന്യ || wasi_config_new == ശൂന്യ
      || wasi_config_capture_stdout == ശൂന്യ || wasi_env_new == ശൂന്യ || wasm_module_imports == ശൂന്യ
      || wasi_env_read_stdout == ശൂന്യ || wasi_env_delete == ശൂന്യ || wasm_func_delete == ശൂന്യ
      || wasm_importtype_vec_delete == ശൂന്യ || wasm_extern_vec_new_uninitialized == ശൂന്യ
      || wasi_get_imports == ശൂന്യ || wasi_get_start_function == ശൂന്യ || wasi_config_inherit_stdout == ശൂന്യ)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmer.so`");

  wat_wasm_file = fopen (pathname, "rbe");

  എങ്കിൽ (! wat_wasm_file)
    error (EXIT_FAILURE, errno, "error opening wat/wasm module");

  fseek (wat_wasm_file, 0L, SEEK_END);
  file_size = ftell (wat_wasm_file);
  fseek (wat_wasm_file, 0L, SEEK_SET);

  wasm_byte_vec_new_uninitialized (&ദ്വിമാന_bytes, file_size);

  എങ്കിൽ (fread (ദ്വിമാന_bytes.data, file_size, 1, wat_wasm_file) != 1)
    error (EXIT_FAILURE, errno, "error loading wat/wasm module");

  /* We can close entrypoint file.   */
  fclose (wat_wasm_file);

  /* We have received a wat file: convert wat to wasm.   */
  എങ്കിൽ (has_suffix (pathname, "wat") > 0)
    {
      wat2wasm (&ദ്വിമാന_bytes, &wasm_bytes);
      ദ്വിമാന_bytes = wasm_bytes;
    }

  engine = wasm_engine_new ();
  store = wasm_store_new (engine);

  module = wasm_module_new (store, &ദ്വിമാന_bytes);

  എങ്കിൽ (! module)
    error (EXIT_FAILURE, 0, "error compiling wasm module");

  config = wasi_config_new ("crun_wasi_program");

  /* Count number of external arguments given.  */
  വേണ്ടി (arg = argv; *arg != ശൂന്യ; ++arg)
    args_size++;

  എങ്കിൽ (args_size > 1)
    {
      wasi_args = str_join_array (1, args_size, argv, " ");
      wasi_config_arg (config, wasi_args);
    }

  wasi_config_inherit_stdout (config);
  wasi_env = wasi_env_new (store, config);
  എങ്കിൽ (! wasi_env)
    {
      error (EXIT_FAILURE, 0, "error building wasi env");
    }

  /* Instantiate.  */
  എങ്കിൽ (! wasi_get_imports (store, wasi_env, module, &imports))
    error (EXIT_FAILURE, 0, "error getting WASI imports");

  instance = wasm_instance_new (store, module, &imports, ശൂന്യ);

  എങ്കിൽ (! instance)
    error (EXIT_FAILURE, 0, "error instantiating module");

  എങ്കിൽ (! wasi_env_initialize_instance (wasi_env, store, instance))
    error (EXIT_FAILURE, 0, "error init wasi env");

  /* Extract export.  */
  wasm_instance_exports (instance, &exports);
  എങ്കിൽ (exports.size == 0)
    error (EXIT_FAILURE, 0, "error getting instance exports");

  run_func = wasi_get_start_function (instance);
  എങ്കിൽ (run_func == ശൂന്യ)
    error (EXIT_FAILURE, 0, "error accessing export");

  എങ്കിൽ (wasm_func_call (run_func, &args, &res))
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

സ്ഥിതം പൂർണ്ണസംഖ്യ
libwasmer_load (ശൂന്യം **cookie, libcrun_error_t *err)
{
  ശൂന്യം *handle;

  handle = dlopen ("libwasmer.so", RTLD_NOW);
  എങ്കിൽ (handle == ശൂന്യ)
    തിരികെ crun_make_error (err, 0, "could not load `libwasmer.so`: %s", dlerror ());
  *cookie = handle;

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libwasmer_unload (ശൂന്യം *cookie, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ r;

  എങ്കിൽ (cookie)
    {
      r = dlclose (cookie);
      എങ്കിൽ (UNLIKELY (r < 0))
        തിരികെ crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libwasmer_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  തിരികെ wasm_can_handle_container (container, err);
}

ഘടന custom_handler_s handler_wasmer = {
  .name = "wasmer",
  .alias = "wasm",
  .feature_string = "WASM:wasmer",
  .supports_open_വൃക്ഷം_namespace = false,
  .load = libwasmer_load,
  .unload = libwasmer_unload,
  .run_func = libwasmer_exec,
  .can_handle_container = libwasmer_can_handle_container,
};

#endif
