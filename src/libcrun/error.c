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
#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdarg.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <time.h>
#অন্তর্ভাব <sys/time.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব "utils.h"

#অন্তর্ভাব <json-c/json.h>
#অন্তর্ভাব <ocispec/json_common.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-journal.h>
#endif

গণনা
{
  LOG_FORMAT_TEXT = 0,
  LOG_FORMAT_JSON,
};

স্থির পূর্ণসংখ্যা log_format;
স্থির পূর্ণসংখ্যা output_verbosity = LIBCRUN_VERBOSITY_ERROR;

পূর্ণসংখ্যা
libcrun_make_error (libcrun_error_t *err, পূর্ণসংখ্যা status, ধ্রুবক অক্ষর *বার্তা, ...)
{
  va_list args_list;
  libcrun_error_t ptr;

  va_start (args_list, বার্তা);
  *err = xmalloc (আকার (কাঠামো libcrun_error_s));
  ptr = *err;
  ptr->status = status;
  যদি (vasprintf (&(ptr->বার্তা), বার্তা, args_list) < 0)
    OOM ();
  va_end (args_list);

  ফেরত -status - 1;
}

পূর্ণসংখ্যা
crun_error_wrap (libcrun_error_t *err, ধ্রুবক অক্ষর *fmt, ...)
{
  cleanup_free অক্ষর *বার্তা = ফাঁকা;
  cleanup_free অক্ষর *tmp = ফাঁকা;
  va_list args_list;
  অক্ষর *swap;
  পূর্ণসংখ্যা ret;

  যদি (err == ফাঁকা || *err == ফাঁকা)
    {
      // Internal error
      ফেরত 0;
    }

  ret = -(*err)->status - 1;

  va_start (args_list, fmt);

  যদি (vasprintf (&বার্তা, fmt, args_list) < 0)
    {
      va_end (args_list);
      বার্তা = ফাঁকা;
      ফেরত ret;
    }
  va_end (args_list);

  xasprintf (&tmp, "%s: %s", বার্তা, (*err)->বার্তা);
  swap = tmp;
  tmp = (*err)->বার্তা;
  (*err)->বার্তা = swap;

  ফেরত ret;
}

পূর্ণসংখ্যা
crun_error_release (libcrun_error_t *err)
{
  ধ্রুবক পূর্ণসংখ্যা saved_errno = errno;
  libcrun_error_t ptr;

  যদি (err == ফাঁকা)
    ফেরত 0;

  ptr = *err;
  যদি (ptr == ফাঁকা)
    ফেরত 0;

  মুক্ত (ptr->বার্তা);
  মুক্ত (ptr);
  *err = ফাঁকা;

  errno = saved_errno;
  ফেরত 0;
}

পূর্ণসংখ্যা
libcrun_error_release (libcrun_error_t *err)
{
  ফেরত crun_error_release (err);
}

পূর্ণসংখ্যা
libcrun_error_get_status (libcrun_error_t err)
{
  যদি (err == ফাঁকা)
    ফেরত 0;
  ফেরত err->status;
}

ধ্রুবক অক্ষর *
libcrun_error_get_message (libcrun_error_t err)
{
  যদি (err == ফাঁকা)
    ফেরত ফাঁকা;
  ফেরত err->বার্তা;
}

শূন্য
crun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  ধ্রুবক পূর্ণসংখ্যা saved_errno = errno;
  libcrun_error_t ref;

  যদি (out == ফাঁকা)
    out = stderr;
  যদি (err == ফাঁকা || *err == ফাঁকা || **err == ফাঁকা)
    {
      // Internal error
      ফেরত;
    }

  ref = **err;
  যদি (ref->status)
    fprintf (out, "%s: %s\n", ref->বার্তা, strerror (ref->status));
  অন্যথা
    fprintf (out, "%s\n", ref->বার্তা);

  মুক্ত (ref->বার্তা);
  মুক্ত (ref);
  **err = ফাঁকা;

  errno = saved_errno;
}

শূন্য
libcrun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  ফেরত crun_error_write_warning_and_release (out, err);
}

শূন্য
libcrun_error_report_and_release (libcrun_error_t *err)
{
  libcrun_error_t ref;

  যদি (err == ফাঁকা || *err == ফাঁকা)
    ফেরত;

  ref = *err;
  libcrun_error (ref->status, "%s", ref->বার্তা);
  crun_error_release (err);
}

পূর্ণসংখ্যা
crun_error_get_errno (libcrun_error_t *err)
{
  যদি (err == ফাঁকা || *err == ফাঁকা)
    ফেরত 0;
  ফেরত (*err)->status;
}

