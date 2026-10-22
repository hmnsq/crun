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
#अन्तर्भाव <config.h>
#अन्तर्भाव <errno.h>

#यदि HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
#  include <seccomp.h>
#  include <sys/ioctl.h>
#  include <linux/seccomp.h>
#  include <sys/sysmacros.h>
#endif

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#अन्तर्भाव "utils.h"
#अन्तर्भाव "seccomp_notify.h"

#ifndef SECCOMP_USER_NOTIF_FLAG_CONTINUE
#  define SECCOMP_USER_NOTIF_FLAG_CONTINUE (1UL << 0)
#endif

संरचना plugin
{
  शून्य *handle;
  शून्य *opaque;
#यदि HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  run_oci_seccomp_notify_handle_request_cb handle_request_cb;
#endif
};

संरचना seccomp_notify_context_s
{
  संरचना plugin *plugins;
  माप_प्रकार n_plugins;

#यदि HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES
  संरचना seccomp_notif_resp *sresp;
  संरचना seccomp_notif *sreq;
  संरचना seccomp_notif_sizes sizes;
#endif
};

शून्य
cleanup_seccomp_notify_pluginsp (शून्य *p)
{
  संरचना seccomp_notify_context_s **pp = p;
  यदि (*pp)
    {
      libcrun_error_t tmp_err = रिक्त;
      libcrun_free_seccomp_notify_plugins (*pp, &tmp_err);
      crun_error_release (&tmp_err);
      *pp = रिक्त;
    }
}

#यदि HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
स्थिर पूर्णाङ्क
seccomp_syscall (अचिन्हित पूर्णाङ्क op, अचिन्हित पूर्णाङ्क flags, शून्य *args)
{
  त्रुटि_सं = 0;
  फर्काऊ syscall (__NR_seccomp, op, flags, args);
}
#endif

