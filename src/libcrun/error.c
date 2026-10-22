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
#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <stdarg.h>
#অন্তর্ভাৱ <unistd.h>
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <time.h>
#অন্তর্ভাৱ <sys/time.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ "utils.h"

#অন্তর্ভাৱ <json-c/json.h>
#অন্তর্ভাৱ <ocispec/json_common.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-journal.h>
#endif

গণনা
{
  LOG_FORMAT_TEXT = 0,
  LOG_FORMAT_JSON,
};

স্থির পূর্ণাঙ্ক log_format;
স্থির পূর্ণাঙ্ক output_verbosity = LIBCRUN_VERBOSITY_ERROR;

পূর্ণাঙ্ক
libcrun_make_error (libcrun_error_t *err, পূর্ণাঙ্ক status, ধ্রুৱ ৱর্ণ *বাৰ্তা, ...)
{
  va_list args_list;
  libcrun_error_t ptr;

  va_start (args_list, বাৰ্তা);
  *err = xmalloc (পরিমাণ (সংরচনা libcrun_error_s));
  ptr = *err;
  ptr->status = status;
  যদি (vasprintf (&(ptr->বাৰ্তা), বাৰ্তা, args_list) < 0)
    OOM ();
  va_end (args_list);

  ঘূৰাই -status - 1;
}

পূর্ণাঙ্ক
crun_error_wrap (libcrun_error_t *err, ধ্রুৱ ৱর্ণ *fmt, ...)
{
  cleanup_free ৱর্ণ *বাৰ্তা = রিক্ত;
  cleanup_free ৱর্ণ *tmp = রিক্ত;
  va_list args_list;
  ৱর্ণ *swap;
  পূর্ণাঙ্ক ret;

  যদি (err == রিক্ত || *err == রিক্ত)
    {
      // Internal error
      ঘূৰাই 0;
    }

  ret = -(*err)->status - 1;

  va_start (args_list, fmt);

  যদি (vasprintf (&বাৰ্তা, fmt, args_list) < 0)
    {
      va_end (args_list);
      বাৰ্তা = রিক্ত;
      ঘূৰাই ret;
    }
  va_end (args_list);

  xasprintf (&tmp, "%s: %s", বাৰ্তা, (*err)->বাৰ্তা);
  swap = tmp;
  tmp = (*err)->বাৰ্তা;
  (*err)->বাৰ্তা = swap;

  ঘূৰাই ret;
}

পূর্ণাঙ্ক
crun_error_release (libcrun_error_t *err)
{
  ধ্রুৱ পূর্ণাঙ্ক saved_errno = errno;
  libcrun_error_t ptr;

  যদি (err == রিক্ত)
    ঘূৰাই 0;

  ptr = *err;
  যদি (ptr == রিক্ত)
    ঘূৰাই 0;

  মুক্ত (ptr->বাৰ্তা);
  মুক্ত (ptr);
  *err = রিক্ত;

  errno = saved_errno;
  ঘূৰাই 0;
}

পূর্ণাঙ্ক
libcrun_error_release (libcrun_error_t *err)
{
  ঘূৰাই crun_error_release (err);
}

পূর্ণাঙ্ক
libcrun_error_get_status (libcrun_error_t err)
{
  যদি (err == রিক্ত)
    ঘূৰাই 0;
  ঘূৰাই err->status;
}

ধ্রুৱ ৱর্ণ *
libcrun_error_get_message (libcrun_error_t err)
{
  যদি (err == রিক্ত)
    ঘূৰাই রিক্ত;
  ঘূৰাই err->বাৰ্তা;
}

শূন্য
crun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  ধ্রুৱ পূর্ণাঙ্ক saved_errno = errno;
  libcrun_error_t ref;

  যদি (out == রিক্ত)
    out = stderr;
  যদি (err == রিক্ত || *err == রিক্ত || **err == রিক্ত)
    {
      // Internal error
      ঘূৰাই;
    }

  ref = **err;
  যদি (ref->status)
    fprintf (out, "%s: %s\n", ref->বাৰ্তা, strerror (ref->status));
  অন্যথা
    fprintf (out, "%s\n", ref->বাৰ্তা);

  মুক্ত (ref->বাৰ্তা);
  মুক্ত (ref);
  **err = রিক্ত;

  errno = saved_errno;
}

শূন্য
libcrun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  ঘূৰাই crun_error_write_warning_and_release (out, err);
}

শূন্য
libcrun_error_report_and_release (libcrun_error_t *err)
{
  libcrun_error_t ref;

  যদি (err == রিক্ত || *err == রিক্ত)
    ঘূৰাই;

  ref = *err;
  libcrun_error (ref->status, "%s", ref->বাৰ্তা);
  crun_error_release (err);
}

