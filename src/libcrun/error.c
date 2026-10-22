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

#define _GNU_SOURCE
#ضمن <config.h>
#ضمن <stdarg.h>
#ضمن <unistd.h>
#ضمن <string.h>
#ضمن <time.h>
#ضمن <sys/time.h>
#ضمن <stdio.h>
#ضمن "utils.h"

#ضمن <json-c/json.h>
#ضمن <ocispec/json_common.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-journal.h>
#endif

تعداد
{
  LOG_FORMAT_TEXT = 0,
  LOG_FORMAT_JSON,
};

ساكن صحيح log_format;
ساكن صحيح output_verbosity = LIBCRUN_VERBOSITY_ERROR;

صحيح
libcrun_make_error (libcrun_error_t *err, صحيح status, ثابت حرف *msg, ...)
{
  va_list args_list;
  libcrun_error_t ptr;

  va_start (args_list, msg);
  *err = xmalloc (حجم (بنية libcrun_error_s));
  ptr = *err;
  ptr->status = status;
  𐤀𐤌 (vasprintf (&(ptr->msg), msg, args_list) < 0)
    OOM ();
  va_end (args_list);

  أرجع -status - 1;
}

صحيح
crun_error_wrap (libcrun_error_t *err, ثابت حرف *fmt, ...)
{
  cleanup_free حرف *msg = عدم;
  cleanup_free حرف *tmp = عدم;
  va_list args_list;
  حرف *swap;
  صحيح ret;

  𐤀𐤌 (err == عدم || *err == عدم)
    {
      // Internal error
      أرجع 0;
    }

  ret = -(*err)->status - 1;

  va_start (args_list, fmt);

  𐤀𐤌 (vasprintf (&msg, fmt, args_list) < 0)
    {
      va_end (args_list);
      msg = عدم;
      أرجع ret;
    }
  va_end (args_list);

  xasprintf (&tmp, "%s: %s", msg, (*err)->msg);
  swap = tmp;
  tmp = (*err)->msg;
  (*err)->msg = swap;

  أرجع ret;
}

صحيح
crun_error_release (libcrun_error_t *err)
{
  ثابت صحيح saved_errno = errno;
  libcrun_error_t ptr;

  𐤀𐤌 (err == عدم)
    أرجع 0;

  ptr = *err;
  𐤀𐤌 (ptr == عدم)
    أرجع 0;

  حرر (ptr->msg);
  حرر (ptr);
  *err = عدم;

  errno = saved_errno;
  أرجع 0;
}

صحيح
libcrun_error_release (libcrun_error_t *err)
{
  أرجع crun_error_release (err);
}

صحيح
libcrun_error_get_status (libcrun_error_t err)
{
  𐤀𐤌 (err == عدم)
    أرجع 0;
  أرجع err->status;
}

ثابت حرف *
libcrun_error_get_message (libcrun_error_t err)
{
  𐤀𐤌 (err == عدم)
    أرجع عدم;
  أرجع err->msg;
}

فراغ
crun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  ثابت صحيح saved_errno = errno;
  libcrun_error_t ref;

  𐤀𐤌 (out == عدم)
    out = stderr;
  𐤀𐤌 (err == عدم || *err == عدم || **err == عدم)
    {
      // Internal error
      أرجع;
    }

  ref = **err;
  𐤀𐤌 (ref->status)
    fprintf (out, "%s: %s\n", ref->msg, strerror (ref->status));
  وإلا
    fprintf (out, "%s\n", ref->msg);

  حرر (ref->msg);
  حرر (ref);
  **err = عدم;

  errno = saved_errno;
}

فراغ
libcrun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  أرجع crun_error_write_warning_and_release (out, err);
}

فراغ
libcrun_error_report_and_release (libcrun_error_t *err)
{
  libcrun_error_t ref;

  𐤀𐤌 (err == عدم || *err == عدم)
    أرجع;

  ref = *err;
  libcrun_error (ref->status, "%s", ref->msg);
  crun_error_release (err);
}

