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
#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdarg.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <time.h>
#അന്തര്ഭാവ <sys/time.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ "utils.h"

#അന്തര്ഭാവ <json-c/json.h>
#അന്തര്ഭാവ <ocispec/json_common.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-journal.h>
#endif

എണ്ണൽ
{
  LOG_FORMAT_TEXT = 0,
  LOG_FORMAT_JSON,
};

സ്ഥിതം പൂർണ്ണസംഖ്യ log_format;
സ്ഥിതം പൂർണ്ണസംഖ്യ output_verbosity = LIBCRUN_VERBOSITY_ERROR;

പൂർണ്ണസംഖ്യ
libcrun_make_error (libcrun_error_t *err, പൂർണ്ണസംഖ്യ status, സ്ഥിരം അക്ഷരം *സന്ദേശം, ...)
{
  va_list args_list;
  libcrun_error_t ptr;

  va_start (args_list, സന്ദേശം);
  *err = xmalloc (വലുപ്പം (ഘടന libcrun_error_s));
  ptr = *err;
  ptr->status = status;
  എങ്കിൽ (vasprintf (&(ptr->സന്ദേശം), സന്ദേശം, args_list) < 0)
    OOM ();
  va_end (args_list);

  തിരികെ -status - 1;
}

പൂർണ്ണസംഖ്യ
crun_error_wrap (libcrun_error_t *err, സ്ഥിരം അക്ഷരം *fmt, ...)
{
  cleanup_free അക്ഷരം *സന്ദേശം = ശൂന്യ;
  cleanup_free അക്ഷരം *tmp = ശൂന്യ;
  va_list args_list;
  അക്ഷരം *swap;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (err == ശൂന്യ || *err == ശൂന്യ)
    {
      // Internal error
      തിരികെ 0;
    }

  ret = -(*err)->status - 1;

  va_start (args_list, fmt);

  എങ്കിൽ (vasprintf (&സന്ദേശം, fmt, args_list) < 0)
    {
      va_end (args_list);
      സന്ദേശം = ശൂന്യ;
      തിരികെ ret;
    }
  va_end (args_list);

  xasprintf (&tmp, "%s: %s", സന്ദേശം, (*err)->സന്ദേശം);
  swap = tmp;
  tmp = (*err)->സന്ദേശം;
  (*err)->സന്ദേശം = swap;

  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
crun_error_release (libcrun_error_t *err)
{
  സ്ഥിരം പൂർണ്ണസംഖ്യ saved_errno = errno;
  libcrun_error_t ptr;

  എങ്കിൽ (err == ശൂന്യ)
    തിരികെ 0;

  ptr = *err;
  എങ്കിൽ (ptr == ശൂന്യ)
    തിരികെ 0;

  സ്വതന്ത്രം (ptr->സന്ദേശം);
  സ്വതന്ത്രം (ptr);
  *err = ശൂന്യ;

  errno = saved_errno;
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_error_release (libcrun_error_t *err)
{
  തിരികെ crun_error_release (err);
}

പൂർണ്ണസംഖ്യ
libcrun_error_get_status (libcrun_error_t err)
{
  എങ്കിൽ (err == ശൂന്യ)
    തിരികെ 0;
  തിരികെ err->status;
}

സ്ഥിരം അക്ഷരം *
libcrun_error_get_message (libcrun_error_t err)
{
  എങ്കിൽ (err == ശൂന്യ)
    തിരികെ ശൂന്യ;
  തിരികെ err->സന്ദേശം;
}

ശൂന്യം
crun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  സ്ഥിരം പൂർണ്ണസംഖ്യ saved_errno = errno;
  libcrun_error_t ref;

  എങ്കിൽ (out == ശൂന്യ)
    out = stderr;
  എങ്കിൽ (err == ശൂന്യ || *err == ശൂന്യ || **err == ശൂന്യ)
    {
      // Internal error
      തിരികെ;
    }

  ref = **err;
  എങ്കിൽ (ref->status)
    fprintf (out, "%s: %s\n", ref->സന്ദേശം, strerror (ref->status));
  അല്ലെങ്കിൽ
    fprintf (out, "%s\n", ref->സന്ദേശം);

  സ്വതന്ത്രം (ref->സന്ദേശം);
  സ്വതന്ത്രം (ref);
  **err = ശൂന്യ;

  errno = saved_errno;
}

ശൂന്യം
libcrun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  തിരികെ crun_error_write_warning_and_release (out, err);
}

