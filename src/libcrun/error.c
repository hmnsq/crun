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
#include <config.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include "utils.h"

#include <json-c/json.h>
#include <ocispec/json_common.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-journal.h>
#endif

enum
{
  LOG_FORMAT_TEXT = 0,
  LOG_FORMAT_JSON,
};

static цяло log_format;
static цяло output_verbosity = LIBCRUN_VERBOSITY_ERROR;

цяло
libcrun_make_error (libcrun_error_t *err, цяло status, const char *msg, ...)
{
  va_list args_list;
  libcrun_error_t ptr;

  va_start (args_list, msg);
  *err = xmalloc (sizeof (struct libcrun_error_s));
  ptr = *err;
  ptr->status = status;
  ако (vasprintf (&(ptr->msg), msg, args_list) < 0)
    OOM ();
  va_end (args_list);

  върни -status - 1;
}

цяло
crun_error_wrap (libcrun_error_t *err, const char *fmt, ...)
{
  cleanup_free char *msg = NULL;
  cleanup_free char *tmp = NULL;
  va_list args_list;
  char *swap;
  цяло ret;

  ако (err == NULL || *err == NULL)
    {
      // Internal error
      върни 0;
    }

  ret = -(*err)->status - 1;

  va_start (args_list, fmt);

  ако (vasprintf (&msg, fmt, args_list) < 0)
    {
      va_end (args_list);
      msg = NULL;
      върни ret;
    }
  va_end (args_list);

  xasprintf (&tmp, "%s: %s", msg, (*err)->msg);
  swap = tmp;
  tmp = (*err)->msg;
  (*err)->msg = swap;

  върни ret;
}

цяло
crun_error_release (libcrun_error_t *err)
{
  const цяло saved_errno = errno;
  libcrun_error_t ptr;

  ако (err == NULL)
    върни 0;

  ptr = *err;
  ако (ptr == NULL)
    върни 0;

  free (ptr->msg);
  free (ptr);
  *err = NULL;

  errno = saved_errno;
  върни 0;
}

цяло
libcrun_error_release (libcrun_error_t *err)
{
  върни crun_error_release (err);
}

цяло
libcrun_error_get_status (libcrun_error_t err)
{
  ако (err == NULL)
    върни 0;
  върни err->status;
}

const char *
libcrun_error_get_message (libcrun_error_t err)
{
  ако (err == NULL)
    върни NULL;
  върни err->msg;
}

void
crun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  const цяло saved_errno = errno;
  libcrun_error_t ref;

  ако (out == NULL)
    out = stderr;
  ако (err == NULL || *err == NULL || **err == NULL)
    {
      // Internal error
      върни;
    }

  ref = **err;
  ако (ref->status)
    fprintf (out, "%s: %s\n", ref->msg, strerror (ref->status));
  иначе
    fprintf (out, "%s\n", ref->msg);

  free (ref->msg);
  free (ref);
  **err = NULL;

  errno = saved_errno;
}

void
libcrun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  върни crun_error_write_warning_and_release (out, err);
}

void
libcrun_error_report_and_release (libcrun_error_t *err)
{
  libcrun_error_t ref;

  ако (err == NULL || *err == NULL)
    върни;

  ref = *err;
  libcrun_error (ref->status, "%s", ref->msg);
  crun_error_release (err);
}

цяло
crun_error_get_errno (libcrun_error_t *err)
{
  ако (err == NULL || *err == NULL)
    върни 0;
  върни (*err)->status;
}

typedef char timestamp_t[64];

static void
get_timestamp (timestamp_t *timestamp, const char *suffix)
{
  const size_t buffer_size = sizeof (timestamp_t);
  char *buffer = (char *) timestamp;
  struct timeval tv;
  size_t written;
  struct tm now;

  gettimeofday (&tv, NULL);
  gmtime_r (&tv.tv_sec, &now);
  written = strftime (buffer, buffer_size, "%Y-%m-%dT%H:%M:%S", &now);
  snprintf (buffer + written, buffer_size - written, ".%06lldZ%.8s", (long long цяло) tv.tv_usec, suffix);
}

/* Neither openlog(3) nor the journald handler copy the identifier they are
   given, and the global output handler outlives the caller's strings (e.g.
   the context is freed while errors are still reported through it), so keep
   an owned copy alive for as long as the handler can use it.  */
static char *log_id;

static const char *
set_log_id (const char *id, char **old_id)
{
  *old_id = log_id;
  log_id = id ? xstrdup (id) : NULL;
  върни log_id;
}

static void *
init_syslog (const char *id)
{
  openlog (id, 0, LOG_USER);
  върни NULL;
}

