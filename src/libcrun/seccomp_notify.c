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
#ضمن <config.h>
#ضمن <errno.h>

#𐡄𐡍 HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
#  include <seccomp.h>
#  include <sys/ioctl.h>
#  include <linux/seccomp.h>
#  include <sys/sysmacros.h>
#endif

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ضمن "utils.h"
#ضمن "seccomp_notify.h"

#ifndef SECCOMP_USER_NOTIF_FLAG_CONTINUE
#  define SECCOMP_USER_NOTIF_FLAG_CONTINUE (1UL << 0)
#endif

بنية plugin
{
  فراغ *handle;
  فراغ *opaque;
#𐡄𐡍 HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  run_oci_seccomp_notify_handle_request_cb handle_request_cb;
#endif
};

بنية seccomp_notify_context_s
{
  بنية plugin *plugins;
  size_t n_plugins;

#𐡄𐡍 HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES
  بنية seccomp_notif_resp *sresp;
  بنية seccomp_notif *sreq;
  بنية seccomp_notif_sizes sizes;
#endif
};

فراغ
cleanup_seccomp_notify_pluginsp (فراغ *p)
{
  بنية seccomp_notify_context_s **pp = p;
  𐡄𐡍 (*pp)
    {
      libcrun_error_t tmp_err = عدم;
      libcrun_free_seccomp_notify_plugins (*pp, &tmp_err);
      crun_error_release (&tmp_err);
      *pp = عدم;
    }
}

#𐡄𐡍 HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
ساكن صحيح
seccomp_syscall (unsigned صحيح op, unsigned صحيح flags, فراغ *args)
{
  errno = 0;
  أرجع syscall (__NR_seccomp, op, flags, args);
}
#endif

