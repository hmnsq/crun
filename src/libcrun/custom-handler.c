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

#ضمن <config.h>
#ضمن "custom-handler.h"
#ضمن "container.h"
#ضمن "utils.h"
#ضمن "linux.h"
#ضمن <unistd.h>
#ضمن <sys/stat.h>
#ضمن <errno.h>
#ضمن <sys/types.h>
#ضمن <fcntl.h>
#ضمن <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#אם HAVE_DLOPEN && HAVE_LIBKRUN
خارجي بنية custom_handler_s handler_libkrun;
#endif
#אם HAVE_DLOPEN && HAVE_WASMTIME
خارجي بنية custom_handler_s handler_wasmtime;
#endif
#אם HAVE_DLOPEN && HAVE_WASMEDGE
خارجي بنية custom_handler_s handler_wasmedge;
#endif
#אם HAVE_DLOPEN && HAVE_WASMER
خارجي بنية custom_handler_s handler_wasmer;
#endif
#אם HAVE_DLOPEN && HAVE_WAMR
خارجي بنية custom_handler_s handler_wamr;
#endif
#אם HAVE_DLOPEN && HAVE_MONO
خارجي بنية custom_handler_s handler_mono;
#endif
#אם HAVE_DLOPEN && HAVE_SPIN
خارجي بنية custom_handler_s handler_spin;
#endif

ساكن بنية custom_handler_s *static_handlers[] = {
#אם HAVE_DLOPEN && HAVE_LIBKRUN
  &handler_libkrun,
#endif
#אם HAVE_DLOPEN && HAVE_WASMEDGE
  &handler_wasmedge,
#endif
#אם HAVE_DLOPEN && HAVE_WASMER
  &handler_wasmer,
#endif
#אם HAVE_DLOPEN && HAVE_WASMTIME
  &handler_wasmtime,
#endif
#אם HAVE_DLOPEN && HAVE_WAMR
  &handler_wamr,
#endif
#אם HAVE_DLOPEN && HAVE_MONO
  &handler_mono,
#endif
#אם HAVE_DLOPEN && HAVE_SPIN
  &handler_spin,
#endif
  عدم,
};

بنية custom_handler_manager_s
{
  بنية custom_handler_s **handlers;
  فراغ **handles;
  size_t handlers_len;
};

بنية custom_handler_manager_s *
libcrun_handler_manager_create (libcrun_error_t *err arg_unused)
{
  بنية custom_handler_s **handlers = عدم;
  فراغ **handles = عدم;
  بنية custom_handler_manager_s *m;
  size_t i, handlers_len;

  /* Count the static handlers.  */
  עבור (handlers_len = 0; static_handlers[handlers_len]; handlers_len++)
    ;

  אם (handlers_len)
    {
      handlers = xmalloc (حجم (بنية custom_handler_s *) * handlers_len);
      handles = xmalloc0 (حجم (فراغ *) * handlers_len);
    }

  עבור (i = 0; i < handlers_len; i++)
    handlers[i] = static_handlers[i];

  m = xmalloc0 (حجم (بنية custom_handler_manager_s));
  m->handlers = handlers;
  m->handles = handles;
  m->handlers_len = handlers_len;

  החזר m;
}

فراغ
handler_manager_free (بنية custom_handler_manager_s *manager)
{
  size_t i;

  אם (manager == عدم)
    החזר;

  עבור (i = 0; i < manager->handlers_len; i++)
    {
#ifdef HAVE_DLOPEN
      אם (manager->handles[i])
        dlclose (manager->handles[i]);
#endif
    }
  حرر (manager->handlers);
  حرر (manager->handles);
  حرر (manager);
}

#ifdef HAVE_DLOPEN
ساكن صحيح
handler_manager_add_so (بنية custom_handler_manager_s *manager, فراغ *handle, libcrun_error_t *err)
{
  بنية custom_handler_s *h = عدم;
  run_oci_get_handler_cb cb;

  cb = (run_oci_get_handler_cb) dlsym (handle, "run_oci_handler_get_handler");
  אם (UNLIKELY (cb == عدم))
    החזר crun_make_error (err, 0, "cannot find symbol `run_oci_handler_get_handler`");

  h = cb ();
  אם (UNLIKELY (h == عدم))
    החזר crun_make_error (err, 0, "the callback `run_oci_handler_get_handler` didn't return a handler");

  manager->handlers = xrealloc (manager->handlers, حجم (بنية custom_handler_s *) * (manager->handlers_len + 1));
  manager->handles = xrealloc (manager->handles, حجم (فراغ *) * (manager->handlers_len + 1));

  manager->handlers[manager->handlers_len] = h;
  manager->handles[manager->handlers_len] = handle;
  manager->handlers_len++;
  החזר 0;
}
#endif

