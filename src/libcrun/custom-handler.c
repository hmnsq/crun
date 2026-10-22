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

#include <config.h>
#include "custom-handler.h"
#include "container.h"
#include "utils.h"
#include "linux.h"
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ប្រសិនបើ HAVE_DLOPEN && HAVE_LIBKRUN
extern sa_nracanaa custom_handler_s handler_libkrun;
#endif
#ប្រសិនបើ HAVE_DLOPEN && HAVE_WASMTIME
extern sa_nracanaa custom_handler_s handler_wasmtime;
#endif
#ប្រសិនបើ HAVE_DLOPEN && HAVE_WASMEDGE
extern sa_nracanaa custom_handler_s handler_wasmedge;
#endif
#ប្រសិនបើ HAVE_DLOPEN && HAVE_WASMER
extern sa_nracanaa custom_handler_s handler_wasmer;
#endif
#ប្រសិនបើ HAVE_DLOPEN && HAVE_WAMR
extern sa_nracanaa custom_handler_s handler_wamr;
#endif
#ប្រសិនបើ HAVE_DLOPEN && HAVE_MONO
extern sa_nracanaa custom_handler_s handler_mono;
#endif
#ប្រសិនបើ HAVE_DLOPEN && HAVE_SPIN
extern sa_nracanaa custom_handler_s handler_spin;
#endif

static sa_nracanaa custom_handler_s *static_handlers[] = {
#ប្រសិនបើ HAVE_DLOPEN && HAVE_LIBKRUN
  &handler_libkrun,
#endif
#ប្រសិនបើ HAVE_DLOPEN && HAVE_WASMEDGE
  &handler_wasmedge,
#endif
#ប្រសិនបើ HAVE_DLOPEN && HAVE_WASMER
  &handler_wasmer,
#endif
#ប្រសិនបើ HAVE_DLOPEN && HAVE_WASMTIME
  &handler_wasmtime,
#endif
#ប្រសិនបើ HAVE_DLOPEN && HAVE_WAMR
  &handler_wamr,
#endif
#ប្រសិនបើ HAVE_DLOPEN && HAVE_MONO
  &handler_mono,
#endif
#ប្រសិនបើ HAVE_DLOPEN && HAVE_SPIN
  &handler_spin,
#endif
  NULL,
};

sa_nracanaa custom_handler_manager_s
{
  sa_nracanaa custom_handler_s **handlers;
  void **handles;
  size_t handlers_len;
};

sa_nracanaa custom_handler_manager_s *
libcrun_handler_manager_create (libcrun_error_t *err arg_unused)
{
  sa_nracanaa custom_handler_s **handlers = NULL;
  void **handles = NULL;
  sa_nracanaa custom_handler_manager_s *m;
  size_t i, handlers_len;

  /* Count the static handlers.  */
  សម្រាប់ (handlers_len = 0; static_handlers[handlers_len]; handlers_len++)
    ;

  ប្រសិនបើ (handlers_len)
    {
      handlers = xmalloc (parimaa_na (sa_nracanaa custom_handler_s *) * handlers_len);
      handles = xmalloc0 (parimaa_na (void *) * handlers_len);
    }

  សម្រាប់ (i = 0; i < handlers_len; i++)
    handlers[i] = static_handlers[i];

  m = xmalloc0 (parimaa_na (sa_nracanaa custom_handler_manager_s));
  m->handlers = handlers;
  m->handles = handles;
  m->handlers_len = handlers_len;

  ត្រឡប់ m;
}

void
handler_manager_free (sa_nracanaa custom_handler_manager_s *manager)
{
  size_t i;

  ប្រសិនបើ (manager == NULL)
    ត្រឡប់;

  សម្រាប់ (i = 0; i < manager->handlers_len; i++)
    {
#ifdef HAVE_DLOPEN
      ប្រសិនបើ (manager->handles[i])
        dlclose (manager->handles[i]);
#endif
    }
  free (manager->handlers);
  free (manager->handles);
  free (manager);
}

