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

#परिभाषय _GNU_SOURCE
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

#यदि_परिभाषितम् HAVE_SYSTEMD
#  include <systemd/sd-journal.h>
#यदि_अन्तः

गणना
{
  LOG_FORMAT_TEXT = 0,
  LOG_FORMAT_JSON,
};

स्थिर पूर्णाङ्क log_format;
स्थिर पूर्णाङ्क output_verbosity = LIBCRUN_VERBOSITY_ERROR;

पूर्णाङ्क
libcrun_make_error (libcrun_error_t *err, पूर्णाङ्क स्थितिः, ध्रुव वर्ण *सन्देशः, ...)
{
  va_list तर्काः_list;
  libcrun_error_t ptr;

  va_start (तर्काः_list, सन्देशः);
  *err = xmalloc (परिमाण (संरचना libcrun_error_s));
  ptr = *err;
  ptr->स्थितिः = स्थितिः;
  यदि (vasprintf (&(ptr->सन्देशः), सन्देशः, तर्काः_list) < 0)
    OOM ();
  va_अन्तः (तर्काः_list);

  प्रत्यागम -स्थितिः - 1;
}

पूर्णाङ्क
crun_error_wrap (libcrun_error_t *err, ध्रुव वर्ण *fmt, ...)
{
  cleanup_free वर्ण *सन्देशः = रिक्त;
  cleanup_free वर्ण *tmp = रिक्त;
  va_list तर्काः_list;
  वर्ण *swap;
  पूर्णाङ्क ret;

  यदि (err == रिक्त || *err == रिक्त)
    {
      // Internal error
      प्रत्यागम 0;
    }

  ret = -(*err)->स्थितिः - 1;

  va_start (तर्काः_list, fmt);

  यदि (vasprintf (&सन्देशः, fmt, तर्काः_list) < 0)
    {
      va_अन्तः (तर्काः_list);
      सन्देशः = रिक्त;
      प्रत्यागम ret;
    }
  va_अन्तः (तर्काः_list);

  xasprintf (&tmp, "%s: %s", सन्देशः, (*err)->सन्देशः);
  swap = tmp;
  tmp = (*err)->सन्देशः;
  (*err)->सन्देशः = swap;

  प्रत्यागम ret;
}

