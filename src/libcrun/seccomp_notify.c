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
#অন্তর্ভাব <config.h>
#অন্তর্ভাব <errno.h>

#যদি HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
#  include <seccomp.h>
#  include <sys/ioctl.h>
#  include <linux/seccomp.h>
#  include <sys/sysmacros.h>
#endif

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "seccomp_notify.h"

#ifndef SECCOMP_USER_NOTIF_FLAG_CONTINUE
#  define SECCOMP_USER_NOTIF_FLAG_CONTINUE (1UL << 0)
#endif

কাঠামো plugin
{
  শূন্য *handle;
  শূন্য *opaque;
#যদি HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  run_oci_seccomp_notify_handle_request_cb handle_request_cb;
#endif
};

কাঠামো seccomp_notify_context_s
{
  কাঠামো plugin *plugins;
  size_t n_plugins;

#যদি HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES
  কাঠামো seccomp_notif_resp *sresp;
  কাঠামো seccomp_notif *sreq;
  কাঠামো seccomp_notif_sizes sizes;
#endif
};

শূন্য
cleanup_seccomp_notify_pluginsp (শূন্য *p)
{
  কাঠামো seccomp_notify_context_s **pp = p;
  যদি (*pp)
    {
      libcrun_error_t tmp_err = ফাঁকা;
      libcrun_free_seccomp_notify_plugins (*pp, &tmp_err);
      crun_error_release (&tmp_err);
      *pp = ফাঁকা;
    }
}

#যদি HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
স্থির পূর্ণসংখ্যা
seccomp_syscall (unsigned পূর্ণসংখ্যা op, unsigned পূর্ণসংখ্যা flags, শূন্য *args)
{
  errno = 0;
  ফেরত syscall (__NR_seccomp, op, flags, args);
}
#endif

