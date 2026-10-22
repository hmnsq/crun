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
#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdarg.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <time.h>
#அந்தர்பாவ <sys/time.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ "utils.h"

#அந்தர்பாவ <json-c/json.h>
#அந்தர்பாவ <ocispec/json_common.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-journal.h>
#endif

எண்ணிக்கை
{
  LOG_FORMAT_TEXT = 0,
  LOG_FORMAT_JSON,
};

நிலையான முழு log_format;
நிலையான முழு output_verbosity = LIBCRUN_VERBOSITY_ERROR;

முழு
libcrun_make_error (libcrun_error_t *err, முழு status, மாறிலி எழுத்து *செய்தி, ...)
{
  va_list args_list;
  libcrun_error_t ptr;

  va_start (args_list, செய்தி);
  *err = xmalloc (அளவு (கட்டமைப்பு libcrun_error_s));
  ptr = *err;
  ptr->status = status;
  எனில் (vasprintf (&(ptr->செய்தி), செய்தி, args_list) < 0)
    OOM ();
  va_end (args_list);

  திரும்பு -status - 1;
}

முழு
crun_error_wrap (libcrun_error_t *err, மாறிலி எழுத்து *fmt, ...)
{
  cleanup_free எழுத்து *செய்தி = வெற்று;
  cleanup_free எழுத்து *tmp = வெற்று;
  va_list args_list;
  எழுத்து *swap;
  முழு ret;

  எனில் (err == வெற்று || *err == வெற்று)
    {
      // Internal error
      திரும்பு 0;
    }

  ret = -(*err)->status - 1;

  va_start (args_list, fmt);

  எனில் (vasprintf (&செய்தி, fmt, args_list) < 0)
    {
      va_end (args_list);
      செய்தி = வெற்று;
      திரும்பு ret;
    }
  va_end (args_list);

  xasprintf (&tmp, "%s: %s", செய்தி, (*err)->செய்தி);
  swap = tmp;
  tmp = (*err)->செய்தி;
  (*err)->செய்தி = swap;

  திரும்பு ret;
}

முழு
crun_error_release (libcrun_error_t *err)
{
  மாறிலி முழு saved_errno = errno;
  libcrun_error_t ptr;

  எனில் (err == வெற்று)
    திரும்பு 0;

  ptr = *err;
  எனில் (ptr == வெற்று)
    திரும்பு 0;

  விடுவி (ptr->செய்தி);
  விடுவி (ptr);
  *err = வெற்று;

  errno = saved_errno;
  திரும்பு 0;
}

முழு
libcrun_error_release (libcrun_error_t *err)
{
  திரும்பு crun_error_release (err);
}

முழு
libcrun_error_get_status (libcrun_error_t err)
{
  எனில் (err == வெற்று)
    திரும்பு 0;
  திரும்பு err->status;
}

மாறிலி எழுத்து *
libcrun_error_get_message (libcrun_error_t err)
{
  எனில் (err == வெற்று)
    திரும்பு வெற்று;
  திரும்பு err->செய்தி;
}

வெறுமை
crun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  மாறிலி முழு saved_errno = errno;
  libcrun_error_t ref;

  எனில் (out == வெற்று)
    out = stderr;
  எனில் (err == வெற்று || *err == வெற்று || **err == வெற்று)
    {
      // Internal error
      திரும்பு;
    }

  ref = **err;
  எனில் (ref->status)
    fprintf (out, "%s: %s\n", ref->செய்தி, strerror (ref->status));
  இல்லையெனில்
    fprintf (out, "%s\n", ref->செய்தி);

  விடுவி (ref->செய்தி);
  விடுவி (ref);
  **err = வெற்று;

  errno = saved_errno;
}

வெறுமை
libcrun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  திரும்பு crun_error_write_warning_and_release (out, err);
}

வெறுமை
libcrun_error_report_and_release (libcrun_error_t *err)
{
  libcrun_error_t ref;

  எனில் (err == வெற்று || *err == வெற்று)
    திரும்பு;

  ref = *err;
  libcrun_error (ref->status, "%s", ref->செய்தி);
  crun_error_release (err);
}

முழு
crun_error_get_errno (libcrun_error_t *err)
{
  எனில் (err == வெற்று || *err == வெற்று)
    திரும்பு 0;
  திரும்பு (*err)->status;
}