पूर्णाङ्क
crun_error_release (libcrun_error_t *err)
{
  ध्रुव पूर्णाङ्क saved_errno = errno;
  libcrun_error_t ptr;

  यदि (err == रिक्त)
    प्रत्यागम 0;

  ptr = *err;
  यदि (ptr == रिक्त)
    प्रत्यागम 0;

  मुक्त (ptr->सन्देशः);
  मुक्त (ptr);
  *err = रिक्त;

  errno = saved_errno;
  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_error_release (libcrun_error_t *err)
{
  प्रत्यागम crun_error_release (err);
}

पूर्णाङ्क
libcrun_error_get_स्थितिः (libcrun_error_t err)
{
  यदि (err == रिक्त)
    प्रत्यागम 0;
  प्रत्यागम err->स्थितिः;
}

ध्रुव वर्ण *
libcrun_error_get_सन्देशः (libcrun_error_t err)
{
  यदि (err == रिक्त)
    प्रत्यागम रिक्त;
  प्रत्यागम err->सन्देशः;
}

शून्य
crun_error_write_warning_and_release (FILE *बहिर्गमनम्, libcrun_error_t **err)
{
  ध्रुव पूर्णाङ्क saved_errno = errno;
  libcrun_error_t ref;

  यदि (बहिर्गमनम् == रिक्त)
    बहिर्गमनम् = stderr;
  यदि (err == रिक्त || *err == रिक्त || **err == रिक्त)
    {
      // Internal error
      प्रत्यागम;
    }

  ref = **err;
  यदि (ref->स्थितिः)
    fprintf (बहिर्गमनम्, "%s: %s\n", ref->सन्देशः, strerror (ref->स्थितिः));
  अन्यथा
    fprintf (बहिर्गमनम्, "%s\n", ref->सन्देशः);

  मुक्त (ref->सन्देशः);
  मुक्त (ref);
  **err = रिक्त;

  errno = saved_errno;
}

शून्य
libcrun_error_write_warning_and_release (FILE *बहिर्गमनम्, libcrun_error_t **err)
{
  प्रत्यागम crun_error_write_warning_and_release (बहिर्गमनम्, err);
}

शून्य
libcrun_error_report_and_release (libcrun_error_t *err)
{
  libcrun_error_t ref;

  यदि (err == रिक्त || *err == रिक्त)
    प्रत्यागम;

  ref = *err;
  libcrun_error (ref->स्थितिः, "%s", ref->सन्देशः);
  crun_error_release (err);
}

पूर्णाङ्क
crun_error_get_errno (libcrun_error_t *err)
{
  यदि (err == रिक्त || *err == रिक्त)
    प्रत्यागम 0;
  प्रत्यागम (*err)->स्थितिः;
}

प्रकारनिर्देश वर्ण timestamp_t[64];

स्थिर शून्य
get_timestamp (timestamp_t *timestamp, ध्रुव वर्ण *suffix)
{
  ध्रुव size_t बफर_आकार = परिमाण (timestamp_t);
  वर्ण *बफर = (वर्ण *) timestamp;
  संरचना timeval tv;
  size_t written;
  संरचना tm now;

  gettimeofday (&tv, रिक्त);
  gmtime_r (&tv.tv_sec, &now);
  written = strftime (बफर, बफर_आकार, "%Y-%m-%dT%H:%M:%S", &now);
  snprintf (बफर + written, बफर_आकार - written, ".%06lldZ%.8s", (दीर्घ दीर्घ पूर्णाङ्क) tv.tv_usec, suffix);
}

/* Neither openlog(3) nor the journald handler copy the identifier they are
   given, and the global output handler outlives the caller's strings (e.g.
   the context is freed while errors are still reported through it), so keep
   an owned copy alive for as long as the handler can use it.  */
स्थिर वर्ण *log_id;

स्थिर ध्रुव वर्ण *
समुच्चयः_log_id (ध्रुव वर्ण *id, वर्ण **old_id)
{
  *old_id = log_id;
  log_id = id ? xstrdup (id) : रिक्त;
  प्रत्यागम log_id;
}

स्थिर शून्य *
init_syslog (ध्रुव वर्ण *id)
{
  openlog (id, 0, LOG_USER);
  प्रत्यागम रिक्त;
}

गणना
{
  LOG_TYPE_FILE = 1,
  LOG_TYPE_SYSLOG = 2,
  LOG_TYPE_JOURNALD = 3
};

स्थिर पूर्णाङ्क
get_log_type (ध्रुव वर्ण *लघुगणकः, ध्रुव वर्ण **दत्त)
{
  ध्रुव वर्ण *sep = strchr (लघुगणकः, ':');
  यदि (sep == रिक्त)
    {
      *दत्त = लघुगणकः;
      प्रत्यागम LOG_TYPE_FILE;
    }
  *दत्त = sep + 1;

  यदि (has_उपसर्गः (लघुगणकः, "syslog:"))
    प्रत्यागम LOG_TYPE_SYSLOG;
  यदि (has_उपसर्गः (लघुगणकः, "journald:"))
    प्रत्यागम LOG_TYPE_JOURNALD;
  यदि (has_उपसर्गः (लघुगणकः, "file:"))
    प्रत्यागम LOG_TYPE_FILE;

  प्रत्यागम -1;
}

पूर्णाङ्क
libcrun_init_logging (crun_output_handler *new_output_handler, शून्य **new_output_handler_arg, ध्रुव वर्ण *id,
                      ध्रुव वर्ण *लघुगणकः, libcrun_error_t *err)
{
  /* Released only after the new handler is installed.  */
  cleanup_free वर्ण *old_log_id = रिक्त;

  यदि (लघुगणकः == रिक्त)
    {
      *new_output_handler = log_write_गन्तव्यम्_stderr;
      *new_output_handler_arg = रिक्त;
    }
  अन्यथा
    {
      ध्रुव वर्ण *arg = रिक्त;
      पूर्णाङ्क log_type = get_log_type (लघुगणकः, &arg);

      यदि (log_type < 0)
        प्रत्यागम crun_make_error (err, errno, "unknown log type `%s`", लघुगणकः);

      विकल्प (log_type)
        {
        पक्ष LOG_TYPE_FILE:
          *new_output_handler = log_write_गन्तव्यम्_प्रवाहः;
          *new_output_handler_arg = fopen (arg, "a+e");
          यदि (*new_output_handler_arg == रिक्त)
            प्रत्यागम crun_make_error (err, errno, "open log file `%s`", लघुगणकः);
          यदि (output_verbosity >= LIBCRUN_VERBOSITY_WARNING)
            setlinebuf (*new_output_handler_arg);
          भङ्ग;

        पक्ष LOG_TYPE_SYSLOG:
          *new_output_handler_arg = init_syslog (समुच्चयः_log_id (arg[0] ? arg : id, &old_log_id));
          *new_output_handler = log_write_गन्तव्यम्_syslog;
          भङ्ग;

        पक्ष LOG_TYPE_JOURNALD:
          *new_output_handler = log_write_गन्तव्यम्_journald;
          *new_output_handler_arg = (शून्य *) समुच्चयः_log_id (id, &old_log_id);
          भङ्ग;
        }
    }
  crun_समुच्चयः_output_handler (*new_output_handler, *new_output_handler_arg);
  प्रत्यागम 0;
}

शून्य
log_write_गन्तव्यम्_प्रवाहः (पूर्णाङ्क errno_, ध्रुव वर्ण *सन्देशः, पूर्णाङ्क verbosity, शून्य *arg)
{
  timestamp_t timestamp = {
    0,
  };
  FILE *प्रवाहः = arg;
  पूर्णाङ्क tty = isatty (fileno (प्रवाहः));
  ध्रुव वर्ण *color_begin = "";
  ध्रुव वर्ण *color_अन्तः = "";

  यदि (tty)
    {
      विकल्प (verbosity)
        {
        पक्ष LIBCRUN_VERBOSITY_DEBUG:
          color_begin = "\x1b[1;34m";
          color_अन्तः = "\x1b[0m";
          भङ्ग;
        पक्ष LIBCRUN_VERBOSITY_WARNING:
          color_begin = "\x1b[1;33m";
          color_अन्तः = "\x1b[0m";
          भङ्ग;
        पक्ष LIBCRUN_VERBOSITY_ERROR:
          color_begin = "\x1b[1;31m";
          color_अन्तः = "\x1b[0m";
          भङ्ग;
        }

      यदि (log_format == LOG_FORMAT_TEXT)
        get_timestamp (&timestamp, ": ");
    }

  यदि (errno_)
    fprintf (प्रवाहः, "%s%s%s: %s%s\n", color_begin, timestamp, सन्देशः, strerror (errno_), color_अन्तः);
  अन्यथा
    fprintf (प्रवाहः, "%s%s%s%s\n", color_begin, timestamp, सन्देशः, color_अन्तः);
}

शून्य
log_write_गन्तव्यम्_stderr (पूर्णाङ्क errno_, ध्रुव वर्ण *सन्देशः, पूर्णाङ्क verbosity, शून्य *arg arg_अप्रयुक्तम्)
{
  log_write_गन्तव्यम्_प्रवाहः (errno_, सन्देशः, verbosity, stderr);
}

शून्य
log_write_गन्तव्यम्_syslog (पूर्णाङ्क errno_, ध्रुव वर्ण *सन्देशः, पूर्णाङ्क verbosity, शून्य *arg arg_अप्रयुक्तम्)
{
  पूर्णाङ्क प्राथमिकता = LOG_ERR;
  विकल्प (verbosity)
    {
    पक्ष LIBCRUN_VERBOSITY_DEBUG:
      प्राथमिकता = LOG_DEBUG;
      भङ्ग;
    पक्ष LIBCRUN_VERBOSITY_WARNING:
      प्राथमिकता = LOG_WARNING;
      भङ्ग;
    पक्ष LIBCRUN_VERBOSITY_ERROR:
      प्राथमिकता = LOG_ERR;
      भङ्ग;
    }
  यदि (errno_ == 0)
    syslog (प्राथमिकता, "%s", सन्देशः);
  अन्यथा
    syslog (प्राथमिकता, "%s: %s", सन्देशः, strerror (errno_));
}

शून्य
log_write_गन्तव्यम्_journald (पूर्णाङ्क errno_, ध्रुव वर्ण *सन्देशः, पूर्णाङ्क verbosity, शून्य *arg arg_अप्रयुक्तम्)
{
  (शून्य) errno_;
  (शून्य) सन्देशः;
  (शून्य) verbosity;
#यदि_परिभाषितम् HAVE_SYSTEMD
  पूर्णाङ्क प्राथमिकता = LOG_ERR;
  विकल्प (verbosity)
    {
    पक्ष LIBCRUN_VERBOSITY_DEBUG:
      प्राथमिकता = LOG_DEBUG;
      भङ्ग;
    पक्ष LIBCRUN_VERBOSITY_WARNING:
      प्राथमिकता = LOG_WARNING;
      भङ्ग;
    पक्ष LIBCRUN_VERBOSITY_ERROR:
      प्राथमिकता = LOG_ERR;
      भङ्ग;
    }
  यदि (errno_ == 0)
    sd_journal_send ("PRIORITY=%d", प्राथमिकता, "MESSAGE=%s", सन्देशः, "ID=%s", arg, रिक्त);
  अन्यथा
    sd_journal_send ("PRIORITY=%d", प्राथमिकता, "MESSAGE=%s: %s", सन्देशः, strerror (errno_), "ID=%s",
                     arg, रिक्त);
#यदि_अन्तः
}

स्थिर crun_output_handler output_handler = log_write_गन्तव्यम्_stderr;
स्थिर शून्य *output_handler_arg = रिक्त;

शून्य
libcrun_समुच्चयः_verbosity (पूर्णाङ्क verbosity)
{
  output_verbosity = verbosity;
}

पूर्णाङ्क
libcrun_get_verbosity ()
{
  प्रत्यागम output_verbosity;
}

शून्य
crun_समुच्चयः_output_handler (crun_output_handler handler, शून्य *arg)
{
  output_handler = handler;
  output_handler_arg = arg;
}

स्थिर वर्ण *
make_json_error (ध्रुव वर्ण *सन्देशः, पूर्णाङ्क errno_, पूर्णाङ्क verbosity)
{
  ध्रुव वर्ण *level;
  विकल्प (verbosity)
    {
    पक्ष LIBCRUN_VERBOSITY_DEBUG:
      level = "debug";
      भङ्ग;
    पक्ष LIBCRUN_VERBOSITY_WARNING:
      level = "warning";
      भङ्ग;
    पक्ष LIBCRUN_VERBOSITY_ERROR:
      level = "error";
      भङ्ग;
    }
  ध्रुव वर्ण *buf = रिक्त;
  json_gen_ctx *gen = रिक्त;
  वर्ण *ret = रिक्त;
  size_t buf_len;
  timestamp_t timestamp = {
    0,
  };

  यदि (! json_gen_init (&gen, रिक्त))
    प्रत्यागम रिक्त;

  get_timestamp (&timestamp, "");

  json_gen_map_open (gen);

  json_gen_string (gen, "msg", वाक्यदैर्घ्यम् ("msg"));
  यदि (errno_ == 0)
    json_gen_string (gen, सन्देशः, वाक्यदैर्घ्यम् (सन्देशः));
  अन्यथा
    {
      cleanup_free वर्ण *tmp = रिक्त;

      xasprintf (&tmp, "%s: %s", सन्देशः, strerror (errno_));
      json_gen_string (gen, tmp, वाक्यदैर्घ्यम् (tmp));
    }

  json_gen_string (gen, "level", वाक्यदैर्घ्यम् ("level"));
  json_gen_string (gen, level, वाक्यदैर्घ्यम् (level));

  json_gen_string (gen, "time", वाक्यदैर्घ्यम् ("time"));
  json_gen_string (gen, timestamp, वाक्यदैर्घ्यम् (timestamp));

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);
  यदि (buf)
    ret = strdup (buf);

  json_gen_free (gen);

  प्रत्यागम ret;
}

