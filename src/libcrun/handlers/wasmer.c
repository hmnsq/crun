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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "../custom-handler.h"
#অন্তর্ভাব "../container.h"
#অন্তর্ভাব "../utils.h"
#অন্তর্ভাব "../linux.h"
#অন্তর্ভাব "handler-utils.h"
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMER
#  include <wasmer.h>
#endif

#যদি HAVE_DLOPEN && HAVE_WASMER
#  define WASMER_BUF_SIZE 128
স্থির পূর্ণাঙ্ক
libwasmer_exec (শূন্য *cookie, libcrun_container_t *container arg_unused,
                ধ্রুব বর্ণ *pathname, বর্ণ *ধ্রুব argv[])
{
  পূর্ণাঙ্ক ret;
  বর্ণ buffer[WASMER_BUF_SIZE] = { 0 };
  size_t data_read_size = WASMER_BUF_SIZE;
  ধ্রুব wasm_func_t *core_func;
  FILE *wat_wasm_file;
  size_t file_size;
  wasm_byte_vec_t wat;
  wasm_byte_vec_t দ্বিমিক_bytes;
  wasm_byte_vec_t wasm_bytes;
  wasm_engine_t *engine;
  wasm_val_t results_val[1] = { WASM_INIT_VAL };
  wasm_store_t *store;
  wasm_module_t *module;
  wasm_instance_t *instance;
  wasm_extern_vec_t exports;
  size_t args_size = 0;
  cleanup_free বর্ণ *wasi_args = রিক্ত;
  wasi_config_t *config;
  বর্ণ *ধ্রুব *arg;
  wasi_env_t *wasi_env;
  wasm_importtype_vec_t import_types;
  wasm_extern_vec_t imports;
  wasm_func_t *run_func;
  wasm_val_vec_t args = WASM_EMPTY_VEC;
  wasm_val_vec_t res = WASM_EMPTY_VEC;

  wasm_engine_t *(*wasm_engine_new) ();
  শূন্য (*wat2wasm) (ধ্রুব wasm_byte_vec_t *wat, wasm_byte_vec_t *out);
  wasm_module_t *(*wasm_module_new) (wasm_store_t *, ধ্রুব wasm_byte_vec_t *দ্বিমিক);
  wasm_store_t *(*wasm_store_new) (wasm_engine_t *);
  wasm_instance_t *(*wasm_instance_new) (wasm_store_t *, ধ্রুব wasm_module_t *, ধ্রুব wasm_extern_vec_t *imports, wasm_trap_t **);
  শূন্য (*wasm_instance_exports) (ধ্রুব wasm_instance_t *, wasm_extern_vec_t *out);
  wasm_func_t *(*wasm_extern_as_func) (wasm_extern_t *);
  শূন্য (*wasm_module_delete) (wasm_module_t *);
  শূন্য (*wasm_instance_delete) (wasm_instance_t *);
  শূন্য (*wasm_store_delete) (wasm_store_t *);
  শূন্য (*wasm_engine_delete) (wasm_engine_t *);
  শূন্য (*wasm_byte_vec_new) (wasm_byte_vec_t *, size_t, ধ্রুব বর্ণ *);
  শূন্য (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  শূন্য (*wasm_importtype_vec_delete) (wasm_importtype_vec_t *);
  শূন্য (*wasm_extern_vec_delete) (wasm_extern_vec_t *);
  শূন্য (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  শূন্য (*wasm_extern_vec_new_uninitialized) (wasm_extern_vec_t *, size_t);
  শূন্য (*wasi_config_capture_stdout) (সংরচনা wasi_config_t *);
  শূন্য (*wasi_config_inherit_stdout) (সংরচনা wasi_config_t *);
  শূন্য (*wasm_module_imports) (ধ্রুব wasm_module_t *, wasm_importtype_vec_t *);
  শূন্য (*wasm_func_delete) (wasm_func_t *);
  wasm_trap_t *(*wasm_func_call) (ধ্রুব wasm_func_t *, ধ্রুব wasm_val_vec_t *args, wasm_val_vec_t *results);
  wasi_config_t *(*wasi_config_new) (ধ্রুব বর্ণ *);
  wasi_env_t *(*wasi_env_new) (wasm_store_t *, সংরচনা wasi_config_t *);
  bool (*wasi_get_imports) (ধ্রুব wasm_store_t *, ধ্রুব সংরচনা wasi_env_t *, ধ্রুব wasm_module_t *, wasm_extern_vec_t *);
  wasm_func_t *(*wasi_get_start_function) (wasm_instance_t *);
  intptr_t (*wasi_env_read_stdout) (সংরচনা wasi_env_t *, বর্ণ *, uintptr_t);
  শূন্য (*wasi_env_delete) (সংরচনা wasi_env_t *);
  শূন্য (*wasi_config_arg) (সংরচনা wasi_config_t *config, ধ্রুব বর্ণ *arg);
  bool (*wasi_env_initialize_instance) (সংরচনা wasi_env_t *, wasm_store_t *, wasm_instance_t *);

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

  যদি (wat2wasm == রিক্ত || wasm_module_delete == রিক্ত || wasm_instance_delete == রিক্ত
      || wasm_engine_delete == রিক্ত || wasm_store_delete == রিক্ত || wasm_func_call == রিক্ত
      || wasm_extern_as_func == রিক্ত || wasm_instance_exports == রিক্ত || wasm_instance_new == রিক্ত
      || wasm_store_new == রিক্ত || wasm_engine_new == রিক্ত || wasm_byte_vec_new == রিক্ত
      || wasm_byte_vec_delete == রিক্ত || wasm_extern_vec_delete == রিক্ত
      || wasm_byte_vec_new_uninitialized == রিক্ত || wasi_config_new == রিক্ত
      || wasi_config_capture_stdout == রিক্ত || wasi_env_new == রিক্ত || wasm_module_imports == রিক্ত
      || wasi_env_read_stdout == রিক্ত || wasi_env_delete == রিক্ত || wasm_func_delete == রিক্ত
      || wasm_importtype_vec_delete == রিক্ত || wasm_extern_vec_new_uninitialized == রিক্ত
      || wasi_get_imports == রিক্ত || wasi_get_start_function == রিক্ত || wasi_config_inherit_stdout == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmer.so`");

  wat_wasm_file = fopen (pathname, "rbe");

  যদি (! wat_wasm_file)
    error (EXIT_FAILURE, errno, "error opening wat/wasm module");

  fseek (wat_wasm_file, 0L, SEEK_END);
  file_size = ftell (wat_wasm_file);
  fseek (wat_wasm_file, 0L, SEEK_SET);

  wasm_byte_vec_new_uninitialized (&দ্বিমিক_bytes, file_size);

  যদি (fread (দ্বিমিক_bytes.data, file_size, 1, wat_wasm_file) != 1)
    error (EXIT_FAILURE, errno, "error loading wat/wasm module");

  /* We can close entrypoint file.   */
  fclose (wat_wasm_file);

  /* We have received a wat file: convert wat to wasm.   */
  যদি (has_suffix (pathname, "wat") > 0)
    {
      wat2wasm (&দ্বিমিক_bytes, &wasm_bytes);
      দ্বিমিক_bytes = wasm_bytes;
    }

  engine = wasm_engine_new ();
  store = wasm_store_new (engine);

  module = wasm_module_new (store, &দ্বিমিক_bytes);

  যদি (! module)
    error (EXIT_FAILURE, 0, "error compiling wasm module");

  config = wasi_config_new ("crun_wasi_program");

  /* Count number of external arguments given.  */
  প্রতি (arg = argv; *arg != রিক্ত; ++arg)
    args_size++;

  যদি (args_size > 1)
    {
      wasi_args = str_join_array (1, args_size, argv, " ");
      wasi_config_arg (config, wasi_args);
    }

  wasi_config_inherit_stdout (config);
  wasi_env = wasi_env_new (store, config);
  যদি (! wasi_env)
    {
      error (EXIT_FAILURE, 0, "error building wasi env");
    }

  /* Instantiate.  */
  যদি (! wasi_get_imports (store, wasi_env, module, &imports))
    error (EXIT_FAILURE, 0, "error getting WASI imports");

  instance = wasm_instance_new (store, module, &imports, রিক্ত);

  যদি (! instance)
    error (EXIT_FAILURE, 0, "error instantiating module");

  যদি (! wasi_env_initialize_instance (wasi_env, store, instance))
    error (EXIT_FAILURE, 0, "error init wasi env");

  /* Extract export.  */
  wasm_instance_exports (instance, &exports);
  যদি (exports.size == 0)
    error (EXIT_FAILURE, 0, "error getting instance exports");

  run_func = wasi_get_start_function (instance);
  যদি (run_func == রিক্ত)
    error (EXIT_FAILURE, 0, "error accessing export");

  যদি (wasm_func_call (run_func, &args, &res))
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

স্থির পূর্ণাঙ্ক
libwasmer_load (শূন্য **cookie, libcrun_error_t *err)
{
  শূন্য *handle;

  handle = dlopen ("libwasmer.so", RTLD_NOW);
  যদি (handle == রিক্ত)
    প্রত্যাগম crun_make_error (err, 0, "could not load `libwasmer.so`: %s", dlerror ());
  *cookie = handle;

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
libwasmer_unload (শূন্য *cookie, libcrun_error_t *err)
{
  পূর্ণাঙ্ক r;

  যদি (cookie)
    {
      r = dlclose (cookie);
      যদি (UNLIKELY (r < 0))
        প্রত্যাগম crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
libwasmer_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  প্রত্যাগম wasm_can_handle_container (container, err);
}

সংরচনা custom_handler_s handler_wasmer = {
  .name = "wasmer",
  .alias = "wasm",
  .feature_string = "WASM:wasmer",
  .supports_open_বৃক্ষ_namespace = false,
  .load = libwasmer_load,
  .unload = libwasmer_unload,
  .run_func = libwasmer_exec,
  .can_handle_container = libwasmer_can_handle_container,
};

#endif
