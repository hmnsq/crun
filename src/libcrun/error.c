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
#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdarg.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <time.h>
#અન્તર્ભાવ <sys/time.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ "utils.h"

#અન્તર્ભાવ <json-c/json.h>
#અન્તર્ભાવ <ocispec/json_common.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-journal.h>
#endif

ગણના
{
  LOG_FORMAT_TEXT = 0,
  LOG_FORMAT_JSON,
};

સ્થિર પૂર્ણાંક log_format;
સ્થિર પૂર્ણાંક output_verbosity = LIBCRUN_VERBOSITY_ERROR;

પૂર્ણાંક
libcrun_make_error (libcrun_error_t *err, પૂર્ણાંક status, અચલ અક્ષર *સંદેશ, ...)
{
  va_list args_list;
  libcrun_error_t ptr;

  va_start (args_list, સંદેશ);
  *err = xmalloc (કદ (સંરચના libcrun_error_s));
  ptr = *err;
  ptr->status = status;
  જો (vasprintf (&(ptr->સંદેશ), સંદેશ, args_list) < 0)
    OOM ();
  va_end (args_list);

  પાછા -status - 1;
}

પૂર્ણાંક
crun_error_wrap (libcrun_error_t *err, અચલ અક્ષર *fmt, ...)
{
  cleanup_free અક્ષર *સંદેશ = ખાલી;
  cleanup_free અક્ષર *tmp = ખાલી;
  va_list args_list;
  અક્ષર *swap;
  પૂર્ણાંક ret;

  જો (err == ખાલી || *err == ખાલી)
    {
      // Internal error
      પાછા 0;
    }

  ret = -(*err)->status - 1;

  va_start (args_list, fmt);

  જો (vasprintf (&સંદેશ, fmt, args_list) < 0)
    {
      va_end (args_list);
      સંદેશ = ખાલી;
      પાછા ret;
    }
  va_end (args_list);

  xasprintf (&tmp, "%s: %s", સંદેશ, (*err)->સંદેશ);
  swap = tmp;
  tmp = (*err)->સંદેશ;
  (*err)->સંદેશ = swap;

  પાછા ret;
}

પૂર્ણાંક
crun_error_release (libcrun_error_t *err)
{
  અચલ પૂર્ણાંક saved_errno = errno;
  libcrun_error_t ptr;

  જો (err == ખાલી)
    પાછા 0;

  ptr = *err;
  જો (ptr == ખાલી)
    પાછા 0;

  મુક્ત (ptr->સંદેશ);
  મુક્ત (ptr);
  *err = ખાલી;

  errno = saved_errno;
  પાછા 0;
}

પૂર્ણાંક
libcrun_error_release (libcrun_error_t *err)
{
  પાછા crun_error_release (err);
}

પૂર્ણાંક
libcrun_error_get_status (libcrun_error_t err)
{
  જો (err == ખાલી)
    પાછા 0;
  પાછા err->status;
}

અચલ અક્ષર *
libcrun_error_get_message (libcrun_error_t err)
{
  જો (err == ખાલી)
    પાછા ખાલી;
  પાછા err->સંદેશ;
}

શૂન્ય
crun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  અચલ પૂર્ણાંક saved_errno = errno;
  libcrun_error_t ref;

  જો (out == ખાલી)
    out = stderr;
  જો (err == ખાલી || *err == ખાલી || **err == ખાલી)
    {
      // Internal error
      પાછા;
    }

  ref = **err;
  જો (ref->status)
    fprintf (out, "%s: %s\n", ref->સંદેશ, strerror (ref->status));
  નહીંતર
    fprintf (out, "%s\n", ref->સંદેશ);

  મુક્ત (ref->સંદેશ);
  મુક્ત (ref);
  **err = ખાલી;

  errno = saved_errno;
}

શૂન્ય
libcrun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  પાછા crun_error_write_warning_and_release (out, err);
}

શૂન્ય
libcrun_error_report_and_release (libcrun_error_t *err)
{
  libcrun_error_t ref;

  જો (err == ખાલી || *err == ખાલી)
    પાછા;

  ref = *err;
  libcrun_error (ref->status, "%s", ref->સંદેશ);
  crun_error_release (err);
}

પૂર્ણાંક
crun_error_get_errno (libcrun_error_t *err)
{
  જો (err == ખાલી || *err == ખાલી)
    પાછા 0;
  પાછા (*err)->status;
}

