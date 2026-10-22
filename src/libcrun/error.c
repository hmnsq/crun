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
#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdarg.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <sys/time.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ "utils.h"

#ಅನ್ತರ್ಭಾವ <json-c/json.h>
#ಅನ್ತರ್ಭಾವ <ocispec/json_common.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-journal.h>
#endif

ಗಣನಾ
{
  LOG_FORMAT_TEXT = 0,
  LOG_FORMAT_JSON,
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ log_format;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ output_verbosity = LIBCRUN_VERBOSITY_ERROR;

ಪೂರ್ಣಾಙ್ಕ
libcrun_make_error (libcrun_error_t *err, ಪೂರ್ಣಾಙ್ಕ status, ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ, ...)
{
  va_list args_list;
  libcrun_error_t ptr;

  va_start (args_list, ಸಂದೇಶ);
  *err = xmalloc (ಪರಿಮಾಣ (ಸಂರಚನಾ libcrun_error_s));
  ptr = *err;
  ptr->status = status;
  ಯದಿ (vasprintf (&(ptr->ಸಂದೇಶ), ಸಂದೇಶ, args_list) < 0)
    OOM ();
  va_end (args_list);

  ಪ್ರತ್ಯಾಗಮ -status - 1;
}

ಪೂರ್ಣಾಙ್ಕ
crun_error_wrap (libcrun_error_t *err, ಧ್ರುವ ವರ್ಣ *fmt, ...)
{
  cleanup_free ವರ್ಣ *ಸಂದೇಶ = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *tmp = ರಿಕ್ತ;
  va_list args_list;
  ವರ್ಣ *swap;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (err == ರಿಕ್ತ || *err == ರಿಕ್ತ)
    {
      // Internal error
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  ret = -(*err)->status - 1;

  va_start (args_list, fmt);

  ಯದಿ (vasprintf (&ಸಂದೇಶ, fmt, args_list) < 0)
    {
      va_end (args_list);
      ಸಂದೇಶ = ರಿಕ್ತ;
      ಪ್ರತ್ಯಾಗಮ ret;
    }
  va_end (args_list);

  xasprintf (&tmp, "%s: %s", ಸಂದೇಶ, (*err)->ಸಂದೇಶ);
  swap = tmp;
  tmp = (*err)->ಸಂದೇಶ;
  (*err)->ಸಂದೇಶ = swap;

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
crun_error_release (libcrun_error_t *err)
{
  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
  libcrun_error_t ptr;

  ಯದಿ (err == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  ptr = *err;
  ಯದಿ (ptr == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಮುಕ್ತ (ptr->ಸಂದೇಶ);
  ಮುಕ್ತ (ptr);
  *err = ರಿಕ್ತ;

  errno = saved_errno;
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_error_release (libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ crun_error_release (err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_error_get_status (libcrun_error_t err)
{
  ಯದಿ (err == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;
  ಪ್ರತ್ಯಾಗಮ err->status;
}

ಧ್ರುವ ವರ್ಣ *
libcrun_error_get_message (libcrun_error_t err)
{
  ಯದಿ (err == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
  ಪ್ರತ್ಯಾಗಮ err->ಸಂದೇಶ;
}

ಶೂನ್ಯ
crun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
  libcrun_error_t ref;

  ಯದಿ (out == ರಿಕ್ತ)
    out = stderr;
  ಯದಿ (err == ರಿಕ್ತ || *err == ರಿಕ್ತ || **err == ರಿಕ್ತ)
    {
      // Internal error
      ಪ್ರತ್ಯಾಗಮ;
    }

  ref = **err;
  ಯದಿ (ref->status)
    fprintf (out, "%s: %s\n", ref->ಸಂದೇಶ, strerror (ref->status));
  ಅನ್ಯಥಾ
    fprintf (out, "%s\n", ref->ಸಂದೇಶ);

  ಮುಕ್ತ (ref->ಸಂದೇಶ);
  ಮುಕ್ತ (ref);
  **err = ರಿಕ್ತ;

  errno = saved_errno;
}

ಶೂನ್ಯ
libcrun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  ಪ್ರತ್ಯಾಗಮ crun_error_write_warning_and_release (out, err);
}

ಶೂನ್ಯ
libcrun_error_report_and_release (libcrun_error_t *err)
{
  libcrun_error_t ref;

  ಯದಿ (err == ರಿಕ್ತ || *err == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;

  ref = *err;
  libcrun_error (ref->status, "%s", ref->ಸಂದೇಶ);
  crun_error_release (err);
}

ಪೂರ್ಣಾಙ್ಕ
crun_error_get_errno (libcrun_error_t *err)
{
  ಯದಿ (err == ರಿಕ್ತ || *err == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;
  ಪ್ರತ್ಯಾಗಮ (*err)->status;
}

ಪ್ರಕಾರನಿರ್ದೇಶ ವರ್ಣ timestamp_t[64];

ಸ್ಥಿರ ಶೂನ್ಯ
get_timestamp (timestamp_t *timestamp, ಧ್ರುವ ವರ್ಣ *suffix)
{
  ಧ್ರುವ size_t buffer_size = ಪರಿಮಾಣ (timestamp_t);
  ವರ್ಣ *buffer = (ವರ್ಣ *) timestamp;
  ಸಂರಚನಾ timeval tv;
  size_t written;
  ಸಂರಚನಾ tm now;

  gettimeofday (&tv, ರಿಕ್ತ);
  gmtime_r (&tv.tv_sec, &now);
  written = strftime (buffer, buffer_size, "%Y-%m-%dT%H:%M:%S", &now);
  snprintf (buffer + written, buffer_size - written, ".%06lldZ%.8s", (long long ಪೂರ್ಣಾಙ್ಕ) tv.tv_usec, suffix);
}

/* Neither openlog(3) nor the journald handler copy the identifier they are
   given, and the global output handler outlives the caller's strings (e.g.
   the context is freed while errors are still reported through it), so keep
   an owned copy alive for as long as the handler can use it.  */
ಸ್ಥಿರ ವರ್ಣ *log_id;

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
set_log_id (ಧ್ರುವ ವರ್ಣ *id, ವರ್ಣ **old_id)
{
  *old_id = log_id;
  log_id = id ? xstrdup (id) : ರಿಕ್ತ;
  ಪ್ರತ್ಯಾಗಮ log_id;
}

ಸ್ಥಿರ ಶೂನ್ಯ *
init_syslog (ಧ್ರುವ ವರ್ಣ *id)
{
  openlog (id, 0, LOG_USER);
  ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಗಣನಾ
{
  LOG_TYPE_FILE = 1,
  LOG_TYPE_SYSLOG = 2,
  LOG_TYPE_JOURNALD = 3
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_log_type (ಧ್ರುವ ವರ್ಣ *log, ಧ್ರುವ ವರ್ಣ **data)
{
  ಧ್ರುವ ವರ್ಣ *sep = strchr (log, ':');
  ಯದಿ (sep == ರಿಕ್ತ)
    {
      *data = log;
      ಪ್ರತ್ಯಾಗಮ LOG_TYPE_FILE;
    }
  *data = sep + 1;

  ಯದಿ (has_prefix (log, "syslog:"))
    ಪ್ರತ್ಯಾಗಮ LOG_TYPE_SYSLOG;
  ಯದಿ (has_prefix (log, "journald:"))
    ಪ್ರತ್ಯಾಗಮ LOG_TYPE_JOURNALD;
  ಯದಿ (has_prefix (log, "file:"))
    ಪ್ರತ್ಯಾಗಮ LOG_TYPE_FILE;

  ಪ್ರತ್ಯಾಗಮ -1;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_init_logging (crun_output_handler *new_output_handler, ಶೂನ್ಯ **new_output_handler_arg, ಧ್ರುವ ವರ್ಣ *id,
                      ಧ್ರುವ ವರ್ಣ *log, libcrun_error_t *err)
{
  /* Released only after the new handler is installed.  */
  cleanup_free ವರ್ಣ *old_log_id = ರಿಕ್ತ;

  ಯದಿ (log == ರಿಕ್ತ)
    {
      *new_output_handler = log_write_to_stderr;
      *new_output_handler_arg = ರಿಕ್ತ;
    }
  ಅನ್ಯಥಾ
    {
      ಧ್ರುವ ವರ್ಣ *arg = ರಿಕ್ತ;
      ಪೂರ್ಣಾಙ್ಕ log_type = get_log_type (log, &arg);

      ಯದಿ (log_type < 0)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "unknown log type `%s`", log);

      ವಿಕಲ್ಪ (log_type)
        {
        ಪಕ್ಷ LOG_TYPE_FILE:
          *new_output_handler = log_write_to_stream;
          *new_output_handler_arg = fopen (arg, "a+e");
          ಯದಿ (*new_output_handler_arg == ರಿಕ್ತ)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open log file `%s`", log);
          ಯದಿ (output_verbosity >= LIBCRUN_VERBOSITY_WARNING)
            setlinebuf (*new_output_handler_arg);
          ಭಙ್ಗ;

        ಪಕ್ಷ LOG_TYPE_SYSLOG:
          *new_output_handler_arg = init_syslog (set_log_id (arg[0] ? arg : id, &old_log_id));
          *new_output_handler = log_write_to_syslog;
          ಭಙ್ಗ;

        ಪಕ್ಷ LOG_TYPE_JOURNALD:
          *new_output_handler = log_write_to_journald;
          *new_output_handler_arg = (ಶೂನ್ಯ *) set_log_id (id, &old_log_id);
          ಭಙ್ಗ;
        }
    }
  crun_set_output_handler (*new_output_handler, *new_output_handler_arg);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ
log_write_to_stream (ಪೂರ್ಣಾಙ್ಕ errno_, ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ, ಪೂರ್ಣಾಙ್ಕ verbosity, ಶೂನ್ಯ *arg)
{
  timestamp_t timestamp = {
    0,
  };
  FILE *stream = arg;
  ಪೂರ್ಣಾಙ್ಕ tty = isatty (fileno (stream));
  ಧ್ರುವ ವರ್ಣ *color_begin = "";
  ಧ್ರುವ ವರ್ಣ *color_end = "";

  ಯದಿ (tty)
    {
      ವಿಕಲ್ಪ (verbosity)
        {
        ಪಕ್ಷ LIBCRUN_VERBOSITY_DEBUG:
          color_begin = "\x1b[1;34m";
          color_end = "\x1b[0m";
          ಭಙ್ಗ;
        ಪಕ್ಷ LIBCRUN_VERBOSITY_WARNING:
          color_begin = "\x1b[1;33m";
          color_end = "\x1b[0m";
          ಭಙ್ಗ;
        ಪಕ್ಷ LIBCRUN_VERBOSITY_ERROR:
          color_begin = "\x1b[1;31m";
          color_end = "\x1b[0m";
          ಭಙ್ಗ;
        }

      ಯದಿ (log_format == LOG_FORMAT_TEXT)
        get_timestamp (&timestamp, ": ");
    }

  ಯದಿ (errno_)
    fprintf (stream, "%s%s%s: %s%s\n", color_begin, timestamp, ಸಂದೇಶ, strerror (errno_), color_end);
  ಅನ್ಯಥಾ
    fprintf (stream, "%s%s%s%s\n", color_begin, timestamp, ಸಂದೇಶ, color_end);
}

ಶೂನ್ಯ
log_write_to_stderr (ಪೂರ್ಣಾಙ್ಕ errno_, ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ, ಪೂರ್ಣಾಙ್ಕ verbosity, ಶೂನ್ಯ *arg arg_unused)
{
  log_write_to_stream (errno_, ಸಂದೇಶ, verbosity, stderr);
}

ಶೂನ್ಯ
log_write_to_syslog (ಪೂರ್ಣಾಙ್ಕ errno_, ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ, ಪೂರ್ಣಾಙ್ಕ verbosity, ಶೂನ್ಯ *arg arg_unused)
{
  ಪೂರ್ಣಾಙ್ಕ priority = LOG_ERR;
  ವಿಕಲ್ಪ (verbosity)
    {
    ಪಕ್ಷ LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      ಭಙ್ಗ;
    ಪಕ್ಷ LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      ಭಙ್ಗ;
    ಪಕ್ಷ LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      ಭಙ್ಗ;
    }
  ಯದಿ (errno_ == 0)
    syslog (priority, "%s", ಸಂದೇಶ);
  ಅನ್ಯಥಾ
    syslog (priority, "%s: %s", ಸಂದೇಶ, strerror (errno_));
}

ಶೂನ್ಯ
log_write_to_journald (ಪೂರ್ಣಾಙ್ಕ errno_, ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ, ಪೂರ್ಣಾಙ್ಕ verbosity, ಶೂನ್ಯ *arg arg_unused)
{
  (ಶೂನ್ಯ) errno_;
  (ಶೂನ್ಯ) ಸಂದೇಶ;
  (ಶೂನ್ಯ) verbosity;
#ifdef HAVE_SYSTEMD
  ಪೂರ್ಣಾಙ್ಕ priority = LOG_ERR;
  ವಿಕಲ್ಪ (verbosity)
    {
    ಪಕ್ಷ LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      ಭಙ್ಗ;
    ಪಕ್ಷ LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      ಭಙ್ಗ;
    ಪಕ್ಷ LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      ಭಙ್ಗ;
    }
  ಯದಿ (errno_ == 0)
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s", ಸಂದೇಶ, "ID=%s", arg, ರಿಕ್ತ);
  ಅನ್ಯಥಾ
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s: %s", ಸಂದೇಶ, strerror (errno_), "ID=%s",
                     arg, ರಿಕ್ತ);
#endif
}

ಸ್ಥಿರ crun_output_handler output_handler = log_write_to_stderr;
ಸ್ಥಿರ ಶೂನ್ಯ *output_handler_arg = ರಿಕ್ತ;

ಶೂನ್ಯ
libcrun_set_verbosity (ಪೂರ್ಣಾಙ್ಕ verbosity)
{
  output_verbosity = verbosity;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_get_verbosity ()
{
  ಪ್ರತ್ಯಾಗಮ output_verbosity;
}

ಶೂನ್ಯ
crun_set_output_handler (crun_output_handler handler, ಶೂನ್ಯ *arg)
{
  output_handler = handler;
  output_handler_arg = arg;
}

ಸ್ಥಿರ ವರ್ಣ *
make_json_error (ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ, ಪೂರ್ಣಾಙ್ಕ errno_, ಪೂರ್ಣಾಙ್ಕ verbosity)
{
  ಧ್ರುವ ವರ್ಣ *level;
  ವಿಕಲ್ಪ (verbosity)
    {
    ಪಕ್ಷ LIBCRUN_VERBOSITY_DEBUG:
      level = "debug";
      ಭಙ್ಗ;
    ಪಕ್ಷ LIBCRUN_VERBOSITY_WARNING:
      level = "warning";
      ಭಙ್ಗ;
    ಪಕ್ಷ LIBCRUN_VERBOSITY_ERROR:
      level = "error";
      ಭಙ್ಗ;
    }
  ಧ್ರುವ ವರ್ಣ *buf = ರಿಕ್ತ;
  json_gen_ctx *gen = ರಿಕ್ತ;
  ವರ್ಣ *ret = ರಿಕ್ತ;
  size_t buf_len;
  timestamp_t timestamp = {
    0,
  };

  ಯದಿ (! json_gen_init (&gen, ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  get_timestamp (&timestamp, "");

  json_gen_map_open (gen);

  json_gen_string (gen, "msg", strlen ("msg"));
  ಯದಿ (errno_ == 0)
    json_gen_string (gen, ಸಂದೇಶ, strlen (ಸಂದೇಶ));
  ಅನ್ಯಥಾ
    {
      cleanup_free ವರ್ಣ *tmp = ರಿಕ್ತ;

      xasprintf (&tmp, "%s: %s", ಸಂದೇಶ, strerror (errno_));
      json_gen_string (gen, tmp, strlen (tmp));
    }

  json_gen_string (gen, "level", strlen ("level"));
  json_gen_string (gen, level, strlen (level));

  json_gen_string (gen, "time", strlen ("time"));
  json_gen_string (gen, timestamp, strlen (timestamp));

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);
  ಯದಿ (buf)
    ret = strdup (buf);

  json_gen_free (gen);

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ
write_log (ಪೂರ್ಣಾಙ್ಕ errno_, ಪೂರ್ಣಾಙ್ಕ verbosity, ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ, va_list args_list)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  cleanup_free ವರ್ಣ *output = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *json = ರಿಕ್ತ;

  ಯದಿ (verbosity > output_verbosity)
    ಪ್ರತ್ಯಾಗಮ;

  ret = vasprintf (&output, ಸಂದೇಶ, args_list);
  ಯದಿ (UNLIKELY (ret < 0))
    OOM ();

  ಯದಿ (verbosity == LIBCRUN_VERBOSITY_ERROR && output_handler != log_write_to_stderr)
    log_write_to_stderr (errno_, output, LIBCRUN_VERBOSITY_ERROR, ರಿಕ್ತ);

  ವಿಕಲ್ಪ (log_format)
    {
    ಪಕ್ಷ LOG_FORMAT_TEXT:
      output_handler (errno_, output, verbosity, output_handler_arg);
      ಭಙ್ಗ;

    ಪಕ್ಷ LOG_FORMAT_JSON:
      json = make_json_error (output, errno_, verbosity);
      ಯದಿ (json)
        output_handler (0, json, verbosity, output_handler_arg);
      ಅನ್ಯಥಾ
        output_handler (errno_, output, verbosity, output_handler_arg);
      ಭಙ್ಗ;
    }
}

ಶೂನ್ಯ
libcrun_debug (ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ, ...)
{
  va_list args_list;
  va_start (args_list, ಸಂದೇಶ);
  write_log (0, LIBCRUN_VERBOSITY_DEBUG, ಸಂದೇಶ, args_list);
  va_end (args_list);
}

ಶೂನ್ಯ
libcrun_warning (ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ, ...)
{
  va_list args_list;
  va_start (args_list, ಸಂದೇಶ);
  write_log (0, LIBCRUN_VERBOSITY_WARNING, ಸಂದೇಶ, args_list);
  va_end (args_list);
}

ಶೂನ್ಯ
libcrun_error (ಪೂರ್ಣಾಙ್ಕ errno_, ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ, ...)
{
  va_list args_list;
  va_start (args_list, ಸಂದೇಶ);

  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, ಸಂದೇಶ, args_list);
  va_end (args_list);
}

ಶೂನ್ಯ __attribute__ ((noreturn))
libcrun_fail_with_error (ಪೂರ್ಣಾಙ್ಕ errno_, ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ, ...)
{
  va_list args_list;
  va_start (args_list, ಸಂದೇಶ);
  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, ಸಂದೇಶ, args_list);
  va_end (args_list);
  exit (EXIT_FAILURE);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_set_log_format (ಧ್ರುವ ವರ್ಣ *format, libcrun_error_t *err)
{
  ಯದಿ (strcmp (format, "text") == 0)
    log_format = LOG_FORMAT_TEXT;
  ಅನ್ಯಥಾ ಯದಿ (strcmp (format, "json") == 0)
    log_format = LOG_FORMAT_JSON;
  ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "unknown log format type `%s`", format);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
json_gen_error_to_crun_error (ಪೂರ್ಣಾಙ್ಕ status, libcrun_error_t *err)
{
  ವಿಕಲ್ಪ (status)
    {
    ಪಕ್ಷ json_gen_status_ok:
      ಪ್ರತ್ಯಾಗಮ 0;

    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "generate JSON document: error %d", status);
    }
}
