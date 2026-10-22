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
#تعریف_کرو GNU_ماخذ

#شامل <config.h>
#شامل "custom-handler.h"
#شامل "container.h"
#شامل "utils.h"
#شامل "linux.h"
#شامل <unistd.h>
#شامل <sys/stat.h>
#شامل <errno.h>
#شامل <sys/types.h>
#شامل <fcntl.h>
#شامل <sched.h>

#اگر_تعریف_شدہ HAVE_DLOPEN
#  include <dlfcn.h>
#اگر_ختم

#اگر HAVE_DLOPEN && HAVE_LIBKRUN
خارجی ساخت custom_handler_s handler_libkrun;
#اگر_ختم
#اگر HAVE_DLOPEN && HAVE_WASMTIME
خارجی ساخت custom_handler_s handler_wasmtime;
#اگر_ختم
#اگر HAVE_DLOPEN && HAVE_WASMEDGE
خارجی ساخت custom_handler_s handler_wasmedge;
#اگر_ختم
#اگر HAVE_DLOPEN && HAVE_WASMER
خارجی ساخت custom_handler_s handler_wasmer;
#اگر_ختم
#اگر HAVE_DLOPEN && HAVE_WAMR
خارجی ساخت custom_handler_s handler_wamr;
#اگر_ختم
#اگر HAVE_DLOPEN && HAVE_MONO
خارجی ساخت custom_handler_s handler_mono;
#اگر_ختم
#اگر HAVE_DLOPEN && HAVE_SPIN
خارجی ساخت custom_handler_s handler_spin;
#اگر_ختم

ساکن ساخت custom_handler_s *static_handlers[] = {
#اگر HAVE_DLOPEN && HAVE_LIBKRUN
  &handler_libkrun,
#اگر_ختم
#اگر HAVE_DLOPEN && HAVE_WASMEDGE
  &handler_wasmedge,
#اگر_ختم
#اگر HAVE_DLOPEN && HAVE_WASMER
  &handler_wasmer,
#اگر_ختم
#اگر HAVE_DLOPEN && HAVE_WASMTIME
  &handler_wasmtime,
#اگر_ختم
#اگر HAVE_DLOPEN && HAVE_WAMR
  &handler_wamr,
#اگر_ختم
#اگر HAVE_DLOPEN && HAVE_MONO
  &handler_mono,
#اگر_ختم
#اگر HAVE_DLOPEN && HAVE_SPIN
  &handler_spin,
#اگر_ختم
  تہی,
};

ساخت custom_handler_manager_s
{
  ساخت custom_handler_s **handlers;
  خالی **handles;
  size_t handlers_len;
};

ساخت custom_handler_manager_s *
libcrun_handler_manager_create (libcrun_error_t *err arg_غیر_مستعمل)
{
  ساخت custom_handler_s **handlers = تہی;
  خالی **handles = تہی;
  ساخت custom_handler_manager_s *m;
  size_t i, handlers_len;

  /* Count the static handlers.  */
  برائے (handlers_len = 0; static_handlers[handlers_len]; handlers_len++)
    ;

  اگر (handlers_len)
    {
      handlers = xmalloc (حجم (ساخت custom_handler_s *) * handlers_len);
      handles = xmalloc0 (حجم (خالی *) * handlers_len);
    }

  برائے (i = 0; i < handlers_len; i++)
    handlers[i] = static_handlers[i];

  m = xmalloc0 (حجم (ساخت custom_handler_manager_s));
  m->handlers = handlers;
  m->handles = handles;
  m->handlers_len = handlers_len;

  واپس m;
}

خالی
handler_manager_free (ساخت custom_handler_manager_s *manager)
{
  size_t i;

  اگر (manager == تہی)
    واپس;

  برائے (i = 0; i < manager->handlers_len; i++)
    {
#اگر_تعریف_شدہ HAVE_DLOPEN
      اگر (manager->handles[i])
        dlclose (manager->handles[i]);
#اگر_ختم
    }
  آزاد (manager->handlers);
  آزاد (manager->handles);
  آزاد (manager);
}

