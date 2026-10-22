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

#घोषणा _GNU_SOURCE
#समावेश <config.h>
#समावेश <stdarg.h>
#समावेश <unistd.h>
#समावेश <string.h>
#समावेश <time.h>
#समावेश <sys/time.h>
#समावेश <stdio.h>
#समावेश "utils.h"

#समावेश <json-c/json.h>
#समावेश <ocispec/json_common.h>

#अगर_घोषित HAVE_SYSTEMD
#  include <systemd/sd-journal.h>
#पूर्ण_अगर

क्रमागत
{
  LOG_FORMAT_TEXT = 0,
  LOG_FORMAT_JSON,
};

अटल पूर्णांक log_format;
अटल पूर्णांक output_verbosity = LIBCRUN_VERBOSITY_ERROR;

पूर्णांक
libcrun_make_error (libcrun_error_t *err, पूर्णांक status, स्थिर अक्षर *संदेश, ...)
{
  बहु_सूची args_list;
  libcrun_error_t ptr;

  बहु_शुरू (args_list, संदेश);
  *err = xmalloc (माप (काष्ठा libcrun_error_s));
  ptr = *err;
  ptr->status = status;
  अगर (vasprintf (&(ptr->संदेश), संदेश, args_list) < 0)
    OOM ();
  बहु_पूर्ण (args_list);

  वापस -status - 1;
}

पूर्णांक
crun_error_wrap (libcrun_error_t *err, स्थिर अक्षर *fmt, ...)
{
  cleanup_free अक्षर *संदेश = शून्य;
  cleanup_free अक्षर *tmp = शून्य;
  बहु_सूची args_list;
  अक्षर *swap;
  पूर्णांक ret;

  अगर (err == शून्य || *err == शून्य)
    {
      // Internal error
      वापस 0;
    }

  ret = -(*err)->status - 1;

  बहु_शुरू (args_list, fmt);

  अगर (vasprintf (&संदेश, fmt, args_list) < 0)
    {
      बहु_पूर्ण (args_list);
      संदेश = शून्य;
      वापस ret;
    }
  बहु_पूर्ण (args_list);

  xasprintf (&tmp, "%s: %s", संदेश, (*err)->संदेश);
  swap = tmp;
  tmp = (*err)->संदेश;
  (*err)->संदेश = swap;

  वापस ret;
}

पूर्णांक
crun_error_release (libcrun_error_t *err)
{
  स्थिर पूर्णांक saved_errno = त्रुटि_सं;
  libcrun_error_t ptr;

  अगर (err == शून्य)
    वापस 0;

  ptr = *err;
  अगर (ptr == शून्य)
    वापस 0;

  मुक्त (ptr->संदेश);
  मुक्त (ptr);
  *err = शून्य;

  त्रुटि_सं = saved_errno;
  वापस 0;
}

पूर्णांक
libcrun_error_release (libcrun_error_t *err)
{
  वापस crun_error_release (err);
}

पूर्णांक
libcrun_error_get_status (libcrun_error_t err)
{
  अगर (err == शून्य)
    वापस 0;
  वापस err->status;
}

स्थिर अक्षर *
libcrun_error_get_message (libcrun_error_t err)
{
  अगर (err == शून्य)
    वापस शून्य;
  वापस err->संदेश;
}

व्योम
crun_error_write_warning_and_release (खाता *out, libcrun_error_t **err)
{
  स्थिर पूर्णांक saved_errno = त्रुटि_सं;
  libcrun_error_t ref;

  अगर (out == शून्य)
    out = मानक_त्रुटि;
  अगर (err == शून्य || *err == शून्य || **err == शून्य)
    {
      // Internal error
      वापस;
    }

  ref = **err;
  अगर (ref->status)
    ख_लिखो (out, "%s: %s\n", ref->संदेश, म_त्रुटि (ref->status));
  अन्यथा
    ख_लिखो (out, "%s\n", ref->संदेश);

  मुक्त (ref->संदेश);
  मुक्त (ref);
  **err = शून्य;

  त्रुटि_सं = saved_errno;
}

व्योम
libcrun_error_write_warning_and_release (खाता *out, libcrun_error_t **err)
{
  वापस crun_error_write_warning_and_release (out, err);
}

व्योम
libcrun_error_report_and_release (libcrun_error_t *err)
{
  libcrun_error_t ref;

  अगर (err == शून्य || *err == शून्य)
    वापस;

  ref = *err;
  libcrun_error (ref->status, "%s", ref->संदेश);
  crun_error_release (err);
}