ശൂന്യം
libcrun_error_report_and_release (libcrun_error_t *err)
{
  libcrun_error_t ref;

  എങ്കിൽ (err == ശൂന്യ || *err == ശൂന്യ)
    തിരികെ;

  ref = *err;
  libcrun_error (ref->status, "%s", ref->സന്ദേശം);
  crun_error_release (err);
}

പൂർണ്ണസംഖ്യ
crun_error_get_errno (libcrun_error_t *err)
{
  എങ്കിൽ (err == ശൂന്യ || *err == ശൂന്യ)
    തിരികെ 0;
  തിരികെ (*err)->status;
}

തരനിർവചനം അക്ഷരം timestamp_t[64];

സ്ഥിതം ശൂന്യം
get_timestamp (timestamp_t *timestamp, സ്ഥിരം അക്ഷരം *suffix)
{
  സ്ഥിരം size_t buffer_size = വലുപ്പം (timestamp_t);
  അക്ഷരം *buffer = (അക്ഷരം *) timestamp;
  ഘടന timeval tv;
  size_t written;
  ഘടന tm now;

  gettimeofday (&tv, ശൂന്യ);
  gmtime_r (&tv.tv_sec, &now);
  written = strftime (buffer, buffer_size, "%Y-%m-%dT%H:%M:%S", &now);
  snprintf (buffer + written, buffer_size - written, ".%06lldZ%.8s", (long long പൂർണ്ണസംഖ്യ) tv.tv_usec, suffix);
}

/* Neither openlog(3) nor the journald handler copy the identifier they are
   given, and the global output handler outlives the caller's strings (e.g.
   the context is freed while errors are still reported through it), so keep
   an owned copy alive for as long as the handler can use it.  */
സ്ഥിതം അക്ഷരം *log_id;

സ്ഥിതം സ്ഥിരം അക്ഷരം *
set_log_id (സ്ഥിരം അക്ഷരം *id, അക്ഷരം **old_id)
{
  *old_id = log_id;
  log_id = id ? xstrdup (id) : ശൂന്യ;
  തിരികെ log_id;
}

സ്ഥിതം ശൂന്യം *
init_syslog (സ്ഥിരം അക്ഷരം *id)
{
  openlog (id, 0, LOG_USER);
  തിരികെ ശൂന്യ;
}

എണ്ണൽ
{
  LOG_TYPE_FILE = 1,
  LOG_TYPE_SYSLOG = 2,
  LOG_TYPE_JOURNALD = 3
};

സ്ഥിതം പൂർണ്ണസംഖ്യ
get_log_type (സ്ഥിരം അക്ഷരം *log, സ്ഥിരം അക്ഷരം **data)
{
  സ്ഥിരം അക്ഷരം *sep = strchr (log, ':');
  എങ്കിൽ (sep == ശൂന്യ)
    {
      *data = log;
      തിരികെ LOG_TYPE_FILE;
    }
  *data = sep + 1;

  എങ്കിൽ (has_prefix (log, "syslog:"))
    തിരികെ LOG_TYPE_SYSLOG;
  എങ്കിൽ (has_prefix (log, "journald:"))
    തിരികെ LOG_TYPE_JOURNALD;
  എങ്കിൽ (has_prefix (log, "file:"))
    തിരികെ LOG_TYPE_FILE;

  തിരികെ -1;
}

