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
#تعریف_کرو GNU_سروت

#ضمن <config.h>
#ضمن "custom-handler.h"
#ضمن "container.h"
#ضمن "utils.h"
#ضمن "linux.h"
#ضمن <unistd.h>
#ضمن <sys/stat.h>
#ضمن <errno.h>
#ضمن <sys/types.h>
#ضمن <fcntl.h>
#ضمن <sched.h>

#جے_تعریف_شدہ HAVE_DLOPEN
#  include <dlfcn.h>
#جے_انت

#إذا HAVE_DLOPEN && HAVE_LIBKRUN
خارجي بنية custom_handler_s handler_libkrun;
#جے_انت
#إذا HAVE_DLOPEN && HAVE_WASMTIME
خارجي بنية custom_handler_s handler_wasmtime;
#جے_انت
#إذا HAVE_DLOPEN && HAVE_WASMEDGE
خارجي بنية custom_handler_s handler_wasmedge;
#جے_انت
#إذا HAVE_DLOPEN && HAVE_WASMER
خارجي بنية custom_handler_s handler_wasmer;
#جے_انت
#إذا HAVE_DLOPEN && HAVE_WAMR
خارجي بنية custom_handler_s handler_wamr;
#جے_انت
#إذا HAVE_DLOPEN && HAVE_MONO
خارجي بنية custom_handler_s handler_mono;
#جے_انت
#إذا HAVE_DLOPEN && HAVE_SPIN
خارجي بنية custom_handler_s handler_spin;
#جے_انت

ساكن بنية custom_handler_s *static_handlers[] = {
#إذا HAVE_DLOPEN && HAVE_LIBKRUN
  &handler_libkrun,
#جے_انت
#إذا HAVE_DLOPEN && HAVE_WASMEDGE
  &handler_wasmedge,
#جے_انت
#إذا HAVE_DLOPEN && HAVE_WASMER
  &handler_wasmer,
#جے_انت
#إذا HAVE_DLOPEN && HAVE_WASMTIME
  &handler_wasmtime,
#جے_انت
#إذا HAVE_DLOPEN && HAVE_WAMR
  &handler_wamr,
#جے_انت
#إذا HAVE_DLOPEN && HAVE_MONO
  &handler_mono,
#جے_انت
#إذا HAVE_DLOPEN && HAVE_SPIN
  &handler_spin,
#جے_انت
  عدم,
};

بنية custom_handler_manager_s
{
  بنية custom_handler_s **handlers;
  فراغ **handles;
  size_t handlers_len;
};

بنية custom_handler_manager_s *
libcrun_handler_manager_create (libcrun_error_t *err arg_بن_ورتیا)
{
  بنية custom_handler_s **handlers = عدم;
  فراغ **handles = عدم;
  بنية custom_handler_manager_s *m;
  size_t i, handlers_len;

  /* Count the static handlers.  */
  لكل (handlers_len = 0; static_handlers[handlers_len]; handlers_len++)
    ;

  إذا (handlers_len)
    {
      handlers = xmalloc (حجم (بنية custom_handler_s *) * handlers_len);
      handles = xmalloc0 (حجم (فراغ *) * handlers_len);
    }

  لكل (i = 0; i < handlers_len; i++)
    handlers[i] = static_handlers[i];

  m = xmalloc0 (حجم (بنية custom_handler_manager_s));
  m->handlers = handlers;
  m->handles = handles;
  m->handlers_len = handlers_len;

  أرجع m;
}

فراغ
handler_manager_free (بنية custom_handler_manager_s *manager)
{
  size_t i;

  إذا (manager == عدم)
    أرجع;

  لكل (i = 0; i < manager->handlers_len; i++)
    {
#جے_تعریف_شدہ HAVE_DLOPEN
      إذا (manager->handles[i])
        dlclose (manager->handles[i]);
#جے_انت
    }
  حرر (manager->handlers);
  حرر (manager->handles);
  حرر (manager);
}

