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
#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>

#ਜੇ HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
#  include <seccomp.h>
#  include <sys/ioctl.h>
#  include <linux/seccomp.h>
#  include <sys/sysmacros.h>
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DLOPEN
#  include <dlfcn.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "seccomp_notify.h"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SECCOMP_USER_NOTIF_FLAG_CONTINUE
#  define SECCOMP_USER_NOTIF_FLAG_CONTINUE (1UL << 0)
#ਜੇ_ਅੰਤ

ਬਣਤਰ plugin
{
  ਖਾਲੀ *handle;
  ਖਾਲੀ *opaque;
#ਜੇ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  ਲੜੀਵਾਰ_oci_seccomp_notify_handle_request_cb handle_request_cb;
#ਜੇ_ਅੰਤ
};

ਬਣਤਰ seccomp_notify_context_s
{
  ਬਣਤਰ plugin *plugins;
  size_t n_plugins;

#ਜੇ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES
  ਬਣਤਰ seccomp_notif_resp *sresp;
  ਬਣਤਰ seccomp_notif *sreq;
  ਬਣਤਰ seccomp_notif_sizes sizes;
#ਜੇ_ਅੰਤ
};

ਖਾਲੀ
cleanup_seccomp_notify_pluginsp (ਖਾਲੀ *p)
{
  ਬਣਤਰ seccomp_notify_context_s **pp = p;
  ਜੇ (*pp)
    {
      libcrun_error_t tmp_err = ਨਿਰਾ;
      libcrun_free_seccomp_notify_plugins (*pp, &tmp_err);
      crun_error_release (&tmp_err);
      *pp = ਨਿਰਾ;
    }
}

#ਜੇ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
ਸਥਿਰ ਪੂਰਨ
seccomp_syscall (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ op, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags, ਖਾਲੀ *ਦਲੀਲਾਂ)
{
  errno = 0;
  ਵਾਪਸ syscall (__NR_seccomp, op, flags, ਦਲੀਲਾਂ);
}
#ਜੇ_ਅੰਤ

LIBCRUN_ਜਨਤਕ ਪੂਰਨ
libcrun_ਲੱਦੋ_seccomp_notify_plugins (ਬਣਤਰ seccomp_notify_context_s **ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *plugins,
                                     ਬਣਤਰ libcrun_ਲੱਦੋ_seccomp_notify_conf_s *conf, libcrun_error_t *err)
{
#ਜੇ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  cleanup_seccomp_notify_context ਬਣਤਰ seccomp_notify_context_s *ctx = xmalloc0 (ਆਕਾਰ *ctx);
  cleanup_free ਅੱਖਰ *b = ਨਿਰਾ;
  ਅੱਖਰ *it, *saveptr;
  size_t s;

  ਜੇ (seccomp_syscall (SECCOMP_GET_NOTIF_SIZES, 0, &ctx->sizes) < 0)
    ਵਾਪਸ crun_make_error (err, errno, "seccomp GET_NOTIF_SIZES");

  ctx->sreq = xmalloc (ctx->sizes.seccomp_notif);
  ctx->sresp = xmalloc (ctx->sizes.seccomp_notif_resp);

  ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (plugins))
    ਵਾਪਸ 0;

  b = xstrdup (plugins);

  ctx->n_plugins = 1;
  ਲਈ (it = b; *it; it++)
    ਜੇ (*it == ':')
      ctx->n_plugins++;

  ctx->plugins = xmalloc0 (ਆਕਾਰ (ਬਣਤਰ plugin) * (ctx->n_plugins + 1));
  ਲਈ (s = 0, it = strtok_r (b, ":", &saveptr); it; s++, it = strtok_r (ਨਿਰਾ, ":", &saveptr))
    {
      ਲੜੀਵਾਰ_oci_seccomp_notify_plugin_ਸੰਸਕਰਣ_cb ਸੰਸਕਰਣ_cb;
      ਲੜੀਵਾਰ_oci_seccomp_notify_ਸ਼ੁਰੂ_cb ਸ਼ੁਰੂ_cb;
      ਖਾਲੀ *opq = ਨਿਰਾ;

      /* do not accept relative paths.  It is fine to accept only filenames as dlopen() semantics apply.  */
      ਜੇ (strchr (it, '/') && it[0] != '/')
        ਵਾਪਸ crun_make_error (err, 0, "invalid relative plugin path: `%s`", it);

      ctx->plugins[s].handle = dlopen (it, RTLD_NOW);
      ਜੇ (ctx->plugins[s].handle == ਨਿਰਾ)
        ਵਾਪਸ crun_make_error (err, 0, "cannot load `%s`: %s", it, dlerror ());

      ਸੰਸਕਰਣ_cb
          = (ਲੜੀਵਾਰ_oci_seccomp_notify_plugin_ਸੰਸਕਰਣ_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_version");
      ਜੇ (ਸੰਸਕਰਣ_cb != ਨਿਰਾ)
        {
          ਪੂਰਨ ਸੰਸਕਰਣ;

          ਸੰਸਕਰਣ = ਸੰਸਕਰਣ_cb ();
          ਜੇ (ਸੰਸਕਰਣ != 1)
            ਵਾਪਸ crun_make_error (err, ENOTSUP, "invalid version supported by the plugin `%s`", it);
        }

      ctx->plugins[s].handle_request_cb = (ਲੜੀਵਾਰ_oci_seccomp_notify_handle_request_cb) dlsym (
          ctx->plugins[s].handle, "run_oci_seccomp_notify_handle_request");
      ਜੇ (ctx->plugins[s].handle_request_cb == ਨਿਰਾ)
        ਵਾਪਸ crun_make_error (err, ENOTSUP, "plugin `%s` doesn't export `run_oci_seccomp_notify_handle_request`", it);

      ਸ਼ੁਰੂ_cb = (ਲੜੀਵਾਰ_oci_seccomp_notify_ਸ਼ੁਰੂ_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_start");
      ਜੇ (ਸ਼ੁਰੂ_cb)
        {
          ਪੂਰਨ ret;

          ret = ਸ਼ੁਰੂ_cb (&opq, conf, ਆਕਾਰ (*conf));
          ਜੇ (UNLIKELY (ret != 0))
            ਵਾਪਸ crun_make_error (err, 0, "error loading `%s`", it);
        }
      ctx->plugins[s].opaque = opq;
    }

  /* Change ownership.  */
  *ਬਾਹਰਲਾ = ctx;
  ctx = ਨਿਰਾ;
  ਵਾਪਸ 0;
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) ਬਾਹਰਲਾ;
  (ਖਾਲੀ) plugins;
  (ਖਾਲੀ) conf;
  (ਖਾਲੀ) err;
  ਵਾਪਸ crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#ਜੇ_ਅੰਤ
}