पूर्णांक
crun_error_get_errno (libcrun_error_t *err)
{
  अगर (err == शून्य || *err == शून्य)
    वापस 0;
  वापस (*err)->status;
}

प्रकार अक्षर timestamp_t[64];

अटल व्योम
get_timestamp (timestamp_t *timestamp, स्थिर अक्षर *suffix)
{
  स्थिर माप_प्रकार बफर_आकार = माप (timestamp_t);
  अक्षर *बफर = (अक्षर *) timestamp;
  काष्ठा timeval tv;
  माप_प्रकार written;
  काष्ठा पंचांग now;

  gettimeofday (&tv, शून्य);
  gmtime_r (&tv.tv_sec, &now);
  written = स_माला (बफर, बफर_आकार, "%Y-%m-%dT%H:%M:%S", &now);
  snprintf (बफर + written, बफर_आकार - written, ".%06lldZ%.8s", (दीर्घ दीर्घ पूर्णांक) tv.tv_usec, suffix);
}

/* Neither openlog(3) nor the journald handler copy the identifier they are
   given, and the global output handler outlives the caller's strings (e.g.
   the context is freed while errors are still reported through it), so keep
   an owned copy alive for as long as the handler can use it.  */
अटल अक्षर *log_id;

अटल स्थिर अक्षर *
set_log_id (स्थिर अक्षर *id, अक्षर **old_id)
{
  *old_id = log_id;
  log_id = id ? xstrdup (id) : शून्य;
  वापस log_id;
}

अटल व्योम *
init_syslog (स्थिर अक्षर *id)
{
  openlog (id, 0, LOG_USER);
  वापस शून्य;
}

क्रमागत
{
  LOG_TYPE_FILE = 1,
  LOG_TYPE_SYSLOG = 2,
  LOG_TYPE_JOURNALD = 3
};

अटल पूर्णांक
get_log_type (स्थिर अक्षर *log, स्थिर अक्षर **तथ्य)
{
  स्थिर अक्षर *sep = म_अक्षर (log, ':');
  अगर (sep == शून्य)
    {
      *तथ्य = log;
      वापस LOG_TYPE_FILE;
    }
  *तथ्य = sep + 1;

  अगर (has_prefix (log, "syslog:"))
    वापस LOG_TYPE_SYSLOG;
  अगर (has_prefix (log, "journald:"))
    वापस LOG_TYPE_JOURNALD;
  अगर (has_prefix (log, "file:"))
    वापस LOG_TYPE_FILE;

  वापस -1;
}

पूर्णांक
libcrun_init_logging (crun_output_handler *new_output_handler, व्योम **new_output_handler_arg, स्थिर अक्षर *id,
                      स्थिर अक्षर *log, libcrun_error_t *err)
{
  /* Released only after the new handler is installed.  */
  cleanup_free अक्षर *old_log_id = शून्य;

  अगर (log == शून्य)
    {
      *new_output_handler = log_write_to_stderr;
      *new_output_handler_arg = शून्य;
    }
  अन्यथा
    {
      स्थिर अक्षर *arg = शून्य;
      पूर्णांक log_type = get_log_type (log, &arg);

      अगर (log_type < 0)
        वापस crun_make_error (err, त्रुटि_सं, "unknown log type `%s`", log);

      चयन (log_type)
        {
        हाल LOG_TYPE_FILE:
          *new_output_handler = log_write_to_stream;
          *new_output_handler_arg = ख_खोलो (arg, "a+e");
          अगर (*new_output_handler_arg == शून्य)
            वापस crun_make_error (err, त्रुटि_सं, "open log file `%s`", log);
          अगर (output_verbosity >= LIBCRUN_VERBOSITY_WARNING)
            setlinebuf (*new_output_handler_arg);
          अवरोध;

        हाल LOG_TYPE_SYSLOG:
          *new_output_handler_arg = init_syslog (set_log_id (arg[0] ? arg : id, &old_log_id));
          *new_output_handler = log_write_to_syslog;
          अवरोध;

        हाल LOG_TYPE_JOURNALD:
          *new_output_handler = log_write_to_journald;
          *new_output_handler_arg = (व्योम *) set_log_id (id, &old_log_id);
          अवरोध;
        }
    }
  crun_set_output_handler (*new_output_handler, *new_output_handler_arg);
  वापस 0;
}