প্রকারলেখ অক্ষর timestamp_t[64];

স্থির শূন্য
get_timestamp (timestamp_t *timestamp, ধ্রুবক অক্ষর *suffix)
{
  ধ্রুবক size_t বাফার_আকার = আকার (timestamp_t);
  অক্ষর *বাফার = (অক্ষর *) timestamp;
  কাঠামো timeval tv;
  size_t written;
  কাঠামো tm now;

  gettimeofday (&tv, ফাঁকা);
  gmtime_r (&tv.tv_sec, &now);
  written = strftime (বাফার, বাফার_আকার, "%Y-%m-%dT%H:%M:%S", &now);
  snprintf (বাফার + written, বাফার_আকার - written, ".%06lldZ%.8s", (long long পূর্ণসংখ্যা) tv.tv_usec, suffix);
}

/* Neither openlog(3) nor the journald handler copy the identifier they are
   given, and the global output handler outlives the caller's strings (e.g.
   the context is freed while errors are still reported through it), so keep
   an owned copy alive for as long as the handler can use it.  */
স্থির অক্ষর *log_id;

স্থির ধ্রুবক অক্ষর *
set_log_id (ধ্রুবক অক্ষর *id, অক্ষর **old_id)
{
  *old_id = log_id;
  log_id = id ? xstrdup (id) : ফাঁকা;
  ফেরত log_id;
}

স্থির শূন্য *
init_syslog (ধ্রুবক অক্ষর *id)
{
  openlog (id, 0, LOG_USER);
  ফেরত ফাঁকা;
}

গণনা
{
  LOG_TYPE_FILE = 1,
  LOG_TYPE_SYSLOG = 2,
  LOG_TYPE_JOURNALD = 3
};

স্থির পূর্ণসংখ্যা
get_log_type (ধ্রুবক অক্ষর *log, ধ্রুবক অক্ষর **তথ্য)
{
  ধ্রুবক অক্ষর *sep = strchr (log, ':');
  যদি (sep == ফাঁকা)
    {
      *তথ্য = log;
      ফেরত LOG_TYPE_FILE;
    }
  *তথ্য = sep + 1;

  যদি (has_prefix (log, "syslog:"))
    ফেরত LOG_TYPE_SYSLOG;
  যদি (has_prefix (log, "journald:"))
    ফেরত LOG_TYPE_JOURNALD;
  যদি (has_prefix (log, "file:"))
    ফেরত LOG_TYPE_FILE;

  ফেরত -1;
}

পূর্ণসংখ্যা
libcrun_init_logging (crun_output_handler *new_output_handler, শূন্য **new_output_handler_arg, ধ্রুবক অক্ষর *id,
                      ধ্রুবক অক্ষর *log, libcrun_error_t *err)
{
  /* Released only after the new handler is installed.  */
  cleanup_free অক্ষর *old_log_id = ফাঁকা;

  যদি (log == ফাঁকা)
    {
      *new_output_handler = log_write_to_stderr;
      *new_output_handler_arg = ফাঁকা;
    }
  অন্যথা
    {
      ধ্রুবক অক্ষর *arg = ফাঁকা;
      পূর্ণসংখ্যা log_type = get_log_type (log, &arg);

      যদি (log_type < 0)
        ফেরত crun_make_error (err, errno, "unknown log type `%s`", log);

      নির্বাচন (log_type)
        {
        ক্ষেত্র LOG_TYPE_FILE:
          *new_output_handler = log_write_to_stream;
          *new_output_handler_arg = fopen (arg, "a+e");
          যদি (*new_output_handler_arg == ফাঁকা)
            ফেরত crun_make_error (err, errno, "open log file `%s`", log);
          যদি (output_verbosity >= LIBCRUN_VERBOSITY_WARNING)
            setlinebuf (*new_output_handler_arg);
          বিরতি;

        ক্ষেত্র LOG_TYPE_SYSLOG:
          *new_output_handler_arg = init_syslog (set_log_id (arg[0] ? arg : id, &old_log_id));
          *new_output_handler = log_write_to_syslog;
          বিরতি;

        ক্ষেত্র LOG_TYPE_JOURNALD:
          *new_output_handler = log_write_to_journald;
          *new_output_handler_arg = (শূন্য *) set_log_id (id, &old_log_id);
          বিরতি;
        }
    }
  crun_set_output_handler (*new_output_handler, *new_output_handler_arg);
  ফেরত 0;
}