صحيح
crun_error_get_errno (libcrun_error_t *err)
{
  𐤀𐤌 (err == عدم || *err == عدم)
    أرجع 0;
  أرجع (*err)->status;
}

تعريفنوع حرف timestamp_t[64];

ساكن فراغ
get_timestamp (timestamp_t *timestamp, ثابت حرف *suffix)
{
  ثابت size_t buffer_size = حجم (timestamp_t);
  حرف *buffer = (حرف *) timestamp;
  بنية timeval tv;
  size_t written;
  بنية tm now;

  gettimeofday (&tv, عدم);
  gmtime_r (&tv.tv_sec, &now);
  written = strftime (buffer, buffer_size, "%Y-%m-%dT%H:%M:%S", &now);
  snprintf (buffer + written, buffer_size - written, ".%06lldZ%.8s", (long long صحيح) tv.tv_usec, suffix);
}

/* Neither openlog(3) nor the journald handler copy the identifier they are
   given, and the global output handler outlives the caller's strings (e.g.
   the context is freed while errors are still reported through it), so keep
   an owned copy alive for as long as the handler can use it.  */
ساكن حرف *log_id;

ساكن ثابت حرف *
set_log_id (ثابت حرف *id, حرف **old_id)
{
  *old_id = log_id;
  log_id = id ? xstrdup (id) : عدم;
  أرجع log_id;
}

ساكن فراغ *
init_syslog (ثابت حرف *id)
{
  openlog (id, 0, LOG_USER);
  أرجع عدم;
}

تعداد
{
  LOG_TYPE_FILE = 1,
  LOG_TYPE_SYSLOG = 2,
  LOG_TYPE_JOURNALD = 3
};

ساكن صحيح
get_log_type (ثابت حرف *log, ثابت حرف **data)
{
  ثابت حرف *sep = strchr (log, ':');
  𐤀𐤌 (sep == عدم)
    {
      *data = log;
      أرجع LOG_TYPE_FILE;
    }
  *data = sep + 1;

  𐤀𐤌 (has_prefix (log, "syslog:"))
    أرجع LOG_TYPE_SYSLOG;
  𐤀𐤌 (has_prefix (log, "journald:"))
    أرجع LOG_TYPE_JOURNALD;
  𐤀𐤌 (has_prefix (log, "file:"))
    أرجع LOG_TYPE_FILE;

  أرجع -1;
}

صحيح
libcrun_init_logging (crun_output_handler *new_output_handler, فراغ **new_output_handler_arg, ثابت حرف *id,
                      ثابت حرف *log, libcrun_error_t *err)
{
  /* Released only after the new handler is installed.  */
  cleanup_free حرف *old_log_id = عدم;

  𐤀𐤌 (log == عدم)
    {
      *new_output_handler = log_write_to_stderr;
      *new_output_handler_arg = عدم;
    }
  وإلا
    {
      ثابت حرف *arg = عدم;
      صحيح log_type = get_log_type (log, &arg);

      𐤀𐤌 (log_type < 0)
        أرجع crun_make_error (err, errno, "unknown log type `%s`", log);

      بدل (log_type)
        {
        حالة LOG_TYPE_FILE:
          *new_output_handler = log_write_to_stream;
          *new_output_handler_arg = fopen (arg, "a+e");
          𐤀𐤌 (*new_output_handler_arg == عدم)
            أرجع crun_make_error (err, errno, "open log file `%s`", log);
          𐤀𐤌 (output_verbosity >= LIBCRUN_VERBOSITY_WARNING)
            setlinebuf (*new_output_handler_arg);
          اكسر;

        حالة LOG_TYPE_SYSLOG:
          *new_output_handler_arg = init_syslog (set_log_id (arg[0] ? arg : id, &old_log_id));
          *new_output_handler = log_write_to_syslog;
          اكسر;

        حالة LOG_TYPE_JOURNALD:
          *new_output_handler = log_write_to_journald;
          *new_output_handler_arg = (فراغ *) set_log_id (id, &old_log_id);
          اكسر;
        }
    }
  crun_set_output_handler (*new_output_handler, *new_output_handler_arg);
  أرجع 0;
}

