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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "custom-handler.h"
#ಅನ್ತರ್ಭಾವ "container.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "linux.h"
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ಯದಿ HAVE_DLOPEN && HAVE_LIBKRUN
ಬಾಹ್ಯ ಸಂರಚನಾ custom_handler_s handler_libkrun;
#endif
#ಯದಿ HAVE_DLOPEN && HAVE_WASMTIME
ಬಾಹ್ಯ ಸಂರಚನಾ custom_handler_s handler_wasmtime;
#endif
#ಯದಿ HAVE_DLOPEN && HAVE_WASMEDGE
ಬಾಹ್ಯ ಸಂರಚನಾ custom_handler_s handler_wasmedge;
#endif
#ಯದಿ HAVE_DLOPEN && HAVE_WASMER
ಬಾಹ್ಯ ಸಂರಚನಾ custom_handler_s handler_wasmer;
#endif
#ಯದಿ HAVE_DLOPEN && HAVE_WAMR
ಬಾಹ್ಯ ಸಂರಚನಾ custom_handler_s handler_wamr;
#endif
#ಯದಿ HAVE_DLOPEN && HAVE_MONO
ಬಾಹ್ಯ ಸಂರಚನಾ custom_handler_s handler_mono;
#endif
#ಯದಿ HAVE_DLOPEN && HAVE_SPIN
ಬಾಹ್ಯ ಸಂರಚನಾ custom_handler_s handler_spin;
#endif

ಸ್ಥಿರ ಸಂರಚನಾ custom_handler_s *static_handlers[] = {
#ಯದಿ HAVE_DLOPEN && HAVE_LIBKRUN
  &handler_libkrun,
#endif
#ಯದಿ HAVE_DLOPEN && HAVE_WASMEDGE
  &handler_wasmedge,
#endif
#ಯದಿ HAVE_DLOPEN && HAVE_WASMER
  &handler_wasmer,
#endif
#ಯದಿ HAVE_DLOPEN && HAVE_WASMTIME
  &handler_wasmtime,
#endif
#ಯದಿ HAVE_DLOPEN && HAVE_WAMR
  &handler_wamr,
#endif
#ಯದಿ HAVE_DLOPEN && HAVE_MONO
  &handler_mono,
#endif
#ಯದಿ HAVE_DLOPEN && HAVE_SPIN
  &handler_spin,
#endif
  ರಿಕ್ತ,
};

ಸಂರಚನಾ custom_handler_manager_s
{
  ಸಂರಚನಾ custom_handler_s **handlers;
  ಶೂನ್ಯ **handles;
  size_t handlers_len;
};

ಸಂರಚನಾ custom_handler_manager_s *
libcrun_handler_manager_create (libcrun_error_t *err arg_unused)
{
  ಸಂರಚನಾ custom_handler_s **handlers = ರಿಕ್ತ;
  ಶೂನ್ಯ **handles = ರಿಕ್ತ;
  ಸಂರಚನಾ custom_handler_manager_s *m;
  size_t i, handlers_len;

  /* Count the static handlers.  */
  ಪ್ರತಿ (handlers_len = 0; static_handlers[handlers_len]; handlers_len++)
    ;

  ಯದಿ (handlers_len)
    {
      handlers = xmalloc (ಪರಿಮಾಣ (ಸಂರಚನಾ custom_handler_s *) * handlers_len);
      handles = xmalloc0 (ಪರಿಮಾಣ (ಶೂನ್ಯ *) * handlers_len);
    }

  ಪ್ರತಿ (i = 0; i < handlers_len; i++)
    handlers[i] = static_handlers[i];

  m = xmalloc0 (ಪರಿಮಾಣ (ಸಂರಚನಾ custom_handler_manager_s));
  m->handlers = handlers;
  m->handles = handles;
  m->handlers_len = handlers_len;

  ಪ್ರತ್ಯಾಗಮ m;
}

