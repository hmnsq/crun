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
#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMTIME
#  include <wasm.h>
#  include <wasi.h>
#  include <wasmtime.h>
#endif

#જો HAVE_DLOPEN && HAVE_WASMTIME
સ્થિર પૂર્ણાંક
libwasmtime_exec (શૂન્ય *cookie, libcrun_container_t *container arg_unused,
                  અચલ અક્ષર *pathname, અક્ષર *અચલ argv[])
{
  size_t args_size = 0;
  અક્ષર *અચલ *arg;
  wasm_byte_vec_t error_message;
  wasm_byte_vec_t wasm_bytes;
  wasm_engine_t *(*wasm_engine_new) ();
  wasmtime_error_t *(*wasmtime_wat2wasm) (અચલ અક્ષર *wat, size_t wat_len, wasm_byte_vec_t *out);
  શૂન્ય (*wasm_engine_delete) (wasm_engine_t *);
  શૂન્ય (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  શૂન્ય (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  wasi_config_t *(*wasi_config_new) (અચલ અક્ષર *);
  wasmtime_store_t *(*wasmtime_store_new) (wasm_engine_t *engine, શૂન્ય *data, શૂન્ય (*finalizer) (શૂન્ય *));
  wasmtime_context_t *(*wasmtime_store_context) (wasmtime_store_t *store);
  wasmtime_linker_t *(*wasmtime_linker_new) (wasm_engine_t *engine);
  wasmtime_error_t *(*wasmtime_linker_define_wasi) (wasmtime_linker_t *linker);
  wasmtime_error_t *(*wasmtime_module_new) (
      wasm_engine_t *engine,
      અચલ uint8_t *wasm,
      size_t wasm_len,
      wasmtime_module_t **ret);
  શૂન્ય (*wasi_config_inherit_argv) (wasi_config_t *config);
  શૂન્ય (*wasi_config_inherit_env) (wasi_config_t *config);
  શૂન્ય (*wasi_config_set_argv) (wasi_config_t *config, પૂર્ણાંક argc, અચલ અક્ષર *argv[]);
  શૂન્ય (*wasi_config_inherit_stdin) (wasi_config_t *config);
  શૂન્ય (*wasi_config_inherit_stdout) (wasi_config_t *config);
  શૂન્ય (*wasi_config_inherit_stderr) (wasi_config_t *config);
  wasmtime_error_t *(*wasmtime_context_set_wasi) (wasmtime_context_t *context, wasi_config_t *wasi);
  wasmtime_error_t *(*wasmtime_linker_module) (
      wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      અચલ અક્ષર *name,
      size_t name_len,
      અચલ wasmtime_module_t *module);
  wasmtime_error_t *(*wasmtime_linker_get_default) (
      અચલ wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      અચલ અક્ષર *name,
      size_t name_len,
      wasmtime_func_t *func);
  wasmtime_error_t *(*wasmtime_func_call) (
      wasmtime_context_t *store,
      અચલ wasmtime_func_t *func,
      અચલ wasmtime_val_t *args,
      size_t nargs,
      wasmtime_val_t *results,
      size_t nresults,
      wasm_trap_t **trap);
  શૂન્ય (*wasmtime_module_delete) (wasmtime_module_t *m);
  શૂન્ય (*wasmtime_store_delete) (wasmtime_store_t *store);
  શૂન્ય (*wasmtime_error_message) (અચલ wasmtime_error_t *error, wasm_name_t *message);
  શૂન્ય (*wasmtime_error_delete) (wasmtime_error_t *error);
  bool (*wasi_config_preopen_dir) (wasi_config_t *config, અચલ અક્ષર *path, અચલ અક્ષર *guest_path);

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

  જો (wasm_engine_new == ખાલી || wasm_engine_delete == ખાલી || wasm_byte_vec_delete == ખાલી
      || wasm_byte_vec_new_uninitialized == ખાલી || wasi_config_new == ખાલી || wasmtime_store_new == ખાલી
      || wasmtime_store_context == ખાલી || wasmtime_linker_new == ખાલી || wasmtime_linker_define_wasi == ખાલી
      || wasmtime_module_new == ખાલી || wasi_config_inherit_argv == ખાલી || wasi_config_inherit_stdout == ખાલી
      || wasi_config_inherit_stdin == ખાલી || wasi_config_inherit_stderr == ખાલી
      || wasi_config_inherit_env == ખાલી || wasmtime_context_set_wasi == ખાલી
      || wasmtime_linker_module == ખાલી || wasmtime_linker_get_default == ખાલી || wasmtime_func_call == ખાલી
      || wasmtime_module_delete == ખાલી || wasmtime_store_delete == ખાલી || wasi_config_set_argv == ખાલી
      || wasmtime_error_delete == ખાલી || wasmtime_error_message == ખાલી || wasi_config_preopen_dir == ખાલી
      || wasmtime_wat2wasm == ખાલી)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmtime.so`");

  // Set up wasmtime context
  wasm_engine_t *engine = wasm_engine_new ();
  પ્રતિજ્ઞા (engine != ખાલી);
  wasmtime_store_t *store = wasmtime_store_new (engine, ખાલી, ખાલી);
  પ્રતિજ્ઞા (store != ખાલી);
  wasmtime_context_t *context = wasmtime_store_context (store);

  // Link with wasi functions defined
  wasmtime_linker_t *linker = wasmtime_linker_new (engine);
  wasmtime_error_t *err = wasmtime_linker_define_wasi (linker);
  જો (err != ખાલી)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to link wasi: %.*s", (પૂર્ણાંક) error_message.size, error_message.data);
    }

  wasm_byte_vec_t wasm;
  // Load and parse container entrypoint
  FILE *file = fopen (pathname, "rbe");
  જો (! file)
    error (EXIT_FAILURE, 0, "error loading entrypoint");
  fseek (file, 0L, SEEK_END);
  size_t file_size = ftell (file);
  wasm_byte_vec_new_uninitialized (&wasm, file_size);
  fseek (file, 0L, SEEK_SET);
  જો (fread (wasm.data, file_size, 1, file) != 1)
    error (EXIT_FAILURE, 0, "error load");
  fclose (file);

  // If entrypoint contains a webassembly text format
  // compile it on the fly and convert to equivalent
  // binary format.
  જો (has_suffix (pathname, "wat") > 0)
    {
      wasmtime_error_t *err = wasmtime_wat2wasm ((અક્ષર *) &wasm_bytes, file_size, &wasm);
      જો (err != ખાલી)
        {
          wasmtime_error_message (err, &error_message);
          wasmtime_error_delete (err);
          error (EXIT_FAILURE, 0, "failed while compiling wat to wasm binary : %.*s", (પૂર્ણાંક) error_message.size, error_message.data);
        }
      wasm = wasm_bytes;
    }

  // Compile wasm modules
  wasmtime_module_t *module = ખાલી;
  err = wasmtime_module_new (engine, (uint8_t *) wasm.data, wasm.size, &module);
  જો (! module)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to compile module: %.*s", (પૂર્ણાંક) error_message.size, error_message.data);
    }
  wasm_byte_vec_delete (&wasm);

  // Init WASI program
  wasi_config_t *wasi_config = wasi_config_new ("crun_wasi_program");
  પ્રતિજ્ઞા (wasi_config);

  // Calculate argc for `wasi_config_set_argv`
  માટે (arg = argv; *arg != ખાલી; ++arg)
    args_size++;

  wasi_config_set_argv (wasi_config, args_size, (અચલ અક્ષર **) argv);
  wasi_config_inherit_env (wasi_config);
  wasi_config_inherit_stdin (wasi_config);
  wasi_config_inherit_stdout (wasi_config);
  wasi_config_inherit_stderr (wasi_config);
  wasi_config_preopen_dir (wasi_config, ".", ".");
  wasm_trap_t *trap = ખાલી;
  err = wasmtime_context_set_wasi (context, wasi_config);
  જો (err != ખાલી)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate WASI: %.*s", (પૂર્ણાંક) error_message.size, error_message.data);
    }

  // Init module
  err = wasmtime_linker_module (linker, context, "", 0, module);
  જો (err != ખાલી)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate module: %.*s", (પૂર્ણાંક) error_message.size, error_message.data);
    }

  // Actually run our .wasm
  wasmtime_func_t func;
  err = wasmtime_linker_get_default (linker, context, "", 0, &func);
  જો (err != ખાલી)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to locate default export for module %.*s", (પૂર્ણાંક) error_message.size, error_message.data);
    }

  err = wasmtime_func_call (context, &func, ખાલી, 0, ખાલી, 0, &trap);
  જો (err != ખાલી || trap != ખાલી)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "error calling default export: %.*s", (પૂર્ણાંક) error_message.size, error_message.data);
    }

  // Clean everything
  wasmtime_module_delete (module);
  wasmtime_store_delete (store);
  wasm_engine_delete (engine);

  exit (EXIT_SUCCESS);
}

સ્થિર પૂર્ણાંક
libwasmtime_load (શૂન્ય **cookie, libcrun_error_t *err)
{
  શૂન્ય *handle;

  handle = dlopen ("libwasmtime.so", RTLD_NOW);
  જો (handle == ખાલી)
    પાછા crun_make_error (err, 0, "could not load `libwasmtime.so`: %s", dlerror ());
  *cookie = handle;

  પાછા 0;
}

સ્થિર પૂર્ણાંક
libwasmtime_unload (શૂન્ય *cookie, libcrun_error_t *err)
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
libwasmtime_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  પાછા wasm_can_handle_container (container, err);
}

સંરચના custom_handler_s handler_wasmtime = {
  .name = "wasmtime",
  .alias = "wasm",
  .feature_string = "WASM:wasmtime",
  .supports_open_વૃક્ષ_namespace = false,
  .load = libwasmtime_load,
  .unload = libwasmtime_unload,
  .run_func = libwasmtime_exec,
  .can_handle_container = libwasmtime_can_handle_container,
};

#endif
