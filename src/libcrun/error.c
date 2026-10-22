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
#अन्तर्भाव <config.h>
#अन्तर्भाव <stdarg.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <sys/time.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव "utils.h"

#अन्तर्भाव <json-c/json.h>
#अन्तर्भाव <ocispec/json_common.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-journal.h>
#endif

गणना
{
  LOG_FORMAT_TEXT = 0,
  LOG_FORMAT_JSON,
};

स्थिर पूर्णाङ्क log_format;
स्थिर पूर्णाङ्क output_verbosity = LIBCRUN_VERBOSITY_ERROR;

पूर्णाङ्क
libcrun_make_error (libcrun_error_t *err, पूर्णाङ्क status, ध्रुव वर्ण *सन्देशः, ...)
{
  va_list args_list;
  libcrun_error_t ptr;

  va_start (args_list, सन्देशः);
  *err = xmalloc (परिमाण (संरचना libcrun_error_s));
  ptr = *err;
  ptr->status = status;
  जइ (vasprintf (&(ptr->सन्देशः), सन्देशः, args_list) < 0)
    OOM ();
  va_end (args_list);

  प्रत्यागम -status - 1;
}

पूर्णाङ्क
crun_error_wrap (libcrun_error_t *err, ध्रुव वर्ण *fmt, ...)
{
  cleanup_free वर्ण *सन्देशः = रिक्त;
  cleanup_free वर्ण *tmp = रिक्त;
  va_list args_list;
  वर्ण *swap;
  पूर्णाङ्क ret;

  जइ (err == रिक्त || *err == रिक्त)
    {
      // Internal error
      प्रत्यागम 0;
    }

  ret = -(*err)->status - 1;

  va_start (args_list, fmt);

  जइ (vasprintf (&सन्देशः, fmt, args_list) < 0)
    {
      va_end (args_list);
      सन्देशः = रिक्त;
      प्रत्यागम ret;
    }
  va_end (args_list);

  xasprintf (&tmp, "%s: %s", सन्देशः, (*err)->सन्देशः);
  swap = tmp;
  tmp = (*err)->सन्देशः;
  (*err)->सन्देशः = swap;

  प्रत्यागम ret;
}

पूर्णाङ्क
crun_error_release (libcrun_error_t *err)
{
  ध्रुव पूर्णाङ्क saved_errno = errno;
  libcrun_error_t ptr;

  जइ (err == रिक्त)
    प्रत्यागम 0;

  ptr = *err;
  जइ (ptr == रिक्त)
    प्रत्यागम 0;

  मुक्त (ptr->सन्देशः);
  मुक्त (ptr);
  *err = रिक्त;

  errno = saved_errno;
  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_error_release (libcrun_error_t *err)
{
  प्रत्यागम crun_error_release (err);
}

पूर्णाङ्क
libcrun_error_get_status (libcrun_error_t err)
{
  जइ (err == रिक्त)
    प्रत्यागम 0;
  प्रत्यागम err->status;
}

ध्रुव वर्ण *
libcrun_error_get_message (libcrun_error_t err)
{
  जइ (err == रिक्त)
    प्रत्यागम रिक्त;
  प्रत्यागम err->सन्देशः;
}

शून्य
crun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  ध्रुव पूर्णाङ्क saved_errno = errno;
  libcrun_error_t ref;

  जइ (out == रिक्त)
    out = stderr;
  जइ (err == रिक्त || *err == रिक्त || **err == रिक्त)
    {
      // Internal error
      प्रत्यागम;
    }

  ref = **err;
  जइ (ref->status)
    fprintf (out, "%s: %s\n", ref->सन्देशः, strerror (ref->status));
  अण्णहा
    fprintf (out, "%s\n", ref->सन्देशः);

  मुक्त (ref->सन्देशः);
  मुक्त (ref);
  **err = रिक्त;

  errno = saved_errno;
}

शून्य
libcrun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  प्रत्यागम crun_error_write_warning_and_release (out, err);
}

शून्य
libcrun_error_report_and_release (libcrun_error_t *err)
{
  libcrun_error_t ref;

  जइ (err == रिक्त || *err == रिक्त)
    प्रत्यागम;

  ref = *err;
  libcrun_error (ref->status, "%s", ref->सन्देशः);
  crun_error_release (err);
}

