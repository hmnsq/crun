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
#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <errno.h>

#എങ്കിൽ HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
#  include <seccomp.h>
#  include <sys/ioctl.h>
#  include <linux/seccomp.h>
#  include <sys/sysmacros.h>
#endif

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "seccomp_notify.h"

#ifndef SECCOMP_USER_NOTIF_FLAG_CONTINUE
#  define SECCOMP_USER_NOTIF_FLAG_CONTINUE (1UL << 0)
#endif

ഘടന plugin
{
  ശൂന്യം *handle;
  ശൂന്യം *opaque;
#എങ്കിൽ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  run_oci_seccomp_notify_handle_request_cb handle_request_cb;
#endif
};

ഘടന seccomp_notify_context_s
{
  ഘടന plugin *plugins;
  size_t n_plugins;

#എങ്കിൽ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES
  ഘടന seccomp_notif_resp *sresp;
  ഘടന seccomp_notif *sreq;
  ഘടന seccomp_notif_sizes sizes;
#endif
};

ശൂന്യം
cleanup_seccomp_notify_pluginsp (ശൂന്യം *p)
{
  ഘടന seccomp_notify_context_s **pp = p;
  എങ്കിൽ (*pp)
    {
      libcrun_error_t tmp_err = ശൂന്യ;
      libcrun_free_seccomp_notify_plugins (*pp, &tmp_err);
      crun_error_release (&tmp_err);
      *pp = ശൂന്യ;
    }
}

#എങ്കിൽ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
സ്ഥിതം പൂർണ്ണസംഖ്യ
seccomp_syscall (unsigned പൂർണ്ണസംഖ്യ op, unsigned പൂർണ്ണസംഖ്യ flags, ശൂന്യം *args)
{
  errno = 0;
  തിരികെ syscall (__NR_seccomp, op, flags, args);
}
#endif

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ
libcrun_load_seccomp_notify_plugins (ഘടന seccomp_notify_context_s **out, സ്ഥിരം അക്ഷരം *plugins,
                                     ഘടന libcrun_load_seccomp_notify_conf_s *conf, libcrun_error_t *err)
{
#എങ്കിൽ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  cleanup_seccomp_notify_context ഘടന seccomp_notify_context_s *ctx = xmalloc0 (വലുപ്പം *ctx);
  cleanup_free അക്ഷരം *b = ശൂന്യ;
  അക്ഷരം *it, *saveptr;
  size_t s;

  എങ്കിൽ (seccomp_syscall (SECCOMP_GET_NOTIF_SIZES, 0, &ctx->sizes) < 0)
    തിരികെ crun_make_error (err, errno, "seccomp GET_NOTIF_SIZES");

  ctx->sreq = xmalloc (ctx->sizes.seccomp_notif);
  ctx->sresp = xmalloc (ctx->sizes.seccomp_notif_resp);

  എങ്കിൽ (is_empty_string (plugins))
    തിരികെ 0;

  b = xstrdup (plugins);

  ctx->n_plugins = 1;
  വേണ്ടി (it = b; *it; it++)
    എങ്കിൽ (*it == ':')
      ctx->n_plugins++;

  ctx->plugins = xmalloc0 (വലുപ്പം (ഘടന plugin) * (ctx->n_plugins + 1));
  വേണ്ടി (s = 0, it = strtok_r (b, ":", &saveptr); it; s++, it = strtok_r (ശൂന്യ, ":", &saveptr))
    {
      run_oci_seccomp_notify_plugin_version_cb version_cb;
      run_oci_seccomp_notify_start_cb start_cb;
      ശൂന്യം *opq = ശൂന്യ;

      /* do not accept relative paths.  It is fine to accept only filenames as dlopen() semantics apply.  */
      എങ്കിൽ (strchr (it, '/') && it[0] != '/')
        തിരികെ crun_make_error (err, 0, "invalid relative plugin path: `%s`", it);

      ctx->plugins[s].handle = dlopen (it, RTLD_NOW);
      എങ്കിൽ (ctx->plugins[s].handle == ശൂന്യ)
        തിരികെ crun_make_error (err, 0, "cannot load `%s`: %s", it, dlerror ());

      version_cb
          = (run_oci_seccomp_notify_plugin_version_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_version");
      എങ്കിൽ (version_cb != ശൂന്യ)
        {
          പൂർണ്ണസംഖ്യ version;

          version = version_cb ();
          എങ്കിൽ (version != 1)
            തിരികെ crun_make_error (err, ENOTSUP, "invalid version supported by the plugin `%s`", it);
        }

      ctx->plugins[s].handle_request_cb = (run_oci_seccomp_notify_handle_request_cb) dlsym (
          ctx->plugins[s].handle, "run_oci_seccomp_notify_handle_request");
      എങ്കിൽ (ctx->plugins[s].handle_request_cb == ശൂന്യ)
        തിരികെ crun_make_error (err, ENOTSUP, "plugin `%s` doesn't export `run_oci_seccomp_notify_handle_request`", it);

      start_cb = (run_oci_seccomp_notify_start_cb) dlsym (ctx->plugins[s].handle, "run_oci_seccomp_notify_start");
      എങ്കിൽ (start_cb)
        {
          പൂർണ്ണസംഖ്യ ret;

          ret = start_cb (&opq, conf, വലുപ്പം (*conf));
          എങ്കിൽ (UNLIKELY (ret != 0))
            തിരികെ crun_make_error (err, 0, "error loading `%s`", it);
        }
      ctx->plugins[s].opaque = opq;
    }

  /* Change ownership.  */
  *out = ctx;
  ctx = ശൂന്യ;
  തിരികെ 0;
#അല്ലെങ്കിൽ
  (ശൂന്യം) out;
  (ശൂന്യം) plugins;
  (ശൂന്യം) conf;
  (ശൂന്യം) err;
  തിരികെ crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ
libcrun_seccomp_notify_plugins (ഘടന seccomp_notify_context_s *ctx, പൂർണ്ണസംഖ്യ seccomp_fd, libcrun_error_t *err)
{
#എങ്കിൽ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  memset (ctx->sreq, 0, ctx->sizes.seccomp_notif);
  memset (ctx->sresp, 0, ctx->sizes.seccomp_notif_resp);

  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_RECV, ctx->sreq);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      എങ്കിൽ (errno == ENOENT)
        തിരികെ 0;
      തിരികെ crun_make_error (err, errno, "ioctl");
    }

  വേണ്ടി (i = 0; i < ctx->n_plugins; i++)
    {
      എങ്കിൽ (ctx->plugins[i].handle_request_cb)
        {
          പൂർണ്ണസംഖ്യ handled = 0;
          പൂർണ്ണസംഖ്യ ret;

          ret = ctx->plugins[i].handle_request_cb (ctx->plugins[i].opaque, &ctx->sizes, ctx->sreq, ctx->sresp,
                                                   seccomp_fd, &handled);
          എങ്കിൽ (UNLIKELY (ret != 0))
            തിരികെ crun_make_error (err, 0, "error handling seccomp notify request");

          തിരഞ്ഞെടുപ്പ് (handled)
            {
            അവസ്ഥ RUN_OCI_SECCOMP_NOTIFY_HANDLE_NOT_HANDLED:
              നിർത്തുക;

            അവസ്ഥ RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE:
              പോകുക send_resp;

              /* The plugin will take care of it.  */
            അവസ്ഥ RUN_OCI_SECCOMP_NOTIFY_HANDLE_DELAYED_RESPONSE:
              തിരികെ 0;

            അവസ്ഥ RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE_AND_CONTINUE:
              ctx->sresp->flags |= SECCOMP_USER_NOTIF_FLAG_CONTINUE;
              പോകുക send_resp;

            സ്ഥിരസ്ഥിതി:
              തിരികെ crun_make_error (err, EINVAL, "unknown action specified by the plugin `%d`", handled);
            }
        }
    }

  /* No plugin could handle the request.  */
  ctx->sresp->error = -ENOTSUP;
  ctx->sresp->flags = 0;

send_resp:
  ctx->sresp->id = ctx->sreq->id;
  ret = ioctl (seccomp_fd, SECCOMP_IOCTL_NOTIF_SEND, ctx->sresp);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      എങ്കിൽ (errno == ENOENT)
        തിരികെ 0;
      തിരികെ crun_make_error (err, errno, "ioctl");
    }
  തിരികെ 0;