enum
{
  LOG_TYPE_FILE = 1,
  LOG_TYPE_SYSLOG = 2,
  LOG_TYPE_JOURNALD = 3
};

static цяло
get_log_type (const char *log, const char **data)
{
  const char *sep = strchr (log, ':');
  ако (sep == NULL)
    {
      *data = log;
      върни LOG_TYPE_FILE;
    }
  *data = sep + 1;

  ако (has_prefix (log, "syslog:"))
    върни LOG_TYPE_SYSLOG;
  ако (has_prefix (log, "journald:"))
    върни LOG_TYPE_JOURNALD;
  ако (has_prefix (log, "file:"))
    върни LOG_TYPE_FILE;

  върни -1;
}

цяло
libcrun_init_logging (crun_output_handler *new_output_handler, void **new_output_handler_arg, const char *id,
                      const char *log, libcrun_error_t *err)
{
  /* Released only after the new handler is installed.  */
  cleanup_free char *old_log_id = NULL;

  ако (log == NULL)
    {
      *new_output_handler = log_write_to_stderr;
      *new_output_handler_arg = NULL;
    }
  иначе
    {
      const char *arg = NULL;
      цяло log_type = get_log_type (log, &arg);

      ако (log_type < 0)
        върни crun_make_error (err, errno, "unknown log type `%s`", log);

      switch (log_type)
        {
        case LOG_TYPE_FILE:
          *new_output_handler = log_write_to_stream;
          *new_output_handler_arg = fopen (arg, "a+e");
          ако (*new_output_handler_arg == NULL)
            върни crun_make_error (err, errno, "open log file `%s`", log);
          ако (output_verbosity >= LIBCRUN_VERBOSITY_WARNING)
            setlinebuf (*new_output_handler_arg);
          break;

        case LOG_TYPE_SYSLOG:
          *new_output_handler_arg = init_syslog (set_log_id (arg[0] ? arg : id, &old_log_id));
          *new_output_handler = log_write_to_syslog;
          break;

        case LOG_TYPE_JOURNALD:
          *new_output_handler = log_write_to_journald;
          *new_output_handler_arg = (void *) set_log_id (id, &old_log_id);
          break;
        }
    }
  crun_set_output_handler (*new_output_handler, *new_output_handler_arg);
  върни 0;
}

void
log_write_to_stream (цяло errno_, const char *msg, цяло verbosity, void *arg)
{
  timestamp_t timestamp = {
    0,
  };
  FILE *stream = arg;
  цяло tty = isatty (fileno (stream));
  const char *color_begin = "";
  const char *color_end = "";

  ако (tty)
    {
      switch (verbosity)
        {
        case LIBCRUN_VERBOSITY_DEBUG:
          color_begin = "\x1b[1;34m";
          color_end = "\x1b[0m";
          break;
        case LIBCRUN_VERBOSITY_WARNING:
          color_begin = "\x1b[1;33m";
          color_end = "\x1b[0m";
          break;
        case LIBCRUN_VERBOSITY_ERROR:
          color_begin = "\x1b[1;31m";
          color_end = "\x1b[0m";
          break;
        }

      ако (log_format == LOG_FORMAT_TEXT)
        get_timestamp (&timestamp, ": ");
    }

  ако (errno_)
    fprintf (stream, "%s%s%s: %s%s\n", color_begin, timestamp, msg, strerror (errno_), color_end);
  иначе
    fprintf (stream, "%s%s%s%s\n", color_begin, timestamp, msg, color_end);
}

void
log_write_to_stderr (цяло errno_, const char *msg, цяло verbosity, void *arg arg_unused)
{
  log_write_to_stream (errno_, msg, verbosity, stderr);
}

void
log_write_to_syslog (цяло errno_, const char *msg, цяло verbosity, void *arg arg_unused)
{
  цяло priority = LOG_ERR;
  switch (verbosity)
    {
    case LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      break;
    case LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      break;
    case LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      break;
    }
  ако (errno_ == 0)
    syslog (priority, "%s", msg);
  иначе
    syslog (priority, "%s: %s", msg, strerror (errno_));
}

void
log_write_to_journald (цяло errno_, const char *msg, цяло verbosity, void *arg arg_unused)
{
  (void) errno_;
  (void) msg;
  (void) verbosity;
#ifdef HAVE_SYSTEMD
  цяло priority = LOG_ERR;
  switch (verbosity)
    {
    case LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      break;
    case LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      break;
    case LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      break;
    }
  ако (errno_ == 0)
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s", msg, "ID=%s", arg, NULL);
  иначе
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s: %s", msg, strerror (errno_), "ID=%s",
                     arg, NULL);
