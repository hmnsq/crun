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
#شامل <config.h>
#شامل <errno.h>

#اگر HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
#  include <seccomp.h>
#  include <sys/ioctl.h>
#  include <linux/seccomp.h>
#  include <sys/sysmacros.h>
#endif

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#شامل "utils.h"
#شامل "seccomp_notify.h"

#ifndef SECCOMP_USER_NOTIF_FLAG_CONTINUE
#  define SECCOMP_USER_NOTIF_FLAG_CONTINUE (1UL << 0)
#endif

ساختار plugin
{
  تهی *handle;
  تهی *opaque;
#اگر HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  run_oci_seccomp_notify_handle_request_cb handle_request_cb;
#endif
};

ساختار seccomp_notify_context_s
{
  ساختار plugin *plugins;
  size_t n_plugins;

#اگر HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES
  ساختار seccomp_notif_resp *sresp;
  ساختار seccomp_notif *sreq;
  ساختار seccomp_notif_sizes sizes;
#endif
};

تهی
cleanup_seccomp_notify_pluginsp (تهی *p)
{
  ساختار seccomp_notify_context_s **pp = p;
  اگر (*pp)
    {
      libcrun_error_t tmp_err = پوچ;
      libcrun_free_seccomp_notify_plugins (*pp, &tmp_err);
      crun_error_release (&tmp_err);
      *pp = پوچ;
    }
}

#اگر HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
ایستا صحیح
seccomp_syscall (unsigned صحیح op, unsigned صحیح flags, تهی *args)
{
  errno = 0;
  بازگشت syscall (__NR_seccomp, op, flags, args);
}
#endif