வகைவரை எழுத்து timestamp_t[64];

நிலையான வெறுமை
get_timestamp (timestamp_t *timestamp, மாறிலி எழுத்து *suffix)
{
  மாறிலி size_t buffer_size = அளவு (timestamp_t);
  எழுத்து *buffer = (எழுத்து *) timestamp;
  கட்டமைப்பு timeval tv;
  size_t written;
  கட்டமைப்பு tm now;

  gettimeofday (&tv, வெற்று);
  gmtime_r (&tv.tv_sec, &now);
  written = strftime (buffer, buffer_size, "%Y-%m-%dT%H:%M:%S", &now);
  snprintf (buffer + written, buffer_size - written, ".%06lldZ%.8s", (long long முழு) tv.tv_usec, suffix);
}

/* Neither openlog(3) nor the journald handler copy the identifier they are
   given, and the global output handler outlives the caller's strings (e.g.
   the context is freed while errors are still reported through it), so keep
   an owned copy alive for as long as the handler can use it.  */
நிலையான எழுத்து *log_id;

நிலையான மாறிலி எழுத்து *
set_log_id (மாறிலி எழுத்து *id, எழுத்து **old_id)
{
  *old_id = log_id;
  log_id = id ? xstrdup (id) : வெற்று;
  திரும்பு log_id;
}

நிலையான வெறுமை *
init_syslog (மாறிலி எழுத்து *id)
{
  openlog (id, 0, LOG_USER);
  திரும்பு வெற்று;
}

எண்ணிக்கை
{
  LOG_TYPE_FILE = 1,
  LOG_TYPE_SYSLOG = 2,
  LOG_TYPE_JOURNALD = 3
};

நிலையான முழு
get_log_type (மாறிலி எழுத்து *log, மாறிலி எழுத்து **data)
{
  மாறிலி எழுத்து *sep = strchr (log, ':');
  எனில் (sep == வெற்று)
    {
      *data = log;
      திரும்பு LOG_TYPE_FILE;
    }
  *data = sep + 1;

  எனில் (has_prefix (log, "syslog:"))
    திரும்பு LOG_TYPE_SYSLOG;
  எனில் (has_prefix (log, "journald:"))
    திரும்பு LOG_TYPE_JOURNALD;
  எனில் (has_prefix (log, "file:"))
    திரும்பு LOG_TYPE_FILE;

  திரும்பு -1;
}

முழு
libcrun_init_logging (crun_output_handler *new_output_handler, வெறுமை **new_output_handler_arg, மாறிலி எழுத்து *id,
                      மாறிலி எழுத்து *log, libcrun_error_t *err)
{
  /* Released only after the new handler is installed.  */
  cleanup_free எழுத்து *old_log_id = வெற்று;

  எனில் (log == வெற்று)
    {
      *new_output_handler = log_write_to_stderr;
      *new_output_handler_arg = வெற்று;
    }
  இல்லையெனில்
    {
      மாறிலி எழுத்து *arg = வெற்று;
      முழு log_type = get_log_type (log, &arg);

      எனில் (log_type < 0)
        திரும்பு crun_make_error (err, errno, "unknown log type `%s`", log);

      தேர்வு (log_type)
        {
        நிலை LOG_TYPE_FILE:
          *new_output_handler = log_write_to_stream;
          *new_output_handler_arg = fopen (arg, "a+e");
          எனில் (*new_output_handler_arg == வெற்று)
            திரும்பு crun_make_error (err, errno, "open log file `%s`", log);
          எனில் (output_verbosity >= LIBCRUN_VERBOSITY_WARNING)
            setlinebuf (*new_output_handler_arg);
          நிறுத்து;

        நிலை LOG_TYPE_SYSLOG:
          *new_output_handler_arg = init_syslog (set_log_id (arg[0] ? arg : id, &old_log_id));
          *new_output_handler = log_write_to_syslog;
          நிறுத்து;

        நிலை LOG_TYPE_JOURNALD:
          *new_output_handler = log_write_to_journald;
          *new_output_handler_arg = (வெறுமை *) set_log_id (id, &old_log_id);
          நிறுத்து;
        }
    }
  crun_set_output_handler (*new_output_handler, *new_output_handler_arg);
  திரும்பு 0;
}

