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
#اگر_ختم

#اگر_تعریف_شدہ HAVE_DLOPEN
#  include <dlfcn.h>
#اگر_ختم

#شامل "utils.h"
#شامل "seccomp_notify.h"

#اگر_غیر_تعریف_شدہ SECCOMP_USER_NOTIF_FLAG_CONTINUE
#  define SECCOMP_USER_NOTIF_FLAG_CONTINUE (1UL << 0)
#اگر_ختم

ساخت plugin
{
  خالی *handle;
  خالی *opaque;
#اگر HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  تسلسل_oci_seccomp_notify_handle_request_cb handle_request_cb;
#اگر_ختم
};

ساخت seccomp_notify_context_s
{
  ساخت plugin *plugins;
  size_t n_plugins;

#اگر HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES
  ساخت seccomp_notif_resp *sresp;
  ساخت seccomp_notif *sreq;
  ساخت seccomp_notif_sizes sizes;
#اگر_ختم
};

خالی
cleanup_seccomp_notify_pluginsp (خالی *p)
{
  ساخت seccomp_notify_context_s **pp = p;
  اگر (*pp)
    {
      libcrun_error_t tmp_err = تہی;
      libcrun_free_seccomp_notify_plugins (*pp, &tmp_err);
      crun_error_release (&tmp_err);
      *pp = تہی;
    }
}

#اگر HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
ساکن صحیح
seccomp_syscall (غیرعلامتی صحیح op, غیرعلامتی صحیح flags, خالی *دلائل)
{
  errno = 0;
  واپس syscall (__NR_seccomp, op, flags, دلائل);
}
#اگر_ختم