પ્રકારવ્યાખ્યા અક્ષર timestamp_t[64];

સ્થિર શૂન્ય
get_timestamp (timestamp_t *timestamp, અચલ અક્ષર *suffix)
{
  અચલ size_t buffer_size = કદ (timestamp_t);
  અક્ષર *buffer = (અક્ષર *) timestamp;
  સંરચના timeval tv;
  size_t written;
  સંરચના tm now;

  gettimeofday (&tv, ખાલી);
  gmtime_r (&tv.tv_sec, &now);
  written = strftime (buffer, buffer_size, "%Y-%m-%dT%H:%M:%S", &now);
  snprintf (buffer + written, buffer_size - written, ".%06lldZ%.8s", (long long પૂર્ણાંક) tv.tv_usec, suffix);
}

/* Neither openlog(3) nor the journald handler copy the identifier they are
   given, and the global output handler outlives the caller's strings (e.g.
   the context is freed while errors are still reported through it), so keep
   an owned copy alive for as long as the handler can use it.  */
સ્થિર અક્ષર *log_id;

સ્થિર અચલ અક્ષર *
set_log_id (અચલ અક્ષર *id, અક્ષર **old_id)
{
  *old_id = log_id;
  log_id = id ? xstrdup (id) : ખાલી;
  પાછા log_id;
}

સ્થિર શૂન્ય *
init_syslog (અચલ અક્ષર *id)
{
  openlog (id, 0, LOG_USER);
  પાછા ખાલી;
}

ગણના
{
  LOG_TYPE_FILE = 1,
  LOG_TYPE_SYSLOG = 2,
  LOG_TYPE_JOURNALD = 3
};

સ્થિર પૂર્ણાંક
get_log_type (અચલ અક્ષર *log, અચલ અક્ષર **data)
{
  અચલ અક્ષર *sep = strchr (log, ':');
  જો (sep == ખાલી)
    {
      *data = log;
      પાછા LOG_TYPE_FILE;
    }
  *data = sep + 1;

  જો (has_prefix (log, "syslog:"))
    પાછા LOG_TYPE_SYSLOG;
  જો (has_prefix (log, "journald:"))
    પાછા LOG_TYPE_JOURNALD;
  જો (has_prefix (log, "file:"))
    પાછા LOG_TYPE_FILE;

  પાછા -1;
}

પૂર્ણાંક
libcrun_init_logging (crun_output_handler *new_output_handler, શૂન્ય **new_output_handler_arg, અચલ અક્ષર *id,
                      અચલ અક્ષર *log, libcrun_error_t *err)
{
  /* Released only after the new handler is installed.  */
  cleanup_free અક્ષર *old_log_id = ખાલી;

  જો (log == ખાલી)
    {
      *new_output_handler = log_write_to_stderr;
      *new_output_handler_arg = ખાલી;
    }
  નહીંતર
    {
      અચલ અક્ષર *arg = ખાલી;
      પૂર્ણાંક log_type = get_log_type (log, &arg);

      જો (log_type < 0)
        પાછા crun_make_error (err, errno, "unknown log type `%s`", log);

      પસંદગી (log_type)
        {
        સ્થિતિ LOG_TYPE_FILE:
          *new_output_handler = log_write_to_stream;
          *new_output_handler_arg = fopen (arg, "a+e");
          જો (*new_output_handler_arg == ખાલી)
            પાછા crun_make_error (err, errno, "open log file `%s`", log);
          જો (output_verbosity >= LIBCRUN_VERBOSITY_WARNING)
            setlinebuf (*new_output_handler_arg);
          તોડો;

        સ્થિતિ LOG_TYPE_SYSLOG:
          *new_output_handler_arg = init_syslog (set_log_id (arg[0] ? arg : id, &old_log_id));
          *new_output_handler = log_write_to_syslog;
          તોડો;

        સ્થિતિ LOG_TYPE_JOURNALD:
          *new_output_handler = log_write_to_journald;
          *new_output_handler_arg = (શૂન્ય *) set_log_id (id, &old_log_id);
          તોડો;
        }
    }
  crun_set_output_handler (*new_output_handler, *new_output_handler_arg);
  પાછા 0;
}

