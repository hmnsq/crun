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
#شامل "custom-handler.h"
#شامل "container.h"
#شامل "utils.h"
#شامل "linux.h"
#شامل <unistd.h>
#شامل <sys/stat.h>
#شامل <errno.h>
#شامل <sys/types.h>
#شامل <fcntl.h>
#شامل <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#اگر HAVE_DLOPEN && HAVE_LIBKRUN
بیرونی ساختار custom_handler_s handler_libkrun;
#endif
#اگر HAVE_DLOPEN && HAVE_WASMTIME
بیرونی ساختار custom_handler_s handler_wasmtime;
#endif
#اگر HAVE_DLOPEN && HAVE_WASMEDGE
بیرونی ساختار custom_handler_s handler_wasmedge;
#endif
#اگر HAVE_DLOPEN && HAVE_WASMER
بیرونی ساختار custom_handler_s handler_wasmer;
#endif
#اگر HAVE_DLOPEN && HAVE_WAMR
بیرونی ساختار custom_handler_s handler_wamr;
#endif
#اگر HAVE_DLOPEN && HAVE_MONO
بیرونی ساختار custom_handler_s handler_mono;
#endif
#اگر HAVE_DLOPEN && HAVE_SPIN
بیرونی ساختار custom_handler_s handler_spin;
#endif

ایستا ساختار custom_handler_s *static_handlers[] = {
#اگر HAVE_DLOPEN && HAVE_LIBKRUN
  &handler_libkrun,
#endif
#اگر HAVE_DLOPEN && HAVE_WASMEDGE
  &handler_wasmedge,
#endif
#اگر HAVE_DLOPEN && HAVE_WASMER
  &handler_wasmer,
#endif
#اگر HAVE_DLOPEN && HAVE_WASMTIME
  &handler_wasmtime,
#endif
#اگر HAVE_DLOPEN && HAVE_WAMR
  &handler_wamr,
#endif
#اگر HAVE_DLOPEN && HAVE_MONO
  &handler_mono,
#endif
#اگر HAVE_DLOPEN && HAVE_SPIN
  &handler_spin,
#endif
  پوچ,
};

ساختار custom_handler_manager_s
{
  ساختار custom_handler_s **handlers;
  تهی **handles;
  size_t handlers_len;
};

ساختار custom_handler_manager_s *
libcrun_handler_manager_create (libcrun_error_t *err arg_unused)
{
  ساختار custom_handler_s **handlers = پوچ;
  تهی **handles = پوچ;
  ساختار custom_handler_manager_s *m;
  size_t i, handlers_len;

  /* Count the static handlers.  */
  برای (handlers_len = 0; static_handlers[handlers_len]; handlers_len++)
    ;

  اگر (handlers_len)
    {
      handlers = xmalloc (اندازه (ساختار custom_handler_s *) * handlers_len);
      handles = xmalloc0 (اندازه (تهی *) * handlers_len);
    }

  برای (i = 0; i < handlers_len; i++)
    handlers[i] = static_handlers[i];

  m = xmalloc0 (اندازه (ساختار custom_handler_manager_s));
  m->handlers = handlers;
  m->handles = handles;
  m->handlers_len = handlers_len;

  بازگشت m;
}

تهی
handler_manager_free (ساختار custom_handler_manager_s *manager)
{
  size_t i;

  اگر (manager == پوچ)
    بازگشت;

  برای (i = 0; i < manager->handlers_len; i++)
    {
#ifdef HAVE_DLOPEN
      اگر (manager->handles[i])
        dlclose (manager->handles[i]);
#endif
    }
  آزاد (manager->handlers);
  آزاد (manager->handles);
  آزاد (manager);
}

#ifdef HAVE_DLOPEN
ایستا صحیح
handler_manager_add_so (ساختار custom_handler_manager_s *manager, تهی *handle, libcrun_error_t *err)
{
  ساختار custom_handler_s *h = پوچ;
  run_oci_get_handler_cb cb;

  cb = (run_oci_get_handler_cb) dlsym (handle, "run_oci_handler_get_handler");
  اگر (UNLIKELY (cb == پوچ))
    بازگشت crun_make_error (err, 0, "cannot find symbol `run_oci_handler_get_handler`");

  h = cb ();
  اگر (UNLIKELY (h == پوچ))
    بازگشت crun_make_error (err, 0, "the callback `run_oci_handler_get_handler` didn't return a handler");

  manager->handlers = xrealloc (manager->handlers, اندازه (ساختار custom_handler_s *) * (manager->handlers_len + 1));
  manager->handles = xrealloc (manager->handles, اندازه (تهی *) * (manager->handlers_len + 1));

  manager->handlers[manager->handlers_len] = h;
  manager->handles[manager->handlers_len] = handle;
  manager->handlers_len++;
  بازگشت 0;
}
#endif

