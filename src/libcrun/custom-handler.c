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

#если HAVE_DLOPEN && HAVE_LIBKRUN
extern struct custom_handler_s handler_libkrun;
#endif
#если HAVE_DLOPEN && HAVE_WASMTIME
extern struct custom_handler_s handler_wasmtime;
#endif
#если HAVE_DLOPEN && HAVE_WASMEDGE
extern struct custom_handler_s handler_wasmedge;
#endif
#если HAVE_DLOPEN && HAVE_WASMER
extern struct custom_handler_s handler_wasmer;
#endif
#если HAVE_DLOPEN && HAVE_WAMR
extern struct custom_handler_s handler_wamr;
#endif
#если HAVE_DLOPEN && HAVE_MONO
extern struct custom_handler_s handler_mono;
#endif
#если HAVE_DLOPEN && HAVE_SPIN
extern struct custom_handler_s handler_spin;
#endif

static struct custom_handler_s *static_handlers[] = {
#если HAVE_DLOPEN && HAVE_LIBKRUN
  &handler_libkrun,
#endif
#если HAVE_DLOPEN && HAVE_WASMEDGE
  &handler_wasmedge,
#endif
#если HAVE_DLOPEN && HAVE_WASMER
  &handler_wasmer,
#endif
#если HAVE_DLOPEN && HAVE_WASMTIME
  &handler_wasmtime,
#endif
#если HAVE_DLOPEN && HAVE_WAMR
  &handler_wamr,
#endif
#если HAVE_DLOPEN && HAVE_MONO
  &handler_mono,
#endif
#если HAVE_DLOPEN && HAVE_SPIN
  &handler_spin,
#endif
  NULL,
};

struct custom_handler_manager_s
{
  struct custom_handler_s **handlers;
  void **handles;
  size_t handlers_len;
};

struct custom_handler_manager_s *
libcrun_handler_manager_create (libcrun_error_t *err arg_unused)
{
  struct custom_handler_s **handlers = NULL;
  void **handles = NULL;
  struct custom_handler_manager_s *m;
  size_t i, handlers_len;

  /* Count the static handlers.  */
  для (handlers_len = 0; static_handlers[handlers_len]; handlers_len++)
    ;

  если (handlers_len)
    {
      handlers = xmalloc (sizeof (struct custom_handler_s *) * handlers_len);
      handles = xmalloc0 (sizeof (void *) * handlers_len);
    }

  для (i = 0; i < handlers_len; i++)
    handlers[i] = static_handlers[i];

  m = xmalloc0 (sizeof (struct custom_handler_manager_s));
  m->handlers = handlers;
  m->handles = handles;
  m->handlers_len = handlers_len;

  вернуть m;
}

void
handler_manager_free (struct custom_handler_manager_s *manager)
{
  size_t i;

  если (manager == NULL)
    вернуть;

  для (i = 0; i < manager->handlers_len; i++)
    {
#ifdef HAVE_DLOPEN
      если (manager->handles[i])
        dlclose (manager->handles[i]);
#endif
    }
  free (manager->handlers);
  free (manager->handles);
  free (manager);
}

#ifdef HAVE_DLOPEN
static целое
handler_manager_add_so (struct custom_handler_manager_s *manager, void *handle, libcrun_error_t *err)
{
  struct custom_handler_s *h = NULL;
  run_oci_get_handler_cb cb;

  cb = (run_oci_get_handler_cb) dlsym (handle, "run_oci_handler_get_handler");
  если (UNLIKELY (cb == NULL))
    вернуть crun_make_error (err, 0, "cannot find symbol `run_oci_handler_get_handler`");

  h = cb ();
  если (UNLIKELY (h == NULL))
    вернуть crun_make_error (err, 0, "the callback `run_oci_handler_get_handler` didn't return a handler");

  manager->handlers = xrealloc (manager->handlers, sizeof (struct custom_handler_s *) * (manager->handlers_len + 1));
  manager->handles = xrealloc (manager->handles, sizeof (void *) * (manager->handlers_len + 1));

  manager->handlers[manager->handlers_len] = h;
  manager->handles[manager->handlers_len] = handle;
  manager->handlers_len++;
  вернуть 0;
}
#endif

целое
libcrun_handler_manager_load_directory (struct custom_handler_manager_s *manager, const char *path, libcrun_error_t *err)
{
#ifdef HAVE_DLOPEN
  cleanup_dir DIR *dir = NULL;
  struct dirent *next;

  dir = opendir (path);
  если (UNLIKELY (dir == NULL))
    вернуть crun_make_error (err, errno, "cannot opendir `%s`", path);

  для (next = readdir (dir); next; next = readdir (dir))
    {
      cleanup_free char *fpath = NULL;
      const char *name;
      void *handle;
      целое ret;

      name = next->d_name;
      если (name[0] == '.')
        continue;

      ret = append_paths (&fpath, err, path, name, NULL);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      handle = dlopen (fpath, RTLD_NOW);
      если (UNLIKELY (handle == NULL))
        вернуть crun_make_error (err, 0, "cannot load `%s`: `%s`", fpath, dlerror ());

      ret = handler_manager_add_so (manager, handle, err);
      если (UNLIKELY (ret < 0))
        {
          dlclose (handle);
          вернуть ret;
        }
    }
  вернуть 0;
#иначе
  (void) manager;
  (void) path;
  вернуть crun_make_error (err, ENOTSUP, "dlopen not available");
#endif
}

