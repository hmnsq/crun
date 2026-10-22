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
#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश "custom-handler.h"
#समावेश "container.h"
#समावेश "utils.h"
#समावेश "linux.h"
#समावेश <unistd.h>
#समावेश <sys/stat.h>
#समावेश <errno.h>
#समावेश <sys/types.h>
#समावेश <fcntl.h>
#समावेश <sched.h>

#अगर_घोषित HAVE_DLOPEN
#  include <dlfcn.h>
#पूर्ण_अगर

#अगर HAVE_DLOPEN && HAVE_LIBKRUN
बाह्य काष्ठा custom_handler_s handler_libkrun;
#पूर्ण_अगर
#अगर HAVE_DLOPEN && HAVE_WASMTIME
बाह्य काष्ठा custom_handler_s handler_wasmtime;
#पूर्ण_अगर
#अगर HAVE_DLOPEN && HAVE_WASMEDGE
बाह्य काष्ठा custom_handler_s handler_wasmedge;
#पूर्ण_अगर
#अगर HAVE_DLOPEN && HAVE_WASMER
बाह्य काष्ठा custom_handler_s handler_wasmer;
#पूर्ण_अगर
#अगर HAVE_DLOPEN && HAVE_WAMR
बाह्य काष्ठा custom_handler_s handler_wamr;
#पूर्ण_अगर
#अगर HAVE_DLOPEN && HAVE_MONO
बाह्य काष्ठा custom_handler_s handler_mono;
#पूर्ण_अगर
#अगर HAVE_DLOPEN && HAVE_SPIN
बाह्य काष्ठा custom_handler_s handler_spin;
#पूर्ण_अगर

अटल काष्ठा custom_handler_s *static_handlers[] = {
#अगर HAVE_DLOPEN && HAVE_LIBKRUN
  &handler_libkrun,
#पूर्ण_अगर
#अगर HAVE_DLOPEN && HAVE_WASMEDGE
  &handler_wasmedge,
#पूर्ण_अगर
#अगर HAVE_DLOPEN && HAVE_WASMER
  &handler_wasmer,
#पूर्ण_अगर
#अगर HAVE_DLOPEN && HAVE_WASMTIME
  &handler_wasmtime,
#पूर्ण_अगर
#अगर HAVE_DLOPEN && HAVE_WAMR
  &handler_wamr,
#पूर्ण_अगर
#अगर HAVE_DLOPEN && HAVE_MONO
  &handler_mono,
#पूर्ण_अगर
#अगर HAVE_DLOPEN && HAVE_SPIN
  &handler_spin,
#पूर्ण_अगर
  शून्य,
};

काष्ठा custom_handler_manager_s
{
  काष्ठा custom_handler_s **handlers;
  व्योम **handles;
  माप_प्रकार handlers_len;
};

काष्ठा custom_handler_manager_s *
libcrun_handler_manager_create (libcrun_error_t *err arg_unused)
{
  काष्ठा custom_handler_s **handlers = शून्य;
  व्योम **handles = शून्य;
  काष्ठा custom_handler_manager_s *m;
  माप_प्रकार i, handlers_len;

  /* Count the static handlers.  */
  क्रम (handlers_len = 0; static_handlers[handlers_len]; handlers_len++)
    ;

  अगर (handlers_len)
    {
      handlers = xmalloc (माप (काष्ठा custom_handler_s *) * handlers_len);
      handles = xmalloc0 (माप (व्योम *) * handlers_len);
    }

  क्रम (i = 0; i < handlers_len; i++)
    handlers[i] = static_handlers[i];

  m = xmalloc0 (माप (काष्ठा custom_handler_manager_s));
  m->handlers = handlers;
  m->handles = handles;
  m->handlers_len = handlers_len;

  वापस m;
}

व्योम
handler_manager_free (काष्ठा custom_handler_manager_s *manager)
{
  माप_प्रकार i;

  अगर (manager == शून्य)
    वापस;

  क्रम (i = 0; i < manager->handlers_len; i++)
    {
#अगर_घोषित HAVE_DLOPEN
      अगर (manager->handles[i])
        dlclose (manager->handles[i]);
#पूर्ण_अगर
    }
  मुक्त (manager->handlers);
  मुक्त (manager->handles);
  मुक्त (manager);
}