#അല്ലെങ്കിൽ
  (ശൂന്യം) ctx;
  (ശൂന്യം) seccomp_fd;
  (ശൂന്യം) err;
  തിരികെ crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ
libcrun_free_seccomp_notify_plugins (ഘടന seccomp_notify_context_s *ctx, libcrun_error_t *err)
{
#എങ്കിൽ HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES && HAVE_SECCOMP
  size_t i;

  എങ്കിൽ (ctx == ശൂന്യ)
    തിരികെ crun_make_error (err, EINVAL, "invalid seccomp notify context");

  സ്വതന്ത്രം (ctx->sreq);
  സ്വതന്ത്രം (ctx->sresp);

  വേണ്ടി (i = 0; i < ctx->n_plugins; i++)
    എങ്കിൽ (ctx->plugins && ctx->plugins[i].handle)
      {
        run_oci_seccomp_notify_stop_cb cb;

        cb = (run_oci_seccomp_notify_stop_cb) dlsym (ctx->plugins[i].handle, "run_oci_seccomp_notify_stop");
        എങ്കിൽ (cb)
          cb (ctx->plugins[i].opaque);
        dlclose (ctx->plugins[i].handle);
      }

  സ്വതന്ത്രം (ctx->plugins);
  സ്വതന്ത്രം (ctx);

  തിരികെ 0;
#അല്ലെങ്കിൽ
  (ശൂന്യം) ctx;
  (ശൂന്യം) err;
  തിരികെ crun_make_error (err, ENOTSUP, "seccomp notify support not available");
#endif
}
