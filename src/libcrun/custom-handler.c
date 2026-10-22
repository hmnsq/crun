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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "custom-handler.h"
#অন্তর্ভাব "container.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "linux.h"
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#যদি HAVE_DLOPEN && HAVE_LIBKRUN
বাহ্যিক কাঠামো custom_handler_s handler_libkrun;
#endif
#যদি HAVE_DLOPEN && HAVE_WASMTIME
বাহ্যিক কাঠামো custom_handler_s handler_wasmtime;
#endif
#যদি HAVE_DLOPEN && HAVE_WASMEDGE
বাহ্যিক কাঠামো custom_handler_s handler_wasmedge;
#endif
#যদি HAVE_DLOPEN && HAVE_WASMER
বাহ্যিক কাঠামো custom_handler_s handler_wasmer;
#endif
#যদি HAVE_DLOPEN && HAVE_WAMR
বাহ্যিক কাঠামো custom_handler_s handler_wamr;
#endif
#যদি HAVE_DLOPEN && HAVE_MONO
বাহ্যিক কাঠামো custom_handler_s handler_mono;
#endif
#যদি HAVE_DLOPEN && HAVE_SPIN
বাহ্যিক কাঠামো custom_handler_s handler_spin;
#endif

স্থির কাঠামো custom_handler_s *static_handlers[] = {
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
  ফাঁকা,
};

কাঠামো custom_handler_manager_s
{
  কাঠামো custom_handler_s **handlers;
  শূন্য **handles;
  size_t handlers_len;
};

কাঠামো custom_handler_manager_s *
libcrun_handler_manager_create (libcrun_error_t *err arg_unused)
{
  কাঠামো custom_handler_s **handlers = ফাঁকা;
  শূন্য **handles = ফাঁকা;
  কাঠামো custom_handler_manager_s *m;
  size_t i, handlers_len;

  /* Count the static handlers.  */
  জন্য (handlers_len = 0; static_handlers[handlers_len]; handlers_len++)
    ;

  যদি (handlers_len)
    {
      handlers = xmalloc (আকার (কাঠামো custom_handler_s *) * handlers_len);
      handles = xmalloc0 (আকার (শূন্য *) * handlers_len);
    }

  জন্য (i = 0; i < handlers_len; i++)
    handlers[i] = static_handlers[i];

  m = xmalloc0 (আকার (কাঠামো custom_handler_manager_s));
  m->handlers = handlers;
  m->handles = handles;
  m->handlers_len = handlers_len;

  ফেরত m;
}

শূন্য
handler_manager_free (কাঠামো custom_handler_manager_s *manager)
{
  size_t i;

  যদি (manager == ফাঁকা)
    ফেরত;

  জন্য (i = 0; i < manager->handlers_len; i++)
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
স্থির পূর্ণসংখ্যা
handler_manager_add_so (কাঠামো custom_handler_manager_s *manager, শূন্য *handle, libcrun_error_t *err)
{
  কাঠামো custom_handler_s *h = ফাঁকা;
  run_oci_get_handler_cb cb;

  cb = (run_oci_get_handler_cb) dlsym (handle, "run_oci_handler_get_handler");
  যদি (UNLIKELY (cb == ফাঁকা))
    ফেরত crun_make_error (err, 0, "cannot find symbol `run_oci_handler_get_handler`");

  h = cb ();
  যদি (UNLIKELY (h == ফাঁকা))
    ফেরত crun_make_error (err, 0, "the callback `run_oci_handler_get_handler` didn't return a handler");

  manager->handlers = xrealloc (manager->handlers, আকার (কাঠামো custom_handler_s *) * (manager->handlers_len + 1));
  manager->handles = xrealloc (manager->handles, আকার (শূন্য *) * (manager->handlers_len + 1));

  manager->handlers[manager->handlers_len] = h;
  manager->handles[manager->handlers_len] = handle;
  manager->handlers_len++;
  ফেরত 0;
}
#endif

পূর্ণসংখ্যা
libcrun_handler_manager_load_directory (কাঠামো custom_handler_manager_s *manager, ধ্রুবক অক্ষর *path, libcrun_error_t *err)
{
#ifdef HAVE_DLOPEN
  cleanup_dir DIR *dir = ফাঁকা;
  কাঠামো dirent *পরবর্তী;

  dir = opendir (path);
  যদি (UNLIKELY (dir == ফাঁকা))
    ফেরত crun_make_error (err, errno, "cannot opendir `%s`", path);

  জন্য (পরবর্তী = readdir (dir); পরবর্তী; পরবর্তী = readdir (dir))
    {
      cleanup_free অক্ষর *fpath = ফাঁকা;
      ধ্রুবক অক্ষর *name;
      শূন্য *handle;
      পূর্ণসংখ্যা ret;

      name = পরবর্তী->d_name;
      যদি (name[0] == '.')
        চালু;

      ret = append_paths (&fpath, err, path, name, ফাঁকা);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      handle = dlopen (fpath, RTLD_NOW);
      যদি (UNLIKELY (handle == ফাঁকা))
        ফেরত crun_make_error (err, 0, "cannot load `%s`: `%s`", fpath, dlerror ());

      ret = handler_manager_add_so (manager, handle, err);
      যদি (UNLIKELY (ret < 0))
        {
          dlclose (handle);
          ফেরত ret;
        }
    }
  ফেরত 0;
#অন্যথা
  (শূন্য) manager;
  (শূন্য) path;
  ফেরত crun_make_error (err, ENOTSUP, "dlopen not available");
#endif
}

কাঠামো custom_handler_s *
handler_by_name (কাঠামো custom_handler_manager_s *manager, ধ্রুবক অক্ষর *name)
{
  size_t i;

  জন্য (i = 0; i < manager->handlers_len; i++)
    {
      যদি (strcmp (manager->handlers[i]->name, name) == 0)
        ফেরত manager->handlers[i];
      যদি (manager->handlers[i]->alias && strcmp (manager->handlers[i]->alias, name) == 0)
        ফেরত manager->handlers[i];
    }
  ফেরত ফাঁকা;
}

শূন্য
libcrun_handler_manager_print_feature_tags (কাঠামো custom_handler_manager_s *manager, FILE *out)
{
  size_t i;

  জন্য (i = 0; i < manager->handlers_len; i++)
    যদি (manager->handlers[i]->feature_string)
      fprintf (out, "+%s ", manager->handlers[i]->feature_string);
}

অক্ষর **
libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err arg_unused)
{
  কাঠামো custom_handler_manager_s *manager;
  অক্ষর **out;
  size_t i, n = 0;

  যদি (ctx == ফাঁকা || ctx->handler_manager == ফাঁকা)
    ফেরত ফাঁকা;

  manager = ctx->handler_manager;

  জন্য (i = 0; i < manager->handlers_len; i++)
    যদি (manager->handlers[i]->feature_string)
      n++;

  যদি (n == 0)
    ফেরত ফাঁকা;

  out = xmalloc0 ((n + 1) * আকার (অক্ষর *));
  n = 0;
  জন্য (i = 0; i < manager->handlers_len; i++)
    যদি (manager->handlers[i]->feature_string)
      out[n++] = xstrdup (manager->handlers[i]->feature_string);

  ফেরত out;
}

