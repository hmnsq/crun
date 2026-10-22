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

#если HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
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

struct plugin
{
  void *handle;
  void *opaque;
#если HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  run_oci_seccomp_notify_handle_request_cb handle_request_cb;
#endif
};

struct seccomp_notify_context_s
{
  struct plugin *plugins;
  size_t n_plugins;

#если HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES
  struct seccomp_notif_resp *sresp;
  struct seccomp_notif *sreq;
  struct seccomp_notif_sizes sizes;
#endif
};

void
cleanup_seccomp_notify_pluginsp (void *p)
{
  struct seccomp_notify_context_s **pp = p;
  если (*pp)
    {
      libcrun_error_t tmp_err = NULL;
      libcrun_free_seccomp_notify_plugins (*pp, &tmp_err);
      crun_error_release (&tmp_err);
      *pp = NULL;
    }
}

#если HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
static целое
seccomp_syscall (unsigned целое op, unsigned целое flags, void *args)
{
  errno = 0;
  вернуть syscall (__NR_seccomp, op, flags, args);
}
#endif

LIBCRUN_PUBLIC целое
libcrun_load_seccomp_notify_plugins (struct seccomp_notify_context_s **out, const char *plugins,
                                     struct libcrun_load_seccomp_notify_conf_s *conf, libcrun_error_t *err)
{
#если HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  cleanup_seccomp_notify_context struct seccomp_notify_context_s *ctx = xmalloc0 (sizeof *ctx);
  cleanup_free char *b = NULL;
  char *it, *saveptr;
  size_t s;

  если (seccomp_syscall (SECCOMP_GET_NOTIF_SIZES, 0, &ctx->sizes) < 0)
    вернуть crun_make_error (err, errno, "seccomp GET_NOTIF_SIZES");

  ctx->sreq = xmalloc (ctx->sizes.seccomp_notif);
  ctx->sresp = xmalloc (ctx->sizes.seccomp_notif_resp);

  если (is_empty_string (plugins))
    вернуть 0;

  b = xstrdup (plugins);

  ctx->n_plugins = 1;
  для (it = b; *it; it++)
    если (*it == ':')
      ctx->n_plugins++;

  ctx->plugins = xmalloc0 (sizeof (struct plugin) * (ctx->n_plugins + 1));
  для (s = 0, it = strtok_r (b, ":", &saveptr); it; s++, it = strtok_r (NULL, ":", &saveptr))
    {
      run_oci_seccomp_notify_plugin_version_cb version_cb;
      run_oci_seccomp_notify_start_cb start_cb;
      void *opq = NULL;

      /* do not accept relative paths.  It is fine to accept only filenames as dlopen() semantics apply.  */
      если (strchr (it, '/') && it[0] != '/')
        вернуть crun_make_error (err, 0, "invalid relative plugin path: `%s`", it);

      ctx->plugins[s].handle = dlopen (it, RTLD_NOW);
      если (ctx->plugins[s].handle == NULL)
        вернуть crun_make_error (err, 0, "cannot load `%s`: %s", it, dlerror ());

      version_cb
          = (run_oci_seccomp_notify_plugin_version_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_version");
      если (version_cb != NULL)
        {
          целое version;

          version = version_cb ();
          если (version != 1)
            вернуть crun_make_error (err, ENOTSUP, "invalid version supported by the plugin `%s`", it);
        }

      ctx->plugins[s].handle_request_cb = (run_oci_seccomp_notify_handle_request_cb) dlsym (
          ctx->plugins[s].handle, "run_oci_seccomp_notify_handle_request");
      если (ctx->plugins[s].handle_request_cb == NULL)
        вернуть crun_make_error (err, ENOTSUP, "plugin `%s` doesn't export `run_oci_seccomp_notify_handle_request`", it);

      start_cb = (run_oci_seccomp_notify_start_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_start");
      если (start_cb)
        {
          целое ret;

          ret = start_cb (&opq, conf, sizeof (*conf));
          если (UNLIKELY (ret != 0))
            вернуть crun_make_error (err, 0, "error loading `%s`", it);
        }
      ctx->plugins[s].opaque = opq;
    }

  /* Change ownership.  */
  *out = ctx;
  ctx = NULL;
  вернуть 0;
#иначе
  (void) out;
  (void) plugins;
  (void) conf;
  (void) err;
  вернуть crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC целое
libcrun_seccomp_notify_plugins (struct seccomp_notify_context_s *ctx, целое seccomp_fd, libcrun_error_t *err)
{
#если HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;
  целое ret;

  memset (ctx->sreq, 0, ctx->sizes.seccomp_notif);
  memset (ctx->sresp, 0, ctx->sizes.seccomp_notif_resp);

  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_RECV, ctx->sreq);
  если (UNLIKELY (ret < 0))
    {
      если (errno == ENOENT)
        вернуть 0;
      вернуть crun_make_error (err, errno, "ioctl");
    }

  для (i = 0; i < ctx->n_plugins; i++)
    {
      если (ctx->plugins[i].handle_request_cb)
        {
          целое handled = 0;
          целое ret;

          ret = ctx->plugins[i].handle_request_cb (ctx->plugins[i].opaque, &ctx->sizes, ctx->sreq, ctx->sresp,
                                                   seccomp_fd, &handled);
          если (UNLIKELY (ret != 0))
            вернуть crun_make_error (err, 0, "error handling seccomp notify request");

          switch (handled)
            {
            case RUN_OCI_SECCOMP_NOTIFY_HANDLE_NOT_HANDLED:
              break;

            case RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE:
              goto send_resp;

              /* The plugin will take care of it.  */
            case RUN_OCI_SECCOMP_NOTIFY_HANDLE_DELAYED_RESPONSE:
              вернуть 0;

            case RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE_AND_CONTINUE:
              ctx->sresp->flags |= SECCOMP_USER_NOTIF_FLAG_CONTINUE;
              goto send_resp;

            default:
              вернуть crun_make_error (err, EINVAL, "unknown action specified by the plugin `%d`", handled);
            }
        }
    }

  /* No plugin could handle the request.  */
  ctx->sresp->error = -ENOTSUP;
  ctx->sresp->flags = 0;

send_resp:
  ctx->sresp->id = ctx->sreq->id;
  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_SEND, ctx->sresp);
  если (UNLIKELY (ret < 0))
    {
      если (errno == ENOENT)
        вернуть 0;
      вернуть crun_make_error (err, errno, "ioctl");
    }
  вернуть 0;
#иначе
  (void) ctx;
  (void) seccomp_fd;
  (void) err;
  вернуть crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC целое
libcrun_free_seccomp_notify_plugins (struct seccomp_notify_context_s *ctx, libcrun_error_t *err)
{
#если HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;

  если (ctx == NULL)
    вернуть crun_make_error (err, EINVAL, "invalid seccomp notify context");

  free (ctx->sreq);
  free (ctx->sresp);

  для (i = 0; i < ctx->n_plugins; i++)
    если (ctx->plugins && ctx->plugins[i].handle)
      {
        run_oci_seccomp_notify_stop_cb cb;

        cb = (run_oci_seccomp_notify_stop_cb) dlsym (ctx->plugins[i].handle, "run_oci_seccomp_notify_stop");
        если (cb)
          cb (ctx->plugins[i].opaque);
        dlclose (ctx->plugins[i].handle);
      }

  free (ctx->plugins);
  free (ctx);

  вернуть 0;
#иначе
  (void) ctx;
  (void) err;
  вернуть crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}