#अगर_घोषित HAVE_DLOPEN
अटल पूर्णांक
handler_manager_add_so (काष्ठा custom_handler_manager_s *manager, व्योम *handle, libcrun_error_t *err)
{
  काष्ठा custom_handler_s *h = शून्य;
  run_oci_get_handler_cb cb;

  cb = (run_oci_get_handler_cb) dlsym (handle, "run_oci_handler_get_handler");
  अगर (UNLIKELY (cb == शून्य))
    वापस crun_make_error (err, 0, "cannot find symbol `run_oci_handler_get_handler`");

  h = cb ();
  अगर (UNLIKELY (h == शून्य))
    वापस crun_make_error (err, 0, "the callback `run_oci_handler_get_handler` didn't return a handler");

  manager->handlers = xrealloc (manager->handlers, माप (काष्ठा custom_handler_s *) * (manager->handlers_len + 1));
  manager->handles = xrealloc (manager->handles, माप (व्योम *) * (manager->handlers_len + 1));

  manager->handlers[manager->handlers_len] = h;
  manager->handles[manager->handlers_len] = handle;
  manager->handlers_len++;
  वापस 0;
}
#पूर्ण_अगर

पूर्णांक
libcrun_handler_manager_load_directory (काष्ठा custom_handler_manager_s *manager, स्थिर अक्षर *path, libcrun_error_t *err)
{
#अगर_घोषित HAVE_DLOPEN
  cleanup_dir सूची *dir = शून्य;
  काष्ठा dirent *अग्र;

  dir = सूची_खोलो (path);
  अगर (UNLIKELY (dir == शून्य))
    वापस crun_make_error (err, त्रुटि_सं, "cannot opendir `%s`", path);

  क्रम (अग्र = सूची_पढ़ो (dir); अग्र; अग्र = सूची_पढ़ो (dir))
    {
      cleanup_free अक्षर *fpath = शून्य;
      स्थिर अक्षर *name;
      व्योम *handle;
      पूर्णांक ret;

      name = अग्र->d_name;
      अगर (name[0] == '.')
        जारी;

      ret = append_paths (&fpath, err, path, name, शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      handle = dlopen (fpath, RTLD_NOW);
      अगर (UNLIKELY (handle == शून्य))
        वापस crun_make_error (err, 0, "cannot load `%s`: `%s`", fpath, dlerror ());

      ret = handler_manager_add_so (manager, handle, err);
      अगर (UNLIKELY (ret < 0))
        {
          dlclose (handle);
          वापस ret;
        }
    }
  वापस 0;
#अन्यथा
  (व्योम) manager;
  (व्योम) path;
  वापस crun_make_error (err, ENOTSUP, "dlopen not available");
#पूर्ण_अगर
}

काष्ठा custom_handler_s *
handler_by_name (काष्ठा custom_handler_manager_s *manager, स्थिर अक्षर *name)
{
  माप_प्रकार i;

  क्रम (i = 0; i < manager->handlers_len; i++)
    {
      अगर (म_भेद (manager->handlers[i]->name, name) == 0)
        वापस manager->handlers[i];
      अगर (manager->handlers[i]->alias && म_भेद (manager->handlers[i]->alias, name) == 0)
        वापस manager->handlers[i];
    }
  वापस शून्य;
}

व्योम
libcrun_handler_manager_print_feature_tags (काष्ठा custom_handler_manager_s *manager, खाता *out)
{
  माप_प्रकार i;

  क्रम (i = 0; i < manager->handlers_len; i++)
    अगर (manager->handlers[i]->feature_string)
      ख_लिखो (out, "+%s ", manager->handlers[i]->feature_string);
}

अक्षर **
libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err arg_unused)
{
  काष्ठा custom_handler_manager_s *manager;
  अक्षर **out;
  माप_प्रकार i, n = 0;

  अगर (ctx == शून्य || ctx->handler_manager == शून्य)
    वापस शून्य;

  manager = ctx->handler_manager;

  क्रम (i = 0; i < manager->handlers_len; i++)
    अगर (manager->handlers[i]->feature_string)
      n++;

  अगर (n == 0)
    वापस शून्य;

  out = xmalloc0 ((n + 1) * माप (अक्षर *));
  n = 0;
  क्रम (i = 0; i < manager->handlers_len; i++)
    अगर (manager->handlers[i]->feature_string)
      out[n++] = xstrdup (manager->handlers[i]->feature_string);

  वापस out;
}

