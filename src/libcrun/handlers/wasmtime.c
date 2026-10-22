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

#अन्तर्भाव <config.h>
#अन्तर्भाव "../custom-handler.h"
#अन्तर्भाव "../container.h"
#अन्तर्भाव "../utils.h"
#अन्तर्भाव "../linux.h"
#अन्तर्भाव "handler-utils.h"
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <sched.h>
#अन्तर्भाव <assert.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_WASMTIME
#  include <wasm.h>
#  include <wasi.h>
#  include <wasmtime.h>
#endif

#यदि HAVE_DLOPEN && HAVE_WASMTIME
स्थिर पूर्णाङ्क
libwasmtime_exec (शून्य *cookie, libcrun_container_t *container arg_unused,
                  ध्रुव वर्ण *pathname, वर्ण *ध्रुव argv[])
{
  size_t args_size = 0;
  वर्ण *ध्रुव *arg;
  wasm_byte_vec_t error_message;
  wasm_byte_vec_t wasm_bytes;
  wasm_engine_t *(*wasm_engine_new) ();
  wasmtime_error_t *(*wasmtime_wat2wasm) (ध्रुव वर्ण *wat, size_t wat_len, wasm_byte_vec_t *out);
  शून्य (*wasm_engine_delete) (wasm_engine_t *);
  शून्य (*wasm_byte_vec_delete) (wasm_byte_vec_t *);
  शून्य (*wasm_byte_vec_new_uninitialized) (wasm_byte_vec_t *, size_t);
  wasi_config_t *(*wasi_config_new) (ध्रुव वर्ण *);
  wasmtime_store_t *(*wasmtime_store_new) (wasm_engine_t *engine, शून्य *data, शून्य (*finalizer) (शून्य *));
  wasmtime_context_t *(*wasmtime_store_context) (wasmtime_store_t *store);
  wasmtime_linker_t *(*wasmtime_linker_new) (wasm_engine_t *engine);
  wasmtime_error_t *(*wasmtime_linker_define_wasi) (wasmtime_linker_t *linker);
  wasmtime_error_t *(*wasmtime_module_new) (
      wasm_engine_t *engine,
      ध्रुव uint8_t *wasm,
      size_t wasm_len,
      wasmtime_module_t **ret);
  शून्य (*wasi_config_inherit_argv) (wasi_config_t *config);
  शून्य (*wasi_config_inherit_env) (wasi_config_t *config);
  शून्य (*wasi_config_set_argv) (wasi_config_t *config, पूर्णाङ्क argc, ध्रुव वर्ण *argv[]);
  शून्य (*wasi_config_inherit_stdin) (wasi_config_t *config);
  शून्य (*wasi_config_inherit_stdout) (wasi_config_t *config);
  शून्य (*wasi_config_inherit_stderr) (wasi_config_t *config);
  wasmtime_error_t *(*wasmtime_context_set_wasi) (wasmtime_context_t *context, wasi_config_t *wasi);
  wasmtime_error_t *(*wasmtime_linker_module) (
      wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      ध्रुव वर्ण *name,
      size_t name_len,
      ध्रुव wasmtime_module_t *module);
  wasmtime_error_t *(*wasmtime_linker_get_default) (
      ध्रुव wasmtime_linker_t *linker,
      wasmtime_context_t *store,
      ध्रुव वर्ण *name,
      size_t name_len,
      wasmtime_func_t *func);
  wasmtime_error_t *(*wasmtime_func_call) (
      wasmtime_context_t *store,
      ध्रुव wasmtime_func_t *func,
      ध्रुव wasmtime_val_t *args,
      size_t nargs,
      wasmtime_val_t *results,
      size_t nresults,
      wasm_trap_t **trap);
  शून्य (*wasmtime_module_delete) (wasmtime_module_t *m);
  शून्य (*wasmtime_store_delete) (wasmtime_store_t *store);
  शून्य (*wasmtime_error_message) (ध्रुव wasmtime_error_t *error, wasm_name_t *message);
  शून्य (*wasmtime_error_delete) (wasmtime_error_t *error);
  bool (*wasi_config_preopen_dir) (wasi_config_t *config, ध्रुव वर्ण *path, ध्रुव वर्ण *guest_path);

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

  यदि (wasm_engine_new == रिक्त || wasm_engine_delete == रिक्त || wasm_byte_vec_delete == रिक्त
      || wasm_byte_vec_new_uninitialized == रिक्त || wasi_config_new == रिक्त || wasmtime_store_new == रिक्त
      || wasmtime_store_context == रिक्त || wasmtime_linker_new == रिक्त || wasmtime_linker_define_wasi == रिक्त
      || wasmtime_module_new == रिक्त || wasi_config_inherit_argv == रिक्त || wasi_config_inherit_stdout == रिक्त
      || wasi_config_inherit_stdin == रिक्त || wasi_config_inherit_stderr == रिक्त
      || wasi_config_inherit_env == रिक्त || wasmtime_context_set_wasi == रिक्त
      || wasmtime_linker_module == रिक्त || wasmtime_linker_get_default == रिक्त || wasmtime_func_call == रिक्त
      || wasmtime_module_delete == रिक्त || wasmtime_store_delete == रिक्त || wasi_config_set_argv == रिक्त
      || wasmtime_error_delete == रिक्त || wasmtime_error_message == रिक्त || wasi_config_preopen_dir == रिक्त
      || wasmtime_wat2wasm == रिक्त)
    error (EXIT_FAILURE, 0, "could not find symbol in `libwasmtime.so`");

  // Set up wasmtime context
  wasm_engine_t *engine = wasm_engine_new ();
  प्रतिज्ञा (engine != रिक्त);
  wasmtime_store_t *store = wasmtime_store_new (engine, रिक्त, रिक्त);
  प्रतिज्ञा (store != रिक्त);
  wasmtime_context_t *context = wasmtime_store_context (store);

  // Link with wasi functions defined
  wasmtime_linker_t *linker = wasmtime_linker_new (engine);
  wasmtime_error_t *err = wasmtime_linker_define_wasi (linker);
  यदि (err != रिक्त)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to link wasi: %.*s", (पूर्णाङ्क) error_message.size, error_message.data);
    }

  wasm_byte_vec_t wasm;
  // Load and parse container entrypoint
  FILE *file = fopen (pathname, "rbe");
  यदि (! file)
    error (EXIT_FAILURE, 0, "error loading entrypoint");
  fseek (file, 0L, SEEK_END);
  size_t file_size = ftell (file);
  wasm_byte_vec_new_uninitialized (&wasm, file_size);
  fseek (file, 0L, SEEK_SET);
  यदि (fread (wasm.data, file_size, 1, file) != 1)
    error (EXIT_FAILURE, 0, "error load");
  fclose (file);

  // If entrypoint contains a webassembly text format
  // compile it on the fly and convert to equivalent
  // binary format.
  यदि (has_suffix (pathname, "wat") > 0)
    {
      wasmtime_error_t *err = wasmtime_wat2wasm ((वर्ण *) &wasm_bytes, file_size, &wasm);
      यदि (err != रिक्त)
        {
          wasmtime_error_message (err, &error_message);
          wasmtime_error_delete (err);
          error (EXIT_FAILURE, 0, "failed while compiling wat to wasm binary : %.*s", (पूर्णाङ्क) error_message.size, error_message.data);
        }
      wasm = wasm_bytes;
    }

  // Compile wasm modules
  wasmtime_module_t *module = रिक्त;
  err = wasmtime_module_new (engine, (uint8_t *) wasm.data, wasm.size, &module);
  यदि (! module)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to compile module: %.*s", (पूर्णाङ्क) error_message.size, error_message.data);
    }
  wasm_byte_vec_delete (&wasm);

  // Init WASI program
  wasi_config_t *wasi_config = wasi_config_new ("crun_wasi_program");
  प्रतिज्ञा (wasi_config);

  // Calculate argc for `wasi_config_set_argv`
  प्रति (arg = argv; *arg != रिक्त; ++arg)
    args_size++;

  wasi_config_set_argv (wasi_config, args_size, (ध्रुव वर्ण **) argv);
  wasi_config_inherit_env (wasi_config);
  wasi_config_inherit_stdin (wasi_config);
  wasi_config_inherit_stdout (wasi_config);
  wasi_config_inherit_stderr (wasi_config);
  wasi_config_preopen_dir (wasi_config, ".", ".");
  wasm_trap_t *trap = रिक्त;
  err = wasmtime_context_set_wasi (context, wasi_config);
  यदि (err != रिक्त)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate WASI: %.*s", (पूर्णाङ्क) error_message.size, error_message.data);
    }

  // Init module
  err = wasmtime_linker_module (linker, context, "", 0, module);
  यदि (err != रिक्त)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to instantiate module: %.*s", (पूर्णाङ्क) error_message.size, error_message.data);
    }

  // Actually run our .wasm
  wasmtime_func_t func;
  err = wasmtime_linker_get_default (linker, context, "", 0, &func);
  यदि (err != रिक्त)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "failed to locate default export for module %.*s", (पूर्णाङ्क) error_message.size, error_message.data);
    }

  err = wasmtime_func_call (context, &func, रिक्त, 0, रिक्त, 0, &trap);
  यदि (err != रिक्त || trap != रिक्त)
    {
      wasmtime_error_message (err, &error_message);
      wasmtime_error_delete (err);
      error (EXIT_FAILURE, 0, "error calling default export: %.*s", (पूर्णाङ्क) error_message.size, error_message.data);
    }

  // Clean everything
  wasmtime_module_delete (module);
  wasmtime_store_delete (store);
  wasm_engine_delete (engine);

  exit (EXIT_SUCCESS);
}