LIBCRUN_عوامی صحیح
libcrun_بھرو_seccomp_notify_plugins (ساخت seccomp_notify_context_s **برآمد, ثابت حرف *plugins,
                                     ساخت libcrun_بھرو_seccomp_notify_conf_s *conf, libcrun_error_t *err)
{
#اگر HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  cleanup_seccomp_notify_context ساخت seccomp_notify_context_s *ctx = xmalloc0 (حجم *ctx);
  cleanup_free حرف *b = تہی;
  حرف *it, *saveptr;
  size_t s;

  اگر (seccomp_syscall (SECCOMP_GET_NOTIF_SIZES, 0, &ctx->sizes) < 0)
    واپس crun_make_error (err, errno, "seccomp GET_NOTIF_SIZES");

  ctx->sreq = xmalloc (ctx->sizes.seccomp_notif);
  ctx->sresp = xmalloc (ctx->sizes.seccomp_notif_resp);

  اگر (is_خالی_خانے_string (plugins))
    واپس 0;

  b = xstrdup (plugins);

  ctx->n_plugins = 1;
  برائے (it = b; *it; it++)
    اگر (*it == ':')
      ctx->n_plugins++;

  ctx->plugins = xmalloc0 (حجم (ساخت plugin) * (ctx->n_plugins + 1));
  برائے (s = 0, it = strtok_r (b, ":", &saveptr); it; s++, it = strtok_r (تہی, ":", &saveptr))
    {
      تسلسل_oci_seccomp_notify_plugin_نسخہ_cb نسخہ_cb;
      تسلسل_oci_seccomp_notify_آغاز_cb آغاز_cb;
      خالی *opq = تہی;

      /* do not accept relative paths.  It is fine to accept only filenames as dlopen() semantics apply.  */
      اگر (strchr (it, '/') && it[0] != '/')
        واپس crun_make_error (err, 0, "invalid relative plugin path: `%s`", it);

      ctx->plugins[s].handle = dlopen (it, RTLD_NOW);
      اگر (ctx->plugins[s].handle == تہی)
        واپس crun_make_error (err, 0, "cannot load `%s`: %s", it, dlerror ());

      نسخہ_cb
          = (تسلسل_oci_seccomp_notify_plugin_نسخہ_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_version");
      اگر (نسخہ_cb != تہی)
        {
          صحیح نسخہ;

          نسخہ = نسخہ_cb ();
          اگر (نسخہ != 1)
            واپس crun_make_error (err, ENOTSUP, "invalid version supported by the plugin `%s`", it);
        }

      ctx->plugins[s].handle_request_cb = (تسلسل_oci_seccomp_notify_handle_request_cb) dlsym (
          ctx->plugins[s].handle, "run_oci_seccomp_notify_handle_request");
      اگر (ctx->plugins[s].handle_request_cb == تہی)
        واپس crun_make_error (err, ENOTSUP, "plugin `%s` doesn't export `run_oci_seccomp_notify_handle_request`", it);

      آغاز_cb = (تسلسل_oci_seccomp_notify_آغاز_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_start");
      اگر (آغاز_cb)
        {
          صحیح ret;

          ret = آغاز_cb (&opq, conf, حجم (*conf));
          اگر (UNLIKELY (ret != 0))
            واپس crun_make_error (err, 0, "error loading `%s`", it);
        }
      ctx->plugins[s].opaque = opq;
    }

  /* Change ownership.  */
  *برآمد = ctx;
  ctx = تہی;
  واپس 0;
#ورنہ
  (خالی) برآمد;
  (خالی) plugins;
  (خالی) conf;
  (خالی) err;
  واپس crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#اگر_ختم
}

LIBCRUN_عوامی صحیح
libcrun_seccomp_notify_plugins (ساخت seccomp_notify_context_s *ctx, صحیح seccomp_fd, libcrun_error_t *err)
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
        واپس 0;
      واپس crun_make_error (err, errno, "ioctl");
    }

  برائے (i = 0; i < ctx->n_plugins; i++)
    {
      اگر (ctx->plugins[i].handle_request_cb)
        {
          صحیح handled = 0;
          صحیح ret;

          ret = ctx->plugins[i].handle_request_cb (ctx->plugins[i].opaque, &ctx->sizes, ctx->sreq, ctx->sresp,
                                                   seccomp_fd, &handled);
          اگر (UNLIKELY (ret != 0))
            واپس crun_make_error (err, 0, "error handling seccomp notify request");

          انتخاب (handled)
            {
            حالت تسلسل_OCI_SECCOMP_NOTIFY_HANDLE_NOT_HANDLED:
              توڑو;

            حالت تسلسل_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE:
              جاؤ send_resp;

              /* The plugin will take care of it.  */
            حالت تسلسل_OCI_SECCOMP_NOTIFY_HANDLE_DELAYED_RESPONSE:
              واپس 0;

            حالت تسلسل_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE_AND_CONTINUE:
              ctx->sresp->flags |= SECCOMP_USER_NOTIF_FLAG_CONTINUE;
              جاؤ send_resp;

            طےشدہ:
              واپس crun_make_error (err, EINVAL, "unknown action specified by the plugin `%d`", handled);
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
        واپس 0;
      واپس crun_make_error (err, errno, "ioctl");
    }
  واپس 0;
#ورنہ
  (خالی) ctx;
  (خالی) seccomp_fd;
  (خالی) err;
  واپس crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#اگر_ختم
}

LIBCRUN_عوامی صحیح
libcrun_free_seccomp_notify_plugins (ساخت seccomp_notify_context_s *ctx, libcrun_error_t *err)
{
#اگر HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;

  اگر (ctx == تہی)
    واپس crun_make_error (err, EINVAL, "invalid seccomp notify context");

  آزاد (ctx->sreq);
  آزاد (ctx->sresp);

  برائے (i = 0; i < ctx->n_plugins; i++)
    اگر (ctx->plugins && ctx->plugins[i].handle)
      {
        تسلسل_oci_seccomp_notify_stop_cb cb;

        cb = (تسلسل_oci_seccomp_notify_stop_cb) dlsym (ctx->plugins[i].handle, "run_oci_seccomp_notify_stop");
        اگر (cb)
          cb (ctx->plugins[i].opaque);
        dlclose (ctx->plugins[i].handle);
      }

  آزاد (ctx->plugins);
  آزاد (ctx);

  واپس 0;
#ورنہ
  (خالی) ctx;
  (خالی) err;
  واپس crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#اگر_ختم
}
