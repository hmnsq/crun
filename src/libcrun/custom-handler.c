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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "custom-handler.h"
#અન્તર્ભાવ "container.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "linux.h"
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#જો HAVE_DLOPEN && HAVE_LIBKRUN
બાહ્ય સંરચના custom_handler_s handler_libkrun;
#endif
#જો HAVE_DLOPEN && HAVE_WASMTIME
બાહ્ય સંરચના custom_handler_s handler_wasmtime;
#endif
#જો HAVE_DLOPEN && HAVE_WASMEDGE
બાહ્ય સંરચના custom_handler_s handler_wasmedge;
#endif
#જો HAVE_DLOPEN && HAVE_WASMER
બાહ્ય સંરચના custom_handler_s handler_wasmer;
#endif
#જો HAVE_DLOPEN && HAVE_WAMR
બાહ્ય સંરચના custom_handler_s handler_wamr;
#endif
#જો HAVE_DLOPEN && HAVE_MONO
બાહ્ય સંરચના custom_handler_s handler_mono;
#endif
#જો HAVE_DLOPEN && HAVE_SPIN
બાહ્ય સંરચના custom_handler_s handler_spin;
#endif

સ્થિર સંરચના custom_handler_s *static_handlers[] = {
#જો HAVE_DLOPEN && HAVE_LIBKRUN
  &handler_libkrun,
#endif
#જો HAVE_DLOPEN && HAVE_WASMEDGE
  &handler_wasmedge,
#endif
#જો HAVE_DLOPEN && HAVE_WASMER
  &handler_wasmer,
#endif
#જો HAVE_DLOPEN && HAVE_WASMTIME
  &handler_wasmtime,
#endif
#જો HAVE_DLOPEN && HAVE_WAMR
  &handler_wamr,
#endif
#જો HAVE_DLOPEN && HAVE_MONO
  &handler_mono,
#endif
#જો HAVE_DLOPEN && HAVE_SPIN
  &handler_spin,
#endif
  ખાલી,
};

સંરચના custom_handler_manager_s
{
  સંરચના custom_handler_s **handlers;
  શૂન્ય **handles;
  size_t handlers_len;
};

સંરચના custom_handler_manager_s *
libcrun_handler_manager_create (libcrun_error_t *err arg_unused)
{
  સંરચના custom_handler_s **handlers = ખાલી;
  શૂન્ય **handles = ખાલી;
  સંરચના custom_handler_manager_s *m;
  size_t i, handlers_len;

  /* Count the static handlers.  */
  માટે (handlers_len = 0; static_handlers[handlers_len]; handlers_len++)
    ;

  જો (handlers_len)
    {
      handlers = xmalloc (કદ (સંરચના custom_handler_s *) * handlers_len);
      handles = xmalloc0 (કદ (શૂન્ય *) * handlers_len);
    }

  માટે (i = 0; i < handlers_len; i++)
    handlers[i] = static_handlers[i];

  m = xmalloc0 (કદ (સંરચના custom_handler_manager_s));
  m->handlers = handlers;
  m->handles = handles;
  m->handlers_len = handlers_len;

  પાછા m;
}

શૂન્ય
handler_manager_free (સંરચના custom_handler_manager_s *manager)
{
  size_t i;

  જો (manager == ખાલી)
    પાછા;

  માટે (i = 0; i < manager->handlers_len; i++)
    {
#ifdef HAVE_DLOPEN
      જો (manager->handles[i])
        dlclose (manager->handles[i]);
#endif
    }
  મુક્ત (manager->handlers);
  મુક્ત (manager->handles);
  મુક્ત (manager);
}

#ifdef HAVE_DLOPEN
સ્થિર પૂર્ણાંક
handler_manager_add_so (સંરચના custom_handler_manager_s *manager, શૂન્ય *handle, libcrun_error_t *err)
{
  સંરચના custom_handler_s *h = ખાલી;
  run_oci_get_handler_cb cb;

  cb = (run_oci_get_handler_cb) dlsym (handle, "run_oci_handler_get_handler");
  જો (UNLIKELY (cb == ખાલી))
    પાછા crun_make_error (err, 0, "cannot find symbol `run_oci_handler_get_handler`");

  h = cb ();
  જો (UNLIKELY (h == ખાલી))
    પાછા crun_make_error (err, 0, "the callback `run_oci_handler_get_handler` didn't return a handler");

  manager->handlers = xrealloc (manager->handlers, કદ (સંરચના custom_handler_s *) * (manager->handlers_len + 1));
  manager->handles = xrealloc (manager->handles, કદ (શૂન્ય *) * (manager->handlers_len + 1));

  manager->handlers[manager->handlers_len] = h;
  manager->handles[manager->handlers_len] = handle;
  manager->handlers_len++;
  પાછા 0;
}
#endif

