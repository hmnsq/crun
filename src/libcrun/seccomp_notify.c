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
#include <config.h>
#include <errno.h>

#གལ་སྲིད HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
#  include <seccomp.h>
#  include <sys/ioctl.h>
#  include <linux/seccomp.h>
#  include <sys/sysmacros.h>
#endif

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#include "utils.h"
#include "seccomp_notify.h"

#ifndef SECCOMP_USER_NOTIF_FLAG_CONTINUE
#  define SECCOMP_USER_NOTIF_FLAG_CONTINUE (1UL << 0)
#endif

sa_nracanaa plugin
{
  void *handle;
  void *opaque;
#གལ་སྲིད HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  run_oci_seccomp_notify_handle_request_cb handle_request_cb;
#endif
};

sa_nracanaa seccomp_notify_context_s
{
  sa_nracanaa plugin *plugins;
  size_t n_plugins;

#གལ་སྲིད HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES
  sa_nracanaa seccomp_notif_resp *sresp;
  sa_nracanaa seccomp_notif *sreq;
  sa_nracanaa seccomp_notif_sizes sizes;
#endif
};

void
cleanup_seccomp_notify_pluginsp (void *p)
{
  sa_nracanaa seccomp_notify_context_s **pp = p;
  གལ་སྲིད (*pp)
    {
      libcrun_error_t tmp_err = NULL;
      libcrun_free_seccomp_notify_plugins (*pp, &tmp_err);
      crun_error_release (&tmp_err);
      *pp = NULL;
    }
}

#གལ་སྲིད HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
static ཧྲིལ
seccomp_syscall (unsigned ཧྲིལ op, unsigned ཧྲིལ flags, void *args)
{
  errno = 0;
  ལོག syscall (__NR_seccomp, op, flags, args);
}
#endif