स्थिर शून्य
write_log (पूर्णाङ्क errno_, पूर्णाङ्क verbosity, ध्रुव वर्ण *सन्देशः, va_list तर्काः_list)
{
  पूर्णाङ्क ret;
  cleanup_free वर्ण *output = रिक्त;
  cleanup_free वर्ण *json = रिक्त;

  यदि (verbosity > output_verbosity)
    प्रत्यागम;

  ret = vasprintf (&output, सन्देशः, तर्काः_list);
  यदि (UNLIKELY (ret < 0))
    OOM ();

  यदि (verbosity == LIBCRUN_VERBOSITY_ERROR && output_handler != log_write_गन्तव्यम्_stderr)
    log_write_गन्तव्यम्_stderr (errno_, output, LIBCRUN_VERBOSITY_ERROR, रिक्त);

  विकल्प (log_format)
    {
    पक्ष LOG_FORMAT_TEXT:
      output_handler (errno_, output, verbosity, output_handler_arg);
      भङ्ग;

    पक्ष LOG_FORMAT_JSON:
      json = make_json_error (output, errno_, verbosity);
      यदि (json)
        output_handler (0, json, verbosity, output_handler_arg);
      अन्यथा
        output_handler (errno_, output, verbosity, output_handler_arg);
      भङ्ग;
    }
}

