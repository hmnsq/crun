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
#شامل <config.h>
#شامل <stdarg.h>
#شامل <unistd.h>
#شامل <string.h>
#شامل <time.h>
#شامل <sys/time.h>
#شامل <stdio.h>
#شامل "utils.h"

#شامل <json-c/json.h>
#شامل <ocispec/json_common.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-journal.h>
#endif

شمارش
{
  LOG_FORMAT_TEXT = 0,
  LOG_FORMAT_JSON,
};

ایستا صحیح log_format;
ایستا صحیح output_verbosity = LIBCRUN_VERBOSITY_ERROR;

صحیح
libcrun_make_error (libcrun_error_t *err, صحیح status, ثابت نویسه *پیام, ...)
{
  va_list args_list;
  libcrun_error_t ptr;

  va_start (args_list, پیام);
  *err = xmalloc (اندازه (ساختار libcrun_error_s));
  ptr = *err;
  ptr->status = status;
  اگر (vasprintf (&(ptr->پیام), پیام, args_list) < 0)
    OOM ();
  va_end (args_list);

  بازگشت -status - 1;
}

صحیح
crun_error_wrap (libcrun_error_t *err, ثابت نویسه *fmt, ...)
{
  cleanup_free نویسه *پیام = پوچ;
  cleanup_free نویسه *tmp = پوچ;
  va_list args_list;
  نویسه *swap;
  صحیح ret;

  اگر (err == پوچ || *err == پوچ)
    {
      // Internal error
      بازگشت 0;
    }

  ret = -(*err)->status - 1;

  va_start (args_list, fmt);

  اگر (vasprintf (&پیام, fmt, args_list) < 0)
    {
      va_end (args_list);
      پیام = پوچ;
      بازگشت ret;
    }
  va_end (args_list);

  xasprintf (&tmp, "%s: %s", پیام, (*err)->پیام);
  swap = tmp;
  tmp = (*err)->پیام;
  (*err)->پیام = swap;

  بازگشت ret;
}

صحیح
crun_error_release (libcrun_error_t *err)
{
  ثابت صحیح saved_errno = errno;
  libcrun_error_t ptr;

  اگر (err == پوچ)
    بازگشت 0;

  ptr = *err;
  اگر (ptr == پوچ)
    بازگشت 0;

  آزاد (ptr->پیام);
  آزاد (ptr);
  *err = پوچ;

  errno = saved_errno;
  بازگشت 0;
}

صحیح
libcrun_error_release (libcrun_error_t *err)
{
  بازگشت crun_error_release (err);
}

صحیح
libcrun_error_get_status (libcrun_error_t err)
{
  اگر (err == پوچ)
    بازگشت 0;
  بازگشت err->status;
}

ثابت نویسه *
libcrun_error_get_message (libcrun_error_t err)
{
  اگر (err == پوچ)
    بازگشت پوچ;
  بازگشت err->پیام;
}

تهی
crun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  ثابت صحیح saved_errno = errno;
  libcrun_error_t ref;

  اگر (out == پوچ)
    out = stderr;
  اگر (err == پوچ || *err == پوچ || **err == پوچ)
    {
      // Internal error
      بازگشت;
    }

  ref = **err;
  اگر (ref->status)
    fprintf (out, "%s: %s\n", ref->پیام, strerror (ref->status));
  وگرنه
    fprintf (out, "%s\n", ref->پیام);

  آزاد (ref->پیام);
  آزاد (ref);
  **err = پوچ;

  errno = saved_errno;
}

تهی
libcrun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  بازگشت crun_error_write_warning_and_release (out, err);
}

تهی
libcrun_error_report_and_release (libcrun_error_t *err)
{
  libcrun_error_t ref;

  اگر (err == پوچ || *err == پوچ)
    بازگشت;

  ref = *err;
  libcrun_error (ref->status, "%s", ref->پیام);
  crun_error_release (err);
}

