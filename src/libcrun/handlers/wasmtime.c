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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ "../custom-handler.h"
#অন্তর্ভাৱ "../container.h"
#অন্তর্ভাৱ "../utils.h"
#অন্তর্ভাৱ "../linux.h"
#অন্তর্ভাৱ "handler-utils.h"
#অন্তর্ভাৱ <unistd.h>
#অন্তর্ভাৱ <sys/stat.h>
#অন্তর্ভাৱ <errno.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <sched.h>
#অন্তর্ভাৱ <assert.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <stdlib.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMTIME
#  include <wasm.h>
#  include <wasi.h>
#  include <wasmtime.h>
#endif

#যদি HAVE_DLOPEN && HAVE_WASMTIME
স্থির পূর্ণাঙ্ক
libwasmtime_exec (শূন্য *cookie, libcrun_container_t *container arg_unused,
                  ধ্রুৱ ৱর্ণ *pathname, ৱর্ণ *ধ্রুৱ argv[])
{
  size_t args_size = 0;
  ৱর্ণ *ধ্রুৱ *arg;
  wasm_byte_vec_t error_message;
  wasm_byte_vec_t wasm_bytes;
  wasm_engine_t *(*wasm_engine_new) ();
  wasmtime_error_t *(*wasmtime_wat2wasm) (ধ্রুৱ ৱর্ণ *wat, size_t wat_len, wasm_byte_vec_t *out);
  শূন্য (*wasm_engine_delete) (wasm_engine_t *);
  শূন্য (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  শূন্য (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  wasi_config_t *(*wasi_config_new) (ধ্রুৱ ৱর্ণ *);
  wasmtime_store_t *(*wasmtime_store_new) (wasm_engine_t *engine, শূন্য *data, শূন্য (*finalizer) (শূন্য *));
  wasmtime_context_t *(*wasmtime_store_context) (wasmtime_store_t *store);
  wasmtime_linker_t *(*wasmtime_linker_new) (wasm_engine_t *engine);
  wasmtime_error_t *(*wasmtime_linker_define_wasi) (wasmtime_linker_t *linker);
  wasmtime_error_t *(*wasmtime_module_new) (
      wasm_engine_t *engine,
      ধ্রুৱ uint8_t *wasm,
      size_t wasm_len,
      wasmtime_module_t **ret);
  শূন্য (*wasi_config_inherit_argv) (wasi_config_t *config);
  শূন্য (*wasi_config_inherit_env) (wasi_config_t *config);
  শূন্য (*wasi_config_set_argv) (wasi_config_t *config, পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ *argv[]);
  শূন্য (*wasi_config_inherit_stdin) (wasi_config_t *config);
  শূন্য (*wasi_config_inherit_stdout) (wasi_config_t *config);
  শূন্য (*wasi_config_inherit_stderr) (wasi_config_t *config);
  wasmtime_error_t *(*wasmtime_context_set_wasi) (wasmtime_context_t *context, wasi_config_t *wasi);
  wasmtime_error_t *(*wasmtime_linker_module) (
      wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      ধ্রুৱ ৱর্ণ *name,
      size_t name_len,
      ধ্রুৱ wasmtime_module_t *module);
  wasmtime_error_t *(*wasmtime_linker_get_default) (
      ধ্রুৱ wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      ধ্রুৱ ৱর্ণ *name,
      size_t name_len,
      wasmtime_func_t *func);
  wasmtime_error_t *(*wasmtime_func_call) (
      wasmtime_context_t *store,
      ধ্রুৱ wasmtime_func_t *func,
      ধ্রুৱ wasmtime_val_t *args,
      size_t nargs,
      wasmtime_val_t *results,
      size_t nresults,
      wasm_trap_t **trap);
  শূন্য (*wasmtime_module_delete) (wasmtime_module_t *m);
  শূন্য (*wasmtime_store_delete) (wasmtime_store_t *store);
  শূন্য (*wasmtime_error_message) (ধ্রুৱ wasmtime_error_t *error, wasm_name_t *message);
  শূন্য (*wasmtime_error_delete) (wasmtime_error_t *error);
  bool (*wasi_config_preopen_dir) (wasi_config_t *config, ধ্রুৱ ৱর্ণ *path, ধ্রুৱ ৱর্ণ *guest_path);

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

  যদি (wasm_engine_new == রিক্ত || wasm_engine_delete == রিক্ত || wasm_byte_vec_delete == রিক্ত
      || wasm_byte_vec_new_uninitialized == রিক্ত || wasi_config_new == রিক্ত || wasmtime_store_new == রিক্ত
      || wasmtime_store_context == রিক্ত || wasmtime_linker_new == রিক্ত || wasmtime_linker_define_wasi == রিক্ত
      || wasmtime_module_new == রিক্ত || wasi_config_inherit_argv == রিক্ত || wasi_config_inherit_stdout == রিক্ত
      || wasi_config_inherit_stdin == রিক্ত || wasi_config_inherit_stderr == রিক্ত
      || wasi_config_inherit_env == রিক্ত || wasmtime_context_set_wasi == রিক্ত
      || wasmtime_linker_module == রিক্ত || wasmtime_linker_get_default == রিক্ত || wasmtime_func_call == রিক্ত
      || wasmtime_module_delete == রিক্ত || wasmtime_store_delete == রিক্ত || wasi_config_set_argv == রিক্ত
      || wasmtime_error_delete == রিক্ত || wasmtime_error_message == রিক্ত || wasi_config_preopen_dir == রিক্ত
      || wasmtime_wat2wasm == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmtime.so`");

  // Set up wasmtime context
  wasm_engine_t *engine = wasm_engine_new ();
  প্রতিজ্ঞা (engine != রিক্ত);
  wasmtime_store_t *store = wasmtime_store_new (engine, রিক্ত, রিক্ত);
  প্রতিজ্ঞা (store != রিক্ত);
  wasmtime_context_t *context = wasmtime_store_context (store);

  // Link with wasi functions defined
  wasmtime_linker_t *linker = wasmtime_linker_new (engine);
  wasmtime_error_t *err = wasmtime_linker_define_wasi (linker);
  যদি (err != রিক্ত)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to link wasi: %.*s", (পূর্ণাঙ্ক) error_message.size, error_message.data);
    }

  wasm_byte_vec_t wasm;
  // Load and parse container entrypoint
  FILE *file = fopen (pathname, "rbe");
  যদি (! file)
    error (EXIT_FAILURE, 0, "error loading entrypoint");
  fseek (file, 0L, SEEK_END);
  size_t file_size = ftell (file);
  wasm_byte_vec_new_uninitialized (&wasm, file_size);
  fseek (file, 0L, SEEK_SET);
  যদি (fread (wasm.data, file_size, 1, file) != 1)
    error (EXIT_FAILURE, 0, "error load");
  fclose (file);

  // If entrypoint contains a webassembly text format
  // compile it on the fly and convert to equivalent
  // binary format.
  যদি (has_suffix (pathname, "wat") > 0)
    {
      wasmtime_error_t *err = wasmtime_wat2wasm ((ৱর্ণ *) &wasm_bytes, file_size, &wasm);
      যদি (err != রিক্ত)
        {
          wasmtime_error_message (err, &error_message);
          wasmtime_error_delete (err);
          error (EXIT_FAILURE, 0, "failed while compiling wat to wasm binary : %.*s", (পূর্ণাঙ্ক) error_message.size, error_message.data);
        }
      wasm = wasm_bytes;
    }

  // Compile wasm modules
  wasmtime_module_t *module = রিক্ত;
  err = wasmtime_module_new (engine, (uint8_t *) wasm.data, wasm.size, &module);
  যদি (! module)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to compile module: %.*s", (পূর্ণাঙ্ক) error_message.size, error_message.data);
    }
  wasm_byte_vec_delete (&wasm);

  // Init WASI program
  wasi_config_t *wasi_config = wasi_config_new ("crun_wasi_program");
  প্রতিজ্ঞা (wasi_config);

  // Calculate argc for `wasi_config_set_argv`
  বাবে (arg = argv; *arg != রিক্ত; ++arg)
    args_size++;

  wasi_config_set_argv (wasi_config, args_size, (ধ্রুৱ ৱর্ণ **) argv);
  wasi_config_inherit_env (wasi_config);
  wasi_config_inherit_stdin (wasi_config);
  wasi_config_inherit_stdout (wasi_config);
  wasi_config_inherit_stderr (wasi_config);
  wasi_config_preopen_dir (wasi_config, ".", ".");
  wasm_trap_t *trap = রিক্ত;
  err = wasmtime_context_set_wasi (context, wasi_config);
  যদি (err != রিক্ত)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate WASI: %.*s", (পূর্ণাঙ্ক) error_message.size, error_message.data);
    }

  // Init module
  err = wasmtime_linker_module (linker, context, "", 0, module);
  যদি (err != রিক্ত)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate module: %.*s", (পূর্ণাঙ্ক) error_message.size, error_message.data);
    }

  // Actually run our .wasm
  wasmtime_func_t func;
  err = wasmtime_linker_get_default (linker, context, "", 0, &func);
  যদি (err != রিক্ত)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to locate default export for module %.*s", (পূর্ণাঙ্ক) error_message.size, error_message.data);
    }

  err = wasmtime_func_call (context, &func, রিক্ত, 0, রিক্ত, 0, &trap);
  যদি (err != রিক্ত || trap != রিক্ত)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "error calling default export: %.*s", (পূর্ণাঙ্ক) error_message.size, error_message.data);
    }

  // Clean everything
  wasmtime_module_delete (module);
  wasmtime_store_delete (store);
  wasm_engine_delete (engine);

  exit (EXIT_SUCCESS);
}

স্থির পূর্ণাঙ্ক
libwasmtime_load (শূন্য **cookie, libcrun_error_t *err)
{
  শূন্য *handle;

  handle = dlopen ("libwasmtime.so", RTLD_NOW);
  যদি (handle == রিক্ত)
    ঘূৰাই crun_make_error (err, 0, "could not load `libwasmtime.so`: %s", dlerror ());
  *cookie = handle;

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
libwasmtime_unload (শূন্য *cookie, libcrun_error_t *err)
{
  পূর্ণাঙ্ক r;

  যদি (cookie)
    {
      r = dlclose (cookie);
      যদি (UNLIKELY (r < 0))
        ঘূৰাই crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
libwasmtime_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  ঘূৰাই wasm_can_handle_container (container, err);
}

সংরচনা custom_handler_s handler_wasmtime = {
  .name = "wasmtime",
  .alias = "wasm",
  .feature_string = "WASM:wasmtime",
  .supports_open_বৃক্ষ_namespace = false,
  .load = libwasmtime_load,
  .unload = libwasmtime_unload,
  .run_func = libwasmtime_exec,
  .can_handle_container = libwasmtime_can_handle_container,
};

#endif