LIBCRUN_PUBLIC صحيح
libcrun_load_seccomp_notify_plugins (بنية seccomp_notify_context_s **out, ثابت حرف *plugins,
                                     بنية libcrun_load_seccomp_notify_conf_s *conf, libcrun_error_t *err)
{
#𐡄𐡍 HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  cleanup_seccomp_notify_context بنية seccomp_notify_context_s *ctx = xmalloc0 (حجم *ctx);
  cleanup_free حرف *b = عدم;
  حرف *it, *saveptr;
  size_t s;

  𐡄𐡍 (seccomp_syscall (SECCOMP_GET_NOTIF_SIZES, 0, &ctx->sizes) < 0)
    أرجع crun_make_error (err, errno, "seccomp GET_NOTIF_SIZES");

  ctx->sreq = xmalloc (ctx->sizes.seccomp_notif);
  ctx->sresp = xmalloc (ctx->sizes.seccomp_notif_resp);

  𐡄𐡍 (is_empty_string (plugins))
    أرجع 0;

  b = xstrdup (plugins);

  ctx->n_plugins = 1;
  لكل (it = b; *it; it++)
    𐡄𐡍 (*it == ':')
      ctx->n_plugins++;

  ctx->plugins = xmalloc0 (حجم (بنية plugin) * (ctx->n_plugins + 1));
  لكل (s = 0, it = strtok_r (b, ":", &saveptr); it; s++, it = strtok_r (عدم, ":", &saveptr))
    {
      run_oci_seccomp_notify_plugin_version_cb version_cb;
      run_oci_seccomp_notify_start_cb start_cb;
      فراغ *opq = عدم;

      /* do not accept relative paths.  It is fine to accept only filenames as dlopen() semantics apply.  */
      𐡄𐡍 (strchr (it, '/') && it[0] != '/')
        أرجع crun_make_error (err, 0, "invalid relative plugin path: `%s`", it);

      ctx->plugins[s].handle = dlopen (it, RTLD_NOW);
      𐡄𐡍 (ctx->plugins[s].handle == عدم)
        أرجع crun_make_error (err, 0, "cannot load `%s`: %s", it, dlerror ());

      version_cb
          = (run_oci_seccomp_notify_plugin_version_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_version");
      𐡄𐡍 (version_cb != عدم)
        {
          صحيح version;

          version = version_cb ();
          𐡄𐡍 (version != 1)
            أرجع crun_make_error (err, ENOTSUP, "invalid version supported by the plugin `%s`", it);
        }

      ctx->plugins[s].handle_request_cb = (run_oci_seccomp_notify_handle_request_cb) dlsym (
          ctx->plugins[s].handle, "run_oci_seccomp_notify_handle_request");
      𐡄𐡍 (ctx->plugins[s].handle_request_cb == عدم)
        أرجع crun_make_error (err, ENOTSUP, "plugin `%s` doesn't export `run_oci_seccomp_notify_handle_request`", it);

      start_cb = (run_oci_seccomp_notify_start_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_start");
      𐡄𐡍 (start_cb)
        {
          صحيح ret;

          ret = start_cb (&opq, conf, حجم (*conf));
          𐡄𐡍 (UNLIKELY (ret != 0))
            أرجع crun_make_error (err, 0, "error loading `%s`", it);
        }
      ctx->plugins[s].opaque = opq;
    }

  /* Change ownership.  */
  *out = ctx;
  ctx = عدم;
  أرجع 0;
#وإلا
  (فراغ) out;
  (فراغ) plugins;
  (فراغ) conf;
  (فراغ) err;
  أرجع crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC صحيح
libcrun_seccomp_notify_plugins (بنية seccomp_notify_context_s *ctx, صحيح seccomp_fd, libcrun_error_t *err)
{
#𐡄𐡍 HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;
  صحيح ret;

  memset (ctx->sreq, 0, ctx->sizes.seccomp_notif);
  memset (ctx->sresp, 0, ctx->sizes.seccomp_notif_resp);

  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_RECV, ctx->sreq);
  𐡄𐡍 (UNLIKELY (ret < 0))
    {
      𐡄𐡍 (errno == ENOENT)
        أرجع 0;
      أرجع crun_make_error (err, errno, "ioctl");
    }

  لكل (i = 0; i < ctx->n_plugins; i++)
    {
      𐡄𐡍 (ctx->plugins[i].handle_request_cb)
        {
          صحيح handled = 0;
          صحيح ret;

          ret = ctx->plugins[i].handle_request_cb (ctx->plugins[i].opaque, &ctx->sizes, ctx->sreq, ctx->sresp,
                                                   seccomp_fd, &handled);
          𐡄𐡍 (UNLIKELY (ret != 0))
            أرجع crun_make_error (err, 0, "error handling seccomp notify request");

          بدل (handled)
            {
            حالة RUN_OCI_SECCOMP_NOTIFY_HANDLE_NOT_HANDLED:
              اكسر;

            حالة RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE:
              اذهب send_resp;

              /* The plugin will take care of it.  */
            حالة RUN_OCI_SECCOMP_NOTIFY_HANDLE_DELAYED_RESPONSE:
              أرجع 0;

            حالة RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE_AND_CONTINUE:
              ctx->sresp->flags |= SECCOMP_USER_NOTIF_FLAG_CONTINUE;
              اذهب send_resp;

            افتراضي:
              أرجع crun_make_error (err, EINVAL, "unknown action specified by the plugin `%d`", handled);
            }
        }
    }

  /* No plugin could handle the request.  */
  ctx->sresp->error = -ENOTSUP;
  ctx->sresp->flags = 0;

send_resp:
  ctx->sresp->id = ctx->sreq->id;
  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_SEND, ctx->sresp);
  𐡄𐡍 (UNLIKELY (ret < 0))
    {
      𐡄𐡍 (errno == ENOENT)
        أرجع 0;
      أرجع crun_make_error (err, errno, "ioctl");
    }
  أرجع 0;
#وإلا
  (فراغ) ctx;
  (فراغ) seccomp_fd;
  (فراغ) err;
  أرجع crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC صحيح
libcrun_free_seccomp_notify_plugins (بنية seccomp_notify_context_s *ctx, libcrun_error_t *err)
{
#𐡄𐡍 HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;

  𐡄𐡍 (ctx == عدم)
    أرجع crun_make_error (err, EINVAL, "invalid seccomp notify context");

  حرر (ctx->sreq);
  حرر (ctx->sresp);

  لكل (i = 0; i < ctx->n_plugins; i++)
    𐡄𐡍 (ctx->plugins && ctx->plugins[i].handle)
      {
        run_oci_seccomp_notify_stop_cb cb;

        cb = (run_oci_seccomp_notify_stop_cb) dlsym (ctx->plugins[i].handle, "run_oci_seccomp_notify_stop");
        𐡄𐡍 (cb)
          cb (ctx->plugins[i].opaque);
        dlclose (ctx->plugins[i].handle);
      }

  حرر (ctx->plugins);
  حرر (ctx);

  أرجع 0;
#وإلا
  (فراغ) ctx;
  (فراغ) err;
  أرجع crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}