صحیح
crun_error_get_errno (libcrun_error_t *err)
{
  اگر (err == پوچ || *err == پوچ)
    بازگشت 0;
  بازگشت (*err)->status;
}

تعریفنوع نویسه timestamp_t[64];

ایستا تهی
get_timestamp (timestamp_t *timestamp, ثابت نویسه *suffix)
{
  ثابت size_t buffer_size = اندازه (timestamp_t);
  نویسه *buffer = (نویسه *) timestamp;
  ساختار timeval tv;
  size_t written;
  ساختار tm now;

  gettimeofday (&tv, پوچ);
  gmtime_r (&tv.tv_sec, &now);
  written = strftime (buffer, buffer_size, "%Y-%m-%dT%H:%M:%S", &now);
  snprintf (buffer + written, buffer_size - written, ".%06lldZ%.8s", (long long صحیح) tv.tv_usec, suffix);
}

/* Neither openlog(3) nor the journald handler copy the identifier they are
   given, and the global output handler outlives the caller's strings (e.g.
   the context is freed while errors are still reported through it), so keep
   an owned copy alive for as long as the handler can use it.  */
ایستا نویسه *log_id;

ایستا ثابت نویسه *
set_log_id (ثابت نویسه *id, نویسه **old_id)
{
  *old_id = log_id;
  log_id = id ? xstrdup (id) : پوچ;
  بازگشت log_id;
}

ایستا تهی *
init_syslog (ثابت نویسه *id)
{
  openlog (id, 0, LOG_USER);
  بازگشت پوچ;
}

شمارش
{
  LOG_TYPE_FILE = 1,
  LOG_TYPE_SYSLOG = 2,
  LOG_TYPE_JOURNALD = 3
};

ایستا صحیح
get_log_type (ثابت نویسه *log, ثابت نویسه **data)
{
  ثابت نویسه *sep = strchr (log, ':');
  اگر (sep == پوچ)
    {
      *data = log;
      بازگشت LOG_TYPE_FILE;
    }
  *data = sep + 1;

  اگر (has_prefix (log, "syslog:"))
    بازگشت LOG_TYPE_SYSLOG;
  اگر (has_prefix (log, "journald:"))
    بازگشت LOG_TYPE_JOURNALD;
  اگر (has_prefix (log, "file:"))
    بازگشت LOG_TYPE_FILE;

  بازگشت -1;
}

صحیح
libcrun_init_logging (crun_output_handler *new_output_handler, تهی **new_output_handler_arg, ثابت نویسه *id,
                      ثابت نویسه *log, libcrun_error_t *err)
{
  /* Released only after the new handler is installed.  */
  cleanup_free نویسه *old_log_id = پوچ;

  اگر (log == پوچ)
    {
      *new_output_handler = log_write_to_stderr;
      *new_output_handler_arg = پوچ;
    }
  وگرنه
    {
      ثابت نویسه *arg = پوچ;
      صحیح log_type = get_log_type (log, &arg);

      اگر (log_type < 0)
        بازگشت crun_make_error (err, errno, "unknown log type `%s`", log);

      انتخاب (log_type)
        {
        حالت LOG_TYPE_FILE:
          *new_output_handler = log_write_to_stream;
          *new_output_handler_arg = fopen (arg, "a+e");
          اگر (*new_output_handler_arg == پوچ)
            بازگشت crun_make_error (err, errno, "open log file `%s`", log);
          اگر (output_verbosity >= LIBCRUN_VERBOSITY_WARNING)
            setlinebuf (*new_output_handler_arg);
          بشکن;

        حالت LOG_TYPE_SYSLOG:
          *new_output_handler_arg = init_syslog (set_log_id (arg[0] ? arg : id, &old_log_id));
          *new_output_handler = log_write_to_syslog;
          بشکن;

        حالت LOG_TYPE_JOURNALD:
          *new_output_handler = log_write_to_journald;
          *new_output_handler_arg = (تهی *) set_log_id (id, &old_log_id);
          بشکن;
        }
    }
  crun_set_output_handler (*new_output_handler, *new_output_handler_arg);
  بازگشت 0;
}