LIBCRUN_PUBLIC صحیح
libcrun_load_seccomp_notify_plugins (ساختار seccomp_notify_context_s **out, ثابت نویسه *plugins,
                                     ساختار libcrun_load_seccomp_notify_conf_s *conf, libcrun_error_t *err)
{
#اگر HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  cleanup_seccomp_notify_context ساختار seccomp_notify_context_s *ctx = xmalloc0 (اندازه *ctx);
  cleanup_free نویسه *b = پوچ;
  نویسه *it, *saveptr;
  size_t s;

  اگر (seccomp_syscall (SECCOMP_GET_NOTIF_SIZES, 0, &ctx->sizes) < 0)
    بازگشت crun_make_error (err, errno, "seccomp GET_NOTIF_SIZES");

  ctx->sreq = xmalloc (ctx->sizes.seccomp_notif);
  ctx->sresp = xmalloc (ctx->sizes.seccomp_notif_resp);

  اگر (is_empty_string (plugins))
    بازگشت 0;

  b = xstrdup (plugins);

  ctx->n_plugins = 1;
  برای (it = b; *it; it++)
    اگر (*it == ':')
      ctx->n_plugins++;

  ctx->plugins = xmalloc0 (اندازه (ساختار plugin) * (ctx->n_plugins + 1));
  برای (s = 0, it = strtok_r (b, ":", &saveptr); it; s++, it = strtok_r (پوچ, ":", &saveptr))
    {
      run_oci_seccomp_notify_plugin_version_cb version_cb;
      run_oci_seccomp_notify_start_cb start_cb;
      تهی *opq = پوچ;

      /* do not accept relative paths.  It is fine to accept only filenames as dlopen() semantics apply.  */
      اگر (strchr (it, '/') && it[0] != '/')
        بازگشت crun_make_error (err, 0, "invalid relative plugin path: `%s`", it);

      ctx->plugins[s].handle = dlopen (it, RTLD_NOW);
      اگر (ctx->plugins[s].handle == پوچ)
        بازگشت crun_make_error (err, 0, "cannot load `%s`: %s", it, dlerror ());

      version_cb
          = (run_oci_seccomp_notify_plugin_version_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_version");
      اگر (version_cb != پوچ)
        {
          صحیح version;

          version = version_cb ();
          اگر (version != 1)
            بازگشت crun_make_error (err, ENOTSUP, "invalid version supported by the plugin `%s`", it);
        }

      ctx->plugins[s].handle_request_cb = (run_oci_seccomp_notify_handle_request_cb) dlsym (
          ctx->plugins[s].handle, "run_oci_seccomp_notify_handle_request");
      اگر (ctx->plugins[s].handle_request_cb == پوچ)
        بازگشت crun_make_error (err, ENOTSUP, "plugin `%s` doesn't export `run_oci_seccomp_notify_handle_request`", it);

      start_cb = (run_oci_seccomp_notify_start_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_start");
      اگر (start_cb)
        {
          صحیح ret;

          ret = start_cb (&opq, conf, اندازه (*conf));
          اگر (UNLIKELY (ret != 0))
            بازگشت crun_make_error (err, 0, "error loading `%s`", it);
        }
      ctx->plugins[s].opaque = opq;
    }

  /* Change ownership.  */
  *out = ctx;
  ctx = پوچ;
  بازگشت 0;
#وگرنه
  (تهی) out;
  (تهی) plugins;
  (تهی) conf;
  (تهی) err;
  بازگشت crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC صحیح
libcrun_seccomp_notify_plugins (ساختار seccomp_notify_context_s *ctx, صحیح seccomp_fd, libcrun_error_t *err)
{
#اگر HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;
  صحیح ret;

  memset (ctx->sreq, 0, ctx->sizes.seccomp_notif);
  memset (ctx->sresp, 0, ctx->sizes.seccomp_notif_resp);

  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_RECV, ctx->sreq);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno == ENOENT)
        بازگشت 0;
      بازگشت crun_make_error (err, errno, "ioctl");
    }

  برای (i = 0; i < ctx->n_plugins; i++)
    {
      اگر (ctx->plugins[i].handle_request_cb)
        {
          صحیح handled = 0;
          صحیح ret;

          ret = ctx->plugins[i].handle_request_cb (ctx->plugins[i].opaque, &ctx->sizes, ctx->sreq, ctx->sresp,
                                                   seccomp_fd, &handled);
          اگر (UNLIKELY (ret != 0))
            بازگشت crun_make_error (err, 0, "error handling seccomp notify request");

          انتخاب (handled)
            {
            حالت RUN_OCI_SECCOMP_NOTIFY_HANDLE_NOT_HANDLED:
              بشکن;

            حالت RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE:
              برو send_resp;

              /* The plugin will take care of it.  */
            حالت RUN_OCI_SECCOMP_NOTIFY_HANDLE_DELAYED_RESPONSE:
              بازگشت 0;

            حالت RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE_AND_CONTINUE:
              ctx->sresp->flags |= SECCOMP_USER_NOTIF_FLAG_CONTINUE;
              برو send_resp;

            پیشفرض:
              بازگشت crun_make_error (err, EINVAL, "unknown action specified by the plugin `%d`", handled);
            }
        }
    }

  /* No plugin could handle the request.  */
  ctx->sresp->error = -ENOTSUP;
  ctx->sresp->flags = 0;

send_resp:
  ctx->sresp->id = ctx->sreq->id;
  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_SEND, ctx->sresp);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno == ENOENT)
        بازگشت 0;
      بازگشت crun_make_error (err, errno, "ioctl");
    }
  بازگشت 0;
#وگرنه
  (تهی) ctx;
  (تهی) seccomp_fd;
  (تهی) err;
  بازگشت crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC صحیح
libcrun_free_seccomp_notify_plugins (ساختار seccomp_notify_context_s *ctx, libcrun_error_t *err)
{
#اگر HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;

  اگر (ctx == پوچ)
    بازگشت crun_make_error (err, EINVAL, "invalid seccomp notify context");

  آزاد (ctx->sreq);
  آزاد (ctx->sresp);

  برای (i = 0; i < ctx->n_plugins; i++)
    اگر (ctx->plugins && ctx->plugins[i].handle)
      {
        run_oci_seccomp_notify_stop_cb cb;

        cb = (run_oci_seccomp_notify_stop_cb) dlsym (ctx->plugins[i].handle, "run_oci_seccomp_notify_stop");
        اگر (cb)
          cb (ctx->plugins[i].opaque);
        dlclose (ctx->plugins[i].handle);
      }

  آزاد (ctx->plugins);
  آزاد (ctx);

  بازگشت 0;
#وگرنه
  (تهی) ctx;
  (تهی) err;
  بازگشت crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}