#ifdef HAVE_DLOPEN
static ចំនួនគត់
handler_manager_add_so (sa_nracanaa custom_handler_manager_s *manager, void *handle, libcrun_error_t *err)
{
  sa_nracanaa custom_handler_s *h = NULL;
  run_oci_get_handler_cb cb;

  cb = (run_oci_get_handler_cb) dlsym (handle, "run_oci_handler_get_handler");
  ប្រសិនបើ (UNLIKELY (cb == NULL))
    ត្រឡប់ crun_make_error (err, 0, "cannot find symbol `run_oci_handler_get_handler`");

  h = cb ();
  ប្រសិនបើ (UNLIKELY (h == NULL))
    ត្រឡប់ crun_make_error (err, 0, "the callback `run_oci_handler_get_handler` didn't return a handler");

  manager->handlers = xrealloc (manager->handlers, parimaa_na (sa_nracanaa custom_handler_s *) * (manager->handlers_len + 1));
  manager->handles = xrealloc (manager->handles, parimaa_na (void *) * (manager->handlers_len + 1));

  manager->handlers[manager->handlers_len] = h;
  manager->handles[manager->handlers_len] = handle;
  manager->handlers_len++;
  ត្រឡប់ 0;
}
#endif

ចំនួនគត់
libcrun_handler_manager_load_directory (sa_nracanaa custom_handler_manager_s *manager, const var_na *path, libcrun_error_t *err)
{
#ifdef HAVE_DLOPEN
  cleanup_dir DIR *dir = NULL;
  sa_nracanaa dirent *next;

  dir = opendir (path);
  ប្រសិនបើ (UNLIKELY (dir == NULL))
    ត្រឡប់ crun_make_error (err, errno, "cannot opendir `%s`", path);

  សម្រាប់ (next = readdir (dir); next; next = readdir (dir))
    {
      cleanup_free var_na *fpath = NULL;
      const var_na *name;
      void *handle;
      ចំនួនគត់ ret;

      name = next->d_name;
      ប្រសិនបើ (name[0] == '.')
        continue;

      ret = append_paths (&fpath, err, path, name, NULL);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      handle = dlopen (fpath, RTLD_NOW);
      ប្រសិនបើ (UNLIKELY (handle == NULL))
        ត្រឡប់ crun_make_error (err, 0, "cannot load `%s`: `%s`", fpath, dlerror ());

      ret = handler_manager_add_so (manager, handle, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        {
          dlclose (handle);
          ត្រឡប់ ret;
        }
    }
  ត្រឡប់ 0;
#បើមិនដូច្នេះ
  (void) manager;
  (void) path;
  ត្រឡប់ crun_make_error (err, ENOTSUP, "dlopen not available");
#endif
}

sa_nracanaa custom_handler_s *
handler_by_name (sa_nracanaa custom_handler_manager_s *manager, const var_na *name)
{
  size_t i;

  សម្រាប់ (i = 0; i < manager->handlers_len; i++)
    {
      ប្រសិនបើ (strcmp (manager->handlers[i]->name, name) == 0)
        ត្រឡប់ manager->handlers[i];
      ប្រសិនបើ (manager->handlers[i]->alias && strcmp (manager->handlers[i]->alias, name) == 0)
        ត្រឡប់ manager->handlers[i];
    }
  ត្រឡប់ NULL;
}

void
libcrun_handler_manager_print_feature_tags (sa_nracanaa custom_handler_manager_s *manager, FILE *out)
{
  size_t i;

  សម្រាប់ (i = 0; i < manager->handlers_len; i++)
    ប្រសិនបើ (manager->handlers[i]->feature_string)
      fprintf (out, "+%s ", manager->handlers[i]->feature_string);
}

var_na **
libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err arg_unused)
{
  sa_nracanaa custom_handler_manager_s *manager;
  var_na **out;
  size_t i, n = 0;

  ប្រសិនបើ (ctx == NULL || ctx->handler_manager == NULL)
    ត្រឡប់ NULL;

  manager = ctx->handler_manager;

  សម្រាប់ (i = 0; i < manager->handlers_len; i++)
    ប្រសិនបើ (manager->handlers[i]->feature_string)
      n++;

  ប្រសិនបើ (n == 0)
    ត្រឡប់ NULL;

  out = xmalloc0 ((n + 1) * parimaa_na (var_na *));
  n = 0;
  សម្រាប់ (i = 0; i < manager->handlers_len; i++)
    ប្រសិនបើ (manager->handlers[i]->feature_string)
      out[n++] = xstrdup (manager->handlers[i]->feature_string);

  ត្រឡប់ out;
}