પૂર્ણાંક
libcrun_handler_manager_load_directory (સંરચના custom_handler_manager_s *manager, અચલ અક્ષર *path, libcrun_error_t *err)
{
#ifdef HAVE_DLOPEN
  cleanup_dir DIR *dir = ખાલી;
  સંરચના dirent *next;

  dir = opendir (path);
  જો (UNLIKELY (dir == ખાલી))
    પાછા crun_make_error (err, errno, "cannot opendir `%s`", path);

  માટે (next = readdir (dir); next; next = readdir (dir))
    {
      cleanup_free અક્ષર *fpath = ખાલી;
      અચલ અક્ષર *name;
      શૂન્ય *handle;
      પૂર્ણાંક ret;

      name = next->d_name;
      જો (name[0] == '.')
        ચાલુ;

      ret = append_paths (&fpath, err, path, name, ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      handle = dlopen (fpath, RTLD_NOW);
      જો (UNLIKELY (handle == ખાલી))
        પાછા crun_make_error (err, 0, "cannot load `%s`: `%s`", fpath, dlerror ());

      ret = handler_manager_add_so (manager, handle, err);
      જો (UNLIKELY (ret < 0))
        {
          dlclose (handle);
          પાછા ret;
        }
    }
  પાછા 0;
#નહીંતર
  (શૂન્ય) manager;
  (શૂન્ય) path;
  પાછા crun_make_error (err, ENOTSUP, "dlopen not available");
#endif
}

સંરચના custom_handler_s *
handler_by_name (સંરચના custom_handler_manager_s *manager, અચલ અક્ષર *name)
{
  size_t i;

  માટે (i = 0; i < manager->handlers_len; i++)
    {
      જો (strcmp (manager->handlers[i]->name, name) == 0)
        પાછા manager->handlers[i];
      જો (manager->handlers[i]->alias && strcmp (manager->handlers[i]->alias, name) == 0)
        પાછા manager->handlers[i];
    }
  પાછા ખાલી;
}

શૂન્ય
libcrun_handler_manager_print_feature_tags (સંરચના custom_handler_manager_s *manager, FILE *out)
{
  size_t i;

  માટે (i = 0; i < manager->handlers_len; i++)
    જો (manager->handlers[i]->feature_string)
      fprintf (out, "+%s ", manager->handlers[i]->feature_string);
}

અક્ષર **
libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err arg_unused)
{
  સંરચના custom_handler_manager_s *manager;
  અક્ષર **out;
  size_t i, n = 0;

  જો (ctx == ખાલી || ctx->handler_manager == ખાલી)
    પાછા ખાલી;

  manager = ctx->handler_manager;

  માટે (i = 0; i < manager->handlers_len; i++)
    જો (manager->handlers[i]->feature_string)
      n++;

  જો (n == 0)
    પાછા ખાલી;

  out = xmalloc0 ((n + 1) * કદ (અક્ષર *));
  n = 0;
  માટે (i = 0; i < manager->handlers_len; i++)
    જો (manager->handlers[i]->feature_string)
      out[n++] = xstrdup (manager->handlers[i]->feature_string);

  પાછા out;
}

સ્થિર અંતઃરેખ સંરચના custom_handler_instance_s *
make_custom_handler_instance_s (સંરચના custom_handler_s *vtable)
{
  સંરચના custom_handler_instance_s *ret = xmalloc0 (કદ (સંરચના custom_handler_instance_s));

  ret->vtable = vtable;
  ret->cookie = ખાલી;

  પાછા ret;
}

સ્થિર પૂર્ણાંક
find_handler_for_container (સંરચના custom_handler_manager_s *manager,
                            libcrun_container_t *container,
                            સંરચના custom_handler_instance_s **out,
                            libcrun_error_t *err)
{
  size_t i;

  memset (out, 0, કદ (*out));

  માટે (i = 0; i < manager->handlers_len; i++)
    {
      પૂર્ણાંક ret;

      જો (manager->handlers[i]->can_handle_container == ખાલી)
        ચાલુ;

      ret = manager->handlers[i]->can_handle_container (container, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      જો (ret)
        {
          *out = make_custom_handler_instance_s (manager->handlers[i]);
          જો ((*out)->vtable->load)
            પાછા (*out)->vtable->load (&((*out)->cookie), err);

          પાછા 0;
        }
    }

  પાછા 0;
}

પૂર્ણાંક
libcrun_configure_handler (સંરચના custom_handler_manager_s *manager,
                           libcrun_context_t *context,
                           libcrun_container_t *container,
                           સંરચના custom_handler_instance_s **out,
                           libcrun_error_t *err)
{
  અચલ અક્ષર *explicit_handler;
  અચલ અક્ષર *annotation;
  પૂર્ણાંક ret;

  *out = ખાલી;

  // Kubernetes sandbox containers must be executed as regular process
  // Example sandbox container can contain pause process
  // See: https://github.com/containers/crun/issues/798
  // before invoking handler check if this is not a kubernetes sandbox
  annotation = find_annotation (container, "io.kubernetes.cri.container-type");
  જો (annotation && (strcmp (annotation, "sandbox") == 0))
    પાછા 0;

  annotation = find_annotation (container, "run.oci.handler");

  /* Fail with EACCESS if global handler is already configured and there was an attempt to override it via spec.  */
  જો (context->handler != ખાલી && annotation != ખાલી)
    પાછા crun_make_error (err, EACCES, "invalid attempt to override already configured global handler: `%s`", context->handler);

  explicit_handler = context->handler ? context->handler : annotation;

  /* If an explicit handler was requested, use it.  */
  જો (explicit_handler)
    {
      સંરચના custom_handler_s *h;

      જો (manager == ખાલી)
        પાછા crun_make_error (err, 0, "handler requested but no manager configured: `%s`", explicit_handler);

      h = handler_by_name (manager, explicit_handler);
      જો (h)
        {
          *out = make_custom_handler_instance_s (h);
          જો ((*out)->vtable->load)
            ret = (*out)->vtable->load (&((*out)->cookie), err);
          નહીંતર
            ret = 0;
          જાઓ out;
        }
    }

  જો (manager == ખાલી)
    પાછા 0;

  ret = find_handler_for_container (manager, container, out, err);

out:
  જો (ret == 0 && *out != ખાલી && container->context != ખાલી && ! (*out)->vtable->supports_open_વૃક્ષ_namespace)
    container->context->no_open_વૃક્ષ_namespace = true;

  પાછા ret;
}
