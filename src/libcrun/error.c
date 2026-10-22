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
#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdarg.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଅନ୍ତର୍ଭାଵ <sys/time.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ "utils.h"

#ଅନ୍ତର୍ଭାଵ <json-c/json.h>
#ଅନ୍ତର୍ଭାଵ <ocispec/json_common.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-journal.h>
#endif

ଗଣନା
{
  LOG_FORMAT_TEXT = 0,
  LOG_FORMAT_JSON,
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା log_format;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା output_verbosity = LIBCRUN_VERBOSITY_ERROR;

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_make_error (libcrun_error_t *err, ପୂର୍ଣ୍ଣସଂଖ୍ୟା status, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ...)
{
  va_list args_list;
  libcrun_error_t ptr;

  va_start (args_list, ସନ୍ଦେଶ);
  *err = xmalloc (ଆକାର (ସଂରଚନା libcrun_error_s));
  ptr = *err;
  ptr->status = status;
  ଯଦି (vasprintf (&(ptr->ସନ୍ଦେଶ), ସନ୍ଦେଶ, args_list) < 0)
    OOM ();
  va_end (args_list);

  ଫେରନ୍ତୁ -status - 1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_error_wrap (libcrun_error_t *err, ଅଚଳ ଅକ୍ଷର *fmt, ...)
{
  cleanup_free ଅକ୍ଷର *ସନ୍ଦେଶ = ଖାଲି;
  cleanup_free ଅକ୍ଷର *tmp = ଖାଲି;
  va_list args_list;
  ଅକ୍ଷର *swap;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (err == ଖାଲି || *err == ଖାଲି)
    {
      // Internal error
      ଫେରନ୍ତୁ 0;
    }

  ret = -(*err)->status - 1;

  va_start (args_list, fmt);

  ଯଦି (vasprintf (&ସନ୍ଦେଶ, fmt, args_list) < 0)
    {
      va_end (args_list);
      ସନ୍ଦେଶ = ଖାଲି;
      ଫେରନ୍ତୁ ret;
    }
  va_end (args_list);

  xasprintf (&tmp, "%s: %s", ସନ୍ଦେଶ, (*err)->ସନ୍ଦେଶ);
  swap = tmp;
  tmp = (*err)->ସନ୍ଦେଶ;
  (*err)->ସନ୍ଦେଶ = swap;

  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_error_release (libcrun_error_t *err)
{
  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
  libcrun_error_t ptr;

  ଯଦି (err == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  ptr = *err;
  ଯଦି (ptr == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  ମୁକ୍ତ (ptr->ସନ୍ଦେଶ);
  ମୁକ୍ତ (ptr);
  *err = ଖାଲି;

  errno = saved_errno;
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_error_release (libcrun_error_t *err)
{
  ଫେରନ୍ତୁ crun_error_release (err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_error_get_status (libcrun_error_t err)
{
  ଯଦି (err == ଖାଲି)
    ଫେରନ୍ତୁ 0;
  ଫେରନ୍ତୁ err->status;
}

ଅଚଳ ଅକ୍ଷର *
libcrun_error_get_message (libcrun_error_t err)
{
  ଯଦି (err == ଖାଲି)
    ଫେରନ୍ତୁ ଖାଲି;
  ଫେରନ୍ତୁ err->ସନ୍ଦେଶ;
}

ଶୂନ୍ୟ
crun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
  libcrun_error_t ref;

  ଯଦି (out == ଖାଲି)
    out = stderr;
  ଯଦି (err == ଖାଲି || *err == ଖାଲି || **err == ଖାଲି)
    {
      // Internal error
      ଫେରନ୍ତୁ;
    }

  ref = **err;
  ଯଦି (ref->status)
    fprintf (out, "%s: %s\n", ref->ସନ୍ଦେଶ, strerror (ref->status));
  ଅନ୍ୟଥା
    fprintf (out, "%s\n", ref->ସନ୍ଦେଶ);

  ମୁକ୍ତ (ref->ସନ୍ଦେଶ);
  ମୁକ୍ତ (ref);
  **err = ଖାଲି;

  errno = saved_errno;
}

ଶୂନ୍ୟ
libcrun_error_write_warning_and_release (FILE *out, libcrun_error_t **err)
{
  ଫେରନ୍ତୁ crun_error_write_warning_and_release (out, err);
}

ଶୂନ୍ୟ
libcrun_error_report_and_release (libcrun_error_t *err)
{
  libcrun_error_t ref;

  ଯଦି (err == ଖାଲି || *err == ଖାଲି)
    ଫେରନ୍ତୁ;

  ref = *err;
  libcrun_error (ref->status, "%s", ref->ସନ୍ଦେଶ);
  crun_error_release (err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_error_get_errno (libcrun_error_t *err)
{
  ଯଦି (err == ଖାଲି || *err == ଖାଲି)
    ଫେରନ୍ତୁ 0;
  ଫେରନ୍ତୁ (*err)->status;
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅକ୍ଷର timestamp_t[64];

ସ୍ଥିର ଶୂନ୍ୟ
get_timestamp (timestamp_t *timestamp, ଅଚଳ ଅକ୍ଷର *suffix)
{
  ଅଚଳ size_t buffer_size = ଆକାର (timestamp_t);
  ଅକ୍ଷର *buffer = (ଅକ୍ଷର *) timestamp;
  ସଂରଚନା timeval tv;
  size_t written;
  ସଂରଚନା tm now;

  gettimeofday (&tv, ଖାଲି);
  gmtime_r (&tv.tv_sec, &now);
  written = strftime (buffer, buffer_size, "%Y-%m-%dT%H:%M:%S", &now);
  snprintf (buffer + written, buffer_size - written, ".%06lldZ%.8s", (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା) tv.tv_usec, suffix);
}

/* Neither openlog(3) nor the journald handler copy the identifier they are
   given, and the global output handler outlives the caller's strings (e.g.
   the context is freed while errors are still reported through it), so keep
   an owned copy alive for as long as the handler can use it.  */
ସ୍ଥିର ଅକ୍ଷର *log_id;

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
set_log_id (ଅଚଳ ଅକ୍ଷର *id, ଅକ୍ଷର **old_id)
{
  *old_id = log_id;
  log_id = id ? xstrdup (id) : ଖାଲି;
  ଫେରନ୍ତୁ log_id;
}

ସ୍ଥିର ଶୂନ୍ୟ *
init_syslog (ଅଚଳ ଅକ୍ଷର *id)
{
  openlog (id, 0, LOG_USER);
  ଫେରନ୍ତୁ ଖାଲି;
}

ଗଣନା
{
  LOG_TYPE_FILE = 1,
  LOG_TYPE_SYSLOG = 2,
  LOG_TYPE_JOURNALD = 3
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_log_type (ଅଚଳ ଅକ୍ଷର *log, ଅଚଳ ଅକ୍ଷର **data)
{
  ଅଚଳ ଅକ୍ଷର *sep = strchr (log, ':');
  ଯଦି (sep == ଖାଲି)
    {
      *data = log;
      ଫେରନ୍ତୁ LOG_TYPE_FILE;
    }
  *data = sep + 1;

  ଯଦି (has_prefix (log, "syslog:"))
    ଫେରନ୍ତୁ LOG_TYPE_SYSLOG;
  ଯଦି (has_prefix (log, "journald:"))
    ଫେରନ୍ତୁ LOG_TYPE_JOURNALD;
  ଯଦି (has_prefix (log, "file:"))
    ଫେରନ୍ତୁ LOG_TYPE_FILE;

  ଫେରନ୍ତୁ -1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_init_logging (crun_output_handler *new_output_handler, ଶୂନ୍ୟ **new_output_handler_arg, ଅଚଳ ଅକ୍ଷର *id,
                      ଅଚଳ ଅକ୍ଷର *log, libcrun_error_t *err)
{
  /* Released only after the new handler is installed.  */
  cleanup_free ଅକ୍ଷର *old_log_id = ଖାଲି;

  ଯଦି (log == ଖାଲି)
    {
      *new_output_handler = log_write_to_stderr;
      *new_output_handler_arg = ଖାଲି;
    }
  ଅନ୍ୟଥା
    {
      ଅଚଳ ଅକ୍ଷର *arg = ଖାଲି;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା log_type = get_log_type (log, &arg);

      ଯଦି (log_type < 0)
        ଫେରନ୍ତୁ crun_make_error (err, errno, "unknown log type `%s`", log);

      ବାଛ (log_type)
        {
        ସ୍ଥିତି LOG_TYPE_FILE:
          *new_output_handler = log_write_to_stream;
          *new_output_handler_arg = fopen (arg, "a+e");
          ଯଦି (*new_output_handler_arg == ଖାଲି)
            ଫେରନ୍ତୁ crun_make_error (err, errno, "open log file `%s`", log);
          ଯଦି (output_verbosity >= LIBCRUN_VERBOSITY_WARNING)
            setlinebuf (*new_output_handler_arg);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି LOG_TYPE_SYSLOG:
          *new_output_handler_arg = init_syslog (set_log_id (arg[0] ? arg : id, &old_log_id));
          *new_output_handler = log_write_to_syslog;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି LOG_TYPE_JOURNALD:
          *new_output_handler = log_write_to_journald;
          *new_output_handler_arg = (ଶୂନ୍ୟ *) set_log_id (id, &old_log_id);
          ଭାଙ୍ଗ;
        }
    }
  crun_set_output_handler (*new_output_handler, *new_output_handler_arg);
  ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ
log_write_to_stream (ପୂର୍ଣ୍ଣସଂଖ୍ୟା errno_, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity, ଶୂନ୍ୟ *arg)
{
  timestamp_t timestamp = {
    0,
  };
  FILE *stream = arg;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା tty = isatty (fileno (stream));
  ଅଚଳ ଅକ୍ଷର *color_begin = "";
  ଅଚଳ ଅକ୍ଷର *color_end = "";

  ଯଦି (tty)
    {
      ବାଛ (verbosity)
        {
        ସ୍ଥିତି LIBCRUN_VERBOSITY_DEBUG:
          color_begin = "\x1b[1;34m";
          color_end = "\x1b[0m";
          ଭାଙ୍ଗ;
        ସ୍ଥିତି LIBCRUN_VERBOSITY_WARNING:
          color_begin = "\x1b[1;33m";
          color_end = "\x1b[0m";
          ଭାଙ୍ଗ;
        ସ୍ଥିତି LIBCRUN_VERBOSITY_ERROR:
          color_begin = "\x1b[1;31m";
          color_end = "\x1b[0m";
          ଭାଙ୍ଗ;
        }

      ଯଦି (log_format == LOG_FORMAT_TEXT)
        get_timestamp (&timestamp, ": ");
    }

  ଯଦି (errno_)
    fprintf (stream, "%s%s%s: %s%s\n", color_begin, timestamp, ସନ୍ଦେଶ, strerror (errno_), color_end);
  ଅନ୍ୟଥା
    fprintf (stream, "%s%s%s%s\n", color_begin, timestamp, ସନ୍ଦେଶ, color_end);
}

ଶୂନ୍ୟ
log_write_to_stderr (ପୂର୍ଣ୍ଣସଂଖ୍ୟା errno_, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity, ଶୂନ୍ୟ *arg arg_unused)
{
  log_write_to_stream (errno_, ସନ୍ଦେଶ, verbosity, stderr);
}

ଶୂନ୍ୟ
log_write_to_syslog (ପୂର୍ଣ୍ଣସଂଖ୍ୟା errno_, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity, ଶୂନ୍ୟ *arg arg_unused)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା priority = LOG_ERR;
  ବାଛ (verbosity)
    {
    ସ୍ଥିତି LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      ଭାଙ୍ଗ;
    }
  ଯଦି (errno_ == 0)
    syslog (priority, "%s", ସନ୍ଦେଶ);
  ଅନ୍ୟଥା
    syslog (priority, "%s: %s", ସନ୍ଦେଶ, strerror (errno_));
}

ଶୂନ୍ୟ
log_write_to_journald (ପୂର୍ଣ୍ଣସଂଖ୍ୟା errno_, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity, ଶୂନ୍ୟ *arg arg_unused)
{
  (ଶୂନ୍ୟ) errno_;
  (ଶୂନ୍ୟ) ସନ୍ଦେଶ;
  (ଶୂନ୍ୟ) verbosity;
#ifdef HAVE_SYSTEMD
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା priority = LOG_ERR;
  ବାଛ (verbosity)
    {
    ସ୍ଥିତି LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      ଭାଙ୍ଗ;
    }
  ଯଦି (errno_ == 0)
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s", ସନ୍ଦେଶ, "ID=%s", arg, ଖାଲି);
  ଅନ୍ୟଥା
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s: %s", ସନ୍ଦେଶ, strerror (errno_), "ID=%s",
                     arg, ଖାଲି);
#endif
}

ସ୍ଥିର crun_output_handler output_handler = log_write_to_stderr;
ସ୍ଥିର ଶୂନ୍ୟ *output_handler_arg = ଖାଲି;

ଶୂନ୍ୟ
libcrun_set_verbosity (ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity)
{
  output_verbosity = verbosity;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_get_verbosity ()
{
  ଫେରନ୍ତୁ output_verbosity;
}

ଶୂନ୍ୟ
crun_set_output_handler (crun_output_handler handler, ଶୂନ୍ୟ *arg)
{
  output_handler = handler;
  output_handler_arg = arg;
}

ସ୍ଥିର ଅକ୍ଷର *
make_json_error (ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା errno_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity)
{
  ଅଚଳ ଅକ୍ଷର *level;
  ବାଛ (verbosity)
    {
    ସ୍ଥିତି LIBCRUN_VERBOSITY_DEBUG:
      level = "debug";
      ଭାଙ୍ଗ;
    ସ୍ଥିତି LIBCRUN_VERBOSITY_WARNING:
      level = "warning";
      ଭାଙ୍ଗ;
    ସ୍ଥିତି LIBCRUN_VERBOSITY_ERROR:
      level = "error";
      ଭାଙ୍ଗ;
    }
  ଅଚଳ ଅକ୍ଷର *buf = ଖାଲି;
  json_gen_ctx *gen = ଖାଲି;
  ଅକ୍ଷର *ret = ଖାଲି;
  size_t buf_len;
  timestamp_t timestamp = {
    0,
  };

  ଯଦି (! json_gen_init (&gen, ଖାଲି))
    ଫେରନ୍ତୁ ଖାଲି;

  get_timestamp (&timestamp, "");

  json_gen_map_open (gen);

  json_gen_string (gen, "msg", strlen ("msg"));
  ଯଦି (errno_ == 0)
    json_gen_string (gen, ସନ୍ଦେଶ, strlen (ସନ୍ଦେଶ));
  ଅନ୍ୟଥା
    {
      cleanup_free ଅକ୍ଷର *tmp = ଖାଲି;

      xasprintf (&tmp, "%s: %s", ସନ୍ଦେଶ, strerror (errno_));
      json_gen_string (gen, tmp, strlen (tmp));
    }

  json_gen_string (gen, "level", strlen ("level"));
  json_gen_string (gen, level, strlen (level));

  json_gen_string (gen, "time", strlen ("time"));
  json_gen_string (gen, timestamp, strlen (timestamp));

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);
  ଯଦି (buf)
    ret = strdup (buf);

  json_gen_free (gen);

  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ
write_log (ପୂର୍ଣ୍ଣସଂଖ୍ୟା errno_, ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, va_list args_list)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  cleanup_free ଅକ୍ଷର *output = ଖାଲି;
  cleanup_free ଅକ୍ଷର *json = ଖାଲି;

  ଯଦି (verbosity > output_verbosity)
    ଫେରନ୍ତୁ;

  ret = vasprintf (&output, ସନ୍ଦେଶ, args_list);
  ଯଦି (UNLIKELY (ret < 0))
    OOM ();

  ଯଦି (verbosity == LIBCRUN_VERBOSITY_ERROR && output_handler != log_write_to_stderr)
    log_write_to_stderr (errno_, output, LIBCRUN_VERBOSITY_ERROR, ଖାଲି);

  ବାଛ (log_format)
    {
    ସ୍ଥିତି LOG_FORMAT_TEXT:
      output_handler (errno_, output, verbosity, output_handler_arg);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି LOG_FORMAT_JSON:
      json = make_json_error (output, errno_, verbosity);
      ଯଦି (json)
        output_handler (0, json, verbosity, output_handler_arg);
      ଅନ୍ୟଥା
        output_handler (errno_, output, verbosity, output_handler_arg);
      ଭାଙ୍ଗ;
    }
}

ଶୂନ୍ୟ
libcrun_debug (ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ...)
{
  va_list args_list;
  va_start (args_list, ସନ୍ଦେଶ);
  write_log (0, LIBCRUN_VERBOSITY_DEBUG, ସନ୍ଦେଶ, args_list);
  va_end (args_list);
}

ଶୂନ୍ୟ
libcrun_warning (ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ...)
{
  va_list args_list;
  va_start (args_list, ସନ୍ଦେଶ);
  write_log (0, LIBCRUN_VERBOSITY_WARNING, ସନ୍ଦେଶ, args_list);
  va_end (args_list);
}

ଶୂନ୍ୟ
libcrun_error (ପୂର୍ଣ୍ଣସଂଖ୍ୟା errno_, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ...)
{
  va_list args_list;
  va_start (args_list, ସନ୍ଦେଶ);

  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, ସନ୍ଦେଶ, args_list);
  va_end (args_list);
}

ଶୂନ୍ୟ __attribute__ ((noreturn))
libcrun_fail_with_error (ପୂର୍ଣ୍ଣସଂଖ୍ୟା errno_, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ...)
{
  va_list args_list;
  va_start (args_list, ସନ୍ଦେଶ);
  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, ସନ୍ଦେଶ, args_list);
  va_end (args_list);
  exit (EXIT_FAILURE);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_set_log_format (ଅଚଳ ଅକ୍ଷର *format, libcrun_error_t *err)
{
  ଯଦି (strcmp (format, "text") == 0)
    log_format = LOG_FORMAT_TEXT;
  ଅନ୍ୟଥା ଯଦି (strcmp (format, "json") == 0)
    log_format = LOG_FORMAT_JSON;
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ crun_make_error (err, 0, "unknown log format type `%s`", format);

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
json_gen_error_to_crun_error (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status, libcrun_error_t *err)
{
  ବାଛ (status)
    {
    ସ୍ଥିତି json_gen_status_ok:
      ଫେରନ୍ତୁ 0;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ crun_make_error (err, 0, "generate JSON document: error %d", status);
    }
}