अटल अंतरभूत काष्ठा custom_handler_instance_s *
make_custom_handler_instance_s (काष्ठा custom_handler_s *vtable)
{
  काष्ठा custom_handler_instance_s *ret = xmalloc0 (माप (काष्ठा custom_handler_instance_s));

  ret->vtable = vtable;
  ret->cookie = शून्य;

  वापस ret;
}

अटल पूर्णांक
find_handler_for_container (काष्ठा custom_handler_manager_s *manager,
                            libcrun_container_t *container,
                            काष्ठा custom_handler_instance_s **out,
                            libcrun_error_t *err)
{
  माप_प्रकार i;

  स_रखो (out, 0, माप (*out));

  क्रम (i = 0; i < manager->handlers_len; i++)
    {
      पूर्णांक ret;

      अगर (manager->handlers[i]->can_handle_container == शून्य)
        जारी;

      ret = manager->handlers[i]->can_handle_container (container, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      अगर (ret)
        {
          *out = make_custom_handler_instance_s (manager->handlers[i]);
          अगर ((*out)->vtable->load)
            वापस (*out)->vtable->load (&((*out)->cookie), err);

          वापस 0;
        }
    }

  वापस 0;
}

पूर्णांक
libcrun_configure_handler (काष्ठा custom_handler_manager_s *manager,
                           libcrun_context_t *context,
                           libcrun_container_t *container,
                           काष्ठा custom_handler_instance_s **out,
                           libcrun_error_t *err)
{
  स्थिर अक्षर *explicit_handler;
  स्थिर अक्षर *annotation;
  पूर्णांक ret;

  *out = शून्य;

  // Kubernetes sandbox containers must be executed as regular process
  // Example sandbox container can contain pause process
  // See: https://github.com/containers/crun/issues/798
  // before invoking handler check if this is not a kubernetes sandbox
  annotation = find_annotation (container, "io.kubernetes.cri.container-type");
  अगर (annotation && (म_भेद (annotation, "sandbox") == 0))
    वापस 0;

  annotation = find_annotation (container, "run.oci.handler");

  /* Fail with EACCESS if global handler is already configured and there was an attempt to override it via spec.  */
  अगर (context->handler != शून्य && annotation != शून्य)
    वापस crun_make_error (err, EACCES, "invalid attempt to override already configured global handler: `%s`", context->handler);

  explicit_handler = context->handler ? context->handler : annotation;

  /* If an explicit handler was requested, use it.  */
  अगर (explicit_handler)
    {
      काष्ठा custom_handler_s *h;

      अगर (manager == शून्य)
        वापस crun_make_error (err, 0, "handler requested but no manager configured: `%s`", explicit_handler);

      h = handler_by_name (manager, explicit_handler);
      अगर (h)
        {
          *out = make_custom_handler_instance_s (h);
          अगर ((*out)->vtable->load)
            ret = (*out)->vtable->load (&((*out)->cookie), err);
          अन्यथा
            ret = 0;
          जाओ out;
        }
    }

  अगर (manager == शून्य)
    वापस 0;

  ret = find_handler_for_container (manager, container, out, err);

out:
  अगर (ret == 0 && *out != शून्य && container->context != शून्य && ! (*out)->vtable->supports_open_वृक्ष_namespace)
    container->context->no_open_वृक्ष_namespace = true;

  वापस ret;
}