LIBCRUN_PUBLIC ཧྲིལ
libcrun_load_seccomp_notify_plugins (sa_nracanaa seccomp_notify_context_s **out, const var_na *plugins,
                                     sa_nracanaa libcrun_load_seccomp_notify_conf_s *conf, libcrun_error_t *err)
{
#གལ་སྲིད HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  cleanup_seccomp_notify_context sa_nracanaa seccomp_notify_context_s *ctx = xmalloc0 (parimaa_na *ctx);
  cleanup_free var_na *b = NULL;
  var_na *it, *saveptr;
  size_t s;

  གལ་སྲིད (seccomp_syscall (SECCOMP_GET_NOTIF_SIZES, 0, &ctx->sizes) < 0)
    ལོག crun_make_error (err, errno, "seccomp GET_NOTIF_SIZES");

  ctx->sreq = xmalloc (ctx->sizes.seccomp_notif);
  ctx->sresp = xmalloc (ctx->sizes.seccomp_notif_resp);

  གལ་སྲིད (is_empty_string (plugins))
    ལོག 0;

  b = xstrdup (plugins);

  ctx->n_plugins = 1;
  དོན་ལུ (it = b; *it; it++)
    གལ་སྲིད (*it == ':')
      ctx->n_plugins++;

  ctx->plugins = xmalloc0 (parimaa_na (sa_nracanaa plugin) * (ctx->n_plugins + 1));
  དོན་ལུ (s = 0, it = strtok_r (b, ":", &saveptr); it; s++, it = strtok_r (NULL, ":", &saveptr))
    {
      run_oci_seccomp_notify_plugin_version_cb version_cb;
      run_oci_seccomp_notify_start_cb start_cb;
      void *opq = NULL;

      /* do not accept relative paths.  It is fine to accept only filenames as dlopen() semantics apply.  */
      གལ་སྲིད (strchr (it, '/') && it[0] != '/')
        ལོག crun_make_error (err, 0, "invalid relative plugin path: `%s`", it);

      ctx->plugins[s].handle = dlopen (it, RTLD_NOW);
      གལ་སྲིད (ctx->plugins[s].handle == NULL)
        ལོག crun_make_error (err, 0, "cannot load `%s`: %s", it, dlerror ());

      version_cb
          = (run_oci_seccomp_notify_plugin_version_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_version");
      གལ་སྲིད (version_cb != NULL)
        {
          ཧྲིལ version;

          version = version_cb ();
          གལ་སྲིད (version != 1)
            ལོག crun_make_error (err, ENOTSUP, "invalid version supported by the plugin `%s`", it);
        }

      ctx->plugins[s].handle_request_cb = (run_oci_seccomp_notify_handle_request_cb) dlsym (
          ctx->plugins[s].handle, "run_oci_seccomp_notify_handle_request");
      གལ་སྲིད (ctx->plugins[s].handle_request_cb == NULL)
        ལོག crun_make_error (err, ENOTSUP, "plugin `%s` doesn't export `run_oci_seccomp_notify_handle_request`", it);

      start_cb = (run_oci_seccomp_notify_start_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_start");
      གལ་སྲིད (start_cb)
        {
          ཧྲིལ ret;

          ret = start_cb (&opq, conf, parimaa_na (*conf));
          གལ་སྲིད (UNLIKELY (ret != 0))
            ལོག crun_make_error (err, 0, "error loading `%s`", it);
        }
      ctx->plugins[s].opaque = opq;
    }

  /* Change ownership.  */
  *out = ctx;
  ctx = NULL;
  ལོག 0;
#དེ་མིན
  (void) out;
  (void) plugins;
  (void) conf;
  (void) err;
  ལོག crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC ཧྲིལ
libcrun_seccomp_notify_plugins (sa_nracanaa seccomp_notify_context_s *ctx, ཧྲིལ seccomp_fd, libcrun_error_t *err)
{
#གལ་སྲིད HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;
  ཧྲིལ ret;

  memset (ctx->sreq, 0, ctx->sizes.seccomp_notif);
  memset (ctx->sresp, 0, ctx->sizes.seccomp_notif_resp);

  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_RECV, ctx->sreq);
  གལ་སྲིད (UNLIKELY (ret < 0))
    {
      གལ་སྲིད (errno == ENOENT)
        ལོག 0;
      ལོག crun_make_error (err, errno, "ioctl");
    }

  དོན་ལུ (i = 0; i < ctx->n_plugins; i++)
    {
      གལ་སྲིད (ctx->plugins[i].handle_request_cb)
        {
          ཧྲིལ handled = 0;
          ཧྲིལ ret;

          ret = ctx->plugins[i].handle_request_cb (ctx->plugins[i].opaque, &ctx->sizes, ctx->sreq, ctx->sresp,
                                                   seccomp_fd, &handled);
          གལ་སྲིད (UNLIKELY (ret != 0))
            ལོག crun_make_error (err, 0, "error handling seccomp notify request");

          switch (handled)
            {
            pak_sa RUN_OCI_SECCOMP_NOTIFY_HANDLE_NOT_HANDLED:
              bha_nga;

            pak_sa RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE:
              goto send_resp;

              /* The plugin will take care of it.  */
            pak_sa RUN_OCI_SECCOMP_NOTIFY_HANDLE_DELAYED_RESPONSE:
              ལོག 0;

            pak_sa RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE_AND_CONTINUE:
              ctx->sresp->flags |= SECCOMP_USER_NOTIF_FLAG_CONTINUE;
              goto send_resp;

            default:
              ལོག crun_make_error (err, EINVAL, "unknown action specified by the plugin `%d`", handled);
            }
        }
    }

  /* No plugin could handle the request.  */
  ctx->sresp->error = -ENOTSUP;
  ctx->sresp->flags = 0;

send_resp:
  ctx->sresp->id = ctx->sreq->id;
  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_SEND, ctx->sresp);
  གལ་སྲིད (UNLIKELY (ret < 0))
    {
      གལ་སྲིད (errno == ENOENT)
        ལོག 0;
      ལོག crun_make_error (err, errno, "ioctl");
    }
  ལོག 0;
#དེ་མིན
  (void) ctx;
  (void) seccomp_fd;
  (void) err;
  ལོག crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC ཧྲིལ
libcrun_free_seccomp_notify_plugins (sa_nracanaa seccomp_notify_context_s *ctx, libcrun_error_t *err)
{
#གལ་སྲིད HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;

  གལ་སྲིད (ctx == NULL)
    ལོག crun_make_error (err, EINVAL, "invalid seccomp notify context");

  free (ctx->sreq);
  free (ctx->sresp);

  དོན་ལུ (i = 0; i < ctx->n_plugins; i++)
    གལ་སྲིད (ctx->plugins && ctx->plugins[i].handle)
      {
        run_oci_seccomp_notify_stop_cb cb;

        cb = (run_oci_seccomp_notify_stop_cb) dlsym (ctx->plugins[i].handle, "run_oci_seccomp_notify_stop");
        གལ་སྲིད (cb)
          cb (ctx->plugins[i].opaque);
        dlclose (ctx->plugins[i].handle);
      }

  free (ctx->plugins);
  free (ctx);

  ལོག 0;
#དེ་མིན
  (void) ctx;
  (void) err;
  ལོག crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}