صحیح
libcrun_handler_manager_load_directory (ساختار custom_handler_manager_s *manager, ثابت نویسه *path, libcrun_error_t *err)
{
#ifdef HAVE_DLOPEN
  cleanup_dir DIR *dir = پوچ;
  ساختار dirent *next;

  dir = opendir (path);
  اگر (UNLIKELY (dir == پوچ))
    بازگشت crun_make_error (err, errno, "cannot opendir `%s`", path);

  برای (next = readdir (dir); next; next = readdir (dir))
    {
      cleanup_free نویسه *fpath = پوچ;
      ثابت نویسه *name;
      تهی *handle;
      صحیح ret;

      name = next->d_name;
      اگر (name[0] == '.')
        ادامه;

      ret = append_paths (&fpath, err, path, name, پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      handle = dlopen (fpath, RTLD_NOW);
      اگر (UNLIKELY (handle == پوچ))
        بازگشت crun_make_error (err, 0, "cannot load `%s`: `%s`", fpath, dlerror ());

      ret = handler_manager_add_so (manager, handle, err);
      اگر (UNLIKELY (ret < 0))
        {
          dlclose (handle);
          بازگشت ret;
        }
    }
  بازگشت 0;
#وگرنه
  (تهی) manager;
  (تهی) path;
  بازگشت crun_make_error (err, ENOTSUP, "dlopen not available");
#endif
}

ساختار custom_handler_s *
handler_by_name (ساختار custom_handler_manager_s *manager, ثابت نویسه *name)
{
  size_t i;

  برای (i = 0; i < manager->handlers_len; i++)
    {
      اگر (strcmp (manager->handlers[i]->name, name) == 0)
        بازگشت manager->handlers[i];
      اگر (manager->handlers[i]->alias && strcmp (manager->handlers[i]->alias, name) == 0)
        بازگشت manager->handlers[i];
    }
  بازگشت پوچ;
}

تهی
libcrun_handler_manager_print_feature_tags (ساختار custom_handler_manager_s *manager, FILE *out)
{
  size_t i;

  برای (i = 0; i < manager->handlers_len; i++)
    اگر (manager->handlers[i]->feature_string)
      fprintf (out, "+%s ", manager->handlers[i]->feature_string);
}

نویسه **
libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err arg_unused)
{
  ساختار custom_handler_manager_s *manager;
  نویسه **out;
  size_t i, n = 0;

  اگر (ctx == پوچ || ctx->handler_manager == پوچ)
    بازگشت پوچ;

  manager = ctx->handler_manager;

  برای (i = 0; i < manager->handlers_len; i++)
    اگر (manager->handlers[i]->feature_string)
      n++;

  اگر (n == 0)
    بازگشت پوچ;

  out = xmalloc0 ((n + 1) * اندازه (نویسه *));
  n = 0;
  برای (i = 0; i < manager->handlers_len; i++)
    اگر (manager->handlers[i]->feature_string)
      out[n++] = xstrdup (manager->handlers[i]->feature_string);

  بازگشت out;
}

ایستا درونخطی ساختار custom_handler_instance_s *
make_custom_handler_instance_s (ساختار custom_handler_s *vtable)
{
  ساختار custom_handler_instance_s *ret = xmalloc0 (اندازه (ساختار custom_handler_instance_s));

  ret->vtable = vtable;
  ret->cookie = پوچ;

  بازگشت ret;
}

ایستا صحیح
find_handler_for_container (ساختار custom_handler_manager_s *manager,
                            libcrun_container_t *container,
                            ساختار custom_handler_instance_s **out,
                            libcrun_error_t *err)
{
  size_t i;

  memset (out, 0, اندازه (*out));

  برای (i = 0; i < manager->handlers_len; i++)
    {
      صحیح ret;

      اگر (manager->handlers[i]->can_handle_container == پوچ)
        ادامه;

      ret = manager->handlers[i]->can_handle_container (container, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      اگر (ret)
        {
          *out = make_custom_handler_instance_s (manager->handlers[i]);
          اگر ((*out)->vtable->load)
            بازگشت (*out)->vtable->load (&((*out)->cookie), err);

          بازگشت 0;
        }
    }

  بازگشت 0;
}

صحیح
libcrun_configure_handler (ساختار custom_handler_manager_s *manager,
                           libcrun_context_t *context,
                           libcrun_container_t *container,
                           ساختار custom_handler_instance_s **out,
                           libcrun_error_t *err)
{
  ثابت نویسه *explicit_handler;
  ثابت نویسه *annotation;
  صحیح ret;

  *out = پوچ;

  // Kubernetes sandbox containers must be executed as regular process
  // Example sandbox container can contain pause process
  // See: https://github.com/containers/crun/issues/798
  // before invoking handler check if this is not a kubernetes sandbox
  annotation = find_annotation (container, "io.kubernetes.cri.container-type");
  اگر (annotation && (strcmp (annotation, "sandbox") == 0))
    بازگشت 0;

  annotation = find_annotation (container, "run.oci.handler");

  /* Fail with EACCESS if global handler is already configured and there was an attempt to override it via spec.  */
  اگر (context->handler != پوچ && annotation != پوچ)
    بازگشت crun_make_error (err, EACCES, "invalid attempt to override already configured global handler: `%s`", context->handler);

  explicit_handler = context->handler ? context->handler : annotation;

  /* If an explicit handler was requested, use it.  */
  اگر (explicit_handler)
    {
      ساختار custom_handler_s *h;

      اگر (manager == پوچ)
        بازگشت crun_make_error (err, 0, "handler requested but no manager configured: `%s`", explicit_handler);

      h = handler_by_name (manager, explicit_handler);
      اگر (h)
        {
          *out = make_custom_handler_instance_s (h);
          اگر ((*out)->vtable->load)
            ret = (*out)->vtable->load (&((*out)->cookie), err);
          وگرنه
            ret = 0;
          برو out;
        }
    }

  اگر (manager == پوچ)
    بازگشت 0;

  ret = find_handler_for_container (manager, container, out, err);

out:
  اگر (ret == 0 && *out != پوچ && container->context != پوچ && ! (*out)->vtable->supports_open_درخت_namespace)
    container->context->no_open_درخت_namespace = true;

  بازگشت ret;
}