શૂન્ય
log_write_to_stream (પૂર્ણાંક errno_, અચલ અક્ષર *સંદેશ, પૂર્ણાંક verbosity, શૂન્ય *arg)
{
  timestamp_t timestamp = {
    0,
  };
  FILE *stream = arg;
  પૂર્ણાંક tty = isatty (fileno (stream));
  અચલ અક્ષર *color_begin = "";
  અચલ અક્ષર *color_end = "";

  જો (tty)
    {
      પસંદગી (verbosity)
        {
        સ્થિતિ LIBCRUN_VERBOSITY_DEBUG:
          color_begin = "\x1b[1;34m";
          color_end = "\x1b[0m";
          તોડો;
        સ્થિતિ LIBCRUN_VERBOSITY_WARNING:
          color_begin = "\x1b[1;33m";
          color_end = "\x1b[0m";
          તોડો;
        સ્થિતિ LIBCRUN_VERBOSITY_ERROR:
          color_begin = "\x1b[1;31m";
          color_end = "\x1b[0m";
          તોડો;
        }

      જો (log_format == LOG_FORMAT_TEXT)
        get_timestamp (&timestamp, ": ");
    }

  જો (errno_)
    fprintf (stream, "%s%s%s: %s%s\n", color_begin, timestamp, સંદેશ, strerror (errno_), color_end);
  નહીંતર
    fprintf (stream, "%s%s%s%s\n", color_begin, timestamp, સંદેશ, color_end);
}

શૂન્ય
log_write_to_stderr (પૂર્ણાંક errno_, અચલ અક્ષર *સંદેશ, પૂર્ણાંક verbosity, શૂન્ય *arg arg_unused)
{
  log_write_to_stream (errno_, સંદેશ, verbosity, stderr);
}

શૂન્ય
log_write_to_syslog (પૂર્ણાંક errno_, અચલ અક્ષર *સંદેશ, પૂર્ણાંક verbosity, શૂન્ય *arg arg_unused)
{
  પૂર્ણાંક priority = LOG_ERR;
  પસંદગી (verbosity)
    {
    સ્થિતિ LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      તોડો;
    સ્થિતિ LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      તોડો;
    સ્થિતિ LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      તોડો;
    }
  જો (errno_ == 0)
    syslog (priority, "%s", સંદેશ);
  નહીંતર
    syslog (priority, "%s: %s", સંદેશ, strerror (errno_));
}

શૂન્ય
log_write_to_journald (પૂર્ણાંક errno_, અચલ અક્ષર *સંદેશ, પૂર્ણાંક verbosity, શૂન્ય *arg arg_unused)
{
  (શૂન્ય) errno_;
  (શૂન્ય) સંદેશ;
  (શૂન્ય) verbosity;
#ifdef HAVE_SYSTEMD
  પૂર્ણાંક priority = LOG_ERR;
  પસંદગી (verbosity)
    {
    સ્થિતિ LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      તોડો;
    સ્થિતિ LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      તોડો;
    સ્થિતિ LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      તોડો;
    }
  જો (errno_ == 0)
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s", સંદેશ, "ID=%s", arg, ખાલી);
  નહીંતર
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s: %s", સંદેશ, strerror (errno_), "ID=%s",
                     arg, ખાલી);
#endif
}

સ્થિર crun_output_handler output_handler = log_write_to_stderr;
સ્થિર શૂન્ય *output_handler_arg = ખાલી;

શૂન્ય
libcrun_set_verbosity (પૂર્ણાંક verbosity)
{
  output_verbosity = verbosity;
}

પૂર્ણાંક
libcrun_get_verbosity ()
{
  પાછા output_verbosity;
}

શૂન્ય
crun_set_output_handler (crun_output_handler handler, શૂન્ય *arg)
{
  output_handler = handler;
  output_handler_arg = arg;
}

