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

ga_nanaa
{
  LOG_FORMAT_TEXT = 0,
  LOG_FORMAT_JSON,
};

static ကိန်းပြည့် log_format;
static ကိန်းပြည့် output_verbosity = LIBCRUN_VERBOSITY_ERROR;

ကိန်းပြည့်
libcrun_make_error (libcrun_error_t *err, ကိန်းပြည့် status, const var_na *msg, ...)
{
  va_list args_list;
  libcrun_error_t ptr;

  va_start (args_list, msg);
  *err = xmalloc (parimaa_na (sa_nracanaa libcrun_error_s));
  ptr = *err;
  ptr->status = status;
  အကယ်၍ (vasprintf (&(ptr->msg), msg, args_list) < 0)
    OOM ();
  va_end (args_list);

  ပြန်ပေး -status - 1;
}

ကိန်းပြည့်
crun_error_wrap (libcrun_error_t *err, const var_na *fmt, ...)
{
  cleanup_free var_na *msg = NULL;
  cleanup_free var_na *tmp = NULL;
  va_list args_list;
  var_na *swap;
  ကိန်းပြည့် ret;

  အကယ်၍ (err == NULL || *err == NULL)
    {
      // Internal error
      ပြန်ပေး 0;
    }

  ret = -(*err)->status - 1;

  va_start (args_list, fmt);

  အကယ်၍ (vasprintf (&msg, fmt, args_list) < 0)
    {
      va_end (args_list);
      msg = NULL;
      ပြန်ပေး ret;
    }
  va_end (args_list);

  xasprintf (&tmp, "%s: %s", msg, (*err)->msg);
  swap = tmp;
  tmp = (*err)->msg;
  (*err)->msg = swap;

  ပြန်ပေး ret;
}

ကိန်းပြည့်
crun_error_release (libcrun_error_t *err)
{
  const ကိန်းပြည့် saved_errno = errno;
  libcrun_error_t ptr;

  အကယ်၍ (err == NULL)
    ပြန်ပေး 0;

  ptr = *err;
  အကယ်၍ (ptr == NULL)
    ပြန်ပေး 0;

  free (ptr->msg);
  free (ptr);
  *err = NULL;

  errno = saved_errno;
  ပြန်ပေး 0;
}

ကိန်းပြည့်
libcrun_error_release (libcrun_error_t *err)
{
  ပြန်ပေး crun_error_release (err);
}

ကိန်းပြည့်
libcrun_error_get_status (libcrun_error_t err)
{
  အကယ်၍ (err == NULL)
    ပြန်ပေး 0;
  ပြန်ပေး err->status;
}

const var_na *
libcrun_error_get_message (libcrun_error_t err)
{
  အကယ်၍ (err == NULL)
    ပြန်ပေး NULL;
  ပြန်ပေး err->msg;
}

void
crun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  const ကိန်းပြည့် saved_errno = errno;
  libcrun_error_t ref;

  အကယ်၍ (out == NULL)
    out = stderr;
  အကယ်၍ (err == NULL || *err == NULL || **err == NULL)
    {
      // Internal error
      ပြန်ပေး;
    }

  ref = **err;
  အကယ်၍ (ref->status)
    fprintf (out, "%s: %s\n", ref->msg, strerror (ref->status));
  မဟုတ်ပါက
    fprintf (out, "%s\n", ref->msg);

  free (ref->msg);
  free (ref);
  **err = NULL;

  errno = saved_errno;
}

void
libcrun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  ပြန်ပေး crun_error_write_warning_and_release (out, err);
}

void
libcrun_error_report_and_release (libcrun_error_t *err)
{
  libcrun_error_t ref;

  အကယ်၍ (err == NULL || *err == NULL)
    ပြန်ပေး;

  ref = *err;
  libcrun_error (ref->status, "%s", ref->msg);
  crun_error_release (err);
}

ကိန်းပြည့်
crun_error_get_errno (libcrun_error_t *err)
{
  အကယ်၍ (err == NULL || *err == NULL)
    ပြန်ပေး 0;
  ပြန်ပေး (*err)->status;
}

