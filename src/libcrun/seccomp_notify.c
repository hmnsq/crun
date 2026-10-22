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
#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <errno.h>

#ಯದಿ HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
#  include <seccomp.h>
#  include <sys/ioctl.h>
#  include <linux/seccomp.h>
#  include <sys/sysmacros.h>
#endif

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "seccomp_notify.h"

#ifndef SECCOMP_USER_NOTIF_FLAG_CONTINUE
#  define SECCOMP_USER_NOTIF_FLAG_CONTINUE (1UL << 0)
#endif

ಸಂರಚನಾ plugin
{
  ಶೂನ್ಯ *handle;
  ಶೂನ್ಯ *opaque;
#ಯದಿ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  run_oci_seccomp_notify_handle_request_cb handle_request_cb;
#endif
};

ಸಂರಚನಾ seccomp_notify_context_s
{
  ಸಂರಚನಾ plugin *plugins;
  size_t n_plugins;

#ಯದಿ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES
  ಸಂರಚನಾ seccomp_notif_resp *sresp;
  ಸಂರಚನಾ seccomp_notif *sreq;
  ಸಂರಚನಾ seccomp_notif_sizes sizes;
#endif
};

ಶೂನ್ಯ
cleanup_seccomp_notify_pluginsp (ಶೂನ್ಯ *p)
{
  ಸಂರಚನಾ seccomp_notify_context_s **pp = p;
  ಯದಿ (*pp)
    {
      libcrun_error_t tmp_err = ರಿಕ್ತ;
      libcrun_free_seccomp_notify_plugins (*pp, &tmp_err);
      crun_error_release (&tmp_err);
      *pp = ರಿಕ್ತ;
    }
}

