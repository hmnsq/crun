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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "../custom-handler.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../container.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../linux.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "handler-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sched.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DLOPEN
#  include <dlfcn.h>
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_WASMTIME
#  include <wasm.h>
#  include <wasi.h>
#  include <wasmtime.h>
#ਜੇ_ਅੰਤ

#ਜੇ HAVE_DLOPEN && HAVE_WASMTIME
ਸਥਿਰ ਪੂਰਨ
libwasmtime_exec (ਖਾਲੀ *cookie, libcrun_container_t *container arg_ਅਣਵਰਤਿਆ,
                  ਅਚਲ ਅੱਖਰ *pathname, ਅੱਖਰ *ਅਚਲ argv[])
{
  size_t ਦਲੀਲਾਂ_ਮਾਪ = 0;
  ਅੱਖਰ *ਅਚਲ *arg;
  wasm_byte_vec_t error_ਸੁਨੇਹਾ;
  wasm_byte_vec_t wasm_ਬਾਈਟਾਂ;
  wasm_engine_t *(*wasm_engine_new) ();
  wasmtime_error_t *(*wasmtime_wat2wasm) (ਅਚਲ ਅੱਖਰ *wat, size_t wat_len, wasm_byte_vec_t *ਬਾਹਰਲਾ);
  ਖਾਲੀ (*wasm_engine_delete) (wasm_engine_t *);
  ਖਾਲੀ (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  ਖਾਲੀ (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  wasi_config_t *(*wasi_config_new) (ਅਚਲ ਅੱਖਰ *);
  wasmtime_store_t *(*wasmtime_store_new) (wasm_engine_t *engine, ਖਾਲੀ *ਅੰਕੜੇ, ਖਾਲੀ (*finalizer) (ਖਾਲੀ *));
  wasmtime_context_t *(*wasmtime_store_context) (wasmtime_store_t *store);
  wasmtime_linker_t *(*wasmtime_linker_new) (wasm_engine_t *engine);
  wasmtime_error_t *(*wasmtime_linker_define_wasi) (wasmtime_linker_t *linker);
  wasmtime_error_t *(*wasmtime_module_new) (
      wasm_engine_t *engine,
      ਅਚਲ uint8_t *wasm,
      size_t wasm_len,
      wasmtime_module_t **ret);
  ਖਾਲੀ (*wasi_config_inherit_argv) (wasi_config_t *config);
  ਖਾਲੀ (*wasi_config_inherit_env) (wasi_config_t *config);
  ਖਾਲੀ (*wasi_config_ਸੈੱਟ_argv) (wasi_config_t *config, ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ *argv[]);
  ਖਾਲੀ (*wasi_config_inherit_stdin) (wasi_config_t *config);
  ਖਾਲੀ (*wasi_config_inherit_stdout) (wasi_config_t *config);
  ਖਾਲੀ (*wasi_config_inherit_stderr) (wasi_config_t *config);
  wasmtime_error_t *(*wasmtime_context_ਸੈੱਟ_wasi) (wasmtime_context_t *context, wasi_config_t *wasi);
  wasmtime_error_t *(*wasmtime_linker_module) (
      wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      ਅਚਲ ਅੱਖਰ *name,
      size_t name_len,
      ਅਚਲ wasmtime_module_t *module);
  wasmtime_error_t *(*wasmtime_linker_get_default) (
      ਅਚਲ wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      ਅਚਲ ਅੱਖਰ *name,
      size_t name_len,
      wasmtime_func_t *func);
  wasmtime_error_t *(*wasmtime_func_call) (
      wasmtime_context_t *store,
      ਅਚਲ wasmtime_func_t *func,
      ਅਚਲ wasmtime_val_t *ਦਲੀਲਾਂ,
      size_t nargs,
      wasmtime_val_t *results,
      size_t nresults,
      wasm_trap_t **trap);
  ਖਾਲੀ (*wasmtime_module_delete) (wasmtime_module_t *m);
  ਖਾਲੀ (*wasmtime_store_delete) (wasmtime_store_t *store);
  ਖਾਲੀ (*wasmtime_error_ਸੁਨੇਹਾ) (ਅਚਲ wasmtime_error_t *error, wasm_name_t *ਸੁਨੇਹਾ_2);
  ਖਾਲੀ (*wasmtime_error_delete) (wasmtime_error_t *error);
  bool (*wasi_config_preopen_dir) (wasi_config_t *config, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *guest_ਰਸਤਾ);

  wasmtime_wat2wasm = dlsym (cookie, "wasmtime_wat2wasm");
  wasm_engine_new = dlsym (cookie, "wasm_engine_new");
  wasm_engine_delete = dlsym (cookie, "wasm_engine_delete");
  wasm_byte_vec_delete = dlsym (cookie, "wasm_byte_vec_delete");
  wasm_byte_vec_new_uninitialized = dlsym (cookie, "wasm_byte_vec_new_uninitialized");
  wasi_config_new = dlsym (cookie, "wasi_config_new");
  wasi_config_ਸੈੱਟ_argv = dlsym (cookie, "wasi_config_set_argv");
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
  wasmtime_context_ਸੈੱਟ_wasi = dlsym (cookie, "wasmtime_context_set_wasi");
  wasmtime_linker_module = dlsym (cookie, "wasmtime_linker_module");
  wasmtime_linker_get_default = dlsym (cookie, "wasmtime_linker_get_default");
  wasmtime_func_call = dlsym (cookie, "wasmtime_func_call");
  wasmtime_module_delete = dlsym (cookie, "wasmtime_module_delete");
  wasmtime_store_delete = dlsym (cookie, "wasmtime_store_delete");
  wasmtime_error_delete = dlsym (cookie, "wasmtime_error_delete");
  wasmtime_error_ਸੁਨੇਹਾ = dlsym (cookie, "wasmtime_error_message");
  wasi_config_preopen_dir = dlsym (cookie, "wasi_config_preopen_dir");

  ਜੇ (wasm_engine_new == ਨਿਰਾ || wasm_engine_delete == ਨਿਰਾ || wasm_byte_vec_delete == ਨਿਰਾ
      || wasm_byte_vec_new_uninitialized == ਨਿਰਾ || wasi_config_new == ਨਿਰਾ || wasmtime_store_new == ਨਿਰਾ
      || wasmtime_store_context == ਨਿਰਾ || wasmtime_linker_new == ਨਿਰਾ || wasmtime_linker_define_wasi == ਨਿਰਾ
      || wasmtime_module_new == ਨਿਰਾ || wasi_config_inherit_argv == ਨਿਰਾ || wasi_config_inherit_stdout == ਨਿਰਾ
      || wasi_config_inherit_stdin == ਨਿਰਾ || wasi_config_inherit_stderr == ਨਿਰਾ
      || wasi_config_inherit_env == ਨਿਰਾ || wasmtime_context_ਸੈੱਟ_wasi == ਨਿਰਾ
      || wasmtime_linker_module == ਨਿਰਾ || wasmtime_linker_get_default == ਨਿਰਾ || wasmtime_func_call == ਨਿਰਾ
      || wasmtime_module_delete == ਨਿਰਾ || wasmtime_store_delete == ਨਿਰਾ || wasi_config_ਸੈੱਟ_argv == ਨਿਰਾ
      || wasmtime_error_delete == ਨਿਰਾ || wasmtime_error_ਸੁਨੇਹਾ == ਨਿਰਾ || wasi_config_preopen_dir == ਨਿਰਾ
      || wasmtime_wat2wasm == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmtime.so`");

  // Set up wasmtime context
  wasm_engine_t *engine = wasm_engine_new ();
  ਪ੍ਰਤਿਜ੍ਞਾ (engine != ਨਿਰਾ);
  wasmtime_store_t *store = wasmtime_store_new (engine, ਨਿਰਾ, ਨਿਰਾ);
  ਪ੍ਰਤਿਜ੍ਞਾ (store != ਨਿਰਾ);
  wasmtime_context_t *context = wasmtime_store_context (store);

  // Link with wasi functions defined
  wasmtime_linker_t *linker = wasmtime_linker_new (engine);
  wasmtime_error_t *err = wasmtime_linker_define_wasi (linker);
  ਜੇ (err != ਨਿਰਾ)
    {
      wasmtime_error_ਸੁਨੇਹਾ (err, &error_ਸੁਨੇਹਾ);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to link wasi: %.*s", (ਪੂਰਨ) error_ਸੁਨੇਹਾ.ਮਾਪ, error_ਸੁਨੇਹਾ.ਅੰਕੜੇ);
    }

  wasm_byte_vec_t wasm;
  // Load and parse container entrypoint
  FILE *file = fopen (pathname, "rbe");
  ਜੇ (! file)
    error (EXIT_FAILURE, 0, "error loading entrypoint");
  fseek (file, 0L, SEEK_END);
  size_t file_ਮਾਪ = ftell (file);
  wasm_byte_vec_new_uninitialized (&wasm, file_ਮਾਪ);
  fseek (file, 0L, SEEK_SET);
  ਜੇ (fread (wasm.ਅੰਕੜੇ, file_ਮਾਪ, 1, file) != 1)
    error (EXIT_FAILURE, 0, "error load");
  fclose (file);

  // If entrypoint contains a webassembly text format
  // compile it on the fly and convert to equivalent
  // binary format.
  ਜੇ (has_suffix (pathname, "wat") > 0)
    {
      wasmtime_error_t *err = wasmtime_wat2wasm ((ਅੱਖਰ *) &wasm_ਬਾਈਟਾਂ, file_ਮਾਪ, &wasm);
      ਜੇ (err != ਨਿਰਾ)
        {
          wasmtime_error_ਸੁਨੇਹਾ (err, &error_ਸੁਨੇਹਾ);
          wasmtime_error_delete (err);
          error (EXIT_FAILURE, 0, "failed while compiling wat to wasm binary : %.*s", (ਪੂਰਨ) error_ਸੁਨੇਹਾ.ਮਾਪ, error_ਸੁਨੇਹਾ.ਅੰਕੜੇ);
        }
      wasm = wasm_ਬਾਈਟਾਂ;
    }

  // Compile wasm modules
  wasmtime_module_t *module = ਨਿਰਾ;
  err = wasmtime_module_new (engine, (uint8_t *) wasm.ਅੰਕੜੇ, wasm.ਮਾਪ, &module);
  ਜੇ (! module)
    {
      wasmtime_error_ਸੁਨੇਹਾ (err, &error_ਸੁਨੇਹਾ);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to compile module: %.*s", (ਪੂਰਨ) error_ਸੁਨੇਹਾ.ਮਾਪ, error_ਸੁਨੇਹਾ.ਅੰਕੜੇ);
    }
  wasm_byte_vec_delete (&wasm);

  // Init WASI program
  wasi_config_t *wasi_config = wasi_config_new ("crun_wasi_program");
  ਪ੍ਰਤਿਜ੍ਞਾ (wasi_config);

  // Calculate argc for `wasi_config_set_argv`
  ਲਈ (arg = argv; *arg != ਨਿਰਾ; ++arg)
    ਦਲੀਲਾਂ_ਮਾਪ++;

  wasi_config_ਸੈੱਟ_argv (wasi_config, ਦਲੀਲਾਂ_ਮਾਪ, (ਅਚਲ ਅੱਖਰ **) argv);
  wasi_config_inherit_env (wasi_config);
  wasi_config_inherit_stdin (wasi_config);
  wasi_config_inherit_stdout (wasi_config);
  wasi_config_inherit_stderr (wasi_config);
  wasi_config_preopen_dir (wasi_config, ".", ".");
  wasm_trap_t *trap = ਨਿਰਾ;
  err = wasmtime_context_ਸੈੱਟ_wasi (context, wasi_config);
  ਜੇ (err != ਨਿਰਾ)
    {
      wasmtime_error_ਸੁਨੇਹਾ (err, &error_ਸੁਨੇਹਾ);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate WASI: %.*s", (ਪੂਰਨ) error_ਸੁਨੇਹਾ.ਮਾਪ, error_ਸੁਨੇਹਾ.ਅੰਕੜੇ);
    }

  // Init module
  err = wasmtime_linker_module (linker, context, "", 0, module);
  ਜੇ (err != ਨਿਰਾ)
    {
      wasmtime_error_ਸੁਨੇਹਾ (err, &error_ਸੁਨੇਹਾ);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate module: %.*s", (ਪੂਰਨ) error_ਸੁਨੇਹਾ.ਮਾਪ, error_ਸੁਨੇਹਾ.ਅੰਕੜੇ);
    }

  // Actually run our .wasm
  wasmtime_func_t func;
  err = wasmtime_linker_get_default (linker, context, "", 0, &func);
  ਜੇ (err != ਨਿਰਾ)
    {
      wasmtime_error_ਸੁਨੇਹਾ (err, &error_ਸੁਨੇਹਾ);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to locate default export for module %.*s", (ਪੂਰਨ) error_ਸੁਨੇਹਾ.ਮਾਪ, error_ਸੁਨੇਹਾ.ਅੰਕੜੇ);
    }

  err = wasmtime_func_call (context, &func, ਨਿਰਾ, 0, ਨਿਰਾ, 0, &trap);
  ਜੇ (err != ਨਿਰਾ || trap != ਨਿਰਾ)
    {
      wasmtime_error_ਸੁਨੇਹਾ (err, &error_ਸੁਨੇਹਾ);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "error calling default export: %.*s", (ਪੂਰਨ) error_ਸੁਨੇਹਾ.ਮਾਪ, error_ਸੁਨੇਹਾ.ਅੰਕੜੇ);
    }

  // Clean everything
  wasmtime_module_delete (module);
  wasmtime_store_delete (store);
  wasm_engine_delete (engine);

  ਬਾਹਰ_ਨਿਕਲੋ (EXIT_SUCCESS);
}

ਸਥਿਰ ਪੂਰਨ
libwasmtime_ਲੱਦੋ (ਖਾਲੀ **cookie, libcrun_error_t *err)
{
  ਖਾਲੀ *handle;

  handle = dlopen ("libwasmtime.so", RTLD_NOW);
  ਜੇ (handle == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, 0, "could not load `libwasmtime.so`: %s", dlerror ());
  *cookie = handle;

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
libwasmtime_unload (ਖਾਲੀ *cookie, libcrun_error_t *err)
{
  ਪੂਰਨ r;

  ਜੇ (cookie)
    {
      r = dlclose (cookie);
      ਜੇ (UNLIKELY (r < 0))
        ਵਾਪਸ crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
libwasmtime_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  ਵਾਪਸ wasm_can_handle_container (container, err);
}

ਬਣਤਰ custom_handler_s handler_wasmtime = {
  .name = "wasmtime",
  .alias = "wasm",
  .feature_string = "WASM:wasmtime",
  .supports_open_ਰੁੱਖ_namespace = false,
  .ਲੱਦੋ = libwasmtime_ਲੱਦੋ,
  .unload = libwasmtime_unload,
  .ਲੜੀਵਾਰ_func = libwasmtime_exec,
  .can_handle_container = libwasmtime_can_handle_container,
};

#ਜੇ_ਅੰਤ
