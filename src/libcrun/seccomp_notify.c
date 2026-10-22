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
#அந்தர்பாவ <config.h>
#அந்தர்பாவ <errno.h>

#எனில் HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
#  include <seccomp.h>
#  include <sys/ioctl.h>
#  include <linux/seccomp.h>
#  include <sys/sysmacros.h>
#endif

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "seccomp_notify.h"

#ifndef SECCOMP_USER_NOTIF_FLAG_CONTINUE
#  define SECCOMP_USER_NOTIF_FLAG_CONTINUE (1UL << 0)
#endif

கட்டமைப்பு plugin
{
  வெறுமை *handle;
  வெறுமை *opaque;
#எனில் HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  run_oci_seccomp_notify_handle_request_cb handle_request_cb;
#endif
};

கட்டமைப்பு seccomp_notify_context_s
{
  கட்டமைப்பு plugin *plugins;
  size_t n_plugins;

#எனில் HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES
  கட்டமைப்பு seccomp_notif_resp *sresp;
  கட்டமைப்பு seccomp_notif *sreq;
  கட்டமைப்பு seccomp_notif_sizes sizes;
#endif
};

வெறுமை
cleanup_seccomp_notify_pluginsp (வெறுமை *p)
{
  கட்டமைப்பு seccomp_notify_context_s **pp = p;
  எனில் (*pp)
    {
      libcrun_error_t tmp_err = வெற்று;
      libcrun_free_seccomp_notify_plugins (*pp, &tmp_err);
      crun_error_release (&tmp_err);
      *pp = வெற்று;
    }
}

#எனில் HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
நிலையான முழு
seccomp_syscall (unsigned முழு op, unsigned முழு flags, வெறுமை *args)
{
  errno = 0;
  திரும்பு syscall (__NR_seccomp, op, flags, args);
}
#endif

