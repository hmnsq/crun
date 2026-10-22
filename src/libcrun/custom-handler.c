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
#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "custom-handler.h"
#ಅನ್ತರ್ಭಾವ "container.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "linux.h"
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <sched.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DLOPEN
#  include <dlfcn.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ HAVE_DLOPEN && HAVE_LIBKRUN
ಬಾಹ್ಯ ರಚನೆ custom_handler_s handler_libkrun;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_DLOPEN && HAVE_WASMTIME
ಬಾಹ್ಯ ರಚನೆ custom_handler_s handler_wasmtime;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_DLOPEN && HAVE_WASMEDGE
ಬಾಹ್ಯ ರಚನೆ custom_handler_s handler_wasmedge;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_DLOPEN && HAVE_WASMER
ಬಾಹ್ಯ ರಚನೆ custom_handler_s handler_wasmer;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_DLOPEN && HAVE_WAMR
ಬಾಹ್ಯ ರಚನೆ custom_handler_s handler_wamr;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_DLOPEN && HAVE_MONO
ಬಾಹ್ಯ ರಚನೆ custom_handler_s handler_mono;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_DLOPEN && HAVE_SPIN
ಬಾಹ್ಯ ರಚನೆ custom_handler_s handler_spin;
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ರಚನೆ custom_handler_s *static_handlers[] = {
#ಆದರೆ HAVE_DLOPEN && HAVE_LIBKRUN
  &handler_libkrun,
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_DLOPEN && HAVE_WASMEDGE
  &handler_wasmedge,
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_DLOPEN && HAVE_WASMER
  &handler_wasmer,
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_DLOPEN && HAVE_WASMTIME
  &handler_wasmtime,
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_DLOPEN && HAVE_WAMR
  &handler_wamr,
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_DLOPEN && HAVE_MONO
  &handler_mono,
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_DLOPEN && HAVE_SPIN
  &handler_spin,
#ಆದರೆ_ಅಂತ್ಯ
  ಖಾಲಿ,
};

ರಚನೆ custom_handler_manager_s
{
  ರಚನೆ custom_handler_s **handlers;
  ಶೂನ್ಯ **handles;
  size_t handlers_len;
};

ರಚನೆ custom_handler_manager_s *
libcrun_handler_manager_create (libcrun_error_t *err arg_ಬಳಸದ)
{
  ರಚನೆ custom_handler_s **handlers = ಖಾಲಿ;
  ಶೂನ್ಯ **handles = ಖಾಲಿ;
  ರಚನೆ custom_handler_manager_s *m;
  size_t i, handlers_len;

  /* Count the static handlers.  */
  ಗಾಗಿ (handlers_len = 0; static_handlers[handlers_len]; handlers_len++)
    ;

  ಆದರೆ (handlers_len)
    {
      handlers = xmalloc (ಗಾತ್ರ (ರಚನೆ custom_handler_s *) * handlers_len);
      handles = xmalloc0 (ಗಾತ್ರ (ಶೂನ್ಯ *) * handlers_len);
    }

  ಗಾಗಿ (i = 0; i < handlers_len; i++)
    handlers[i] = static_handlers[i];

  m = xmalloc0 (ಗಾತ್ರ (ರಚನೆ custom_handler_manager_s));
  m->handlers = handlers;
  m->handles = handles;
  m->handlers_len = handlers_len;

  ಹಿಂತಿರುಗಿ m;
}

