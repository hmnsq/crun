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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "../custom-handler.h"
#ଅନ୍ତର୍ଭାଵ "../container.h"
#ଅନ୍ତର୍ଭାଵ "../utils.h"
#ଅନ୍ତର୍ଭାଵ "../linux.h"
#ଅନ୍ତର୍ଭାଵ "handler-utils.h"
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <sched.h>
#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMTIME
#  include <wasm.h>
#  include <wasi.h>
#  include <wasmtime.h>
#endif

#ଯଦି HAVE_DLOPEN && HAVE_WASMTIME
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libwasmtime_exec (ଶୂନ୍ୟ *cookie, libcrun_container_t *container arg_unused,
                  ଅଚଳ ଅକ୍ଷର *pathname, ଅକ୍ଷର *ଅଚଳ argv[])
{
  size_t args_size = 0;
  ଅକ୍ଷର *ଅଚଳ *arg;
  wasm_byte_vec_t error_message;
  wasm_byte_vec_t wasm_bytes;
  wasm_engine_t *(*wasm_engine_new) ();
  wasmtime_error_t *(*wasmtime_wat2wasm) (ଅଚଳ ଅକ୍ଷର *wat, size_t wat_len, wasm_byte_vec_t *out);
  ଶୂନ୍ୟ (*wasm_engine_delete) (wasm_engine_t *);
  ଶୂନ୍ୟ (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  ଶୂନ୍ୟ (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  wasi_config_t *(*wasi_config_new) (ଅଚଳ ଅକ୍ଷର *);
  wasmtime_store_t *(*wasmtime_store_new) (wasm_engine_t *engine, ଶୂନ୍ୟ *data, ଶୂନ୍ୟ (*finalizer) (ଶୂନ୍ୟ *));
  wasmtime_context_t *(*wasmtime_store_context) (wasmtime_store_t *store);
  wasmtime_linker_t *(*wasmtime_linker_new) (wasm_engine_t *engine);
  wasmtime_error_t *(*wasmtime_linker_define_wasi) (wasmtime_linker_t *linker);
  wasmtime_error_t *(*wasmtime_module_new) (
      wasm_engine_t *engine,
      ଅଚଳ uint8_t *wasm,
      size_t wasm_len,
      wasmtime_module_t **ret);
  ଶୂନ୍ୟ (*wasi_config_inherit_argv) (wasi_config_t *config);
  ଶୂନ୍ୟ (*wasi_config_inherit_env) (wasi_config_t *config);
  ଶୂନ୍ୟ (*wasi_config_set_argv) (wasi_config_t *config, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର *argv[]);
  ଶୂନ୍ୟ (*wasi_config_inherit_stdin) (wasi_config_t *config);
  ଶୂନ୍ୟ (*wasi_config_inherit_stdout) (wasi_config_t *config);
  ଶୂନ୍ୟ (*wasi_config_inherit_stderr) (wasi_config_t *config);
  wasmtime_error_t *(*wasmtime_context_set_wasi) (wasmtime_context_t *context, wasi_config_t *wasi);
  wasmtime_error_t *(*wasmtime_linker_module) (
      wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      ଅଚଳ ଅକ୍ଷର *name,
      size_t name_len,
      ଅଚଳ wasmtime_module_t *module);
  wasmtime_error_t *(*wasmtime_linker_get_default) (
      ଅଚଳ wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      ଅଚଳ ଅକ୍ଷର *name,
      size_t name_len,
      wasmtime_func_t *func);
  wasmtime_error_t *(*wasmtime_func_call) (
      wasmtime_context_t *store,
      ଅଚଳ wasmtime_func_t *func,
      ଅଚଳ wasmtime_val_t *args,
      size_t nargs,
      wasmtime_val_t *results,
      size_t nresults,
      wasm_trap_t **trap);
  ଶୂନ୍ୟ (*wasmtime_module_delete) (wasmtime_module_t *m);
  ଶୂନ୍ୟ (*wasmtime_store_delete) (wasmtime_store_t *store);
  ଶୂନ୍ୟ (*wasmtime_error_message) (ଅଚଳ wasmtime_error_t *error, wasm_name_t *message);
  ଶୂନ୍ୟ (*wasmtime_error_delete) (wasmtime_error_t *error);
  bool (*wasi_config_preopen_dir) (wasi_config_t *config, ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *guest_path);

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

  ଯଦି (wasm_engine_new == ଖାଲି || wasm_engine_delete == ଖାଲି || wasm_byte_vec_delete == ଖାଲି
      || wasm_byte_vec_new_uninitialized == ଖାଲି || wasi_config_new == ଖାଲି || wasmtime_store_new == ଖାଲି
      || wasmtime_store_context == ଖାଲି || wasmtime_linker_new == ଖାଲି || wasmtime_linker_define_wasi == ଖାଲି
      || wasmtime_module_new == ଖାଲି || wasi_config_inherit_argv == ଖାଲି || wasi_config_inherit_stdout == ଖାଲି
      || wasi_config_inherit_stdin == ଖାଲି || wasi_config_inherit_stderr == ଖାଲି
      || wasi_config_inherit_env == ଖାଲି || wasmtime_context_set_wasi == ଖାଲି
      || wasmtime_linker_module == ଖାଲି || wasmtime_linker_get_default == ଖାଲି || wasmtime_func_call == ଖାଲି
      || wasmtime_module_delete == ଖାଲି || wasmtime_store_delete == ଖାଲି || wasi_config_set_argv == ଖାଲି
      || wasmtime_error_delete == ଖାଲି || wasmtime_error_message == ଖାଲି || wasi_config_preopen_dir == ଖାଲି
      || wasmtime_wat2wasm == ଖାଲି)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmtime.so`");

  // Set up wasmtime context
  wasm_engine_t *engine = wasm_engine_new ();
  ପ୍ରତିଜ୍ଞା (engine != ଖାଲି);
  wasmtime_store_t *store = wasmtime_store_new (engine, ଖାଲି, ଖାଲି);
  ପ୍ରତିଜ୍ଞା (store != ଖାଲି);
  wasmtime_context_t *context = wasmtime_store_context (store);

  // Link with wasi functions defined
  wasmtime_linker_t *linker = wasmtime_linker_new (engine);
  wasmtime_error_t *err = wasmtime_linker_define_wasi (linker);
  ଯଦି (err != ଖାଲି)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to link wasi: %.*s", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) error_message.size, error_message.data);
    }

  wasm_byte_vec_t wasm;
  // Load and parse container entrypoint
  FILE *file = fopen (pathname, "rbe");
  ଯଦି (! file)
    error (EXIT_FAILURE, 0, "error loading entrypoint");
  fseek (file, 0L, SEEK_END);
  size_t file_size = ftell (file);
  wasm_byte_vec_new_uninitialized (&wasm, file_size);
  fseek (file, 0L, SEEK_SET);
  ଯଦି (fread (wasm.data, file_size, 1, file) != 1)
    error (EXIT_FAILURE, 0, "error load");
  fclose (file);

  // If entrypoint contains a webassembly text format
  // compile it on the fly and convert to equivalent
  // binary format.
  ଯଦି (has_suffix (pathname, "wat") > 0)
    {
      wasmtime_error_t *err = wasmtime_wat2wasm ((ଅକ୍ଷର *) &wasm_bytes, file_size, &wasm);
      ଯଦି (err != ଖାଲି)
        {
          wasmtime_error_message (err, &error_message);
          wasmtime_error_delete (err);
          error (EXIT_FAILURE, 0, "failed while compiling wat to wasm binary : %.*s", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) error_message.size, error_message.data);
        }
      wasm = wasm_bytes;
    }

  // Compile wasm modules
  wasmtime_module_t *module = ଖାଲି;
  err = wasmtime_module_new (engine, (uint8_t *) wasm.data, wasm.size, &module);
  ଯଦି (! module)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to compile module: %.*s", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) error_message.size, error_message.data);
    }
  wasm_byte_vec_delete (&wasm);

  // Init WASI program
  wasi_config_t *wasi_config = wasi_config_new ("crun_wasi_program");
  ପ୍ରତିଜ୍ଞା (wasi_config);

  // Calculate argc for `wasi_config_set_argv`
  ପାଇଁ (arg = argv; *arg != ଖାଲି; ++arg)
    args_size++;

  wasi_config_set_argv (wasi_config, args_size, (ଅଚଳ ଅକ୍ଷର **) argv);
  wasi_config_inherit_env (wasi_config);
  wasi_config_inherit_stdin (wasi_config);
  wasi_config_inherit_stdout (wasi_config);
  wasi_config_inherit_stderr (wasi_config);
  wasi_config_preopen_dir (wasi_config, ".", ".");
  wasm_trap_t *trap = ଖାଲି;
  err = wasmtime_context_set_wasi (context, wasi_config);
  ଯଦି (err != ଖାଲି)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate WASI: %.*s", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) error_message.size, error_message.data);
    }

  // Init module
  err = wasmtime_linker_module (linker, context, "", 0, module);
  ଯଦି (err != ଖାଲି)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate module: %.*s", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) error_message.size, error_message.data);
    }

  // Actually run our .wasm
  wasmtime_func_t func;
  err = wasmtime_linker_get_default (linker, context, "", 0, &func);
  ଯଦି (err != ଖାଲି)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to locate default export for module %.*s", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) error_message.size, error_message.data);
    }

  err = wasmtime_func_call (context, &func, ଖାଲି, 0, ଖାଲି, 0, &trap);
  ଯଦି (err != ଖାଲି || trap != ଖାଲି)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "error calling default export: %.*s", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) error_message.size, error_message.data);
    }

  // Clean everything
  wasmtime_module_delete (module);
  wasmtime_store_delete (store);
  wasm_engine_delete (engine);

  exit (EXIT_SUCCESS);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libwasmtime_load (ଶୂନ୍ୟ **cookie, libcrun_error_t *err)
{
  ଶୂନ୍ୟ *handle;

  handle = dlopen ("libwasmtime.so", RTLD_NOW);
  ଯଦି (handle == ଖାଲି)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "could not load `libwasmtime.so`: %s", dlerror ());
  *cookie = handle;

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libwasmtime_unload (ଶୂନ୍ୟ *cookie, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା r;

  ଯଦି (cookie)
    {
      r = dlclose (cookie);
      ଯଦି (UNLIKELY (r < 0))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libwasmtime_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ wasm_can_handle_container (container, err);
}

ସଂରଚନା custom_handler_s handler_wasmtime = {
  .name = "wasmtime",
  .alias = "wasm",
  .feature_string = "WASM:wasmtime",
  .supports_open_ବୃକ୍ଷ_namespace = false,
  .load = libwasmtime_load,
  .unload = libwasmtime_unload,
  .run_func = libwasmtime_exec,
  .can_handle_container = libwasmtime_can_handle_container,
};

#endif
