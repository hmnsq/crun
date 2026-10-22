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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "custom-handler.h"
#അന്തര്ഭാവ "container.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "linux.h"
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <sched.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#എങ്കിൽ HAVE_DLOPEN && HAVE_LIBKRUN
ബാഹ്യ ഘടന custom_handler_s handler_libkrun;
#endif
#എങ്കിൽ HAVE_DLOPEN && HAVE_WASMTIME
ബാഹ്യ ഘടന custom_handler_s handler_wasmtime;
#endif
#എങ്കിൽ HAVE_DLOPEN && HAVE_WASMEDGE
ബാഹ്യ ഘടന custom_handler_s handler_wasmedge;
#endif
#എങ്കിൽ HAVE_DLOPEN && HAVE_WASMER
ബാഹ്യ ഘടന custom_handler_s handler_wasmer;
#endif
#എങ്കിൽ HAVE_DLOPEN && HAVE_WAMR
ബാഹ്യ ഘടന custom_handler_s handler_wamr;
#endif
#എങ്കിൽ HAVE_DLOPEN && HAVE_MONO
ബാഹ്യ ഘടന custom_handler_s handler_mono;
#endif
#എങ്കിൽ HAVE_DLOPEN && HAVE_SPIN
ബാഹ്യ ഘടന custom_handler_s handler_spin;
#endif

സ്ഥിതം ഘടന custom_handler_s *static_handlers[] = {
#എങ്കിൽ HAVE_DLOPEN && HAVE_LIBKRUN
  &handler_libkrun,
#endif
#എങ്കിൽ HAVE_DLOPEN && HAVE_WASMEDGE
  &handler_wasmedge,
#endif
#എങ്കിൽ HAVE_DLOPEN && HAVE_WASMER
  &handler_wasmer,
#endif
#എങ്കിൽ HAVE_DLOPEN && HAVE_WASMTIME
  &handler_wasmtime,
#endif
#എങ്കിൽ HAVE_DLOPEN && HAVE_WAMR
  &handler_wamr,
#endif
#എങ്കിൽ HAVE_DLOPEN && HAVE_MONO
  &handler_mono,
#endif
#എങ്കിൽ HAVE_DLOPEN && HAVE_SPIN
  &handler_spin,
#endif
  ശൂന്യ,
};

ഘടന custom_handler_manager_s
{
  ഘടന custom_handler_s **handlers;
  ശൂന്യം **handles;
  size_t handlers_len;
};

ഘടന custom_handler_manager_s *
libcrun_handler_manager_create (libcrun_error_t *err arg_unused)
{
  ഘടന custom_handler_s **handlers = ശൂന്യ;
  ശൂന്യം **handles = ശൂന്യ;
  ഘടന custom_handler_manager_s *m;
  size_t i, handlers_len;

  /* Count the static handlers.  */
  വേണ്ടി (handlers_len = 0; static_handlers[handlers_len]; handlers_len++)
    ;

  എങ്കിൽ (handlers_len)
    {
      handlers = xmalloc (വലുപ്പം (ഘടന custom_handler_s *) * handlers_len);
      handles = xmalloc0 (വലുപ്പം (ശൂന്യം *) * handlers_len);
    }

  വേണ്ടി (i = 0; i < handlers_len; i++)
    handlers[i] = static_handlers[i];

  m = xmalloc0 (വലുപ്പം (ഘടന custom_handler_manager_s));
  m->handlers = handlers;
  m->handles = handles;
  m->handlers_len = handlers_len;

  തിരികെ m;
}

ശൂന്യം
handler_manager_free (ഘടന custom_handler_manager_s *manager)
{
  size_t i;

  എങ്കിൽ (manager == ശൂന്യ)
    തിരികെ;

  വേണ്ടി (i = 0; i < manager->handlers_len; i++)
    {
#ifdef HAVE_DLOPEN
      എങ്കിൽ (manager->handles[i])
        dlclose (manager->handles[i]);
#endif
    }
  സ്വതന്ത്രം (manager->handlers);
  സ്വതന്ത്രം (manager->handles);
  സ്വതന്ത്രം (manager);
}

