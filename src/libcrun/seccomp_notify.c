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

সংরচনা plugin
{
  শূন্য *handle;
  শূন্য *opaque;
#যদি HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  run_oci_seccomp_notify_handle_request_cb handle_request_cb;
#endif
};

সংরচনা seccomp_notify_context_s
{
  সংরচনা plugin *plugins;
  size_t n_plugins;

#যদি HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES
  সংরচনা seccomp_notif_resp *sresp;
  সংরচনা seccomp_notif *sreq;
  সংরচনা seccomp_notif_sizes sizes;
#endif
};

শূন্য
cleanup_seccomp_notify_pluginsp (শূন্য *p)
{
  সংরচনা seccomp_notify_context_s **pp = p;
  যদি (*pp)
    {
      libcrun_error_t tmp_err = রিক্ত;
      libcrun_free_seccomp_notify_plugins (*pp, &tmp_err);
      crun_error_release (&tmp_err);
      *pp = রিক্ত;
    }
}

#যদি HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
স্থির পূর্ণাঙ্ক
seccomp_syscall (unsigned পূর্ণাঙ্ক op, unsigned পূর্ণাঙ্ক flags, শূন্য *args)
{
  errno = 0;
  প্রত্যাগম syscall (__NR_seccomp, op, flags, args);
}
#endif

