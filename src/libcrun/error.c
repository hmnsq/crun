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

स्थिर पूर्णांक log_format;
स्थिर पूर्णांक output_verbosity = LIBCRUN_VERBOSITY_ERROR;

पूर्णांक
libcrun_make_error (libcrun_error_t *err, पूर्णांक status, स्थिरांक अक्षर *संदेश, ...)
{
  बहु_सूची args_list;
  libcrun_error_t ptr;

  बहु_शुरू (args_list, संदेश);
  *err = xmalloc (आकार (संरचना libcrun_error_s));
  ptr = *err;
  ptr->status = status;
  जर (vasprintf (&(ptr->संदेश), संदेश, args_list) < 0)
    OOM ();
  बहु_पूर्ण (args_list);

  परत -status - 1;
}

पूर्णांक
crun_error_wrap (libcrun_error_t *err, स्थिरांक अक्षर *fmt, ...)
{
  cleanup_free अक्षर *संदेश = रिक्त;
  cleanup_free अक्षर *tmp = रिक्त;
  बहु_सूची args_list;
  अक्षर *swap;
  पूर्णांक ret;

  जर (err == रिक्त || *err == रिक्त)
    {
      // Internal error
      परत 0;
    }

  ret = -(*err)->status - 1;

  बहु_शुरू (args_list, fmt);

  जर (vasprintf (&संदेश, fmt, args_list) < 0)
    {
      बहु_पूर्ण (args_list);
      संदेश = रिक्त;
      परत ret;
    }
  बहु_पूर्ण (args_list);

  xasprintf (&tmp, "%s: %s", संदेश, (*err)->संदेश);
  swap = tmp;
  tmp = (*err)->संदेश;
  (*err)->संदेश = swap;

  परत ret;
}

पूर्णांक
crun_error_release (libcrun_error_t *err)
{
  स्थिरांक पूर्णांक saved_errno = त्रुटि_सं;
  libcrun_error_t ptr;

  जर (err == रिक्त)
    परत 0;

  ptr = *err;
  जर (ptr == रिक्त)
    परत 0;

  मुक्त (ptr->संदेश);
  मुक्त (ptr);
  *err = रिक्त;

  त्रुटि_सं = saved_errno;
  परत 0;
}

पूर्णांक
libcrun_error_release (libcrun_error_t *err)
{
  परत crun_error_release (err);
}

पूर्णांक
libcrun_error_get_status (libcrun_error_t err)
{
  जर (err == रिक्त)
    परत 0;
  परत err->status;
}

स्थिरांक अक्षर *
libcrun_error_get_message (libcrun_error_t err)
{
  जर (err == रिक्त)
    परत रिक्त;
  परत err->संदेश;
}

शून्य
crun_error_write_warning_and_release (खाता *out, libcrun_error_t **err)
{
  स्थिरांक पूर्णांक saved_errno = त्रुटि_सं;
  libcrun_error_t ref;

  जर (out == रिक्त)
    out = मानक_त्रुटि;
  जर (err == रिक्त || *err == रिक्त || **err == रिक्त)
    {
      // Internal error
      परत;
    }

  ref = **err;
  जर (ref->status)
    ख_लिखो (out, "%s: %s\n", ref->संदेश, म_त्रुटि (ref->status));
  अन्यथा
    ख_लिखो (out, "%s\n", ref->संदेश);

  मुक्त (ref->संदेश);
  मुक्त (ref);
  **err = रिक्त;

  त्रुटि_सं = saved_errno;
}

शून्य
libcrun_error_write_warning_and_release (खाता *out, libcrun_error_t **err)
{
  परत crun_error_write_warning_and_release (out, err);
}

शून्य
libcrun_error_report_and_release (libcrun_error_t *err)
{
  libcrun_error_t ref;

  जर (err == रिक्त || *err == रिक्त)
    परत;

  ref = *err;
  libcrun_error (ref->status, "%s", ref->संदेश);
  crun_error_release (err);
}

पूर्णांक
crun_error_get_errno (libcrun_error_t *err)
{
  जर (err == रिक्त || *err == रिक्त)
    परत 0;
  परत (*err)->status;
}

प्रकारव्याख्या अक्षर timestamp_t[64];

