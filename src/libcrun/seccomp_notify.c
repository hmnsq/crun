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
#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>

#ଯଦି HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
#  include <seccomp.h>
#  include <sys/ioctl.h>
#  include <linux/seccomp.h>
#  include <sys/sysmacros.h>
#endif

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "seccomp_notify.h"

#ifndef SECCOMP_USER_NOTIF_FLAG_CONTINUE
#  define SECCOMP_USER_NOTIF_FLAG_CONTINUE (1UL << 0)
#endif

ସଂରଚନା plugin
{
  ଶୂନ୍ୟ *handle;
  ଶୂନ୍ୟ *opaque;
#ଯଦି HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  run_oci_seccomp_notify_handle_request_cb handle_request_cb;
#endif
};

ସଂରଚନା seccomp_notify_context_s
{
  ସଂରଚନା plugin *plugins;
  size_t n_plugins;

#ଯଦି HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES
  ସଂରଚନା seccomp_notif_resp *sresp;
  ସଂରଚନା seccomp_notif *sreq;
  ସଂରଚନା seccomp_notif_sizes sizes;
#endif
};

ଶୂନ୍ୟ
cleanup_seccomp_notify_pluginsp (ଶୂନ୍ୟ *p)
{
  ସଂରଚନା seccomp_notify_context_s **pp = p;
  ଯଦି (*pp)
    {
      libcrun_error_t tmp_err = ଖାଲି;
      libcrun_free_seccomp_notify_plugins (*pp, &tmp_err);
      crun_error_release (&tmp_err);
      *pp = ଖାଲି;
    }
}

#ଯଦି HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
seccomp_syscall (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା op, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ଶୂନ୍ୟ *args)
{
  errno = 0;
  ଫେରନ୍ତୁ syscall (__NR_seccomp, op, flags, args);
}
#endif

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_load_seccomp_notify_plugins (ସଂରଚନା seccomp_notify_context_s **out, ଅଚଳ ଅକ୍ଷର *plugins,
                                     ସଂରଚନା libcrun_load_seccomp_notify_conf_s *conf, libcrun_error_t *err)
{
#ଯଦି HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  cleanup_seccomp_notify_context ସଂରଚନା seccomp_notify_context_s *ctx = xmalloc0 (ଆକାର *ctx);
  cleanup_free ଅକ୍ଷର *b = ଖାଲି;
  ଅକ୍ଷର *it, *saveptr;
  size_t s;

  ଯଦି (seccomp_syscall (SECCOMP_GET_NOTIF_SIZES, 0, &ctx->sizes) < 0)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "seccomp GET_NOTIF_SIZES");

  ctx->sreq = xmalloc (ctx->sizes.seccomp_notif);
  ctx->sresp = xmalloc (ctx->sizes.seccomp_notif_resp);

  ଯଦି (is_empty_string (plugins))
    ଫେରନ୍ତୁ 0;

  b = xstrdup (plugins);

  ctx->n_plugins = 1;
  ପାଇଁ (it = b; *it; it++)
    ଯଦି (*it == ':')
      ctx->n_plugins++;

  ctx->plugins = xmalloc0 (ଆକାର (ସଂରଚନା plugin) * (ctx->n_plugins + 1));
  ପାଇଁ (s = 0, it = strtok_r (b, ":", &saveptr); it; s++, it = strtok_r (ଖାଲି, ":", &saveptr))
    {
      run_oci_seccomp_notify_plugin_version_cb version_cb;
      run_oci_seccomp_notify_start_cb start_cb;
      ଶୂନ୍ୟ *opq = ଖାଲି;

      /* do not accept relative paths.  It is fine to accept only filenames as dlopen() semantics apply.  */
      ଯଦି (strchr (it, '/') && it[0] != '/')
        ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid relative plugin path: `%s`", it);

      ctx->plugins[s].handle = dlopen (it, RTLD_NOW);
      ଯଦି (ctx->plugins[s].handle == ଖାଲି)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot load `%s`: %s", it, dlerror ());

      version_cb
          = (run_oci_seccomp_notify_plugin_version_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_version");
      ଯଦି (version_cb != ଖାଲି)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା version;

          version = version_cb ();
          ଯଦି (version != 1)
            ଫେରନ୍ତୁ crun_make_error (err, ENOTSUP, "invalid version supported by the plugin `%s`", it);
        }

      ctx->plugins[s].handle_request_cb = (run_oci_seccomp_notify_handle_request_cb) dlsym (
          ctx->plugins[s].handle, "run_oci_seccomp_notify_handle_request");
      ଯଦି (ctx->plugins[s].handle_request_cb == ଖାଲି)
        ଫେରନ୍ତୁ crun_make_error (err, ENOTSUP, "plugin `%s` doesn't export `run_oci_seccomp_notify_handle_request`", it);

      start_cb = (run_oci_seccomp_notify_start_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_start");
      ଯଦି (start_cb)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

          ret = start_cb (&opq, conf, ଆକାର (*conf));
          ଯଦି (UNLIKELY (ret != 0))
            ଫେରନ୍ତୁ crun_make_error (err, 0, "error loading `%s`", it);
        }
      ctx->plugins[s].opaque = opq;
    }

  /* Change ownership.  */
  *out = ctx;
  ctx = ଖାଲି;
  ଫେରନ୍ତୁ 0;
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) out;
  (ଶୂନ୍ୟ) plugins;
  (ଶୂନ୍ୟ) conf;
  (ଶୂନ୍ୟ) err;
  ଫେରନ୍ତୁ crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_seccomp_notify_plugins (ସଂରଚନା seccomp_notify_context_s *ctx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା seccomp_fd, libcrun_error_t *err)
{
#ଯଦି HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  memset (ctx->sreq, 0, ctx->sizes.seccomp_notif);
  memset (ctx->sresp, 0, ctx->sizes.seccomp_notif_resp);

  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_RECV, ctx->sreq);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ଯଦି (errno == ENOENT)
        ଫେରନ୍ତୁ 0;
      ଫେରନ୍ତୁ crun_make_error (err, errno, "ioctl");
    }

  ପାଇଁ (i = 0; i < ctx->n_plugins; i++)
    {
      ଯଦି (ctx->plugins[i].handle_request_cb)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା handled = 0;
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

          ret = ctx->plugins[i].handle_request_cb (ctx->plugins[i].opaque, &ctx->sizes, ctx->sreq, ctx->sresp,
                                                   seccomp_fd, &handled);
          ଯଦି (UNLIKELY (ret != 0))
            ଫେରନ୍ତୁ crun_make_error (err, 0, "error handling seccomp notify request");

          ବାଛ (handled)
            {
            ସ୍ଥିତି RUN_OCI_SECCOMP_NOTIFY_HANDLE_NOT_HANDLED:
              ଭାଙ୍ଗ;

            ସ୍ଥିତି RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE:
              ଯାଅ send_resp;

              /* The plugin will take care of it.  */
            ସ୍ଥିତି RUN_OCI_SECCOMP_NOTIFY_HANDLE_DELAYED_RESPONSE:
              ଫେରନ୍ତୁ 0;

            ସ୍ଥିତି RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE_AND_CONTINUE:
              ctx->sresp->flags |= SECCOMP_USER_NOTIF_FLAG_CONTINUE;
              ଯାଅ send_resp;

            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
              ଫେରନ୍ତୁ crun_make_error (err, EINVAL, "unknown action specified by the plugin `%d`", handled);
            }
        }
    }

  /* No plugin could handle the request.  */
  ctx->sresp->error = -ENOTSUP;
  ctx->sresp->flags = 0;

