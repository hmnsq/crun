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

#شامل <config.h>
#شامل "../custom-handler.h"
#شامل "../container.h"
#شامل "../utils.h"
#شامل "../linux.h"
#شامل "handler-utils.h"
#شامل <unistd.h>
#شامل <sys/stat.h>
#شامل <errno.h>
#شامل <sys/types.h>
#شامل <fcntl.h>
#شامل <sched.h>
#شامل <assert.h>
#شامل <stdio.h>
#شامل <stdlib.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMTIME
#  include <wasm.h>
#  include <wasi.h>
#  include <wasmtime.h>
#endif

#اگر HAVE_DLOPEN && HAVE_WASMTIME
ایستا صحیح
libwasmtime_exec (تهی *cookie, libcrun_container_t *container arg_unused,
                  ثابت نویسه *pathname, نویسه *ثابت argv[])
{
  size_t args_size = 0;
  نویسه *ثابت *arg;
  wasm_byte_vec_t error_message;
  wasm_byte_vec_t wasm_bytes;
  wasm_engine_t *(*wasm_engine_new) ();
  wasmtime_error_t *(*wasmtime_wat2wasm) (ثابت نویسه *wat, size_t wat_len, wasm_byte_vec_t *out);
  تهی (*wasm_engine_delete) (wasm_engine_t *);
  تهی (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  تهی (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  wasi_config_t *(*wasi_config_new) (ثابت نویسه *);
  wasmtime_store_t *(*wasmtime_store_new) (wasm_engine_t *engine, تهی *data, تهی (*finalizer) (تهی *));
  wasmtime_context_t *(*wasmtime_store_context) (wasmtime_store_t *store);
  wasmtime_linker_t *(*wasmtime_linker_new) (wasm_engine_t *engine);
  wasmtime_error_t *(*wasmtime_linker_define_wasi) (wasmtime_linker_t *linker);
  wasmtime_error_t *(*wasmtime_module_new) (
      wasm_engine_t *engine,
      ثابت uint8_t *wasm,
      size_t wasm_len,
      wasmtime_module_t **ret);
  تهی (*wasi_config_inherit_argv) (wasi_config_t *config);
  تهی (*wasi_config_inherit_env) (wasi_config_t *config);
  تهی (*wasi_config_set_argv) (wasi_config_t *config, صحیح argc, ثابت نویسه *argv[]);
  تهی (*wasi_config_inherit_stdin) (wasi_config_t *config);
  تهی (*wasi_config_inherit_stdout) (wasi_config_t *config);
  تهی (*wasi_config_inherit_stderr) (wasi_config_t *config);
  wasmtime_error_t *(*wasmtime_context_set_wasi) (wasmtime_context_t *context, wasi_config_t *wasi);
  wasmtime_error_t *(*wasmtime_linker_module) (
      wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      ثابت نویسه *name,
      size_t name_len,
      ثابت wasmtime_module_t *module);
  wasmtime_error_t *(*wasmtime_linker_get_default) (
      ثابت wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      ثابت نویسه *name,
      size_t name_len,
      wasmtime_func_t *func);
  wasmtime_error_t *(*wasmtime_func_call) (
      wasmtime_context_t *store,
      ثابت wasmtime_func_t *func,
      ثابت wasmtime_val_t *args,
      size_t nargs,
      wasmtime_val_t *results,
      size_t nresults,
      wasm_trap_t **trap);
  تهی (*wasmtime_module_delete) (wasmtime_module_t *m);
  تهی (*wasmtime_store_delete) (wasmtime_store_t *store);
  تهی (*wasmtime_error_message) (ثابت wasmtime_error_t *error, wasm_name_t *message);
  تهی (*wasmtime_error_delete) (wasmtime_error_t *error);
  bool (*wasi_config_preopen_dir) (wasi_config_t *config, ثابت نویسه *path, ثابت نویسه *guest_path);

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

  اگر (wasm_engine_new == پوچ || wasm_engine_delete == پوچ || wasm_byte_vec_delete == پوچ
      || wasm_byte_vec_new_uninitialized == پوچ || wasi_config_new == پوچ || wasmtime_store_new == پوچ
      || wasmtime_store_context == پوچ || wasmtime_linker_new == پوچ || wasmtime_linker_define_wasi == پوچ
      || wasmtime_module_new == پوچ || wasi_config_inherit_argv == پوچ || wasi_config_inherit_stdout == پوچ
      || wasi_config_inherit_stdin == پوچ || wasi_config_inherit_stderr == پوچ
      || wasi_config_inherit_env == پوچ || wasmtime_context_set_wasi == پوچ
      || wasmtime_linker_module == پوچ || wasmtime_linker_get_default == پوچ || wasmtime_func_call == پوچ
      || wasmtime_module_delete == پوچ || wasmtime_store_delete == پوچ || wasi_config_set_argv == پوچ
      || wasmtime_error_delete == پوچ || wasmtime_error_message == پوچ || wasi_config_preopen_dir == پوچ
      || wasmtime_wat2wasm == پوچ)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmtime.so`");

  // Set up wasmtime context
  wasm_engine_t *engine = wasm_engine_new ();
  أكد (engine != پوچ);
  wasmtime_store_t *store = wasmtime_store_new (engine, پوچ, پوچ);
  أكد (store != پوچ);
  wasmtime_context_t *context = wasmtime_store_context (store);

  // Link with wasi functions defined
  wasmtime_linker_t *linker = wasmtime_linker_new (engine);
  wasmtime_error_t *err = wasmtime_linker_define_wasi (linker);
  اگر (err != پوچ)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to link wasi: %.*s", (صحیح) error_message.size, error_message.data);
    }

  wasm_byte_vec_t wasm;
  // Load and parse container entrypoint
  FILE *file = fopen (pathname, "rbe");
  اگر (! file)
    error (EXIT_FAILURE, 0, "error loading entrypoint");
  fseek (file, 0L, SEEK_END);
  size_t file_size = ftell (file);
  wasm_byte_vec_new_uninitialized (&wasm, file_size);
  fseek (file, 0L, SEEK_SET);
  اگر (fread (wasm.data, file_size, 1, file) != 1)
    error (EXIT_FAILURE, 0, "error load");
  fclose (file);

  // If entrypoint contains a webassembly text format
  // compile it on the fly and convert to equivalent
  // binary format.
  اگر (has_suffix (pathname, "wat") > 0)
    {
      wasmtime_error_t *err = wasmtime_wat2wasm ((نویسه *) &wasm_bytes, file_size, &wasm);
      اگر (err != پوچ)
        {
          wasmtime_error_message (err, &error_message);
          wasmtime_error_delete (err);
          error (EXIT_FAILURE, 0, "failed while compiling wat to wasm binary : %.*s", (صحیح) error_message.size, error_message.data);
        }
      wasm = wasm_bytes;
    }

  // Compile wasm modules
  wasmtime_module_t *module = پوچ;
  err = wasmtime_module_new (engine, (uint8_t *) wasm.data, wasm.size, &module);
  اگر (! module)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to compile module: %.*s", (صحیح) error_message.size, error_message.data);
    }
  wasm_byte_vec_delete (&wasm);

  // Init WASI program
  wasi_config_t *wasi_config = wasi_config_new ("crun_wasi_program");
  أكد (wasi_config);

  // Calculate argc for `wasi_config_set_argv`
  برای (arg = argv; *arg != پوچ; ++arg)
    args_size++;

  wasi_config_set_argv (wasi_config, args_size, (ثابت نویسه **) argv);
  wasi_config_inherit_env (wasi_config);
  wasi_config_inherit_stdin (wasi_config);
  wasi_config_inherit_stdout (wasi_config);
  wasi_config_inherit_stderr (wasi_config);
  wasi_config_preopen_dir (wasi_config, ".", ".");
  wasm_trap_t *trap = پوچ;
  err = wasmtime_context_set_wasi (context, wasi_config);
  اگر (err != پوچ)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate WASI: %.*s", (صحیح) error_message.size, error_message.data);
    }

  // Init module
  err = wasmtime_linker_module (linker, context, "", 0, module);
  اگر (err != پوچ)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate module: %.*s", (صحیح) error_message.size, error_message.data);
    }

  // Actually run our .wasm
  wasmtime_func_t func;
  err = wasmtime_linker_get_default (linker, context, "", 0, &func);
  اگر (err != پوچ)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to locate default export for module %.*s", (صحیح) error_message.size, error_message.data);
    }

  err = wasmtime_func_call (context, &func, پوچ, 0, پوچ, 0, &trap);
  اگر (err != پوچ || trap != پوچ)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "error calling default export: %.*s", (صحیح) error_message.size, error_message.data);
    }

  // Clean everything
  wasmtime_module_delete (module);
  wasmtime_store_delete (store);
  wasm_engine_delete (engine);

  exit (EXIT_SUCCESS);
}

ایستا صحیح
libwasmtime_load (تهی **cookie, libcrun_error_t *err)
{
  تهی *handle;

  handle = dlopen ("libwasmtime.so", RTLD_NOW);
  اگر (handle == پوچ)
    بازگشت crun_make_error (err, 0, "could not load `libwasmtime.so`: %s", dlerror ());
  *cookie = handle;

  بازگشت 0;
}

ایستا صحیح
libwasmtime_unload (تهی *cookie, libcrun_error_t *err)
{
  صحیح r;

  اگر (cookie)
    {
      r = dlclose (cookie);
      اگر (UNLIKELY (r < 0))
        بازگشت crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  بازگشت 0;
}

ایستا صحیح
libwasmtime_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  بازگشت wasm_can_handle_container (container, err);
}

ساختار custom_handler_s handler_wasmtime = {
  .name = "wasmtime",
  .alias = "wasm",
  .feature_string = "WASM:wasmtime",
  .supports_open_درخت_namespace = false,
  .load = libwasmtime_load,
  .unload = libwasmtime_unload,
  .run_func = libwasmtime_exec,
  .can_handle_container = libwasmtime_can_handle_container,
};

#endif
