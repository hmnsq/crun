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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "../custom-handler.h"
#അന്തര്ഭാവ "../container.h"
#അന്തര്ഭാവ "../utils.h"
#അന്തര്ഭാവ "../linux.h"
#അന്തര്ഭാവ "handler-utils.h"
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <sched.h>
#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMTIME
#  include <wasm.h>
#  include <wasi.h>
#  include <wasmtime.h>
#endif

#എങ്കിൽ HAVE_DLOPEN && HAVE_WASMTIME
സ്ഥിതം പൂർണ്ണസംഖ്യ
libwasmtime_exec (ശൂന്യം *cookie, libcrun_container_t *container arg_unused,
                  സ്ഥിരം അക്ഷരം *pathname, അക്ഷരം *സ്ഥിരം argv[])
{
  size_t args_size = 0;
  അക്ഷരം *സ്ഥിരം *arg;
  wasm_byte_vec_t error_message;
  wasm_byte_vec_t wasm_bytes;
  wasm_engine_t *(*wasm_engine_new) ();
  wasmtime_error_t *(*wasmtime_wat2wasm) (സ്ഥിരം അക്ഷരം *wat, size_t wat_len, wasm_byte_vec_t *out);
  ശൂന്യം (*wasm_engine_delete) (wasm_engine_t *);
  ശൂന്യം (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  ശൂന്യം (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  wasi_config_t *(*wasi_config_new) (സ്ഥിരം അക്ഷരം *);
  wasmtime_store_t *(*wasmtime_store_new) (wasm_engine_t *engine, ശൂന്യം *data, ശൂന്യം (*finalizer) (ശൂന്യം *));
  wasmtime_context_t *(*wasmtime_store_context) (wasmtime_store_t *store);
  wasmtime_linker_t *(*wasmtime_linker_new) (wasm_engine_t *engine);
  wasmtime_error_t *(*wasmtime_linker_define_wasi) (wasmtime_linker_t *linker);
  wasmtime_error_t *(*wasmtime_module_new) (
      wasm_engine_t *engine,
      സ്ഥിരം uint8_t *wasm,
      size_t wasm_len,
      wasmtime_module_t **ret);
  ശൂന്യം (*wasi_config_inherit_argv) (wasi_config_t *config);
  ശൂന്യം (*wasi_config_inherit_env) (wasi_config_t *config);
  ശൂന്യം (*wasi_config_set_argv) (wasi_config_t *config, പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം *argv[]);
  ശൂന്യം (*wasi_config_inherit_stdin) (wasi_config_t *config);
  ശൂന്യം (*wasi_config_inherit_stdout) (wasi_config_t *config);
  ശൂന്യം (*wasi_config_inherit_stderr) (wasi_config_t *config);
  wasmtime_error_t *(*wasmtime_context_set_wasi) (wasmtime_context_t *context, wasi_config_t *wasi);
  wasmtime_error_t *(*wasmtime_linker_module) (
      wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      സ്ഥിരം അക്ഷരം *name,
      size_t name_len,
      സ്ഥിരം wasmtime_module_t *module);
  wasmtime_error_t *(*wasmtime_linker_get_default) (
      സ്ഥിരം wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      സ്ഥിരം അക്ഷരം *name,
      size_t name_len,
      wasmtime_func_t *func);
  wasmtime_error_t *(*wasmtime_func_call) (
      wasmtime_context_t *store,
      സ്ഥിരം wasmtime_func_t *func,
      സ്ഥിരം wasmtime_val_t *args,
      size_t nargs,
      wasmtime_val_t *results,
      size_t nresults,
      wasm_trap_t **trap);
  ശൂന്യം (*wasmtime_module_delete) (wasmtime_module_t *m);
  ശൂന്യം (*wasmtime_store_delete) (wasmtime_store_t *store);
  ശൂന്യം (*wasmtime_error_message) (സ്ഥിരം wasmtime_error_t *error, wasm_name_t *message);
  ശൂന്യം (*wasmtime_error_delete) (wasmtime_error_t *error);
  bool (*wasi_config_preopen_dir) (wasi_config_t *config, സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *guest_path);

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

  എങ്കിൽ (wasm_engine_new == ശൂന്യ || wasm_engine_delete == ശൂന്യ || wasm_byte_vec_delete == ശൂന്യ
      || wasm_byte_vec_new_uninitialized == ശൂന്യ || wasi_config_new == ശൂന്യ || wasmtime_store_new == ശൂന്യ
      || wasmtime_store_context == ശൂന്യ || wasmtime_linker_new == ശൂന്യ || wasmtime_linker_define_wasi == ശൂന്യ
      || wasmtime_module_new == ശൂന്യ || wasi_config_inherit_argv == ശൂന്യ || wasi_config_inherit_stdout == ശൂന്യ
      || wasi_config_inherit_stdin == ശൂന്യ || wasi_config_inherit_stderr == ശൂന്യ
      || wasi_config_inherit_env == ശൂന്യ || wasmtime_context_set_wasi == ശൂന്യ
      || wasmtime_linker_module == ശൂന്യ || wasmtime_linker_get_default == ശൂന്യ || wasmtime_func_call == ശൂന്യ
      || wasmtime_module_delete == ശൂന്യ || wasmtime_store_delete == ശൂന്യ || wasi_config_set_argv == ശൂന്യ
      || wasmtime_error_delete == ശൂന്യ || wasmtime_error_message == ശൂന്യ || wasi_config_preopen_dir == ശൂന്യ
      || wasmtime_wat2wasm == ശൂന്യ)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmtime.so`");

  // Set up wasmtime context
  wasm_engine_t *engine = wasm_engine_new ();
  പ്രതിജ്ഞാ (engine != ശൂന്യ);
  wasmtime_store_t *store = wasmtime_store_new (engine, ശൂന്യ, ശൂന്യ);
  പ്രതിജ്ഞാ (store != ശൂന്യ);
  wasmtime_context_t *context = wasmtime_store_context (store);

  // Link with wasi functions defined
  wasmtime_linker_t *linker = wasmtime_linker_new (engine);
  wasmtime_error_t *err = wasmtime_linker_define_wasi (linker);
  എങ്കിൽ (err != ശൂന്യ)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to link wasi: %.*s", (പൂർണ്ണസംഖ്യ) error_message.size, error_message.data);
    }

  wasm_byte_vec_t wasm;
  // Load and parse container entrypoint
  FILE *file = fopen (pathname, "rbe");
  എങ്കിൽ (! file)
    error (EXIT_FAILURE, 0, "error loading entrypoint");
  fseek (file, 0L, SEEK_END);
  size_t file_size = ftell (file);
  wasm_byte_vec_new_uninitialized (&wasm, file_size);
  fseek (file, 0L, SEEK_SET);
  എങ്കിൽ (fread (wasm.data, file_size, 1, file) != 1)
    error (EXIT_FAILURE, 0, "error load");
  fclose (file);

  // If entrypoint contains a webassembly text format
  // compile it on the fly and convert to equivalent
  // binary format.
  എങ്കിൽ (has_suffix (pathname, "wat") > 0)
    {
      wasmtime_error_t *err = wasmtime_wat2wasm ((അക്ഷരം *) &wasm_bytes, file_size, &wasm);
      എങ്കിൽ (err != ശൂന്യ)
        {
          wasmtime_error_message (err, &error_message);
          wasmtime_error_delete (err);
          error (EXIT_FAILURE, 0, "failed while compiling wat to wasm binary : %.*s", (പൂർണ്ണസംഖ്യ) error_message.size, error_message.data);
        }
      wasm = wasm_bytes;
    }

  // Compile wasm modules
  wasmtime_module_t *module = ശൂന്യ;
  err = wasmtime_module_new (engine, (uint8_t *) wasm.data, wasm.size, &module);
  എങ്കിൽ (! module)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to compile module: %.*s", (പൂർണ്ണസംഖ്യ) error_message.size, error_message.data);
    }
  wasm_byte_vec_delete (&wasm);

  // Init WASI program
  wasi_config_t *wasi_config = wasi_config_new ("crun_wasi_program");
  പ്രതിജ്ഞാ (wasi_config);

  // Calculate argc for `wasi_config_set_argv`
  വേണ്ടി (arg = argv; *arg != ശൂന്യ; ++arg)
    args_size++;

  wasi_config_set_argv (wasi_config, args_size, (സ്ഥിരം അക്ഷരം **) argv);
  wasi_config_inherit_env (wasi_config);
  wasi_config_inherit_stdin (wasi_config);
  wasi_config_inherit_stdout (wasi_config);
  wasi_config_inherit_stderr (wasi_config);
  wasi_config_preopen_dir (wasi_config, ".", ".");
  wasm_trap_t *trap = ശൂന്യ;
  err = wasmtime_context_set_wasi (context, wasi_config);
  എങ്കിൽ (err != ശൂന്യ)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate WASI: %.*s", (പൂർണ്ണസംഖ്യ) error_message.size, error_message.data);
    }

  // Init module
  err = wasmtime_linker_module (linker, context, "", 0, module);
  എങ്കിൽ (err != ശൂന്യ)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate module: %.*s", (പൂർണ്ണസംഖ്യ) error_message.size, error_message.data);
    }

  // Actually run our .wasm
  wasmtime_func_t func;
  err = wasmtime_linker_get_default (linker, context, "", 0, &func);
  എങ്കിൽ (err != ശൂന്യ)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to locate default export for module %.*s", (പൂർണ്ണസംഖ്യ) error_message.size, error_message.data);
    }

  err = wasmtime_func_call (context, &func, ശൂന്യ, 0, ശൂന്യ, 0, &trap);
  എങ്കിൽ (err != ശൂന്യ || trap != ശൂന്യ)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "error calling default export: %.*s", (പൂർണ്ണസംഖ്യ) error_message.size, error_message.data);
    }

  // Clean everything
  wasmtime_module_delete (module);
  wasmtime_store_delete (store);
  wasm_engine_delete (engine);

  exit (EXIT_SUCCESS);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libwasmtime_load (ശൂന്യം **cookie, libcrun_error_t *err)
{
  ശൂന്യം *handle;

  handle = dlopen ("libwasmtime.so", RTLD_NOW);
  എങ്കിൽ (handle == ശൂന്യ)
    തിരികെ crun_make_error (err, 0, "could not load `libwasmtime.so`: %s", dlerror ());
  *cookie = handle;

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libwasmtime_unload (ശൂന്യം *cookie, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ r;

  എങ്കിൽ (cookie)
    {
      r = dlclose (cookie);
      എങ്കിൽ (UNLIKELY (r < 0))
        തിരികെ crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libwasmtime_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  തിരികെ wasm_can_handle_container (container, err);
}

ഘടന custom_handler_s handler_wasmtime = {
  .name = "wasmtime",
  .alias = "wasm",
  .feature_string = "WASM:wasmtime",
  .supports_open_വൃക്ഷം_namespace = false,
  .load = libwasmtime_load,
  .unload = libwasmtime_unload,
  .run_func = libwasmtime_exec,
  .can_handle_container = libwasmtime_can_handle_container,
};

#endif