স্থির অন্তঃরেখ কাঠামো custom_handler_instance_s *
make_custom_handler_instance_s (কাঠামো custom_handler_s *vtable)
{
  কাঠামো custom_handler_instance_s *ret = xmalloc0 (আকার (কাঠামো custom_handler_instance_s));

  ret->vtable = vtable;
  ret->cookie = ফাঁকা;

  ফেরত ret;
}

স্থির পূর্ণসংখ্যা
find_handler_for_container (কাঠামো custom_handler_manager_s *manager,
                            libcrun_container_t *container,
                            কাঠামো custom_handler_instance_s **out,
                            libcrun_error_t *err)
{
  size_t i;

  memset (out, 0, আকার (*out));

  জন্য (i = 0; i < manager->handlers_len; i++)
    {
      পূর্ণসংখ্যা ret;

      যদি (manager->handlers[i]->can_handle_container == ফাঁকা)
        চালু;

      ret = manager->handlers[i]->can_handle_container (container, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      যদি (ret)
        {
          *out = make_custom_handler_instance_s (manager->handlers[i]);
          যদি ((*out)->vtable->load)
            ফেরত (*out)->vtable->load (&((*out)->cookie), err);

          ফেরত 0;
        }
    }

  ফেরত 0;
}

পূর্ণসংখ্যা
libcrun_configure_handler (কাঠামো custom_handler_manager_s *manager,
                           libcrun_context_t *context,
                           libcrun_container_t *container,
                           কাঠামো custom_handler_instance_s **out,
                           libcrun_error_t *err)
{
  ধ্রুবক অক্ষর *explicit_handler;
  ধ্রুবক অক্ষর *annotation;
  পূর্ণসংখ্যা ret;

  *out = ফাঁকা;

  // Kubernetes sandbox containers must be executed as regular process
  // Example sandbox container can contain pause process
  // See: https://github.com/containers/crun/issues/798
  // before invoking handler check if this is not a kubernetes sandbox
  annotation = find_annotation (container, "io.kubernetes.cri.container-type");
  যদি (annotation && (strcmp (annotation, "sandbox") == 0))
    ফেরত 0;

  annotation = find_annotation (container, "run.oci.handler");

  /* Fail with EACCESS if global handler is already configured and there was an attempt to override it via spec.  */
  যদি (context->handler != ফাঁকা && annotation != ফাঁকা)
    ফেরত crun_make_error (err, EACCES, "invalid attempt to override already configured global handler: `%s`", context->handler);

  explicit_handler = context->handler ? context->handler : annotation;

  /* If an explicit handler was requested, use it.  */
  যদি (explicit_handler)
    {
      কাঠামো custom_handler_s *h;

      যদি (manager == ফাঁকা)
        ফেরত crun_make_error (err, 0, "handler requested but no manager configured: `%s`", explicit_handler);

      h = handler_by_name (manager, explicit_handler);
      যদি (h)
        {
          *out = make_custom_handler_instance_s (h);
          যদি ((*out)->vtable->load)
            ret = (*out)->vtable->load (&((*out)->cookie), err);
          অন্যথা
            ret = 0;
          যাও out;
        }
    }

  যদি (manager == ফাঁকা)
    ফেরত 0;

  ret = find_handler_for_container (manager, container, out, err);

out:
  যদি (ret == 0 && *out != ফাঁকা && container->context != ফাঁকা && ! (*out)->vtable->supports_open_বৃক্ষ_namespace)
    container->context->no_open_বৃক্ষ_namespace = true;

  ফেরত ret;
}