পূর্ণাঙ্ক
crun_error_get_errno (libcrun_error_t *err)
{
  যদি (err == রিক্ত || *err == রিক্ত)
    ঘূৰাই 0;
  ঘূৰাই (*err)->status;
}

প্রকারনির্দেশ ৱর্ণ timestamp_t[64];

স্থির শূন্য
get_timestamp (timestamp_t *timestamp, ধ্রুৱ ৱর্ণ *suffix)
{
  ধ্রুৱ size_t buffer_size = পরিমাণ (timestamp_t);
  ৱর্ণ *buffer = (ৱর্ণ *) timestamp;
  সংরচনা timeval tv;
  size_t written;
  সংরচনা tm now;

  gettimeofday (&tv, রিক্ত);
  gmtime_r (&tv.tv_sec, &now);
  written = strftime (buffer, buffer_size, "%Y-%m-%dT%H:%M:%S", &now);
  snprintf (buffer + written, buffer_size - written, ".%06lldZ%.8s", (long long পূর্ণাঙ্ক) tv.tv_usec, suffix);
}

/* Neither openlog(3) nor the journald handler copy the identifier they are
   given, and the global output handler outlives the caller's strings (e.g.
   the context is freed while errors are still reported through it), so keep
   an owned copy alive for as long as the handler can use it.  */
স্থির ৱর্ণ *log_id;

স্থির ধ্রুৱ ৱর্ণ *
set_log_id (ধ্রুৱ ৱর্ণ *id, ৱর্ণ **old_id)
{
  *old_id = log_id;
  log_id = id ? xstrdup (id) : রিক্ত;
  ঘূৰাই log_id;
}

স্থির শূন্য *
init_syslog (ধ্রুৱ ৱর্ণ *id)
{
  openlog (id, 0, LOG_USER);
  ঘূৰাই রিক্ত;
}

গণনা
{
  LOG_TYPE_FILE = 1,
  LOG_TYPE_SYSLOG = 2,
  LOG_TYPE_JOURNALD = 3
};

স্থির পূর্ণাঙ্ক
get_log_type (ধ্রুৱ ৱর্ণ *log, ধ্রুৱ ৱর্ণ **data)
{
  ধ্রুৱ ৱর্ণ *sep = strchr (log, ':');
  যদি (sep == রিক্ত)
    {
      *data = log;
      ঘূৰাই LOG_TYPE_FILE;
    }
  *data = sep + 1;

  যদি (has_prefix (log, "syslog:"))
    ঘূৰাই LOG_TYPE_SYSLOG;
  যদি (has_prefix (log, "journald:"))
    ঘূৰাই LOG_TYPE_JOURNALD;
  যদি (has_prefix (log, "file:"))
    ঘূৰাই LOG_TYPE_FILE;

  ঘূৰাই -1;
}

পূর্ণাঙ্ক
libcrun_init_logging (crun_output_handler *new_output_handler, শূন্য **new_output_handler_arg, ধ্রুৱ ৱর্ণ *id,
                      ধ্রুৱ ৱর্ণ *log, libcrun_error_t *err)
{
  /* Released only after the new handler is installed.  */
  cleanup_free ৱর্ণ *old_log_id = রিক্ত;

  যদি (log == রিক্ত)
    {
      *new_output_handler = log_write_to_stderr;
      *new_output_handler_arg = রিক্ত;
    }
  অন্যথা
    {
      ধ্রুৱ ৱর্ণ *arg = রিক্ত;
      পূর্ণাঙ্ক log_type = get_log_type (log, &arg);

      যদি (log_type < 0)
        ঘূৰাই crun_make_error (err, errno, "unknown log type `%s`", log);

      ৱিকল্প (log_type)
        {
        পক্ষ LOG_TYPE_FILE:
          *new_output_handler = log_write_to_stream;
          *new_output_handler_arg = fopen (arg, "a+e");
          যদি (*new_output_handler_arg == রিক্ত)
            ঘূৰাই crun_make_error (err, errno, "open log file `%s`", log);
          যদি (output_verbosity >= LIBCRUN_VERBOSITY_WARNING)
            setlinebuf (*new_output_handler_arg);
          ভঙ্গ;

        পক্ষ LOG_TYPE_SYSLOG:
          *new_output_handler_arg = init_syslog (set_log_id (arg[0] ? arg : id, &old_log_id));
          *new_output_handler = log_write_to_syslog;
          ভঙ্গ;

        পক্ষ LOG_TYPE_JOURNALD:
          *new_output_handler = log_write_to_journald;
          *new_output_handler_arg = (শূন্য *) set_log_id (id, &old_log_id);
          ভঙ্গ;
        }
    }
  crun_set_output_handler (*new_output_handler, *new_output_handler_arg);
  ঘূৰাই 0;
}