LIBCRUN_PUBLIC முழு
libcrun_load_seccomp_notify_plugins (கட்டமைப்பு seccomp_notify_context_s **out, மாறிலி எழுத்து *plugins,
                                     கட்டமைப்பு libcrun_load_seccomp_notify_conf_s *conf, libcrun_error_t *err)
{
#எனில் HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  cleanup_seccomp_notify_context கட்டமைப்பு seccomp_notify_context_s *ctx = xmalloc0 (அளவு *ctx);
  cleanup_free எழுத்து *b = வெற்று;
  எழுத்து *it, *saveptr;
  size_t s;

  எனில் (seccomp_syscall (SECCOMP_GET_NOTIF_SIZES, 0, &ctx->sizes) < 0)
    திரும்பு crun_make_error (err, errno, "seccomp GET_NOTIF_SIZES");

  ctx->sreq = xmalloc (ctx->sizes.seccomp_notif);
  ctx->sresp = xmalloc (ctx->sizes.seccomp_notif_resp);

  எனில் (is_empty_string (plugins))
    திரும்பு 0;

  b = xstrdup (plugins);

  ctx->n_plugins = 1;
  க்கு (it = b; *it; it++)
    எனில் (*it == ':')
      ctx->n_plugins++;

  ctx->plugins = xmalloc0 (அளவு (கட்டமைப்பு plugin) * (ctx->n_plugins + 1));
  க்கு (s = 0, it = strtok_r (b, ":", &saveptr); it; s++, it = strtok_r (வெற்று, ":", &saveptr))
    {
      run_oci_seccomp_notify_plugin_version_cb version_cb;
      run_oci_seccomp_notify_start_cb start_cb;
      வெறுமை *opq = வெற்று;

      /* do not accept relative paths.  It is fine to accept only filenames as dlopen() semantics apply.  */
      எனில் (strchr (it, '/') && it[0] != '/')
        திரும்பு crun_make_error (err, 0, "invalid relative plugin path: `%s`", it);

      ctx->plugins[s].handle = dlopen (it, RTLD_NOW);
      எனில் (ctx->plugins[s].handle == வெற்று)
        திரும்பு crun_make_error (err, 0, "cannot load `%s`: %s", it, dlerror ());

      version_cb
          = (run_oci_seccomp_notify_plugin_version_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_version");
      எனில் (version_cb != வெற்று)
        {
          முழு version;

          version = version_cb ();
          எனில் (version != 1)
            திரும்பு crun_make_error (err, ENOTSUP, "invalid version supported by the plugin `%s`", it);
        }

      ctx->plugins[s].handle_request_cb = (run_oci_seccomp_notify_handle_request_cb) dlsym (
          ctx->plugins[s].handle, "run_oci_seccomp_notify_handle_request");
      எனில் (ctx->plugins[s].handle_request_cb == வெற்று)
        திரும்பு crun_make_error (err, ENOTSUP, "plugin `%s` doesn't export `run_oci_seccomp_notify_handle_request`", it);

      start_cb = (run_oci_seccomp_notify_start_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_start");
      எனில் (start_cb)
        {
          முழு ret;

          ret = start_cb (&opq, conf, அளவு (*conf));
          எனில் (UNLIKELY (ret != 0))
            திரும்பு crun_make_error (err, 0, "error loading `%s`", it);
        }
      ctx->plugins[s].opaque = opq;
    }

  /* Change ownership.  */
  *out = ctx;
  ctx = வெற்று;
  திரும்பு 0;
#இல்லையெனில்
  (வெறுமை) out;
  (வெறுமை) plugins;
  (வெறுமை) conf;
  (வெறுமை) err;
  திரும்பு crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC முழு
libcrun_seccomp_notify_plugins (கட்டமைப்பு seccomp_notify_context_s *ctx, முழு seccomp_fd, libcrun_error_t *err)
{
#எனில் HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;
  முழு ret;

  memset (ctx->sreq, 0, ctx->sizes.seccomp_notif);
  memset (ctx->sresp, 0, ctx->sizes.seccomp_notif_resp);

  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_RECV, ctx->sreq);
  எனில் (UNLIKELY (ret < 0))
    {
      எனில் (errno == ENOENT)
        திரும்பு 0;
      திரும்பு crun_make_error (err, errno, "ioctl");
    }

  க்கு (i = 0; i < ctx->n_plugins; i++)
    {
      எனில் (ctx->plugins[i].handle_request_cb)
        {
          முழு handled = 0;
          முழு ret;

          ret = ctx->plugins[i].handle_request_cb (ctx->plugins[i].opaque, &ctx->sizes, ctx->sreq, ctx->sresp,
                                                   seccomp_fd, &handled);
          எனில் (UNLIKELY (ret != 0))
            திரும்பு crun_make_error (err, 0, "error handling seccomp notify request");

          தேர்வு (handled)
            {
            நிலை RUN_OCI_SECCOMP_NOTIFY_HANDLE_NOT_HANDLED:
              நிறுத்து;

            நிலை RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE:
              செல் send_resp;

              /* The plugin will take care of it.  */
            நிலை RUN_OCI_SECCOMP_NOTIFY_HANDLE_DELAYED_RESPONSE:
              திரும்பு 0;

            நிலை RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE_AND_CONTINUE:
              ctx->sresp->flags |= SECCOMP_USER_NOTIF_FLAG_CONTINUE;
              செல் send_resp;

            இயல்பு:
              திரும்பு crun_make_error (err, EINVAL, "unknown action specified by the plugin `%d`", handled);
            }
        }
    }

  /* No plugin could handle the request.  */
  ctx->sresp->error = -ENOTSUP;
  ctx->sresp->flags = 0;

send_resp:
  ctx->sresp->id = ctx->sreq->id;
  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_SEND, ctx->sresp);
  எனில் (UNLIKELY (ret < 0))
    {
      எனில் (errno == ENOENT)
        திரும்பு 0;
      திரும்பு crun_make_error (err, errno, "ioctl");
    }
  திரும்பு 0;
#இல்லையெனில்
  (வெறுமை) ctx;
  (வெறுமை) seccomp_fd;
  (வெறுமை) err;
  திரும்பு crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC முழு
libcrun_free_seccomp_notify_plugins (கட்டமைப்பு seccomp_notify_context_s *ctx, libcrun_error_t *err)
{
#எனில் HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;

  எனில் (ctx == வெற்று)
    திரும்பு crun_make_error (err, EINVAL, "invalid seccomp notify context");

  விடுவி (ctx->sreq);
  விடுவி (ctx->sresp);

  க்கு (i = 0; i < ctx->n_plugins; i++)
    எனில் (ctx->plugins && ctx->plugins[i].handle)
      {
        run_oci_seccomp_notify_stop_cb cb;

        cb = (run_oci_seccomp_notify_stop_cb) dlsym (ctx->plugins[i].handle, "run_oci_seccomp_notify_stop");
        எனில் (cb)
          cb (ctx->plugins[i].opaque);
        dlclose (ctx->plugins[i].handle);
      }

  விடுவி (ctx->plugins);
  விடுவி (ctx);

  திரும்பு 0;
#இல்லையெனில்
  (வெறுமை) ctx;
  (வெறுமை) err;
  திரும்பு crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}
