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
#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश "../custom-handler.h"
#समावेश "../container.h"
#समावेश "../utils.h"
#समावेश "../linux.h"
#समावेश "handler-utils.h"
#समावेश <unistd.h>
#समावेश <sys/stat.h>
#समावेश <errno.h>
#समावेश <sys/types.h>
#समावेश <fcntl.h>
#समावेश <sched.h>
#समावेश <assert.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>

#अगर_घोषित HAVE_DLOPEN
#  include <dlfcn.h>
#पूर्ण_अगर

#अगर_घोषित HAVE_WASMTIME
#  include <wasm.h>
#  include <wasi.h>
#  include <wasmtime.h>
#पूर्ण_अगर

#अगर HAVE_DLOPEN && HAVE_WASMTIME
अटल पूर्णांक
libwasmtime_exec (व्योम *cookie, libcrun_container_t *container arg_unused,
                  स्थिर अक्षर *pathname, अक्षर *स्थिर argv[])
{
  माप_प्रकार args_आकार = 0;
  अक्षर *स्थिर *arg;
  wasm_byte_vec_t error_message;
  wasm_byte_vec_t wasm_bytes;
  wasm_engine_t *(*wasm_engine_new) ();
  wasmtime_error_t *(*wasmtime_wat2wasm) (स्थिर अक्षर *wat, माप_प्रकार wat_len, wasm_byte_vec_t *out);
  व्योम (*wasm_engine_delete) (wasm_engine_t *);
  व्योम (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  व्योम (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, माप_प्रकार);
  wasi_config_t *(*wasi_config_new) (स्थिर अक्षर *);
  wasmtime_store_t *(*wasmtime_store_new) (wasm_engine_t *engine, व्योम *तथ्य, व्योम (*finalizer) (व्योम *));
  wasmtime_context_t *(*wasmtime_store_context) (wasmtime_store_t *store);
  wasmtime_linker_t *(*wasmtime_linker_new) (wasm_engine_t *engine);
  wasmtime_error_t *(*wasmtime_linker_define_wasi) (wasmtime_linker_t *linker);
  wasmtime_error_t *(*wasmtime_module_new) (
      wasm_engine_t *engine,
      स्थिर uint8_t *wasm,
      माप_प्रकार wasm_len,
      wasmtime_module_t **ret);
  व्योम (*wasi_config_inherit_argv) (wasi_config_t *config);
  व्योम (*wasi_config_inherit_env) (wasi_config_t *config);
  व्योम (*wasi_config_set_argv) (wasi_config_t *config, पूर्णांक argc, स्थिर अक्षर *argv[]);
  व्योम (*wasi_config_inherit_stdin) (wasi_config_t *config);
  व्योम (*wasi_config_inherit_stdout) (wasi_config_t *config);
  व्योम (*wasi_config_inherit_stderr) (wasi_config_t *config);
  wasmtime_error_t *(*wasmtime_context_set_wasi) (wasmtime_context_t *context, wasi_config_t *wasi);
  wasmtime_error_t *(*wasmtime_linker_module) (
      wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      स्थिर अक्षर *name,
      माप_प्रकार name_len,
      स्थिर wasmtime_module_t *module);
  wasmtime_error_t *(*wasmtime_linker_get_default) (
      स्थिर wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      स्थिर अक्षर *name,
      माप_प्रकार name_len,
      wasmtime_func_t *func);
  wasmtime_error_t *(*wasmtime_func_call) (
      wasmtime_context_t *store,
      स्थिर wasmtime_func_t *func,
      स्थिर wasmtime_val_t *args,
      माप_प्रकार nargs,
      wasmtime_val_t *results,
      माप_प्रकार nresults,
      wasm_trap_t **trap);
  व्योम (*wasmtime_module_delete) (wasmtime_module_t *m);
  व्योम (*wasmtime_store_delete) (wasmtime_store_t *store);
  व्योम (*wasmtime_error_message) (स्थिर wasmtime_error_t *error, wasm_name_t *message);
  व्योम (*wasmtime_error_delete) (wasmtime_error_t *error);
  bool (*wasi_config_preopen_dir) (wasi_config_t *config, स्थिर अक्षर *path, स्थिर अक्षर *guest_path);

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

  अगर (wasm_engine_new == शून्य || wasm_engine_delete == शून्य || wasm_byte_vec_delete == शून्य
      || wasm_byte_vec_new_uninitialized == शून्य || wasi_config_new == शून्य || wasmtime_store_new == शून्य
      || wasmtime_store_context == शून्य || wasmtime_linker_new == शून्य || wasmtime_linker_define_wasi == शून्य
      || wasmtime_module_new == शून्य || wasi_config_inherit_argv == शून्य || wasi_config_inherit_stdout == शून्य
      || wasi_config_inherit_stdin == शून्य || wasi_config_inherit_stderr == शून्य
      || wasi_config_inherit_env == शून्य || wasmtime_context_set_wasi == शून्य
      || wasmtime_linker_module == शून्य || wasmtime_linker_get_default == शून्य || wasmtime_func_call == शून्य
      || wasmtime_module_delete == शून्य || wasmtime_store_delete == शून्य || wasi_config_set_argv == शून्य
      || wasmtime_error_delete == शून्य || wasmtime_error_message == शून्य || wasi_config_preopen_dir == शून्य
      || wasmtime_wat2wasm == शून्य)
    error (निकास_त्रुटि, 0, "could not find symbol in `libwasmtime.so`");

  // Set up wasmtime context
  wasm_engine_t *engine = wasm_engine_new ();
  निश्चित (engine != शून्य);
  wasmtime_store_t *store = wasmtime_store_new (engine, शून्य, शून्य);
  निश्चित (store != शून्य);
  wasmtime_context_t *context = wasmtime_store_context (store);

  // Link with wasi functions defined
  wasmtime_linker_t *linker = wasmtime_linker_new (engine);
  wasmtime_error_t *err = wasmtime_linker_define_wasi (linker);
  अगर (err != शून्य)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (निकास_त्रुटि, 0, "failed to link wasi: %.*s", (पूर्णांक) error_message.आकार, error_message.तथ्य);
    }

  wasm_byte_vec_t wasm;
  // Load and parse container entrypoint
  खाता *file = ख_खोलो (pathname, "rbe");
  अगर (! file)
    error (निकास_त्रुटि, 0, "error loading entrypoint");
  ख_जाओ (file, 0L, अंत_से);
  माप_प्रकार file_आकार = ख_बताओ (file);
  wasm_byte_vec_new_uninitialized (&wasm, file_आकार);
  ख_जाओ (file, 0L, शुरू_से);
  अगर (ख_पढ़ो (wasm.तथ्य, file_आकार, 1, file) != 1)
    error (निकास_त्रुटि, 0, "error load");
  ख_बंद (file);

  // If entrypoint contains a webassembly text format
  // compile it on the fly and convert to equivalent
  // binary format.
  अगर (has_suffix (pathname, "wat") > 0)
    {
      wasmtime_error_t *err = wasmtime_wat2wasm ((अक्षर *) &wasm_bytes, file_आकार, &wasm);
      अगर (err != शून्य)
        {
          wasmtime_error_message (err, &error_message);
          wasmtime_error_delete (err);
          error (निकास_त्रुटि, 0, "failed while compiling wat to wasm binary : %.*s", (पूर्णांक) error_message.आकार, error_message.तथ्य);
        }
      wasm = wasm_bytes;
    }

  // Compile wasm modules
  wasmtime_module_t *module = शून्य;
  err = wasmtime_module_new (engine, (uint8_t *) wasm.तथ्य, wasm.आकार, &module);
  अगर (! module)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (निकास_त्रुटि, 0, "failed to compile module: %.*s", (पूर्णांक) error_message.आकार, error_message.तथ्य);
    }
  wasm_byte_vec_delete (&wasm);

  // Init WASI program
  wasi_config_t *wasi_config = wasi_config_new ("crun_wasi_program");
  निश्चित (wasi_config);

  // Calculate argc for `wasi_config_set_argv`
  क्रम (arg = argv; *arg != शून्य; ++arg)
    args_आकार++;

  wasi_config_set_argv (wasi_config, args_आकार, (स्थिर अक्षर **) argv);
  wasi_config_inherit_env (wasi_config);
  wasi_config_inherit_stdin (wasi_config);
  wasi_config_inherit_stdout (wasi_config);
  wasi_config_inherit_stderr (wasi_config);
  wasi_config_preopen_dir (wasi_config, ".", ".");
  wasm_trap_t *trap = शून्य;
  err = wasmtime_context_set_wasi (context, wasi_config);
  अगर (err != शून्य)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (निकास_त्रुटि, 0, "failed to instantiate WASI: %.*s", (पूर्णांक) error_message.आकार, error_message.तथ्य);
    }

  // Init module
  err = wasmtime_linker_module (linker, context, "", 0, module);
  अगर (err != शून्य)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (निकास_त्रुटि, 0, "failed to instantiate module: %.*s", (पूर्णांक) error_message.आकार, error_message.तथ्य);
    }

  // Actually run our .wasm
  wasmtime_func_t func;
  err = wasmtime_linker_get_default (linker, context, "", 0, &func);
  अगर (err != शून्य)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (निकास_त्रुटि, 0, "failed to locate default export for module %.*s", (पूर्णांक) error_message.आकार, error_message.तथ्य);
    }

  err = wasmtime_func_call (context, &func, शून्य, 0, शून्य, 0, &trap);
  अगर (err != शून्य || trap != शून्य)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (निकास_त्रुटि, 0, "error calling default export: %.*s", (पूर्णांक) error_message.आकार, error_message.तथ्य);
    }

  // Clean everything
  wasmtime_module_delete (module);
  wasmtime_store_delete (store);
  wasm_engine_delete (engine);

  निकास (निकास_सफल);
}

अटल पूर्णांक
libwasmtime_load (व्योम **cookie, libcrun_error_t *err)
{
  व्योम *handle;

  handle = dlopen ("libwasmtime.so", RTLD_NOW);
  अगर (handle == शून्य)
    वापस crun_make_error (err, 0, "could not load `libwasmtime.so`: %s", dlerror ());
  *cookie = handle;

  वापस 0;
}

अटल पूर्णांक
libwasmtime_unload (व्योम *cookie, libcrun_error_t *err)
{
  पूर्णांक r;

  अगर (cookie)
    {
      r = dlclose (cookie);
      अगर (UNLIKELY (r < 0))
        वापस crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  वापस 0;
}

अटल पूर्णांक
libwasmtime_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  वापस wasm_can_handle_container (container, err);
}

काष्ठा custom_handler_s handler_wasmtime = {
  .name = "wasmtime",
  .alias = "wasm",
  .feature_string = "WASM:wasmtime",
  .supports_open_वृक्ष_namespace = false,
  .load = libwasmtime_load,
  .unload = libwasmtime_unload,
  .run_func = libwasmtime_exec,
  .can_handle_container = libwasmtime_can_handle_container,
};

#पूर्ण_अगर