#ifdef HAVE_DLOPEN
സ്ഥിതം പൂർണ്ണസംഖ്യ
handler_manager_add_so (ഘടന custom_handler_manager_s *manager, ശൂന്യം *handle, libcrun_error_t *err)
{
  ഘടന custom_handler_s *h = ശൂന്യ;
  run_oci_get_handler_cb cb;

  cb = (run_oci_get_handler_cb) dlsym (handle, "run_oci_handler_get_handler");
  എങ്കിൽ (UNLIKELY (cb == ശൂന്യ))
    തിരികെ crun_make_error (err, 0, "cannot find symbol `run_oci_handler_get_handler`");

  h = cb ();
  എങ്കിൽ (UNLIKELY (h == ശൂന്യ))
    തിരികെ crun_make_error (err, 0, "the callback `run_oci_handler_get_handler` didn't return a handler");

  manager->handlers = xrealloc (manager->handlers, വലുപ്പം (ഘടന custom_handler_s *) * (manager->handlers_len + 1));
  manager->handles = xrealloc (manager->handles, വലുപ്പം (ശൂന്യം *) * (manager->handlers_len + 1));

  manager->handlers[manager->handlers_len] = h;
  manager->handles[manager->handlers_len] = handle;
  manager->handlers_len++;
  തിരികെ 0;
}
#endif

പൂർണ്ണസംഖ്യ
libcrun_handler_manager_load_directory (ഘടന custom_handler_manager_s *manager, സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err)
{
#ifdef HAVE_DLOPEN
  cleanup_dir DIR *dir = ശൂന്യ;
  ഘടന dirent *next;

  dir = opendir (path);
  എങ്കിൽ (UNLIKELY (dir == ശൂന്യ))
    തിരികെ crun_make_error (err, errno, "cannot opendir `%s`", path);

  വേണ്ടി (next = readdir (dir); next; next = readdir (dir))
    {
      cleanup_free അക്ഷരം *fpath = ശൂന്യ;
      സ്ഥിരം അക്ഷരം *name;
      ശൂന്യം *handle;
      പൂർണ്ണസംഖ്യ ret;

      name = next->d_name;
      എങ്കിൽ (name[0] == '.')
        തുടരുക;

      ret = append_paths (&fpath, err, path, name, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      handle = dlopen (fpath, RTLD_NOW);
      എങ്കിൽ (UNLIKELY (handle == ശൂന്യ))
        തിരികെ crun_make_error (err, 0, "cannot load `%s`: `%s`", fpath, dlerror ());

      ret = handler_manager_add_so (manager, handle, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          dlclose (handle);
          തിരികെ ret;
        }
    }
  തിരികെ 0;
#അല്ലെങ്കിൽ
  (ശൂന്യം) manager;
  (ശൂന്യം) path;
  തിരികെ crun_make_error (err, ENOTSUP, "dlopen not available");
#endif
}

ഘടന custom_handler_s *
handler_by_name (ഘടന custom_handler_manager_s *manager, സ്ഥിരം അക്ഷരം *name)
{
  size_t i;

  വേണ്ടി (i = 0; i < manager->handlers_len; i++)
    {
      എങ്കിൽ (strcmp (manager->handlers[i]->name, name) == 0)
        തിരികെ manager->handlers[i];
      എങ്കിൽ (manager->handlers[i]->alias && strcmp (manager->handlers[i]->alias, name) == 0)
        തിരികെ manager->handlers[i];
    }
  തിരികെ ശൂന്യ;
}

ശൂന്യം
libcrun_handler_manager_print_feature_tags (ഘടന custom_handler_manager_s *manager, FILE *out)
{
  size_t i;

  വേണ്ടി (i = 0; i < manager->handlers_len; i++)
    എങ്കിൽ (manager->handlers[i]->feature_string)
      fprintf (out, "+%s ", manager->handlers[i]->feature_string);
}

അക്ഷരം **
libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err arg_unused)
{
  ഘടന custom_handler_manager_s *manager;
  അക്ഷരം **out;
  size_t i, n = 0;

  എങ്കിൽ (ctx == ശൂന്യ || ctx->handler_manager == ശൂന്യ)
    തിരികെ ശൂന്യ;

  manager = ctx->handler_manager;

  വേണ്ടി (i = 0; i < manager->handlers_len; i++)
    എങ്കിൽ (manager->handlers[i]->feature_string)
      n++;

  എങ്കിൽ (n == 0)
    തിരികെ ശൂന്യ;

  out = xmalloc0 ((n + 1) * വലുപ്പം (അക്ഷരം *));
  n = 0;
  വേണ്ടി (i = 0; i < manager->handlers_len; i++)
    എങ്കിൽ (manager->handlers[i]->feature_string)
      out[n++] = xstrdup (manager->handlers[i]->feature_string);

  തിരികെ out;
}

