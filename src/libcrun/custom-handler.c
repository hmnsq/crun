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
#ਅਨ੍ਤਰ੍ਭਾਵ "custom-handler.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "container.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "linux.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sched.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DLOPEN
#  include <dlfcn.h>
#ਜੇ_ਅੰਤ

#ਜੇ HAVE_DLOPEN && HAVE_LIBKRUN
ਬਾਹਰੀ ਬਣਤਰ custom_handler_s handler_libkrun;
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_DLOPEN && HAVE_WASMTIME
ਬਾਹਰੀ ਬਣਤਰ custom_handler_s handler_wasmtime;
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_DLOPEN && HAVE_WASMEDGE
ਬਾਹਰੀ ਬਣਤਰ custom_handler_s handler_wasmedge;
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_DLOPEN && HAVE_WASMER
ਬਾਹਰੀ ਬਣਤਰ custom_handler_s handler_wasmer;
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_DLOPEN && HAVE_WAMR
ਬਾਹਰੀ ਬਣਤਰ custom_handler_s handler_wamr;
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_DLOPEN && HAVE_MONO
ਬਾਹਰੀ ਬਣਤਰ custom_handler_s handler_mono;
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_DLOPEN && HAVE_SPIN
ਬਾਹਰੀ ਬਣਤਰ custom_handler_s handler_spin;
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਬਣਤਰ custom_handler_s *static_handlers[] = {
#ਜੇ HAVE_DLOPEN && HAVE_LIBKRUN
  &handler_libkrun,
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_DLOPEN && HAVE_WASMEDGE
  &handler_wasmedge,
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_DLOPEN && HAVE_WASMER
  &handler_wasmer,
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_DLOPEN && HAVE_WASMTIME
  &handler_wasmtime,
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_DLOPEN && HAVE_WAMR
  &handler_wamr,
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_DLOPEN && HAVE_MONO
  &handler_mono,
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_DLOPEN && HAVE_SPIN
  &handler_spin,
#ਜੇ_ਅੰਤ
  ਨਿਰਾ,
};

ਬਣਤਰ custom_handler_manager_s
{
  ਬਣਤਰ custom_handler_s **handlers;
  ਖਾਲੀ **handles;
  size_t handlers_len;
};

ਬਣਤਰ custom_handler_manager_s *
libcrun_handler_manager_create (libcrun_error_t *err arg_ਅਣਵਰਤਿਆ)
{
  ਬਣਤਰ custom_handler_s **handlers = ਨਿਰਾ;
  ਖਾਲੀ **handles = ਨਿਰਾ;
  ਬਣਤਰ custom_handler_manager_s *m;
  size_t i, handlers_len;

  /* Count the static handlers.  */
  ਲਈ (handlers_len = 0; static_handlers[handlers_len]; handlers_len++)
    ;

  ਜੇ (handlers_len)
    {
      handlers = xmalloc (ਆਕਾਰ (ਬਣਤਰ custom_handler_s *) * handlers_len);
      handles = xmalloc0 (ਆਕਾਰ (ਖਾਲੀ *) * handlers_len);
    }

  ਲਈ (i = 0; i < handlers_len; i++)
    handlers[i] = static_handlers[i];

  m = xmalloc0 (ਆਕਾਰ (ਬਣਤਰ custom_handler_manager_s));
  m->handlers = handlers;
  m->handles = handles;
  m->handlers_len = handlers_len;

  ਵਾਪਸ m;
}

ਖਾਲੀ
handler_manager_free (ਬਣਤਰ custom_handler_manager_s *manager)
{
  size_t i;

  ਜੇ (manager == ਨਿਰਾ)
    ਵਾਪਸ;

  ਲਈ (i = 0; i < manager->handlers_len; i++)
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DLOPEN
      ਜੇ (manager->handles[i])
        dlclose (manager->handles[i]);
#ਜੇ_ਅੰਤ
    }
  ਮੁਕਤ (manager->handlers);
  ਮੁਕਤ (manager->handles);
  ਮੁਕਤ (manager);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DLOPEN