struct custom_handler_s *
handler_by_name (struct custom_handler_manager_s *manager, const char *name)
{
  size_t i;

  для (i = 0; i < manager->handlers_len; i++)
    {
      если (strcmp (manager->handlers[i]->name, name) == 0)
        вернуть manager->handlers[i];
      если (manager->handlers[i]->alias && strcmp (manager->handlers[i]->alias, name) == 0)
        вернуть manager->handlers[i];
    }
  вернуть NULL;
}

void
libcrun_handler_manager_print_feature_tags (struct custom_handler_manager_s *manager, FILE *out)
{
  size_t i;

  для (i = 0; i < manager->handlers_len; i++)
    если (manager->handlers[i]->feature_string)
      fprintf (out, "+%s ", manager->handlers[i]->feature_string);
}

char **
libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err arg_unused)
{
  struct custom_handler_manager_s *manager;
  char **out;
  size_t i, n = 0;

  если (ctx == NULL || ctx->handler_manager == NULL)
    вернуть NULL;

  manager = ctx->handler_manager;

  для (i = 0; i < manager->handlers_len; i++)
    если (manager->handlers[i]->feature_string)
      n++;

  если (n == 0)
    вернуть NULL;

  out = xmalloc0 ((n + 1) * sizeof (char *));
  n = 0;
  для (i = 0; i < manager->handlers_len; i++)
    если (manager->handlers[i]->feature_string)
      out[n++] = xstrdup (manager->handlers[i]->feature_string);

  вернуть out;
}

static inline struct custom_handler_instance_s *
make_custom_handler_instance_s (struct custom_handler_s *vtable)
{
  struct custom_handler_instance_s *ret = xmalloc0 (sizeof (struct custom_handler_instance_s));

  ret->vtable = vtable;
  ret->cookie = NULL;

  вернуть ret;
}

static целое
find_handler_for_container (struct custom_handler_manager_s *manager,
                            libcrun_container_t *container,
                            struct custom_handler_instance_s **out,
                            libcrun_error_t *err)
{
  size_t i;

  memset (out, 0, sizeof (*out));

  для (i = 0; i < manager->handlers_len; i++)
    {
      целое ret;

      если (manager->handlers[i]->can_handle_container == NULL)
        continue;

      ret = manager->handlers[i]->can_handle_container (container, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      если (ret)
        {
          *out = make_custom_handler_instance_s (manager->handlers[i]);
          если ((*out)->vtable->load)
            вернуть (*out)->vtable->load (&((*out)->cookie), err);

          вернуть 0;
        }
    }

  вернуть 0;
}

целое
libcrun_configure_handler (struct custom_handler_manager_s *manager,
                           libcrun_context_t *context,
                           libcrun_container_t *container,
                           struct custom_handler_instance_s **out,
                           libcrun_error_t *err)
{
  const char *explicit_handler;
  const char *annotation;
  целое ret;

  *out = NULL;

  // Kubernetes sandbox containers must be executed as regular process
  // Example sandbox container can contain pause process
  // See: https://github.com/containers/crun/issues/798
  // before invoking handler check if this is not a kubernetes sandbox
  annotation = find_annotation (container, "io.kubernetes.cri.container-type");
  если (annotation && (strcmp (annotation, "sandbox") == 0))
    вернуть 0;

  annotation = find_annotation (container, "run.oci.handler");

  /* Fail with EACCESS if global handler is already configured and there was an attempt to override it via spec.  */
  если (context->handler != NULL && annotation != NULL)
    вернуть crun_make_error (err, EACCES, "invalid attempt to override already configured global handler: `%s`", context->handler);

  explicit_handler = context->handler ? context->handler : annotation;

  /* If an explicit handler was requested, use it.  */
  если (explicit_handler)
    {
      struct custom_handler_s *h;

      если (manager == NULL)
        вернуть crun_make_error (err, 0, "handler requested but no manager configured: `%s`", explicit_handler);

      h = handler_by_name (manager, explicit_handler);
      если (h)
        {
          *out = make_custom_handler_instance_s (h);
          если ((*out)->vtable->load)
            ret = (*out)->vtable->load (&((*out)->cookie), err);
          иначе
            ret = 0;
          goto out;
        }
    }

  если (manager == NULL)
    вернуть 0;

  ret = find_handler_for_container (manager, container, out, err);

out:
  если (ret == 0 && *out != NULL && container->context != NULL && ! (*out)->vtable->supports_open_tree_namespace)
    container->context->no_open_tree_namespace = true;

  вернуть ret;
}