تهی
log_write_to_stream (صحیح errno_, ثابت نویسه *پیام, صحیح verbosity, تهی *arg)
{
  timestamp_t timestamp = {
    0,
  };
  FILE *stream = arg;
  صحیح tty = isatty (fileno (stream));
  ثابت نویسه *color_begin = "";
  ثابت نویسه *color_end = "";

  اگر (tty)
    {
      انتخاب (verbosity)
        {
        حالت LIBCRUN_VERBOSITY_DEBUG:
          color_begin = "\x1b[1;34m";
          color_end = "\x1b[0m";
          بشکن;
        حالت LIBCRUN_VERBOSITY_WARNING:
          color_begin = "\x1b[1;33m";
          color_end = "\x1b[0m";
          بشکن;
        حالت LIBCRUN_VERBOSITY_ERROR:
          color_begin = "\x1b[1;31m";
          color_end = "\x1b[0m";
          بشکن;
        }

      اگر (log_format == LOG_FORMAT_TEXT)
        get_timestamp (&timestamp, ": ");
    }

  اگر (errno_)
    fprintf (stream, "%s%s%s: %s%s\n", color_begin, timestamp, پیام, strerror (errno_), color_end);
  وگرنه
    fprintf (stream, "%s%s%s%s\n", color_begin, timestamp, پیام, color_end);
}

تهی
log_write_to_stderr (صحیح errno_, ثابت نویسه *پیام, صحیح verbosity, تهی *arg arg_unused)
{
  log_write_to_stream (errno_, پیام, verbosity, stderr);
}

تهی
log_write_to_syslog (صحیح errno_, ثابت نویسه *پیام, صحیح verbosity, تهی *arg arg_unused)
{
  صحیح priority = LOG_ERR;
  انتخاب (verbosity)
    {
    حالت LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      بشکن;
    حالت LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      بشکن;
    حالت LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      بشکن;
    }
  اگر (errno_ == 0)
    syslog (priority, "%s", پیام);
  وگرنه
    syslog (priority, "%s: %s", پیام, strerror (errno_));
}

تهی
log_write_to_journald (صحیح errno_, ثابت نویسه *پیام, صحیح verbosity, تهی *arg arg_unused)
{
  (تهی) errno_;
  (تهی) پیام;
  (تهی) verbosity;
#ifdef HAVE_SYSTEMD
  صحیح priority = LOG_ERR;
  انتخاب (verbosity)
    {
    حالت LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      بشکن;
    حالت LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      بشکن;
    حالت LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      بشکن;
    }
  اگر (errno_ == 0)
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s", پیام, "ID=%s", arg, پوچ);
  وگرنه
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s: %s", پیام, strerror (errno_), "ID=%s",
                     arg, پوچ);
#endif
}

ایستا crun_output_handler output_handler = log_write_to_stderr;
ایستا تهی *output_handler_arg = پوچ;

تهی
libcrun_set_verbosity (صحیح verbosity)
{
  output_verbosity = verbosity;
}

صحیح
libcrun_get_verbosity ()
{
  بازگشت output_verbosity;
}

تهی
crun_set_output_handler (crun_output_handler handler, تهی *arg)
{
  output_handler = handler;
  output_handler_arg = arg;
}

