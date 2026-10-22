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
#تعریف_کن GNU_منبع

#ضمن <config.h>
#ضمن "../custom-handler.h"
#ضمن "../container.h"
#ضمن "../utils.h"
#ضمن "../linux.h"
#ضمن "handler-utils.h"
#ضمن <unistd.h>
#ضمن <sys/stat.h>
#ضمن <errno.h>
#ضمن <sys/types.h>
#ضمن <fcntl.h>
#ضمن <sched.h>

#اگر_تعریف_شده HAVE_DLOPEN
#  include <dlfcn.h>
#پایان_اگر

#اگر_تعریف_شده HAVE_WASMER
#  include <wasmer.h>
#پایان_اگر

#إذا HAVE_DLOPEN && HAVE_WASMER
#  define WASMER_BUF_اندازه 128
ساكن صحيح
libwasmer_exec (فراغ *cookie, libcrun_container_t *container arg_استفادهنشده,
                ثابت حرف *pathname, حرف *ثابت argv[])
{
  صحيح ret;
  حرف میانگیر[WASMER_BUF_اندازه] = { 0 };
  size_t داده_read_اندازه = WASMER_BUF_اندازه;
  ثابت wasm_func_t *core_func;
  FILE *wat_wasm_file;
  size_t file_اندازه;
  wasm_byte_vec_t wat;
  wasm_byte_vec_t دودویی_بایتها;
  wasm_byte_vec_t wasm_بایتها;
  wasm_engine_t *engine;
  wasm_val_t results_val[1] = { WASM_INIT_VAL };
  wasm_store_t *store;
  wasm_module_t *module;
  wasm_instance_t *instance;
  wasm_extern_vec_t exports;
  size_t آرگومانها_اندازه = 0;
  cleanup_free حرف *wasi_آرگومانها = عدم;
  wasi_config_t *config;
  حرف *ثابت *arg;
  wasi_env_t *wasi_env;
  wasm_importtype_vec_t import_types;
  wasm_extern_vec_t imports;
  wasm_func_t *پیاپی_func;
  wasm_val_vec_t آرگومانها = WASM_خالی_VEC;
  wasm_val_vec_t res = WASM_خالی_VEC;

  wasm_engine_t *(*wasm_engine_new) ();
  فراغ (*wat2wasm) (ثابت wasm_byte_vec_t *wat, wasm_byte_vec_t *بیرون);
  wasm_module_t *(*wasm_module_new) (wasm_store_t *, ثابت wasm_byte_vec_t *دودویی);
  wasm_store_t *(*wasm_store_new) (wasm_engine_t *);
  wasm_instance_t *(*wasm_instance_new) (wasm_store_t *, ثابت wasm_module_t *, ثابت wasm_extern_vec_t *imports, wasm_trap_t **);
  فراغ (*wasm_instance_exports) (ثابت wasm_instance_t *, wasm_extern_vec_t *بیرون);
  wasm_func_t *(*wasm_extern_as_func) (wasm_extern_t *);
  فراغ (*wasm_module_delete) (wasm_module_t *);
  فراغ (*wasm_instance_delete) (wasm_instance_t *);
  فراغ (*wasm_store_delete) (wasm_store_t *);
  فراغ (*wasm_engine_delete) (wasm_engine_t *);
  فراغ (*wasm_byte_vec_new) (wasm_byte_vec_t *, size_t, ثابت حرف *);
  فراغ (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  فراغ (*wasm_importtype_vec_delete) (wasm_importtype_vec_t *);
  فراغ (*wasm_extern_vec_delete) (wasm_extern_vec_t *);
  فراغ (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  فراغ (*wasm_extern_vec_new_uninitialized) (wasm_extern_vec_t *, size_t);
  فراغ (*wasi_config_capture_stdout) (بنية wasi_config_t *);
  فراغ (*wasi_config_inherit_stdout) (بنية wasi_config_t *);
  فراغ (*wasm_module_imports) (ثابت wasm_module_t *, wasm_importtype_vec_t *);
  فراغ (*wasm_func_delete) (wasm_func_t *);
  wasm_trap_t *(*wasm_func_call) (ثابت wasm_func_t *, ثابت wasm_val_vec_t *آرگومانها, wasm_val_vec_t *results);
  wasi_config_t *(*wasi_config_new) (ثابت حرف *);
  wasi_env_t *(*wasi_env_new) (wasm_store_t *, بنية wasi_config_t *);
  bool (*wasi_get_imports) (ثابت wasm_store_t *, ثابت بنية wasi_env_t *, ثابت wasm_module_t *, wasm_extern_vec_t *);
  wasm_func_t *(*wasi_get_شروع_function) (wasm_instance_t *);
  intptr_t (*wasi_env_read_stdout) (بنية wasi_env_t *, حرف *, uintptr_t);
  فراغ (*wasi_env_delete) (بنية wasi_env_t *);
  فراغ (*wasi_config_arg) (بنية wasi_config_t *config, ثابت حرف *arg);
  bool (*wasi_env_initialize_instance) (بنية wasi_env_t *, wasm_store_t *, wasm_instance_t *);

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
  wasi_get_شروع_function = dlsym (cookie, "wasi_get_start_function");
  wasi_env_read_stdout = dlsym (cookie, "wasi_env_read_stdout");
  wasi_env_delete = dlsym (cookie, "wasi_env_delete");
  wasm_func_delete = dlsym (cookie, "wasm_func_delete");
  wasi_env_initialize_instance = dlsym (cookie, "wasi_env_initialize_instance");

  إذا (wat2wasm == عدم || wasm_module_delete == عدم || wasm_instance_delete == عدم
      || wasm_engine_delete == عدم || wasm_store_delete == عدم || wasm_func_call == عدم
      || wasm_extern_as_func == عدم || wasm_instance_exports == عدم || wasm_instance_new == عدم
      || wasm_store_new == عدم || wasm_engine_new == عدم || wasm_byte_vec_new == عدم
      || wasm_byte_vec_delete == عدم || wasm_extern_vec_delete == عدم
      || wasm_byte_vec_new_uninitialized == عدم || wasi_config_new == عدم
      || wasi_config_capture_stdout == عدم || wasi_env_new == عدم || wasm_module_imports == عدم
      || wasi_env_read_stdout == عدم || wasi_env_delete == عدم || wasm_func_delete == عدم
      || wasm_importtype_vec_delete == عدم || wasm_extern_vec_new_uninitialized == عدم
      || wasi_get_imports == عدم || wasi_get_شروع_function == عدم || wasi_config_inherit_stdout == عدم)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmer.so`");

  wat_wasm_file = fopen (pathname, "rbe");

  إذا (! wat_wasm_file)
    error (EXIT_FAILURE, errno, "error opening wat/wasm module");

  fseek (wat_wasm_file, 0L, SEEK_END);
  file_اندازه = ftell (wat_wasm_file);
  fseek (wat_wasm_file, 0L, SEEK_SET);

  wasm_byte_vec_new_uninitialized (&دودویی_بایتها, file_اندازه);

  إذا (fread (دودویی_بایتها.داده, file_اندازه, 1, wat_wasm_file) != 1)
    error (EXIT_FAILURE, errno, "error loading wat/wasm module");

  /* We can close entrypoint file.   */
  fclose (wat_wasm_file);

  /* We have received a wat file: convert wat to wasm.   */
  إذا (has_suffix (pathname, "wat") > 0)
    {
      wat2wasm (&دودویی_بایتها, &wasm_بایتها);
      دودویی_بایتها = wasm_بایتها;
    }

  engine = wasm_engine_new ();
  store = wasm_store_new (engine);

  module = wasm_module_new (store, &دودویی_بایتها);

  إذا (! module)
    error (EXIT_FAILURE, 0, "error compiling wasm module");

  config = wasi_config_new ("crun_wasi_program");

  /* Count number of external arguments given.  */
  لكل (arg = argv; *arg != عدم; ++arg)
    آرگومانها_اندازه++;

  إذا (آرگومانها_اندازه > 1)
    {
      wasi_آرگومانها = str_join_array (1, آرگومانها_اندازه, argv, " ");
      wasi_config_arg (config, wasi_آرگومانها);
    }

  wasi_config_inherit_stdout (config);
  wasi_env = wasi_env_new (store, config);
  إذا (! wasi_env)
    {
      error (EXIT_FAILURE, 0, "error building wasi env");
    }

  /* Instantiate.  */
  إذا (! wasi_get_imports (store, wasi_env, module, &imports))
    error (EXIT_FAILURE, 0, "error getting WASI imports");

  instance = wasm_instance_new (store, module, &imports, عدم);

  إذا (! instance)
    error (EXIT_FAILURE, 0, "error instantiating module");

  إذا (! wasi_env_initialize_instance (wasi_env, store, instance))
    error (EXIT_FAILURE, 0, "error init wasi env");

  /* Extract export.  */
  wasm_instance_exports (instance, &exports);
  إذا (exports.اندازه == 0)
    error (EXIT_FAILURE, 0, "error getting instance exports");

  پیاپی_func = wasi_get_شروع_function (instance);
  إذا (پیاپی_func == عدم)
    error (EXIT_FAILURE, 0, "error accessing export");

  إذا (wasm_func_call (پیاپی_func, &آرگومانها, &res))
    error (EXIT_FAILURE, 0, "error calling wasm function");

  wasm_extern_vec_delete (&exports);
  wasm_extern_vec_delete (&imports);

  /* Shut down.  */
  wasm_module_delete (module);
  wasm_instance_delete (instance);
  wasm_func_delete (پیاپی_func);
  wasi_env_delete (wasi_env);
  wasm_store_delete (store);
  wasm_engine_delete (engine);

  خروج (EXIT_SUCCESS);
}

ساكن صحيح
libwasmer_بارگیری (فراغ **cookie, libcrun_error_t *err)
{
  فراغ *handle;

  handle = dlopen ("libwasmer.so", RTLD_NOW);
  إذا (handle == عدم)
    أرجع crun_make_error (err, 0, "could not load `libwasmer.so`: %s", dlerror ());
  *cookie = handle;

  أرجع 0;
}

ساكن صحيح
libwasmer_unload (فراغ *cookie, libcrun_error_t *err)
{
  صحيح r;

  إذا (cookie)
    {
      r = dlclose (cookie);
      إذا (UNLIKELY (r < 0))
        أرجع crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  أرجع 0;
}

ساكن صحيح
libwasmer_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  أرجع wasm_can_handle_container (container, err);
}

بنية custom_handler_s handler_wasmer = {
  .name = "wasmer",
  .alias = "wasm",
  .feature_string = "WASM:wasmer",
  .supports_open_درخت_namespace = false,
  .بارگیری = libwasmer_بارگیری,
  .unload = libwasmer_unload,
  .پیاپی_func = libwasmer_exec,
  .can_handle_container = libwasmer_can_handle_container,
};

#پایان_اگر
