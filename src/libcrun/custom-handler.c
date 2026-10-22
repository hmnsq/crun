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
#அந்தர்பாவ "custom-handler.h"
#அந்தர்பாவ "container.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "linux.h"
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#எனில் HAVE_DLOPEN && HAVE_LIBKRUN
வெளி கட்டமைப்பு custom_handler_s handler_libkrun;
#endif
#எனில் HAVE_DLOPEN && HAVE_WASMTIME
வெளி கட்டமைப்பு custom_handler_s handler_wasmtime;
#endif
#எனில் HAVE_DLOPEN && HAVE_WASMEDGE
வெளி கட்டமைப்பு custom_handler_s handler_wasmedge;
#endif
#எனில் HAVE_DLOPEN && HAVE_WASMER
வெளி கட்டமைப்பு custom_handler_s handler_wasmer;
#endif
#எனில் HAVE_DLOPEN && HAVE_WAMR
வெளி கட்டமைப்பு custom_handler_s handler_wamr;
#endif
#எனில் HAVE_DLOPEN && HAVE_MONO
வெளி கட்டமைப்பு custom_handler_s handler_mono;
#endif
#எனில் HAVE_DLOPEN && HAVE_SPIN
வெளி கட்டமைப்பு custom_handler_s handler_spin;
#endif

நிலையான கட்டமைப்பு custom_handler_s *static_handlers[] = {
#எனில் HAVE_DLOPEN && HAVE_LIBKRUN
  &handler_libkrun,
#endif
#எனில் HAVE_DLOPEN && HAVE_WASMEDGE
  &handler_wasmedge,
#endif
#எனில் HAVE_DLOPEN && HAVE_WASMER
  &handler_wasmer,
#endif
#எனில் HAVE_DLOPEN && HAVE_WASMTIME
  &handler_wasmtime,
#endif
#எனில் HAVE_DLOPEN && HAVE_WAMR
  &handler_wamr,
#endif
#எனில் HAVE_DLOPEN && HAVE_MONO
  &handler_mono,
#endif
#எனில் HAVE_DLOPEN && HAVE_SPIN
  &handler_spin,
#endif
  வெற்று,
};

கட்டமைப்பு custom_handler_manager_s
{
  கட்டமைப்பு custom_handler_s **handlers;
  வெறுமை **handles;
  size_t handlers_len;
};

கட்டமைப்பு custom_handler_manager_s *
libcrun_handler_manager_create (libcrun_error_t *err arg_unused)
{
  கட்டமைப்பு custom_handler_s **handlers = வெற்று;
  வெறுமை **handles = வெற்று;
  கட்டமைப்பு custom_handler_manager_s *m;
  size_t i, handlers_len;

  /* Count the static handlers.  */
  க்கு (handlers_len = 0; static_handlers[handlers_len]; handlers_len++)
    ;

  எனில் (handlers_len)
    {
      handlers = xmalloc (அளவு (கட்டமைப்பு custom_handler_s *) * handlers_len);
      handles = xmalloc0 (அளவு (வெறுமை *) * handlers_len);
    }

  க்கு (i = 0; i < handlers_len; i++)
    handlers[i] = static_handlers[i];

  m = xmalloc0 (அளவு (கட்டமைப்பு custom_handler_manager_s));
  m->handlers = handlers;
  m->handles = handles;
  m->handlers_len = handlers_len;

  திரும்பு m;
}

வெறுமை
handler_manager_free (கட்டமைப்பு custom_handler_manager_s *manager)
{
  size_t i;

  எனில் (manager == வெற்று)
    திரும்பு;

  க்கு (i = 0; i < manager->handlers_len; i++)
    {
#ifdef HAVE_DLOPEN
      எனில் (manager->handles[i])
        dlclose (manager->handles[i]);
#endif
    }
  விடுவி (manager->handlers);
  விடுவி (manager->handles);
  விடுவி (manager);
}