शून्य
libcrun_debug (ध्रुव वर्ण *सन्देशः, ...)
{
  va_list तर्काः_list;
  va_start (तर्काः_list, सन्देशः);
  write_log (0, LIBCRUN_VERBOSITY_DEBUG, सन्देशः, तर्काः_list);
  va_अन्तः (तर्काः_list);
}

शून्य
libcrun_warning (ध्रुव वर्ण *सन्देशः, ...)
{
  va_list तर्काः_list;
  va_start (तर्काः_list, सन्देशः);
  write_log (0, LIBCRUN_VERBOSITY_WARNING, सन्देशः, तर्काः_list);
  va_अन्तः (तर्काः_list);
}

शून्य
libcrun_error (पूर्णाङ्क errno_, ध्रुव वर्ण *सन्देशः, ...)
{
  va_list तर्काः_list;
  va_start (तर्काः_list, सन्देशः);

  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, सन्देशः, तर्काः_list);
  va_अन्तः (तर्काः_list);
}

शून्य __attribute__ ((noreturn))
libcrun_fail_with_error (पूर्णाङ्क errno_, ध्रुव वर्ण *सन्देशः, ...)
{
  va_list तर्काः_list;
  va_start (तर्काः_list, सन्देशः);
  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, सन्देशः, तर्काः_list);
  va_अन्तः (तर्काः_list);
  निर्गच्छ (EXIT_FAILURE);
}

पूर्णाङ्क
libcrun_समुच्चयः_log_format (ध्रुव वर्ण *format, libcrun_error_t *err)
{
  यदि (वाक्यं_तुलय (format, "text") == 0)
    log_format = LOG_FORMAT_TEXT;
  अन्यथा यदि (वाक्यं_तुलय (format, "json") == 0)
    log_format = LOG_FORMAT_JSON;
  अन्यथा
    प्रत्यागम crun_make_error (err, 0, "unknown log format type `%s`", format);

  प्रत्यागम 0;
}

पूर्णाङ्क
json_gen_error_गन्तव्यम्_crun_error (पूर्णाङ्क स्थितिः, libcrun_error_t *err)
{
  विकल्प (स्थितिः)
    {
    पक्ष json_gen_स्थितिः_ok:
      प्रत्यागम 0;

    स्वभाव:
      प्रत्यागम crun_make_error (err, 0, "generate JSON document: error %d", स्थितिः);
    }
}