ਸਥਿਰ ਪੂਰਨ
handler_manager_ਜੋੜੋ_so (ਬਣਤਰ custom_handler_manager_s *manager, ਖਾਲੀ *handle, libcrun_error_t *err)
{
  ਬਣਤਰ custom_handler_s *h = ਨਿਰਾ;
  ਲੜੀਵਾਰ_oci_get_handler_cb cb;

  cb = (ਲੜੀਵਾਰ_oci_get_handler_cb) dlsym (handle, "run_oci_handler_get_handler");
  ਜੇ (UNLIKELY (cb == ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, 0, "cannot find symbol `run_oci_handler_get_handler`");

  h = cb ();
  ਜੇ (UNLIKELY (h == ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, 0, "the callback `run_oci_handler_get_handler` didn't return a handler");

  manager->handlers = xrealloc (manager->handlers, ਆਕਾਰ (ਬਣਤਰ custom_handler_s *) * (manager->handlers_len + 1));
  manager->handles = xrealloc (manager->handles, ਆਕਾਰ (ਖਾਲੀ *) * (manager->handlers_len + 1));

  manager->handlers[manager->handlers_len] = h;
  manager->handles[manager->handlers_len] = handle;
  manager->handlers_len++;
  ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ

ਪੂਰਨ
libcrun_handler_manager_ਲੱਦੋ_directory (ਬਣਤਰ custom_handler_manager_s *manager, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DLOPEN
  cleanup_dir DIR *dir = ਨਿਰਾ;
  ਬਣਤਰ dirent *ਅਗਲਾ;

  dir = opendir (ਰਸਤਾ);
  ਜੇ (UNLIKELY (dir == ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, errno, "cannot opendir `%s`", ਰਸਤਾ);

  ਲਈ (ਅਗਲਾ = readdir (dir); ਅਗਲਾ; ਅਗਲਾ = readdir (dir))
    {
      cleanup_free ਅੱਖਰ *fpath = ਨਿਰਾ;
      ਅਚਲ ਅੱਖਰ *name;
      ਖਾਲੀ *handle;
      ਪੂਰਨ ret;

      name = ਅਗਲਾ->d_name;
      ਜੇ (name[0] == '.')
        ਜਾਰੀ;

      ret = append_paths (&fpath, err, ਰਸਤਾ, name, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      handle = dlopen (fpath, RTLD_NOW);
      ਜੇ (UNLIKELY (handle == ਨਿਰਾ))
        ਵਾਪਸ crun_make_error (err, 0, "cannot load `%s`: `%s`", fpath, dlerror ());

      ret = handler_manager_ਜੋੜੋ_so (manager, handle, err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          dlclose (handle);
          ਵਾਪਸ ret;
        }
    }
  ਵਾਪਸ 0;
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) manager;
  (ਖਾਲੀ) ਰਸਤਾ;
  ਵਾਪਸ crun_make_error (err, ENOTSUP, "dlopen not available");
#ਜੇ_ਅੰਤ
}

ਬਣਤਰ custom_handler_s *
handler_by_name (ਬਣਤਰ custom_handler_manager_s *manager, ਅਚਲ ਅੱਖਰ *name)
{
  size_t i;

  ਲਈ (i = 0; i < manager->handlers_len; i++)
    {
      ਜੇ (strcmp (manager->handlers[i]->name, name) == 0)
        ਵਾਪਸ manager->handlers[i];
      ਜੇ (manager->handlers[i]->alias && strcmp (manager->handlers[i]->alias, name) == 0)
        ਵਾਪਸ manager->handlers[i];
    }
  ਵਾਪਸ ਨਿਰਾ;
}

ਖਾਲੀ
libcrun_handler_manager_print_feature_ਟੈਗ_ਸੂਚੀ (ਬਣਤਰ custom_handler_manager_s *manager, FILE *ਬਾਹਰਲਾ)
{
  size_t i;

  ਲਈ (i = 0; i < manager->handlers_len; i++)
    ਜੇ (manager->handlers[i]->feature_string)
      fprintf (ਬਾਹਰਲਾ, "+%s ", manager->handlers[i]->feature_string);
}

ਅੱਖਰ **
libcrun_context_get_handler_feature_ਟੈਗ_ਸੂਚੀ (libcrun_context_t *ctx, libcrun_error_t *err arg_ਅਣਵਰਤਿਆ)
{
  ਬਣਤਰ custom_handler_manager_s *manager;
  ਅੱਖਰ **ਬਾਹਰਲਾ;
  size_t i, n = 0;

  ਜੇ (ctx == ਨਿਰਾ || ctx->handler_manager == ਨਿਰਾ)
    ਵਾਪਸ ਨਿਰਾ;

  manager = ctx->handler_manager;

  ਲਈ (i = 0; i < manager->handlers_len; i++)
    ਜੇ (manager->handlers[i]->feature_string)
      n++;

  ਜੇ (n == 0)
    ਵਾਪਸ ਨਿਰਾ;

  ਬਾਹਰਲਾ = xmalloc0 ((n + 1) * ਆਕਾਰ (ਅੱਖਰ *));
  n = 0;
  ਲਈ (i = 0; i < manager->handlers_len; i++)
    ਜੇ (manager->handlers[i]->feature_string)
      ਬਾਹਰਲਾ[n++] = xstrdup (manager->handlers[i]->feature_string);

  ਵਾਪਸ ਬਾਹਰਲਾ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ custom_handler_instance_s *
make_custom_handler_instance_s (ਬਣਤਰ custom_handler_s *vtable)
{
  ਬਣਤਰ custom_handler_instance_s *ret = xmalloc0 (ਆਕਾਰ (ਬਣਤਰ custom_handler_instance_s));

  ret->vtable = vtable;
  ret->cookie = ਨਿਰਾ;

  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
ਲੱਭੋ_handler_for_container (ਬਣਤਰ custom_handler_manager_s *manager,
                            libcrun_container_t *container,
                            ਬਣਤਰ custom_handler_instance_s **ਬਾਹਰਲਾ,
                            libcrun_error_t *err)
{
  size_t i;

  memset (ਬਾਹਰਲਾ, 0, ਆਕਾਰ (*ਬਾਹਰਲਾ));

  ਲਈ (i = 0; i < manager->handlers_len; i++)
    {
      ਪੂਰਨ ret;

      ਜੇ (manager->handlers[i]->can_handle_container == ਨਿਰਾ)
        ਜਾਰੀ;

      ret = manager->handlers[i]->can_handle_container (container, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ਜੇ (ret)
        {
          *ਬਾਹਰਲਾ = make_custom_handler_instance_s (manager->handlers[i]);
          ਜੇ ((*ਬਾਹਰਲਾ)->vtable->ਲੱਦੋ)
            ਵਾਪਸ (*ਬਾਹਰਲਾ)->vtable->ਲੱਦੋ (&((*ਬਾਹਰਲਾ)->cookie), err);

          ਵਾਪਸ 0;
        }
    }

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_configure_handler (ਬਣਤਰ custom_handler_manager_s *manager,
                           libcrun_context_t *context,
                           libcrun_container_t *container,
                           ਬਣਤਰ custom_handler_instance_s **ਬਾਹਰਲਾ,
                           libcrun_error_t *err)
{
  ਅਚਲ ਅੱਖਰ *explicit_handler;
  ਅਚਲ ਅੱਖਰ *annotation;
  ਪੂਰਨ ret;

  *ਬਾਹਰਲਾ = ਨਿਰਾ;

  // Kubernetes sandbox containers must be executed as regular process
  // Example sandbox container can contain pause process
  // See: https://github.com/containers/crun/issues/798
  // before invoking handler check if this is not a kubernetes sandbox
  annotation = ਲੱਭੋ_annotation (container, "io.kubernetes.cri.container-type");
  ਜੇ (annotation && (strcmp (annotation, "sandbox") == 0))
    ਵਾਪਸ 0;

  annotation = ਲੱਭੋ_annotation (container, "run.oci.handler");

  /* Fail with EACCESS if global handler is already configured and there was an attempt to override it via spec.  */
  ਜੇ (context->handler != ਨਿਰਾ && annotation != ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, EACCES, "invalid attempt to override already configured global handler: `%s`", context->handler);

  explicit_handler = context->handler ? context->handler : annotation;

  /* If an explicit handler was requested, use it.  */
  ਜੇ (explicit_handler)
    {
      ਬਣਤਰ custom_handler_s *h;

      ਜੇ (manager == ਨਿਰਾ)
        ਵਾਪਸ crun_make_error (err, 0, "handler requested but no manager configured: `%s`", explicit_handler);

      h = handler_by_name (manager, explicit_handler);
      ਜੇ (h)
        {
          *ਬਾਹਰਲਾ = make_custom_handler_instance_s (h);
          ਜੇ ((*ਬਾਹਰਲਾ)->vtable->ਲੱਦੋ)
            ret = (*ਬਾਹਰਲਾ)->vtable->ਲੱਦੋ (&((*ਬਾਹਰਲਾ)->cookie), err);
          ਨਹੀਂਤਾਂ
            ret = 0;
          ਜਾਓ ਬਾਹਰਲਾ;
        }
    }

  ਜੇ (manager == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ret = ਲੱਭੋ_handler_for_container (manager, container, ਬਾਹਰਲਾ, err);

ਬਾਹਰਲਾ:
  ਜੇ (ret == 0 && *ਬਾਹਰਲਾ != ਨਿਰਾ && container->context != ਨਿਰਾ && ! (*ਬਾਹਰਲਾ)->vtable->supports_open_ਰੁੱਖ_namespace)
    container->context->no_open_ਰੁੱਖ_namespace = true;

  ਵਾਪਸ ret;
}