LIBCRUN_PUBLIC पूर्णाङ्क
libcrun_load_seccomp_notify_plugins (संरचना seccomp_notify_context_s **out, अचर वर्ण *plugins,
                                     संरचना libcrun_load_seccomp_notify_conf_s *conf, libcrun_error_t *err)
{
#यदि HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  cleanup_seccomp_notify_context संरचना seccomp_notify_context_s *ctx = xmalloc0 (परिमाण *ctx);
  cleanup_free वर्ण *b = रिक्त;
  वर्ण *it, *saveptr;
  माप_प्रकार s;

  यदि (seccomp_syscall (SECCOMP_GET_NOTIF_SIZES, 0, &ctx->sizes) < 0)
    फर्काऊ crun_make_error (err, त्रुटि_सं, "seccomp GET_NOTIF_SIZES");

  ctx->sreq = xmalloc (ctx->sizes.seccomp_notif);
  ctx->sresp = xmalloc (ctx->sizes.seccomp_notif_resp);

  यदि (is_empty_string (plugins))
    फर्काऊ 0;

  b = xstrdup (plugins);

  ctx->n_plugins = 1;
  लागि (it = b; *it; it++)
    यदि (*it == ':')
      ctx->n_plugins++;

  ctx->plugins = xmalloc0 (परिमाण (संरचना plugin) * (ctx->n_plugins + 1));
  लागि (s = 0, it = strtok_r (b, ":", &saveptr); it; s++, it = strtok_r (रिक्त, ":", &saveptr))
    {
      run_oci_seccomp_notify_plugin_version_cb version_cb;
      run_oci_seccomp_notify_start_cb start_cb;
      शून्य *opq = रिक्त;

      /* do not accept relative paths.  It is fine to accept only filenames as dlopen() semantics apply.  */
      यदि (म_अक्षर (it, '/') && it[0] != '/')
        फर्काऊ crun_make_error (err, 0, "invalid relative plugin path: `%s`", it);

      ctx->plugins[s].handle = dlopen (it, RTLD_NOW);
      यदि (ctx->plugins[s].handle == रिक्त)
        फर्काऊ crun_make_error (err, 0, "cannot load `%s`: %s", it, dlerror ());

      version_cb
          = (run_oci_seccomp_notify_plugin_version_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_version");
      यदि (version_cb != रिक्त)
        {
          पूर्णाङ्क version;

          version = version_cb ();
          यदि (version != 1)
            फर्काऊ crun_make_error (err, ENOTSUP, "invalid version supported by the plugin `%s`", it);
        }

      ctx->plugins[s].handle_request_cb = (run_oci_seccomp_notify_handle_request_cb) dlsym (
          ctx->plugins[s].handle, "run_oci_seccomp_notify_handle_request");
      यदि (ctx->plugins[s].handle_request_cb == रिक्त)
        फर्काऊ crun_make_error (err, ENOTSUP, "plugin `%s` doesn't export `run_oci_seccomp_notify_handle_request`", it);

      start_cb = (run_oci_seccomp_notify_start_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_start");
      यदि (start_cb)
        {
          पूर्णाङ्क ret;

          ret = start_cb (&opq, conf, परिमाण (*conf));
          यदि (UNLIKELY (ret != 0))
            फर्काऊ crun_make_error (err, 0, "error loading `%s`", it);
        }
      ctx->plugins[s].opaque = opq;
    }

  /* Change ownership.  */
  *out = ctx;
  ctx = रिक्त;
  फर्काऊ 0;
#अन्यथा
  (शून्य) out;
  (शून्य) plugins;
  (शून्य) conf;
  (शून्य) err;
  फर्काऊ crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC पूर्णाङ्क
libcrun_seccomp_notify_plugins (संरचना seccomp_notify_context_s *ctx, पूर्णाङ्क seccomp_fd, libcrun_error_t *err)
{
#यदि HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  माप_प्रकार i;
  पूर्णाङ्क ret;

  स_रखो (ctx->sreq, 0, ctx->sizes.seccomp_notif);
  स_रखो (ctx->sresp, 0, ctx->sizes.seccomp_notif_resp);

  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_RECV, ctx->sreq);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (त्रुटि_सं == ENOENT)
        फर्काऊ 0;
      फर्काऊ crun_make_error (err, त्रुटि_सं, "ioctl");
    }

  लागि (i = 0; i < ctx->n_plugins; i++)
    {
      यदि (ctx->plugins[i].handle_request_cb)
        {
          पूर्णाङ्क handled = 0;
          पूर्णाङ्क ret;

          ret = ctx->plugins[i].handle_request_cb (ctx->plugins[i].opaque, &ctx->sizes, ctx->sreq, ctx->sresp,
                                                   seccomp_fd, &handled);
          यदि (UNLIKELY (ret != 0))
            फर्काऊ crun_make_error (err, 0, "error handling seccomp notify request");

          विकल्प (handled)
            {
            पक्ष RUN_OCI_SECCOMP_NOTIFY_HANDLE_NOT_HANDLED:
              रोक;

            पक्ष RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE:
              गच्छ send_resp;

              /* The plugin will take care of it.  */
            पक्ष RUN_OCI_SECCOMP_NOTIFY_HANDLE_DELAYED_RESPONSE:
              फर्काऊ 0;

            पक्ष RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE_AND_CONTINUE:
              ctx->sresp->flags |= SECCOMP_USER_NOTIF_FLAG_CONTINUE;
              गच्छ send_resp;

            स्वभाव:
              फर्काऊ crun_make_error (err, EINVAL, "unknown action specified by the plugin `%d`", handled);
            }
        }
    }

  /* No plugin could handle the request.  */
  ctx->sresp->error = -ENOTSUP;
  ctx->sresp->flags = 0;

send_resp:
  ctx->sresp->id = ctx->sreq->id;
  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_SEND, ctx->sresp);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (त्रुटि_सं == ENOENT)
        फर्काऊ 0;
      फर्काऊ crun_make_error (err, त्रुटि_सं, "ioctl");
    }
  फर्काऊ 0;
#अन्यथा
  (शून्य) ctx;
  (शून्य) seccomp_fd;
  (शून्य) err;
  फर्काऊ crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC पूर्णाङ्क
libcrun_free_seccomp_notify_plugins (संरचना seccomp_notify_context_s *ctx, libcrun_error_t *err)
{
#यदि HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  माप_प्रकार i;

  यदि (ctx == रिक्त)
    फर्काऊ crun_make_error (err, EINVAL, "invalid seccomp notify context");

  मुक्त (ctx->sreq);
  मुक्त (ctx->sresp);

  लागि (i = 0; i < ctx->n_plugins; i++)
    यदि (ctx->plugins && ctx->plugins[i].handle)
      {
        run_oci_seccomp_notify_stop_cb cb;

        cb = (run_oci_seccomp_notify_stop_cb) dlsym (ctx->plugins[i].handle, "run_oci_seccomp_notify_stop");
        यदि (cb)
          cb (ctx->plugins[i].opaque);
        dlclose (ctx->plugins[i].handle);
      }

  मुक्त (ctx->plugins);
  मुक्त (ctx);

  फर्काऊ 0;
#अन्यथा
  (शून्य) ctx;
  (शून्य) err;
  फर्काऊ crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}