#اگر_تعریف_شدہ HAVE_DLOPEN
ساکن صحیح
handler_manager_جوڑو_so (ساخت custom_handler_manager_s *manager, خالی *handle, libcrun_error_t *err)
{
  ساخت custom_handler_s *h = تہی;
  تسلسل_oci_get_handler_cb cb;

  cb = (تسلسل_oci_get_handler_cb) dlsym (handle, "run_oci_handler_get_handler");
  اگر (UNLIKELY (cb == تہی))
    واپس crun_make_error (err, 0, "cannot find symbol `run_oci_handler_get_handler`");

  h = cb ();
  اگر (UNLIKELY (h == تہی))
    واپس crun_make_error (err, 0, "the callback `run_oci_handler_get_handler` didn't return a handler");

  manager->handlers = xrealloc (manager->handlers, حجم (ساخت custom_handler_s *) * (manager->handlers_len + 1));
  manager->handles = xrealloc (manager->handles, حجم (خالی *) * (manager->handlers_len + 1));

  manager->handlers[manager->handlers_len] = h;
  manager->handles[manager->handlers_len] = handle;
  manager->handlers_len++;
  واپس 0;
}
#اگر_ختم

صحیح
libcrun_handler_manager_بھرو_directory (ساخت custom_handler_manager_s *manager, ثابت حرف *راستہ, libcrun_error_t *err)
{
#اگر_تعریف_شدہ HAVE_DLOPEN
  cleanup_dir DIR *dir = تہی;
  ساخت dirent *اگلا;

  dir = opendir (راستہ);
  اگر (UNLIKELY (dir == تہی))
    واپس crun_make_error (err, errno, "cannot opendir `%s`", راستہ);

  برائے (اگلا = readdir (dir); اگلا; اگلا = readdir (dir))
    {
      cleanup_free حرف *fpath = تہی;
      ثابت حرف *name;
      خالی *handle;
      صحیح ret;

      name = اگلا->d_name;
      اگر (name[0] == '.')
        جاری;

      ret = append_paths (&fpath, err, راستہ, name, تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      handle = dlopen (fpath, RTLD_NOW);
      اگر (UNLIKELY (handle == تہی))
        واپس crun_make_error (err, 0, "cannot load `%s`: `%s`", fpath, dlerror ());

      ret = handler_manager_جوڑو_so (manager, handle, err);
      اگر (UNLIKELY (ret < 0))
        {
          dlclose (handle);
          واپس ret;
        }
    }
  واپس 0;
#ورنہ
  (خالی) manager;
  (خالی) راستہ;
  واپس crun_make_error (err, ENOTSUP, "dlopen not available");
#اگر_ختم
}

ساخت custom_handler_s *
handler_by_name (ساخت custom_handler_manager_s *manager, ثابت حرف *name)
{
  size_t i;

  برائے (i = 0; i < manager->handlers_len; i++)
    {
      اگر (strcmp (manager->handlers[i]->name, name) == 0)
        واپس manager->handlers[i];
      اگر (manager->handlers[i]->alias && strcmp (manager->handlers[i]->alias, name) == 0)
        واپس manager->handlers[i];
    }
  واپس تہی;
}

خالی
libcrun_handler_manager_print_feature_لیبل_فہرست (ساخت custom_handler_manager_s *manager, FILE *برآمد)
{
  size_t i;

  برائے (i = 0; i < manager->handlers_len; i++)
    اگر (manager->handlers[i]->feature_string)
      fprintf (برآمد, "+%s ", manager->handlers[i]->feature_string);
}

حرف **
libcrun_context_get_handler_feature_لیبل_فہرست (libcrun_context_t *ctx, libcrun_error_t *err arg_غیر_مستعمل)
{
  ساخت custom_handler_manager_s *manager;
  حرف **برآمد;
  size_t i, n = 0;

  اگر (ctx == تہی || ctx->handler_manager == تہی)
    واپس تہی;

  manager = ctx->handler_manager;

  برائے (i = 0; i < manager->handlers_len; i++)
    اگر (manager->handlers[i]->feature_string)
      n++;

  اگر (n == 0)
    واپس تہی;

  برآمد = xmalloc0 ((n + 1) * حجم (حرف *));
  n = 0;
  برائے (i = 0; i < manager->handlers_len; i++)
    اگر (manager->handlers[i]->feature_string)
      برآمد[n++] = xstrdup (manager->handlers[i]->feature_string);

  واپس برآمد;
}

ساکن اندرونی ساخت custom_handler_instance_s *
make_custom_handler_instance_s (ساخت custom_handler_s *vtable)
{
  ساخت custom_handler_instance_s *ret = xmalloc0 (حجم (ساخت custom_handler_instance_s));

  ret->vtable = vtable;
  ret->cookie = تہی;

  واپس ret;
}

ساکن صحیح
ڈھونڈو_handler_for_container (ساخت custom_handler_manager_s *manager,
                            libcrun_container_t *container,
                            ساخت custom_handler_instance_s **برآمد,
                            libcrun_error_t *err)
{
  size_t i;

  memset (برآمد, 0, حجم (*برآمد));

  برائے (i = 0; i < manager->handlers_len; i++)
    {
      صحیح ret;

      اگر (manager->handlers[i]->can_handle_container == تہی)
        جاری;

      ret = manager->handlers[i]->can_handle_container (container, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      اگر (ret)
        {
          *برآمد = make_custom_handler_instance_s (manager->handlers[i]);
          اگر ((*برآمد)->vtable->بھرو)
            واپس (*برآمد)->vtable->بھرو (&((*برآمد)->cookie), err);

          واپس 0;
        }
    }

  واپس 0;
}

صحیح
libcrun_configure_handler (ساخت custom_handler_manager_s *manager,
                           libcrun_context_t *context,
                           libcrun_container_t *container,
                           ساخت custom_handler_instance_s **برآمد,
                           libcrun_error_t *err)
{
  ثابت حرف *explicit_handler;
  ثابت حرف *annotation;
  صحیح ret;

  *برآمد = تہی;

  // Kubernetes sandbox containers must be executed as regular process
  // Example sandbox container can contain pause process
  // See: https://github.com/containers/crun/issues/798
  // before invoking handler check if this is not a kubernetes sandbox
  annotation = ڈھونڈو_annotation (container, "io.kubernetes.cri.container-type");
  اگر (annotation && (strcmp (annotation, "sandbox") == 0))
    واپس 0;

  annotation = ڈھونڈو_annotation (container, "run.oci.handler");

  /* Fail with EACCESS if global handler is already configured and there was an attempt to override it via spec.  */
  اگر (context->handler != تہی && annotation != تہی)
    واپس crun_make_error (err, EACCES, "invalid attempt to override already configured global handler: `%s`", context->handler);

  explicit_handler = context->handler ? context->handler : annotation;

  /* If an explicit handler was requested, use it.  */
  اگر (explicit_handler)
    {
      ساخت custom_handler_s *h;

      اگر (manager == تہی)
        واپس crun_make_error (err, 0, "handler requested but no manager configured: `%s`", explicit_handler);

      h = handler_by_name (manager, explicit_handler);
      اگر (h)
        {
          *برآمد = make_custom_handler_instance_s (h);
          اگر ((*برآمد)->vtable->بھرو)
            ret = (*برآمد)->vtable->بھرو (&((*برآمد)->cookie), err);
          ورنہ
            ret = 0;
          جاؤ برآمد;
        }
    }

  اگر (manager == تہی)
    واپس 0;

  ret = ڈھونڈو_handler_for_container (manager, container, برآمد, err);

برآمد:
  اگر (ret == 0 && *برآمد != تہی && container->context != تہی && ! (*برآمد)->vtable->supports_open_درخت_namespace)
    container->context->no_open_درخت_namespace = true;

  واپس ret;
}
