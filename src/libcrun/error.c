/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdarg.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <json-c/json.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ocispec/json_common.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYSTEMD
#  include <systemd/sd-journal.h>
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ
{
  LOG_FORMAT_ਪਾਠ = 0,
  LOG_FORMAT_JSON,
};

ਸਥਿਰ ਪੂਰਨ log_format;
ਸਥਿਰ ਪੂਰਨ output_verbosity = LIBCRUN_VERBOSITY_ERROR;

ਪੂਰਨ
libcrun_make_error (libcrun_error_t *err, ਪੂਰਨ ਸਥਿਤੀ, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ...)
{
  va_list ਦਲੀਲਾਂ_list;
  libcrun_error_t ptr;

  va_ਸ਼ੁਰੂ (ਦਲੀਲਾਂ_list, ਸੁਨੇਹਾ);
  *err = xmalloc (ਆਕਾਰ (ਬਣਤਰ libcrun_error_s));
  ptr = *err;
  ptr->ਸਥਿਤੀ = ਸਥਿਤੀ;
  ਜੇ (vasprintf (&(ptr->ਸੁਨੇਹਾ), ਸੁਨੇਹਾ, ਦਲੀਲਾਂ_list) < 0)
    OOM ();
  va_ਸਿਰਾ (ਦਲੀਲਾਂ_list);

  ਵਾਪਸ -ਸਥਿਤੀ - 1;
}

ਪੂਰਨ
crun_error_wrap (libcrun_error_t *err, ਅਚਲ ਅੱਖਰ *fmt, ...)
{
  cleanup_free ਅੱਖਰ *ਸੁਨੇਹਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *tmp = ਨਿਰਾ;
  va_list ਦਲੀਲਾਂ_list;
  ਅੱਖਰ *ਅਦਲਾ_ਬਦਲੀ;
  ਪੂਰਨ ret;

  ਜੇ (err == ਨਿਰਾ || *err == ਨਿਰਾ)
    {
      // Internal error
      ਵਾਪਸ 0;
    }

  ret = -(*err)->ਸਥਿਤੀ - 1;

  va_ਸ਼ੁਰੂ (ਦਲੀਲਾਂ_list, fmt);

  ਜੇ (vasprintf (&ਸੁਨੇਹਾ, fmt, ਦਲੀਲਾਂ_list) < 0)
    {
      va_ਸਿਰਾ (ਦਲੀਲਾਂ_list);
      ਸੁਨੇਹਾ = ਨਿਰਾ;
      ਵਾਪਸ ret;
    }
  va_ਸਿਰਾ (ਦਲੀਲਾਂ_list);

  xasprintf (&tmp, "%s: %s", ਸੁਨੇਹਾ, (*err)->ਸੁਨੇਹਾ);
  ਅਦਲਾ_ਬਦਲੀ = tmp;
  tmp = (*err)->ਸੁਨੇਹਾ;
  (*err)->ਸੁਨੇਹਾ = ਅਦਲਾ_ਬਦਲੀ;

  ਵਾਪਸ ret;
}