typedef var_na timestamp_t[64];

static void
get_timestamp (timestamp_t *timestamp, const var_na *suffix)
{
  const size_t buffer_size = parimaa_na (timestamp_t);
  var_na *buffer = (var_na *) timestamp;
  sa_nracanaa timeval tv;
  size_t written;
  sa_nracanaa tm now;

  gettimeofday (&tv, NULL);
  gmtime_r (&tv.tv_sec, &now);
  written = strftime (buffer, buffer_size, "%Y-%m-%dT%H:%M:%S", &now);
  snprintf (buffer + written, buffer_size - written, ".%06lldZ%.8s", (long long ကိန်းပြည့်) tv.tv_usec, suffix);
}

/* Neither openlog(3) nor the journald handler copy the identifier they are
   given, and the global output handler outlives the caller's strings (e.g.
   the context is freed while errors are still reported through it), so keep
   an owned copy alive for as long as the handler can use it.  */
static var_na *log_id;

static const var_na *
set_log_id (const var_na *id, var_na **old_id)
{
  *old_id = log_id;
  log_id = id ? xstrdup (id) : NULL;
  ပြန်ပေး log_id;
}

static void *
init_syslog (const var_na *id)
{
  openlog (id, 0, LOG_USER);
  ပြန်ပေး NULL;
}

ga_nanaa
{
  LOG_TYPE_FILE = 1,
  LOG_TYPE_SYSLOG = 2,
  LOG_TYPE_JOURNALD = 3
};

static ကိန်းပြည့်
get_log_type (const var_na *log, const var_na **data)
{
  const var_na *sep = strchr (log, ':');
  အကယ်၍ (sep == NULL)
    {
      *data = log;
      ပြန်ပေး LOG_TYPE_FILE;
    }
  *data = sep + 1;

  အကယ်၍ (has_prefix (log, "syslog:"))
    ပြန်ပေး LOG_TYPE_SYSLOG;
  အကယ်၍ (has_prefix (log, "journald:"))
    ပြန်ပေး LOG_TYPE_JOURNALD;
  အကယ်၍ (has_prefix (log, "file:"))
    ပြန်ပေး LOG_TYPE_FILE;

  ပြန်ပေး -1;
}

ကိန်းပြည့်
libcrun_init_logging (crun_output_handler *new_output_handler, void **new_output_handler_arg, const var_na *id,
                      const var_na *log, libcrun_error_t *err)
{
  /* Released only after the new handler is installed.  */
  cleanup_free var_na *old_log_id = NULL;

  အကယ်၍ (log == NULL)
    {
      *new_output_handler = log_write_to_stderr;
      *new_output_handler_arg = NULL;
    }
  မဟုတ်ပါက
    {
      const var_na *arg = NULL;
      ကိန်းပြည့် log_type = get_log_type (log, &arg);

      အကယ်၍ (log_type < 0)
        ပြန်ပေး crun_make_error (err, errno, "unknown log type `%s`", log);

      switch (log_type)
        {
        pak_sa LOG_TYPE_FILE:
          *new_output_handler = log_write_to_stream;
          *new_output_handler_arg = fopen (arg, "a+e");
          အကယ်၍ (*new_output_handler_arg == NULL)
            ပြန်ပေး crun_make_error (err, errno, "open log file `%s`", log);
          အကယ်၍ (output_verbosity >= LIBCRUN_VERBOSITY_WARNING)
            setlinebuf (*new_output_handler_arg);
          bha_nga;

        pak_sa LOG_TYPE_SYSLOG:
          *new_output_handler_arg = init_syslog (set_log_id (arg[0] ? arg : id, &old_log_id));
          *new_output_handler = log_write_to_syslog;
          bha_nga;

        pak_sa LOG_TYPE_JOURNALD:
          *new_output_handler = log_write_to_journald;
          *new_output_handler_arg = (void *) set_log_id (id, &old_log_id);
          bha_nga;
        }
    }
  crun_set_output_handler (*new_output_handler, *new_output_handler_arg);
  ပြန်ပေး 0;
}