पूर्णाङ्क
crun_error_get_errno (libcrun_error_t *err)
{
  जइ (err == रिक्त || *err == रिक्त)
    प्रत्यागम 0;
  प्रत्यागम (*err)->status;
}

प्रकारनिर्देश वर्ण timestamp_t[64];

स्थिर शून्य
get_timestamp (timestamp_t *timestamp, ध्रुव वर्ण *suffix)
{
  ध्रुव size_t buffer_size = परिमाण (timestamp_t);
  वर्ण *buffer = (वर्ण *) timestamp;
  संरचना timeval tv;
  size_t written;
  संरचना tm now;

  gettimeofday (&tv, रिक्त);
  gmtime_r (&tv.tv_sec, &now);
  written = strftime (buffer, buffer_size, "%Y-%m-%dT%H:%M:%S", &now);
  snprintf (buffer + written, buffer_size - written, ".%06lldZ%.8s", (long long पूर्णाङ्क) tv.tv_usec, suffix);
}

/* Neither openlog(3) nor the journald handler copy the identifier they are
   given, and the global output handler outlives the caller's strings (e.g.
   the context is freed while errors are still reported through it), so keep
   an owned copy alive for as long as the handler can use it.  */
स्थिर वर्ण *log_id;

स्थिर ध्रुव वर्ण *
set_log_id (ध्रुव वर्ण *id, वर्ण **old_id)
{
  *old_id = log_id;
  log_id = id ? xstrdup (id) : रिक्त;
  प्रत्यागम log_id;
}

स्थिर शून्य *
init_syslog (ध्रुव वर्ण *id)
{
  openlog (id, 0, LOG_USER);
  प्रत्यागम रिक्त;
}

गणना
{
  LOG_TYPE_FILE = 1,
  LOG_TYPE_SYSLOG = 2,
  LOG_TYPE_JOURNALD = 3
};

स्थिर पूर्णाङ्क
get_log_type (ध्रुव वर्ण *log, ध्रुव वर्ण **data)
{
  ध्रुव वर्ण *sep = strchr (log, ':');
  जइ (sep == रिक्त)
    {
      *data = log;
      प्रत्यागम LOG_TYPE_FILE;
    }
  *data = sep + 1;

  जइ (has_prefix (log, "syslog:"))
    प्रत्यागम LOG_TYPE_SYSLOG;
  जइ (has_prefix (log, "journald:"))
    प्रत्यागम LOG_TYPE_JOURNALD;
  जइ (has_prefix (log, "file:"))
    प्रत्यागम LOG_TYPE_FILE;

  प्रत्यागम -1;
}

पूर्णाङ्क
libcrun_init_logging (crun_output_handler *new_output_handler, शून्य **new_output_handler_arg, ध्रुव वर्ण *id,
                      ध्रुव वर्ण *log, libcrun_error_t *err)
{
  /* Released only after the new handler is installed.  */
  cleanup_free वर्ण *old_log_id = रिक्त;

  जइ (log == रिक्त)
    {
      *new_output_handler = log_write_to_stderr;
      *new_output_handler_arg = रिक्त;
    }
  अण्णहा
    {
      ध्रुव वर्ण *arg = रिक्त;
      पूर्णाङ्क log_type = get_log_type (log, &arg);

      जइ (log_type < 0)
        प्रत्यागम crun_make_error (err, errno, "unknown log type `%s`", log);

      विकल्प (log_type)
        {
        पक्ष LOG_TYPE_FILE:
          *new_output_handler = log_write_to_stream;
          *new_output_handler_arg = fopen (arg, "a+e");
          जइ (*new_output_handler_arg == रिक्त)
            प्रत्यागम crun_make_error (err, errno, "open log file `%s`", log);
          जइ (output_verbosity >= LIBCRUN_VERBOSITY_WARNING)
            setlinebuf (*new_output_handler_arg);
          भङ्ग;

        पक्ष LOG_TYPE_SYSLOG:
          *new_output_handler_arg = init_syslog (set_log_id (arg[0] ? arg : id, &old_log_id));
          *new_output_handler = log_write_to_syslog;
          भङ्ग;

        पक्ष LOG_TYPE_JOURNALD:
          *new_output_handler = log_write_to_journald;
          *new_output_handler_arg = (शून्य *) set_log_id (id, &old_log_id);
          भङ्ग;
        }
    }
  crun_set_output_handler (*new_output_handler, *new_output_handler_arg);
  प्रत्यागम 0;
}

