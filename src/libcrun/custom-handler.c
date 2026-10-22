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
#अन्तर्भाव "custom-handler.h"
#अन्तर्भाव "container.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "linux.h"
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#सचे HAVE_DLOPEN && HAVE_LIBKRUN
बाह्य संरचना custom_handler_s handler_libkrun;
#endif
#सचे HAVE_DLOPEN && HAVE_WASMTIME
बाह्य संरचना custom_handler_s handler_wasmtime;
#endif
#सचे HAVE_DLOPEN && HAVE_WASMEDGE
बाह्य संरचना custom_handler_s handler_wasmedge;
#endif
#सचे HAVE_DLOPEN && HAVE_WASMER
बाह्य संरचना custom_handler_s handler_wasmer;
#endif
#सचे HAVE_DLOPEN && HAVE_WAMR
बाह्य संरचना custom_handler_s handler_wamr;
#endif
#सचे HAVE_DLOPEN && HAVE_MONO
बाह्य संरचना custom_handler_s handler_mono;
#endif
#सचे HAVE_DLOPEN && HAVE_SPIN
बाह्य संरचना custom_handler_s handler_spin;
#endif

स्थिर संरचना custom_handler_s *static_handlers[] = {
#सचे HAVE_DLOPEN && HAVE_LIBKRUN
  &handler_libkrun,
#endif
#सचे HAVE_DLOPEN && HAVE_WASMEDGE
  &handler_wasmedge,
#endif
#सचे HAVE_DLOPEN && HAVE_WASMER
  &handler_wasmer,
#endif
#सचे HAVE_DLOPEN && HAVE_WASMTIME
  &handler_wasmtime,
#endif
#सचे HAVE_DLOPEN && HAVE_WAMR
  &handler_wamr,
#endif
#सचे HAVE_DLOPEN && HAVE_MONO
  &handler_mono,
#endif
#सचे HAVE_DLOPEN && HAVE_SPIN
  &handler_spin,
#endif
  रिक्त,
};

संरचना custom_handler_manager_s
{
  संरचना custom_handler_s **handlers;
  शून्य **handles;
  size_t handlers_len;
};

संरचना custom_handler_manager_s *
libcrun_handler_manager_create (libcrun_error_t *err arg_unused)
{
  संरचना custom_handler_s **handlers = रिक्त;
  शून्य **handles = रिक्त;
  संरचना custom_handler_manager_s *m;
  size_t i, handlers_len;

  /* Count the static handlers.  */
  अत्थाय (handlers_len = 0; static_handlers[handlers_len]; handlers_len++)
    ;

  सचे (handlers_len)
    {
      handlers = xmalloc (परिमाण (संरचना custom_handler_s *) * handlers_len);
      handles = xmalloc0 (परिमाण (शून्य *) * handlers_len);
    }

  अत्थाय (i = 0; i < handlers_len; i++)
    handlers[i] = static_handlers[i];

  m = xmalloc0 (परिमाण (संरचना custom_handler_manager_s));
  m->handlers = handlers;
  m->handles = handles;
  m->handlers_len = handlers_len;

  पटिनिवत्त m;
}

शून्य
handler_manager_free (संरचना custom_handler_manager_s *manager)
{
  size_t i;

  सचे (manager == रिक्त)
    पटिनिवत्त;

  अत्थाय (i = 0; i < manager->handlers_len; i++)
    {
#ifdef HAVE_DLOPEN
      सचे (manager->handles[i])
        dlclose (manager->handles[i]);
#endif
    }
  मुक्त (manager->handlers);
  मुक्त (manager->handles);
  मुक्त (manager);
}

#ifdef HAVE_DLOPEN
स्थिर पूर्णाङ्क
handler_manager_add_so (संरचना custom_handler_manager_s *manager, शून्य *handle, libcrun_error_t *err)
{
  संरचना custom_handler_s *h = रिक्त;
  run_oci_get_handler_cb cb;

  cb = (run_oci_get_handler_cb) dlsym (handle, "run_oci_handler_get_handler");
  सचे (UNLIKELY (cb == रिक्त))
    पटिनिवत्त crun_make_error (err, 0, "cannot find symbol `run_oci_handler_get_handler`");

  h = cb ();
  सचे (UNLIKELY (h == रिक्त))
    पटिनिवत्त crun_make_error (err, 0, "the callback `run_oci_handler_get_handler` didn't return a handler");

  manager->handlers = xrealloc (manager->handlers, परिमाण (संरचना custom_handler_s *) * (manager->handlers_len + 1));
  manager->handles = xrealloc (manager->handles, परिमाण (शून्य *) * (manager->handlers_len + 1));

  manager->handlers[manager->handlers_len] = h;
  manager->handles[manager->handlers_len] = handle;
  manager->handlers_len++;
  पटिनिवत्त 0;
}
#endif

पूर्णाङ्क
libcrun_handler_manager_load_directory (संरचना custom_handler_manager_s *manager, ध्रुव वर्ण *path, libcrun_error_t *err)
{
#ifdef HAVE_DLOPEN
  cleanup_dir DIR *dir = रिक्त;
  संरचना dirent *next;

  dir = opendir (path);
  सचे (UNLIKELY (dir == रिक्त))
    पटिनिवत्त crun_make_error (err, errno, "cannot opendir `%s`", path);

  अत्थाय (next = readdir (dir); next; next = readdir (dir))
    {
      cleanup_free वर्ण *fpath = रिक्त;
      ध्रुव वर्ण *name;
      शून्य *handle;
      पूर्णाङ्क ret;

      name = next->d_name;
      सचे (name[0] == '.')
        अनुवर्त;

      ret = append_paths (&fpath, err, path, name, रिक्त);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      handle = dlopen (fpath, RTLD_NOW);
      सचे (UNLIKELY (handle == रिक्त))
        पटिनिवत्त crun_make_error (err, 0, "cannot load `%s`: `%s`", fpath, dlerror ());

      ret = handler_manager_add_so (manager, handle, err);
      सचे (UNLIKELY (ret < 0))
        {
          dlclose (handle);
          पटिनिवत्त ret;
        }
    }
  पटिनिवत्त 0;
#अञ्ञथा
  (शून्य) manager;
  (शून्य) path;
  पटिनिवत्त crun_make_error (err, ENOTSUP, "dlopen not available");
#endif
}