void
log_write_to_stream (ကိန်းပြည့် errno_, const var_na *msg, ကိန်းပြည့် verbosity, void *arg)
{
  timestamp_t timestamp = {
    0,
  };
  FILE *stream = arg;
  ကိန်းပြည့် tty = isatty (fileno (stream));
  const var_na *color_begin = "";
  const var_na *color_end = "";

  အကယ်၍ (tty)
    {
      switch (verbosity)
        {
        pak_sa LIBCRUN_VERBOSITY_DEBUG:
          color_begin = "\x1b[1;34m";
          color_end = "\x1b[0m";
          bha_nga;
        pak_sa LIBCRUN_VERBOSITY_WARNING:
          color_begin = "\x1b[1;33m";
          color_end = "\x1b[0m";
          bha_nga;
        pak_sa LIBCRUN_VERBOSITY_ERROR:
          color_begin = "\x1b[1;31m";
          color_end = "\x1b[0m";
          bha_nga;
        }

      အကယ်၍ (log_format == LOG_FORMAT_TEXT)
        get_timestamp (&timestamp, ": ");
    }

  အကယ်၍ (errno_)
    fprintf (stream, "%s%s%s: %s%s\n", color_begin, timestamp, msg, strerror (errno_), color_end);
  မဟုတ်ပါက
    fprintf (stream, "%s%s%s%s\n", color_begin, timestamp, msg, color_end);
}

void
log_write_to_stderr (ကိန်းပြည့် errno_, const var_na *msg, ကိန်းပြည့် verbosity, void *arg arg_unused)
{
  log_write_to_stream (errno_, msg, verbosity, stderr);
}

void
log_write_to_syslog (ကိန်းပြည့် errno_, const var_na *msg, ကိန်းပြည့် verbosity, void *arg arg_unused)
{
  ကိန်းပြည့် priority = LOG_ERR;
  switch (verbosity)
    {
    pak_sa LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      bha_nga;
    pak_sa LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      bha_nga;
    pak_sa LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      bha_nga;
    }
  အကယ်၍ (errno_ == 0)
    syslog (priority, "%s", msg);
  မဟုတ်ပါက
    syslog (priority, "%s: %s", msg, strerror (errno_));
}

void
log_write_to_journald (ကိန်းပြည့် errno_, const var_na *msg, ကိန်းပြည့် verbosity, void *arg arg_unused)
{
  (void) errno_;
  (void) msg;
  (void) verbosity;
#ifdef HAVE_SYSTEMD
  ကိန်းပြည့် priority = LOG_ERR;
  switch (verbosity)
    {
    pak_sa LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      bha_nga;
    pak_sa LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      bha_nga;
    pak_sa LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      bha_nga;
    }
  အကယ်၍ (errno_ == 0)
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s", msg, "ID=%s", arg, NULL);
  မဟုတ်ပါက
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s: %s", msg, strerror (errno_), "ID=%s",
                     arg, NULL);
#endif
}

static crun_output_handler output_handler = log_write_to_stderr;
static void *output_handler_arg = NULL;

void
libcrun_set_verbosity (ကိန်းပြည့် verbosity)
{
  output_verbosity = verbosity;
}

ကိန်းပြည့်
libcrun_get_verbosity ()
{
  ပြန်ပေး output_verbosity;
}

void
crun_set_output_handler (crun_output_handler handler, void *arg)
{
  output_handler = handler;
  output_handler_arg = arg;
}