शून्य
log_write_to_stream (पूर्णाङ्क errno_, ध्रुव वर्ण *सन्देशः, पूर्णाङ्क verbosity, शून्य *arg)
{
  timestamp_t timestamp = {
    0,
  };
  FILE *stream = arg;
  पूर्णाङ्क tty = isatty (fileno (stream));
  ध्रुव वर्ण *color_begin = "";
  ध्रुव वर्ण *color_end = "";

  जइ (tty)
    {
      विकल्प (verbosity)
        {
        पक्ष LIBCRUN_VERBOSITY_DEBUG:
          color_begin = "\x1b[1;34m";
          color_end = "\x1b[0m";
          भङ्ग;
        पक्ष LIBCRUN_VERBOSITY_WARNING:
          color_begin = "\x1b[1;33m";
          color_end = "\x1b[0m";
          भङ्ग;
        पक्ष LIBCRUN_VERBOSITY_ERROR:
          color_begin = "\x1b[1;31m";
          color_end = "\x1b[0m";
          भङ्ग;
        }

      जइ (log_format == LOG_FORMAT_TEXT)
        get_timestamp (&timestamp, ": ");
    }

  जइ (errno_)
    fprintf (stream, "%s%s%s: %s%s\n", color_begin, timestamp, सन्देशः, strerror (errno_), color_end);
  अण्णहा
    fprintf (stream, "%s%s%s%s\n", color_begin, timestamp, सन्देशः, color_end);
}

शून्य
log_write_to_stderr (पूर्णाङ्क errno_, ध्रुव वर्ण *सन्देशः, पूर्णाङ्क verbosity, शून्य *arg arg_unused)
{
  log_write_to_stream (errno_, सन्देशः, verbosity, stderr);
}

शून्य
log_write_to_syslog (पूर्णाङ्क errno_, ध्रुव वर्ण *सन्देशः, पूर्णाङ्क verbosity, शून्य *arg arg_unused)
{
  पूर्णाङ्क priority = LOG_ERR;
  विकल्प (verbosity)
    {
    पक्ष LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      भङ्ग;
    पक्ष LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      भङ्ग;
    पक्ष LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      भङ्ग;
    }
  जइ (errno_ == 0)
    syslog (priority, "%s", सन्देशः);
  अण्णहा
    syslog (priority, "%s: %s", सन्देशः, strerror (errno_));
}

शून्य
log_write_to_journald (पूर्णाङ्क errno_, ध्रुव वर्ण *सन्देशः, पूर्णाङ्क verbosity, शून्य *arg arg_unused)
{
  (शून्य) errno_;
  (शून्य) सन्देशः;
  (शून्य) verbosity;
#ifdef HAVE_SYSTEMD
  पूर्णाङ्क priority = LOG_ERR;
  विकल्प (verbosity)
    {
    पक्ष LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      भङ्ग;
    पक्ष LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      भङ्ग;
    पक्ष LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      भङ्ग;
    }
  जइ (errno_ == 0)
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s", सन्देशः, "ID=%s", arg, रिक्त);
  अण्णहा
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s: %s", सन्देशः, strerror (errno_), "ID=%s",
                     arg, रिक्त);
#endif
}

स्थिर crun_output_handler output_handler = log_write_to_stderr;
स्थिर शून्य *output_handler_arg = रिक्त;

शून्य
libcrun_set_verbosity (पूर्णाङ्क verbosity)
{
  output_verbosity = verbosity;
}

पूर्णाङ्क
libcrun_get_verbosity ()
{
  प्रत्यागम output_verbosity;
}

शून्य
crun_set_output_handler (crun_output_handler handler, शून्य *arg)
{
  output_handler = handler;
  output_handler_arg = arg;
}

