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
#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMTIME
#  include <wasm.h>
#  include <wasi.h>
#  include <wasmtime.h>
#endif

#ಯದಿ HAVE_DLOPEN && HAVE_WASMTIME
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libwasmtime_exec (ಶೂನ್ಯ *cookie, libcrun_container_t *container arg_unused,
                  ಧ್ರುವ ವರ್ಣ *pathname, ವರ್ಣ *ಧ್ರುವ argv[])
{
  size_t args_size = 0;
  ವರ್ಣ *ಧ್ರುವ *arg;
  wasm_byte_vec_t error_message;
  wasm_byte_vec_t wasm_bytes;
  wasm_engine_t *(*wasm_engine_new) ();
  wasmtime_error_t *(*wasmtime_wat2wasm) (ಧ್ರುವ ವರ್ಣ *wat, size_t wat_len, wasm_byte_vec_t *out);
  ಶೂನ್ಯ (*wasm_engine_delete) (wasm_engine_t *);
  ಶೂನ್ಯ (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  ಶೂನ್ಯ (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  wasi_config_t *(*wasi_config_new) (ಧ್ರುವ ವರ್ಣ *);
  wasmtime_store_t *(*wasmtime_store_new) (wasm_engine_t *engine, ಶೂನ್ಯ *data, ಶೂನ್ಯ (*finalizer) (ಶೂನ್ಯ *));
  wasmtime_context_t *(*wasmtime_store_context) (wasmtime_store_t *store);
  wasmtime_linker_t *(*wasmtime_linker_new) (wasm_engine_t *engine);
  wasmtime_error_t *(*wasmtime_linker_define_wasi) (wasmtime_linker_t *linker);
  wasmtime_error_t *(*wasmtime_module_new) (
      wasm_engine_t *engine,
      ಧ್ರುವ uint8_t *wasm,
      size_t wasm_len,
      wasmtime_module_t **ret);
  ಶೂನ್ಯ (*wasi_config_inherit_argv) (wasi_config_t *config);
  ಶೂನ್ಯ (*wasi_config_inherit_env) (wasi_config_t *config);
  ಶೂನ್ಯ (*wasi_config_set_argv) (wasi_config_t *config, ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ *argv[]);
  ಶೂನ್ಯ (*wasi_config_inherit_stdin) (wasi_config_t *config);
  ಶೂನ್ಯ (*wasi_config_inherit_stdout) (wasi_config_t *config);
  ಶೂನ್ಯ (*wasi_config_inherit_stderr) (wasi_config_t *config);
  wasmtime_error_t *(*wasmtime_context_set_wasi) (wasmtime_context_t *context, wasi_config_t *wasi);
  wasmtime_error_t *(*wasmtime_linker_module) (
      wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      ಧ್ರುವ ವರ್ಣ *name,
      size_t name_len,
      ಧ್ರುವ wasmtime_module_t *module);
  wasmtime_error_t *(*wasmtime_linker_get_default) (
      ಧ್ರುವ wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      ಧ್ರುವ ವರ್ಣ *name,
      size_t name_len,
      wasmtime_func_t *func);
  wasmtime_error_t *(*wasmtime_func_call) (
      wasmtime_context_t *store,
      ಧ್ರುವ wasmtime_func_t *func,
      ಧ್ರುವ wasmtime_val_t *args,
      size_t nargs,
      wasmtime_val_t *results,
      size_t nresults,
      wasm_trap_t **trap);
  ಶೂನ್ಯ (*wasmtime_module_delete) (wasmtime_module_t *m);
  ಶೂನ್ಯ (*wasmtime_store_delete) (wasmtime_store_t *store);
  ಶೂನ್ಯ (*wasmtime_error_message) (ಧ್ರುವ wasmtime_error_t *error, wasm_name_t *message);
  ಶೂನ್ಯ (*wasmtime_error_delete) (wasmtime_error_t *error);
  bool (*wasi_config_preopen_dir) (wasi_config_t *config, ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *guest_path);

  wasmtime_wat2wasm = dlsym (cookie, "wasmtime_wat2wasm");
  wasm_engine_new = dlsym (cookie, "wasm_engine_new");
  wasm_engine_delete = dlsym (cookie, "wasm_engine_delete");
  wasm_byte_vec_delete = dlsym (cookie, "wasm_byte_vec_delete");
  wasm_byte_vec_new_uninitialized = dlsym (cookie, "wasm_byte_vec_new_uninitialized");
  wasi_config_new = dlsym (cookie, "wasi_config_new");
  wasi_config_set_argv = dlsym (cookie, "wasi_config_set_argv");
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
  wasmtime_context_set_wasi = dlsym (cookie, "wasmtime_context_set_wasi");
  wasmtime_linker_module = dlsym (cookie, "wasmtime_linker_module");
  wasmtime_linker_get_default = dlsym (cookie, "wasmtime_linker_get_default");
  wasmtime_func_call = dlsym (cookie, "wasmtime_func_call");
  wasmtime_module_delete = dlsym (cookie, "wasmtime_module_delete");
  wasmtime_store_delete = dlsym (cookie, "wasmtime_store_delete");
  wasmtime_error_delete = dlsym (cookie, "wasmtime_error_delete");
  wasmtime_error_message = dlsym (cookie, "wasmtime_error_message");
  wasi_config_preopen_dir = dlsym (cookie, "wasi_config_preopen_dir");

  ಯದಿ (wasm_engine_new == ರಿಕ್ತ || wasm_engine_delete == ರಿಕ್ತ || wasm_byte_vec_delete == ರಿಕ್ತ
      || wasm_byte_vec_new_uninitialized == ರಿಕ್ತ || wasi_config_new == ರಿಕ್ತ || wasmtime_store_new == ರಿಕ್ತ
      || wasmtime_store_context == ರಿಕ್ತ || wasmtime_linker_new == ರಿಕ್ತ || wasmtime_linker_define_wasi == ರಿಕ್ತ
      || wasmtime_module_new == ರಿಕ್ತ || wasi_config_inherit_argv == ರಿಕ್ತ || wasi_config_inherit_stdout == ರಿಕ್ತ
      || wasi_config_inherit_stdin == ರಿಕ್ತ || wasi_config_inherit_stderr == ರಿಕ್ತ
      || wasi_config_inherit_env == ರಿಕ್ತ || wasmtime_context_set_wasi == ರಿಕ್ತ
      || wasmtime_linker_module == ರಿಕ್ತ || wasmtime_linker_get_default == ರಿಕ್ತ || wasmtime_func_call == ರಿಕ್ತ
      || wasmtime_module_delete == ರಿಕ್ತ || wasmtime_store_delete == ರಿಕ್ತ || wasi_config_set_argv == ರಿಕ್ತ
      || wasmtime_error_delete == ರಿಕ್ತ || wasmtime_error_message == ರಿಕ್ತ || wasi_config_preopen_dir == ರಿಕ್ತ
      || wasmtime_wat2wasm == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmtime.so`");

  // Set up wasmtime context
  wasm_engine_t *engine = wasm_engine_new ();
  ಪ್ರತಿಜ್ಞಾ (engine != ರಿಕ್ತ);
  wasmtime_store_t *store = wasmtime_store_new (engine, ರಿಕ್ತ, ರಿಕ್ತ);
  ಪ್ರತಿಜ್ಞಾ (store != ರಿಕ್ತ);
  wasmtime_context_t *context = wasmtime_store_context (store);

  // Link with wasi functions defined
  wasmtime_linker_t *linker = wasmtime_linker_new (engine);
  wasmtime_error_t *err = wasmtime_linker_define_wasi (linker);
  ಯದಿ (err != ರಿಕ್ತ)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to link wasi: %.*s", (ಪೂರ್ಣಾಙ್ಕ) error_message.size, error_message.data);
    }

  wasm_byte_vec_t wasm;
  // Load and parse container entrypoint
  FILE *file = fopen (pathname, "rbe");
  ಯದಿ (! file)
    error (EXIT_FAILURE, 0, "error loading entrypoint");
  fseek (file, 0L, SEEK_END);
  size_t file_size = ftell (file);
  wasm_byte_vec_new_uninitialized (&wasm, file_size);
  fseek (file, 0L, SEEK_SET);
  ಯದಿ (fread (wasm.data, file_size, 1, file) != 1)
    error (EXIT_FAILURE, 0, "error load");
  fclose (file);

  // If entrypoint contains a webassembly text format
  // compile it on the fly and convert to equivalent
  // binary format.
  ಯದಿ (has_suffix (pathname, "wat") > 0)
    {
      wasmtime_error_t *err = wasmtime_wat2wasm ((ವರ್ಣ *) &wasm_bytes, file_size, &wasm);
      ಯದಿ (err != ರಿಕ್ತ)
        {
          wasmtime_error_message (err, &error_message);
          wasmtime_error_delete (err);
          error (EXIT_FAILURE, 0, "failed while compiling wat to wasm binary : %.*s", (ಪೂರ್ಣಾಙ್ಕ) error_message.size, error_message.data);
        }
      wasm = wasm_bytes;
    }

  // Compile wasm modules
  wasmtime_module_t *module = ರಿಕ್ತ;
  err = wasmtime_module_new (engine, (uint8_t *) wasm.data, wasm.size, &module);
  ಯದಿ (! module)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to compile module: %.*s", (ಪೂರ್ಣಾಙ್ಕ) error_message.size, error_message.data);
    }
  wasm_byte_vec_delete (&wasm);

  // Init WASI program
  wasi_config_t *wasi_config = wasi_config_new ("crun_wasi_program");
  ಪ್ರತಿಜ್ಞಾ (wasi_config);

  // Calculate argc for `wasi_config_set_argv`
  ಪ್ರತಿ (arg = argv; *arg != ರಿಕ್ತ; ++arg)
    args_size++;

  wasi_config_set_argv (wasi_config, args_size, (ಧ್ರುವ ವರ್ಣ **) argv);
  wasi_config_inherit_env (wasi_config);
  wasi_config_inherit_stdin (wasi_config);
  wasi_config_inherit_stdout (wasi_config);
  wasi_config_inherit_stderr (wasi_config);
  wasi_config_preopen_dir (wasi_config, ".", ".");
  wasm_trap_t *trap = ರಿಕ್ತ;
  err = wasmtime_context_set_wasi (context, wasi_config);
  ಯದಿ (err != ರಿಕ್ತ)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate WASI: %.*s", (ಪೂರ್ಣಾಙ್ಕ) error_message.size, error_message.data);
    }

  // Init module
  err = wasmtime_linker_module (linker, context, "", 0, module);
  ಯದಿ (err != ರಿಕ್ತ)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate module: %.*s", (ಪೂರ್ಣಾಙ್ಕ) error_message.size, error_message.data);
    }

  // Actually run our .wasm
  wasmtime_func_t func;
  err = wasmtime_linker_get_default (linker, context, "", 0, &func);
  ಯದಿ (err != ರಿಕ್ತ)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to locate default export for module %.*s", (ಪೂರ್ಣಾಙ್ಕ) error_message.size, error_message.data);
    }

  err = wasmtime_func_call (context, &func, ರಿಕ್ತ, 0, ರಿಕ್ತ, 0, &trap);
  ಯದಿ (err != ರಿಕ್ತ || trap != ರಿಕ್ತ)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "error calling default export: %.*s", (ಪೂರ್ಣಾಙ್ಕ) error_message.size, error_message.data);
    }

  // Clean everything
  wasmtime_module_delete (module);
  wasmtime_store_delete (store);
  wasm_engine_delete (engine);

  exit (EXIT_SUCCESS);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libwasmtime_load (ಶೂನ್ಯ **cookie, libcrun_error_t *err)
{
  ಶೂನ್ಯ *handle;

  handle = dlopen ("libwasmtime.so", RTLD_NOW);
  ಯದಿ (handle == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not load `libwasmtime.so`: %s", dlerror ());
  *cookie = handle;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libwasmtime_unload (ಶೂನ್ಯ *cookie, libcrun_error_t *err)
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
libwasmtime_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ wasm_can_handle_container (container, err);
}

ಸಂರಚನಾ custom_handler_s handler_wasmtime = {
  .name = "wasmtime",
  .alias = "wasm",
  .feature_string = "WASM:wasmtime",
  .supports_open_ವೃಕ್ಷ_namespace = false,
  .load = libwasmtime_load,
  .unload = libwasmtime_unload,
  .run_func = libwasmtime_exec,
  .can_handle_container = libwasmtime_can_handle_container,
};

#endif
