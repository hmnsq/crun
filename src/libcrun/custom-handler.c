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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ "custom-handler.h"
#অন্তর্ভাৱ "container.h"
#অন্তর্ভাৱ "utils.h"
#অন্তর্ভাৱ "linux.h"
#অন্তর্ভাৱ <unistd.h>
#অন্তর্ভাৱ <sys/stat.h>
#অন্তর্ভাৱ <errno.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#যদি HAVE_DLOPEN && HAVE_LIBKRUN
বাহ্য সংরচনা custom_handler_s handler_libkrun;
#endif
#যদি HAVE_DLOPEN && HAVE_WASMTIME
বাহ্য সংরচনা custom_handler_s handler_wasmtime;
#endif
#যদি HAVE_DLOPEN && HAVE_WASMEDGE
বাহ্য সংরচনা custom_handler_s handler_wasmedge;
#endif
#যদি HAVE_DLOPEN && HAVE_WASMER
বাহ্য সংরচনা custom_handler_s handler_wasmer;
#endif
#যদি HAVE_DLOPEN && HAVE_WAMR
বাহ্য সংরচনা custom_handler_s handler_wamr;
#endif
#যদি HAVE_DLOPEN && HAVE_MONO
বাহ্য সংরচনা custom_handler_s handler_mono;
#endif
#যদি HAVE_DLOPEN && HAVE_SPIN
বাহ্য সংরচনা custom_handler_s handler_spin;
#endif

স্থির সংরচনা custom_handler_s *static_handlers[] = {
#যদি HAVE_DLOPEN && HAVE_LIBKRUN
  &handler_libkrun,
#endif
#যদি HAVE_DLOPEN && HAVE_WASMEDGE
  &handler_wasmedge,
#endif
#যদি HAVE_DLOPEN && HAVE_WASMER
  &handler_wasmer,
#endif
#যদি HAVE_DLOPEN && HAVE_WASMTIME
  &handler_wasmtime,
#endif
#যদি HAVE_DLOPEN && HAVE_WAMR
  &handler_wamr,
#endif
#যদি HAVE_DLOPEN && HAVE_MONO
  &handler_mono,
#endif
#যদি HAVE_DLOPEN && HAVE_SPIN
  &handler_spin,
#endif
  রিক্ত,
};

সংরচনা custom_handler_manager_s
{
  সংরচনা custom_handler_s **handlers;
  শূন্য **handles;
  size_t handlers_len;
};

সংরচনা custom_handler_manager_s *
libcrun_handler_manager_create (libcrun_error_t *err arg_unused)
{
  সংরচনা custom_handler_s **handlers = রিক্ত;
  শূন্য **handles = রিক্ত;
  সংরচনা custom_handler_manager_s *m;
  size_t i, handlers_len;

  /* Count the static handlers.  */
  বাবে (handlers_len = 0; static_handlers[handlers_len]; handlers_len++)
    ;

  যদি (handlers_len)
    {
      handlers = xmalloc (পরিমাণ (সংরচনা custom_handler_s *) * handlers_len);
      handles = xmalloc0 (পরিমাণ (শূন্য *) * handlers_len);
    }

  বাবে (i = 0; i < handlers_len; i++)
    handlers[i] = static_handlers[i];

  m = xmalloc0 (পরিমাণ (সংরচনা custom_handler_manager_s));
  m->handlers = handlers;
  m->handles = handles;
  m->handlers_len = handlers_len;

  ঘূৰাই m;
}

শূন্য
handler_manager_free (সংরচনা custom_handler_manager_s *manager)
{
  size_t i;

  যদি (manager == রিক্ত)
    ঘূৰাই;

  বাবে (i = 0; i < manager->handlers_len; i++)
    {
#ifdef HAVE_DLOPEN
      যদি (manager->handles[i])
        dlclose (manager->handles[i]);
#endif
    }
  মুক্ত (manager->handlers);
  মুক্ত (manager->handles);
  মুক্ত (manager);
}

