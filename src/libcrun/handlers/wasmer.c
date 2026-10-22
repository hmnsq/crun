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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "../custom-handler.h"
#ଅନ୍ତର୍ଭାଵ "../container.h"
#ଅନ୍ତର୍ଭାଵ "../utils.h"
#ଅନ୍ତର୍ଭାଵ "../linux.h"
#ଅନ୍ତର୍ଭାଵ "handler-utils.h"
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMER
#  include <wasmer.h>
#endif

#ଯଦି HAVE_DLOPEN && HAVE_WASMER
#  define WASMER_BUF_SIZE 128
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libwasmer_exec (ଶୂନ୍ୟ *cookie, libcrun_container_t *container arg_unused,
                ଅଚଳ ଅକ୍ଷର *pathname, ଅକ୍ଷର *ଅଚଳ argv[])
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ଅକ୍ଷର buffer[WASMER_BUF_SIZE] = { 0 };
  size_t data_read_size = WASMER_BUF_SIZE;
  ଅଚଳ wasm_func_t *core_func;
  FILE *wat_wasm_file;
  size_t file_size;
  wasm_byte_vec_t wat;
  wasm_byte_vec_t ଦ୍ୱିମିକ_bytes;
  wasm_byte_vec_t wasm_bytes;
  wasm_engine_t *engine;
  wasm_val_t results_val[1] = { WASM_INIT_VAL };
  wasm_store_t *store;
  wasm_module_t *module;
  wasm_instance_t *instance;
  wasm_extern_vec_t exports;
  size_t args_size = 0;
  cleanup_free ଅକ୍ଷର *wasi_args = ଖାଲି;
  wasi_config_t *config;
  ଅକ୍ଷର *ଅଚଳ *arg;
  wasi_env_t *wasi_env;
  wasm_importtype_vec_t import_types;
  wasm_extern_vec_t imports;
  wasm_func_t *run_func;
  wasm_val_vec_t args = WASM_EMPTY_VEC;
  wasm_val_vec_t res = WASM_EMPTY_VEC;

  wasm_engine_t *(*wasm_engine_new) ();
  ଶୂନ୍ୟ (*wat2wasm) (ଅଚଳ wasm_byte_vec_t *wat, wasm_byte_vec_t *out);
  wasm_module_t *(*wasm_module_new) (wasm_store_t *, ଅଚଳ wasm_byte_vec_t *ଦ୍ୱିମିକ);
  wasm_store_t *(*wasm_store_new) (wasm_engine_t *);
  wasm_instance_t *(*wasm_instance_new) (wasm_store_t *, ଅଚଳ wasm_module_t *, ଅଚଳ wasm_extern_vec_t *imports, wasm_trap_t **);
  ଶୂନ୍ୟ (*wasm_instance_exports) (ଅଚଳ wasm_instance_t *, wasm_extern_vec_t *out);
  wasm_func_t *(*wasm_extern_as_func) (wasm_extern_t *);
  ଶୂନ୍ୟ (*wasm_module_delete) (wasm_module_t *);
  ଶୂନ୍ୟ (*wasm_instance_delete) (wasm_instance_t *);
  ଶୂନ୍ୟ (*wasm_store_delete) (wasm_store_t *);
  ଶୂନ୍ୟ (*wasm_engine_delete) (wasm_engine_t *);
  ଶୂନ୍ୟ (*wasm_byte_vec_new) (wasm_byte_vec_t *, size_t, ଅଚଳ ଅକ୍ଷର *);
  ଶୂନ୍ୟ (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  ଶୂନ୍ୟ (*wasm_importtype_vec_delete) (wasm_importtype_vec_t *);
  ଶୂନ୍ୟ (*wasm_extern_vec_delete) (wasm_extern_vec_t *);
  ଶୂନ୍ୟ (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  ଶୂନ୍ୟ (*wasm_extern_vec_new_uninitialized) (wasm_extern_vec_t *, size_t);
  ଶୂନ୍ୟ (*wasi_config_capture_stdout) (ସଂରଚନା wasi_config_t *);
  ଶୂନ୍ୟ (*wasi_config_inherit_stdout) (ସଂରଚନା wasi_config_t *);
  ଶୂନ୍ୟ (*wasm_module_imports) (ଅଚଳ wasm_module_t *, wasm_importtype_vec_t *);
  ଶୂନ୍ୟ (*wasm_func_delete) (wasm_func_t *);
  wasm_trap_t *(*wasm_func_call) (ଅଚଳ wasm_func_t *, ଅଚଳ wasm_val_vec_t *args, wasm_val_vec_t *results);
  wasi_config_t *(*wasi_config_new) (ଅଚଳ ଅକ୍ଷର *);
  wasi_env_t *(*wasi_env_new) (wasm_store_t *, ସଂରଚନା wasi_config_t *);
  bool (*wasi_get_imports) (ଅଚଳ wasm_store_t *, ଅଚଳ ସଂରଚନା wasi_env_t *, ଅଚଳ wasm_module_t *, wasm_extern_vec_t *);
  wasm_func_t *(*wasi_get_start_function) (wasm_instance_t *);
  intptr_t (*wasi_env_read_stdout) (ସଂରଚନା wasi_env_t *, ଅକ୍ଷର *, uintptr_t);
  ଶୂନ୍ୟ (*wasi_env_delete) (ସଂରଚନା wasi_env_t *);
  ଶୂନ୍ୟ (*wasi_config_arg) (ସଂରଚନା wasi_config_t *config, ଅଚଳ ଅକ୍ଷର *arg);
  bool (*wasi_env_initialize_instance) (ସଂରଚନା wasi_env_t *, wasm_store_t *, wasm_instance_t *);

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

  ଯଦି (wat2wasm == ଖାଲି || wasm_module_delete == ଖାଲି || wasm_instance_delete == ଖାଲି
      || wasm_engine_delete == ଖାଲି || wasm_store_delete == ଖାଲି || wasm_func_call == ଖାଲି
      || wasm_extern_as_func == ଖାଲି || wasm_instance_exports == ଖାଲି || wasm_instance_new == ଖାଲି
      || wasm_store_new == ଖାଲି || wasm_engine_new == ଖାଲି || wasm_byte_vec_new == ଖାଲି
      || wasm_byte_vec_delete == ଖାଲି || wasm_extern_vec_delete == ଖାଲି
      || wasm_byte_vec_new_uninitialized == ଖାଲି || wasi_config_new == ଖାଲି
      || wasi_config_capture_stdout == ଖାଲି || wasi_env_new == ଖାଲି || wasm_module_imports == ଖାଲି
      || wasi_env_read_stdout == ଖାଲି || wasi_env_delete == ଖାଲି || wasm_func_delete == ଖାଲି
      || wasm_importtype_vec_delete == ଖାଲି || wasm_extern_vec_new_uninitialized == ଖାଲି
      || wasi_get_imports == ଖାଲି || wasi_get_start_function == ଖାଲି || wasi_config_inherit_stdout == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmer.so`");

  wat_wasm_file = fopen (pathname, "rbe");

  ଯଦି (! wat_wasm_file)
    error (EXIT_FAILURE, errno, "error opening wat/wasm module");

  fseek (wat_wasm_file, 0L, SEEK_END);
  file_size = ftell (wat_wasm_file);
  fseek (wat_wasm_file, 0L, SEEK_SET);

  wasm_byte_vec_new_uninitialized (&ଦ୍ୱିମିକ_bytes, file_size);

  ଯଦି (fread (ଦ୍ୱିମିକ_bytes.data, file_size, 1, wat_wasm_file) != 1)
    error (EXIT_FAILURE, errno, "error loading wat/wasm module");

  /* We can close entrypoint file.   */
  fclose (wat_wasm_file);

  /* We have received a wat file: convert wat to wasm.   */
  ଯଦି (has_suffix (pathname, "wat") > 0)
    {
      wat2wasm (&ଦ୍ୱିମିକ_bytes, &wasm_bytes);
      ଦ୍ୱିମିକ_bytes = wasm_bytes;
    }

  engine = wasm_engine_new ();
  store = wasm_store_new (engine);

  module = wasm_module_new (store, &ଦ୍ୱିମିକ_bytes);

  ଯଦି (! module)
    error (EXIT_FAILURE, 0, "error compiling wasm module");

  config = wasi_config_new ("crun_wasi_program");

  /* Count number of external arguments given.  */
  ପାଇଁ (arg = argv; *arg != ଖାଲି; ++arg)
    args_size++;

  ଯଦି (args_size > 1)
    {
      wasi_args = str_join_array (1, args_size, argv, " ");
      wasi_config_arg (config, wasi_args);
    }

  wasi_config_inherit_stdout (config);
  wasi_env = wasi_env_new (store, config);
  ଯଦି (! wasi_env)
    {
      error (EXIT_FAILURE, 0, "error building wasi env");
    }

  /* Instantiate.  */
  ଯଦି (! wasi_get_imports (store, wasi_env, module, &imports))
    error (EXIT_FAILURE, 0, "error getting WASI imports");

  instance = wasm_instance_new (store, module, &imports, ଖାଲି);

  ଯଦି (! instance)
    error (EXIT_FAILURE, 0, "error instantiating module");

  ଯଦି (! wasi_env_initialize_instance (wasi_env, store, instance))
    error (EXIT_FAILURE, 0, "error init wasi env");

  /* Extract export.  */
  wasm_instance_exports (instance, &exports);
  ଯଦି (exports.size == 0)
    error (EXIT_FAILURE, 0, "error getting instance exports");

  run_func = wasi_get_start_function (instance);
  ଯଦି (run_func == ଖାଲି)
    error (EXIT_FAILURE, 0, "error accessing export");

  ଯଦି (wasm_func_call (run_func, &args, &res))
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

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libwasmer_load (ଶୂନ୍ୟ **cookie, libcrun_error_t *err)
{
  ଶୂନ୍ୟ *handle;

  handle = dlopen ("libwasmer.so", RTLD_NOW);
  ଯଦି (handle == ଖାଲି)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "could not load `libwasmer.so`: %s", dlerror ());
  *cookie = handle;

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libwasmer_unload (ଶୂନ୍ୟ *cookie, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା r;

  ଯଦି (cookie)
    {
      r = dlclose (cookie);
      ଯଦି (UNLIKELY (r < 0))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libwasmer_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ wasm_can_handle_container (container, err);
}

ସଂରଚନା custom_handler_s handler_wasmer = {
  .name = "wasmer",
  .alias = "wasm",
  .feature_string = "WASM:wasmer",
  .supports_open_ବୃକ୍ଷ_namespace = false,
  .load = libwasmer_load,
  .unload = libwasmer_unload,
  .run_func = libwasmer_exec,
  .can_handle_container = libwasmer_can_handle_container,
};

#endif
