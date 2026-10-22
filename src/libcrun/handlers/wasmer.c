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
#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ "../custom-handler.h"
#అన్తర్భావ "../container.h"
#అన్తర్భావ "../utils.h"
#అన్తర్భావ "../linux.h"
#అన్తర్భావ "handler-utils.h"
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <errno.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <sched.h>

#నిర్వచితమైతే HAVE_DLOPEN
#  include <dlfcn.h>
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_WASMER
#  include <wasmer.h>
#అయితే_ముగింపు

#ఒకవేళ HAVE_DLOPEN && HAVE_WASMER
#  define WASMER_BUF_కొలత 128
స్థిర పూర్ణాంకం
libwasmer_exec (శూన్య *cookie, libcrun_container_t *container arg_వాడని,
                స్థిరరాశి అక్షరం *pathname, అక్షరం *స్థిరరాశి argv[])
{
  పూర్ణాంకం ret;
  అక్షరం బఫరు[WASMER_BUF_కొలత] = { 0 };
  size_t దత్తాంశం_read_కొలత = WASMER_BUF_కొలత;
  స్థిరరాశి wasm_func_t *core_func;
  FILE *wat_wasm_file;
  size_t file_కొలత;
  wasm_byte_vec_t wat;
  wasm_byte_vec_t ద్విమాన_బైట్లు;
  wasm_byte_vec_t wasm_బైట్లు;
  wasm_engine_t *engine;
  wasm_val_t results_val[1] = { WASM_INIT_VAL };
  wasm_store_t *store;
  wasm_module_t *module;
  wasm_instance_t *instance;
  wasm_extern_vec_t exports;
  size_t వాదనలు_కొలత = 0;
  cleanup_free అక్షరం *wasi_వాదనలు = ఖాళీ;
  wasi_config_t *config;
  అక్షరం *స్థిరరాశి *arg;
  wasi_env_t *wasi_env;
  wasm_importtype_vec_t import_types;
  wasm_extern_vec_t imports;
  wasm_func_t *నిరంతరం_func;
  wasm_val_vec_t వాదనలు = WASM_ఖాళీలు_VEC;
  wasm_val_vec_t res = WASM_ఖాళీలు_VEC;

  wasm_engine_t *(*wasm_engine_new) ();
  శూన్య (*wat2wasm) (స్థిరరాశి wasm_byte_vec_t *wat, wasm_byte_vec_t *బహిర్గతం);
  wasm_module_t *(*wasm_module_new) (wasm_store_t *, స్థిరరాశి wasm_byte_vec_t *ద్విమాన);
  wasm_store_t *(*wasm_store_new) (wasm_engine_t *);
  wasm_instance_t *(*wasm_instance_new) (wasm_store_t *, స్థిరరాశి wasm_module_t *, స్థిరరాశి wasm_extern_vec_t *imports, wasm_trap_t **);
  శూన్య (*wasm_instance_exports) (స్థిరరాశి wasm_instance_t *, wasm_extern_vec_t *బహిర్గతం);
  wasm_func_t *(*wasm_extern_as_func) (wasm_extern_t *);
  శూన్య (*wasm_module_delete) (wasm_module_t *);
  శూన్య (*wasm_instance_delete) (wasm_instance_t *);
  శూన్య (*wasm_store_delete) (wasm_store_t *);
  శూన్య (*wasm_engine_delete) (wasm_engine_t *);
  శూన్య (*wasm_byte_vec_new) (wasm_byte_vec_t *, size_t, స్థిరరాశి అక్షరం *);
  శూన్య (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  శూన్య (*wasm_importtype_vec_delete) (wasm_importtype_vec_t *);
  శూన్య (*wasm_extern_vec_delete) (wasm_extern_vec_t *);
  శూన్య (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  శూన్య (*wasm_extern_vec_new_uninitialized) (wasm_extern_vec_t *, size_t);
  శూన్య (*wasi_config_capture_stdout) (నిర్మాణం wasi_config_t *);
  శూన్య (*wasi_config_inherit_stdout) (నిర్మాణం wasi_config_t *);
  శూన్య (*wasm_module_imports) (స్థిరరాశి wasm_module_t *, wasm_importtype_vec_t *);
  శూన్య (*wasm_func_delete) (wasm_func_t *);
  wasm_trap_t *(*wasm_func_call) (స్థిరరాశి wasm_func_t *, స్థిరరాశి wasm_val_vec_t *వాదనలు, wasm_val_vec_t *results);
  wasi_config_t *(*wasi_config_new) (స్థిరరాశి అక్షరం *);
  wasi_env_t *(*wasi_env_new) (wasm_store_t *, నిర్మాణం wasi_config_t *);
  bool (*wasi_get_imports) (స్థిరరాశి wasm_store_t *, స్థిరరాశి నిర్మాణం wasi_env_t *, స్థిరరాశి wasm_module_t *, wasm_extern_vec_t *);
  wasm_func_t *(*wasi_get_ప్రారంభం_function) (wasm_instance_t *);
  intptr_t (*wasi_env_read_stdout) (నిర్మాణం wasi_env_t *, అక్షరం *, uintptr_t);
  శూన్య (*wasi_env_delete) (నిర్మాణం wasi_env_t *);
  శూన్య (*wasi_config_arg) (నిర్మాణం wasi_config_t *config, స్థిరరాశి అక్షరం *arg);
  bool (*wasi_env_initialize_instance) (నిర్మాణం wasi_env_t *, wasm_store_t *, wasm_instance_t *);

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
  wasi_get_ప్రారంభం_function = dlsym (cookie, "wasi_get_start_function");
  wasi_env_read_stdout = dlsym (cookie, "wasi_env_read_stdout");
  wasi_env_delete = dlsym (cookie, "wasi_env_delete");
  wasm_func_delete = dlsym (cookie, "wasm_func_delete");
  wasi_env_initialize_instance = dlsym (cookie, "wasi_env_initialize_instance");

  ఒకవేళ (wat2wasm == ఖాళీ || wasm_module_delete == ఖాళీ || wasm_instance_delete == ఖాళీ
      || wasm_engine_delete == ఖాళీ || wasm_store_delete == ఖాళీ || wasm_func_call == ఖాళీ
      || wasm_extern_as_func == ఖాళీ || wasm_instance_exports == ఖాళీ || wasm_instance_new == ఖాళీ
      || wasm_store_new == ఖాళీ || wasm_engine_new == ఖాళీ || wasm_byte_vec_new == ఖాళీ
      || wasm_byte_vec_delete == ఖాళీ || wasm_extern_vec_delete == ఖాళీ
      || wasm_byte_vec_new_uninitialized == ఖాళీ || wasi_config_new == ఖాళీ
      || wasi_config_capture_stdout == ఖాళీ || wasi_env_new == ఖాళీ || wasm_module_imports == ఖాళీ
      || wasi_env_read_stdout == ఖాళీ || wasi_env_delete == ఖాళీ || wasm_func_delete == ఖాళీ
      || wasm_importtype_vec_delete == ఖాళీ || wasm_extern_vec_new_uninitialized == ఖాళీ
      || wasi_get_imports == ఖాళీ || wasi_get_ప్రారంభం_function == ఖాళీ || wasi_config_inherit_stdout == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmer.so`");

  wat_wasm_file = fopen (pathname, "rbe");

  ఒకవేళ (! wat_wasm_file)
    error (EXIT_FAILURE, errno, "error opening wat/wasm module");

  fseek (wat_wasm_file, 0L, SEEK_END);
  file_కొలత = ftell (wat_wasm_file);
  fseek (wat_wasm_file, 0L, SEEK_SET);

  wasm_byte_vec_new_uninitialized (&ద్విమాన_బైట్లు, file_కొలత);

  ఒకవేళ (fread (ద్విమాన_బైట్లు.దత్తాంశం, file_కొలత, 1, wat_wasm_file) != 1)
    error (EXIT_FAILURE, errno, "error loading wat/wasm module");

  /* We can close entrypoint file.   */
  fclose (wat_wasm_file);

  /* We have received a wat file: convert wat to wasm.   */
  ఒకవేళ (has_suffix (pathname, "wat") > 0)
    {
      wat2wasm (&ద్విమాన_బైట్లు, &wasm_బైట్లు);
      ద్విమాన_బైట్లు = wasm_బైట్లు;
    }

  engine = wasm_engine_new ();
  store = wasm_store_new (engine);

  module = wasm_module_new (store, &ద్విమాన_బైట్లు);

  ఒకవేళ (! module)
    error (EXIT_FAILURE, 0, "error compiling wasm module");

  config = wasi_config_new ("crun_wasi_program");

  /* Count number of external arguments given.  */
  కొరకు (arg = argv; *arg != ఖాళీ; ++arg)
    వాదనలు_కొలత++;

  ఒకవేళ (వాదనలు_కొలత > 1)
    {
      wasi_వాదనలు = str_join_array (1, వాదనలు_కొలత, argv, " ");
      wasi_config_arg (config, wasi_వాదనలు);
    }

  wasi_config_inherit_stdout (config);
  wasi_env = wasi_env_new (store, config);
  ఒకవేళ (! wasi_env)
    {
      error (EXIT_FAILURE, 0, "error building wasi env");
    }

  /* Instantiate.  */
  ఒకవేళ (! wasi_get_imports (store, wasi_env, module, &imports))
    error (EXIT_FAILURE, 0, "error getting WASI imports");

  instance = wasm_instance_new (store, module, &imports, ఖాళీ);

  ఒకవేళ (! instance)
    error (EXIT_FAILURE, 0, "error instantiating module");

  ఒకవేళ (! wasi_env_initialize_instance (wasi_env, store, instance))
    error (EXIT_FAILURE, 0, "error init wasi env");

  /* Extract export.  */
  wasm_instance_exports (instance, &exports);
  ఒకవేళ (exports.కొలత == 0)
    error (EXIT_FAILURE, 0, "error getting instance exports");

  నిరంతరం_func = wasi_get_ప్రారంభం_function (instance);
  ఒకవేళ (నిరంతరం_func == ఖాళీ)
    error (EXIT_FAILURE, 0, "error accessing export");

  ఒకవేళ (wasm_func_call (నిరంతరం_func, &వాదనలు, &res))
    error (EXIT_FAILURE, 0, "error calling wasm function");

  wasm_extern_vec_delete (&exports);
  wasm_extern_vec_delete (&imports);

  /* Shut down.  */
  wasm_module_delete (module);
  wasm_instance_delete (instance);
  wasm_func_delete (నిరంతరం_func);
  wasi_env_delete (wasi_env);
  wasm_store_delete (store);
  wasm_engine_delete (engine);

  నిష్క్రమించు (EXIT_SUCCESS);
}

స్థిర పూర్ణాంకం
libwasmer_ఎక్కించు (శూన్య **cookie, libcrun_error_t *err)
{
  శూన్య *handle;

  handle = dlopen ("libwasmer.so", RTLD_NOW);
  ఒకవేళ (handle == ఖాళీ)
    తిరిగి crun_make_error (err, 0, "could not load `libwasmer.so`: %s", dlerror ());
  *cookie = handle;

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
libwasmer_unload (శూన్య *cookie, libcrun_error_t *err)
{
  పూర్ణాంకం r;

  ఒకవేళ (cookie)
    {
      r = dlclose (cookie);
      ఒకవేళ (UNLIKELY (r < 0))
        తిరిగి crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
libwasmer_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  తిరిగి wasm_can_handle_container (container, err);
}

నిర్మాణం custom_handler_s handler_wasmer = {
  .name = "wasmer",
  .alias = "wasm",
  .feature_string = "WASM:wasmer",
  .supports_open_వృక్షం_namespace = false,
  .ఎక్కించు = libwasmer_ఎక్కించు,
  .unload = libwasmer_unload,
  .నిరంతరం_func = libwasmer_exec,
  .can_handle_container = libwasmer_can_handle_container,
};

#అయితే_ముగింపు