வெறுமை
log_write_to_stream (முழு errno_, மாறிலி எழுத்து *செய்தி, முழு verbosity, வெறுமை *arg)
{
  timestamp_t timestamp = {
    0,
  };
  FILE *stream = arg;
  முழு tty = isatty (fileno (stream));
  மாறிலி எழுத்து *color_begin = "";
  மாறிலி எழுத்து *color_end = "";

  எனில் (tty)
    {
      தேர்வு (verbosity)
        {
        நிலை LIBCRUN_VERBOSITY_DEBUG:
          color_begin = "\x1b[1;34m";
          color_end = "\x1b[0m";
          நிறுத்து;
        நிலை LIBCRUN_VERBOSITY_WARNING:
          color_begin = "\x1b[1;33m";
          color_end = "\x1b[0m";
          நிறுத்து;
        நிலை LIBCRUN_VERBOSITY_ERROR:
          color_begin = "\x1b[1;31m";
          color_end = "\x1b[0m";
          நிறுத்து;
        }

      எனில் (log_format == LOG_FORMAT_TEXT)
        get_timestamp (&timestamp, ": ");
    }

  எனில் (errno_)
    fprintf (stream, "%s%s%s: %s%s\n", color_begin, timestamp, செய்தி, strerror (errno_), color_end);
  இல்லையெனில்
    fprintf (stream, "%s%s%s%s\n", color_begin, timestamp, செய்தி, color_end);
}

வெறுமை
log_write_to_stderr (முழு errno_, மாறிலி எழுத்து *செய்தி, முழு verbosity, வெறுமை *arg arg_unused)
{
  log_write_to_stream (errno_, செய்தி, verbosity, stderr);
}

வெறுமை
log_write_to_syslog (முழு errno_, மாறிலி எழுத்து *செய்தி, முழு verbosity, வெறுமை *arg arg_unused)
{
  முழு priority = LOG_ERR;
  தேர்வு (verbosity)
    {
    நிலை LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      நிறுத்து;
    நிலை LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      நிறுத்து;
    நிலை LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      நிறுத்து;
    }
  எனில் (errno_ == 0)
    syslog (priority, "%s", செய்தி);
  இல்லையெனில்
    syslog (priority, "%s: %s", செய்தி, strerror (errno_));
}

வெறுமை
log_write_to_journald (முழு errno_, மாறிலி எழுத்து *செய்தி, முழு verbosity, வெறுமை *arg arg_unused)
{
  (வெறுமை) errno_;
  (வெறுமை) செய்தி;
  (வெறுமை) verbosity;
#ifdef HAVE_SYSTEMD
  முழு priority = LOG_ERR;
  தேர்வு (verbosity)
    {
    நிலை LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      நிறுத்து;
    நிலை LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      நிறுத்து;
    நிலை LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      நிறுத்து;
    }
  எனில் (errno_ == 0)
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s", செய்தி, "ID=%s", arg, வெற்று);
  இல்லையெனில்
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s: %s", செய்தி, strerror (errno_), "ID=%s",
                     arg, வெற்று);
#endif
}

நிலையான crun_output_handler output_handler = log_write_to_stderr;
நிலையான வெறுமை *output_handler_arg = வெற்று;

வெறுமை
libcrun_set_verbosity (முழு verbosity)
{
  output_verbosity = verbosity;
}

முழு
libcrun_get_verbosity ()
{
  திரும்பு output_verbosity;
}

வெறுமை
crun_set_output_handler (crun_output_handler handler, வெறுமை *arg)
{
  output_handler = handler;
  output_handler_arg = arg;
}