#ifdef HAVE_DLOPEN
நிலையான முழு
handler_manager_add_so (கட்டமைப்பு custom_handler_manager_s *manager, வெறுமை *handle, libcrun_error_t *err)
{
  கட்டமைப்பு custom_handler_s *h = வெற்று;
  run_oci_get_handler_cb cb;

  cb = (run_oci_get_handler_cb) dlsym (handle, "run_oci_handler_get_handler");
  எனில் (UNLIKELY (cb == வெற்று))
    திரும்பு crun_make_error (err, 0, "cannot find symbol `run_oci_handler_get_handler`");

  h = cb ();
  எனில் (UNLIKELY (h == வெற்று))
    திரும்பு crun_make_error (err, 0, "the callback `run_oci_handler_get_handler` didn't return a handler");

  manager->handlers = xrealloc (manager->handlers, அளவு (கட்டமைப்பு custom_handler_s *) * (manager->handlers_len + 1));
  manager->handles = xrealloc (manager->handles, அளவு (வெறுமை *) * (manager->handlers_len + 1));

  manager->handlers[manager->handlers_len] = h;
  manager->handles[manager->handlers_len] = handle;
  manager->handlers_len++;
  திரும்பு 0;
}
#endif

முழு
libcrun_handler_manager_load_directory (கட்டமைப்பு custom_handler_manager_s *manager, மாறிலி எழுத்து *path, libcrun_error_t *err)
{
#ifdef HAVE_DLOPEN
  cleanup_dir DIR *dir = வெற்று;
  கட்டமைப்பு dirent *next;

  dir = opendir (path);
  எனில் (UNLIKELY (dir == வெற்று))
    திரும்பு crun_make_error (err, errno, "cannot opendir `%s`", path);

  க்கு (next = readdir (dir); next; next = readdir (dir))
    {
      cleanup_free எழுத்து *fpath = வெற்று;
      மாறிலி எழுத்து *name;
      வெறுமை *handle;
      முழு ret;

      name = next->d_name;
      எனில் (name[0] == '.')
        தொடர்;

      ret = append_paths (&fpath, err, path, name, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      handle = dlopen (fpath, RTLD_NOW);
      எனில் (UNLIKELY (handle == வெற்று))
        திரும்பு crun_make_error (err, 0, "cannot load `%s`: `%s`", fpath, dlerror ());

      ret = handler_manager_add_so (manager, handle, err);
      எனில் (UNLIKELY (ret < 0))
        {
          dlclose (handle);
          திரும்பு ret;
        }
    }
  திரும்பு 0;
#இல்லையெனில்
  (வெறுமை) manager;
  (வெறுமை) path;
  திரும்பு crun_make_error (err, ENOTSUP, "dlopen not available");
#endif
}

கட்டமைப்பு custom_handler_s *
handler_by_name (கட்டமைப்பு custom_handler_manager_s *manager, மாறிலி எழுத்து *name)
{
  size_t i;

  க்கு (i = 0; i < manager->handlers_len; i++)
    {
      எனில் (strcmp (manager->handlers[i]->name, name) == 0)
        திரும்பு manager->handlers[i];
      எனில் (manager->handlers[i]->alias && strcmp (manager->handlers[i]->alias, name) == 0)
        திரும்பு manager->handlers[i];
    }
  திரும்பு வெற்று;
}

வெறுமை
libcrun_handler_manager_print_feature_tags (கட்டமைப்பு custom_handler_manager_s *manager, FILE *out)
{
  size_t i;

  க்கு (i = 0; i < manager->handlers_len; i++)
    எனில் (manager->handlers[i]->feature_string)
      fprintf (out, "+%s ", manager->handlers[i]->feature_string);
}

எழுத்து **
libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err arg_unused)
{
  கட்டமைப்பு custom_handler_manager_s *manager;
  எழுத்து **out;
  size_t i, n = 0;

  எனில் (ctx == வெற்று || ctx->handler_manager == வெற்று)
    திரும்பு வெற்று;

  manager = ctx->handler_manager;

  க்கு (i = 0; i < manager->handlers_len; i++)
    எனில் (manager->handlers[i]->feature_string)
      n++;

  எனில் (n == 0)
    திரும்பு வெற்று;

  out = xmalloc0 ((n + 1) * அளவு (எழுத்து *));
  n = 0;
  க்கு (i = 0; i < manager->handlers_len; i++)
    எனில் (manager->handlers[i]->feature_string)
      out[n++] = xstrdup (manager->handlers[i]->feature_string);

  திரும்பு out;
}