പൂർണ്ണസംഖ്യ
libcrun_init_logging (crun_output_handler *new_output_handler, ശൂന്യം **new_output_handler_arg, സ്ഥിരം അക്ഷരം *id,
                      സ്ഥിരം അക്ഷരം *log, libcrun_error_t *err)
{
  /* Released only after the new handler is installed.  */
  cleanup_free അക്ഷരം *old_log_id = ശൂന്യ;

  എങ്കിൽ (log == ശൂന്യ)
    {
      *new_output_handler = log_write_to_stderr;
      *new_output_handler_arg = ശൂന്യ;
    }
  അല്ലെങ്കിൽ
    {
      സ്ഥിരം അക്ഷരം *arg = ശൂന്യ;
      പൂർണ്ണസംഖ്യ log_type = get_log_type (log, &arg);

      എങ്കിൽ (log_type < 0)
        തിരികെ crun_make_error (err, errno, "unknown log type `%s`", log);

      തിരഞ്ഞെടുപ്പ് (log_type)
        {
        അവസ്ഥ LOG_TYPE_FILE:
          *new_output_handler = log_write_to_stream;
          *new_output_handler_arg = fopen (arg, "a+e");
          എങ്കിൽ (*new_output_handler_arg == ശൂന്യ)
            തിരികെ crun_make_error (err, errno, "open log file `%s`", log);
          എങ്കിൽ (output_verbosity >= LIBCRUN_VERBOSITY_WARNING)
            setlinebuf (*new_output_handler_arg);
          നിർത്തുക;

        അവസ്ഥ LOG_TYPE_SYSLOG:
          *new_output_handler_arg = init_syslog (set_log_id (arg[0] ? arg : id, &old_log_id));
          *new_output_handler = log_write_to_syslog;
          നിർത്തുക;

        അവസ്ഥ LOG_TYPE_JOURNALD:
          *new_output_handler = log_write_to_journald;
          *new_output_handler_arg = (ശൂന്യം *) set_log_id (id, &old_log_id);
          നിർത്തുക;
        }
    }
  crun_set_output_handler (*new_output_handler, *new_output_handler_arg);
  തിരികെ 0;
}

ശൂന്യം
log_write_to_stream (പൂർണ്ണസംഖ്യ errno_, സ്ഥിരം അക്ഷരം *സന്ദേശം, പൂർണ്ണസംഖ്യ verbosity, ശൂന്യം *arg)
{
  timestamp_t timestamp = {
    0,
  };
  FILE *stream = arg;
  പൂർണ്ണസംഖ്യ tty = isatty (fileno (stream));
  സ്ഥിരം അക്ഷരം *color_begin = "";
  സ്ഥിരം അക്ഷരം *color_end = "";

  എങ്കിൽ (tty)
    {
      തിരഞ്ഞെടുപ്പ് (verbosity)
        {
        അവസ്ഥ LIBCRUN_VERBOSITY_DEBUG:
          color_begin = "\x1b[1;34m";
          color_end = "\x1b[0m";
          നിർത്തുക;
        അവസ്ഥ LIBCRUN_VERBOSITY_WARNING:
          color_begin = "\x1b[1;33m";
          color_end = "\x1b[0m";
          നിർത്തുക;
        അവസ്ഥ LIBCRUN_VERBOSITY_ERROR:
          color_begin = "\x1b[1;31m";
          color_end = "\x1b[0m";
          നിർത്തുക;
        }

      എങ്കിൽ (log_format == LOG_FORMAT_TEXT)
        get_timestamp (&timestamp, ": ");
    }

  എങ്കിൽ (errno_)
    fprintf (stream, "%s%s%s: %s%s\n", color_begin, timestamp, സന്ദേശം, strerror (errno_), color_end);
  അല്ലെങ്കിൽ
    fprintf (stream, "%s%s%s%s\n", color_begin, timestamp, സന്ദേശം, color_end);
}

ശൂന്യം
log_write_to_stderr (പൂർണ്ണസംഖ്യ errno_, സ്ഥിരം അക്ഷരം *സന്ദേശം, പൂർണ്ണസംഖ്യ verbosity, ശൂന്യം *arg arg_unused)
{
  log_write_to_stream (errno_, സന്ദേശം, verbosity, stderr);
}