நிலையான எழுத்து *
make_json_error (மாறிலி எழுத்து *செய்தி, முழு errno_, முழு verbosity)
{
  மாறிலி எழுத்து *level;
  தேர்வு (verbosity)
    {
    நிலை LIBCRUN_VERBOSITY_DEBUG:
      level = "debug";
      நிறுத்து;
    நிலை LIBCRUN_VERBOSITY_WARNING:
      level = "warning";
      நிறுத்து;
    நிலை LIBCRUN_VERBOSITY_ERROR:
      level = "error";
      நிறுத்து;
    }
  மாறிலி எழுத்து *buf = வெற்று;
  json_gen_ctx *gen = வெற்று;
  எழுத்து *ret = வெற்று;
  size_t buf_len;
  timestamp_t timestamp = {
    0,
  };

  எனில் (! json_gen_init (&gen, வெற்று))
    திரும்பு வெற்று;

  get_timestamp (&timestamp, "");

  json_gen_map_open (gen);

  json_gen_string (gen, "msg", strlen ("msg"));
  எனில் (errno_ == 0)
    json_gen_string (gen, செய்தி, strlen (செய்தி));
  இல்லையெனில்
    {
      cleanup_free எழுத்து *tmp = வெற்று;

      xasprintf (&tmp, "%s: %s", செய்தி, strerror (errno_));
      json_gen_string (gen, tmp, strlen (tmp));
    }

  json_gen_string (gen, "level", strlen ("level"));
  json_gen_string (gen, level, strlen (level));

  json_gen_string (gen, "time", strlen ("time"));
  json_gen_string (gen, timestamp, strlen (timestamp));

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);
  எனில் (buf)
    ret = strdup (buf);

  json_gen_free (gen);

  திரும்பு ret;
}

நிலையான வெறுமை
write_log (முழு errno_, முழு verbosity, மாறிலி எழுத்து *செய்தி, va_list args_list)
{
  முழு ret;
  cleanup_free எழுத்து *output = வெற்று;
  cleanup_free எழுத்து *json = வெற்று;

  எனில் (verbosity > output_verbosity)
    திரும்பு;

  ret = vasprintf (&output, செய்தி, args_list);
  எனில் (UNLIKELY (ret < 0))
    OOM ();

  எனில் (verbosity == LIBCRUN_VERBOSITY_ERROR && output_handler != log_write_to_stderr)
    log_write_to_stderr (errno_, output, LIBCRUN_VERBOSITY_ERROR, வெற்று);

  தேர்வு (log_format)
    {
    நிலை LOG_FORMAT_TEXT:
      output_handler (errno_, output, verbosity, output_handler_arg);
      நிறுத்து;

    நிலை LOG_FORMAT_JSON:
      json = make_json_error (output, errno_, verbosity);
      எனில் (json)
        output_handler (0, json, verbosity, output_handler_arg);
      இல்லையெனில்
        output_handler (errno_, output, verbosity, output_handler_arg);
      நிறுத்து;
    }
}

வெறுமை
libcrun_debug (மாறிலி எழுத்து *செய்தி, ...)
{
  va_list args_list;
  va_start (args_list, செய்தி);
  write_log (0, LIBCRUN_VERBOSITY_DEBUG, செய்தி, args_list);
  va_end (args_list);
}

வெறுமை
libcrun_warning (மாறிலி எழுத்து *செய்தி, ...)
{
  va_list args_list;
  va_start (args_list, செய்தி);
  write_log (0, LIBCRUN_VERBOSITY_WARNING, செய்தி, args_list);
  va_end (args_list);
}

வெறுமை
libcrun_error (முழு errno_, மாறிலி எழுத்து *செய்தி, ...)
{
  va_list args_list;
  va_start (args_list, செய்தி);

  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, செய்தி, args_list);
  va_end (args_list);
}

வெறுமை __attribute__ ((noreturn))
libcrun_fail_with_error (முழு errno_, மாறிலி எழுத்து *செய்தி, ...)
{
  va_list args_list;
  va_start (args_list, செய்தி);
  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, செய்தி, args_list);
  va_end (args_list);
  exit (EXIT_FAILURE);
}

முழு
libcrun_set_log_format (மாறிலி எழுத்து *format, libcrun_error_t *err)
{
  எனில் (strcmp (format, "text") == 0)
    log_format = LOG_FORMAT_TEXT;
  இல்லையெனில் எனில் (strcmp (format, "json") == 0)
    log_format = LOG_FORMAT_JSON;
  இல்லையெனில்
    திரும்பு crun_make_error (err, 0, "unknown log format type `%s`", format);

  திரும்பு 0;
}

முழு
json_gen_error_to_crun_error (முழு status, libcrun_error_t *err)
{
  தேர்வு (status)
    {
    நிலை json_gen_status_ok:
      திரும்பு 0;

    இயல்பு:
      திரும்பு crun_make_error (err, 0, "generate JSON document: error %d", status);
    }
}
