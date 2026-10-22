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
#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <errno.h>

#જો HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
#  include <seccomp.h>
#  include <sys/ioctl.h>
#  include <linux/seccomp.h>
#  include <sys/sysmacros.h>
#endif

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "seccomp_notify.h"

#ifndef SECCOMP_USER_NOTIF_FLAG_CONTINUE
#  define SECCOMP_USER_NOTIF_FLAG_CONTINUE (1UL << 0)
#endif

સંરચના plugin
{
  શૂન્ય *handle;
  શૂન્ય *opaque;
#જો HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  run_oci_seccomp_notify_handle_request_cb handle_request_cb;
#endif
};

સંરચના seccomp_notify_context_s
{
  સંરચના plugin *plugins;
  size_t n_plugins;

#જો HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES
  સંરચના seccomp_notif_resp *sresp;
  સંરચના seccomp_notif *sreq;
  સંરચના seccomp_notif_sizes sizes;
#endif
};

શૂન્ય
cleanup_seccomp_notify_pluginsp (શૂન્ય *p)
{
  સંરચના seccomp_notify_context_s **pp = p;
  જો (*pp)
    {
      libcrun_error_t tmp_err = ખાલી;
      libcrun_free_seccomp_notify_plugins (*pp, &tmp_err);
      crun_error_release (&tmp_err);
      *pp = ખાલી;
    }
}

#જો HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
સ્થિર પૂર્ણાંક
seccomp_syscall (unsigned પૂર્ણાંક op, unsigned પૂર્ણાંક flags, શૂન્ય *args)
{
  errno = 0;
  પાછા syscall (__NR_seccomp, op, flags, args);
}
#endif