ಶೂನ್ಯ
handler_manager_free (ರಚನೆ custom_handler_manager_s *manager)
{
  size_t i;

  ಆದರೆ (manager == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;

  ಗಾಗಿ (i = 0; i < manager->handlers_len; i++)
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DLOPEN
      ಆದರೆ (manager->handles[i])
        dlclose (manager->handles[i]);
#ಆದರೆ_ಅಂತ್ಯ
    }
  ಬಿಡುಗಡೆ (manager->handlers);
  ಬಿಡುಗಡೆ (manager->handles);
  ಬಿಡುಗಡೆ (manager);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DLOPEN
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
handler_manager_ಕೂಡಿಸು_so (ರಚನೆ custom_handler_manager_s *manager, ಶೂನ್ಯ *handle, libcrun_error_t *err)
{
  ರಚನೆ custom_handler_s *h = ಖಾಲಿ;
  ಸತತ_oci_get_handler_cb cb;

  cb = (ಸತತ_oci_get_handler_cb) dlsym (handle, "run_oci_handler_get_handler");
  ಆದರೆ (UNLIKELY (cb == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot find symbol `run_oci_handler_get_handler`");

  h = cb ();
  ಆದರೆ (UNLIKELY (h == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the callback `run_oci_handler_get_handler` didn't return a handler");

  manager->handlers = xrealloc (manager->handlers, ಗಾತ್ರ (ರಚನೆ custom_handler_s *) * (manager->handlers_len + 1));
  manager->handles = xrealloc (manager->handles, ಗಾತ್ರ (ಶೂನ್ಯ *) * (manager->handlers_len + 1));

  manager->handlers[manager->handlers_len] = h;
  manager->handles[manager->handlers_len] = handle;
  manager->handlers_len++;
  ಹಿಂತಿರುಗಿ 0;
}
#ಆದರೆ_ಅಂತ್ಯ

ಪೂರ್ಣಾಂಕ
libcrun_handler_manager_ತುಂಬು_directory (ರಚನೆ custom_handler_manager_s *manager, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DLOPEN
  cleanup_dir DIR *dir = ಖಾಲಿ;
  ರಚನೆ dirent *ಮುಂದಿನ;

  dir = opendir (ಮಾರ್ಗ);
  ಆದರೆ (UNLIKELY (dir == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot opendir `%s`", ಮಾರ್ಗ);

  ಗಾಗಿ (ಮುಂದಿನ = readdir (dir); ಮುಂದಿನ; ಮುಂದಿನ = readdir (dir))
    {
      cleanup_free ಅಕ್ಷರ *fpath = ಖಾಲಿ;
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
      ಶೂನ್ಯ *handle;
      ಪೂರ್ಣಾಂಕ ret;

      name = ಮುಂದಿನ->d_name;
      ಆದರೆ (name[0] == '.')
        ಮುಂದುವರಿ;

      ret = append_paths (&fpath, err, ಮಾರ್ಗ, name, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      handle = dlopen (fpath, RTLD_NOW);
      ಆದರೆ (UNLIKELY (handle == ಖಾಲಿ))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot load `%s`: `%s`", fpath, dlerror ());

      ret = handler_manager_ಕೂಡಿಸು_so (manager, handle, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          dlclose (handle);
          ಹಿಂತಿರುಗಿ ret;
        }
    }
  ಹಿಂತಿರುಗಿ 0;
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) manager;
  (ಶೂನ್ಯ) ಮಾರ್ಗ;
  ಹಿಂತಿರುಗಿ crun_make_error (err, ENOTSUP, "dlopen not available");
#ಆದರೆ_ಅಂತ್ಯ
}

ರಚನೆ custom_handler_s *
handler_by_name (ರಚನೆ custom_handler_manager_s *manager, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
  size_t i;

  ಗಾಗಿ (i = 0; i < manager->handlers_len; i++)
    {
      ಆದರೆ (strcmp (manager->handlers[i]->name, name) == 0)
        ಹಿಂತಿರುಗಿ manager->handlers[i];
      ಆದರೆ (manager->handlers[i]->alias && strcmp (manager->handlers[i]->alias, name) == 0)
        ಹಿಂತಿರುಗಿ manager->handlers[i];
    }
  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಶೂನ್ಯ
libcrun_handler_manager_print_feature_ಟ್ಯಾಗುಗಳು (ರಚನೆ custom_handler_manager_s *manager, FILE *ಹೊರಹೋಗು)
{
  size_t i;

  ಗಾಗಿ (i = 0; i < manager->handlers_len; i++)
    ಆದರೆ (manager->handlers[i]->feature_string)
      fprintf (ಹೊರಹೋಗು, "+%s ", manager->handlers[i]->feature_string);
}

ಅಕ್ಷರ **
libcrun_context_get_handler_feature_ಟ್ಯಾಗುಗಳು (libcrun_context_t *ctx, libcrun_error_t *err arg_ಬಳಸದ)
{
  ರಚನೆ custom_handler_manager_s *manager;
  ಅಕ್ಷರ **ಹೊರಹೋಗು;
  size_t i, n = 0;

  ಆದರೆ (ctx == ಖಾಲಿ || ctx->handler_manager == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  manager = ctx->handler_manager;

  ಗಾಗಿ (i = 0; i < manager->handlers_len; i++)
    ಆದರೆ (manager->handlers[i]->feature_string)
      n++;

  ಆದರೆ (n == 0)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  ಹೊರಹೋಗು = xmalloc0 ((n + 1) * ಗಾತ್ರ (ಅಕ್ಷರ *));
  n = 0;
  ಗಾಗಿ (i = 0; i < manager->handlers_len; i++)
    ಆದರೆ (manager->handlers[i]->feature_string)
      ಹೊರಹೋಗು[n++] = xstrdup (manager->handlers[i]->feature_string);

  ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ custom_handler_instance_s *
make_custom_handler_instance_s (ರಚನೆ custom_handler_s *vtable)
{
  ರಚನೆ custom_handler_instance_s *ret = xmalloc0 (ಗಾತ್ರ (ರಚನೆ custom_handler_instance_s));

  ret->vtable = vtable;
  ret->cookie = ಖಾಲಿ;

  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಕಂಡುಹಿಡಿ_handler_for_container (ರಚನೆ custom_handler_manager_s *manager,
                            libcrun_container_t *container,
                            ರಚನೆ custom_handler_instance_s **ಹೊರಹೋಗು,
                            libcrun_error_t *err)
{
  size_t i;

  memset (ಹೊರಹೋಗು, 0, ಗಾತ್ರ (*ಹೊರಹೋಗು));

  ಗಾಗಿ (i = 0; i < manager->handlers_len; i++)
    {
      ಪೂರ್ಣಾಂಕ ret;

      ಆದರೆ (manager->handlers[i]->can_handle_container == ಖಾಲಿ)
        ಮುಂದುವರಿ;

      ret = manager->handlers[i]->can_handle_container (container, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ಆದರೆ (ret)
        {
          *ಹೊರಹೋಗು = make_custom_handler_instance_s (manager->handlers[i]);
          ಆದರೆ ((*ಹೊರಹೋಗು)->vtable->ತುಂಬು)
            ಹಿಂತಿರುಗಿ (*ಹೊರಹೋಗು)->vtable->ತುಂಬು (&((*ಹೊರಹೋಗು)->cookie), err);

          ಹಿಂತಿರುಗಿ 0;
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_configure_handler (ರಚನೆ custom_handler_manager_s *manager,
                           libcrun_context_t *context,
                           libcrun_container_t *container,
                           ರಚನೆ custom_handler_instance_s **ಹೊರಹೋಗು,
                           libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *explicit_handler;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *annotation;
  ಪೂರ್ಣಾಂಕ ret;

  *ಹೊರಹೋಗು = ಖಾಲಿ;

  // Kubernetes sandbox containers must be executed as regular process
  // Example sandbox container can contain pause process
  // See: https://github.com/containers/crun/issues/798
  // before invoking handler check if this is not a kubernetes sandbox
  annotation = ಕಂಡುಹಿಡಿ_annotation (container, "io.kubernetes.cri.container-type");
  ಆದರೆ (annotation && (strcmp (annotation, "sandbox") == 0))
    ಹಿಂತಿರುಗಿ 0;

  annotation = ಕಂಡುಹಿಡಿ_annotation (container, "run.oci.handler");

  /* Fail with EACCESS if global handler is already configured and there was an attempt to override it via spec.  */
  ಆದರೆ (context->handler != ಖಾಲಿ && annotation != ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, EACCES, "invalid attempt to override already configured global handler: `%s`", context->handler);

  explicit_handler = context->handler ? context->handler : annotation;

  /* If an explicit handler was requested, use it.  */
  ಆದರೆ (explicit_handler)
    {
      ರಚನೆ custom_handler_s *h;

      ಆದರೆ (manager == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "handler requested but no manager configured: `%s`", explicit_handler);

      h = handler_by_name (manager, explicit_handler);
      ಆದರೆ (h)
        {
          *ಹೊರಹೋಗು = make_custom_handler_instance_s (h);
          ಆದರೆ ((*ಹೊರಹೋಗು)->vtable->ತುಂಬು)
            ret = (*ಹೊರಹೋಗು)->vtable->ತುಂಬು (&((*ಹೊರಹೋಗು)->cookie), err);
          ಇಲ್ಲದಿದ್ದರೆ
            ret = 0;
          ಹೋಗು ಹೊರಹೋಗು;
        }
    }

  ಆದರೆ (manager == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ret = ಕಂಡುಹಿಡಿ_handler_for_container (manager, container, ಹೊರಹೋಗು, err);

ಹೊರಹೋಗು:
  ಆದರೆ (ret == 0 && *ಹೊರಹೋಗು != ಖಾಲಿ && container->context != ಖಾಲಿ && ! (*ಹೊರಹೋಗು)->vtable->supports_open_ವೃಕ್ಷ_namespace)
    container->context->no_open_ವೃಕ್ಷ_namespace = true;

  ಹಿಂತಿರುಗಿ ret;
}