static var_na *
make_json_error (const var_na *msg, ကိန်းပြည့် errno_, ကိန်းပြည့် verbosity)
{
  const var_na *level;
  switch (verbosity)
    {
    pak_sa LIBCRUN_VERBOSITY_DEBUG:
      level = "debug";
      bha_nga;
    pak_sa LIBCRUN_VERBOSITY_WARNING:
      level = "warning";
      bha_nga;
    pak_sa LIBCRUN_VERBOSITY_ERROR:
      level = "error";
      bha_nga;
    }
  const var_na *buf = NULL;
  json_gen_ctx *gen = NULL;
  var_na *ret = NULL;
  size_t buf_len;
  timestamp_t timestamp = {
    0,
  };

  အကယ်၍ (! json_gen_init (&gen, NULL))
    ပြန်ပေး NULL;

  get_timestamp (&timestamp, "");

  json_gen_map_open (gen);

  json_gen_string (gen, "msg", strlen ("msg"));
  အကယ်၍ (errno_ == 0)
    json_gen_string (gen, msg, strlen (msg));
  မဟုတ်ပါက
    {
      cleanup_free var_na *tmp = NULL;

      xasprintf (&tmp, "%s: %s", msg, strerror (errno_));
      json_gen_string (gen, tmp, strlen (tmp));
    }

  json_gen_string (gen, "level", strlen ("level"));
  json_gen_string (gen, level, strlen (level));

  json_gen_string (gen, "time", strlen ("time"));
  json_gen_string (gen, timestamp, strlen (timestamp));

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);
  အကယ်၍ (buf)
    ret = strdup (buf);

  json_gen_free (gen);

  ပြန်ပေး ret;
}

static void
write_log (ကိန်းပြည့် errno_, ကိန်းပြည့် verbosity, const var_na *msg, va_list args_list)
{
  ကိန်းပြည့် ret;
  cleanup_free var_na *output = NULL;
  cleanup_free var_na *json = NULL;

  အကယ်၍ (verbosity > output_verbosity)
    ပြန်ပေး;

  ret = vasprintf (&output, msg, args_list);
  အကယ်၍ (UNLIKELY (ret < 0))
    OOM ();

  အကယ်၍ (verbosity == LIBCRUN_VERBOSITY_ERROR && output_handler != log_write_to_stderr)
    log_write_to_stderr (errno_, output, LIBCRUN_VERBOSITY_ERROR, NULL);

  switch (log_format)
    {
    pak_sa LOG_FORMAT_TEXT:
      output_handler (errno_, output, verbosity, output_handler_arg);
      bha_nga;

    pak_sa LOG_FORMAT_JSON:
      json = make_json_error (output, errno_, verbosity);
      အကယ်၍ (json)
        output_handler (0, json, verbosity, output_handler_arg);
      မဟုတ်ပါက
        output_handler (errno_, output, verbosity, output_handler_arg);
      bha_nga;
    }
}

void
libcrun_debug (const var_na *msg, ...)
{
  va_list args_list;
  va_start (args_list, msg);
  write_log (0, LIBCRUN_VERBOSITY_DEBUG, msg, args_list);
  va_end (args_list);
}

void
libcrun_warning (const var_na *msg, ...)
{
  va_list args_list;
  va_start (args_list, msg);
  write_log (0, LIBCRUN_VERBOSITY_WARNING, msg, args_list);
  va_end (args_list);
}

void
libcrun_error (ကိန်းပြည့် errno_, const var_na *msg, ...)
{
  va_list args_list;
  va_start (args_list, msg);

  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, msg, args_list);
  va_end (args_list);
}

void __attribute__ ((noreturn))
libcrun_fail_with_error (ကိန်းပြည့် errno_, const var_na *msg, ...)
{
  va_list args_list;
  va_start (args_list, msg);
  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, msg, args_list);
  va_end (args_list);
  exit (EXIT_FAILURE);
}

ကိန်းပြည့်
libcrun_set_log_format (const var_na *format, libcrun_error_t *err)
{
  အကယ်၍ (strcmp (format, "text") == 0)
    log_format = LOG_FORMAT_TEXT;
  မဟုတ်ပါက အကယ်၍ (strcmp (format, "json") == 0)
    log_format = LOG_FORMAT_JSON;
  မဟုတ်ပါက
    ပြန်ပေး crun_make_error (err, 0, "unknown log format type `%s`", format);

  ပြန်ပေး 0;
}

ကိန်းပြည့်
json_gen_error_to_crun_error (ကိန်းပြည့် status, libcrun_error_t *err)
{
  switch (status)
    {
    pak_sa json_gen_status_ok:
      ပြန်ပေး 0;

    default:
      ပြန်ပေး crun_make_error (err, 0, "generate JSON document: error %d", status);
    }
}