শূন্য
log_write_to_stream (পূর্ণসংখ্যা errno_, ধ্রুবক অক্ষর *বার্তা, পূর্ণসংখ্যা verbosity, শূন্য *arg)
{
  timestamp_t timestamp = {
    0,
  };
  FILE *stream = arg;
  পূর্ণসংখ্যা tty = isatty (fileno (stream));
  ধ্রুবক অক্ষর *color_begin = "";
  ধ্রুবক অক্ষর *color_end = "";

  যদি (tty)
    {
      নির্বাচন (verbosity)
        {
        ক্ষেত্র LIBCRUN_VERBOSITY_DEBUG:
          color_begin = "\x1b[1;34m";
          color_end = "\x1b[0m";
          বিরতি;
        ক্ষেত্র LIBCRUN_VERBOSITY_WARNING:
          color_begin = "\x1b[1;33m";
          color_end = "\x1b[0m";
          বিরতি;
        ক্ষেত্র LIBCRUN_VERBOSITY_ERROR:
          color_begin = "\x1b[1;31m";
          color_end = "\x1b[0m";
          বিরতি;
        }

      যদি (log_format == LOG_FORMAT_TEXT)
        get_timestamp (&timestamp, ": ");
    }

  যদি (errno_)
    fprintf (stream, "%s%s%s: %s%s\n", color_begin, timestamp, বার্তা, strerror (errno_), color_end);
  অন্যথা
    fprintf (stream, "%s%s%s%s\n", color_begin, timestamp, বার্তা, color_end);
}

শূন্য
log_write_to_stderr (পূর্ণসংখ্যা errno_, ধ্রুবক অক্ষর *বার্তা, পূর্ণসংখ্যা verbosity, শূন্য *arg arg_unused)
{
  log_write_to_stream (errno_, বার্তা, verbosity, stderr);
}

শূন্য
log_write_to_syslog (পূর্ণসংখ্যা errno_, ধ্রুবক অক্ষর *বার্তা, পূর্ণসংখ্যা verbosity, শূন্য *arg arg_unused)
{
  পূর্ণসংখ্যা priority = LOG_ERR;
  নির্বাচন (verbosity)
    {
    ক্ষেত্র LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      বিরতি;
    ক্ষেত্র LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      বিরতি;
    ক্ষেত্র LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      বিরতি;
    }
  যদি (errno_ == 0)
    syslog (priority, "%s", বার্তা);
  অন্যথা
    syslog (priority, "%s: %s", বার্তা, strerror (errno_));
}

শূন্য
log_write_to_journald (পূর্ণসংখ্যা errno_, ধ্রুবক অক্ষর *বার্তা, পূর্ণসংখ্যা verbosity, শূন্য *arg arg_unused)
{
  (শূন্য) errno_;
  (শূন্য) বার্তা;
  (শূন্য) verbosity;
#ifdef HAVE_SYSTEMD
  পূর্ণসংখ্যা priority = LOG_ERR;
  নির্বাচন (verbosity)
    {
    ক্ষেত্র LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      বিরতি;
    ক্ষেত্র LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      বিরতি;
    ক্ষেত্র LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      বিরতি;
    }
  যদি (errno_ == 0)
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s", বার্তা, "ID=%s", arg, ফাঁকা);
  অন্যথা
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s: %s", বার্তা, strerror (errno_), "ID=%s",
                     arg, ফাঁকা);
#endif
}

স্থির crun_output_handler output_handler = log_write_to_stderr;
স্থির শূন্য *output_handler_arg = ফাঁকা;

শূন্য
libcrun_set_verbosity (পূর্ণসংখ্যা verbosity)
{
  output_verbosity = verbosity;
}

পূর্ণসংখ্যা
libcrun_get_verbosity ()
{
  ফেরত output_verbosity;
}

শূন্য
crun_set_output_handler (crun_output_handler handler, শূন্য *arg)
{
  output_handler = handler;
  output_handler_arg = arg;
}

