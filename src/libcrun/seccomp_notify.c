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
#అన్తర్భావ <config.h>
#అన్తర్భావ <errno.h>

#ఒకవేళ HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
#  include <seccomp.h>
#  include <sys/ioctl.h>
#  include <linux/seccomp.h>
#  include <sys/sysmacros.h>
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_DLOPEN
#  include <dlfcn.h>
#అయితే_ముగింపు

#అన్తర్భావ "utils.h"
#అన్తర్భావ "seccomp_notify.h"

#నిర్వచితం_కాకపోతే SECCOMP_USER_NOTIF_FLAG_CONTINUE
#  define SECCOMP_USER_NOTIF_FLAG_CONTINUE (1UL << 0)
#అయితే_ముగింపు

నిర్మాణం plugin
{
  శూన్య *handle;
  శూన్య *opaque;
#ఒకవేళ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  నిరంతరం_oci_seccomp_notify_handle_request_cb handle_request_cb;
#అయితే_ముగింపు
};

నిర్మాణం seccomp_notify_context_s
{
  నిర్మాణం plugin *plugins;
  size_t n_plugins;

#ఒకవేళ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES
  నిర్మాణం seccomp_notif_resp *sresp;
  నిర్మాణం seccomp_notif *sreq;
  నిర్మాణం seccomp_notif_sizes sizes;
#అయితే_ముగింపు
};

శూన్య
cleanup_seccomp_notify_pluginsp (శూన్య *p)
{
  నిర్మాణం seccomp_notify_context_s **pp = p;
  ఒకవేళ (*pp)
    {
      libcrun_error_t tmp_err = ఖాళీ;
      libcrun_free_seccomp_notify_plugins (*pp, &tmp_err);
      crun_error_release (&tmp_err);
      *pp = ఖాళీ;
    }
}

#ఒకవేళ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
స్థిర పూర్ణాంకం
seccomp_syscall (గుర్తులేని పూర్ణాంకం op, గుర్తులేని పూర్ణాంకం flags, శూన్య *వాదనలు)
{
  errno = 0;
  తిరిగి syscall (__NR_seccomp, op, flags, వాదనలు);
}
#అయితే_ముగింపు