स्थिर शून्य
get_timestamp (timestamp_t *timestamp, स्थिरांक अक्षर *suffix)
{
  स्थिरांक माप_प्रकार buffer_size = आकार (timestamp_t);
  अक्षर *buffer = (अक्षर *) timestamp;
  संरचना timeval tv;
  माप_प्रकार written;
  संरचना पंचांग now;

  gettimeofday (&tv, रिक्त);
  gmtime_r (&tv.tv_sec, &now);
  written = स_माला (buffer, buffer_size, "%Y-%m-%dT%H:%M:%S", &now);
  snprintf (buffer + written, buffer_size - written, ".%06lldZ%.8s", (दीर्घ दीर्घ पूर्णांक) tv.tv_usec, suffix);
}

/* Neither openlog(3) nor the journald handler copy the identifier they are
   given, and the global output handler outlives the caller's strings (e.g.
   the context is freed while errors are still reported through it), so keep
   an owned copy alive for as long as the handler can use it.  */
स्थिर अक्षर *log_id;

स्थिर स्थिरांक अक्षर *
set_log_id (स्थिरांक अक्षर *id, अक्षर **old_id)
{
  *old_id = log_id;
  log_id = id ? xstrdup (id) : रिक्त;
  परत log_id;
}

स्थिर शून्य *
init_syslog (स्थिरांक अक्षर *id)
{
  openlog (id, 0, LOG_USER);
  परत रिक्त;
}

गणना
{
  LOG_TYPE_FILE = 1,
  LOG_TYPE_SYSLOG = 2,
  LOG_TYPE_JOURNALD = 3
};

स्थिर पूर्णांक
get_log_type (स्थिरांक अक्षर *log, स्थिरांक अक्षर **data)
{
  स्थिरांक अक्षर *sep = म_अक्षर (log, ':');
  जर (sep == रिक्त)
    {
      *data = log;
      परत LOG_TYPE_FILE;
    }
  *data = sep + 1;

  जर (has_prefix (log, "syslog:"))
    परत LOG_TYPE_SYSLOG;
  जर (has_prefix (log, "journald:"))
    परत LOG_TYPE_JOURNALD;
  जर (has_prefix (log, "file:"))
    परत LOG_TYPE_FILE;

  परत -1;
}

पूर्णांक
libcrun_init_logging (crun_output_handler *new_output_handler, शून्य **new_output_handler_arg, स्थिरांक अक्षर *id,
                      स्थिरांक अक्षर *log, libcrun_error_t *err)
{
  /* Released only after the new handler is installed.  */
  cleanup_free अक्षर *old_log_id = रिक्त;

  जर (log == रिक्त)
    {
      *new_output_handler = log_write_to_stderr;
      *new_output_handler_arg = रिक्त;
    }
  अन्यथा
    {
      स्थिरांक अक्षर *arg = रिक्त;
      पूर्णांक log_type = get_log_type (log, &arg);

      जर (log_type < 0)
        परत crun_make_error (err, त्रुटि_सं, "unknown log type `%s`", log);

      निवड (log_type)
        {
        स्थिती LOG_TYPE_FILE:
          *new_output_handler = log_write_to_stream;
          *new_output_handler_arg = ख_खोलो (arg, "a+e");
          जर (*new_output_handler_arg == रिक्त)
            परत crun_make_error (err, त्रुटि_सं, "open log file `%s`", log);
          जर (output_verbosity >= LIBCRUN_VERBOSITY_WARNING)
            setlinebuf (*new_output_handler_arg);
          थांबा;

        स्थिती LOG_TYPE_SYSLOG:
          *new_output_handler_arg = init_syslog (set_log_id (arg[0] ? arg : id, &old_log_id));
          *new_output_handler = log_write_to_syslog;
          थांबा;

        स्थिती LOG_TYPE_JOURNALD:
          *new_output_handler = log_write_to_journald;
          *new_output_handler_arg = (शून्य *) set_log_id (id, &old_log_id);
          थांबा;
        }
    }
  crun_set_output_handler (*new_output_handler, *new_output_handler_arg);
  परत 0;
}

