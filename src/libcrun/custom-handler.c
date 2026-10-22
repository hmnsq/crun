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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "custom-handler.h"
#ଅନ୍ତର୍ଭାଵ "container.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "linux.h"
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ଯଦି HAVE_DLOPEN && HAVE_LIBKRUN
ବାହ୍ୟ ସଂରଚନା custom_handler_s handler_libkrun;
#endif
#ଯଦି HAVE_DLOPEN && HAVE_WASMTIME
ବାହ୍ୟ ସଂରଚନା custom_handler_s handler_wasmtime;
#endif
#ଯଦି HAVE_DLOPEN && HAVE_WASMEDGE
ବାହ୍ୟ ସଂରଚନା custom_handler_s handler_wasmedge;
#endif
#ଯଦି HAVE_DLOPEN && HAVE_WASMER
ବାହ୍ୟ ସଂରଚନା custom_handler_s handler_wasmer;
#endif
#ଯଦି HAVE_DLOPEN && HAVE_WAMR
ବାହ୍ୟ ସଂରଚନା custom_handler_s handler_wamr;
#endif
#ଯଦି HAVE_DLOPEN && HAVE_MONO
ବାହ୍ୟ ସଂରଚନା custom_handler_s handler_mono;
#endif
#ଯଦି HAVE_DLOPEN && HAVE_SPIN
ବାହ୍ୟ ସଂରଚନା custom_handler_s handler_spin;
#endif

ସ୍ଥିର ସଂରଚନା custom_handler_s *static_handlers[] = {
#ଯଦି HAVE_DLOPEN && HAVE_LIBKRUN
  &handler_libkrun,
#endif
#ଯଦି HAVE_DLOPEN && HAVE_WASMEDGE
  &handler_wasmedge,
#endif
#ଯଦି HAVE_DLOPEN && HAVE_WASMER
  &handler_wasmer,
#endif
#ଯଦି HAVE_DLOPEN && HAVE_WASMTIME
  &handler_wasmtime,
#endif
#ଯଦି HAVE_DLOPEN && HAVE_WAMR
  &handler_wamr,
#endif
#ଯଦି HAVE_DLOPEN && HAVE_MONO
  &handler_mono,
#endif
#ଯଦି HAVE_DLOPEN && HAVE_SPIN
  &handler_spin,
#endif
  ଖାଲି,
};

ସଂରଚନା custom_handler_manager_s
{
  ସଂରଚନା custom_handler_s **handlers;
  ଶୂନ୍ୟ **handles;
  size_t handlers_len;
};

ସଂରଚନା custom_handler_manager_s *
libcrun_handler_manager_create (libcrun_error_t *err arg_unused)
{
  ସଂରଚନା custom_handler_s **handlers = ଖାଲି;
  ଶୂନ୍ୟ **handles = ଖାଲି;
  ସଂରଚନା custom_handler_manager_s *m;
  size_t i, handlers_len;

  /* Count the static handlers.  */
  ପାଇଁ (handlers_len = 0; static_handlers[handlers_len]; handlers_len++)
    ;

  ଯଦି (handlers_len)
    {
      handlers = xmalloc (ଆକାର (ସଂରଚନା custom_handler_s *) * handlers_len);
      handles = xmalloc0 (ଆକାର (ଶୂନ୍ୟ *) * handlers_len);
    }

  ପାଇଁ (i = 0; i < handlers_len; i++)
    handlers[i] = static_handlers[i];

  m = xmalloc0 (ଆକାର (ସଂରଚନା custom_handler_manager_s));
  m->handlers = handlers;
  m->handles = handles;
  m->handlers_len = handlers_len;

  ଫେରନ୍ତୁ m;
}

ଶୂନ୍ୟ
handler_manager_free (ସଂରଚନା custom_handler_manager_s *manager)
{
  size_t i;

  ଯଦି (manager == ଖାଲି)
    ଫେରନ୍ତୁ;

  ପାଇଁ (i = 0; i < manager->handlers_len; i++)
    {
#ifdef HAVE_DLOPEN
      ଯଦି (manager->handles[i])
        dlclose (manager->handles[i]);
#endif
    }
  ମୁକ୍ତ (manager->handlers);
  ମୁକ୍ତ (manager->handles);
  ମୁକ୍ତ (manager);
}

