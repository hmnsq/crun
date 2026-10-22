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

#ಆದರೆ HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
#  include <seccomp.h>
#  include <sys/ioctl.h>
#  include <linux/seccomp.h>
#  include <sys/sysmacros.h>
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DLOPEN
#  include <dlfcn.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "seccomp_notify.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SECCOMP_USER_NOTIF_FLAG_CONTINUE
#  define SECCOMP_USER_NOTIF_FLAG_CONTINUE (1UL << 0)
#ಆದರೆ_ಅಂತ್ಯ

ರಚನೆ plugin
{
  ಶೂನ್ಯ *handle;
  ಶೂನ್ಯ *opaque;
#ಆದರೆ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  ಸತತ_oci_seccomp_notify_handle_request_cb handle_request_cb;
#ಆದರೆ_ಅಂತ್ಯ
};

ರಚನೆ seccomp_notify_context_s
{
  ರಚನೆ plugin *plugins;
  size_t n_plugins;

#ಆದರೆ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES
  ರಚನೆ seccomp_notif_resp *sresp;
  ರಚನೆ seccomp_notif *sreq;
  ರಚನೆ seccomp_notif_sizes sizes;
#ಆದರೆ_ಅಂತ್ಯ
};

ಶೂನ್ಯ
cleanup_seccomp_notify_pluginsp (ಶೂನ್ಯ *p)
{
  ರಚನೆ seccomp_notify_context_s **pp = p;
  ಆದರೆ (*pp)
    {
      libcrun_error_t tmp_err = ಖಾಲಿ;
      libcrun_free_seccomp_notify_plugins (*pp, &tmp_err);
      crun_error_release (&tmp_err);
      *pp = ಖಾಲಿ;
    }
}