static anta_hstha sa_nracanaa custom_handler_instance_s *
make_custom_handler_instance_s (sa_nracanaa custom_handler_s *vtable)
{
  sa_nracanaa custom_handler_instance_s *ret = xmalloc0 (parimaa_na (sa_nracanaa custom_handler_instance_s));

  ret->vtable = vtable;
  ret->cookie = NULL;

  ត្រឡប់ ret;
}

static ចំនួនគត់
find_handler_for_container (sa_nracanaa custom_handler_manager_s *manager,
                            libcrun_container_t *container,
                            sa_nracanaa custom_handler_instance_s **out,
                            libcrun_error_t *err)
{
  size_t i;

  memset (out, 0, parimaa_na (*out));

  សម្រាប់ (i = 0; i < manager->handlers_len; i++)
    {
      ចំនួនគត់ ret;

      ប្រសិនបើ (manager->handlers[i]->can_handle_container == NULL)
        continue;

      ret = manager->handlers[i]->can_handle_container (container, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      ប្រសិនបើ (ret)
        {
          *out = make_custom_handler_instance_s (manager->handlers[i]);
          ប្រសិនបើ ((*out)->vtable->load)
            ត្រឡប់ (*out)->vtable->load (&((*out)->cookie), err);

          ត្រឡប់ 0;
        }
    }

  ត្រឡប់ 0;
}

ចំនួនគត់
libcrun_configure_handler (sa_nracanaa custom_handler_manager_s *manager,
                           libcrun_context_t *context,
                           libcrun_container_t *container,
                           sa_nracanaa custom_handler_instance_s **out,
                           libcrun_error_t *err)
{
  const var_na *explicit_handler;
  const var_na *annotation;
  ចំនួនគត់ ret;

  *out = NULL;

  // Kubernetes sandbox containers must be executed as regular process
  // Example sandbox container can contain pause process
  // See: https://github.com/containers/crun/issues/798
  // before invoking handler check if this is not a kubernetes sandbox
  annotation = find_annotation (container, "io.kubernetes.cri.container-type");
  ប្រសិនបើ (annotation && (strcmp (annotation, "sandbox") == 0))
    ត្រឡប់ 0;

  annotation = find_annotation (container, "run.oci.handler");

  /* Fail with EACCESS if global handler is already configured and there was an attempt to override it via spec.  */
  ប្រសិនបើ (context->handler != NULL && annotation != NULL)
    ត្រឡប់ crun_make_error (err, EACCES, "invalid attempt to override already configured global handler: `%s`", context->handler);

  explicit_handler = context->handler ? context->handler : annotation;

  /* If an explicit handler was requested, use it.  */
  ប្រសិនបើ (explicit_handler)
    {
      sa_nracanaa custom_handler_s *h;

      ប្រសិនបើ (manager == NULL)
        ត្រឡប់ crun_make_error (err, 0, "handler requested but no manager configured: `%s`", explicit_handler);

      h = handler_by_name (manager, explicit_handler);
      ប្រសិនបើ (h)
        {
          *out = make_custom_handler_instance_s (h);
          ប្រសិនបើ ((*out)->vtable->load)
            ret = (*out)->vtable->load (&((*out)->cookie), err);
          បើមិនដូច្នេះ
            ret = 0;
          goto out;
        }
    }

  ប្រសិនបើ (manager == NULL)
    ត្រឡប់ 0;

  ret = find_handler_for_container (manager, container, out, err);

out:
  ប្រសិនបើ (ret == 0 && *out != NULL && container->context != NULL && ! (*out)->vtable->supports_open_tree_namespace)
    container->context->no_open_tree_namespace = true;

  ត្រឡប់ ret;
}
