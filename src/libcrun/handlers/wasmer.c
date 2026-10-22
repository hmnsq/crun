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
#تعریف_کرو GNU_ماخذ

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

#اگر_تعریف_شدہ HAVE_DLOPEN
#  include <dlfcn.h>
#اگر_ختم

#اگر_تعریف_شدہ HAVE_WASMER
#  include <wasmer.h>
#اگر_ختم

#اگر HAVE_DLOPEN && HAVE_WASMER
#  define WASMER_BUF_سائز 128
ساکن صحیح
libwasmer_exec (خالی *cookie, libcrun_container_t *container arg_غیر_مستعمل,
                ثابت حرف *pathname, حرف *ثابت argv[])
{
  صحیح ret;
  حرف بفر[WASMER_BUF_سائز] = { 0 };
  size_t ڈیٹا_read_سائز = WASMER_BUF_سائز;
  ثابت wasm_func_t *core_func;
  FILE *wat_wasm_file;
  size_t file_سائز;
  wasm_byte_vec_t wat;
  wasm_byte_vec_t ثنائی_بائٹیں;
  wasm_byte_vec_t wasm_بائٹیں;
  wasm_engine_t *engine;
  wasm_val_t results_val[1] = { WASM_INIT_VAL };
  wasm_store_t *store;
  wasm_module_t *module;
  wasm_instance_t *instance;
  wasm_extern_vec_t exports;
  size_t دلائل_سائز = 0;
  cleanup_free حرف *wasi_دلائل = تہی;
  wasi_config_t *config;
  حرف *ثابت *arg;
  wasi_env_t *wasi_env;
  wasm_importtype_vec_t import_types;
  wasm_extern_vec_t imports;
  wasm_func_t *تسلسل_func;
  wasm_val_vec_t دلائل = WASM_خالی_خانے_VEC;
  wasm_val_vec_t res = WASM_خالی_خانے_VEC;

  wasm_engine_t *(*wasm_engine_new) ();
  خالی (*wat2wasm) (ثابت wasm_byte_vec_t *wat, wasm_byte_vec_t *برآمد);
  wasm_module_t *(*wasm_module_new) (wasm_store_t *, ثابت wasm_byte_vec_t *ثنائی);
  wasm_store_t *(*wasm_store_new) (wasm_engine_t *);
  wasm_instance_t *(*wasm_instance_new) (wasm_store_t *, ثابت wasm_module_t *, ثابت wasm_extern_vec_t *imports, wasm_trap_t **);
  خالی (*wasm_instance_exports) (ثابت wasm_instance_t *, wasm_extern_vec_t *برآمد);
  wasm_func_t *(*wasm_extern_as_func) (wasm_extern_t *);
  خالی (*wasm_module_delete) (wasm_module_t *);
  خالی (*wasm_instance_delete) (wasm_instance_t *);
  خالی (*wasm_store_delete) (wasm_store_t *);
  خالی (*wasm_engine_delete) (wasm_engine_t *);
  خالی (*wasm_byte_vec_new) (wasm_byte_vec_t *, size_t, ثابت حرف *);
  خالی (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  خالی (*wasm_importtype_vec_delete) (wasm_importtype_vec_t *);
  خالی (*wasm_extern_vec_delete) (wasm_extern_vec_t *);
  خالی (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  خالی (*wasm_extern_vec_new_uninitialized) (wasm_extern_vec_t *, size_t);
  خالی (*wasi_config_capture_stdout) (ساخت wasi_config_t *);
  خالی (*wasi_config_inherit_stdout) (ساخت wasi_config_t *);
  خالی (*wasm_module_imports) (ثابت wasm_module_t *, wasm_importtype_vec_t *);
  خالی (*wasm_func_delete) (wasm_func_t *);
  wasm_trap_t *(*wasm_func_call) (ثابت wasm_func_t *, ثابت wasm_val_vec_t *دلائل, wasm_val_vec_t *results);
  wasi_config_t *(*wasi_config_new) (ثابت حرف *);
  wasi_env_t *(*wasi_env_new) (wasm_store_t *, ساخت wasi_config_t *);
  bool (*wasi_get_imports) (ثابت wasm_store_t *, ثابت ساخت wasi_env_t *, ثابت wasm_module_t *, wasm_extern_vec_t *);
  wasm_func_t *(*wasi_get_آغاز_function) (wasm_instance_t *);
  intptr_t (*wasi_env_read_stdout) (ساخت wasi_env_t *, حرف *, uintptr_t);
  خالی (*wasi_env_delete) (ساخت wasi_env_t *);
  خالی (*wasi_config_arg) (ساخت wasi_config_t *config, ثابت حرف *arg);
  bool (*wasi_env_initialize_instance) (ساخت wasi_env_t *, wasm_store_t *, wasm_instance_t *);

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
  wasi_get_آغاز_function = dlsym (cookie, "wasi_get_start_function");
  wasi_env_read_stdout = dlsym (cookie, "wasi_env_read_stdout");
  wasi_env_delete = dlsym (cookie, "wasi_env_delete");
  wasm_func_delete = dlsym (cookie, "wasm_func_delete");
  wasi_env_initialize_instance = dlsym (cookie, "wasi_env_initialize_instance");

  اگر (wat2wasm == تہی || wasm_module_delete == تہی || wasm_instance_delete == تہی
      || wasm_engine_delete == تہی || wasm_store_delete == تہی || wasm_func_call == تہی
      || wasm_extern_as_func == تہی || wasm_instance_exports == تہی || wasm_instance_new == تہی
      || wasm_store_new == تہی || wasm_engine_new == تہی || wasm_byte_vec_new == تہی
      || wasm_byte_vec_delete == تہی || wasm_extern_vec_delete == تہی
      || wasm_byte_vec_new_uninitialized == تہی || wasi_config_new == تہی
      || wasi_config_capture_stdout == تہی || wasi_env_new == تہی || wasm_module_imports == تہی
      || wasi_env_read_stdout == تہی || wasi_env_delete == تہی || wasm_func_delete == تہی
      || wasm_importtype_vec_delete == تہی || wasm_extern_vec_new_uninitialized == تہی
      || wasi_get_imports == تہی || wasi_get_آغاز_function == تہی || wasi_config_inherit_stdout == تہی)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmer.so`");

  wat_wasm_file = fopen (pathname, "rbe");

  اگر (! wat_wasm_file)
    error (EXIT_FAILURE, errno, "error opening wat/wasm module");

  fseek (wat_wasm_file, 0L, SEEK_END);
  file_سائز = ftell (wat_wasm_file);
  fseek (wat_wasm_file, 0L, SEEK_SET);

  wasm_byte_vec_new_uninitialized (&ثنائی_بائٹیں, file_سائز);

  اگر (fread (ثنائی_بائٹیں.ڈیٹا, file_سائز, 1, wat_wasm_file) != 1)
    error (EXIT_FAILURE, errno, "error loading wat/wasm module");

  /* We can close entrypoint file.   */
  fclose (wat_wasm_file);

  /* We have received a wat file: convert wat to wasm.   */
  اگر (has_suffix (pathname, "wat") > 0)
    {
      wat2wasm (&ثنائی_بائٹیں, &wasm_بائٹیں);
      ثنائی_بائٹیں = wasm_بائٹیں;
    }

  engine = wasm_engine_new ();
  store = wasm_store_new (engine);

  module = wasm_module_new (store, &ثنائی_بائٹیں);

  اگر (! module)
    error (EXIT_FAILURE, 0, "error compiling wasm module");

  config = wasi_config_new ("crun_wasi_program");

  /* Count number of external arguments given.  */
  برائے (arg = argv; *arg != تہی; ++arg)
    دلائل_سائز++;

  اگر (دلائل_سائز > 1)
    {
      wasi_دلائل = str_join_array (1, دلائل_سائز, argv, " ");
      wasi_config_arg (config, wasi_دلائل);
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

  instance = wasm_instance_new (store, module, &imports, تہی);

  اگر (! instance)
    error (EXIT_FAILURE, 0, "error instantiating module");

  اگر (! wasi_env_initialize_instance (wasi_env, store, instance))
    error (EXIT_FAILURE, 0, "error init wasi env");

  /* Extract export.  */
  wasm_instance_exports (instance, &exports);
  اگر (exports.سائز == 0)
    error (EXIT_FAILURE, 0, "error getting instance exports");

  تسلسل_func = wasi_get_آغاز_function (instance);
  اگر (تسلسل_func == تہی)
    error (EXIT_FAILURE, 0, "error accessing export");

  اگر (wasm_func_call (تسلسل_func, &دلائل, &res))
    error (EXIT_FAILURE, 0, "error calling wasm function");

  wasm_extern_vec_delete (&exports);
  wasm_extern_vec_delete (&imports);

  /* Shut down.  */
  wasm_module_delete (module);
  wasm_instance_delete (instance);
  wasm_func_delete (تسلسل_func);
  wasi_env_delete (wasi_env);
  wasm_store_delete (store);
  wasm_engine_delete (engine);

  باہر_نکلو (EXIT_SUCCESS);
}

ساکن صحیح
libwasmer_بھرو (خالی **cookie, libcrun_error_t *err)
{
  خالی *handle;

  handle = dlopen ("libwasmer.so", RTLD_NOW);
  اگر (handle == تہی)
    واپس crun_make_error (err, 0, "could not load `libwasmer.so`: %s", dlerror ());
  *cookie = handle;

  واپس 0;
}

ساکن صحیح
libwasmer_unload (خالی *cookie, libcrun_error_t *err)
{
  صحیح r;

  اگر (cookie)
    {
      r = dlclose (cookie);
      اگر (UNLIKELY (r < 0))
        واپس crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  واپس 0;
}

ساکن صحیح
libwasmer_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  واپس wasm_can_handle_container (container, err);
}

ساخت custom_handler_s handler_wasmer = {
  .name = "wasmer",
  .alias = "wasm",
  .feature_string = "WASM:wasmer",
  .supports_open_درخت_namespace = false,
  .بھرو = libwasmer_بھرو,
  .unload = libwasmer_unload,
  .تسلسل_func = libwasmer_exec,
  .can_handle_container = libwasmer_can_handle_container,
};

#اگر_ختم