LIBCRUN_PUBLIC পূর্ণাঙ্ক
libcrun_load_seccomp_notify_plugins (সংরচনা seccomp_notify_context_s **out, ধ্রুব বর্ণ *plugins,
                                     সংরচনা libcrun_load_seccomp_notify_conf_s *conf, libcrun_error_t *err)
{
#যদি HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  cleanup_seccomp_notify_context সংরচনা seccomp_notify_context_s *ctx = xmalloc0 (পরিমাণ *ctx);
  cleanup_free বর্ণ *b = রিক্ত;
  বর্ণ *it, *saveptr;
  size_t s;

  যদি (seccomp_syscall (SECCOMP_GET_NOTIF_SIZES, 0, &ctx->sizes) < 0)
    প্রত্যাগম crun_make_error (err, errno, "seccomp GET_NOTIF_SIZES");

  ctx->sreq = xmalloc (ctx->sizes.seccomp_notif);
  ctx->sresp = xmalloc (ctx->sizes.seccomp_notif_resp);

  যদি (is_empty_string (plugins))
    প্রত্যাগম 0;

  b = xstrdup (plugins);

  ctx->n_plugins = 1;
  প্রতি (it = b; *it; it++)
    যদি (*it == ':')
      ctx->n_plugins++;

  ctx->plugins = xmalloc0 (পরিমাণ (সংরচনা plugin) * (ctx->n_plugins + 1));
  প্রতি (s = 0, it = strtok_r (b, ":", &saveptr); it; s++, it = strtok_r (রিক্ত, ":", &saveptr))
    {
      run_oci_seccomp_notify_plugin_version_cb version_cb;
      run_oci_seccomp_notify_start_cb start_cb;
      শূন্য *opq = রিক্ত;

      /* do not accept relative paths.  It is fine to accept only filenames as dlopen() semantics apply.  */
      যদি (strchr (it, '/') && it[0] != '/')
        প্রত্যাগম crun_make_error (err, 0, "invalid relative plugin path: `%s`", it);

      ctx->plugins[s].handle = dlopen (it, RTLD_NOW);
      যদি (ctx->plugins[s].handle == রিক্ত)
        প্রত্যাগম crun_make_error (err, 0, "cannot load `%s`: %s", it, dlerror ());

      version_cb
          = (run_oci_seccomp_notify_plugin_version_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_version");
      যদি (version_cb != রিক্ত)
        {
          পূর্ণাঙ্ক version;

          version = version_cb ();
          যদি (version != 1)
            প্রত্যাগম crun_make_error (err, ENOTSUP, "invalid version supported by the plugin `%s`", it);
        }

      ctx->plugins[s].handle_request_cb = (run_oci_seccomp_notify_handle_request_cb) dlsym (
          ctx->plugins[s].handle, "run_oci_seccomp_notify_handle_request");
      যদি (ctx->plugins[s].handle_request_cb == রিক্ত)
        প্রত্যাগম crun_make_error (err, ENOTSUP, "plugin `%s` doesn't export `run_oci_seccomp_notify_handle_request`", it);

      start_cb = (run_oci_seccomp_notify_start_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_start");
      যদি (start_cb)
        {
          পূর্ণাঙ্ক ret;

          ret = start_cb (&opq, conf, পরিমাণ (*conf));
          যদি (UNLIKELY (ret != 0))
            প্রত্যাগম crun_make_error (err, 0, "error loading `%s`", it);
        }
      ctx->plugins[s].opaque = opq;
    }

  /* Change ownership.  */
  *out = ctx;
  ctx = রিক্ত;
  প্রত্যাগম 0;
#অন্যথা
  (শূন্য) out;
  (শূন্য) plugins;
  (শূন্য) conf;
  (শূন্য) err;
  প্রত্যাগম crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC পূর্ণাঙ্ক
libcrun_seccomp_notify_plugins (সংরচনা seccomp_notify_context_s *ctx, পূর্ণাঙ্ক seccomp_fd, libcrun_error_t *err)
{
#যদি HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;
  পূর্ণাঙ্ক ret;

  memset (ctx->sreq, 0, ctx->sizes.seccomp_notif);
  memset (ctx->sresp, 0, ctx->sizes.seccomp_notif_resp);

  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_RECV, ctx->sreq);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (errno == ENOENT)
        প্রত্যাগম 0;
      প্রত্যাগম crun_make_error (err, errno, "ioctl");
    }

  প্রতি (i = 0; i < ctx->n_plugins; i++)
    {
      যদি (ctx->plugins[i].handle_request_cb)
        {
          পূর্ণাঙ্ক handled = 0;
          পূর্ণাঙ্ক ret;

          ret = ctx->plugins[i].handle_request_cb (ctx->plugins[i].opaque, &ctx->sizes, ctx->sreq, ctx->sresp,
                                                   seccomp_fd, &handled);
          যদি (UNLIKELY (ret != 0))
            প্রত্যাগম crun_make_error (err, 0, "error handling seccomp notify request");

          বিকল্প (handled)
            {
            পক্ষ RUN_OCI_SECCOMP_NOTIFY_HANDLE_NOT_HANDLED:
              ভঙ্গ;

            পক্ষ RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE:
              গচ্ছ send_resp;

              /* The plugin will take care of it.  */
            পক্ষ RUN_OCI_SECCOMP_NOTIFY_HANDLE_DELAYED_RESPONSE:
              প্রত্যাগম 0;

            পক্ষ RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE_AND_CONTINUE:
              ctx->sresp->flags |= SECCOMP_USER_NOTIF_FLAG_CONTINUE;
              গচ্ছ send_resp;

            স্বভাব:
              প্রত্যাগম crun_make_error (err, EINVAL, "unknown action specified by the plugin `%d`", handled);
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
        প্রত্যাগম 0;
      প্রত্যাগম crun_make_error (err, errno, "ioctl");
    }
  প্রত্যাগম 0;
#অন্যথা
  (শূন্য) ctx;
  (শূন্য) seccomp_fd;
  (শূন্য) err;
  প্রত্যাগম crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC পূর্ণাঙ্ক
libcrun_free_seccomp_notify_plugins (সংরচনা seccomp_notify_context_s *ctx, libcrun_error_t *err)
{
#যদি HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;

  যদি (ctx == রিক্ত)
    প্রত্যাগম crun_make_error (err, EINVAL, "invalid seccomp notify context");

  মুক্ত (ctx->sreq);
  মুক্ত (ctx->sresp);

  প্রতি (i = 0; i < ctx->n_plugins; i++)
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

  প্রত্যাগম 0;
#অন্যথা
  (শূন্য) ctx;
  (শূন্য) err;
  প্রত্যাগম crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}
