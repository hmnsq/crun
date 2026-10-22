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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "../custom-handler.h"
#અન્તર્ભાવ "../container.h"
#અન્તર્ભાવ "../utils.h"
#અન્તર્ભાવ "../linux.h"
#અન્તર્ભાવ "handler-utils.h"
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMER
#  include <wasmer.h>
#endif

#જો HAVE_DLOPEN && HAVE_WASMER
#  define WASMER_BUF_SIZE 128
સ્થિર પૂર્ણાંક
libwasmer_exec (શૂન્ય *cookie, libcrun_container_t *container arg_unused,
                અચલ અક્ષર *pathname, અક્ષર *અચલ argv[])
{
  પૂર્ણાંક ret;
  અક્ષર buffer[WASMER_BUF_SIZE] = { 0 };
  size_t data_read_size = WASMER_BUF_SIZE;
  અચલ wasm_func_t *core_func;
  FILE *wat_wasm_file;
  size_t file_size;
  wasm_byte_vec_t wat;
  wasm_byte_vec_t દ્વિઅંકી_bytes;
  wasm_byte_vec_t wasm_bytes;
  wasm_engine_t *engine;
  wasm_val_t results_val[1] = { WASM_INIT_VAL };
  wasm_store_t *store;
  wasm_module_t *module;
  wasm_instance_t *instance;
  wasm_extern_vec_t exports;
  size_t args_size = 0;
  cleanup_free અક્ષર *wasi_args = ખાલી;
  wasi_config_t *config;
  અક્ષર *અચલ *arg;
  wasi_env_t *wasi_env;
  wasm_importtype_vec_t import_types;
  wasm_extern_vec_t imports;
  wasm_func_t *run_func;
  wasm_val_vec_t args = WASM_EMPTY_VEC;
  wasm_val_vec_t res = WASM_EMPTY_VEC;

  wasm_engine_t *(*wasm_engine_new) ();
  શૂન્ય (*wat2wasm) (અચલ wasm_byte_vec_t *wat, wasm_byte_vec_t *out);
  wasm_module_t *(*wasm_module_new) (wasm_store_t *, અચલ wasm_byte_vec_t *દ્વિઅંકી);
  wasm_store_t *(*wasm_store_new) (wasm_engine_t *);
  wasm_instance_t *(*wasm_instance_new) (wasm_store_t *, અચલ wasm_module_t *, અચલ wasm_extern_vec_t *imports, wasm_trap_t **);
  શૂન્ય (*wasm_instance_exports) (અચલ wasm_instance_t *, wasm_extern_vec_t *out);
  wasm_func_t *(*wasm_extern_as_func) (wasm_extern_t *);
  શૂન્ય (*wasm_module_delete) (wasm_module_t *);
  શૂન્ય (*wasm_instance_delete) (wasm_instance_t *);
  શૂન્ય (*wasm_store_delete) (wasm_store_t *);
  શૂન્ય (*wasm_engine_delete) (wasm_engine_t *);
  શૂન્ય (*wasm_byte_vec_new) (wasm_byte_vec_t *, size_t, અચલ અક્ષર *);
  શૂન્ય (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  શૂન્ય (*wasm_importtype_vec_delete) (wasm_importtype_vec_t *);
  શૂન્ય (*wasm_extern_vec_delete) (wasm_extern_vec_t *);
  શૂન્ય (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  શૂન્ય (*wasm_extern_vec_new_uninitialized) (wasm_extern_vec_t *, size_t);
  શૂન્ય (*wasi_config_capture_stdout) (સંરચના wasi_config_t *);
  શૂન્ય (*wasi_config_inherit_stdout) (સંરચના wasi_config_t *);
  શૂન્ય (*wasm_module_imports) (અચલ wasm_module_t *, wasm_importtype_vec_t *);
  શૂન્ય (*wasm_func_delete) (wasm_func_t *);
  wasm_trap_t *(*wasm_func_call) (અચલ wasm_func_t *, અચલ wasm_val_vec_t *args, wasm_val_vec_t *results);
  wasi_config_t *(*wasi_config_new) (અચલ અક્ષર *);
  wasi_env_t *(*wasi_env_new) (wasm_store_t *, સંરચના wasi_config_t *);
  bool (*wasi_get_imports) (અચલ wasm_store_t *, અચલ સંરચના wasi_env_t *, અચલ wasm_module_t *, wasm_extern_vec_t *);
  wasm_func_t *(*wasi_get_start_function) (wasm_instance_t *);
  intptr_t (*wasi_env_read_stdout) (સંરચના wasi_env_t *, અક્ષર *, uintptr_t);
  શૂન્ય (*wasi_env_delete) (સંરચના wasi_env_t *);
  શૂન્ય (*wasi_config_arg) (સંરચના wasi_config_t *config, અચલ અક્ષર *arg);
  bool (*wasi_env_initialize_instance) (સંરચના wasi_env_t *, wasm_store_t *, wasm_instance_t *);

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

  જો (wat2wasm == ખાલી || wasm_module_delete == ખાલી || wasm_instance_delete == ખાલી
      || wasm_engine_delete == ખાલી || wasm_store_delete == ખાલી || wasm_func_call == ખાલી
      || wasm_extern_as_func == ખાલી || wasm_instance_exports == ખાલી || wasm_instance_new == ખાલી
      || wasm_store_new == ખાલી || wasm_engine_new == ખાલી || wasm_byte_vec_new == ખાલી
      || wasm_byte_vec_delete == ખાલી || wasm_extern_vec_delete == ખાલી
      || wasm_byte_vec_new_uninitialized == ખાલી || wasi_config_new == ખાલી
      || wasi_config_capture_stdout == ખાલી || wasi_env_new == ખાલી || wasm_module_imports == ખાલી
      || wasi_env_read_stdout == ખાલી || wasi_env_delete == ખાલી || wasm_func_delete == ખાલી
      || wasm_importtype_vec_delete == ખાલી || wasm_extern_vec_new_uninitialized == ખાલી
      || wasi_get_imports == ખાલી || wasi_get_start_function == ખાલી || wasi_config_inherit_stdout == ખાલી)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmer.so`");

  wat_wasm_file = fopen (pathname, "rbe");

  જો (! wat_wasm_file)
    error (EXIT_FAILURE, errno, "error opening wat/wasm module");

  fseek (wat_wasm_file, 0L, SEEK_END);
  file_size = ftell (wat_wasm_file);
  fseek (wat_wasm_file, 0L, SEEK_SET);

  wasm_byte_vec_new_uninitialized (&દ્વિઅંકી_bytes, file_size);

  જો (fread (દ્વિઅંકી_bytes.data, file_size, 1, wat_wasm_file) != 1)
    error (EXIT_FAILURE, errno, "error loading wat/wasm module");

  /* We can close entrypoint file.   */
  fclose (wat_wasm_file);

  /* We have received a wat file: convert wat to wasm.   */
  જો (has_suffix (pathname, "wat") > 0)
    {
      wat2wasm (&દ્વિઅંકી_bytes, &wasm_bytes);
      દ્વિઅંકી_bytes = wasm_bytes;
    }

  engine = wasm_engine_new ();
  store = wasm_store_new (engine);

  module = wasm_module_new (store, &દ્વિઅંકી_bytes);

  જો (! module)
    error (EXIT_FAILURE, 0, "error compiling wasm module");

  config = wasi_config_new ("crun_wasi_program");

  /* Count number of external arguments given.  */
  માટે (arg = argv; *arg != ખાલી; ++arg)
    args_size++;

  જો (args_size > 1)
    {
      wasi_args = str_join_array (1, args_size, argv, " ");
      wasi_config_arg (config, wasi_args);
    }

  wasi_config_inherit_stdout (config);
  wasi_env = wasi_env_new (store, config);
  જો (! wasi_env)
    {
      error (EXIT_FAILURE, 0, "error building wasi env");
    }

  /* Instantiate.  */
  જો (! wasi_get_imports (store, wasi_env, module, &imports))
    error (EXIT_FAILURE, 0, "error getting WASI imports");

  instance = wasm_instance_new (store, module, &imports, ખાલી);

  જો (! instance)
    error (EXIT_FAILURE, 0, "error instantiating module");

  જો (! wasi_env_initialize_instance (wasi_env, store, instance))
    error (EXIT_FAILURE, 0, "error init wasi env");

  /* Extract export.  */
  wasm_instance_exports (instance, &exports);
  જો (exports.size == 0)
    error (EXIT_FAILURE, 0, "error getting instance exports");

  run_func = wasi_get_start_function (instance);
  જો (run_func == ખાલી)
    error (EXIT_FAILURE, 0, "error accessing export");

  જો (wasm_func_call (run_func, &args, &res))
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

સ્થિર પૂર્ણાંક
libwasmer_load (શૂન્ય **cookie, libcrun_error_t *err)
{
  શૂન્ય *handle;

  handle = dlopen ("libwasmer.so", RTLD_NOW);
  જો (handle == ખાલી)
    પાછા crun_make_error (err, 0, "could not load `libwasmer.so`: %s", dlerror ());
  *cookie = handle;

  પાછા 0;
}

સ્થિર પૂર્ણાંક
libwasmer_unload (શૂન્ય *cookie, libcrun_error_t *err)
{
  પૂર્ણાંક r;

  જો (cookie)
    {
      r = dlclose (cookie);
      જો (UNLIKELY (r < 0))
        પાછા crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
libwasmer_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  પાછા wasm_can_handle_container (container, err);
}

સંરચના custom_handler_s handler_wasmer = {
  .name = "wasmer",
  .alias = "wasm",
  .feature_string = "WASM:wasmer",
  .supports_open_વૃક્ષ_namespace = false,
  .load = libwasmer_load,
  .unload = libwasmer_unload,
  .run_func = libwasmer_exec,
  .can_handle_container = libwasmer_can_handle_container,
};

#endif
