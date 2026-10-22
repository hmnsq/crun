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
#تعریف_کرو GNU_ماخذ

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

#اگر_تعریف_شدہ HAVE_DLOPEN
#  include <dlfcn.h>
#اگر_ختم

#اگر_تعریف_شدہ HAVE_WASMTIME
#  include <wasm.h>
#  include <wasi.h>
#  include <wasmtime.h>
#اگر_ختم

#اگر HAVE_DLOPEN && HAVE_WASMTIME
ساکن صحیح
libwasmtime_exec (خالی *cookie, libcrun_container_t *container arg_غیر_مستعمل,
                  ثابت حرف *pathname, حرف *ثابت argv[])
{
  size_t دلائل_سائز = 0;
  حرف *ثابت *arg;
  wasm_byte_vec_t error_پیغام;
  wasm_byte_vec_t wasm_بائٹیں;
  wasm_engine_t *(*wasm_engine_new) ();
  wasmtime_error_t *(*wasmtime_wat2wasm) (ثابت حرف *wat, size_t wat_len, wasm_byte_vec_t *برآمد);
  خالی (*wasm_engine_delete) (wasm_engine_t *);
  خالی (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  خالی (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  wasi_config_t *(*wasi_config_new) (ثابت حرف *);
  wasmtime_store_t *(*wasmtime_store_new) (wasm_engine_t *engine, خالی *ڈیٹا, خالی (*finalizer) (خالی *));
  wasmtime_context_t *(*wasmtime_store_context) (wasmtime_store_t *store);
  wasmtime_linker_t *(*wasmtime_linker_new) (wasm_engine_t *engine);
  wasmtime_error_t *(*wasmtime_linker_define_wasi) (wasmtime_linker_t *linker);
  wasmtime_error_t *(*wasmtime_module_new) (
      wasm_engine_t *engine,
      ثابت uint8_t *wasm,
      size_t wasm_len,
      wasmtime_module_t **ret);
  خالی (*wasi_config_inherit_argv) (wasi_config_t *config);
  خالی (*wasi_config_inherit_env) (wasi_config_t *config);
  خالی (*wasi_config_سیٹ_argv) (wasi_config_t *config, صحیح argc, ثابت حرف *argv[]);
  خالی (*wasi_config_inherit_stdin) (wasi_config_t *config);
  خالی (*wasi_config_inherit_stdout) (wasi_config_t *config);
  خالی (*wasi_config_inherit_stderr) (wasi_config_t *config);
  wasmtime_error_t *(*wasmtime_context_سیٹ_wasi) (wasmtime_context_t *context, wasi_config_t *wasi);
  wasmtime_error_t *(*wasmtime_linker_module) (
      wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      ثابت حرف *name,
      size_t name_len,
      ثابت wasmtime_module_t *module);
  wasmtime_error_t *(*wasmtime_linker_get_default) (
      ثابت wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      ثابت حرف *name,
      size_t name_len,
      wasmtime_func_t *func);
  wasmtime_error_t *(*wasmtime_func_call) (
      wasmtime_context_t *store,
      ثابت wasmtime_func_t *func,
      ثابت wasmtime_val_t *دلائل,
      size_t nargs,
      wasmtime_val_t *results,
      size_t nresults,
      wasm_trap_t **trap);
  خالی (*wasmtime_module_delete) (wasmtime_module_t *m);
  خالی (*wasmtime_store_delete) (wasmtime_store_t *store);
  خالی (*wasmtime_error_پیغام) (ثابت wasmtime_error_t *error, wasm_name_t *پیغام_2);
  خالی (*wasmtime_error_delete) (wasmtime_error_t *error);
  bool (*wasi_config_preopen_dir) (wasi_config_t *config, ثابت حرف *راستہ, ثابت حرف *guest_راستہ);

  wasmtime_wat2wasm = dlsym (cookie, "wasmtime_wat2wasm");
  wasm_engine_new = dlsym (cookie, "wasm_engine_new");
  wasm_engine_delete = dlsym (cookie, "wasm_engine_delete");
  wasm_byte_vec_delete = dlsym (cookie, "wasm_byte_vec_delete");
  wasm_byte_vec_new_uninitialized = dlsym (cookie, "wasm_byte_vec_new_uninitialized");
  wasi_config_new = dlsym (cookie, "wasi_config_new");
  wasi_config_سیٹ_argv = dlsym (cookie, "wasi_config_set_argv");
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
  wasmtime_context_سیٹ_wasi = dlsym (cookie, "wasmtime_context_set_wasi");
  wasmtime_linker_module = dlsym (cookie, "wasmtime_linker_module");
  wasmtime_linker_get_default = dlsym (cookie, "wasmtime_linker_get_default");
  wasmtime_func_call = dlsym (cookie, "wasmtime_func_call");
  wasmtime_module_delete = dlsym (cookie, "wasmtime_module_delete");
  wasmtime_store_delete = dlsym (cookie, "wasmtime_store_delete");
  wasmtime_error_delete = dlsym (cookie, "wasmtime_error_delete");
  wasmtime_error_پیغام = dlsym (cookie, "wasmtime_error_message");
  wasi_config_preopen_dir = dlsym (cookie, "wasi_config_preopen_dir");

  اگر (wasm_engine_new == تہی || wasm_engine_delete == تہی || wasm_byte_vec_delete == تہی
      || wasm_byte_vec_new_uninitialized == تہی || wasi_config_new == تہی || wasmtime_store_new == تہی
      || wasmtime_store_context == تہی || wasmtime_linker_new == تہی || wasmtime_linker_define_wasi == تہی
      || wasmtime_module_new == تہی || wasi_config_inherit_argv == تہی || wasi_config_inherit_stdout == تہی
      || wasi_config_inherit_stdin == تہی || wasi_config_inherit_stderr == تہی
      || wasi_config_inherit_env == تہی || wasmtime_context_سیٹ_wasi == تہی
      || wasmtime_linker_module == تہی || wasmtime_linker_get_default == تہی || wasmtime_func_call == تہی
      || wasmtime_module_delete == تہی || wasmtime_store_delete == تہی || wasi_config_سیٹ_argv == تہی
      || wasmtime_error_delete == تہی || wasmtime_error_پیغام == تہی || wasi_config_preopen_dir == تہی
      || wasmtime_wat2wasm == تہی)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmtime.so`");

  // Set up wasmtime context
  wasm_engine_t *engine = wasm_engine_new ();
  أكد (engine != تہی);
  wasmtime_store_t *store = wasmtime_store_new (engine, تہی, تہی);
  أكد (store != تہی);
  wasmtime_context_t *context = wasmtime_store_context (store);

  // Link with wasi functions defined
  wasmtime_linker_t *linker = wasmtime_linker_new (engine);
  wasmtime_error_t *err = wasmtime_linker_define_wasi (linker);
  اگر (err != تہی)
    {
      wasmtime_error_پیغام (err, &error_پیغام);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to link wasi: %.*s", (صحیح) error_پیغام.سائز, error_پیغام.ڈیٹا);
    }

  wasm_byte_vec_t wasm;
  // Load and parse container entrypoint
  FILE *file = fopen (pathname, "rbe");
  اگر (! file)
    error (EXIT_FAILURE, 0, "error loading entrypoint");
  fseek (file, 0L, SEEK_END);
  size_t file_سائز = ftell (file);
  wasm_byte_vec_new_uninitialized (&wasm, file_سائز);
  fseek (file, 0L, SEEK_SET);
  اگر (fread (wasm.ڈیٹا, file_سائز, 1, file) != 1)
    error (EXIT_FAILURE, 0, "error load");
  fclose (file);

  // If entrypoint contains a webassembly text format
  // compile it on the fly and convert to equivalent
  // binary format.
  اگر (has_suffix (pathname, "wat") > 0)
    {
      wasmtime_error_t *err = wasmtime_wat2wasm ((حرف *) &wasm_بائٹیں, file_سائز, &wasm);
      اگر (err != تہی)
        {
          wasmtime_error_پیغام (err, &error_پیغام);
          wasmtime_error_delete (err);
          error (EXIT_FAILURE, 0, "failed while compiling wat to wasm binary : %.*s", (صحیح) error_پیغام.سائز, error_پیغام.ڈیٹا);
        }
      wasm = wasm_بائٹیں;
    }

  // Compile wasm modules
  wasmtime_module_t *module = تہی;
  err = wasmtime_module_new (engine, (uint8_t *) wasm.ڈیٹا, wasm.سائز, &module);
  اگر (! module)
    {
      wasmtime_error_پیغام (err, &error_پیغام);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to compile module: %.*s", (صحیح) error_پیغام.سائز, error_پیغام.ڈیٹا);
    }
  wasm_byte_vec_delete (&wasm);

  // Init WASI program
  wasi_config_t *wasi_config = wasi_config_new ("crun_wasi_program");
  أكد (wasi_config);

  // Calculate argc for `wasi_config_set_argv`
  برائے (arg = argv; *arg != تہی; ++arg)
    دلائل_سائز++;

  wasi_config_سیٹ_argv (wasi_config, دلائل_سائز, (ثابت حرف **) argv);
  wasi_config_inherit_env (wasi_config);
  wasi_config_inherit_stdin (wasi_config);
  wasi_config_inherit_stdout (wasi_config);
  wasi_config_inherit_stderr (wasi_config);
  wasi_config_preopen_dir (wasi_config, ".", ".");
  wasm_trap_t *trap = تہی;
  err = wasmtime_context_سیٹ_wasi (context, wasi_config);
  اگر (err != تہی)
    {
      wasmtime_error_پیغام (err, &error_پیغام);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate WASI: %.*s", (صحیح) error_پیغام.سائز, error_پیغام.ڈیٹا);
    }

  // Init module
  err = wasmtime_linker_module (linker, context, "", 0, module);
  اگر (err != تہی)
    {
      wasmtime_error_پیغام (err, &error_پیغام);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate module: %.*s", (صحیح) error_پیغام.سائز, error_پیغام.ڈیٹا);
    }

  // Actually run our .wasm
  wasmtime_func_t func;
  err = wasmtime_linker_get_default (linker, context, "", 0, &func);
  اگر (err != تہی)
    {
      wasmtime_error_پیغام (err, &error_پیغام);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to locate default export for module %.*s", (صحیح) error_پیغام.سائز, error_پیغام.ڈیٹا);
    }

  err = wasmtime_func_call (context, &func, تہی, 0, تہی, 0, &trap);
  اگر (err != تہی || trap != تہی)
    {
      wasmtime_error_پیغام (err, &error_پیغام);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "error calling default export: %.*s", (صحیح) error_پیغام.سائز, error_پیغام.ڈیٹا);
    }

  // Clean everything
  wasmtime_module_delete (module);
  wasmtime_store_delete (store);
  wasm_engine_delete (engine);

  باہر_نکلو (EXIT_SUCCESS);
}

ساکن صحیح
libwasmtime_بھرو (خالی **cookie, libcrun_error_t *err)
{
  خالی *handle;

  handle = dlopen ("libwasmtime.so", RTLD_NOW);
  اگر (handle == تہی)
    واپس crun_make_error (err, 0, "could not load `libwasmtime.so`: %s", dlerror ());
  *cookie = handle;

  واپس 0;
}

ساکن صحیح
libwasmtime_unload (خالی *cookie, libcrun_error_t *err)
{
  صحیح r;

  اگر (cookie)
    {
      r = dlclose (cookie);
      اگر (UNLIKELY (r < 0))
        واپس crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  واپس 0;
}

ساکن صحیح
libwasmtime_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  واپس wasm_can_handle_container (container, err);
}

ساخت custom_handler_s handler_wasmtime = {
  .name = "wasmtime",
  .alias = "wasm",
  .feature_string = "WASM:wasmtime",
  .supports_open_درخت_namespace = false,
  .بھرو = libwasmtime_بھرو,
  .unload = libwasmtime_unload,
  .تسلسل_func = libwasmtime_exec,
  .can_handle_container = libwasmtime_can_handle_container,
};

#اگر_ختم