व्योम
log_write_to_stream (पूर्णांक errno_, स्थिर अक्षर *संदेश, पूर्णांक verbosity, व्योम *arg)
{
  timestamp_t timestamp = {
    0,
  };
  खाता *stream = arg;
  पूर्णांक tty = isatty (fileno (stream));
  स्थिर अक्षर *color_begin = "";
  स्थिर अक्षर *color_end = "";

  अगर (tty)
    {
      चयन (verbosity)
        {
        हाल LIBCRUN_VERBOSITY_DEBUG:
          color_begin = "\x1b[1;34m";
          color_end = "\x1b[0m";
          अवरोध;
        हाल LIBCRUN_VERBOSITY_WARNING:
          color_begin = "\x1b[1;33m";
          color_end = "\x1b[0m";
          अवरोध;
        हाल LIBCRUN_VERBOSITY_ERROR:
          color_begin = "\x1b[1;31m";
          color_end = "\x1b[0m";
          अवरोध;
        }

      अगर (log_format == LOG_FORMAT_TEXT)
        get_timestamp (&timestamp, ": ");
    }

  अगर (errno_)
    ख_लिखो (stream, "%s%s%s: %s%s\n", color_begin, timestamp, संदेश, म_त्रुटि (errno_), color_end);
  अन्यथा
    ख_लिखो (stream, "%s%s%s%s\n", color_begin, timestamp, संदेश, color_end);
}

व्योम
log_write_to_stderr (पूर्णांक errno_, स्थिर अक्षर *संदेश, पूर्णांक verbosity, व्योम *arg arg_unused)
{
  log_write_to_stream (errno_, संदेश, verbosity, मानक_त्रुटि);
}

व्योम
log_write_to_syslog (पूर्णांक errno_, स्थिर अक्षर *संदेश, पूर्णांक verbosity, व्योम *arg arg_unused)
{
  पूर्णांक priority = LOG_ERR;
  चयन (verbosity)
    {
    हाल LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      अवरोध;
    हाल LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      अवरोध;
    हाल LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      अवरोध;
    }
  अगर (errno_ == 0)
    syslog (priority, "%s", संदेश);
  अन्यथा
    syslog (priority, "%s: %s", संदेश, म_त्रुटि (errno_));
}

व्योम
log_write_to_journald (पूर्णांक errno_, स्थिर अक्षर *संदेश, पूर्णांक verbosity, व्योम *arg arg_unused)
{
  (व्योम) errno_;
  (व्योम) संदेश;
  (व्योम) verbosity;
#अगर_घोषित HAVE_SYSTEMD
  पूर्णांक priority = LOG_ERR;
  चयन (verbosity)
    {
    हाल LIBCRUN_VERBOSITY_DEBUG:
      priority = LOG_DEBUG;
      अवरोध;
    हाल LIBCRUN_VERBOSITY_WARNING:
      priority = LOG_WARNING;
      अवरोध;
    हाल LIBCRUN_VERBOSITY_ERROR:
      priority = LOG_ERR;
      अवरोध;
    }
  अगर (errno_ == 0)
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s", संदेश, "ID=%s", arg, शून्य);
  अन्यथा
    sd_journal_send ("PRIORITY=%d", priority, "MESSAGE=%s: %s", संदेश, म_त्रुटि (errno_), "ID=%s",
                     arg, शून्य);
#पूर्ण_अगर
}

अटल crun_output_handler output_handler = log_write_to_stderr;
अटल व्योम *output_handler_arg = शून्य;

व्योम
libcrun_set_verbosity (पूर्णांक verbosity)
{
  output_verbosity = verbosity;
}

पूर्णांक
libcrun_get_verbosity ()
{
  वापस output_verbosity;
}

व्योम
crun_set_output_handler (crun_output_handler handler, व्योम *arg)
{
  output_handler = handler;
  output_handler_arg = arg;
}

