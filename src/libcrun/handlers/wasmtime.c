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
#అన్తర్భావ <assert.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>

#నిర్వచితమైతే HAVE_DLOPEN
#  include <dlfcn.h>
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_WASMTIME
#  include <wasm.h>
#  include <wasi.h>
#  include <wasmtime.h>
#అయితే_ముగింపు

#ఒకవేళ HAVE_DLOPEN && HAVE_WASMTIME
స్థిర పూర్ణాంకం
libwasmtime_exec (శూన్య *cookie, libcrun_container_t *container arg_వాడని,
                  స్థిరరాశి అక్షరం *pathname, అక్షరం *స్థిరరాశి argv[])
{
  size_t వాదనలు_కొలత = 0;
  అక్షరం *స్థిరరాశి *arg;
  wasm_byte_vec_t error_సందేశం;
  wasm_byte_vec_t wasm_బైట్లు;
  wasm_engine_t *(*wasm_engine_new) ();
  wasmtime_error_t *(*wasmtime_wat2wasm) (స్థిరరాశి అక్షరం *wat, size_t wat_len, wasm_byte_vec_t *బహిర్గతం);
  శూన్య (*wasm_engine_delete) (wasm_engine_t *);
  శూన్య (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  శూన్య (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  wasi_config_t *(*wasi_config_new) (స్థిరరాశి అక్షరం *);
  wasmtime_store_t *(*wasmtime_store_new) (wasm_engine_t *engine, శూన్య *దత్తాంశం, శూన్య (*finalizer) (శూన్య *));
  wasmtime_context_t *(*wasmtime_store_context) (wasmtime_store_t *store);
  wasmtime_linker_t *(*wasmtime_linker_new) (wasm_engine_t *engine);
  wasmtime_error_t *(*wasmtime_linker_define_wasi) (wasmtime_linker_t *linker);
  wasmtime_error_t *(*wasmtime_module_new) (
      wasm_engine_t *engine,
      స్థిరరాశి uint8_t *wasm,
      size_t wasm_len,
      wasmtime_module_t **ret);
  శూన్య (*wasi_config_inherit_argv) (wasi_config_t *config);
  శూన్య (*wasi_config_inherit_env) (wasi_config_t *config);
  శూన్య (*wasi_config_సమితి_argv) (wasi_config_t *config, పూర్ణాంకం argc, స్థిరరాశి అక్షరం *argv[]);
  శూన్య (*wasi_config_inherit_stdin) (wasi_config_t *config);
  శూన్య (*wasi_config_inherit_stdout) (wasi_config_t *config);
  శూన్య (*wasi_config_inherit_stderr) (wasi_config_t *config);
  wasmtime_error_t *(*wasmtime_context_సమితి_wasi) (wasmtime_context_t *context, wasi_config_t *wasi);
  wasmtime_error_t *(*wasmtime_linker_module) (
      wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      స్థిరరాశి అక్షరం *name,
      size_t name_len,
      స్థిరరాశి wasmtime_module_t *module);
  wasmtime_error_t *(*wasmtime_linker_get_default) (
      స్థిరరాశి wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      స్థిరరాశి అక్షరం *name,
      size_t name_len,
      wasmtime_func_t *func);
  wasmtime_error_t *(*wasmtime_func_call) (
      wasmtime_context_t *store,
      స్థిరరాశి wasmtime_func_t *func,
      స్థిరరాశి wasmtime_val_t *వాదనలు,
      size_t nargs,
      wasmtime_val_t *results,
      size_t nresults,
      wasm_trap_t **trap);
  శూన్య (*wasmtime_module_delete) (wasmtime_module_t *m);
  శూన్య (*wasmtime_store_delete) (wasmtime_store_t *store);
  శూన్య (*wasmtime_error_సందేశం) (స్థిరరాశి wasmtime_error_t *error, wasm_name_t *సందేశం_2);
  శూన్య (*wasmtime_error_delete) (wasmtime_error_t *error);
  bool (*wasi_config_preopen_dir) (wasi_config_t *config, స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *guest_మార్గం);

  wasmtime_wat2wasm = dlsym (cookie, "wasmtime_wat2wasm");
  wasm_engine_new = dlsym (cookie, "wasm_engine_new");
  wasm_engine_delete = dlsym (cookie, "wasm_engine_delete");
  wasm_byte_vec_delete = dlsym (cookie, "wasm_byte_vec_delete");
  wasm_byte_vec_new_uninitialized = dlsym (cookie, "wasm_byte_vec_new_uninitialized");
  wasi_config_new = dlsym (cookie, "wasi_config_new");
  wasi_config_సమితి_argv = dlsym (cookie, "wasi_config_set_argv");
  wasmtime_store_new = dlsym (cookie, "wasmtime_store_new");
  wasmtime_store_context = dlsym (cookie, "wasmtime_store_context");
  wasmtime_linker_new = dlsym (cookie, "wasmtime_linker_new");
  wasmtime_linker_define_wasi = dlsym (cookie, "wasmtime_linker_define_wasi");
  wasmtime_module_new = dlsym (cookie, "wasmtime_module_new");
  wasi_config_inherit_argv = dlsym (cookie, "wasi_config_inherit_argv");
  wasi_config_inherit_stdout = dlsym (cookie, "wasi_config_inherit_stdout");
  wasi_config_inherit_stdin = dlsym (cookie, "wasi_config_inherit_stdin");
  wasi_config_inherit_stderr = dlsym (cookie, "wasi_config_inherit_stderr");
  wasi_config_inherit_env = dlsym (cookie, "wasi_config_inherit_env");
  wasmtime_context_సమితి_wasi = dlsym (cookie, "wasmtime_context_set_wasi");
  wasmtime_linker_module = dlsym (cookie, "wasmtime_linker_module");
  wasmtime_linker_get_default = dlsym (cookie, "wasmtime_linker_get_default");
  wasmtime_func_call = dlsym (cookie, "wasmtime_func_call");
  wasmtime_module_delete = dlsym (cookie, "wasmtime_module_delete");
  wasmtime_store_delete = dlsym (cookie, "wasmtime_store_delete");
  wasmtime_error_delete = dlsym (cookie, "wasmtime_error_delete");
  wasmtime_error_సందేశం = dlsym (cookie, "wasmtime_error_message");
  wasi_config_preopen_dir = dlsym (cookie, "wasi_config_preopen_dir");

  ఒకవేళ (wasm_engine_new == ఖాళీ || wasm_engine_delete == ఖాళీ || wasm_byte_vec_delete == ఖాళీ
      || wasm_byte_vec_new_uninitialized == ఖాళీ || wasi_config_new == ఖాళీ || wasmtime_store_new == ఖాళీ
      || wasmtime_store_context == ఖాళీ || wasmtime_linker_new == ఖాళీ || wasmtime_linker_define_wasi == ఖాళీ
      || wasmtime_module_new == ఖాళీ || wasi_config_inherit_argv == ఖాళీ || wasi_config_inherit_stdout == ఖాళీ
      || wasi_config_inherit_stdin == ఖాళీ || wasi_config_inherit_stderr == ఖాళీ
      || wasi_config_inherit_env == ఖాళీ || wasmtime_context_సమితి_wasi == ఖాళీ
      || wasmtime_linker_module == ఖాళీ || wasmtime_linker_get_default == ఖాళీ || wasmtime_func_call == ఖాళీ
      || wasmtime_module_delete == ఖాళీ || wasmtime_store_delete == ఖాళీ || wasi_config_సమితి_argv == ఖాళీ
      || wasmtime_error_delete == ఖాళీ || wasmtime_error_సందేశం == ఖాళీ || wasi_config_preopen_dir == ఖాళీ
      || wasmtime_wat2wasm == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmtime.so`");

  // Set up wasmtime context
  wasm_engine_t *engine = wasm_engine_new ();
  ప్రతిజ్ఞా (engine != ఖాళీ);
  wasmtime_store_t *store = wasmtime_store_new (engine, ఖాళీ, ఖాళీ);
  ప్రతిజ్ఞా (store != ఖాళీ);
  wasmtime_context_t *context = wasmtime_store_context (store);

  // Link with wasi functions defined
  wasmtime_linker_t *linker = wasmtime_linker_new (engine);
  wasmtime_error_t *err = wasmtime_linker_define_wasi (linker);
  ఒకవేళ (err != ఖాళీ)
    {
      wasmtime_error_సందేశం (err, &error_సందేశం);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to link wasi: %.*s", (పూర్ణాంకం) error_సందేశం.కొలత, error_సందేశం.దత్తాంశం);
    }

  wasm_byte_vec_t wasm;
  // Load and parse container entrypoint
  FILE *file = fopen (pathname, "rbe");
  ఒకవేళ (! file)
    error (EXIT_FAILURE, 0, "error loading entrypoint");
  fseek (file, 0L, SEEK_END);
  size_t file_కొలత = ftell (file);
  wasm_byte_vec_new_uninitialized (&wasm, file_కొలత);
  fseek (file, 0L, SEEK_SET);
  ఒకవేళ (fread (wasm.దత్తాంశం, file_కొలత, 1, file) != 1)
    error (EXIT_FAILURE, 0, "error load");
  fclose (file);

  // If entrypoint contains a webassembly text format
  // compile it on the fly and convert to equivalent
  // binary format.
  ఒకవేళ (has_suffix (pathname, "wat") > 0)
    {
      wasmtime_error_t *err = wasmtime_wat2wasm ((అక్షరం *) &wasm_బైట్లు, file_కొలత, &wasm);
      ఒకవేళ (err != ఖాళీ)
        {
          wasmtime_error_సందేశం (err, &error_సందేశం);
          wasmtime_error_delete (err);
          error (EXIT_FAILURE, 0, "failed while compiling wat to wasm binary : %.*s", (పూర్ణాంకం) error_సందేశం.కొలత, error_సందేశం.దత్తాంశం);
        }
      wasm = wasm_బైట్లు;
    }

  // Compile wasm modules
  wasmtime_module_t *module = ఖాళీ;
  err = wasmtime_module_new (engine, (uint8_t *) wasm.దత్తాంశం, wasm.కొలత, &module);
  ఒకవేళ (! module)
    {
      wasmtime_error_సందేశం (err, &error_సందేశం);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to compile module: %.*s", (పూర్ణాంకం) error_సందేశం.కొలత, error_సందేశం.దత్తాంశం);
    }
  wasm_byte_vec_delete (&wasm);

  // Init WASI program
  wasi_config_t *wasi_config = wasi_config_new ("crun_wasi_program");
  ప్రతిజ్ఞా (wasi_config);

  // Calculate argc for `wasi_config_set_argv`
  కొరకు (arg = argv; *arg != ఖాళీ; ++arg)
    వాదనలు_కొలత++;

  wasi_config_సమితి_argv (wasi_config, వాదనలు_కొలత, (స్థిరరాశి అక్షరం **) argv);
  wasi_config_inherit_env (wasi_config);
  wasi_config_inherit_stdin (wasi_config);
  wasi_config_inherit_stdout (wasi_config);
  wasi_config_inherit_stderr (wasi_config);
  wasi_config_preopen_dir (wasi_config, ".", ".");
  wasm_trap_t *trap = ఖాళీ;
  err = wasmtime_context_సమితి_wasi (context, wasi_config);
  ఒకవేళ (err != ఖాళీ)
    {
      wasmtime_error_సందేశం (err, &error_సందేశం);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate WASI: %.*s", (పూర్ణాంకం) error_సందేశం.కొలత, error_సందేశం.దత్తాంశం);
    }

  // Init module
  err = wasmtime_linker_module (linker, context, "", 0, module);
  ఒకవేళ (err != ఖాళీ)
    {
      wasmtime_error_సందేశం (err, &error_సందేశం);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate module: %.*s", (పూర్ణాంకం) error_సందేశం.కొలత, error_సందేశం.దత్తాంశం);
    }

  // Actually run our .wasm
  wasmtime_func_t func;
  err = wasmtime_linker_get_default (linker, context, "", 0, &func);
  ఒకవేళ (err != ఖాళీ)
    {
      wasmtime_error_సందేశం (err, &error_సందేశం);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to locate default export for module %.*s", (పూర్ణాంకం) error_సందేశం.కొలత, error_సందేశం.దత్తాంశం);
    }

  err = wasmtime_func_call (context, &func, ఖాళీ, 0, ఖాళీ, 0, &trap);
  ఒకవేళ (err != ఖాళీ || trap != ఖాళీ)
    {
      wasmtime_error_సందేశం (err, &error_సందేశం);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "error calling default export: %.*s", (పూర్ణాంకం) error_సందేశం.కొలత, error_సందేశం.దత్తాంశం);
    }

  // Clean everything
  wasmtime_module_delete (module);
  wasmtime_store_delete (store);
  wasm_engine_delete (engine);

  నిష్క్రమించు (EXIT_SUCCESS);
}

స్థిర పూర్ణాంకం
libwasmtime_ఎక్కించు (శూన్య **cookie, libcrun_error_t *err)
{
  శూన్య *handle;

  handle = dlopen ("libwasmtime.so", RTLD_NOW);
  ఒకవేళ (handle == ఖాళీ)
    తిరిగి crun_make_error (err, 0, "could not load `libwasmtime.so`: %s", dlerror ());
  *cookie = handle;

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
libwasmtime_unload (శూన్య *cookie, libcrun_error_t *err)
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
libwasmtime_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  తిరిగి wasm_can_handle_container (container, err);
}

నిర్మాణం custom_handler_s handler_wasmtime = {
  .name = "wasmtime",
  .alias = "wasm",
  .feature_string = "WASM:wasmtime",
  .supports_open_వృక్షం_namespace = false,
  .ఎక్కించు = libwasmtime_ఎక్కించు,
  .unload = libwasmtime_unload,
  .నిరంతరం_func = libwasmtime_exec,
  .can_handle_container = libwasmtime_can_handle_container,
};

#అయితే_ముగింపు
