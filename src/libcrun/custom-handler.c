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
#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ "custom-handler.h"
#అన్తర్భావ "container.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "linux.h"
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <errno.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <sched.h>

#నిర్వచితమైతే HAVE_DLOPEN
#  include <dlfcn.h>
#అయితే_ముగింపు

#ఒకవేళ HAVE_DLOPEN && HAVE_LIBKRUN
బాహ్య నిర్మాణం custom_handler_s handler_libkrun;
#అయితే_ముగింపు
#ఒకవేళ HAVE_DLOPEN && HAVE_WASMTIME
బాహ్య నిర్మాణం custom_handler_s handler_wasmtime;
#అయితే_ముగింపు
#ఒకవేళ HAVE_DLOPEN && HAVE_WASMEDGE
బాహ్య నిర్మాణం custom_handler_s handler_wasmedge;
#అయితే_ముగింపు
#ఒకవేళ HAVE_DLOPEN && HAVE_WASMER
బాహ్య నిర్మాణం custom_handler_s handler_wasmer;
#అయితే_ముగింపు
#ఒకవేళ HAVE_DLOPEN && HAVE_WAMR
బాహ్య నిర్మాణం custom_handler_s handler_wamr;
#అయితే_ముగింపు
#ఒకవేళ HAVE_DLOPEN && HAVE_MONO
బాహ్య నిర్మాణం custom_handler_s handler_mono;
#అయితే_ముగింపు
#ఒకవేళ HAVE_DLOPEN && HAVE_SPIN
బాహ్య నిర్మాణం custom_handler_s handler_spin;
#అయితే_ముగింపు

స్థిర నిర్మాణం custom_handler_s *static_handlers[] = {
#ఒకవేళ HAVE_DLOPEN && HAVE_LIBKRUN
  &handler_libkrun,
#అయితే_ముగింపు
#ఒకవేళ HAVE_DLOPEN && HAVE_WASMEDGE
  &handler_wasmedge,
#అయితే_ముగింపు
#ఒకవేళ HAVE_DLOPEN && HAVE_WASMER
  &handler_wasmer,
#అయితే_ముగింపు
#ఒకవేళ HAVE_DLOPEN && HAVE_WASMTIME
  &handler_wasmtime,
#అయితే_ముగింపు
#ఒకవేళ HAVE_DLOPEN && HAVE_WAMR
  &handler_wamr,
#అయితే_ముగింపు
#ఒకవేళ HAVE_DLOPEN && HAVE_MONO
  &handler_mono,
#అయితే_ముగింపు
#ఒకవేళ HAVE_DLOPEN && HAVE_SPIN
  &handler_spin,
#అయితే_ముగింపు
  ఖాళీ,
};

నిర్మాణం custom_handler_manager_s
{
  నిర్మాణం custom_handler_s **handlers;
  శూన్య **handles;
  size_t handlers_len;
};

నిర్మాణం custom_handler_manager_s *
libcrun_handler_manager_create (libcrun_error_t *err arg_వాడని)
{
  నిర్మాణం custom_handler_s **handlers = ఖాళీ;
  శూన్య **handles = ఖాళీ;
  నిర్మాణం custom_handler_manager_s *m;
  size_t i, handlers_len;

  /* Count the static handlers.  */
  కొరకు (handlers_len = 0; static_handlers[handlers_len]; handlers_len++)
    ;

  ఒకవేళ (handlers_len)
    {
      handlers = xmalloc (పరిమాణం (నిర్మాణం custom_handler_s *) * handlers_len);
      handles = xmalloc0 (పరిమాణం (శూన్య *) * handlers_len);
    }

  కొరకు (i = 0; i < handlers_len; i++)
    handlers[i] = static_handlers[i];

  m = xmalloc0 (పరిమాణం (నిర్మాణం custom_handler_manager_s));
  m->handlers = handlers;
  m->handles = handles;
  m->handlers_len = handlers_len;

  తిరిగి m;
}

శూన్య
handler_manager_free (నిర్మాణం custom_handler_manager_s *manager)
{
  size_t i;

  ఒకవేళ (manager == ఖాళీ)
    తిరిగి;

  కొరకు (i = 0; i < manager->handlers_len; i++)
    {
#నిర్వచితమైతే HAVE_DLOPEN
      ఒకవేళ (manager->handles[i])
        dlclose (manager->handles[i]);
#అయితే_ముగింపు
    }
  విడుదల (manager->handlers);
  విడుదల (manager->handles);
  విడుదల (manager);
}