#endif
}

static crun_output_handler output_handler = log_write_to_stderr;
static void *output_handler_arg = NULL;

void
libcrun_set_verbosity (цяло verbosity)
{
  output_verbosity = verbosity;
}

цяло
libcrun_get_verbosity ()
{
  върни output_verbosity;
}

void
crun_set_output_handler (crun_output_handler handler, void *arg)
{
  output_handler = handler;
  output_handler_arg = arg;
}

static char *
make_json_error (const char *msg, цяло errno_, цяло verbosity)
{
  const char *level;
  switch (verbosity)
    {
    case LIBCRUN_VERBOSITY_DEBUG:
      level = "debug";
      break;
    case LIBCRUN_VERBOSITY_WARNING:
      level = "warning";
      break;
    case LIBCRUN_VERBOSITY_ERROR:
      level = "error";
      break;
    }
  const char *buf = NULL;
  json_gen_ctx *gen = NULL;
  char *ret = NULL;
  size_t buf_len;
  timestamp_t timestamp = {
    0,
  };

  ако (! json_gen_init (&gen, NULL))
    върни NULL;

  get_timestamp (&timestamp, "");

  json_gen_map_open (gen);

  json_gen_string (gen, "msg", strlen ("msg"));
  ако (errno_ == 0)
    json_gen_string (gen, msg, strlen (msg));
  иначе
    {
      cleanup_free char *tmp = NULL;

      xasprintf (&tmp, "%s: %s", msg, strerror (errno_));
      json_gen_string (gen, tmp, strlen (tmp));
    }

  json_gen_string (gen, "level", strlen ("level"));
  json_gen_string (gen, level, strlen (level));

  json_gen_string (gen, "time", strlen ("time"));
  json_gen_string (gen, timestamp, strlen (timestamp));

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);
  ако (buf)
    ret = strdup (buf);

  json_gen_free (gen);

  върни ret;
}

static void
write_log (цяло errno_, цяло verbosity, const char *msg, va_list args_list)
{
  цяло ret;
  cleanup_free char *output = NULL;
  cleanup_free char *json = NULL;

  ако (verbosity > output_verbosity)
    върни;

  ret = vasprintf (&output, msg, args_list);
  ако (UNLIKELY (ret < 0))
    OOM ();

  ако (verbosity == LIBCRUN_VERBOSITY_ERROR && output_handler != log_write_to_stderr)
    log_write_to_stderr (errno_, output, LIBCRUN_VERBOSITY_ERROR, NULL);

  switch (log_format)
    {
    case LOG_FORMAT_TEXT:
      output_handler (errno_, output, verbosity, output_handler_arg);
      break;

    case LOG_FORMAT_JSON:
      json = make_json_error (output, errno_, verbosity);
      ако (json)
        output_handler (0, json, verbosity, output_handler_arg);
      иначе
        output_handler (errno_, output, verbosity, output_handler_arg);
      break;
    }
}

void
libcrun_debug (const char *msg, ...)
{
  va_list args_list;
  va_start (args_list, msg);
  write_log (0, LIBCRUN_VERBOSITY_DEBUG, msg, args_list);
  va_end (args_list);
}

void
libcrun_warning (const char *msg, ...)
{
  va_list args_list;
  va_start (args_list, msg);
  write_log (0, LIBCRUN_VERBOSITY_WARNING, msg, args_list);
  va_end (args_list);
}

void
libcrun_error (цяло errno_, const char *msg, ...)
{
  va_list args_list;
  va_start (args_list, msg);

  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, msg, args_list);
  va_end (args_list);
}

void __attribute__ ((noreturn))
libcrun_fail_with_error (цяло errno_, const char *msg, ...)
{
  va_list args_list;
  va_start (args_list, msg);
  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, msg, args_list);
  va_end (args_list);
  exit (EXIT_FAILURE);
}

цяло
libcrun_set_log_format (const char *format, libcrun_error_t *err)
{
  ако (strcmp (format, "text") == 0)
    log_format = LOG_FORMAT_TEXT;
  иначе ако (strcmp (format, "json") == 0)
    log_format = LOG_FORMAT_JSON;
  иначе
    върни crun_make_error (err, 0, "unknown log format type `%s`", format);

  върни 0;
}

цяло
json_gen_error_to_crun_error (цяло status, libcrun_error_t *err)
{
  switch (status)
    {
    case json_gen_status_ok:
      върни 0;

    default:
      върни crun_make_error (err, 0, "generate JSON document: error %d", status);
    }
}