സ്ഥിതം വരിയിൽ ഘടന custom_handler_instance_s *
make_custom_handler_instance_s (ഘടന custom_handler_s *vtable)
{
  ഘടന custom_handler_instance_s *ret = xmalloc0 (വലുപ്പം (ഘടന custom_handler_instance_s));

  ret->vtable = vtable;
  ret->cookie = ശൂന്യ;

  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
find_handler_for_container (ഘടന custom_handler_manager_s *manager,
                            libcrun_container_t *container,
                            ഘടന custom_handler_instance_s **out,
                            libcrun_error_t *err)
{
  size_t i;

  memset (out, 0, വലുപ്പം (*out));

  വേണ്ടി (i = 0; i < manager->handlers_len; i++)
    {
      പൂർണ്ണസംഖ്യ ret;

      എങ്കിൽ (manager->handlers[i]->can_handle_container == ശൂന്യ)
        തുടരുക;

      ret = manager->handlers[i]->can_handle_container (container, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      എങ്കിൽ (ret)
        {
          *out = make_custom_handler_instance_s (manager->handlers[i]);
          എങ്കിൽ ((*out)->vtable->load)
            തിരികെ (*out)->vtable->load (&((*out)->cookie), err);

          തിരികെ 0;
        }
    }

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_configure_handler (ഘടന custom_handler_manager_s *manager,
                           libcrun_context_t *context,
                           libcrun_container_t *container,
                           ഘടന custom_handler_instance_s **out,
                           libcrun_error_t *err)
{
  സ്ഥിരം അക്ഷരം *explicit_handler;
  സ്ഥിരം അക്ഷരം *annotation;
  പൂർണ്ണസംഖ്യ ret;

  *out = ശൂന്യ;

  // Kubernetes sandbox containers must be executed as regular process
  // Example sandbox container can contain pause process
  // See: https://github.com/containers/crun/issues/798
  // before invoking handler check if this is not a kubernetes sandbox
  annotation = find_annotation (container, "io.kubernetes.cri.container-type");
  എങ്കിൽ (annotation && (strcmp (annotation, "sandbox") == 0))
    തിരികെ 0;

  annotation = find_annotation (container, "run.oci.handler");

  /* Fail with EACCESS if global handler is already configured and there was an attempt to override it via spec.  */
  എങ്കിൽ (context->handler != ശൂന്യ && annotation != ശൂന്യ)
    തിരികെ crun_make_error (err, EACCES, "invalid attempt to override already configured global handler: `%s`", context->handler);

  explicit_handler = context->handler ? context->handler : annotation;

  /* If an explicit handler was requested, use it.  */
  എങ്കിൽ (explicit_handler)
    {
      ഘടന custom_handler_s *h;

      എങ്കിൽ (manager == ശൂന്യ)
        തിരികെ crun_make_error (err, 0, "handler requested but no manager configured: `%s`", explicit_handler);

      h = handler_by_name (manager, explicit_handler);
      എങ്കിൽ (h)
        {
          *out = make_custom_handler_instance_s (h);
          എങ്കിൽ ((*out)->vtable->load)
            ret = (*out)->vtable->load (&((*out)->cookie), err);
          അല്ലെങ്കിൽ
            ret = 0;
          പോകുക out;
        }
    }

  എങ്കിൽ (manager == ശൂന്യ)
    തിരികെ 0;

  ret = find_handler_for_container (manager, container, out, err);

out:
  എങ്കിൽ (ret == 0 && *out != ശൂന്യ && container->context != ശൂന്യ && ! (*out)->vtable->supports_open_വൃക്ഷം_namespace)
    container->context->no_open_വൃക്ഷം_namespace = true;

  തിരികെ ret;
}