#నిర్వచితమైతే HAVE_DLOPEN
స్థిర పూర్ణాంకం
handler_manager_కూడు_so (నిర్మాణం custom_handler_manager_s *manager, శూన్య *handle, libcrun_error_t *err)
{
  నిర్మాణం custom_handler_s *h = ఖాళీ;
  నిరంతరం_oci_get_handler_cb cb;

  cb = (నిరంతరం_oci_get_handler_cb) dlsym (handle, "run_oci_handler_get_handler");
  ఒకవేళ (UNLIKELY (cb == ఖాళీ))
    తిరిగి crun_make_error (err, 0, "cannot find symbol `run_oci_handler_get_handler`");

  h = cb ();
  ఒకవేళ (UNLIKELY (h == ఖాళీ))
    తిరిగి crun_make_error (err, 0, "the callback `run_oci_handler_get_handler` didn't return a handler");

  manager->handlers = xrealloc (manager->handlers, పరిమాణం (నిర్మాణం custom_handler_s *) * (manager->handlers_len + 1));
  manager->handles = xrealloc (manager->handles, పరిమాణం (శూన్య *) * (manager->handlers_len + 1));

  manager->handlers[manager->handlers_len] = h;
  manager->handles[manager->handlers_len] = handle;
  manager->handlers_len++;
  తిరిగి 0;
}
#అయితే_ముగింపు