শূন্য
log_write_to_stream (পূর্ণাঙ্ক errno_, ধ্রুৱ ৱর্ণ *বাৰ্তা, পূর্ণাঙ্ক verbosity, শূন্য *arg)
{
  timestamp_t timestamp = {
    0,
  };
  FILE *stream = arg;
  পূর্ণাঙ্ক tty = isatty (fileno (stream));
  ধ্রুৱ ৱর্ণ *color_begin = "";
  ধ্রুৱ ৱর্ণ *color_end = "";

  যদি (tty)
    {
      ৱিকল্প (verbosity)
        {
        পক্ষ LIBCRUN_VERBOSITY_DEBUG:
          color_begin = "\x1b[1;34m";
          color_end = "\x1b[0m";
          ভঙ্গ;
        পক্ষ LIBCRUN_VERBOSITY_WARNING:
          color_begin = "\x1b[1;33m";
          color_end = "\x1b[0m";
          ভঙ্গ;
        পক্ষ LIBCRUN_VERBOSITY_ERROR:
          color_begin = "\x1b[1;31m";
          color_end = "\x1b[0m";
          ভঙ্গ;
        }

      যদি (log_format == LOG_FORMAT_TEXT)
        get_timestamp (&timestamp, ": ");
    }

  যদি (errno_)
    fprintf (stream, "%s%s%s: %s%s\n", color_begin, timestamp, বাৰ্তা, strerror (errno_), color_end);
  অন্যথা
    fprintf (stream, "%s%s%s%s\n", color_begin, timestamp, বাৰ্তা, color_end);
}

শূন্য
log_write_to_stderr (পূর্ণাঙ্ক errno_, ধ্রুৱ ৱর্ণ *বাৰ্তা, পূর্ণাঙ্ক verbosity, শূন্য *arg arg_unused)
{
  log_write_to_stream (errno_, বাৰ্তা, verbosity, stderr);
}

শূন্য
log_write_to_syslog (পূর্ণাঙ্ক errno_, ধ্রুৱ ৱর্ণ *বাৰ্তা, পূর্ণাঙ্ক verbosity, শূন্য *arg arg_unused)
{
  পূর্ণাঙ্ক priority = LOG_ERR;
  ৱিকল্প (verbosity)
    {
    পক্ষ LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      ভঙ্গ;
    পক্ষ LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      ভঙ্গ;
    পক্ষ LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      ভঙ্গ;
    }
  যদি (errno_ == 0)
    syslog (priority, "%s", বাৰ্তা);
  অন্যথা
    syslog (priority, "%s: %s", বাৰ্তা, strerror (errno_));
}

শূন্য
log_write_to_journald (পূর্ণাঙ্ক errno_, ধ্রুৱ ৱর্ণ *বাৰ্তা, পূর্ণাঙ্ক verbosity, শূন্য *arg arg_unused)
{
  (শূন্য) errno_;
  (শূন্য) বাৰ্তা;
  (শূন্য) verbosity;
#ifdef HAVE_SYSTEMD
  পূর্ণাঙ্ক priority = LOG_ERR;
  ৱিকল্প (verbosity)
    {
    পক্ষ LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      ভঙ্গ;
    পক্ষ LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      ভঙ্গ;
    পক্ষ LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      ভঙ্গ;
    }
  যদি (errno_ == 0)
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s", বাৰ্তা, "ID=%s", arg, রিক্ত);
  অন্যথা
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s: %s", বাৰ্তা, strerror (errno_), "ID=%s",
                     arg, রিক্ত);
#endif
}

স্থির crun_output_handler output_handler = log_write_to_stderr;
স্থির শূন্য *output_handler_arg = রিক্ত;

শূন্য
libcrun_set_verbosity (পূর্ণাঙ্ক verbosity)
{
  output_verbosity = verbosity;
}

পূর্ণাঙ্ক
libcrun_get_verbosity ()
{
  ঘূৰাই output_verbosity;
}

শূন্য
crun_set_output_handler (crun_output_handler handler, শূন্য *arg)
{
  output_handler = handler;
  output_handler_arg = arg;
}