#ಆದರೆ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
seccomp_syscall (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ op, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags, ಶೂನ್ಯ *ವಾದಗಳು)
{
  errno = 0;
  ಹಿಂತಿರುಗಿ syscall (__NR_seccomp, op, flags, ವಾದಗಳು);
}
#ಆದರೆ_ಅಂತ್ಯ

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ
libcrun_ತುಂಬು_seccomp_notify_plugins (ರಚನೆ seccomp_notify_context_s **ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *plugins,
                                     ರಚನೆ libcrun_ತುಂಬು_seccomp_notify_conf_s *conf, libcrun_error_t *err)
{
#ಆದರೆ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  cleanup_seccomp_notify_context ರಚನೆ seccomp_notify_context_s *ctx = xmalloc0 (ಗಾತ್ರ *ctx);
  cleanup_free ಅಕ್ಷರ *b = ಖಾಲಿ;
  ಅಕ್ಷರ *it, *saveptr;
  size_t s;

  ಆದರೆ (seccomp_syscall (SECCOMP_GET_NOTIF_SIZES, 0, &ctx->sizes) < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "seccomp GET_NOTIF_SIZES");

  ctx->sreq = xmalloc (ctx->sizes.seccomp_notif);
  ctx->sresp = xmalloc (ctx->sizes.seccomp_notif_resp);

  ಆದರೆ (is_ಖಾಲಿಗಳು_string (plugins))
    ಹಿಂತಿರುಗಿ 0;

  b = xstrdup (plugins);

  ctx->n_plugins = 1;
  ಗಾಗಿ (it = b; *it; it++)
    ಆದರೆ (*it == ':')
      ctx->n_plugins++;

  ctx->plugins = xmalloc0 (ಗಾತ್ರ (ರಚನೆ plugin) * (ctx->n_plugins + 1));
  ಗಾಗಿ (s = 0, it = strtok_r (b, ":", &saveptr); it; s++, it = strtok_r (ಖಾಲಿ, ":", &saveptr))
    {
      ಸತತ_oci_seccomp_notify_plugin_ಆವೃತ್ತಿ_cb ಆವೃತ್ತಿ_cb;
      ಸತತ_oci_seccomp_notify_ಆರಂಭ_cb ಆರಂಭ_cb;
      ಶೂನ್ಯ *opq = ಖಾಲಿ;

      /* do not accept relative paths.  It is fine to accept only filenames as dlopen() semantics apply.  */
      ಆದರೆ (strchr (it, '/') && it[0] != '/')
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid relative plugin path: `%s`", it);

      ctx->plugins[s].handle = dlopen (it, RTLD_NOW);
      ಆದರೆ (ctx->plugins[s].handle == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot load `%s`: %s", it, dlerror ());

      ಆವೃತ್ತಿ_cb
          = (ಸತತ_oci_seccomp_notify_plugin_ಆವೃತ್ತಿ_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_version");
      ಆದರೆ (ಆವೃತ್ತಿ_cb != ಖಾಲಿ)
        {
          ಪೂರ್ಣಾಂಕ ಆವೃತ್ತಿ;

          ಆವೃತ್ತಿ = ಆವೃತ್ತಿ_cb ();
          ಆದರೆ (ಆವೃತ್ತಿ != 1)
            ಹಿಂತಿರುಗಿ crun_make_error (err, ENOTSUP, "invalid version supported by the plugin `%s`", it);
        }

      ctx->plugins[s].handle_request_cb = (ಸತತ_oci_seccomp_notify_handle_request_cb) dlsym (
          ctx->plugins[s].handle, "run_oci_seccomp_notify_handle_request");
      ಆದರೆ (ctx->plugins[s].handle_request_cb == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, ENOTSUP, "plugin `%s` doesn't export `run_oci_seccomp_notify_handle_request`", it);

      ಆರಂಭ_cb = (ಸತತ_oci_seccomp_notify_ಆರಂಭ_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_start");
      ಆದರೆ (ಆರಂಭ_cb)
        {
          ಪೂರ್ಣಾಂಕ ret;

          ret = ಆರಂಭ_cb (&opq, conf, ಗಾತ್ರ (*conf));
          ಆದರೆ (UNLIKELY (ret != 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "error loading `%s`", it);
        }
      ctx->plugins[s].opaque = opq;
    }

  /* Change ownership.  */
  *ಹೊರಹೋಗು = ctx;
  ctx = ಖಾಲಿ;
  ಹಿಂತಿರುಗಿ 0;
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) ಹೊರಹೋಗು;
  (ಶೂನ್ಯ) plugins;
  (ಶೂನ್ಯ) conf;
  (ಶೂನ್ಯ) err;
  ಹಿಂತಿರುಗಿ crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#ಆದರೆ_ಅಂತ್ಯ
}

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ
libcrun_seccomp_notify_plugins (ರಚನೆ seccomp_notify_context_s *ctx, ಪೂರ್ಣಾಂಕ seccomp_fd, libcrun_error_t *err)
{
#ಆದರೆ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  memset (ctx->sreq, 0, ctx->sizes.seccomp_notif);
  memset (ctx->sresp, 0, ctx->sizes.seccomp_notif_resp);

  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_RECV, ctx->sreq);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಆದರೆ (errno == ENOENT)
        ಹಿಂತಿರುಗಿ 0;
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "ioctl");
    }

  ಗಾಗಿ (i = 0; i < ctx->n_plugins; i++)
    {
      ಆದರೆ (ctx->plugins[i].handle_request_cb)
        {
          ಪೂರ್ಣಾಂಕ handled = 0;
          ಪೂರ್ಣಾಂಕ ret;

          ret = ctx->plugins[i].handle_request_cb (ctx->plugins[i].opaque, &ctx->sizes, ctx->sreq, ctx->sresp,
                                                   seccomp_fd, &handled);
          ಆದರೆ (UNLIKELY (ret != 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "error handling seccomp notify request");

          ಆಯ್ಕೆ (handled)
            {
            ಸ್ಥಿತಿ ಸತತ_OCI_SECCOMP_NOTIFY_HANDLE_NOT_HANDLED:
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ ಸತತ_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE:
              ಹೋಗು send_resp;

              /* The plugin will take care of it.  */
            ಸ್ಥಿತಿ ಸತತ_OCI_SECCOMP_NOTIFY_HANDLE_DELAYED_RESPONSE:
              ಹಿಂತಿರುಗಿ 0;

            ಸ್ಥಿತಿ ಸತತ_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE_AND_CONTINUE:
              ctx->sresp->flags |= SECCOMP_USER_NOTIF_FLAG_CONTINUE;
              ಹೋಗು send_resp;

            ಪೂರ್ವನಿಯೋಜಿತ:
              ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "unknown action specified by the plugin `%d`", handled);
            }
        }
    }

  /* No plugin could handle the request.  */
  ctx->sresp->error = -ENOTSUP;
  ctx->sresp->flags = 0;

send_resp:
  ctx->sresp->id = ctx->sreq->id;
  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_SEND, ctx->sresp);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಆದರೆ (errno == ENOENT)
        ಹಿಂತಿರುಗಿ 0;
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "ioctl");
    }
  ಹಿಂತಿರುಗಿ 0;
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) ctx;
  (ಶೂನ್ಯ) seccomp_fd;
  (ಶೂನ್ಯ) err;
  ಹಿಂತಿರುಗಿ crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#ಆದರೆ_ಅಂತ್ಯ
}

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ
libcrun_free_seccomp_notify_plugins (ರಚನೆ seccomp_notify_context_s *ctx, libcrun_error_t *err)
{
#ಆದರೆ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;

  ಆದರೆ (ctx == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "invalid seccomp notify context");

  ಬಿಡುಗಡೆ (ctx->sreq);
  ಬಿಡುಗಡೆ (ctx->sresp);

  ಗಾಗಿ (i = 0; i < ctx->n_plugins; i++)
    ಆದರೆ (ctx->plugins && ctx->plugins[i].handle)
      {
        ಸತತ_oci_seccomp_notify_stop_cb cb;

        cb = (ಸತತ_oci_seccomp_notify_stop_cb) dlsym (ctx->plugins[i].handle, "run_oci_seccomp_notify_stop");
        ಆದರೆ (cb)
          cb (ctx->plugins[i].opaque);
        dlclose (ctx->plugins[i].handle);
      }

  ಬಿಡುಗಡೆ (ctx->plugins);
  ಬಿಡುಗಡೆ (ctx);

  ಹಿಂತಿರುಗಿ 0;
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) ctx;
  (ಶೂನ್ಯ) err;
  ಹಿಂತಿರುಗಿ crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#ಆದರೆ_ಅಂತ್ಯ
}