संरचना custom_handler_s *
handler_by_name (संरचना custom_handler_manager_s *manager, ध्रुव वर्ण *name)
{
  size_t i;

  अत्थाय (i = 0; i < manager->handlers_len; i++)
    {
      सचे (strcmp (manager->handlers[i]->name, name) == 0)
        पटिनिवत्त manager->handlers[i];
      सचे (manager->handlers[i]->alias && strcmp (manager->handlers[i]->alias, name) == 0)
        पटिनिवत्त manager->handlers[i];
    }
  पटिनिवत्त रिक्त;
}

शून्य
libcrun_handler_manager_print_feature_tags (संरचना custom_handler_manager_s *manager, FILE *out)
{
  size_t i;

  अत्थाय (i = 0; i < manager->handlers_len; i++)
    सचे (manager->handlers[i]->feature_string)
      fprintf (out, "+%s ", manager->handlers[i]->feature_string);
}

वर्ण **
libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err arg_unused)
{
  संरचना custom_handler_manager_s *manager;
  वर्ण **out;
  size_t i, n = 0;

  सचे (ctx == रिक्त || ctx->handler_manager == रिक्त)
    पटिनिवत्त रिक्त;

  manager = ctx->handler_manager;

  अत्थाय (i = 0; i < manager->handlers_len; i++)
    सचे (manager->handlers[i]->feature_string)
      n++;

  सचे (n == 0)
    पटिनिवत्त रिक्त;

  out = xmalloc0 ((n + 1) * परिमाण (वर्ण *));
  n = 0;
  अत्थाय (i = 0; i < manager->handlers_len; i++)
    सचे (manager->handlers[i]->feature_string)
      out[n++] = xstrdup (manager->handlers[i]->feature_string);

  पटिनिवत्त out;
}

स्थिर अन्तःस्थ संरचना custom_handler_instance_s *
make_custom_handler_instance_s (संरचना custom_handler_s *vtable)
{
  संरचना custom_handler_instance_s *ret = xmalloc0 (परिमाण (संरचना custom_handler_instance_s));

  ret->vtable = vtable;
  ret->cookie = रिक्त;

  पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क
find_handler_for_container (संरचना custom_handler_manager_s *manager,
                            libcrun_container_t *container,
                            संरचना custom_handler_instance_s **out,
                            libcrun_error_t *err)
{
  size_t i;

  memset (out, 0, परिमाण (*out));

  अत्थाय (i = 0; i < manager->handlers_len; i++)
    {
      पूर्णाङ्क ret;

      सचे (manager->handlers[i]->can_handle_container == रिक्त)
        अनुवर्त;

      ret = manager->handlers[i]->can_handle_container (container, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      सचे (ret)
        {
          *out = make_custom_handler_instance_s (manager->handlers[i]);
          सचे ((*out)->vtable->load)
            पटिनिवत्त (*out)->vtable->load (&((*out)->cookie), err);

          पटिनिवत्त 0;
        }
    }

  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_configure_handler (संरचना custom_handler_manager_s *manager,
                           libcrun_context_t *context,
                           libcrun_container_t *container,
                           संरचना custom_handler_instance_s **out,
                           libcrun_error_t *err)
{
  ध्रुव वर्ण *explicit_handler;
  ध्रुव वर्ण *annotation;
  पूर्णाङ्क ret;

  *out = रिक्त;

  // Kubernetes sandbox containers must be executed as regular process
  // Example sandbox container can contain pause process
  // See: https://github.com/containers/crun/issues/798
  // before invoking handler check if this is not a kubernetes sandbox
  annotation = find_annotation (container, "io.kubernetes.cri.container-type");
  सचे (annotation && (strcmp (annotation, "sandbox") == 0))
    पटिनिवत्त 0;

  annotation = find_annotation (container, "run.oci.handler");

  /* Fail with EACCESS if global handler is already configured and there was an attempt to override it via spec.  */
  सचे (context->handler != रिक्त && annotation != रिक्त)
    पटिनिवत्त crun_make_error (err, EACCES, "invalid attempt to override already configured global handler: `%s`", context->handler);

  explicit_handler = context->handler ? context->handler : annotation;

  /* If an explicit handler was requested, use it.  */
  सचे (explicit_handler)
    {
      संरचना custom_handler_s *h;

      सचे (manager == रिक्त)
        पटिनिवत्त crun_make_error (err, 0, "handler requested but no manager configured: `%s`", explicit_handler);

      h = handler_by_name (manager, explicit_handler);
      सचे (h)
        {
          *out = make_custom_handler_instance_s (h);
          सचे ((*out)->vtable->load)
            ret = (*out)->vtable->load (&((*out)->cookie), err);
          अञ्ञथा
            ret = 0;
          गच्छ out;
        }
    }

  सचे (manager == रिक्त)
    पटिनिवत्त 0;

  ret = find_handler_for_container (manager, container, out, err);

out:
  सचे (ret == 0 && *out != रिक्त && container->context != रिक्त && ! (*out)->vtable->supports_open_वृक्षः_namespace)
    container->context->no_open_वृक्षः_namespace = true;

  पटिनिवत्त ret;
}