#جے_تعریف_شدہ HAVE_DLOPEN
ساكن صحيح
handler_manager_جوڑو_so (بنية custom_handler_manager_s *manager, فراغ *handle, libcrun_error_t *err)
{
  بنية custom_handler_s *h = عدم;
  لگاتار_oci_get_handler_cb cb;

  cb = (لگاتار_oci_get_handler_cb) dlsym (handle, "run_oci_handler_get_handler");
  إذا (UNLIKELY (cb == عدم))
    أرجع crun_make_error (err, 0, "cannot find symbol `run_oci_handler_get_handler`");

  h = cb ();
  إذا (UNLIKELY (h == عدم))
    أرجع crun_make_error (err, 0, "the callback `run_oci_handler_get_handler` didn't return a handler");

  manager->handlers = xrealloc (manager->handlers, حجم (بنية custom_handler_s *) * (manager->handlers_len + 1));
  manager->handles = xrealloc (manager->handles, حجم (فراغ *) * (manager->handlers_len + 1));

  manager->handlers[manager->handlers_len] = h;
  manager->handles[manager->handlers_len] = handle;
  manager->handlers_len++;
  أرجع 0;
}
#جے_انت

صحيح
libcrun_handler_manager_لدو_directory (بنية custom_handler_manager_s *manager, ثابت حرف *رستہ, libcrun_error_t *err)
{
#جے_تعریف_شدہ HAVE_DLOPEN
  cleanup_dir DIR *dir = عدم;
  بنية dirent *اگلا;

  dir = opendir (رستہ);
  إذا (UNLIKELY (dir == عدم))
    أرجع crun_make_error (err, errno, "cannot opendir `%s`", رستہ);

  لكل (اگلا = readdir (dir); اگلا; اگلا = readdir (dir))
    {
      cleanup_free حرف *fpath = عدم;
      ثابت حرف *name;
      فراغ *handle;
      صحيح ret;

      name = اگلا->d_name;
      إذا (name[0] == '.')
        استمر;

      ret = append_paths (&fpath, err, رستہ, name, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      handle = dlopen (fpath, RTLD_NOW);
      إذا (UNLIKELY (handle == عدم))
        أرجع crun_make_error (err, 0, "cannot load `%s`: `%s`", fpath, dlerror ());

      ret = handler_manager_جوڑو_so (manager, handle, err);
      إذا (UNLIKELY (ret < 0))
        {
          dlclose (handle);
          أرجع ret;
        }
    }
  أرجع 0;
#وإلا
  (فراغ) manager;
  (فراغ) رستہ;
  أرجع crun_make_error (err, ENOTSUP, "dlopen not available");
#جے_انت
}

بنية custom_handler_s *
handler_by_name (بنية custom_handler_manager_s *manager, ثابت حرف *name)
{
  size_t i;

  لكل (i = 0; i < manager->handlers_len; i++)
    {
      إذا (strcmp (manager->handlers[i]->name, name) == 0)
        أرجع manager->handlers[i];
      إذا (manager->handlers[i]->alias && strcmp (manager->handlers[i]->alias, name) == 0)
        أرجع manager->handlers[i];
    }
  أرجع عدم;
}

فراغ
libcrun_handler_manager_print_feature_ٹیگ_سوچی (بنية custom_handler_manager_s *manager, FILE *باہرلا)
{
  size_t i;

  لكل (i = 0; i < manager->handlers_len; i++)
    إذا (manager->handlers[i]->feature_string)
      fprintf (باہرلا, "+%s ", manager->handlers[i]->feature_string);
}

حرف **
libcrun_context_get_handler_feature_ٹیگ_سوچی (libcrun_context_t *ctx, libcrun_error_t *err arg_بن_ورتیا)
{
  بنية custom_handler_manager_s *manager;
  حرف **باہرلا;
  size_t i, n = 0;

  إذا (ctx == عدم || ctx->handler_manager == عدم)
    أرجع عدم;

  manager = ctx->handler_manager;

  لكل (i = 0; i < manager->handlers_len; i++)
    إذا (manager->handlers[i]->feature_string)
      n++;

  إذا (n == 0)
    أرجع عدم;

  باہرلا = xmalloc0 ((n + 1) * حجم (حرف *));
  n = 0;
  لكل (i = 0; i < manager->handlers_len; i++)
    إذا (manager->handlers[i]->feature_string)
      باہرلا[n++] = xstrdup (manager->handlers[i]->feature_string);

  أرجع باہرلا;
}

ساكن مضمن بنية custom_handler_instance_s *
make_custom_handler_instance_s (بنية custom_handler_s *vtable)
{
  بنية custom_handler_instance_s *ret = xmalloc0 (حجم (بنية custom_handler_instance_s));

  ret->vtable = vtable;
  ret->cookie = عدم;

  أرجع ret;
}

ساكن صحيح
لبھو_handler_for_container (بنية custom_handler_manager_s *manager,
                            libcrun_container_t *container,
                            بنية custom_handler_instance_s **باہرلا,
                            libcrun_error_t *err)
{
  size_t i;

  memset (باہرلا, 0, حجم (*باہرلا));

  لكل (i = 0; i < manager->handlers_len; i++)
    {
      صحيح ret;

      إذا (manager->handlers[i]->can_handle_container == عدم)
        استمر;

      ret = manager->handlers[i]->can_handle_container (container, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      إذا (ret)
        {
          *باہرلا = make_custom_handler_instance_s (manager->handlers[i]);
          إذا ((*باہرلا)->vtable->لدو)
            أرجع (*باہرلا)->vtable->لدو (&((*باہرلا)->cookie), err);

          أرجع 0;
        }
    }

  أرجع 0;
}

صحيح
libcrun_configure_handler (بنية custom_handler_manager_s *manager,
                           libcrun_context_t *context,
                           libcrun_container_t *container,
                           بنية custom_handler_instance_s **باہرلا,
                           libcrun_error_t *err)
{
  ثابت حرف *explicit_handler;
  ثابت حرف *annotation;
  صحيح ret;

  *باہرلا = عدم;

  // Kubernetes sandbox containers must be executed as regular process
  // Example sandbox container can contain pause process
  // See: https://github.com/containers/crun/issues/798
  // before invoking handler check if this is not a kubernetes sandbox
  annotation = لبھو_annotation (container, "io.kubernetes.cri.container-type");
  إذا (annotation && (strcmp (annotation, "sandbox") == 0))
    أرجع 0;

  annotation = لبھو_annotation (container, "run.oci.handler");

  /* Fail with EACCESS if global handler is already configured and there was an attempt to override it via spec.  */
  إذا (context->handler != عدم && annotation != عدم)
    أرجع crun_make_error (err, EACCES, "invalid attempt to override already configured global handler: `%s`", context->handler);

  explicit_handler = context->handler ? context->handler : annotation;

  /* If an explicit handler was requested, use it.  */
  إذا (explicit_handler)
    {
      بنية custom_handler_s *h;

      إذا (manager == عدم)
        أرجع crun_make_error (err, 0, "handler requested but no manager configured: `%s`", explicit_handler);

      h = handler_by_name (manager, explicit_handler);
      إذا (h)
        {
          *باہرلا = make_custom_handler_instance_s (h);
          إذا ((*باہرلا)->vtable->لدو)
            ret = (*باہرلا)->vtable->لدو (&((*باہرلا)->cookie), err);
          وإلا
            ret = 0;
          اذهب باہرلا;
        }
    }

  إذا (manager == عدم)
    أرجع 0;

  ret = لبھو_handler_for_container (manager, container, باہرلا, err);

باہرلا:
  إذا (ret == 0 && *باہرلا != عدم && container->context != عدم && ! (*باہرلا)->vtable->supports_open_درخت_namespace)
    container->context->no_open_درخت_namespace = true;

  أرجع ret;
}