#ifdef HAVE_DLOPEN
স্থির পূর্ণাঙ্ক
handler_manager_add_so (সংরচনা custom_handler_manager_s *manager, শূন্য *handle, libcrun_error_t *err)
{
  সংরচনা custom_handler_s *h = রিক্ত;
  run_oci_get_handler_cb cb;

  cb = (run_oci_get_handler_cb) dlsym (handle, "run_oci_handler_get_handler");
  যদি (UNLIKELY (cb == রিক্ত))
    ঘূৰাই crun_make_error (err, 0, "cannot find symbol `run_oci_handler_get_handler`");

  h = cb ();
  যদি (UNLIKELY (h == রিক্ত))
    ঘূৰাই crun_make_error (err, 0, "the callback `run_oci_handler_get_handler` didn't return a handler");

  manager->handlers = xrealloc (manager->handlers, পরিমাণ (সংরচনা custom_handler_s *) * (manager->handlers_len + 1));
  manager->handles = xrealloc (manager->handles, পরিমাণ (শূন্য *) * (manager->handlers_len + 1));

  manager->handlers[manager->handlers_len] = h;
  manager->handles[manager->handlers_len] = handle;
  manager->handlers_len++;
  ঘূৰাই 0;
}
#endif

পূর্ণাঙ্ক
libcrun_handler_manager_load_directory (সংরচনা custom_handler_manager_s *manager, ধ্রুৱ ৱর্ণ *path, libcrun_error_t *err)
{
#ifdef HAVE_DLOPEN
  cleanup_dir DIR *dir = রিক্ত;
  সংরচনা dirent *next;

  dir = opendir (path);
  যদি (UNLIKELY (dir == রিক্ত))
    ঘূৰাই crun_make_error (err, errno, "cannot opendir `%s`", path);

  বাবে (next = readdir (dir); next; next = readdir (dir))
    {
      cleanup_free ৱর্ণ *fpath = রিক্ত;
      ধ্রুৱ ৱর্ণ *name;
      শূন্য *handle;
      পূর্ণাঙ্ক ret;

      name = next->d_name;
      যদি (name[0] == '.')
        অনুৱর্ত;

      ret = append_paths (&fpath, err, path, name, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      handle = dlopen (fpath, RTLD_NOW);
      যদি (UNLIKELY (handle == রিক্ত))
        ঘূৰাই crun_make_error (err, 0, "cannot load `%s`: `%s`", fpath, dlerror ());

      ret = handler_manager_add_so (manager, handle, err);
      যদি (UNLIKELY (ret < 0))
        {
          dlclose (handle);
          ঘূৰাই ret;
        }
    }
  ঘূৰাই 0;
#অন্যথা
  (শূন্য) manager;
  (শূন্য) path;
  ঘূৰাই crun_make_error (err, ENOTSUP, "dlopen not available");
#endif
}

সংরচনা custom_handler_s *
handler_by_name (সংরচনা custom_handler_manager_s *manager, ধ্রুৱ ৱর্ণ *name)
{
  size_t i;

  বাবে (i = 0; i < manager->handlers_len; i++)
    {
      যদি (strcmp (manager->handlers[i]->name, name) == 0)
        ঘূৰাই manager->handlers[i];
      যদি (manager->handlers[i]->alias && strcmp (manager->handlers[i]->alias, name) == 0)
        ঘূৰাই manager->handlers[i];
    }
  ঘূৰাই রিক্ত;
}

শূন্য
libcrun_handler_manager_print_feature_tags (সংরচনা custom_handler_manager_s *manager, FILE *out)
{
  size_t i;

  বাবে (i = 0; i < manager->handlers_len; i++)
    যদি (manager->handlers[i]->feature_string)
      fprintf (out, "+%s ", manager->handlers[i]->feature_string);
}

ৱর্ণ **
libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err arg_unused)
{
  সংরচনা custom_handler_manager_s *manager;
  ৱর্ণ **out;
  size_t i, n = 0;

  যদি (ctx == রিক্ত || ctx->handler_manager == রিক্ত)
    ঘূৰাই রিক্ত;

  manager = ctx->handler_manager;

  বাবে (i = 0; i < manager->handlers_len; i++)
    যদি (manager->handlers[i]->feature_string)
      n++;

  যদি (n == 0)
    ঘূৰাই রিক্ত;

  out = xmalloc0 ((n + 1) * পরিমাণ (ৱর্ণ *));
  n = 0;
  বাবে (i = 0; i < manager->handlers_len; i++)
    যদি (manager->handlers[i]->feature_string)
      out[n++] = xstrdup (manager->handlers[i]->feature_string);

  ঘূৰাই out;
}

