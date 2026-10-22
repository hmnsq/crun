/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2020 Giuseppe Scrivano <giuseppe@scrivano.org>
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
#समावेश <config.h>
#समावेश <errno.h>

#अगर HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
#  include <seccomp.h>
#  include <sys/ioctl.h>
#  include <linux/seccomp.h>
#  include <sys/sysmacros.h>
#पूर्ण_अगर

#अगर_घोषित HAVE_DLOPEN
#  include <dlfcn.h>
#पूर्ण_अगर

#समावेश "utils.h"
#समावेश "seccomp_notify.h"

#अगर_अघोषित SECCOMP_USER_NOTIF_FLAG_CONTINUE
#  define SECCOMP_USER_NOTIF_FLAG_CONTINUE (1UL << 0)
#पूर्ण_अगर

काष्ठा plugin
{
  व्योम *handle;
  व्योम *opaque;
#अगर HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  run_oci_seccomp_notify_handle_request_cb handle_request_cb;
#पूर्ण_अगर
};

काष्ठा seccomp_notify_context_s
{
  काष्ठा plugin *plugins;
  माप_प्रकार n_plugins;

#अगर HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES
  काष्ठा seccomp_notif_resp *sresp;
  काष्ठा seccomp_notif *sreq;
  काष्ठा seccomp_notif_sizes sizes;
#पूर्ण_अगर
};

व्योम
cleanup_seccomp_notify_pluginsp (व्योम *p)
{
  काष्ठा seccomp_notify_context_s **pp = p;
  अगर (*pp)
    {
      libcrun_error_t tmp_err = शून्य;
      libcrun_free_seccomp_notify_plugins (*pp, &tmp_err);
      crun_error_release (&tmp_err);
      *pp = शून्य;
    }
}

#अगर HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
अटल पूर्णांक
seccomp_syscall (अचिन्हित पूर्णांक op, अचिन्हित पूर्णांक flags, व्योम *args)
{
  त्रुटि_सं = 0;
  वापस syscall (__NR_seccomp, op, flags, args);
}
#पूर्ण_अगर