ਪੂਰਨ
crun_error_release (libcrun_error_t *err)
{
  ਅਚਲ ਪੂਰਨ saved_errno = errno;
  libcrun_error_t ptr;

  ਜੇ (err == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ptr = *err;
  ਜੇ (ptr == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ਮੁਕਤ (ptr->ਸੁਨੇਹਾ);
  ਮੁਕਤ (ptr);
  *err = ਨਿਰਾ;

  errno = saved_errno;
  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_error_release (libcrun_error_t *err)
{
  ਵਾਪਸ crun_error_release (err);
}

ਪੂਰਨ
libcrun_error_get_ਸਥਿਤੀ (libcrun_error_t err)
{
  ਜੇ (err == ਨਿਰਾ)
    ਵਾਪਸ 0;
  ਵਾਪਸ err->ਸਥਿਤੀ;
}

ਅਚਲ ਅੱਖਰ *
libcrun_error_get_ਸੁਨੇਹਾ (libcrun_error_t err)
{
  ਜੇ (err == ਨਿਰਾ)
    ਵਾਪਸ ਨਿਰਾ;
  ਵਾਪਸ err->ਸੁਨੇਹਾ;
}

ਖਾਲੀ
crun_error_write_warning_and_release (FILE *ਬਾਹਰਲਾ, libcrun_error_t **err)
{
  ਅਚਲ ਪੂਰਨ saved_errno = errno;
  libcrun_error_t ref;

  ਜੇ (ਬਾਹਰਲਾ == ਨਿਰਾ)
    ਬਾਹਰਲਾ = stderr;
  ਜੇ (err == ਨਿਰਾ || *err == ਨਿਰਾ || **err == ਨਿਰਾ)
    {
      // Internal error
      ਵਾਪਸ;
    }

  ref = **err;
  ਜੇ (ref->ਸਥਿਤੀ)
    fprintf (ਬਾਹਰਲਾ, "%s: %s\n", ref->ਸੁਨੇਹਾ, strerror (ref->ਸਥਿਤੀ));
  ਨਹੀਂਤਾਂ
    fprintf (ਬਾਹਰਲਾ, "%s\n", ref->ਸੁਨੇਹਾ);

  ਮੁਕਤ (ref->ਸੁਨੇਹਾ);
  ਮੁਕਤ (ref);
  **err = ਨਿਰਾ;

  errno = saved_errno;
}

ਖਾਲੀ
libcrun_error_write_warning_and_release (FILE *ਬਾਹਰਲਾ, libcrun_error_t **err)
{
  ਵਾਪਸ crun_error_write_warning_and_release (ਬਾਹਰਲਾ, err);
}

ਖਾਲੀ
libcrun_error_report_and_release (libcrun_error_t *err)
{
  libcrun_error_t ref;

  ਜੇ (err == ਨਿਰਾ || *err == ਨਿਰਾ)
    ਵਾਪਸ;

  ref = *err;
  libcrun_error (ref->ਸਥਿਤੀ, "%s", ref->ਸੁਨੇਹਾ);
  crun_error_release (err);
}

ਪੂਰਨ
crun_error_get_errno (libcrun_error_t *err)
{
  ਜੇ (err == ਨਿਰਾ || *err == ਨਿਰਾ)
    ਵਾਪਸ 0;
  ਵਾਪਸ (*err)->ਸਥਿਤੀ;
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੱਖਰ timestamp_t[64];

ਸਥਿਰ ਖਾਲੀ
get_timestamp (timestamp_t *timestamp, ਅਚਲ ਅੱਖਰ *suffix)
{
  ਅਚਲ size_t ਬਫਰ_ਮਾਪ = ਆਕਾਰ (timestamp_t);
  ਅੱਖਰ *ਬਫਰ = (ਅੱਖਰ *) timestamp;
  ਬਣਤਰ timeval tv;
  size_t written;
  ਬਣਤਰ tm now;

  gettimeofday (&tv, ਨਿਰਾ);
  gmtime_r (&tv.tv_sec, &now);
  written = strftime (ਬਫਰ, ਬਫਰ_ਮਾਪ, "%Y-%m-%dT%H:%M:%S", &now);
  snprintf (ਬਫਰ + written, ਬਫਰ_ਮਾਪ - written, ".%06lldZ%.8s", (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ) tv.tv_usec, suffix);
}

/* Neither openlog(3) nor the journald handler copy the identifier they are
   given, and the global output handler outlives the caller's strings (e.g.
   the context is freed while errors are still reported through it), so keep
   an owned copy alive for as long as the handler can use it.  */
ਸਥਿਰ ਅੱਖਰ *log_id;

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
ਸੈੱਟ_log_id (ਅਚਲ ਅੱਖਰ *id, ਅੱਖਰ **old_id)
{
  *old_id = log_id;
  log_id = id ? xstrdup (id) : ਨਿਰਾ;
  ਵਾਪਸ log_id;
}

ਸਥਿਰ ਖਾਲੀ *
init_syslog (ਅਚਲ ਅੱਖਰ *id)
{
  openlog (id, 0, LOG_USER);
  ਵਾਪਸ ਨਿਰਾ;
}

ਗਿਣਤੀ
{
  LOG_TYPE_FILE = 1,
  LOG_TYPE_SYSLOG = 2,
  LOG_TYPE_JOURNALD = 3
};

ਸਥਿਰ ਪੂਰਨ
get_log_type (ਅਚਲ ਅੱਖਰ *ਲਘੂਗਣਕ, ਅਚਲ ਅੱਖਰ **ਅੰਕੜੇ)
{
  ਅਚਲ ਅੱਖਰ *sep = strchr (ਲਘੂਗਣਕ, ':');
  ਜੇ (sep == ਨਿਰਾ)
    {
      *ਅੰਕੜੇ = ਲਘੂਗਣਕ;
      ਵਾਪਸ LOG_TYPE_FILE;
    }
  *ਅੰਕੜੇ = sep + 1;

  ਜੇ (has_ਅਗੇਤਰ (ਲਘੂਗਣਕ, "syslog:"))
    ਵਾਪਸ LOG_TYPE_SYSLOG;
  ਜੇ (has_ਅਗੇਤਰ (ਲਘੂਗਣਕ, "journald:"))
    ਵਾਪਸ LOG_TYPE_JOURNALD;
  ਜੇ (has_ਅਗੇਤਰ (ਲਘੂਗਣਕ, "file:"))
    ਵਾਪਸ LOG_TYPE_FILE;

  ਵਾਪਸ -1;
}

ਪੂਰਨ
libcrun_init_logging (crun_output_handler *new_output_handler, ਖਾਲੀ **new_output_handler_arg, ਅਚਲ ਅੱਖਰ *id,
                      ਅਚਲ ਅੱਖਰ *ਲਘੂਗਣਕ, libcrun_error_t *err)
{
  /* Released only after the new handler is installed.  */
  cleanup_free ਅੱਖਰ *old_log_id = ਨਿਰਾ;

  ਜੇ (ਲਘੂਗਣਕ == ਨਿਰਾ)
    {
      *new_output_handler = log_write_ਮੰਜ਼ਿਲ_stderr;
      *new_output_handler_arg = ਨਿਰਾ;
    }
  ਨਹੀਂਤਾਂ
    {
      ਅਚਲ ਅੱਖਰ *arg = ਨਿਰਾ;
      ਪੂਰਨ log_type = get_log_type (ਲਘੂਗਣਕ, &arg);

      ਜੇ (log_type < 0)
        ਵਾਪਸ crun_make_error (err, errno, "unknown log type `%s`", ਲਘੂਗਣਕ);

      ਚੋਣ (log_type)
        {
        ਹਾਲਤ LOG_TYPE_FILE:
          *new_output_handler = log_write_ਮੰਜ਼ਿਲ_ਧਾਰਾ;
          *new_output_handler_arg = fopen (arg, "a+e");
          ਜੇ (*new_output_handler_arg == ਨਿਰਾ)
            ਵਾਪਸ crun_make_error (err, errno, "open log file `%s`", ਲਘੂਗਣਕ);
          ਜੇ (output_verbosity >= LIBCRUN_VERBOSITY_WARNING)
            setlinebuf (*new_output_handler_arg);
          ਰੋਕੋ;

        ਹਾਲਤ LOG_TYPE_SYSLOG:
          *new_output_handler_arg = init_syslog (ਸੈੱਟ_log_id (arg[0] ? arg : id, &old_log_id));
          *new_output_handler = log_write_ਮੰਜ਼ਿਲ_syslog;
          ਰੋਕੋ;

        ਹਾਲਤ LOG_TYPE_JOURNALD:
          *new_output_handler = log_write_ਮੰਜ਼ਿਲ_journald;
          *new_output_handler_arg = (ਖਾਲੀ *) ਸੈੱਟ_log_id (id, &old_log_id);
          ਰੋਕੋ;
        }
    }
  crun_ਸੈੱਟ_output_handler (*new_output_handler, *new_output_handler_arg);
  ਵਾਪਸ 0;
}

ਖਾਲੀ
log_write_ਮੰਜ਼ਿਲ_ਧਾਰਾ (ਪੂਰਨ errno_, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ਪੂਰਨ verbosity, ਖਾਲੀ *arg)
{
  timestamp_t timestamp = {
    0,
  };
  FILE *ਧਾਰਾ = arg;
  ਪੂਰਨ tty = isatty (fileno (ਧਾਰਾ));
  ਅਚਲ ਅੱਖਰ *color_begin = "";
  ਅਚਲ ਅੱਖਰ *color_ਸਿਰਾ = "";

  ਜੇ (tty)
    {
      ਚੋਣ (verbosity)
        {
        ਹਾਲਤ LIBCRUN_VERBOSITY_DEBUG:
          color_begin = "\x1b[1;34m";
          color_ਸਿਰਾ = "\x1b[0m";
          ਰੋਕੋ;
        ਹਾਲਤ LIBCRUN_VERBOSITY_WARNING:
          color_begin = "\x1b[1;33m";
          color_ਸਿਰਾ = "\x1b[0m";
          ਰੋਕੋ;
        ਹਾਲਤ LIBCRUN_VERBOSITY_ERROR:
          color_begin = "\x1b[1;31m";
          color_ਸਿਰਾ = "\x1b[0m";
          ਰੋਕੋ;
        }

      ਜੇ (log_format == LOG_FORMAT_ਪਾਠ)
        get_timestamp (&timestamp, ": ");
    }

  ਜੇ (errno_)
    fprintf (ਧਾਰਾ, "%s%s%s: %s%s\n", color_begin, timestamp, ਸੁਨੇਹਾ, strerror (errno_), color_ਸਿਰਾ);
  ਨਹੀਂਤਾਂ
    fprintf (ਧਾਰਾ, "%s%s%s%s\n", color_begin, timestamp, ਸੁਨੇਹਾ, color_ਸਿਰਾ);
}

ਖਾਲੀ
log_write_ਮੰਜ਼ਿਲ_stderr (ਪੂਰਨ errno_, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ਪੂਰਨ verbosity, ਖਾਲੀ *arg arg_ਅਣਵਰਤਿਆ)
{
  log_write_ਮੰਜ਼ਿਲ_ਧਾਰਾ (errno_, ਸੁਨੇਹਾ, verbosity, stderr);
}

ਖਾਲੀ
log_write_ਮੰਜ਼ਿਲ_syslog (ਪੂਰਨ errno_, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ਪੂਰਨ verbosity, ਖਾਲੀ *arg arg_ਅਣਵਰਤਿਆ)
{
  ਪੂਰਨ ਤਰਜੀਹ = LOG_ERR;
  ਚੋਣ (verbosity)
    {
    ਹਾਲਤ LIBCRUN_VERBOSITY_DEBUG:
      ਤਰਜੀਹ = LOG_DEBUG;
      ਰੋਕੋ;
    ਹਾਲਤ LIBCRUN_VERBOSITY_WARNING:
      ਤਰਜੀਹ = LOG_WARNING;
      ਰੋਕੋ;
    ਹਾਲਤ LIBCRUN_VERBOSITY_ERROR:
      ਤਰਜੀਹ = LOG_ERR;
      ਰੋਕੋ;
    }
  ਜੇ (errno_ == 0)
    syslog (ਤਰਜੀਹ, "%s", ਸੁਨੇਹਾ);
  ਨਹੀਂਤਾਂ
    syslog (ਤਰਜੀਹ, "%s: %s", ਸੁਨੇਹਾ, strerror (errno_));
}

ਖਾਲੀ
log_write_ਮੰਜ਼ਿਲ_journald (ਪੂਰਨ errno_, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ਪੂਰਨ verbosity, ਖਾਲੀ *arg arg_ਅਣਵਰਤਿਆ)
{
  (ਖਾਲੀ) errno_;
  (ਖਾਲੀ) ਸੁਨੇਹਾ;
  (ਖਾਲੀ) verbosity;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYSTEMD
  ਪੂਰਨ ਤਰਜੀਹ = LOG_ERR;
  ਚੋਣ (verbosity)
    {
    ਹਾਲਤ LIBCRUN_VERBOSITY_DEBUG:
      ਤਰਜੀਹ = LOG_DEBUG;
      ਰੋਕੋ;
    ਹਾਲਤ LIBCRUN_VERBOSITY_WARNING:
      ਤਰਜੀਹ = LOG_WARNING;
      ਰੋਕੋ;
    ਹਾਲਤ LIBCRUN_VERBOSITY_ERROR:
      ਤਰਜੀਹ = LOG_ERR;
      ਰੋਕੋ;
    }
  ਜੇ (errno_ == 0)
    sd_journal_send ("PRIORITY=%d", ਤਰਜੀਹ, "MESSAGE=%s", ਸੁਨੇਹਾ, "ID=%s", arg, ਨਿਰਾ);
  ਨਹੀਂਤਾਂ
    sd_journal_send ("PRIORITY=%d", ਤਰਜੀਹ, "MESSAGE=%s: %s", ਸੁਨੇਹਾ, strerror (errno_), "ID=%s",
                     arg, ਨਿਰਾ);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ crun_output_handler output_handler = log_write_ਮੰਜ਼ਿਲ_stderr;
ਸਥਿਰ ਖਾਲੀ *output_handler_arg = ਨਿਰਾ;

ਖਾਲੀ
libcrun_ਸੈੱਟ_verbosity (ਪੂਰਨ verbosity)
{
  output_verbosity = verbosity;
}

ਪੂਰਨ
libcrun_get_verbosity ()
{
  ਵਾਪਸ output_verbosity;
}

ਖਾਲੀ
crun_ਸੈੱਟ_output_handler (crun_output_handler handler, ਖਾਲੀ *arg)
{
  output_handler = handler;
  output_handler_arg = arg;
}

ਸਥਿਰ ਅੱਖਰ *
make_json_error (ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ਪੂਰਨ errno_, ਪੂਰਨ verbosity)
{
  ਅਚਲ ਅੱਖਰ *level;
  ਚੋਣ (verbosity)
    {
    ਹਾਲਤ LIBCRUN_VERBOSITY_DEBUG:
      level = "debug";
      ਰੋਕੋ;
    ਹਾਲਤ LIBCRUN_VERBOSITY_WARNING:
      level = "warning";
      ਰੋਕੋ;
    ਹਾਲਤ LIBCRUN_VERBOSITY_ERROR:
      level = "error";
      ਰੋਕੋ;
    }
  ਅਚਲ ਅੱਖਰ *buf = ਨਿਰਾ;
  json_gen_ctx *gen = ਨਿਰਾ;
  ਅੱਖਰ *ret = ਨਿਰਾ;
  size_t buf_len;
  timestamp_t timestamp = {
    0,
  };

  ਜੇ (! json_gen_init (&gen, ਨਿਰਾ))
    ਵਾਪਸ ਨਿਰਾ;

  get_timestamp (&timestamp, "");

  json_gen_map_open (gen);

  json_gen_string (gen, "msg", ਲੜੀ_ਲੰਬਾਈ ("msg"));
  ਜੇ (errno_ == 0)
    json_gen_string (gen, ਸੁਨੇਹਾ, ਲੜੀ_ਲੰਬਾਈ (ਸੁਨੇਹਾ));
  ਨਹੀਂਤਾਂ
    {
      cleanup_free ਅੱਖਰ *tmp = ਨਿਰਾ;

      xasprintf (&tmp, "%s: %s", ਸੁਨੇਹਾ, strerror (errno_));
      json_gen_string (gen, tmp, ਲੜੀ_ਲੰਬਾਈ (tmp));
    }

  json_gen_string (gen, "level", ਲੜੀ_ਲੰਬਾਈ ("level"));
  json_gen_string (gen, level, ਲੜੀ_ਲੰਬਾਈ (level));

  json_gen_string (gen, "time", ਲੜੀ_ਲੰਬਾਈ ("time"));
  json_gen_string (gen, timestamp, ਲੜੀ_ਲੰਬਾਈ (timestamp));

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);
  ਜੇ (buf)
    ret = strdup (buf);

  json_gen_free (gen);

  ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ
write_log (ਪੂਰਨ errno_, ਪੂਰਨ verbosity, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, va_list ਦਲੀਲਾਂ_list)
{
  ਪੂਰਨ ret;
  cleanup_free ਅੱਖਰ *output = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *json = ਨਿਰਾ;

  ਜੇ (verbosity > output_verbosity)
    ਵਾਪਸ;

  ret = vasprintf (&output, ਸੁਨੇਹਾ, ਦਲੀਲਾਂ_list);
  ਜੇ (UNLIKELY (ret < 0))
    OOM ();

  ਜੇ (verbosity == LIBCRUN_VERBOSITY_ERROR && output_handler != log_write_ਮੰਜ਼ਿਲ_stderr)
    log_write_ਮੰਜ਼ਿਲ_stderr (errno_, output, LIBCRUN_VERBOSITY_ERROR, ਨਿਰਾ);

  ਚੋਣ (log_format)
    {
    ਹਾਲਤ LOG_FORMAT_ਪਾਠ:
      output_handler (errno_, output, verbosity, output_handler_arg);
      ਰੋਕੋ;

    ਹਾਲਤ LOG_FORMAT_JSON:
      json = make_json_error (output, errno_, verbosity);
      ਜੇ (json)
        output_handler (0, json, verbosity, output_handler_arg);
      ਨਹੀਂਤਾਂ
        output_handler (errno_, output, verbosity, output_handler_arg);
      ਰੋਕੋ;
    }
}

ਖਾਲੀ
libcrun_debug (ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ...)
{
  va_list ਦਲੀਲਾਂ_list;
  va_ਸ਼ੁਰੂ (ਦਲੀਲਾਂ_list, ਸੁਨੇਹਾ);
  write_log (0, LIBCRUN_VERBOSITY_DEBUG, ਸੁਨੇਹਾ, ਦਲੀਲਾਂ_list);
  va_ਸਿਰਾ (ਦਲੀਲਾਂ_list);
}

ਖਾਲੀ
libcrun_warning (ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ...)
{
  va_list ਦਲੀਲਾਂ_list;
  va_ਸ਼ੁਰੂ (ਦਲੀਲਾਂ_list, ਸੁਨੇਹਾ);
  write_log (0, LIBCRUN_VERBOSITY_WARNING, ਸੁਨੇਹਾ, ਦਲੀਲਾਂ_list);
  va_ਸਿਰਾ (ਦਲੀਲਾਂ_list);
}

ਖਾਲੀ
libcrun_error (ਪੂਰਨ errno_, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ...)
{
  va_list ਦਲੀਲਾਂ_list;
  va_ਸ਼ੁਰੂ (ਦਲੀਲਾਂ_list, ਸੁਨੇਹਾ);

  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, ਸੁਨੇਹਾ, ਦਲੀਲਾਂ_list);
  va_ਸਿਰਾ (ਦਲੀਲਾਂ_list);
}

ਖਾਲੀ __attribute__ ((noreturn))
libcrun_fail_with_error (ਪੂਰਨ errno_, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ...)
{
  va_list ਦਲੀਲਾਂ_list;
  va_ਸ਼ੁਰੂ (ਦਲੀਲਾਂ_list, ਸੁਨੇਹਾ);
  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, ਸੁਨੇਹਾ, ਦਲੀਲਾਂ_list);
  va_ਸਿਰਾ (ਦਲੀਲਾਂ_list);
  ਬਾਹਰ_ਨਿਕਲੋ (EXIT_FAILURE);
}

ਪੂਰਨ
libcrun_ਸੈੱਟ_log_format (ਅਚਲ ਅੱਖਰ *format, libcrun_error_t *err)
{
  ਜੇ (strcmp (format, "text") == 0)
    log_format = LOG_FORMAT_ਪਾਠ;
  ਨਹੀਂਤਾਂ ਜੇ (strcmp (format, "json") == 0)
    log_format = LOG_FORMAT_JSON;
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ crun_make_error (err, 0, "unknown log format type `%s`", format);

  ਵਾਪਸ 0;
}

ਪੂਰਨ
json_gen_error_ਮੰਜ਼ਿਲ_crun_error (ਪੂਰਨ ਸਥਿਤੀ, libcrun_error_t *err)
{
  ਚੋਣ (ਸਥਿਤੀ)
    {
    ਹਾਲਤ json_gen_ਸਥਿਤੀ_ok:
      ਵਾਪਸ 0;

    ਮੂਲ:
      ਵਾਪਸ crun_make_error (err, 0, "generate JSON document: error %d", ਸਥਿਤੀ);
    }
}