LIBCRUN_PUBLIC পূর্ণসংখ্যা
libcrun_load_seccomp_notify_plugins (কাঠামো seccomp_notify_context_s **out, ধ্রুবক অক্ষর *plugins,
                                     কাঠামো libcrun_load_seccomp_notify_conf_s *conf, libcrun_error_t *err)
{
#যদি HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  cleanup_seccomp_notify_context কাঠামো seccomp_notify_context_s *ctx = xmalloc0 (আকার *ctx);
  cleanup_free অক্ষর *b = ফাঁকা;
  অক্ষর *it, *saveptr;
  size_t s;

  যদি (seccomp_syscall (SECCOMP_GET_NOTIF_SIZES, 0, &ctx->sizes) < 0)
    ফেরত crun_make_error (err, errno, "seccomp GET_NOTIF_SIZES");

  ctx->sreq = xmalloc (ctx->sizes.seccomp_notif);
  ctx->sresp = xmalloc (ctx->sizes.seccomp_notif_resp);

  যদি (is_empty_string (plugins))
    ফেরত 0;

  b = xstrdup (plugins);

  ctx->n_plugins = 1;
  জন্য (it = b; *it; it++)
    যদি (*it == ':')
      ctx->n_plugins++;

  ctx->plugins = xmalloc0 (আকার (কাঠামো plugin) * (ctx->n_plugins + 1));
  জন্য (s = 0, it = strtok_r (b, ":", &saveptr); it; s++, it = strtok_r (ফাঁকা, ":", &saveptr))
    {
      run_oci_seccomp_notify_plugin_version_cb version_cb;
      run_oci_seccomp_notify_start_cb start_cb;
      শূন্য *opq = ফাঁকা;

      /* do not accept relative paths.  It is fine to accept only filenames as dlopen() semantics apply.  */
      যদি (strchr (it, '/') && it[0] != '/')
        ফেরত crun_make_error (err, 0, "invalid relative plugin path: `%s`", it);

      ctx->plugins[s].handle = dlopen (it, RTLD_NOW);
      যদি (ctx->plugins[s].handle == ফাঁকা)
        ফেরত crun_make_error (err, 0, "cannot load `%s`: %s", it, dlerror ());

      version_cb
          = (run_oci_seccomp_notify_plugin_version_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_version");
      যদি (version_cb != ফাঁকা)
        {
          পূর্ণসংখ্যা version;

          version = version_cb ();
          যদি (version != 1)
            ফেরত crun_make_error (err, ENOTSUP, "invalid version supported by the plugin `%s`", it);
        }

      ctx->plugins[s].handle_request_cb = (run_oci_seccomp_notify_handle_request_cb) dlsym (
          ctx->plugins[s].handle, "run_oci_seccomp_notify_handle_request");
      যদি (ctx->plugins[s].handle_request_cb == ফাঁকা)
        ফেরত crun_make_error (err, ENOTSUP, "plugin `%s` doesn't export `run_oci_seccomp_notify_handle_request`", it);

      start_cb = (run_oci_seccomp_notify_start_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_start");
      যদি (start_cb)
        {
          পূর্ণসংখ্যা ret;

          ret = start_cb (&opq, conf, আকার (*conf));
          যদি (UNLIKELY (ret != 0))
            ফেরত crun_make_error (err, 0, "error loading `%s`", it);
        }
      ctx->plugins[s].opaque = opq;
    }

  /* Change ownership.  */
  *out = ctx;
  ctx = ফাঁকা;
  ফেরত 0;
#অন্যথা
  (শূন্য) out;
  (শূন্য) plugins;
  (শূন্য) conf;
  (শূন্য) err;
  ফেরত crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC পূর্ণসংখ্যা
libcrun_seccomp_notify_plugins (কাঠামো seccomp_notify_context_s *ctx, পূর্ণসংখ্যা seccomp_fd, libcrun_error_t *err)
{
#যদি HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;
  পূর্ণসংখ্যা ret;

  memset (ctx->sreq, 0, ctx->sizes.seccomp_notif);
  memset (ctx->sresp, 0, ctx->sizes.seccomp_notif_resp);

  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_RECV, ctx->sreq);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (errno == ENOENT)
        ফেরত 0;
      ফেরত crun_make_error (err, errno, "ioctl");
    }

  জন্য (i = 0; i < ctx->n_plugins; i++)
    {
      যদি (ctx->plugins[i].handle_request_cb)
        {
          পূর্ণসংখ্যা handled = 0;
          পূর্ণসংখ্যা ret;

          ret = ctx->plugins[i].handle_request_cb (ctx->plugins[i].opaque, &ctx->sizes, ctx->sreq, ctx->sresp,
                                                   seccomp_fd, &handled);
          যদি (UNLIKELY (ret != 0))
            ফেরত crun_make_error (err, 0, "error handling seccomp notify request");

          নির্বাচন (handled)
            {
            ক্ষেত্র RUN_OCI_SECCOMP_NOTIFY_HANDLE_NOT_HANDLED:
              বিরতি;

            ক্ষেত্র RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE:
              যাও send_resp;

              /* The plugin will take care of it.  */
            ক্ষেত্র RUN_OCI_SECCOMP_NOTIFY_HANDLE_DELAYED_RESPONSE:
              ফেরত 0;

            ক্ষেত্র RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE_AND_CONTINUE:
              ctx->sresp->flags |= SECCOMP_USER_NOTIF_FLAG_CONTINUE;
              যাও send_resp;

            পূর্বনির্ধারিত:
              ফেরত crun_make_error (err, EINVAL, "unknown action specified by the plugin `%d`", handled);
            }
        }
    }

  /* No plugin could handle the request.  */
  ctx->sresp->error = -ENOTSUP;
  ctx->sresp->flags = 0;

send_resp:
  ctx->sresp->id = ctx->sreq->id;
  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_SEND, ctx->sresp);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (errno == ENOENT)
        ফেরত 0;
      ফেরত crun_make_error (err, errno, "ioctl");
    }
  ফেরত 0;
#অন্যথা
  (শূন্য) ctx;
  (শূন্য) seccomp_fd;
  (শূন্য) err;
  ফেরত crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC পূর্ণসংখ্যা
libcrun_free_seccomp_notify_plugins (কাঠামো seccomp_notify_context_s *ctx, libcrun_error_t *err)
{
#যদি HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;

  যদি (ctx == ফাঁকা)
    ফেরত crun_make_error (err, EINVAL, "invalid seccomp notify context");

  মুক্ত (ctx->sreq);
  মুক্ত (ctx->sresp);

  জন্য (i = 0; i < ctx->n_plugins; i++)
    যদি (ctx->plugins && ctx->plugins[i].handle)
      {
        run_oci_seccomp_notify_stop_cb cb;

        cb = (run_oci_seccomp_notify_stop_cb) dlsym (ctx->plugins[i].handle, "run_oci_seccomp_notify_stop");
        যদি (cb)
          cb (ctx->plugins[i].opaque);
        dlclose (ctx->plugins[i].handle);
      }

  মুক্ত (ctx->plugins);
  মুক্ত (ctx);

  ফেরত 0;
#অন্যথা
  (শূন্য) ctx;
  (শূন্য) err;
  ফেরত crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}