ಶೂನ್ಯ
handler_manager_free (ಸಂರಚನಾ custom_handler_manager_s *manager)
{
  size_t i;

  ಯದಿ (manager == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;

  ಪ್ರತಿ (i = 0; i < manager->handlers_len; i++)
    {
#ifdef HAVE_DLOPEN
      ಯದಿ (manager->handles[i])
        dlclose (manager->handles[i]);
#endif
    }
  ಮುಕ್ತ (manager->handlers);
  ಮುಕ್ತ (manager->handles);
  ಮುಕ್ತ (manager);
}

#ifdef HAVE_DLOPEN
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
handler_manager_add_so (ಸಂರಚನಾ custom_handler_manager_s *manager, ಶೂನ್ಯ *handle, libcrun_error_t *err)
{
  ಸಂರಚನಾ custom_handler_s *h = ರಿಕ್ತ;
  run_oci_get_handler_cb cb;

  cb = (run_oci_get_handler_cb) dlsym (handle, "run_oci_handler_get_handler");
  ಯದಿ (UNLIKELY (cb == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot find symbol `run_oci_handler_get_handler`");

  h = cb ();
  ಯದಿ (UNLIKELY (h == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the callback `run_oci_handler_get_handler` didn't return a handler");

  manager->handlers = xrealloc (manager->handlers, ಪರಿಮಾಣ (ಸಂರಚನಾ custom_handler_s *) * (manager->handlers_len + 1));
  manager->handles = xrealloc (manager->handles, ಪರಿಮಾಣ (ಶೂನ್ಯ *) * (manager->handlers_len + 1));

  manager->handlers[manager->handlers_len] = h;
  manager->handles[manager->handlers_len] = handle;
  manager->handlers_len++;
  ಪ್ರತ್ಯಾಗಮ 0;
}
#endif

ಪೂರ್ಣಾಙ್ಕ
libcrun_handler_manager_load_directory (ಸಂರಚನಾ custom_handler_manager_s *manager, ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err)
{
#ifdef HAVE_DLOPEN
  cleanup_dir DIR *dir = ರಿಕ್ತ;
  ಸಂರಚನಾ dirent *next;

  dir = opendir (path);
  ಯದಿ (UNLIKELY (dir == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot opendir `%s`", path);

  ಪ್ರತಿ (next = readdir (dir); next; next = readdir (dir))
    {
      cleanup_free ವರ್ಣ *fpath = ರಿಕ್ತ;
      ಧ್ರುವ ವರ್ಣ *name;
      ಶೂನ್ಯ *handle;
      ಪೂರ್ಣಾಙ್ಕ ret;

      name = next->d_name;
      ಯದಿ (name[0] == '.')
        ಅನುವರ್ತ;

      ret = append_paths (&fpath, err, path, name, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      handle = dlopen (fpath, RTLD_NOW);
      ಯದಿ (UNLIKELY (handle == ರಿಕ್ತ))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot load `%s`: `%s`", fpath, dlerror ());

      ret = handler_manager_add_so (manager, handle, err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          dlclose (handle);
          ಪ್ರತ್ಯಾಗಮ ret;
        }
    }
  ಪ್ರತ್ಯಾಗಮ 0;
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) manager;
  (ಶೂನ್ಯ) path;
  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ENOTSUP, "dlopen not available");
#endif
}

ಸಂರಚನಾ custom_handler_s *
handler_by_name (ಸಂರಚನಾ custom_handler_manager_s *manager, ಧ್ರುವ ವರ್ಣ *name)
{
  size_t i;

  ಪ್ರತಿ (i = 0; i < manager->handlers_len; i++)
    {
      ಯದಿ (strcmp (manager->handlers[i]->name, name) == 0)
        ಪ್ರತ್ಯಾಗಮ manager->handlers[i];
      ಯದಿ (manager->handlers[i]->alias && strcmp (manager->handlers[i]->alias, name) == 0)
        ಪ್ರತ್ಯಾಗಮ manager->handlers[i];
    }
  ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಶೂನ್ಯ
libcrun_handler_manager_print_feature_tags (ಸಂರಚನಾ custom_handler_manager_s *manager, FILE *out)
{
  size_t i;

  ಪ್ರತಿ (i = 0; i < manager->handlers_len; i++)
    ಯದಿ (manager->handlers[i]->feature_string)
      fprintf (out, "+%s ", manager->handlers[i]->feature_string);
}

ವರ್ಣ **
libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err arg_unused)
{
  ಸಂರಚನಾ custom_handler_manager_s *manager;
  ವರ್ಣ **out;
  size_t i, n = 0;

  ಯದಿ (ctx == ರಿಕ್ತ || ctx->handler_manager == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  manager = ctx->handler_manager;

  ಪ್ರತಿ (i = 0; i < manager->handlers_len; i++)
    ಯದಿ (manager->handlers[i]->feature_string)
      n++;

  ಯದಿ (n == 0)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  out = xmalloc0 ((n + 1) * ಪರಿಮಾಣ (ವರ್ಣ *));
  n = 0;
  ಪ್ರತಿ (i = 0; i < manager->handlers_len; i++)
    ಯದಿ (manager->handlers[i]->feature_string)
      out[n++] = xstrdup (manager->handlers[i]->feature_string);

  ಪ್ರತ್ಯಾಗಮ out;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ custom_handler_instance_s *
make_custom_handler_instance_s (ಸಂರಚನಾ custom_handler_s *vtable)
{
  ಸಂರಚನಾ custom_handler_instance_s *ret = xmalloc0 (ಪರಿಮಾಣ (ಸಂರಚನಾ custom_handler_instance_s));

  ret->vtable = vtable;
  ret->cookie = ರಿಕ್ತ;

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
find_handler_for_container (ಸಂರಚನಾ custom_handler_manager_s *manager,
                            libcrun_container_t *container,
                            ಸಂರಚನಾ custom_handler_instance_s **out,
                            libcrun_error_t *err)
{
  size_t i;

  memset (out, 0, ಪರಿಮಾಣ (*out));

  ಪ್ರತಿ (i = 0; i < manager->handlers_len; i++)
    {
      ಪೂರ್ಣಾಙ್ಕ ret;

      ಯದಿ (manager->handlers[i]->can_handle_container == ರಿಕ್ತ)
        ಅನುವರ್ತ;

      ret = manager->handlers[i]->can_handle_container (container, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ಯದಿ (ret)
        {
          *out = make_custom_handler_instance_s (manager->handlers[i]);
          ಯದಿ ((*out)->vtable->load)
            ಪ್ರತ್ಯಾಗಮ (*out)->vtable->load (&((*out)->cookie), err);

          ಪ್ರತ್ಯಾಗಮ 0;
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_configure_handler (ಸಂರಚನಾ custom_handler_manager_s *manager,
                           libcrun_context_t *context,
                           libcrun_container_t *container,
                           ಸಂರಚನಾ custom_handler_instance_s **out,
                           libcrun_error_t *err)
{
  ಧ್ರುವ ವರ್ಣ *explicit_handler;
  ಧ್ರುವ ವರ್ಣ *annotation;
  ಪೂರ್ಣಾಙ್ಕ ret;

  *out = ರಿಕ್ತ;

  // Kubernetes sandbox containers must be executed as regular process
  // Example sandbox container can contain pause process
  // See: https://github.com/containers/crun/issues/798
  // before invoking handler check if this is not a kubernetes sandbox
  annotation = find_annotation (container, "io.kubernetes.cri.container-type");
  ಯದಿ (annotation && (strcmp (annotation, "sandbox") == 0))
    ಪ್ರತ್ಯಾಗಮ 0;

  annotation = find_annotation (container, "run.oci.handler");

  /* Fail with EACCESS if global handler is already configured and there was an attempt to override it via spec.  */
  ಯದಿ (context->handler != ರಿಕ್ತ && annotation != ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EACCES, "invalid attempt to override already configured global handler: `%s`", context->handler);

  explicit_handler = context->handler ? context->handler : annotation;

  /* If an explicit handler was requested, use it.  */
  ಯದಿ (explicit_handler)
    {
      ಸಂರಚನಾ custom_handler_s *h;

      ಯದಿ (manager == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "handler requested but no manager configured: `%s`", explicit_handler);

      h = handler_by_name (manager, explicit_handler);
      ಯದಿ (h)
        {
          *out = make_custom_handler_instance_s (h);
          ಯದಿ ((*out)->vtable->load)
            ret = (*out)->vtable->load (&((*out)->cookie), err);
          ಅನ್ಯಥಾ
            ret = 0;
          ಗಚ್ಛ out;
        }
    }

  ಯದಿ (manager == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  ret = find_handler_for_container (manager, container, out, err);

out:
  ಯದಿ (ret == 0 && *out != ರಿಕ್ತ && container->context != ರಿಕ್ತ && ! (*out)->vtable->supports_open_ವೃಕ್ಷ_namespace)
    container->context->no_open_ವೃಕ್ಷ_namespace = true;

  ಪ್ರತ್ಯಾಗಮ ret;
}