فراغ
log_write_to_stream (صحيح errno_, ثابت حرف *msg, صحيح verbosity, فراغ *arg)
{
  timestamp_t timestamp = {
    0,
  };
  FILE *stream = arg;
  صحيح tty = isatty (fileno (stream));
  ثابت حرف *color_begin = "";
  ثابت حرف *color_end = "";

  𐤀𐤌 (tty)
    {
      بدل (verbosity)
        {
        حالة LIBCRUN_VERBOSITY_DEBUG:
          color_begin = "\x1b[1;34m";
          color_end = "\x1b[0m";
          اكسر;
        حالة LIBCRUN_VERBOSITY_WARNING:
          color_begin = "\x1b[1;33m";
          color_end = "\x1b[0m";
          اكسر;
        حالة LIBCRUN_VERBOSITY_ERROR:
          color_begin = "\x1b[1;31m";
          color_end = "\x1b[0m";
          اكسر;
        }

      𐤀𐤌 (log_format == LOG_FORMAT_TEXT)
        get_timestamp (&timestamp, ": ");
    }

  𐤀𐤌 (errno_)
    fprintf (stream, "%s%s%s: %s%s\n", color_begin, timestamp, msg, strerror (errno_), color_end);
  وإلا
    fprintf (stream, "%s%s%s%s\n", color_begin, timestamp, msg, color_end);
}

فراغ
log_write_to_stderr (صحيح errno_, ثابت حرف *msg, صحيح verbosity, فراغ *arg arg_unused)
{
  log_write_to_stream (errno_, msg, verbosity, stderr);
}

فراغ
log_write_to_syslog (صحيح errno_, ثابت حرف *msg, صحيح verbosity, فراغ *arg arg_unused)
{
  صحيح priority = LOG_ERR;
  بدل (verbosity)
    {
    حالة LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      اكسر;
    حالة LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      اكسر;
    حالة LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      اكسر;
    }
  𐤀𐤌 (errno_ == 0)
    syslog (priority, "%s", msg);
  وإلا
    syslog (priority, "%s: %s", msg, strerror (errno_));
}

فراغ
log_write_to_journald (صحيح errno_, ثابت حرف *msg, صحيح verbosity, فراغ *arg arg_unused)
{
  (فراغ) errno_;
  (فراغ) msg;
  (فراغ) verbosity;
#ifdef HAVE_SYSTEMD
  صحيح priority = LOG_ERR;
  بدل (verbosity)
    {
    حالة LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      اكسر;
    حالة LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      اكسر;
    حالة LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      اكسر;
    }
  𐤀𐤌 (errno_ == 0)
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s", msg, "ID=%s", arg, عدم);
  وإلا
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s: %s", msg, strerror (errno_), "ID=%s",
                     arg, عدم);
#endif
}

ساكن crun_output_handler output_handler = log_write_to_stderr;
ساكن فراغ *output_handler_arg = عدم;

فراغ
libcrun_set_verbosity (صحيح verbosity)
{
  output_verbosity = verbosity;
}

صحيح
libcrun_get_verbosity ()
{
  أرجع output_verbosity;
}

فراغ
crun_set_output_handler (crun_output_handler handler, فراغ *arg)
{
  output_handler = handler;
  output_handler_arg = arg;
}