పూర్ణాంకం
libcrun_handler_manager_ఎక్కించు_directory (నిర్మాణం custom_handler_manager_s *manager, స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err)
{
#నిర్వచితమైతే HAVE_DLOPEN
  cleanup_dir DIR *dir = ఖాళీ;
  నిర్మాణం dirent *తదుపరి;

  dir = opendir (మార్గం);
  ఒకవేళ (UNLIKELY (dir == ఖాళీ))
    తిరిగి crun_make_error (err, errno, "cannot opendir `%s`", మార్గం);

  కొరకు (తదుపరి = readdir (dir); తదుపరి; తదుపరి = readdir (dir))
    {
      cleanup_free అక్షరం *fpath = ఖాళీ;
      స్థిరరాశి అక్షరం *name;
      శూన్య *handle;
      పూర్ణాంకం ret;

      name = తదుపరి->d_name;
      ఒకవేళ (name[0] == '.')
        కొనసాగు;

      ret = append_paths (&fpath, err, మార్గం, name, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      handle = dlopen (fpath, RTLD_NOW);
      ఒకవేళ (UNLIKELY (handle == ఖాళీ))
        తిరిగి crun_make_error (err, 0, "cannot load `%s`: `%s`", fpath, dlerror ());

      ret = handler_manager_కూడు_so (manager, handle, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          dlclose (handle);
          తిరిగి ret;
        }
    }
  తిరిగి 0;
#లేకపోతే
  (శూన్య) manager;
  (శూన్య) మార్గం;
  తిరిగి crun_make_error (err, ENOTSUP, "dlopen not available");
#అయితే_ముగింపు
}

నిర్మాణం custom_handler_s *
handler_by_name (నిర్మాణం custom_handler_manager_s *manager, స్థిరరాశి అక్షరం *name)
{
  size_t i;

  కొరకు (i = 0; i < manager->handlers_len; i++)
    {
      ఒకవేళ (strcmp (manager->handlers[i]->name, name) == 0)
        తిరిగి manager->handlers[i];
      ఒకవేళ (manager->handlers[i]->alias && strcmp (manager->handlers[i]->alias, name) == 0)
        తిరిగి manager->handlers[i];
    }
  తిరిగి ఖాళీ;
}

శూన్య
libcrun_handler_manager_print_feature_ట్యాగులు (నిర్మాణం custom_handler_manager_s *manager, FILE *బహిర్గతం)
{
  size_t i;

  కొరకు (i = 0; i < manager->handlers_len; i++)
    ఒకవేళ (manager->handlers[i]->feature_string)
      fprintf (బహిర్గతం, "+%s ", manager->handlers[i]->feature_string);
}

అక్షరం **
libcrun_context_get_handler_feature_ట్యాగులు (libcrun_context_t *ctx, libcrun_error_t *err arg_వాడని)
{
  నిర్మాణం custom_handler_manager_s *manager;
  అక్షరం **బహిర్గతం;
  size_t i, n = 0;

  ఒకవేళ (ctx == ఖాళీ || ctx->handler_manager == ఖాళీ)
    తిరిగి ఖాళీ;

  manager = ctx->handler_manager;

  కొరకు (i = 0; i < manager->handlers_len; i++)
    ఒకవేళ (manager->handlers[i]->feature_string)
      n++;

  ఒకవేళ (n == 0)
    తిరిగి ఖాళీ;

  బహిర్గతం = xmalloc0 ((n + 1) * పరిమాణం (అక్షరం *));
  n = 0;
  కొరకు (i = 0; i < manager->handlers_len; i++)
    ఒకవేళ (manager->handlers[i]->feature_string)
      బహిర్గతం[n++] = xstrdup (manager->handlers[i]->feature_string);

  తిరిగి బహిర్గతం;
}

స్థిర అంతఃవరుస నిర్మాణం custom_handler_instance_s *
make_custom_handler_instance_s (నిర్మాణం custom_handler_s *vtable)
{
  నిర్మాణం custom_handler_instance_s *ret = xmalloc0 (పరిమాణం (నిర్మాణం custom_handler_instance_s));

  ret->vtable = vtable;
  ret->cookie = ఖాళీ;

  తిరిగి ret;
}

స్థిర పూర్ణాంకం
కనుగొను_handler_for_container (నిర్మాణం custom_handler_manager_s *manager,
                            libcrun_container_t *container,
                            నిర్మాణం custom_handler_instance_s **బహిర్గతం,
                            libcrun_error_t *err)
{
  size_t i;

  memset (బహిర్గతం, 0, పరిమాణం (*బహిర్గతం));

  కొరకు (i = 0; i < manager->handlers_len; i++)
    {
      పూర్ణాంకం ret;

      ఒకవేళ (manager->handlers[i]->can_handle_container == ఖాళీ)
        కొనసాగు;

      ret = manager->handlers[i]->can_handle_container (container, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ఒకవేళ (ret)
        {
          *బహిర్గతం = make_custom_handler_instance_s (manager->handlers[i]);
          ఒకవేళ ((*బహిర్గతం)->vtable->ఎక్కించు)
            తిరిగి (*బహిర్గతం)->vtable->ఎక్కించు (&((*బహిర్గతం)->cookie), err);

          తిరిగి 0;
        }
    }

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_configure_handler (నిర్మాణం custom_handler_manager_s *manager,
                           libcrun_context_t *context,
                           libcrun_container_t *container,
                           నిర్మాణం custom_handler_instance_s **బహిర్గతం,
                           libcrun_error_t *err)
{
  స్థిరరాశి అక్షరం *explicit_handler;
  స్థిరరాశి అక్షరం *annotation;
  పూర్ణాంకం ret;

  *బహిర్గతం = ఖాళీ;

  // Kubernetes sandbox containers must be executed as regular process
  // Example sandbox container can contain pause process
  // See: https://github.com/containers/crun/issues/798
  // before invoking handler check if this is not a kubernetes sandbox
  annotation = కనుగొను_annotation (container, "io.kubernetes.cri.container-type");
  ఒకవేళ (annotation && (strcmp (annotation, "sandbox") == 0))
    తిరిగి 0;

  annotation = కనుగొను_annotation (container, "run.oci.handler");

  /* Fail with EACCESS if global handler is already configured and there was an attempt to override it via spec.  */
  ఒకవేళ (context->handler != ఖాళీ && annotation != ఖాళీ)
    తిరిగి crun_make_error (err, EACCES, "invalid attempt to override already configured global handler: `%s`", context->handler);

  explicit_handler = context->handler ? context->handler : annotation;

  /* If an explicit handler was requested, use it.  */
  ఒకవేళ (explicit_handler)
    {
      నిర్మాణం custom_handler_s *h;

      ఒకవేళ (manager == ఖాళీ)
        తిరిగి crun_make_error (err, 0, "handler requested but no manager configured: `%s`", explicit_handler);

      h = handler_by_name (manager, explicit_handler);
      ఒకవేళ (h)
        {
          *బహిర్గతం = make_custom_handler_instance_s (h);
          ఒకవేళ ((*బహిర్గతం)->vtable->ఎక్కించు)
            ret = (*బహిర్గతం)->vtable->ఎక్కించు (&((*బహిర్గతం)->cookie), err);
          లేకపోతే
            ret = 0;
          వెళ్ళు బహిర్గతం;
        }
    }

  ఒకవేళ (manager == ఖాళీ)
    తిరిగి 0;

  ret = కనుగొను_handler_for_container (manager, container, బహిర్గతం, err);

బహిర్గతం:
  ఒకవేళ (ret == 0 && *బహిర్గతం != ఖాళీ && container->context != ఖాళీ && ! (*బహిర్గతం)->vtable->supports_open_వృక్షం_namespace)
    container->context->no_open_వృక్షం_namespace = true;

  తిరిగి ret;
}
