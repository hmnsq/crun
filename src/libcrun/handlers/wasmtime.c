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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "../custom-handler.h"
#அந்தர்பாவ "../container.h"
#அந்தர்பாவ "../utils.h"
#அந்தர்பாவ "../linux.h"
#அந்தர்பாவ "handler-utils.h"
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <sched.h>
#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMTIME
#  include <wasm.h>
#  include <wasi.h>
#  include <wasmtime.h>
#endif

#எனில் HAVE_DLOPEN && HAVE_WASMTIME
நிலையான முழு
libwasmtime_exec (வெறுமை *cookie, libcrun_container_t *container arg_unused,
                  மாறிலி எழுத்து *pathname, எழுத்து *மாறிலி argv[])
{
  size_t args_size = 0;
  எழுத்து *மாறிலி *arg;
  wasm_byte_vec_t error_message;
  wasm_byte_vec_t wasm_bytes;
  wasm_engine_t *(*wasm_engine_new) ();
  wasmtime_error_t *(*wasmtime_wat2wasm) (மாறிலி எழுத்து *wat, size_t wat_len, wasm_byte_vec_t *out);
  வெறுமை (*wasm_engine_delete) (wasm_engine_t *);
  வெறுமை (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  வெறுமை (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  wasi_config_t *(*wasi_config_new) (மாறிலி எழுத்து *);
  wasmtime_store_t *(*wasmtime_store_new) (wasm_engine_t *engine, வெறுமை *data, வெறுமை (*finalizer) (வெறுமை *));
  wasmtime_context_t *(*wasmtime_store_context) (wasmtime_store_t *store);
  wasmtime_linker_t *(*wasmtime_linker_new) (wasm_engine_t *engine);
  wasmtime_error_t *(*wasmtime_linker_define_wasi) (wasmtime_linker_t *linker);
  wasmtime_error_t *(*wasmtime_module_new) (
      wasm_engine_t *engine,
      மாறிலி uint8_t *wasm,
      size_t wasm_len,
      wasmtime_module_t **ret);
  வெறுமை (*wasi_config_inherit_argv) (wasi_config_t *config);
  வெறுமை (*wasi_config_inherit_env) (wasi_config_t *config);
  வெறுமை (*wasi_config_set_argv) (wasi_config_t *config, முழு argc, மாறிலி எழுத்து *argv[]);
  வெறுமை (*wasi_config_inherit_stdin) (wasi_config_t *config);
  வெறுமை (*wasi_config_inherit_stdout) (wasi_config_t *config);
  வெறுமை (*wasi_config_inherit_stderr) (wasi_config_t *config);
  wasmtime_error_t *(*wasmtime_context_set_wasi) (wasmtime_context_t *context, wasi_config_t *wasi);
  wasmtime_error_t *(*wasmtime_linker_module) (
      wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      மாறிலி எழுத்து *name,
      size_t name_len,
      மாறிலி wasmtime_module_t *module);
  wasmtime_error_t *(*wasmtime_linker_get_default) (
      மாறிலி wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      மாறிலி எழுத்து *name,
      size_t name_len,
      wasmtime_func_t *func);
  wasmtime_error_t *(*wasmtime_func_call) (
      wasmtime_context_t *store,
      மாறிலி wasmtime_func_t *func,
      மாறிலி wasmtime_val_t *args,
      size_t nargs,
      wasmtime_val_t *results,
      size_t nresults,
      wasm_trap_t **trap);
  வெறுமை (*wasmtime_module_delete) (wasmtime_module_t *m);
  வெறுமை (*wasmtime_store_delete) (wasmtime_store_t *store);
  வெறுமை (*wasmtime_error_message) (மாறிலி wasmtime_error_t *error, wasm_name_t *message);
  வெறுமை (*wasmtime_error_delete) (wasmtime_error_t *error);
  bool (*wasi_config_preopen_dir) (wasi_config_t *config, மாறிலி எழுத்து *path, மாறிலி எழுத்து *guest_path);

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

  எனில் (wasm_engine_new == வெற்று || wasm_engine_delete == வெற்று || wasm_byte_vec_delete == வெற்று
      || wasm_byte_vec_new_uninitialized == வெற்று || wasi_config_new == வெற்று || wasmtime_store_new == வெற்று
      || wasmtime_store_context == வெற்று || wasmtime_linker_new == வெற்று || wasmtime_linker_define_wasi == வெற்று
      || wasmtime_module_new == வெற்று || wasi_config_inherit_argv == வெற்று || wasi_config_inherit_stdout == வெற்று
      || wasi_config_inherit_stdin == வெற்று || wasi_config_inherit_stderr == வெற்று
      || wasi_config_inherit_env == வெற்று || wasmtime_context_set_wasi == வெற்று
      || wasmtime_linker_module == வெற்று || wasmtime_linker_get_default == வெற்று || wasmtime_func_call == வெற்று
      || wasmtime_module_delete == வெற்று || wasmtime_store_delete == வெற்று || wasi_config_set_argv == வெற்று
      || wasmtime_error_delete == வெற்று || wasmtime_error_message == வெற்று || wasi_config_preopen_dir == வெற்று
      || wasmtime_wat2wasm == வெற்று)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmtime.so`");

  // Set up wasmtime context
  wasm_engine_t *engine = wasm_engine_new ();
  ப்ரதிஜ்ஞா (engine != வெற்று);
  wasmtime_store_t *store = wasmtime_store_new (engine, வெற்று, வெற்று);
  ப்ரதிஜ்ஞா (store != வெற்று);
  wasmtime_context_t *context = wasmtime_store_context (store);

  // Link with wasi functions defined
  wasmtime_linker_t *linker = wasmtime_linker_new (engine);
  wasmtime_error_t *err = wasmtime_linker_define_wasi (linker);
  எனில் (err != வெற்று)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to link wasi: %.*s", (முழு) error_message.size, error_message.data);
    }

  wasm_byte_vec_t wasm;
  // Load and parse container entrypoint
  FILE *file = fopen (pathname, "rbe");
  எனில் (! file)
    error (EXIT_FAILURE, 0, "error loading entrypoint");
  fseek (file, 0L, SEEK_END);
  size_t file_size = ftell (file);
  wasm_byte_vec_new_uninitialized (&wasm, file_size);
  fseek (file, 0L, SEEK_SET);
  எனில் (fread (wasm.data, file_size, 1, file) != 1)
    error (EXIT_FAILURE, 0, "error load");
  fclose (file);

  // If entrypoint contains a webassembly text format
  // compile it on the fly and convert to equivalent
  // binary format.
  எனில் (has_suffix (pathname, "wat") > 0)
    {
      wasmtime_error_t *err = wasmtime_wat2wasm ((எழுத்து *) &wasm_bytes, file_size, &wasm);
      எனில் (err != வெற்று)
        {
          wasmtime_error_message (err, &error_message);
          wasmtime_error_delete (err);
          error (EXIT_FAILURE, 0, "failed while compiling wat to wasm binary : %.*s", (முழு) error_message.size, error_message.data);
        }
      wasm = wasm_bytes;
    }

  // Compile wasm modules
  wasmtime_module_t *module = வெற்று;
  err = wasmtime_module_new (engine, (uint8_t *) wasm.data, wasm.size, &module);
  எனில் (! module)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to compile module: %.*s", (முழு) error_message.size, error_message.data);
    }
  wasm_byte_vec_delete (&wasm);

  // Init WASI program
  wasi_config_t *wasi_config = wasi_config_new ("crun_wasi_program");
  ப்ரதிஜ்ஞா (wasi_config);

  // Calculate argc for `wasi_config_set_argv`
  க்கு (arg = argv; *arg != வெற்று; ++arg)
    args_size++;

  wasi_config_set_argv (wasi_config, args_size, (மாறிலி எழுத்து **) argv);
  wasi_config_inherit_env (wasi_config);
  wasi_config_inherit_stdin (wasi_config);
  wasi_config_inherit_stdout (wasi_config);
  wasi_config_inherit_stderr (wasi_config);
  wasi_config_preopen_dir (wasi_config, ".", ".");
  wasm_trap_t *trap = வெற்று;
  err = wasmtime_context_set_wasi (context, wasi_config);
  எனில் (err != வெற்று)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate WASI: %.*s", (முழு) error_message.size, error_message.data);
    }

  // Init module
  err = wasmtime_linker_module (linker, context, "", 0, module);
  எனில் (err != வெற்று)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate module: %.*s", (முழு) error_message.size, error_message.data);
    }

  // Actually run our .wasm
  wasmtime_func_t func;
  err = wasmtime_linker_get_default (linker, context, "", 0, &func);
  எனில் (err != வெற்று)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to locate default export for module %.*s", (முழு) error_message.size, error_message.data);
    }

  err = wasmtime_func_call (context, &func, வெற்று, 0, வெற்று, 0, &trap);
  எனில் (err != வெற்று || trap != வெற்று)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "error calling default export: %.*s", (முழு) error_message.size, error_message.data);
    }

  // Clean everything
  wasmtime_module_delete (module);
  wasmtime_store_delete (store);
  wasm_engine_delete (engine);

  exit (EXIT_SUCCESS);
}

நிலையான முழு
libwasmtime_load (வெறுமை **cookie, libcrun_error_t *err)
{
  வெறுமை *handle;

  handle = dlopen ("libwasmtime.so", RTLD_NOW);
  எனில் (handle == வெற்று)
    திரும்பு crun_make_error (err, 0, "could not load `libwasmtime.so`: %s", dlerror ());
  *cookie = handle;

  திரும்பு 0;
}

நிலையான முழு
libwasmtime_unload (வெறுமை *cookie, libcrun_error_t *err)
{
  முழு r;

  எனில் (cookie)
    {
      r = dlclose (cookie);
      எனில் (UNLIKELY (r < 0))
        திரும்பு crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  திரும்பு 0;
}

நிலையான முழு
libwasmtime_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  திரும்பு wasm_can_handle_container (container, err);
}

கட்டமைப்பு custom_handler_s handler_wasmtime = {
  .name = "wasmtime",
  .alias = "wasm",
  .feature_string = "WASM:wasmtime",
  .supports_open_மரம்_namespace = false,
  .load = libwasmtime_load,
  .unload = libwasmtime_unload,
  .run_func = libwasmtime_exec,
  .can_handle_container = libwasmtime_can_handle_container,
};

#endif