LIBCRUN_ప్రజా పూర్ణాంకం
libcrun_ఎక్కించు_seccomp_notify_plugins (నిర్మాణం seccomp_notify_context_s **బహిర్గతం, స్థిరరాశి అక్షరం *plugins,
                                     నిర్మాణం libcrun_ఎక్కించు_seccomp_notify_conf_s *conf, libcrun_error_t *err)
{
#ఒకవేళ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  cleanup_seccomp_notify_context నిర్మాణం seccomp_notify_context_s *ctx = xmalloc0 (పరిమాణం *ctx);
  cleanup_free అక్షరం *b = ఖాళీ;
  అక్షరం *it, *saveptr;
  size_t s;

  ఒకవేళ (seccomp_syscall (SECCOMP_GET_NOTIF_SIZES, 0, &ctx->sizes) < 0)
    తిరిగి crun_make_error (err, errno, "seccomp GET_NOTIF_SIZES");

  ctx->sreq = xmalloc (ctx->sizes.seccomp_notif);
  ctx->sresp = xmalloc (ctx->sizes.seccomp_notif_resp);

  ఒకవేళ (is_ఖాళీలు_string (plugins))
    తిరిగి 0;

  b = xstrdup (plugins);

  ctx->n_plugins = 1;
  కొరకు (it = b; *it; it++)
    ఒకవేళ (*it == ':')
      ctx->n_plugins++;

  ctx->plugins = xmalloc0 (పరిమాణం (నిర్మాణం plugin) * (ctx->n_plugins + 1));
  కొరకు (s = 0, it = strtok_r (b, ":", &saveptr); it; s++, it = strtok_r (ఖాళీ, ":", &saveptr))
    {
      నిరంతరం_oci_seccomp_notify_plugin_సంస్కరణ_cb సంస్కరణ_cb;
      నిరంతరం_oci_seccomp_notify_ప్రారంభం_cb ప్రారంభం_cb;
      శూన్య *opq = ఖాళీ;

      /* do not accept relative paths.  It is fine to accept only filenames as dlopen() semantics apply.  */
      ఒకవేళ (strchr (it, '/') && it[0] != '/')
        తిరిగి crun_make_error (err, 0, "invalid relative plugin path: `%s`", it);

      ctx->plugins[s].handle = dlopen (it, RTLD_NOW);
      ఒకవేళ (ctx->plugins[s].handle == ఖాళీ)
        తిరిగి crun_make_error (err, 0, "cannot load `%s`: %s", it, dlerror ());

      సంస్కరణ_cb
          = (నిరంతరం_oci_seccomp_notify_plugin_సంస్కరణ_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_version");
      ఒకవేళ (సంస్కరణ_cb != ఖాళీ)
        {
          పూర్ణాంకం సంస్కరణ;

          సంస్కరణ = సంస్కరణ_cb ();
          ఒకవేళ (సంస్కరణ != 1)
            తిరిగి crun_make_error (err, ENOTSUP, "invalid version supported by the plugin `%s`", it);
        }

      ctx->plugins[s].handle_request_cb = (నిరంతరం_oci_seccomp_notify_handle_request_cb) dlsym (
          ctx->plugins[s].handle, "run_oci_seccomp_notify_handle_request");
      ఒకవేళ (ctx->plugins[s].handle_request_cb == ఖాళీ)
        తిరిగి crun_make_error (err, ENOTSUP, "plugin `%s` doesn't export `run_oci_seccomp_notify_handle_request`", it);

      ప్రారంభం_cb = (నిరంతరం_oci_seccomp_notify_ప్రారంభం_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_start");
      ఒకవేళ (ప్రారంభం_cb)
        {
          పూర్ణాంకం ret;

          ret = ప్రారంభం_cb (&opq, conf, పరిమాణం (*conf));
          ఒకవేళ (UNLIKELY (ret != 0))
            తిరిగి crun_make_error (err, 0, "error loading `%s`", it);
        }
      ctx->plugins[s].opaque = opq;
    }

  /* Change ownership.  */
  *బహిర్గతం = ctx;
  ctx = ఖాళీ;
  తిరిగి 0;
#లేకపోతే
  (శూన్య) బహిర్గతం;
  (శూన్య) plugins;
  (శూన్య) conf;
  (శూన్య) err;
  తిరిగి crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#అయితే_ముగింపు
}

LIBCRUN_ప్రజా పూర్ణాంకం
libcrun_seccomp_notify_plugins (నిర్మాణం seccomp_notify_context_s *ctx, పూర్ణాంకం seccomp_fd, libcrun_error_t *err)
{
#ఒకవేళ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;
  పూర్ణాంకం ret;

  memset (ctx->sreq, 0, ctx->sizes.seccomp_notif);
  memset (ctx->sresp, 0, ctx->sizes.seccomp_notif_resp);

  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_RECV, ctx->sreq);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ఒకవేళ (errno == ENOENT)
        తిరిగి 0;
      తిరిగి crun_make_error (err, errno, "ioctl");
    }

  కొరకు (i = 0; i < ctx->n_plugins; i++)
    {
      ఒకవేళ (ctx->plugins[i].handle_request_cb)
        {
          పూర్ణాంకం handled = 0;
          పూర్ణాంకం ret;

          ret = ctx->plugins[i].handle_request_cb (ctx->plugins[i].opaque, &ctx->sizes, ctx->sreq, ctx->sresp,
                                                   seccomp_fd, &handled);
          ఒకవేళ (UNLIKELY (ret != 0))
            తిరిగి crun_make_error (err, 0, "error handling seccomp notify request");

          ఎంపిక (handled)
            {
            స్థితి నిరంతరం_OCI_SECCOMP_NOTIFY_HANDLE_NOT_HANDLED:
              ఆపు;

            స్థితి నిరంతరం_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE:
              వెళ్ళు send_resp;

              /* The plugin will take care of it.  */
            స్థితి నిరంతరం_OCI_SECCOMP_NOTIFY_HANDLE_DELAYED_RESPONSE:
              తిరిగి 0;

            స్థితి నిరంతరం_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE_AND_CONTINUE:
              ctx->sresp->flags |= SECCOMP_USER_NOTIF_FLAG_CONTINUE;
              వెళ్ళు send_resp;

            అప్రమేయ:
              తిరిగి crun_make_error (err, EINVAL, "unknown action specified by the plugin `%d`", handled);
            }
        }
    }

  /* No plugin could handle the request.  */
  ctx->sresp->error = -ENOTSUP;
  ctx->sresp->flags = 0;

send_resp:
  ctx->sresp->id = ctx->sreq->id;
  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_SEND, ctx->sresp);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ఒకవేళ (errno == ENOENT)
        తిరిగి 0;
      తిరిగి crun_make_error (err, errno, "ioctl");
    }
  తిరిగి 0;
#లేకపోతే
  (శూన్య) ctx;
  (శూన్య) seccomp_fd;
  (శూన్య) err;
  తిరిగి crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#అయితే_ముగింపు
}

LIBCRUN_ప్రజా పూర్ణాంకం
libcrun_free_seccomp_notify_plugins (నిర్మాణం seccomp_notify_context_s *ctx, libcrun_error_t *err)
{
#ఒకవేళ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;

  ఒకవేళ (ctx == ఖాళీ)
    తిరిగి crun_make_error (err, EINVAL, "invalid seccomp notify context");

  విడుదల (ctx->sreq);
  విడుదల (ctx->sresp);

  కొరకు (i = 0; i < ctx->n_plugins; i++)
    ఒకవేళ (ctx->plugins && ctx->plugins[i].handle)
      {
        నిరంతరం_oci_seccomp_notify_stop_cb cb;

        cb = (నిరంతరం_oci_seccomp_notify_stop_cb) dlsym (ctx->plugins[i].handle, "run_oci_seccomp_notify_stop");
        ఒకవేళ (cb)
          cb (ctx->plugins[i].opaque);
        dlclose (ctx->plugins[i].handle);
      }

  విడుదల (ctx->plugins);
  విడుదల (ctx);

  తిరిగి 0;
#లేకపోతే
  (శూన్య) ctx;
  (శూన్య) err;
  తిరిగి crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#అయితే_ముగింపు
}