शून्य
log_write_to_stream (पूर्णांक errno_, स्थिरांक अक्षर *संदेश, पूर्णांक verbosity, शून्य *arg)
{
  timestamp_t timestamp = {
    0,
  };
  खाता *stream = arg;
  पूर्णांक tty = isatty (fileno (stream));
  स्थिरांक अक्षर *color_begin = "";
  स्थिरांक अक्षर *color_end = "";

  जर (tty)
    {
      निवड (verbosity)
        {
        स्थिती LIBCRUN_VERBOSITY_DEBUG:
          color_begin = "\x1b[1;34m";
          color_end = "\x1b[0m";
          थांबा;
        स्थिती LIBCRUN_VERBOSITY_WARNING:
          color_begin = "\x1b[1;33m";
          color_end = "\x1b[0m";
          थांबा;
        स्थिती LIBCRUN_VERBOSITY_ERROR:
          color_begin = "\x1b[1;31m";
          color_end = "\x1b[0m";
          थांबा;
        }

      जर (log_format == LOG_FORMAT_TEXT)
        get_timestamp (&timestamp, ": ");
    }

  जर (errno_)
    ख_लिखो (stream, "%s%s%s: %s%s\n", color_begin, timestamp, संदेश, म_त्रुटि (errno_), color_end);
  अन्यथा
    ख_लिखो (stream, "%s%s%s%s\n", color_begin, timestamp, संदेश, color_end);
}

शून्य
log_write_to_stderr (पूर्णांक errno_, स्थिरांक अक्षर *संदेश, पूर्णांक verbosity, शून्य *arg arg_unused)
{
  log_write_to_stream (errno_, संदेश, verbosity, मानक_त्रुटि);
}

शून्य
log_write_to_syslog (पूर्णांक errno_, स्थिरांक अक्षर *संदेश, पूर्णांक verbosity, शून्य *arg arg_unused)
{
  पूर्णांक priority = LOG_ERR;
  निवड (verbosity)
    {
    स्थिती LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      थांबा;
    स्थिती LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      थांबा;
    स्थिती LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      थांबा;
    }
  जर (errno_ == 0)
    syslog (priority, "%s", संदेश);
  अन्यथा
    syslog (priority, "%s: %s", संदेश, म_त्रुटि (errno_));
}

शून्य
log_write_to_journald (पूर्णांक errno_, स्थिरांक अक्षर *संदेश, पूर्णांक verbosity, शून्य *arg arg_unused)
{
  (शून्य) errno_;
  (शून्य) संदेश;
  (शून्य) verbosity;
#ifdef HAVE_SYSTEMD
  पूर्णांक priority = LOG_ERR;
  निवड (verbosity)
    {
    स्थिती LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      थांबा;
    स्थिती LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      थांबा;
    स्थिती LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      थांबा;
    }
  जर (errno_ == 0)
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s", संदेश, "ID=%s", arg, रिक्त);
  अन्यथा
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s: %s", संदेश, म_त्रुटि (errno_), "ID=%s",
                     arg, रिक्त);
#endif
}

स्थिर crun_output_handler output_handler = log_write_to_stderr;
स्थिर शून्य *output_handler_arg = रिक्त;

शून्य
libcrun_set_verbosity (पूर्णांक verbosity)
{
  output_verbosity = verbosity;
}

पूर्णांक
libcrun_get_verbosity ()
{
  परत output_verbosity;
}

शून्य
crun_set_output_handler (crun_output_handler handler, शून्य *arg)
{
  output_handler = handler;
  output_handler_arg = arg;
}