ساكن حرف *
make_json_error (ثابت حرف *msg, صحيح errno_, صحيح verbosity)
{
  ثابت حرف *level;
  بدل (verbosity)
    {
    حالة LIBCRUN_VERBOSITY_DEBUG:
      level = "debug";
      اكسر;
    حالة LIBCRUN_VERBOSITY_WARNING:
      level = "warning";
      اكسر;
    حالة LIBCRUN_VERBOSITY_ERROR:
      level = "error";
      اكسر;
    }
  ثابت حرف *buf = عدم;
  json_gen_ctx *gen = عدم;
  حرف *ret = عدم;
  size_t buf_len;
  timestamp_t timestamp = {
    0,
  };

  𐤀𐤌 (! json_gen_init (&gen, عدم))
    أرجع عدم;

  get_timestamp (&timestamp, "");

  json_gen_map_open (gen);

  json_gen_string (gen, "msg", strlen ("msg"));
  𐤀𐤌 (errno_ == 0)
    json_gen_string (gen, msg, strlen (msg));
  وإلا
    {
      cleanup_free حرف *tmp = عدم;

      xasprintf (&tmp, "%s: %s", msg, strerror (errno_));
      json_gen_string (gen, tmp, strlen (tmp));
    }

  json_gen_string (gen, "level", strlen ("level"));
  json_gen_string (gen, level, strlen (level));

  json_gen_string (gen, "time", strlen ("time"));
  json_gen_string (gen, timestamp, strlen (timestamp));

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);
  𐤀𐤌 (buf)
    ret = strdup (buf);

  json_gen_free (gen);

  أرجع ret;
}

ساكن فراغ
write_log (صحيح errno_, صحيح verbosity, ثابت حرف *msg, va_list args_list)
{
  صحيح ret;
  cleanup_free حرف *output = عدم;
  cleanup_free حرف *json = عدم;

  𐤀𐤌 (verbosity > output_verbosity)
    أرجع;

  ret = vasprintf (&output, msg, args_list);
  𐤀𐤌 (UNLIKELY (ret < 0))
    OOM ();

  𐤀𐤌 (verbosity == LIBCRUN_VERBOSITY_ERROR && output_handler != log_write_to_stderr)
    log_write_to_stderr (errno_, output, LIBCRUN_VERBOSITY_ERROR, عدم);

  بدل (log_format)
    {
    حالة LOG_FORMAT_TEXT:
      output_handler (errno_, output, verbosity, output_handler_arg);
      اكسر;

    حالة LOG_FORMAT_JSON:
      json = make_json_error (output, errno_, verbosity);
      𐤀𐤌 (json)
        output_handler (0, json, verbosity, output_handler_arg);
      وإلا
        output_handler (errno_, output, verbosity, output_handler_arg);
      اكسر;
    }
}

فراغ
libcrun_debug (ثابت حرف *msg, ...)
{
  va_list args_list;
  va_start (args_list, msg);
  write_log (0, LIBCRUN_VERBOSITY_DEBUG, msg, args_list);
  va_end (args_list);
}

فراغ
libcrun_warning (ثابت حرف *msg, ...)
{
  va_list args_list;
  va_start (args_list, msg);
  write_log (0, LIBCRUN_VERBOSITY_WARNING, msg, args_list);
  va_end (args_list);
}

فراغ
libcrun_error (صحيح errno_, ثابت حرف *msg, ...)
{
  va_list args_list;
  va_start (args_list, msg);

  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, msg, args_list);
  va_end (args_list);
}

فراغ __attribute__ ((noreturn))
libcrun_fail_with_error (صحيح errno_, ثابت حرف *msg, ...)
{
  va_list args_list;
  va_start (args_list, msg);
  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, msg, args_list);
  va_end (args_list);
  exit (EXIT_FAILURE);
}

صحيح
libcrun_set_log_format (ثابت حرف *format, libcrun_error_t *err)
{
  𐤀𐤌 (strcmp (format, "text") == 0)
    log_format = LOG_FORMAT_TEXT;
  وإلا 𐤀𐤌 (strcmp (format, "json") == 0)
    log_format = LOG_FORMAT_JSON;
  وإلا
    أرجع crun_make_error (err, 0, "unknown log format type `%s`", format);

  أرجع 0;
}

صحيح
json_gen_error_to_crun_error (صحيح status, libcrun_error_t *err)
{
  بدل (status)
    {
    حالة json_gen_status_ok:
      أرجع 0;

    افتراضي:
      أرجع crun_make_error (err, 0, "generate JSON document: error %d", status);
    }
}