ശൂന്യം
log_write_to_syslog (പൂർണ്ണസംഖ്യ errno_, സ്ഥിരം അക്ഷരം *സന്ദേശം, പൂർണ്ണസംഖ്യ verbosity, ശൂന്യം *arg arg_unused)
{
  പൂർണ്ണസംഖ്യ priority = LOG_ERR;
  തിരഞ്ഞെടുപ്പ് (verbosity)
    {
    അവസ്ഥ LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      നിർത്തുക;
    അവസ്ഥ LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      നിർത്തുക;
    അവസ്ഥ LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      നിർത്തുക;
    }
  എങ്കിൽ (errno_ == 0)
    syslog (priority, "%s", സന്ദേശം);
  അല്ലെങ്കിൽ
    syslog (priority, "%s: %s", സന്ദേശം, strerror (errno_));
}

ശൂന്യം
log_write_to_journald (പൂർണ്ണസംഖ്യ errno_, സ്ഥിരം അക്ഷരം *സന്ദേശം, പൂർണ്ണസംഖ്യ verbosity, ശൂന്യം *arg arg_unused)
{
  (ശൂന്യം) errno_;
  (ശൂന്യം) സന്ദേശം;
  (ശൂന്യം) verbosity;
#ifdef HAVE_SYSTEMD
  പൂർണ്ണസംഖ്യ priority = LOG_ERR;
  തിരഞ്ഞെടുപ്പ് (verbosity)
    {
    അവസ്ഥ LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      നിർത്തുക;
    അവസ്ഥ LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      നിർത്തുക;
    അവസ്ഥ LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      നിർത്തുക;
    }
  എങ്കിൽ (errno_ == 0)
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s", സന്ദേശം, "ID=%s", arg, ശൂന്യ);
  അല്ലെങ്കിൽ
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s: %s", സന്ദേശം, strerror (errno_), "ID=%s",
                     arg, ശൂന്യ);
#endif
}

സ്ഥിതം crun_output_handler output_handler = log_write_to_stderr;
സ്ഥിതം ശൂന്യം *output_handler_arg = ശൂന്യ;

ശൂന്യം
libcrun_set_verbosity (പൂർണ്ണസംഖ്യ verbosity)
{
  output_verbosity = verbosity;
}

പൂർണ്ണസംഖ്യ
libcrun_get_verbosity ()
{
  തിരികെ output_verbosity;
}

ശൂന്യം
crun_set_output_handler (crun_output_handler handler, ശൂന്യം *arg)
{
  output_handler = handler;
  output_handler_arg = arg;
}

സ്ഥിതം അക്ഷരം *
make_json_error (സ്ഥിരം അക്ഷരം *സന്ദേശം, പൂർണ്ണസംഖ്യ errno_, പൂർണ്ണസംഖ്യ verbosity)
{
  സ്ഥിരം അക്ഷരം *level;
  തിരഞ്ഞെടുപ്പ് (verbosity)
    {
    അവസ്ഥ LIBCRUN_VERBOSITY_DEBUG:
      level = "debug";
      നിർത്തുക;
    അവസ്ഥ LIBCRUN_VERBOSITY_WARNING:
      level = "warning";
      നിർത്തുക;
    അവസ്ഥ LIBCRUN_VERBOSITY_ERROR:
      level = "error";
      നിർത്തുക;
    }
  സ്ഥിരം അക്ഷരം *buf = ശൂന്യ;
  json_gen_ctx *gen = ശൂന്യ;
  അക്ഷരം *ret = ശൂന്യ;
  size_t buf_len;
  timestamp_t timestamp = {
    0,
  };

  എങ്കിൽ (! json_gen_init (&gen, ശൂന്യ))
    തിരികെ ശൂന്യ;

  get_timestamp (&timestamp, "");

  json_gen_map_open (gen);

  json_gen_string (gen, "msg", strlen ("msg"));
  എങ്കിൽ (errno_ == 0)
    json_gen_string (gen, സന്ദേശം, strlen (സന്ദേശം));
  അല്ലെങ്കിൽ
    {
      cleanup_free അക്ഷരം *tmp = ശൂന്യ;

      xasprintf (&tmp, "%s: %s", സന്ദേശം, strerror (errno_));
      json_gen_string (gen, tmp, strlen (tmp));
    }

  json_gen_string (gen, "level", strlen ("level"));
  json_gen_string (gen, level, strlen (level));

  json_gen_string (gen, "time", strlen ("time"));
  json_gen_string (gen, timestamp, strlen (timestamp));

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);
  എങ്കിൽ (buf)
    ret = strdup (buf);

  json_gen_free (gen);

  തിരികെ ret;
}