LIBCRUN_PUBLIC पूर्णांक
libcrun_load_seccomp_notify_plugins (काष्ठा seccomp_notify_context_s **out, स्थिर अक्षर *plugins,
                                     काष्ठा libcrun_load_seccomp_notify_conf_s *conf, libcrun_error_t *err)
{
#अगर HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  cleanup_seccomp_notify_context काष्ठा seccomp_notify_context_s *ctx = xmalloc0 (माप *ctx);
  cleanup_free अक्षर *b = शून्य;
  अक्षर *it, *saveptr;
  माप_प्रकार s;

  अगर (seccomp_syscall (SECCOMP_GET_NOTIF_SIZES, 0, &ctx->sizes) < 0)
    वापस crun_make_error (err, त्रुटि_सं, "seccomp GET_NOTIF_SIZES");

  ctx->sreq = xmalloc (ctx->sizes.seccomp_notif);
  ctx->sresp = xmalloc (ctx->sizes.seccomp_notif_resp);

  अगर (is_empty_string (plugins))
    वापस 0;

  b = xstrdup (plugins);

  ctx->n_plugins = 1;
  क्रम (it = b; *it; it++)
    अगर (*it == ':')
      ctx->n_plugins++;

  ctx->plugins = xmalloc0 (माप (काष्ठा plugin) * (ctx->n_plugins + 1));
  क्रम (s = 0, it = strtok_r (b, ":", &saveptr); it; s++, it = strtok_r (शून्य, ":", &saveptr))
    {
      run_oci_seccomp_notify_plugin_version_cb version_cb;
      run_oci_seccomp_notify_start_cb start_cb;
      व्योम *opq = शून्य;

      /* do not accept relative paths.  It is fine to accept only filenames as dlopen() semantics apply.  */
      अगर (म_अक्षर (it, '/') && it[0] != '/')
        वापस crun_make_error (err, 0, "invalid relative plugin path: `%s`", it);

      ctx->plugins[s].handle = dlopen (it, RTLD_NOW);
      अगर (ctx->plugins[s].handle == शून्य)
        वापस crun_make_error (err, 0, "cannot load `%s`: %s", it, dlerror ());

      version_cb
          = (run_oci_seccomp_notify_plugin_version_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_version");
      अगर (version_cb != शून्य)
        {
          पूर्णांक version;

          version = version_cb ();
          अगर (version != 1)
            वापस crun_make_error (err, ENOTSUP, "invalid version supported by the plugin `%s`", it);
        }

      ctx->plugins[s].handle_request_cb = (run_oci_seccomp_notify_handle_request_cb) dlsym (
          ctx->plugins[s].handle, "run_oci_seccomp_notify_handle_request");
      अगर (ctx->plugins[s].handle_request_cb == शून्य)
        वापस crun_make_error (err, ENOTSUP, "plugin `%s` doesn't export `run_oci_seccomp_notify_handle_request`", it);

      start_cb = (run_oci_seccomp_notify_start_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_start");
      अगर (start_cb)
        {
          पूर्णांक ret;

          ret = start_cb (&opq, conf, माप (*conf));
          अगर (UNLIKELY (ret != 0))
            वापस crun_make_error (err, 0, "error loading `%s`", it);
        }
      ctx->plugins[s].opaque = opq;
    }

  /* Change ownership.  */
  *out = ctx;
  ctx = शून्य;
  वापस 0;
#अन्यथा
  (व्योम) out;
  (व्योम) plugins;
  (व्योम) conf;
  (व्योम) err;
  वापस crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#पूर्ण_अगर
}

LIBCRUN_PUBLIC पूर्णांक
libcrun_seccomp_notify_plugins (काष्ठा seccomp_notify_context_s *ctx, पूर्णांक seccomp_fd, libcrun_error_t *err)
{
#अगर HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  माप_प्रकार i;
  पूर्णांक ret;

  स_रखो (ctx->sreq, 0, ctx->sizes.seccomp_notif);
  स_रखो (ctx->sresp, 0, ctx->sizes.seccomp_notif_resp);

  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_RECV, ctx->sreq);
  अगर (UNLIKELY (ret < 0))
    {
      अगर (त्रुटि_सं == ENOENT)
        वापस 0;
      वापस crun_make_error (err, त्रुटि_सं, "ioctl");
    }

  क्रम (i = 0; i < ctx->n_plugins; i++)
    {
      अगर (ctx->plugins[i].handle_request_cb)
        {
          पूर्णांक handled = 0;
          पूर्णांक ret;

          ret = ctx->plugins[i].handle_request_cb (ctx->plugins[i].opaque, &ctx->sizes, ctx->sreq, ctx->sresp,
                                                   seccomp_fd, &handled);
          अगर (UNLIKELY (ret != 0))
            वापस crun_make_error (err, 0, "error handling seccomp notify request");

          चयन (handled)
            {
            हाल RUN_OCI_SECCOMP_NOTIFY_HANDLE_NOT_HANDLED:
              अवरोध;

            हाल RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE:
              जाओ send_resp;

              /* The plugin will take care of it.  */
            हाल RUN_OCI_SECCOMP_NOTIFY_HANDLE_DELAYED_RESPONSE:
              वापस 0;

            हाल RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE_AND_CONTINUE:
              ctx->sresp->flags |= SECCOMP_USER_NOTIF_FLAG_CONTINUE;
              जाओ send_resp;

            शेष:
              वापस crun_make_error (err, EINVAL, "unknown action specified by the plugin `%d`", handled);
            }
        }
    }

  /* No plugin could handle the request.  */
  ctx->sresp->error = -ENOTSUP;
  ctx->sresp->flags = 0;

send_resp:
  ctx->sresp->id = ctx->sreq->id;
  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_SEND, ctx->sresp);
  अगर (UNLIKELY (ret < 0))
    {
      अगर (त्रुटि_सं == ENOENT)
        वापस 0;
      वापस crun_make_error (err, त्रुटि_सं, "ioctl");
    }
  वापस 0;
#अन्यथा
  (व्योम) ctx;
  (व्योम) seccomp_fd;
  (व्योम) err;
  वापस crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#पूर्ण_अगर
}

LIBCRUN_PUBLIC पूर्णांक
libcrun_free_seccomp_notify_plugins (काष्ठा seccomp_notify_context_s *ctx, libcrun_error_t *err)
{
#अगर HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  माप_प्रकार i;

  अगर (ctx == शून्य)
    वापस crun_make_error (err, EINVAL, "invalid seccomp notify context");

  मुक्त (ctx->sreq);
  मुक्त (ctx->sresp);

  क्रम (i = 0; i < ctx->n_plugins; i++)
    अगर (ctx->plugins && ctx->plugins[i].handle)
      {
        run_oci_seccomp_notify_stop_cb cb;

        cb = (run_oci_seccomp_notify_stop_cb) dlsym (ctx->plugins[i].handle, "run_oci_seccomp_notify_stop");
        अगर (cb)
          cb (ctx->plugins[i].opaque);
        dlclose (ctx->plugins[i].handle);
      }

  मुक्त (ctx->plugins);
  मुक्त (ctx);

  वापस 0;
#अन्यथा
  (व्योम) ctx;
  (व्योम) err;
  वापस crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#पूर्ण_अगर
}