send_resp:
  ctx->sresp->id = ctx->sreq->id;
  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_SEND, ctx->sresp);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ଯଦି (errno == ENOENT)
        ଫେରନ୍ତୁ 0;
      ଫେରନ୍ତୁ crun_make_error (err, errno, "ioctl");
    }
  ଫେରନ୍ତୁ 0;
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) ctx;
  (ଶୂନ୍ୟ) seccomp_fd;
  (ଶୂନ୍ୟ) err;
  ଫେରନ୍ତୁ crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_free_seccomp_notify_plugins (ସଂରଚନା seccomp_notify_context_s *ctx, libcrun_error_t *err)
{
#ଯଦି HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;

  ଯଦି (ctx == ଖାଲି)
    ଫେରନ୍ତୁ crun_make_error (err, EINVAL, "invalid seccomp notify context");

  ମୁକ୍ତ (ctx->sreq);
  ମୁକ୍ତ (ctx->sresp);

  ପାଇଁ (i = 0; i < ctx->n_plugins; i++)
    ଯଦି (ctx->plugins && ctx->plugins[i].handle)
      {
        run_oci_seccomp_notify_stop_cb cb;

        cb = (run_oci_seccomp_notify_stop_cb) dlsym (ctx->plugins[i].handle, "run_oci_seccomp_notify_stop");
        ଯଦି (cb)
          cb (ctx->plugins[i].opaque);
        dlclose (ctx->plugins[i].handle);
      }

  ମୁକ୍ତ (ctx->plugins);
  ମୁକ୍ତ (ctx);

  ଫେରନ୍ତୁ 0;
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) ctx;
  (ଶୂନ୍ୟ) err;
  ଫେରନ୍ତୁ crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}