स्थिर वर्ण *
make_json_error (ध्रुव वर्ण *सन्देशः, पूर्णाङ्क errno_, पूर्णाङ्क verbosity)
{
  ध्रुव वर्ण *level;
  विकल्प (verbosity)
    {
    पक्ष LIBCRUN_VERBOSITY_DEBUG:
      level = "debug";
      भङ्ग;
    पक्ष LIBCRUN_VERBOSITY_WARNING:
      level = "warning";
      भङ्ग;
    पक्ष LIBCRUN_VERBOSITY_ERROR:
      level = "error";
      भङ्ग;
    }
  ध्रुव वर्ण *buf = रिक्त;
  json_gen_ctx *gen = रिक्त;
  वर्ण *ret = रिक्त;
  size_t buf_len;
  timestamp_t timestamp = {
    0,
  };

  जइ (! json_gen_init (&gen, रिक्त))
    प्रत्यागम रिक्त;

  get_timestamp (&timestamp, "");

  json_gen_map_open (gen);

  json_gen_string (gen, "msg", strlen ("msg"));
  जइ (errno_ == 0)
    json_gen_string (gen, सन्देशः, strlen (सन्देशः));
  अण्णहा
    {
      cleanup_free वर्ण *tmp = रिक्त;

      xasprintf (&tmp, "%s: %s", सन्देशः, strerror (errno_));
      json_gen_string (gen, tmp, strlen (tmp));
    }

  json_gen_string (gen, "level", strlen ("level"));
  json_gen_string (gen, level, strlen (level));

  json_gen_string (gen, "time", strlen ("time"));
  json_gen_string (gen, timestamp, strlen (timestamp));

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);
  जइ (buf)
    ret = strdup (buf);

  json_gen_free (gen);

  प्रत्यागम ret;
}

स्थिर शून्य
write_log (पूर्णाङ्क errno_, पूर्णाङ्क verbosity, ध्रुव वर्ण *सन्देशः, va_list args_list)
{
  पूर्णाङ्क ret;
  cleanup_free वर्ण *output = रिक्त;
  cleanup_free वर्ण *json = रिक्त;

  जइ (verbosity > output_verbosity)
    प्रत्यागम;

  ret = vasprintf (&output, सन्देशः, args_list);
  जइ (UNLIKELY (ret < 0))
    OOM ();

  जइ (verbosity == LIBCRUN_VERBOSITY_ERROR && output_handler != log_write_to_stderr)
    log_write_to_stderr (errno_, output, LIBCRUN_VERBOSITY_ERROR, रिक्त);

  विकल्प (log_format)
    {
    पक्ष LOG_FORMAT_TEXT:
      output_handler (errno_, output, verbosity, output_handler_arg);
      भङ्ग;

    पक्ष LOG_FORMAT_JSON:
      json = make_json_error (output, errno_, verbosity);
      जइ (json)
        output_handler (0, json, verbosity, output_handler_arg);
      अण्णहा
        output_handler (errno_, output, verbosity, output_handler_arg);
      भङ्ग;
    }
}

शून्य
libcrun_debug (ध्रुव वर्ण *सन्देशः, ...)
{
  va_list args_list;
  va_start (args_list, सन्देशः);
  write_log (0, LIBCRUN_VERBOSITY_DEBUG, सन्देशः, args_list);
  va_end (args_list);
}

शून्य
libcrun_warning (ध्रुव वर्ण *सन्देशः, ...)
{
  va_list args_list;
  va_start (args_list, सन्देशः);
  write_log (0, LIBCRUN_VERBOSITY_WARNING, सन्देशः, args_list);
  va_end (args_list);
}

शून्य
libcrun_error (पूर्णाङ्क errno_, ध्रुव वर्ण *सन्देशः, ...)
{
  va_list args_list;
  va_start (args_list, सन्देशः);

  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, सन्देशः, args_list);
  va_end (args_list);
}

शून्य __attribute__ ((noreturn))
libcrun_fail_with_error (पूर्णाङ्क errno_, ध्रुव वर्ण *सन्देशः, ...)
{
  va_list args_list;
  va_start (args_list, सन्देशः);
  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, सन्देशः, args_list);
  va_end (args_list);
  exit (EXIT_FAILURE);
}

पूर्णाङ्क
libcrun_set_log_format (ध्रुव वर्ण *format, libcrun_error_t *err)
{
  जइ (strcmp (format, "text") == 0)
    log_format = LOG_FORMAT_TEXT;
  अण्णहा जइ (strcmp (format, "json") == 0)
    log_format = LOG_FORMAT_JSON;
  अण्णहा
    प्रत्यागम crun_make_error (err, 0, "unknown log format type `%s`", format);

  प्रत्यागम 0;
}

पूर्णाङ्क
json_gen_error_to_crun_error (पूर्णाङ्क status, libcrun_error_t *err)
{
  विकल्प (status)
    {
    पक्ष json_gen_status_ok:
      प्रत्यागम 0;

    स्वभाव:
      प्रत्यागम crun_make_error (err, 0, "generate JSON document: error %d", status);
    }
}