अटल अक्षर *
make_json_error (स्थिर अक्षर *संदेश, पूर्णांक errno_, पूर्णांक verbosity)
{
  स्थिर अक्षर *level;
  चयन (verbosity)
    {
    हाल LIBCRUN_VERBOSITY_DEBUG:
      level = "debug";
      अवरोध;
    हाल LIBCRUN_VERBOSITY_WARNING:
      level = "warning";
      अवरोध;
    हाल LIBCRUN_VERBOSITY_ERROR:
      level = "error";
      अवरोध;
    }
  स्थिर अक्षर *buf = शून्य;
  json_gen_ctx *gen = शून्य;
  अक्षर *ret = शून्य;
  माप_प्रकार buf_len;
  timestamp_t timestamp = {
    0,
  };

  अगर (! json_gen_init (&gen, शून्य))
    वापस शून्य;

  get_timestamp (&timestamp, "");

  json_gen_map_open (gen);

  json_gen_string (gen, "msg", म_माप ("msg"));
  अगर (errno_ == 0)
    json_gen_string (gen, संदेश, म_माप (संदेश));
  अन्यथा
    {
      cleanup_free अक्षर *tmp = शून्य;

      xasprintf (&tmp, "%s: %s", संदेश, म_त्रुटि (errno_));
      json_gen_string (gen, tmp, म_माप (tmp));
    }

  json_gen_string (gen, "level", म_माप ("level"));
  json_gen_string (gen, level, म_माप (level));

  json_gen_string (gen, "time", म_माप ("time"));
  json_gen_string (gen, timestamp, म_माप (timestamp));

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);
  अगर (buf)
    ret = strdup (buf);

  json_gen_free (gen);

  वापस ret;
}

अटल व्योम
write_log (पूर्णांक errno_, पूर्णांक verbosity, स्थिर अक्षर *संदेश, बहु_सूची args_list)
{
  पूर्णांक ret;
  cleanup_free अक्षर *output = शून्य;
  cleanup_free अक्षर *json = शून्य;

  अगर (verbosity > output_verbosity)
    वापस;

  ret = vasprintf (&output, संदेश, args_list);
  अगर (UNLIKELY (ret < 0))
    OOM ();

  अगर (verbosity == LIBCRUN_VERBOSITY_ERROR && output_handler != log_write_to_stderr)
    log_write_to_stderr (errno_, output, LIBCRUN_VERBOSITY_ERROR, शून्य);

  चयन (log_format)
    {
    हाल LOG_FORMAT_TEXT:
      output_handler (errno_, output, verbosity, output_handler_arg);
      अवरोध;

    हाल LOG_FORMAT_JSON:
      json = make_json_error (output, errno_, verbosity);
      अगर (json)
        output_handler (0, json, verbosity, output_handler_arg);
      अन्यथा
        output_handler (errno_, output, verbosity, output_handler_arg);
      अवरोध;
    }
}

व्योम
libcrun_debug (स्थिर अक्षर *संदेश, ...)
{
  बहु_सूची args_list;
  बहु_शुरू (args_list, संदेश);
  write_log (0, LIBCRUN_VERBOSITY_DEBUG, संदेश, args_list);
  बहु_पूर्ण (args_list);
}

व्योम
libcrun_warning (स्थिर अक्षर *संदेश, ...)
{
  बहु_सूची args_list;
  बहु_शुरू (args_list, संदेश);
  write_log (0, LIBCRUN_VERBOSITY_WARNING, संदेश, args_list);
  बहु_पूर्ण (args_list);
}

व्योम
libcrun_error (पूर्णांक errno_, स्थिर अक्षर *संदेश, ...)
{
  बहु_सूची args_list;
  बहु_शुरू (args_list, संदेश);

  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, संदेश, args_list);
  बहु_पूर्ण (args_list);
}

व्योम __attribute__ ((noreturn))
libcrun_fail_with_error (पूर्णांक errno_, स्थिर अक्षर *संदेश, ...)
{
  बहु_सूची args_list;
  बहु_शुरू (args_list, संदेश);
  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, संदेश, args_list);
  बहु_पूर्ण (args_list);
  निकास (निकास_त्रुटि);
}

पूर्णांक
libcrun_set_log_format (स्थिर अक्षर *format, libcrun_error_t *err)
{
  अगर (म_भेद (format, "text") == 0)
    log_format = LOG_FORMAT_TEXT;
  अन्यथा अगर (म_भेद (format, "json") == 0)
    log_format = LOG_FORMAT_JSON;
  अन्यथा
    वापस crun_make_error (err, 0, "unknown log format type `%s`", format);

  वापस 0;
}

पूर्णांक
json_gen_error_to_crun_error (पूर्णांक status, libcrun_error_t *err)
{
  चयन (status)
    {
    हाल json_gen_status_ok:
      वापस 0;

    शेष:
      वापस crun_make_error (err, 0, "generate JSON document: error %d", status);
    }
}