સ્થિર અક્ષર *
make_json_error (અચલ અક્ષર *સંદેશ, પૂર્ણાંક errno_, પૂર્ણાંક verbosity)
{
  અચલ અક્ષર *level;
  પસંદગી (verbosity)
    {
    સ્થિતિ LIBCRUN_VERBOSITY_DEBUG:
      level = "debug";
      તોડો;
    સ્થિતિ LIBCRUN_VERBOSITY_WARNING:
      level = "warning";
      તોડો;
    સ્થિતિ LIBCRUN_VERBOSITY_ERROR:
      level = "error";
      તોડો;
    }
  અચલ અક્ષર *buf = ખાલી;
  json_gen_ctx *gen = ખાલી;
  અક્ષર *ret = ખાલી;
  size_t buf_len;
  timestamp_t timestamp = {
    0,
  };

  જો (! json_gen_init (&gen, ખાલી))
    પાછા ખાલી;

  get_timestamp (&timestamp, "");

  json_gen_map_open (gen);

  json_gen_string (gen, "msg", strlen ("msg"));
  જો (errno_ == 0)
    json_gen_string (gen, સંદેશ, strlen (સંદેશ));
  નહીંતર
    {
      cleanup_free અક્ષર *tmp = ખાલી;

      xasprintf (&tmp, "%s: %s", સંદેશ, strerror (errno_));
      json_gen_string (gen, tmp, strlen (tmp));
    }

  json_gen_string (gen, "level", strlen ("level"));
  json_gen_string (gen, level, strlen (level));

  json_gen_string (gen, "time", strlen ("time"));
  json_gen_string (gen, timestamp, strlen (timestamp));

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);
  જો (buf)
    ret = strdup (buf);

  json_gen_free (gen);

  પાછા ret;
}

સ્થિર શૂન્ય
write_log (પૂર્ણાંક errno_, પૂર્ણાંક verbosity, અચલ અક્ષર *સંદેશ, va_list args_list)
{
  પૂર્ણાંક ret;
  cleanup_free અક્ષર *output = ખાલી;
  cleanup_free અક્ષર *json = ખાલી;

  જો (verbosity > output_verbosity)
    પાછા;

  ret = vasprintf (&output, સંદેશ, args_list);
  જો (UNLIKELY (ret < 0))
    OOM ();

  જો (verbosity == LIBCRUN_VERBOSITY_ERROR && output_handler != log_write_to_stderr)
    log_write_to_stderr (errno_, output, LIBCRUN_VERBOSITY_ERROR, ખાલી);

  પસંદગી (log_format)
    {
    સ્થિતિ LOG_FORMAT_TEXT:
      output_handler (errno_, output, verbosity, output_handler_arg);
      તોડો;

    સ્થિતિ LOG_FORMAT_JSON:
      json = make_json_error (output, errno_, verbosity);
      જો (json)
        output_handler (0, json, verbosity, output_handler_arg);
      નહીંતર
        output_handler (errno_, output, verbosity, output_handler_arg);
      તોડો;
    }
}

શૂન્ય
libcrun_debug (અચલ અક્ષર *સંદેશ, ...)
{
  va_list args_list;
  va_start (args_list, સંદેશ);
  write_log (0, LIBCRUN_VERBOSITY_DEBUG, સંદેશ, args_list);
  va_end (args_list);
}

શૂન્ય
libcrun_warning (અચલ અક્ષર *સંદેશ, ...)
{
  va_list args_list;
  va_start (args_list, સંદેશ);
  write_log (0, LIBCRUN_VERBOSITY_WARNING, સંદેશ, args_list);
  va_end (args_list);
}

શૂન્ય
libcrun_error (પૂર્ણાંક errno_, અચલ અક્ષર *સંદેશ, ...)
{
  va_list args_list;
  va_start (args_list, સંદેશ);

  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, સંદેશ, args_list);
  va_end (args_list);
}

શૂન્ય __attribute__ ((noreturn))
libcrun_fail_with_error (પૂર્ણાંક errno_, અચલ અક્ષર *સંદેશ, ...)
{
  va_list args_list;
  va_start (args_list, સંદેશ);
  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, સંદેશ, args_list);
  va_end (args_list);
  exit (EXIT_FAILURE);
}

પૂર્ણાંક
libcrun_set_log_format (અચલ અક્ષર *format, libcrun_error_t *err)
{
  જો (strcmp (format, "text") == 0)
    log_format = LOG_FORMAT_TEXT;
  નહીંતર જો (strcmp (format, "json") == 0)
    log_format = LOG_FORMAT_JSON;
  નહીંતર
    પાછા crun_make_error (err, 0, "unknown log format type `%s`", format);

  પાછા 0;
}

પૂર્ણાંક
json_gen_error_to_crun_error (પૂર્ણાંક status, libcrun_error_t *err)
{
  પસંદગી (status)
    {
    સ્થિતિ json_gen_status_ok:
      પાછા 0;

    મૂળભૂત:
      પાછા crun_make_error (err, 0, "generate JSON document: error %d", status);
    }
}