LIBCRUN_ਜਨਤਕ ਪੂਰਨ
libcrun_seccomp_notify_plugins (ਬਣਤਰ seccomp_notify_context_s *ctx, ਪੂਰਨ seccomp_fd, libcrun_error_t *err)
{
#ਜੇ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;
  ਪੂਰਨ ret;

  memset (ctx->sreq, 0, ctx->sizes.seccomp_notif);
  memset (ctx->sresp, 0, ctx->sizes.seccomp_notif_resp);

  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_RECV, ctx->sreq);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਜੇ (errno == ENOENT)
        ਵਾਪਸ 0;
      ਵਾਪਸ crun_make_error (err, errno, "ioctl");
    }

  ਲਈ (i = 0; i < ctx->n_plugins; i++)
    {
      ਜੇ (ctx->plugins[i].handle_request_cb)
        {
          ਪੂਰਨ handled = 0;
          ਪੂਰਨ ret;

          ret = ctx->plugins[i].handle_request_cb (ctx->plugins[i].opaque, &ctx->sizes, ctx->sreq, ctx->sresp,
                                                   seccomp_fd, &handled);
          ਜੇ (UNLIKELY (ret != 0))
            ਵਾਪਸ crun_make_error (err, 0, "error handling seccomp notify request");

          ਚੋਣ (handled)
            {
            ਹਾਲਤ ਲੜੀਵਾਰ_OCI_SECCOMP_NOTIFY_HANDLE_NOT_HANDLED:
              ਰੋਕੋ;

            ਹਾਲਤ ਲੜੀਵਾਰ_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE:
              ਜਾਓ send_resp;

              /* The plugin will take care of it.  */
            ਹਾਲਤ ਲੜੀਵਾਰ_OCI_SECCOMP_NOTIFY_HANDLE_DELAYED_RESPONSE:
              ਵਾਪਸ 0;

            ਹਾਲਤ ਲੜੀਵਾਰ_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE_AND_CONTINUE:
              ctx->sresp->flags |= SECCOMP_USER_NOTIF_FLAG_CONTINUE;
              ਜਾਓ send_resp;

            ਮੂਲ:
              ਵਾਪਸ crun_make_error (err, EINVAL, "unknown action specified by the plugin `%d`", handled);
            }
        }
    }

  /* No plugin could handle the request.  */
  ctx->sresp->error = -ENOTSUP;
  ctx->sresp->flags = 0;

send_resp:
  ctx->sresp->id = ctx->sreq->id;
  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_SEND, ctx->sresp);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਜੇ (errno == ENOENT)
        ਵਾਪਸ 0;
      ਵਾਪਸ crun_make_error (err, errno, "ioctl");
    }
  ਵਾਪਸ 0;
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) ctx;
  (ਖਾਲੀ) seccomp_fd;
  (ਖਾਲੀ) err;
  ਵਾਪਸ crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#ਜੇ_ਅੰਤ
}

LIBCRUN_ਜਨਤਕ ਪੂਰਨ
libcrun_free_seccomp_notify_plugins (ਬਣਤਰ seccomp_notify_context_s *ctx, libcrun_error_t *err)
{
#ਜੇ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;

  ਜੇ (ctx == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, EINVAL, "invalid seccomp notify context");

  ਮੁਕਤ (ctx->sreq);
  ਮੁਕਤ (ctx->sresp);

  ਲਈ (i = 0; i < ctx->n_plugins; i++)
    ਜੇ (ctx->plugins && ctx->plugins[i].handle)
      {
        ਲੜੀਵਾਰ_oci_seccomp_notify_stop_cb cb;

        cb = (ਲੜੀਵਾਰ_oci_seccomp_notify_stop_cb) dlsym (ctx->plugins[i].handle, "run_oci_seccomp_notify_stop");
        ਜੇ (cb)
          cb (ctx->plugins[i].opaque);
        dlclose (ctx->plugins[i].handle);
      }

  ਮੁਕਤ (ctx->plugins);
  ਮੁਕਤ (ctx);

  ਵਾਪਸ 0;
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) ctx;
  (ਖਾਲੀ) err;
  ਵਾਪਸ crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#ਜੇ_ਅੰਤ
}