صحيح
libcrun_handler_manager_load_directory (بنية custom_handler_manager_s *manager, ثابت حرف *path, libcrun_error_t *err)
{
#ifdef HAVE_DLOPEN
  cleanup_dir DIR *dir = عدم;
  بنية dirent *next;

  dir = opendir (path);
  אם (UNLIKELY (dir == عدم))
    החזר crun_make_error (err, errno, "cannot opendir `%s`", path);

  עבור (next = readdir (dir); next; next = readdir (dir))
    {
      cleanup_free حرف *fpath = عدم;
      ثابت حرف *name;
      فراغ *handle;
      صحيح ret;

      name = next->d_name;
      אם (name[0] == '.')
        استمر;

      ret = append_paths (&fpath, err, path, name, عدم);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      handle = dlopen (fpath, RTLD_NOW);
      אם (UNLIKELY (handle == عدم))
        החזר crun_make_error (err, 0, "cannot load `%s`: `%s`", fpath, dlerror ());

      ret = handler_manager_add_so (manager, handle, err);
      אם (UNLIKELY (ret < 0))
        {
          dlclose (handle);
          החזר ret;
        }
    }
  החזר 0;
#אחרת
  (فراغ) manager;
  (فراغ) path;
  החזר crun_make_error (err, ENOTSUP, "dlopen not available");
#endif
}

بنية custom_handler_s *
handler_by_name (بنية custom_handler_manager_s *manager, ثابت حرف *name)
{
  size_t i;

  עבור (i = 0; i < manager->handlers_len; i++)
    {
      אם (strcmp (manager->handlers[i]->name, name) == 0)
        החזר manager->handlers[i];
      אם (manager->handlers[i]->alias && strcmp (manager->handlers[i]->alias, name) == 0)
        החזר manager->handlers[i];
    }
  החזר عدم;
}

فراغ
libcrun_handler_manager_print_feature_tags (بنية custom_handler_manager_s *manager, FILE *out)
{
  size_t i;

  עבור (i = 0; i < manager->handlers_len; i++)
    אם (manager->handlers[i]->feature_string)
      fprintf (out, "+%s ", manager->handlers[i]->feature_string);
}

حرف **
libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err arg_unused)
{
  بنية custom_handler_manager_s *manager;
  حرف **out;
  size_t i, n = 0;

  אם (ctx == عدم || ctx->handler_manager == عدم)
    החזר عدم;

  manager = ctx->handler_manager;

  עבור (i = 0; i < manager->handlers_len; i++)
    אם (manager->handlers[i]->feature_string)
      n++;

  אם (n == 0)
    החזר عدم;

  out = xmalloc0 ((n + 1) * حجم (حرف *));
  n = 0;
  עבור (i = 0; i < manager->handlers_len; i++)
    אם (manager->handlers[i]->feature_string)
      out[n++] = xstrdup (manager->handlers[i]->feature_string);

  החזר out;
}

ساكن مضمن بنية custom_handler_instance_s *
make_custom_handler_instance_s (بنية custom_handler_s *vtable)
{
  بنية custom_handler_instance_s *ret = xmalloc0 (حجم (بنية custom_handler_instance_s));

  ret->vtable = vtable;
  ret->cookie = عدم;

  החזר ret;
}

ساكن صحيح
find_handler_for_container (بنية custom_handler_manager_s *manager,
                            libcrun_container_t *container,
                            بنية custom_handler_instance_s **out,
                            libcrun_error_t *err)
{
  size_t i;

  memset (out, 0, حجم (*out));

  עבור (i = 0; i < manager->handlers_len; i++)
    {
      صحيح ret;

      אם (manager->handlers[i]->can_handle_container == عدم)
        استمر;

      ret = manager->handlers[i]->can_handle_container (container, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      אם (ret)
        {
          *out = make_custom_handler_instance_s (manager->handlers[i]);
          אם ((*out)->vtable->load)
            החזר (*out)->vtable->load (&((*out)->cookie), err);

          החזר 0;
        }
    }

  החזר 0;
}

صحيح
libcrun_configure_handler (بنية custom_handler_manager_s *manager,
                           libcrun_context_t *context,
                           libcrun_container_t *container,
                           بنية custom_handler_instance_s **out,
                           libcrun_error_t *err)
{
  ثابت حرف *explicit_handler;
  ثابت حرف *annotation;
  صحيح ret;

  *out = عدم;

  // Kubernetes sandbox containers must be executed as regular process
  // Example sandbox container can contain pause process
  // See: https://github.com/containers/crun/issues/798
  // before invoking handler check if this is not a kubernetes sandbox
  annotation = find_annotation (container, "io.kubernetes.cri.container-type");
  אם (annotation && (strcmp (annotation, "sandbox") == 0))
    החזר 0;

  annotation = find_annotation (container, "run.oci.handler");

  /* Fail with EACCESS if global handler is already configured and there was an attempt to override it via spec.  */
  אם (context->handler != عدم && annotation != عدم)
    החזר crun_make_error (err, EACCES, "invalid attempt to override already configured global handler: `%s`", context->handler);

  explicit_handler = context->handler ? context->handler : annotation;

  /* If an explicit handler was requested, use it.  */
  אם (explicit_handler)
    {
      بنية custom_handler_s *h;

      אם (manager == عدم)
        החזר crun_make_error (err, 0, "handler requested but no manager configured: `%s`", explicit_handler);

      h = handler_by_name (manager, explicit_handler);
      אם (h)
        {
          *out = make_custom_handler_instance_s (h);
          אם ((*out)->vtable->load)
            ret = (*out)->vtable->load (&((*out)->cookie), err);
          אחרת
            ret = 0;
          اذهب out;
        }
    }

  אם (manager == عدم)
    החזר 0;

  ret = find_handler_for_container (manager, container, out, err);

out:
  אם (ret == 0 && *out != عدم && container->context != عدم && ! (*out)->vtable->supports_open_tree_namespace)
    container->context->no_open_tree_namespace = true;

  החזר ret;
}