#ಯದಿ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
seccomp_syscall (unsigned ಪೂರ್ಣಾಙ್ಕ op, unsigned ಪೂರ್ಣಾಙ್ಕ flags, ಶೂನ್ಯ *args)
{
  errno = 0;
  ಪ್ರತ್ಯಾಗಮ syscall (__NR_seccomp, op, flags, args);
}
#endif

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ
libcrun_load_seccomp_notify_plugins (ಸಂರಚನಾ seccomp_notify_context_s **out, ಧ್ರುವ ವರ್ಣ *plugins,
                                     ಸಂರಚನಾ libcrun_load_seccomp_notify_conf_s *conf, libcrun_error_t *err)
{
#ಯದಿ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  cleanup_seccomp_notify_context ಸಂರಚನಾ seccomp_notify_context_s *ctx = xmalloc0 (ಪರಿಮಾಣ *ctx);
  cleanup_free ವರ್ಣ *b = ರಿಕ್ತ;
  ವರ್ಣ *it, *saveptr;
  size_t s;

  ಯದಿ (seccomp_syscall (SECCOMP_GET_NOTIF_SIZES, 0, &ctx->sizes) < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "seccomp GET_NOTIF_SIZES");

  ctx->sreq = xmalloc (ctx->sizes.seccomp_notif);
  ctx->sresp = xmalloc (ctx->sizes.seccomp_notif_resp);

  ಯದಿ (is_empty_string (plugins))
    ಪ್ರತ್ಯಾಗಮ 0;

  b = xstrdup (plugins);

  ctx->n_plugins = 1;
  ಪ್ರತಿ (it = b; *it; it++)
    ಯದಿ (*it == ':')
      ctx->n_plugins++;

  ctx->plugins = xmalloc0 (ಪರಿಮಾಣ (ಸಂರಚನಾ plugin) * (ctx->n_plugins + 1));
  ಪ್ರತಿ (s = 0, it = strtok_r (b, ":", &saveptr); it; s++, it = strtok_r (ರಿಕ್ತ, ":", &saveptr))
    {
      run_oci_seccomp_notify_plugin_version_cb version_cb;
      run_oci_seccomp_notify_start_cb start_cb;
      ಶೂನ್ಯ *opq = ರಿಕ್ತ;

      /* do not accept relative paths.  It is fine to accept only filenames as dlopen() semantics apply.  */
      ಯದಿ (strchr (it, '/') && it[0] != '/')
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid relative plugin path: `%s`", it);

      ctx->plugins[s].handle = dlopen (it, RTLD_NOW);
      ಯದಿ (ctx->plugins[s].handle == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot load `%s`: %s", it, dlerror ());

      version_cb
          = (run_oci_seccomp_notify_plugin_version_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_version");
      ಯದಿ (version_cb != ರಿಕ್ತ)
        {
          ಪೂರ್ಣಾಙ್ಕ version;

          version = version_cb ();
          ಯದಿ (version != 1)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ENOTSUP, "invalid version supported by the plugin `%s`", it);
        }

      ctx->plugins[s].handle_request_cb = (run_oci_seccomp_notify_handle_request_cb) dlsym (
          ctx->plugins[s].handle, "run_oci_seccomp_notify_handle_request");
      ಯದಿ (ctx->plugins[s].handle_request_cb == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ENOTSUP, "plugin `%s` doesn't export `run_oci_seccomp_notify_handle_request`", it);

      start_cb = (run_oci_seccomp_notify_start_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_start");
      ಯದಿ (start_cb)
        {
          ಪೂರ್ಣಾಙ್ಕ ret;

          ret = start_cb (&opq, conf, ಪರಿಮಾಣ (*conf));
          ಯದಿ (UNLIKELY (ret != 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "error loading `%s`", it);
        }
      ctx->plugins[s].opaque = opq;
    }

  /* Change ownership.  */
  *out = ctx;
  ctx = ರಿಕ್ತ;
  ಪ್ರತ್ಯಾಗಮ 0;
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) out;
  (ಶೂನ್ಯ) plugins;
  (ಶೂನ್ಯ) conf;
  (ಶೂನ್ಯ) err;
  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ
libcrun_seccomp_notify_plugins (ಸಂರಚನಾ seccomp_notify_context_s *ctx, ಪೂರ್ಣಾಙ್ಕ seccomp_fd, libcrun_error_t *err)
{
#ಯದಿ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  memset (ctx->sreq, 0, ctx->sizes.seccomp_notif);
  memset (ctx->sresp, 0, ctx->sizes.seccomp_notif_resp);

  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_RECV, ctx->sreq);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಯದಿ (errno == ENOENT)
        ಪ್ರತ್ಯಾಗಮ 0;
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "ioctl");
    }

  ಪ್ರತಿ (i = 0; i < ctx->n_plugins; i++)
    {
      ಯದಿ (ctx->plugins[i].handle_request_cb)
        {
          ಪೂರ್ಣಾಙ್ಕ handled = 0;
          ಪೂರ್ಣಾಙ್ಕ ret;

          ret = ctx->plugins[i].handle_request_cb (ctx->plugins[i].opaque, &ctx->sizes, ctx->sreq, ctx->sresp,
                                                   seccomp_fd, &handled);
          ಯದಿ (UNLIKELY (ret != 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "error handling seccomp notify request");

          ವಿಕಲ್ಪ (handled)
            {
            ಪಕ್ಷ RUN_OCI_SECCOMP_NOTIFY_HANDLE_NOT_HANDLED:
              ಭಙ್ಗ;

            ಪಕ್ಷ RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE:
              ಗಚ್ಛ send_resp;

              /* The plugin will take care of it.  */
            ಪಕ್ಷ RUN_OCI_SECCOMP_NOTIFY_HANDLE_DELAYED_RESPONSE:
              ಪ್ರತ್ಯಾಗಮ 0;

            ಪಕ್ಷ RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE_AND_CONTINUE:
              ctx->sresp->flags |= SECCOMP_USER_NOTIF_FLAG_CONTINUE;
              ಗಚ್ಛ send_resp;

            ಸ್ವಭಾವ:
              ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EINVAL, "unknown action specified by the plugin `%d`", handled);
            }
        }
    }

  /* No plugin could handle the request.  */
  ctx->sresp->error = -ENOTSUP;
  ctx->sresp->flags = 0;

send_resp:
  ctx->sresp->id = ctx->sreq->id;
  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_SEND, ctx->sresp);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಯದಿ (errno == ENOENT)
        ಪ್ರತ್ಯಾಗಮ 0;
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "ioctl");
    }
  ಪ್ರತ್ಯಾಗಮ 0;
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) ctx;
  (ಶೂನ್ಯ) seccomp_fd;
  (ಶೂನ್ಯ) err;
  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ
libcrun_free_seccomp_notify_plugins (ಸಂರಚನಾ seccomp_notify_context_s *ctx, libcrun_error_t *err)
{
#ಯದಿ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;

  ಯದಿ (ctx == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EINVAL, "invalid seccomp notify context");

  ಮುಕ್ತ (ctx->sreq);
  ಮುಕ್ತ (ctx->sresp);

  ಪ್ರತಿ (i = 0; i < ctx->n_plugins; i++)
    ಯದಿ (ctx->plugins && ctx->plugins[i].handle)
      {
        run_oci_seccomp_notify_stop_cb cb;

        cb = (run_oci_seccomp_notify_stop_cb) dlsym (ctx->plugins[i].handle, "run_oci_seccomp_notify_stop");
        ಯದಿ (cb)
          cb (ctx->plugins[i].opaque);
        dlclose (ctx->plugins[i].handle);
      }

  ಮುಕ್ತ (ctx->plugins);
  ಮುಕ್ತ (ctx);

  ಪ್ರತ್ಯಾಗಮ 0;
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) ctx;
  (ಶೂನ್ಯ) err;
  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}