স্থির অন্তঃস্থ সংরচনা custom_handler_instance_s *
make_custom_handler_instance_s (সংরচনা custom_handler_s *vtable)
{
  সংরচনা custom_handler_instance_s *ret = xmalloc0 (পরিমাণ (সংরচনা custom_handler_instance_s));

  ret->vtable = vtable;
  ret->cookie = রিক্ত;

  ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক
find_handler_for_container (সংরচনা custom_handler_manager_s *manager,
                            libcrun_container_t *container,
                            সংরচনা custom_handler_instance_s **out,
                            libcrun_error_t *err)
{
  size_t i;

  memset (out, 0, পরিমাণ (*out));

  বাবে (i = 0; i < manager->handlers_len; i++)
    {
      পূর্ণাঙ্ক ret;

      যদি (manager->handlers[i]->can_handle_container == রিক্ত)
        অনুৱর্ত;

      ret = manager->handlers[i]->can_handle_container (container, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      যদি (ret)
        {
          *out = make_custom_handler_instance_s (manager->handlers[i]);
          যদি ((*out)->vtable->load)
            ঘূৰাই (*out)->vtable->load (&((*out)->cookie), err);

          ঘূৰাই 0;
        }
    }

  ঘূৰাই 0;
}

পূর্ণাঙ্ক
libcrun_configure_handler (সংরচনা custom_handler_manager_s *manager,
                           libcrun_context_t *context,
                           libcrun_container_t *container,
                           সংরচনা custom_handler_instance_s **out,
                           libcrun_error_t *err)
{
  ধ্রুৱ ৱর্ণ *explicit_handler;
  ধ্রুৱ ৱর্ণ *annotation;
  পূর্ণাঙ্ক ret;

  *out = রিক্ত;

  // Kubernetes sandbox containers must be executed as regular process
  // Example sandbox container can contain pause process
  // See: https://github.com/containers/crun/issues/798
  // before invoking handler check if this is not a kubernetes sandbox
  annotation = find_annotation (container, "io.kubernetes.cri.container-type");
  যদি (annotation && (strcmp (annotation, "sandbox") == 0))
    ঘূৰাই 0;

  annotation = find_annotation (container, "run.oci.handler");

  /* Fail with EACCESS if global handler is already configured and there was an attempt to override it via spec.  */
  যদি (context->handler != রিক্ত && annotation != রিক্ত)
    ঘূৰাই crun_make_error (err, EACCES, "invalid attempt to override already configured global handler: `%s`", context->handler);

  explicit_handler = context->handler ? context->handler : annotation;

  /* If an explicit handler was requested, use it.  */
  যদি (explicit_handler)
    {
      সংরচনা custom_handler_s *h;

      যদি (manager == রিক্ত)
        ঘূৰাই crun_make_error (err, 0, "handler requested but no manager configured: `%s`", explicit_handler);

      h = handler_by_name (manager, explicit_handler);
      যদি (h)
        {
          *out = make_custom_handler_instance_s (h);
          যদি ((*out)->vtable->load)
            ret = (*out)->vtable->load (&((*out)->cookie), err);
          অন্যথা
            ret = 0;
          গচ্ছ out;
        }
    }

  যদি (manager == রিক্ত)
    ঘূৰাই 0;

  ret = find_handler_for_container (manager, container, out, err);

out:
  যদি (ret == 0 && *out != রিক্ত && container->context != রিক্ত && ! (*out)->vtable->supports_open_বৃক্ষ_namespace)
    container->context->no_open_বৃক্ষ_namespace = true;

  ঘূৰাই ret;
}