#ifdef HAVE_DLOPEN
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
handler_manager_add_so (ସଂରଚନା custom_handler_manager_s *manager, ଶୂନ୍ୟ *handle, libcrun_error_t *err)
{
  ସଂରଚନା custom_handler_s *h = ଖାଲି;
  run_oci_get_handler_cb cb;

  cb = (run_oci_get_handler_cb) dlsym (handle, "run_oci_handler_get_handler");
  ଯଦି (UNLIKELY (cb == ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot find symbol `run_oci_handler_get_handler`");

  h = cb ();
  ଯଦି (UNLIKELY (h == ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "the callback `run_oci_handler_get_handler` didn't return a handler");

  manager->handlers = xrealloc (manager->handlers, ଆକାର (ସଂରଚନା custom_handler_s *) * (manager->handlers_len + 1));
  manager->handles = xrealloc (manager->handles, ଆକାର (ଶୂନ୍ୟ *) * (manager->handlers_len + 1));

  manager->handlers[manager->handlers_len] = h;
  manager->handles[manager->handlers_len] = handle;
  manager->handlers_len++;
  ଫେରନ୍ତୁ 0;
}
#endif

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_handler_manager_load_directory (ସଂରଚନା custom_handler_manager_s *manager, ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err)
{
#ifdef HAVE_DLOPEN
  cleanup_dir DIR *dir = ଖାଲି;
  ସଂରଚନା dirent *next;

  dir = opendir (path);
  ଯଦି (UNLIKELY (dir == ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "cannot opendir `%s`", path);

  ପାଇଁ (next = readdir (dir); next; next = readdir (dir))
    {
      cleanup_free ଅକ୍ଷର *fpath = ଖାଲି;
      ଅଚଳ ଅକ୍ଷର *name;
      ଶୂନ୍ୟ *handle;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      name = next->d_name;
      ଯଦି (name[0] == '.')
        ଜାରି;

      ret = append_paths (&fpath, err, path, name, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      handle = dlopen (fpath, RTLD_NOW);
      ଯଦି (UNLIKELY (handle == ଖାଲି))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot load `%s`: `%s`", fpath, dlerror ());

      ret = handler_manager_add_so (manager, handle, err);
      ଯଦି (UNLIKELY (ret < 0))
        {
          dlclose (handle);
          ଫେରନ୍ତୁ ret;
        }
    }
  ଫେରନ୍ତୁ 0;
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) manager;
  (ଶୂନ୍ୟ) path;
  ଫେରନ୍ତୁ crun_make_error (err, ENOTSUP, "dlopen not available");
#endif
}

ସଂରଚନା custom_handler_s *
handler_by_name (ସଂରଚନା custom_handler_manager_s *manager, ଅଚଳ ଅକ୍ଷର *name)
{
  size_t i;

  ପାଇଁ (i = 0; i < manager->handlers_len; i++)
    {
      ଯଦି (strcmp (manager->handlers[i]->name, name) == 0)
        ଫେରନ୍ତୁ manager->handlers[i];
      ଯଦି (manager->handlers[i]->alias && strcmp (manager->handlers[i]->alias, name) == 0)
        ଫେରନ୍ତୁ manager->handlers[i];
    }
  ଫେରନ୍ତୁ ଖାଲି;
}

ଶୂନ୍ୟ
libcrun_handler_manager_print_feature_tags (ସଂରଚନା custom_handler_manager_s *manager, FILE *out)
{
  size_t i;

  ପାଇଁ (i = 0; i < manager->handlers_len; i++)
    ଯଦି (manager->handlers[i]->feature_string)
      fprintf (out, "+%s ", manager->handlers[i]->feature_string);
}

ଅକ୍ଷର **
libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err arg_unused)
{
  ସଂରଚନା custom_handler_manager_s *manager;
  ଅକ୍ଷର **out;
  size_t i, n = 0;

  ଯଦି (ctx == ଖାଲି || ctx->handler_manager == ଖାଲି)
    ଫେରନ୍ତୁ ଖାଲି;

  manager = ctx->handler_manager;

  ପାଇଁ (i = 0; i < manager->handlers_len; i++)
    ଯଦି (manager->handlers[i]->feature_string)
      n++;

  ଯଦି (n == 0)
    ଫେରନ୍ତୁ ଖାଲି;

  out = xmalloc0 ((n + 1) * ଆକାର (ଅକ୍ଷର *));
  n = 0;
  ପାଇଁ (i = 0; i < manager->handlers_len; i++)
    ଯଦି (manager->handlers[i]->feature_string)
      out[n++] = xstrdup (manager->handlers[i]->feature_string);

  ଫେରନ୍ତୁ out;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା custom_handler_instance_s *
make_custom_handler_instance_s (ସଂରଚନା custom_handler_s *vtable)
{
  ସଂରଚନା custom_handler_instance_s *ret = xmalloc0 (ଆକାର (ସଂରଚନା custom_handler_instance_s));

  ret->vtable = vtable;
  ret->cookie = ଖାଲି;

  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
find_handler_for_container (ସଂରଚନା custom_handler_manager_s *manager,
                            libcrun_container_t *container,
                            ସଂରଚନା custom_handler_instance_s **out,
                            libcrun_error_t *err)
{
  size_t i;

  memset (out, 0, ଆକାର (*out));

  ପାଇଁ (i = 0; i < manager->handlers_len; i++)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      ଯଦି (manager->handlers[i]->can_handle_container == ଖାଲି)
        ଜାରି;

      ret = manager->handlers[i]->can_handle_container (container, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ଯଦି (ret)
        {
          *out = make_custom_handler_instance_s (manager->handlers[i]);
          ଯଦି ((*out)->vtable->load)
            ଫେରନ୍ତୁ (*out)->vtable->load (&((*out)->cookie), err);

          ଫେରନ୍ତୁ 0;
        }
    }

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_configure_handler (ସଂରଚନା custom_handler_manager_s *manager,
                           libcrun_context_t *context,
                           libcrun_container_t *container,
                           ସଂରଚନା custom_handler_instance_s **out,
                           libcrun_error_t *err)
{
  ଅଚଳ ଅକ୍ଷର *explicit_handler;
  ଅଚଳ ଅକ୍ଷର *annotation;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  *out = ଖାଲି;

  // Kubernetes sandbox containers must be executed as regular process
  // Example sandbox container can contain pause process
  // See: https://github.com/containers/crun/issues/798
  // before invoking handler check if this is not a kubernetes sandbox
  annotation = find_annotation (container, "io.kubernetes.cri.container-type");
  ଯଦି (annotation && (strcmp (annotation, "sandbox") == 0))
    ଫେରନ୍ତୁ 0;

  annotation = find_annotation (container, "run.oci.handler");

  /* Fail with EACCESS if global handler is already configured and there was an attempt to override it via spec.  */
  ଯଦି (context->handler != ଖାଲି && annotation != ଖାଲି)
    ଫେରନ୍ତୁ crun_make_error (err, EACCES, "invalid attempt to override already configured global handler: `%s`", context->handler);

  explicit_handler = context->handler ? context->handler : annotation;

  /* If an explicit handler was requested, use it.  */
  ଯଦି (explicit_handler)
    {
      ସଂରଚନା custom_handler_s *h;

      ଯଦି (manager == ଖାଲି)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "handler requested but no manager configured: `%s`", explicit_handler);

      h = handler_by_name (manager, explicit_handler);
      ଯଦି (h)
        {
          *out = make_custom_handler_instance_s (h);
          ଯଦି ((*out)->vtable->load)
            ret = (*out)->vtable->load (&((*out)->cookie), err);
          ଅନ୍ୟଥା
            ret = 0;
          ଯାଅ out;
        }
    }

  ଯଦି (manager == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  ret = find_handler_for_container (manager, container, out, err);

out:
  ଯଦି (ret == 0 && *out != ଖାଲି && container->context != ଖାଲି && ! (*out)->vtable->supports_open_ବୃକ୍ଷ_namespace)
    container->context->no_open_ବୃକ୍ଷ_namespace = true;

  ଫେରନ୍ତୁ ret;
}