ایستا نویسه *
make_json_error (ثابت نویسه *پیام, صحیح errno_, صحیح verbosity)
{
  ثابت نویسه *level;
  انتخاب (verbosity)
    {
    حالت LIBCRUN_VERBOSITY_DEBUG:
      level = "debug";
      بشکن;
    حالت LIBCRUN_VERBOSITY_WARNING:
      level = "warning";
      بشکن;
    حالت LIBCRUN_VERBOSITY_ERROR:
      level = "error";
      بشکن;
    }
  ثابت نویسه *buf = پوچ;
  json_gen_ctx *gen = پوچ;
  نویسه *ret = پوچ;
  size_t buf_len;
  timestamp_t timestamp = {
    0,
  };

  اگر (! json_gen_init (&gen, پوچ))
    بازگشت پوچ;

  get_timestamp (&timestamp, "");

  json_gen_map_open (gen);

  json_gen_string (gen, "msg", strlen ("msg"));
  اگر (errno_ == 0)
    json_gen_string (gen, پیام, strlen (پیام));
  وگرنه
    {
      cleanup_free نویسه *tmp = پوچ;

      xasprintf (&tmp, "%s: %s", پیام, strerror (errno_));
      json_gen_string (gen, tmp, strlen (tmp));
    }

  json_gen_string (gen, "level", strlen ("level"));
  json_gen_string (gen, level, strlen (level));

  json_gen_string (gen, "time", strlen ("time"));
  json_gen_string (gen, timestamp, strlen (timestamp));

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);
  اگر (buf)
    ret = strdup (buf);

  json_gen_free (gen);

  بازگشت ret;
}

ایستا تهی
write_log (صحیح errno_, صحیح verbosity, ثابت نویسه *پیام, va_list args_list)
{
  صحیح ret;
  cleanup_free نویسه *output = پوچ;
  cleanup_free نویسه *json = پوچ;

  اگر (verbosity > output_verbosity)
    بازگشت;

  ret = vasprintf (&output, پیام, args_list);
  اگر (UNLIKELY (ret < 0))
    OOM ();

  اگر (verbosity == LIBCRUN_VERBOSITY_ERROR && output_handler != log_write_to_stderr)
    log_write_to_stderr (errno_, output, LIBCRUN_VERBOSITY_ERROR, پوچ);

  انتخاب (log_format)
    {
    حالت LOG_FORMAT_TEXT:
      output_handler (errno_, output, verbosity, output_handler_arg);
      بشکن;

    حالت LOG_FORMAT_JSON:
      json = make_json_error (output, errno_, verbosity);
      اگر (json)
        output_handler (0, json, verbosity, output_handler_arg);
      وگرنه
        output_handler (errno_, output, verbosity, output_handler_arg);
      بشکن;
    }
}

تهی
libcrun_debug (ثابت نویسه *پیام, ...)
{
  va_list args_list;
  va_start (args_list, پیام);
  write_log (0, LIBCRUN_VERBOSITY_DEBUG, پیام, args_list);
  va_end (args_list);
}

تهی
libcrun_warning (ثابت نویسه *پیام, ...)
{
  va_list args_list;
  va_start (args_list, پیام);
  write_log (0, LIBCRUN_VERBOSITY_WARNING, پیام, args_list);
  va_end (args_list);
}

تهی
libcrun_error (صحیح errno_, ثابت نویسه *پیام, ...)
{
  va_list args_list;
  va_start (args_list, پیام);

  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, پیام, args_list);
  va_end (args_list);
}

تهی __attribute__ ((noreturn))
libcrun_fail_with_error (صحیح errno_, ثابت نویسه *پیام, ...)
{
  va_list args_list;
  va_start (args_list, پیام);
  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, پیام, args_list);
  va_end (args_list);
  exit (EXIT_FAILURE);
}

صحیح
libcrun_set_log_format (ثابت نویسه *format, libcrun_error_t *err)
{
  اگر (strcmp (format, "text") == 0)
    log_format = LOG_FORMAT_TEXT;
  وگرنه اگر (strcmp (format, "json") == 0)
    log_format = LOG_FORMAT_JSON;
  وگرنه
    بازگشت crun_make_error (err, 0, "unknown log format type `%s`", format);

  بازگشت 0;
}

صحیح
json_gen_error_to_crun_error (صحیح status, libcrun_error_t *err)
{
  انتخاب (status)
    {
    حالت json_gen_status_ok:
      بازگشت 0;

    پیشفرض:
      بازگشت crun_make_error (err, 0, "generate JSON document: error %d", status);
    }
}
