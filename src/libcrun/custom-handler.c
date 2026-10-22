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

#जर HAVE_DLOPEN && HAVE_LIBKRUN
बाह्य संरचना custom_handler_s handler_libkrun;
#endif
#जर HAVE_DLOPEN && HAVE_WASMTIME
बाह्य संरचना custom_handler_s handler_wasmtime;
#endif
#जर HAVE_DLOPEN && HAVE_WASMEDGE
बाह्य संरचना custom_handler_s handler_wasmedge;
#endif
#जर HAVE_DLOPEN && HAVE_WASMER
बाह्य संरचना custom_handler_s handler_wasmer;
#endif
#जर HAVE_DLOPEN && HAVE_WAMR
बाह्य संरचना custom_handler_s handler_wamr;
#endif
#जर HAVE_DLOPEN && HAVE_MONO
बाह्य संरचना custom_handler_s handler_mono;
#endif
#जर HAVE_DLOPEN && HAVE_SPIN
बाह्य संरचना custom_handler_s handler_spin;
#endif

स्थिर संरचना custom_handler_s *static_handlers[] = {
#जर HAVE_DLOPEN && HAVE_LIBKRUN
  &handler_libkrun,
#endif
#जर HAVE_DLOPEN && HAVE_WASMEDGE
  &handler_wasmedge,
#endif
#जर HAVE_DLOPEN && HAVE_WASMER
  &handler_wasmer,
#endif
#जर HAVE_DLOPEN && HAVE_WASMTIME
  &handler_wasmtime,
#endif
#जर HAVE_DLOPEN && HAVE_WAMR
  &handler_wamr,
#endif
#जर HAVE_DLOPEN && HAVE_MONO
  &handler_mono,
#endif
#जर HAVE_DLOPEN && HAVE_SPIN
  &handler_spin,
#endif
  रिक्त,
};

संरचना custom_handler_manager_s
{
  संरचना custom_handler_s **handlers;
  शून्य **handles;
  माप_प्रकार handlers_len;
};

संरचना custom_handler_manager_s *
libcrun_handler_manager_create (libcrun_error_t *err arg_unused)
{
  संरचना custom_handler_s **handlers = रिक्त;
  शून्य **handles = रिक्त;
  संरचना custom_handler_manager_s *m;
  माप_प्रकार i, handlers_len;

  /* Count the static handlers.  */
  साठी (handlers_len = 0; static_handlers[handlers_len]; handlers_len++)
    ;

  जर (handlers_len)
    {
      handlers = xmalloc (आकार (संरचना custom_handler_s *) * handlers_len);
      handles = xmalloc0 (आकार (शून्य *) * handlers_len);
    }

  साठी (i = 0; i < handlers_len; i++)
    handlers[i] = static_handlers[i];

  m = xmalloc0 (आकार (संरचना custom_handler_manager_s));
  m->handlers = handlers;
  m->handles = handles;
  m->handlers_len = handlers_len;

  परत m;
}

शून्य
handler_manager_free (संरचना custom_handler_manager_s *manager)
{
  माप_प्रकार i;

  जर (manager == रिक्त)
    परत;

  साठी (i = 0; i < manager->handlers_len; i++)
    {
#ifdef HAVE_DLOPEN
      जर (manager->handles[i])
        dlclose (manager->handles[i]);
#endif
    }
  मुक्त (manager->handlers);
  मुक्त (manager->handles);
  मुक्त (manager);
}

#ifdef HAVE_DLOPEN
स्थिर पूर्णांक
handler_manager_add_so (संरचना custom_handler_manager_s *manager, शून्य *handle, libcrun_error_t *err)
{
  संरचना custom_handler_s *h = रिक्त;
  run_oci_get_handler_cb cb;

  cb = (run_oci_get_handler_cb) dlsym (handle, "run_oci_handler_get_handler");
  जर (UNLIKELY (cb == रिक्त))
    परत crun_make_error (err, 0, "cannot find symbol `run_oci_handler_get_handler`");

  h = cb ();
  जर (UNLIKELY (h == रिक्त))
    परत crun_make_error (err, 0, "the callback `run_oci_handler_get_handler` didn't return a handler");

  manager->handlers = xrealloc (manager->handlers, आकार (संरचना custom_handler_s *) * (manager->handlers_len + 1));
  manager->handles = xrealloc (manager->handles, आकार (शून्य *) * (manager->handlers_len + 1));

  manager->handlers[manager->handlers_len] = h;
  manager->handles[manager->handlers_len] = handle;
  manager->handlers_len++;
  परत 0;
}
#endif

पूर्णांक
libcrun_handler_manager_load_directory (संरचना custom_handler_manager_s *manager, स्थिरांक अक्षर *path, libcrun_error_t *err)
{
#ifdef HAVE_DLOPEN
  cleanup_dir सूची *dir = रिक्त;
  संरचना dirent *next;

  dir = सूची_खोलो (path);
  जर (UNLIKELY (dir == रिक्त))
    परत crun_make_error (err, त्रुटि_सं, "cannot opendir `%s`", path);

  साठी (next = सूची_पढ़ो (dir); next; next = सूची_पढ़ो (dir))
    {
      cleanup_free अक्षर *fpath = रिक्त;
      स्थिरांक अक्षर *name;
      शून्य *handle;
      पूर्णांक ret;

      name = next->d_name;
      जर (name[0] == '.')
        सुरूठेवा;

      ret = append_paths (&fpath, err, path, name, रिक्त);
      जर (UNLIKELY (ret < 0))
        परत ret;

      handle = dlopen (fpath, RTLD_NOW);
      जर (UNLIKELY (handle == रिक्त))
        परत crun_make_error (err, 0, "cannot load `%s`: `%s`", fpath, dlerror ());

      ret = handler_manager_add_so (manager, handle, err);
      जर (UNLIKELY (ret < 0))
        {
          dlclose (handle);
          परत ret;
        }
    }
  परत 0;
#अन्यथा
  (शून्य) manager;
  (शून्य) path;
  परत crun_make_error (err, ENOTSUP, "dlopen not available");
#endif
}