സ്ഥിതം ശൂന്യം
write_log (പൂർണ്ണസംഖ്യ errno_, പൂർണ്ണസംഖ്യ verbosity, സ്ഥിരം അക്ഷരം *സന്ദേശം, va_list args_list)
{
  പൂർണ്ണസംഖ്യ ret;
  cleanup_free അക്ഷരം *output = ശൂന്യ;
  cleanup_free അക്ഷരം *json = ശൂന്യ;

  എങ്കിൽ (verbosity > output_verbosity)
    തിരികെ;

  ret = vasprintf (&output, സന്ദേശം, args_list);
  എങ്കിൽ (UNLIKELY (ret < 0))
    OOM ();

  എങ്കിൽ (verbosity == LIBCRUN_VERBOSITY_ERROR && output_handler != log_write_to_stderr)
    log_write_to_stderr (errno_, output, LIBCRUN_VERBOSITY_ERROR, ശൂന്യ);

  തിരഞ്ഞെടുപ്പ് (log_format)
    {
    അവസ്ഥ LOG_FORMAT_TEXT:
      output_handler (errno_, output, verbosity, output_handler_arg);
      നിർത്തുക;

    അവസ്ഥ LOG_FORMAT_JSON:
      json = make_json_error (output, errno_, verbosity);
      എങ്കിൽ (json)
        output_handler (0, json, verbosity, output_handler_arg);
      അല്ലെങ്കിൽ
        output_handler (errno_, output, verbosity, output_handler_arg);
      നിർത്തുക;
    }
}

ശൂന്യം
libcrun_debug (സ്ഥിരം അക്ഷരം *സന്ദേശം, ...)
{
  va_list args_list;
  va_start (args_list, സന്ദേശം);
  write_log (0, LIBCRUN_VERBOSITY_DEBUG, സന്ദേശം, args_list);
  va_end (args_list);
}

ശൂന്യം
libcrun_warning (സ്ഥിരം അക്ഷരം *സന്ദേശം, ...)
{
  va_list args_list;
  va_start (args_list, സന്ദേശം);
  write_log (0, LIBCRUN_VERBOSITY_WARNING, സന്ദേശം, args_list);
  va_end (args_list);
}

ശൂന്യം
libcrun_error (പൂർണ്ണസംഖ്യ errno_, സ്ഥിരം അക്ഷരം *സന്ദേശം, ...)
{
  va_list args_list;
  va_start (args_list, സന്ദേശം);

  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, സന്ദേശം, args_list);
  va_end (args_list);
}

ശൂന്യം __attribute__ ((noreturn))
libcrun_fail_with_error (പൂർണ്ണസംഖ്യ errno_, സ്ഥിരം അക്ഷരം *സന്ദേശം, ...)
{
  va_list args_list;
  va_start (args_list, സന്ദേശം);
  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, സന്ദേശം, args_list);
  va_end (args_list);
  exit (EXIT_FAILURE);
}

പൂർണ്ണസംഖ്യ
libcrun_set_log_format (സ്ഥിരം അക്ഷരം *format, libcrun_error_t *err)
{
  എങ്കിൽ (strcmp (format, "text") == 0)
    log_format = LOG_FORMAT_TEXT;
  അല്ലെങ്കിൽ എങ്കിൽ (strcmp (format, "json") == 0)
    log_format = LOG_FORMAT_JSON;
  അല്ലെങ്കിൽ
    തിരികെ crun_make_error (err, 0, "unknown log format type `%s`", format);

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
json_gen_error_to_crun_error (പൂർണ്ണസംഖ്യ status, libcrun_error_t *err)
{
  തിരഞ്ഞെടുപ്പ് (status)
    {
    അവസ്ഥ json_gen_status_ok:
      തിരികെ 0;

    സ്ഥിരസ്ഥിതി:
      തിരികെ crun_make_error (err, 0, "generate JSON document: error %d", status);
    }
}