LIBCRUN_PUBLIC પૂર્ણાંક
libcrun_load_seccomp_notify_plugins (સંરચના seccomp_notify_context_s **out, અચલ અક્ષર *plugins,
                                     સંરચના libcrun_load_seccomp_notify_conf_s *conf, libcrun_error_t *err)
{
#જો HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  cleanup_seccomp_notify_context સંરચના seccomp_notify_context_s *ctx = xmalloc0 (કદ *ctx);
  cleanup_free અક્ષર *b = ખાલી;
  અક્ષર *it, *saveptr;
  size_t s;

  જો (seccomp_syscall (SECCOMP_GET_NOTIF_SIZES, 0, &ctx->sizes) < 0)
    પાછા crun_make_error (err, errno, "seccomp GET_NOTIF_SIZES");

  ctx->sreq = xmalloc (ctx->sizes.seccomp_notif);
  ctx->sresp = xmalloc (ctx->sizes.seccomp_notif_resp);

  જો (is_empty_string (plugins))
    પાછા 0;

  b = xstrdup (plugins);

  ctx->n_plugins = 1;
  માટે (it = b; *it; it++)
    જો (*it == ':')
      ctx->n_plugins++;

  ctx->plugins = xmalloc0 (કદ (સંરચના plugin) * (ctx->n_plugins + 1));
  માટે (s = 0, it = strtok_r (b, ":", &saveptr); it; s++, it = strtok_r (ખાલી, ":", &saveptr))
    {
      run_oci_seccomp_notify_plugin_version_cb version_cb;
      run_oci_seccomp_notify_start_cb start_cb;
      શૂન્ય *opq = ખાલી;

      /* do not accept relative paths.  It is fine to accept only filenames as dlopen() semantics apply.  */
      જો (strchr (it, '/') && it[0] != '/')
        પાછા crun_make_error (err, 0, "invalid relative plugin path: `%s`", it);

      ctx->plugins[s].handle = dlopen (it, RTLD_NOW);
      જો (ctx->plugins[s].handle == ખાલી)
        પાછા crun_make_error (err, 0, "cannot load `%s`: %s", it, dlerror ());

      version_cb
          = (run_oci_seccomp_notify_plugin_version_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_version");
      જો (version_cb != ખાલી)
        {
          પૂર્ણાંક version;

          version = version_cb ();
          જો (version != 1)
            પાછા crun_make_error (err, ENOTSUP, "invalid version supported by the plugin `%s`", it);
        }

      ctx->plugins[s].handle_request_cb = (run_oci_seccomp_notify_handle_request_cb) dlsym (
          ctx->plugins[s].handle, "run_oci_seccomp_notify_handle_request");
      જો (ctx->plugins[s].handle_request_cb == ખાલી)
        પાછા crun_make_error (err, ENOTSUP, "plugin `%s` doesn't export `run_oci_seccomp_notify_handle_request`", it);

      start_cb = (run_oci_seccomp_notify_start_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_start");
      જો (start_cb)
        {
          પૂર્ણાંક ret;

          ret = start_cb (&opq, conf, કદ (*conf));
          જો (UNLIKELY (ret != 0))
            પાછા crun_make_error (err, 0, "error loading `%s`", it);
        }
      ctx->plugins[s].opaque = opq;
    }

  /* Change ownership.  */
  *out = ctx;
  ctx = ખાલી;
  પાછા 0;
#નહીંતર
  (શૂન્ય) out;
  (શૂન્ય) plugins;
  (શૂન્ય) conf;
  (શૂન્ય) err;
  પાછા crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC પૂર્ણાંક
libcrun_seccomp_notify_plugins (સંરચના seccomp_notify_context_s *ctx, પૂર્ણાંક seccomp_fd, libcrun_error_t *err)
{
#જો HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;
  પૂર્ણાંક ret;

  memset (ctx->sreq, 0, ctx->sizes.seccomp_notif);
  memset (ctx->sresp, 0, ctx->sizes.seccomp_notif_resp);

  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_RECV, ctx->sreq);
  જો (UNLIKELY (ret < 0))
    {
      જો (errno == ENOENT)
        પાછા 0;
      પાછા crun_make_error (err, errno, "ioctl");
    }

  માટે (i = 0; i < ctx->n_plugins; i++)
    {
      જો (ctx->plugins[i].handle_request_cb)
        {
          પૂર્ણાંક handled = 0;
          પૂર્ણાંક ret;

          ret = ctx->plugins[i].handle_request_cb (ctx->plugins[i].opaque, &ctx->sizes, ctx->sreq, ctx->sresp,
                                                   seccomp_fd, &handled);
          જો (UNLIKELY (ret != 0))
            પાછા crun_make_error (err, 0, "error handling seccomp notify request");

          પસંદગી (handled)
            {
            સ્થિતિ RUN_OCI_SECCOMP_NOTIFY_HANDLE_NOT_HANDLED:
              તોડો;

            સ્થિતિ RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE:
              જાઓ send_resp;

              /* The plugin will take care of it.  */
            સ્થિતિ RUN_OCI_SECCOMP_NOTIFY_HANDLE_DELAYED_RESPONSE:
              પાછા 0;

            સ્થિતિ RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE_AND_CONTINUE:
              ctx->sresp->flags |= SECCOMP_USER_NOTIF_FLAG_CONTINUE;
              જાઓ send_resp;

            મૂળભૂત:
              પાછા crun_make_error (err, EINVAL, "unknown action specified by the plugin `%d`", handled);
            }
        }
    }

  /* No plugin could handle the request.  */
  ctx->sresp->error = -ENOTSUP;
  ctx->sresp->flags = 0;

send_resp:
  ctx->sresp->id = ctx->sreq->id;
  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_SEND, ctx->sresp);
  જો (UNLIKELY (ret < 0))
    {
      જો (errno == ENOENT)
        પાછા 0;
      પાછા crun_make_error (err, errno, "ioctl");
    }
  પાછા 0;
#નહીંતર
  (શૂન્ય) ctx;
  (શૂન્ય) seccomp_fd;
  (શૂન્ય) err;
  પાછા crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC પૂર્ણાંક
libcrun_free_seccomp_notify_plugins (સંરચના seccomp_notify_context_s *ctx, libcrun_error_t *err)
{
#જો HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;

  જો (ctx == ખાલી)
    પાછા crun_make_error (err, EINVAL, "invalid seccomp notify context");

  મુક્ત (ctx->sreq);
  મુક્ત (ctx->sresp);

  માટે (i = 0; i < ctx->n_plugins; i++)
    જો (ctx->plugins && ctx->plugins[i].handle)
      {
        run_oci_seccomp_notify_stop_cb cb;

        cb = (run_oci_seccomp_notify_stop_cb) dlsym (ctx->plugins[i].handle, "run_oci_seccomp_notify_stop");
        જો (cb)
          cb (ctx->plugins[i].opaque);
        dlclose (ctx->plugins[i].handle);
      }

  મુક્ત (ctx->plugins);
  મુક્ત (ctx);

  પાછા 0;
#નહીંતર
  (શૂન્ય) ctx;
  (શૂન્ય) err;
  પાછા crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}