संरचना custom_handler_s *
handler_by_name (संरचना custom_handler_manager_s *manager, स्थिरांक अक्षर *name)
{
  माप_प्रकार i;

  साठी (i = 0; i < manager->handlers_len; i++)
    {
      जर (म_भेद (manager->handlers[i]->name, name) == 0)
        परत manager->handlers[i];
      जर (manager->handlers[i]->alias && म_भेद (manager->handlers[i]->alias, name) == 0)
        परत manager->handlers[i];
    }
  परत रिक्त;
}

शून्य
libcrun_handler_manager_print_feature_tags (संरचना custom_handler_manager_s *manager, खाता *out)
{
  माप_प्रकार i;

  साठी (i = 0; i < manager->handlers_len; i++)
    जर (manager->handlers[i]->feature_string)
      ख_लिखो (out, "+%s ", manager->handlers[i]->feature_string);
}

अक्षर **
libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err arg_unused)
{
  संरचना custom_handler_manager_s *manager;
  अक्षर **out;
  माप_प्रकार i, n = 0;

  जर (ctx == रिक्त || ctx->handler_manager == रिक्त)
    परत रिक्त;

  manager = ctx->handler_manager;

  साठी (i = 0; i < manager->handlers_len; i++)
    जर (manager->handlers[i]->feature_string)
      n++;

  जर (n == 0)
    परत रिक्त;

  out = xmalloc0 ((n + 1) * आकार (अक्षर *));
  n = 0;
  साठी (i = 0; i < manager->handlers_len; i++)
    जर (manager->handlers[i]->feature_string)
      out[n++] = xstrdup (manager->handlers[i]->feature_string);

  परत out;
}

स्थिर ओळीत संरचना custom_handler_instance_s *
make_custom_handler_instance_s (संरचना custom_handler_s *vtable)
{
  संरचना custom_handler_instance_s *ret = xmalloc0 (आकार (संरचना custom_handler_instance_s));

  ret->vtable = vtable;
  ret->cookie = रिक्त;

  परत ret;
}

स्थिर पूर्णांक
find_handler_for_container (संरचना custom_handler_manager_s *manager,
                            libcrun_container_t *container,
                            संरचना custom_handler_instance_s **out,
                            libcrun_error_t *err)
{
  माप_प्रकार i;

  स_रखो (out, 0, आकार (*out));

  साठी (i = 0; i < manager->handlers_len; i++)
    {
      पूर्णांक ret;

      जर (manager->handlers[i]->can_handle_container == रिक्त)
        सुरूठेवा;

      ret = manager->handlers[i]->can_handle_container (container, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      जर (ret)
        {
          *out = make_custom_handler_instance_s (manager->handlers[i]);
          जर ((*out)->vtable->load)
            परत (*out)->vtable->load (&((*out)->cookie), err);

          परत 0;
        }
    }

  परत 0;
}

पूर्णांक
libcrun_configure_handler (संरचना custom_handler_manager_s *manager,
                           libcrun_context_t *context,
                           libcrun_container_t *container,
                           संरचना custom_handler_instance_s **out,
                           libcrun_error_t *err)
{
  स्थिरांक अक्षर *explicit_handler;
  स्थिरांक अक्षर *annotation;
  पूर्णांक ret;

  *out = रिक्त;

  // Kubernetes sandbox containers must be executed as regular process
  // Example sandbox container can contain pause process
  // See: https://github.com/containers/crun/issues/798
  // before invoking handler check if this is not a kubernetes sandbox
  annotation = find_annotation (container, "io.kubernetes.cri.container-type");
  जर (annotation && (म_भेद (annotation, "sandbox") == 0))
    परत 0;

  annotation = find_annotation (container, "run.oci.handler");

  /* Fail with EACCESS if global handler is already configured and there was an attempt to override it via spec.  */
  जर (context->handler != रिक्त && annotation != रिक्त)
    परत crun_make_error (err, EACCES, "invalid attempt to override already configured global handler: `%s`", context->handler);

  explicit_handler = context->handler ? context->handler : annotation;

  /* If an explicit handler was requested, use it.  */
  जर (explicit_handler)
    {
      संरचना custom_handler_s *h;

      जर (manager == रिक्त)
        परत crun_make_error (err, 0, "handler requested but no manager configured: `%s`", explicit_handler);

      h = handler_by_name (manager, explicit_handler);
      जर (h)
        {
          *out = make_custom_handler_instance_s (h);
          जर ((*out)->vtable->load)
            ret = (*out)->vtable->load (&((*out)->cookie), err);
          अन्यथा
            ret = 0;
          जा out;
        }
    }

  जर (manager == रिक्त)
    परत 0;

  ret = find_handler_for_container (manager, container, out, err);

out:
  जर (ret == 0 && *out != रिक्त && container->context != रिक्त && ! (*out)->vtable->supports_open_वृक्ष_namespace)
    container->context->no_open_वृक्ष_namespace = true;

  परत ret;
}