நிலையான உள்வரி கட்டமைப்பு custom_handler_instance_s *
make_custom_handler_instance_s (கட்டமைப்பு custom_handler_s *vtable)
{
  கட்டமைப்பு custom_handler_instance_s *ret = xmalloc0 (அளவு (கட்டமைப்பு custom_handler_instance_s));

  ret->vtable = vtable;
  ret->cookie = வெற்று;

  திரும்பு ret;
}

நிலையான முழு
find_handler_for_container (கட்டமைப்பு custom_handler_manager_s *manager,
                            libcrun_container_t *container,
                            கட்டமைப்பு custom_handler_instance_s **out,
                            libcrun_error_t *err)
{
  size_t i;

  memset (out, 0, அளவு (*out));

  க்கு (i = 0; i < manager->handlers_len; i++)
    {
      முழு ret;

      எனில் (manager->handlers[i]->can_handle_container == வெற்று)
        தொடர்;

      ret = manager->handlers[i]->can_handle_container (container, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      எனில் (ret)
        {
          *out = make_custom_handler_instance_s (manager->handlers[i]);
          எனில் ((*out)->vtable->load)
            திரும்பு (*out)->vtable->load (&((*out)->cookie), err);

          திரும்பு 0;
        }
    }

  திரும்பு 0;
}

முழு
libcrun_configure_handler (கட்டமைப்பு custom_handler_manager_s *manager,
                           libcrun_context_t *context,
                           libcrun_container_t *container,
                           கட்டமைப்பு custom_handler_instance_s **out,
                           libcrun_error_t *err)
{
  மாறிலி எழுத்து *explicit_handler;
  மாறிலி எழுத்து *annotation;
  முழு ret;

  *out = வெற்று;

  // Kubernetes sandbox containers must be executed as regular process
  // Example sandbox container can contain pause process
  // See: https://github.com/containers/crun/issues/798
  // before invoking handler check if this is not a kubernetes sandbox
  annotation = find_annotation (container, "io.kubernetes.cri.container-type");
  எனில் (annotation && (strcmp (annotation, "sandbox") == 0))
    திரும்பு 0;

  annotation = find_annotation (container, "run.oci.handler");

  /* Fail with EACCESS if global handler is already configured and there was an attempt to override it via spec.  */
  எனில் (context->handler != வெற்று && annotation != வெற்று)
    திரும்பு crun_make_error (err, EACCES, "invalid attempt to override already configured global handler: `%s`", context->handler);

  explicit_handler = context->handler ? context->handler : annotation;

  /* If an explicit handler was requested, use it.  */
  எனில் (explicit_handler)
    {
      கட்டமைப்பு custom_handler_s *h;

      எனில் (manager == வெற்று)
        திரும்பு crun_make_error (err, 0, "handler requested but no manager configured: `%s`", explicit_handler);

      h = handler_by_name (manager, explicit_handler);
      எனில் (h)
        {
          *out = make_custom_handler_instance_s (h);
          எனில் ((*out)->vtable->load)
            ret = (*out)->vtable->load (&((*out)->cookie), err);
          இல்லையெனில்
            ret = 0;
          செல் out;
        }
    }

  எனில் (manager == வெற்று)
    திரும்பு 0;

  ret = find_handler_for_container (manager, container, out, err);

out:
  எனில் (ret == 0 && *out != வெற்று && container->context != வெற்று && ! (*out)->vtable->supports_open_மரம்_namespace)
    container->context->no_open_மரம்_namespace = true;

  திரும்பு ret;
}