স্থির অক্ষর *
make_json_error (ধ্রুবক অক্ষর *বার্তা, পূর্ণসংখ্যা errno_, পূর্ণসংখ্যা verbosity)
{
  ধ্রুবক অক্ষর *level;
  নির্বাচন (verbosity)
    {
    ক্ষেত্র LIBCRUN_VERBOSITY_DEBUG:
      level = "debug";
      বিরতি;
    ক্ষেত্র LIBCRUN_VERBOSITY_WARNING:
      level = "warning";
      বিরতি;
    ক্ষেত্র LIBCRUN_VERBOSITY_ERROR:
      level = "error";
      বিরতি;
    }
  ধ্রুবক অক্ষর *buf = ফাঁকা;
  json_gen_ctx *gen = ফাঁকা;
  অক্ষর *ret = ফাঁকা;
  size_t buf_len;
  timestamp_t timestamp = {
    0,
  };

  যদি (! json_gen_init (&gen, ফাঁকা))
    ফেরত ফাঁকা;

  get_timestamp (&timestamp, "");

  json_gen_map_open (gen);

  json_gen_string (gen, "msg", strlen ("msg"));
  যদি (errno_ == 0)
    json_gen_string (gen, বার্তা, strlen (বার্তা));
  অন্যথা
    {
      cleanup_free অক্ষর *tmp = ফাঁকা;

      xasprintf (&tmp, "%s: %s", বার্তা, strerror (errno_));
      json_gen_string (gen, tmp, strlen (tmp));
    }

  json_gen_string (gen, "level", strlen ("level"));
  json_gen_string (gen, level, strlen (level));

  json_gen_string (gen, "time", strlen ("time"));
  json_gen_string (gen, timestamp, strlen (timestamp));

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);
  যদি (buf)
    ret = strdup (buf);

  json_gen_free (gen);

  ফেরত ret;
}

স্থির শূন্য
write_log (পূর্ণসংখ্যা errno_, পূর্ণসংখ্যা verbosity, ধ্রুবক অক্ষর *বার্তা, va_list args_list)
{
  পূর্ণসংখ্যা ret;
  cleanup_free অক্ষর *output = ফাঁকা;
  cleanup_free অক্ষর *json = ফাঁকা;

  যদি (verbosity > output_verbosity)
    ফেরত;

  ret = vasprintf (&output, বার্তা, args_list);
  যদি (UNLIKELY (ret < 0))
    OOM ();

  যদি (verbosity == LIBCRUN_VERBOSITY_ERROR && output_handler != log_write_to_stderr)
    log_write_to_stderr (errno_, output, LIBCRUN_VERBOSITY_ERROR, ফাঁকা);

  নির্বাচন (log_format)
    {
    ক্ষেত্র LOG_FORMAT_TEXT:
      output_handler (errno_, output, verbosity, output_handler_arg);
      বিরতি;

    ক্ষেত্র LOG_FORMAT_JSON:
      json = make_json_error (output, errno_, verbosity);
      যদি (json)
        output_handler (0, json, verbosity, output_handler_arg);
      অন্যথা
        output_handler (errno_, output, verbosity, output_handler_arg);
      বিরতি;
    }
}

শূন্য
libcrun_debug (ধ্রুবক অক্ষর *বার্তা, ...)
{
  va_list args_list;
  va_start (args_list, বার্তা);
  write_log (0, LIBCRUN_VERBOSITY_DEBUG, বার্তা, args_list);
  va_end (args_list);
}

শূন্য
libcrun_warning (ধ্রুবক অক্ষর *বার্তা, ...)
{
  va_list args_list;
  va_start (args_list, বার্তা);
  write_log (0, LIBCRUN_VERBOSITY_WARNING, বার্তা, args_list);
  va_end (args_list);
}

শূন্য
libcrun_error (পূর্ণসংখ্যা errno_, ধ্রুবক অক্ষর *বার্তা, ...)
{
  va_list args_list;
  va_start (args_list, বার্তা);

  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, বার্তা, args_list);
  va_end (args_list);
}

শূন্য __attribute__ ((noreturn))
libcrun_fail_with_error (পূর্ণসংখ্যা errno_, ধ্রুবক অক্ষর *বার্তা, ...)
{
  va_list args_list;
  va_start (args_list, বার্তা);
  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, বার্তা, args_list);
  va_end (args_list);
  exit (EXIT_FAILURE);
}

পূর্ণসংখ্যা
libcrun_set_log_format (ধ্রুবক অক্ষর *format, libcrun_error_t *err)
{
  যদি (strcmp (format, "text") == 0)
    log_format = LOG_FORMAT_TEXT;
  অন্যথা যদি (strcmp (format, "json") == 0)
    log_format = LOG_FORMAT_JSON;
  অন্যথা
    ফেরত crun_make_error (err, 0, "unknown log format type `%s`", format);

  ফেরত 0;
}

পূর্ণসংখ্যা
json_gen_error_to_crun_error (পূর্ণসংখ্যা status, libcrun_error_t *err)
{
  নির্বাচন (status)
    {
    ক্ষেত্র json_gen_status_ok:
      ফেরত 0;

    পূর্বনির্ধারিত:
      ফেরত crun_make_error (err, 0, "generate JSON document: error %d", status);
    }
}