स्थिर पूर्णाङ्क
libwasmtime_load (शून्य **cookie, libcrun_error_t *err)
{
  शून्य *handle;

  handle = dlopen ("libwasmtime.so", RTLD_NOW);
  यदि (handle == रिक्त)
    प्रत्यागम crun_make_error (err, 0, "could not load `libwasmtime.so`: %s", dlerror ());
  *cookie = handle;

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
libwasmtime_unload (शून्य *cookie, libcrun_error_t *err)
{
  पूर्णाङ्क r;

  यदि (cookie)
    {
      r = dlclose (cookie);
      यदि (UNLIKELY (r < 0))
        प्रत्यागम crun_make_error (err, 0, "could not unload handle: %s", dlerror ());
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
libwasmtime_can_handle_container (libcrun_container_t *container, libcrun_error_t *err)
{
  प्रत्यागम wasm_can_handle_container (container, err);
}

संरचना custom_handler_s handler_wasmtime = {
  .name = "wasmtime",
  .alias = "wasm",
  .feature_string = "WASM:wasmtime",
  .supports_open_वृक्ष_namespace = false,
  .load = libwasmtime_load,
  .unload = libwasmtime_unload,
  .run_func = libwasmtime_exec,
  .can_handle_container = libwasmtime_can_handle_container,
};

#endif