স্থির ৱর্ণ *
make_json_error (ধ্রুৱ ৱর্ণ *বাৰ্তা, পূর্ণাঙ্ক errno_, পূর্ণাঙ্ক verbosity)
{
  ধ্রুৱ ৱর্ণ *level;
  ৱিকল্প (verbosity)
    {
    পক্ষ LIBCRUN_VERBOSITY_DEBUG:
      level = "debug";
      ভঙ্গ;
    পক্ষ LIBCRUN_VERBOSITY_WARNING:
      level = "warning";
      ভঙ্গ;
    পক্ষ LIBCRUN_VERBOSITY_ERROR:
      level = "error";
      ভঙ্গ;
    }
  ধ্রুৱ ৱর্ণ *buf = রিক্ত;
  json_gen_ctx *gen = রিক্ত;
  ৱর্ণ *ret = রিক্ত;
  size_t buf_len;
  timestamp_t timestamp = {
    0,
  };

  যদি (! json_gen_init (&gen, রিক্ত))
    ঘূৰাই রিক্ত;

  get_timestamp (&timestamp, "");

  json_gen_map_open (gen);

  json_gen_string (gen, "msg", strlen ("msg"));
  যদি (errno_ == 0)
    json_gen_string (gen, বাৰ্তা, strlen (বাৰ্তা));
  অন্যথা
    {
      cleanup_free ৱর্ণ *tmp = রিক্ত;

      xasprintf (&tmp, "%s: %s", বাৰ্তা, strerror (errno_));
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

  ঘূৰাই ret;
}

স্থির শূন্য
write_log (পূর্ণাঙ্ক errno_, পূর্ণাঙ্ক verbosity, ধ্রুৱ ৱর্ণ *বাৰ্তা, va_list args_list)
{
  পূর্ণাঙ্ক ret;
  cleanup_free ৱর্ণ *output = রিক্ত;
  cleanup_free ৱর্ণ *json = রিক্ত;

  যদি (verbosity > output_verbosity)
    ঘূৰাই;

  ret = vasprintf (&output, বাৰ্তা, args_list);
  যদি (UNLIKELY (ret < 0))
    OOM ();

  যদি (verbosity == LIBCRUN_VERBOSITY_ERROR && output_handler != log_write_to_stderr)
    log_write_to_stderr (errno_, output, LIBCRUN_VERBOSITY_ERROR, রিক্ত);

  ৱিকল্প (log_format)
    {
    পক্ষ LOG_FORMAT_TEXT:
      output_handler (errno_, output, verbosity, output_handler_arg);
      ভঙ্গ;

    পক্ষ LOG_FORMAT_JSON:
      json = make_json_error (output, errno_, verbosity);
      যদি (json)
        output_handler (0, json, verbosity, output_handler_arg);
      অন্যথা
        output_handler (errno_, output, verbosity, output_handler_arg);
      ভঙ্গ;
    }
}

শূন্য
libcrun_debug (ধ্রুৱ ৱর্ণ *বাৰ্তা, ...)
{
  va_list args_list;
  va_start (args_list, বাৰ্তা);
  write_log (0, LIBCRUN_VERBOSITY_DEBUG, বাৰ্তা, args_list);
  va_end (args_list);
}

শূন্য
libcrun_warning (ধ্রুৱ ৱর্ণ *বাৰ্তা, ...)
{
  va_list args_list;
  va_start (args_list, বাৰ্তা);
  write_log (0, LIBCRUN_VERBOSITY_WARNING, বাৰ্তা, args_list);
  va_end (args_list);
}

শূন্য
libcrun_error (পূর্ণাঙ্ক errno_, ধ্রুৱ ৱর্ণ *বাৰ্তা, ...)
{
  va_list args_list;
  va_start (args_list, বাৰ্তা);

  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, বাৰ্তা, args_list);
  va_end (args_list);
}

শূন্য __attribute__ ((noreturn))
libcrun_fail_with_error (পূর্ণাঙ্ক errno_, ধ্রুৱ ৱর্ণ *বাৰ্তা, ...)
{
  va_list args_list;
  va_start (args_list, বাৰ্তা);
  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, বাৰ্তা, args_list);
  va_end (args_list);
  exit (EXIT_FAILURE);
}

পূর্ণাঙ্ক
libcrun_set_log_format (ধ্রুৱ ৱর্ণ *format, libcrun_error_t *err)
{
  যদি (strcmp (format, "text") == 0)
    log_format = LOG_FORMAT_TEXT;
  অন্যথা যদি (strcmp (format, "json") == 0)
    log_format = LOG_FORMAT_JSON;
  অন্যথা
    ঘূৰাই crun_make_error (err, 0, "unknown log format type `%s`", format);

  ঘূৰাই 0;
}

পূর্ণাঙ্ক
json_gen_error_to_crun_error (পূর্ণাঙ্ক status, libcrun_error_t *err)
{
  ৱিকল্প (status)
    {
    পক্ষ json_gen_status_ok:
      ঘূৰাই 0;

    স্ৱভাৱ:
      ঘূৰাই crun_make_error (err, 0, "generate JSON document: error %d", status);
    }
}