स्थिर अक्षर *
make_json_error (स्थिरांक अक्षर *संदेश, पूर्णांक errno_, पूर्णांक verbosity)
{
  स्थिरांक अक्षर *level;
  निवड (verbosity)
    {
    स्थिती LIBCRUN_VERBOSITY_DEBUG:
      level = "debug";
      थांबा;
    स्थिती LIBCRUN_VERBOSITY_WARNING:
      level = "warning";
      थांबा;
    स्थिती LIBCRUN_VERBOSITY_ERROR:
      level = "error";
      थांबा;
    }
  स्थिरांक अक्षर *buf = रिक्त;
  json_gen_ctx *gen = रिक्त;
  अक्षर *ret = रिक्त;
  माप_प्रकार buf_len;
  timestamp_t timestamp = {
    0,
  };

  जर (! json_gen_init (&gen, रिक्त))
    परत रिक्त;

  get_timestamp (&timestamp, "");

  json_gen_map_open (gen);

  json_gen_string (gen, "msg", म_माप ("msg"));
  जर (errno_ == 0)
    json_gen_string (gen, संदेश, म_माप (संदेश));
  अन्यथा
    {
      cleanup_free अक्षर *tmp = रिक्त;

      xasprintf (&tmp, "%s: %s", संदेश, म_त्रुटि (errno_));
      json_gen_string (gen, tmp, म_माप (tmp));
    }

  json_gen_string (gen, "level", म_माप ("level"));
  json_gen_string (gen, level, म_माप (level));

  json_gen_string (gen, "time", म_माप ("time"));
  json_gen_string (gen, timestamp, म_माप (timestamp));

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);
  जर (buf)
    ret = strdup (buf);

  json_gen_free (gen);

  परत ret;
}

स्थिर शून्य
write_log (पूर्णांक errno_, पूर्णांक verbosity, स्थिरांक अक्षर *संदेश, बहु_सूची args_list)
{
  पूर्णांक ret;
  cleanup_free अक्षर *output = रिक्त;
  cleanup_free अक्षर *json = रिक्त;

  जर (verbosity > output_verbosity)
    परत;

  ret = vasprintf (&output, संदेश, args_list);
  जर (UNLIKELY (ret < 0))
    OOM ();

  जर (verbosity == LIBCRUN_VERBOSITY_ERROR && output_handler != log_write_to_stderr)
    log_write_to_stderr (errno_, output, LIBCRUN_VERBOSITY_ERROR, रिक्त);

  निवड (log_format)
    {
    स्थिती LOG_FORMAT_TEXT:
      output_handler (errno_, output, verbosity, output_handler_arg);
      थांबा;

    स्थिती LOG_FORMAT_JSON:
      json = make_json_error (output, errno_, verbosity);
      जर (json)
        output_handler (0, json, verbosity, output_handler_arg);
      अन्यथा
        output_handler (errno_, output, verbosity, output_handler_arg);
      थांबा;
    }
}

शून्य
libcrun_debug (स्थिरांक अक्षर *संदेश, ...)
{
  बहु_सूची args_list;
  बहु_शुरू (args_list, संदेश);
  write_log (0, LIBCRUN_VERBOSITY_DEBUG, संदेश, args_list);
  बहु_पूर्ण (args_list);
}

शून्य
libcrun_warning (स्थिरांक अक्षर *संदेश, ...)
{
  बहु_सूची args_list;
  बहु_शुरू (args_list, संदेश);
  write_log (0, LIBCRUN_VERBOSITY_WARNING, संदेश, args_list);
  बहु_पूर्ण (args_list);
}

शून्य
libcrun_error (पूर्णांक errno_, स्थिरांक अक्षर *संदेश, ...)
{
  बहु_सूची args_list;
  बहु_शुरू (args_list, संदेश);

  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, संदेश, args_list);
  बहु_पूर्ण (args_list);
}

शून्य __attribute__ ((noreturn))
libcrun_fail_with_error (पूर्णांक errno_, स्थिरांक अक्षर *संदेश, ...)
{
  बहु_सूची args_list;
  बहु_शुरू (args_list, संदेश);
  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, संदेश, args_list);
  बहु_पूर्ण (args_list);
  निकास (निकास_त्रुटि);
}

पूर्णांक
libcrun_set_log_format (स्थिरांक अक्षर *format, libcrun_error_t *err)
{
  जर (म_भेद (format, "text") == 0)
    log_format = LOG_FORMAT_TEXT;
  अन्यथा जर (म_भेद (format, "json") == 0)
    log_format = LOG_FORMAT_JSON;
  अन्यथा
    परत crun_make_error (err, 0, "unknown log format type `%s`", format);

  परत 0;
}

पूर्णांक
json_gen_error_to_crun_error (पूर्णांक status, libcrun_error_t *err)
{
  निवड (status)
    {
    स्थिती json_gen_status_ok:
      परत 0;

    पूर्वनिर्धारित:
      परत crun_make_error (err, 0, "generate JSON document: error %d", status);
    }
}
