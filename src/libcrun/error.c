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

#تعریف_کړه GNU_سرچینه
#ضمن <config.h>
#ضمن <stdarg.h>
#ضمن <unistd.h>
#ضمن <string.h>
#ضمن <time.h>
#ضمن <sys/time.h>
#ضمن <stdio.h>
#ضمن "utils.h"

#ضمن <json-c/json.h>
#ضمن <ocispec/json_common.h>

#که_تعریف_شوی HAVE_SYSTEMD
#  include <systemd/sd-journal.h>
#د_که_پای

تعداد
{
  LOG_FORMAT_متن = 0,
  LOG_FORMAT_JSON,
};

ساكن صحيح log_format;
ساكن صحيح output_verbosity = LIBCRUN_VERBOSITY_ERROR;

صحيح
libcrun_make_error (libcrun_error_t *err, صحيح حالت, ثابت حرف *پیغام, ...)
{
  va_list دلیلونه_list;
  libcrun_error_t ptr;

  va_پیل (دلیلونه_list, پیغام);
  *err = xmalloc (حجم (بنية libcrun_error_s));
  ptr = *err;
  ptr->حالت = حالت;
  إذا (vasprintf (&(ptr->پیغام), پیغام, دلیلونه_list) < 0)
    OOM ();
  va_پای_ټکی (دلیلونه_list);

  أرجع -حالت - 1;
}

صحيح
crun_error_wrap (libcrun_error_t *err, ثابت حرف *fmt, ...)
{
  cleanup_free حرف *پیغام = عدم;
  cleanup_free حرف *tmp = عدم;
  va_list دلیلونه_list;
  حرف *بدلون;
  صحيح ret;

  إذا (err == عدم || *err == عدم)
    {
      // Internal error
      أرجع 0;
    }

  ret = -(*err)->حالت - 1;

  va_پیل (دلیلونه_list, fmt);

  إذا (vasprintf (&پیغام, fmt, دلیلونه_list) < 0)
    {
      va_پای_ټکی (دلیلونه_list);
      پیغام = عدم;
      أرجع ret;
    }
  va_پای_ټکی (دلیلونه_list);

  xasprintf (&tmp, "%s: %s", پیغام, (*err)->پیغام);
  بدلون = tmp;
  tmp = (*err)->پیغام;
  (*err)->پیغام = بدلون;

  أرجع ret;
}

صحيح
crun_error_release (libcrun_error_t *err)
{
  ثابت صحيح saved_errno = errno;
  libcrun_error_t ptr;

  إذا (err == عدم)
    أرجع 0;

  ptr = *err;
  إذا (ptr == عدم)
    أرجع 0;

  حرر (ptr->پیغام);
  حرر (ptr);
  *err = عدم;

  errno = saved_errno;
  أرجع 0;
}

صحيح
libcrun_error_release (libcrun_error_t *err)
{
  أرجع crun_error_release (err);
}

صحيح
libcrun_error_get_حالت (libcrun_error_t err)
{
  إذا (err == عدم)
    أرجع 0;
  أرجع err->حالت;
}

ثابت حرف *
libcrun_error_get_پیغام (libcrun_error_t err)
{
  إذا (err == عدم)
    أرجع عدم;
  أرجع err->پیغام;
}

فراغ
crun_error_write_warning_and_release (FILE *بهر, libcrun_error_t **err)
{
  ثابت صحيح saved_errno = errno;
  libcrun_error_t ref;

  إذا (بهر == عدم)
    بهر = stderr;
  إذا (err == عدم || *err == عدم || **err == عدم)
    {
      // Internal error
      أرجع;
    }

  ref = **err;
  إذا (ref->حالت)
    fprintf (بهر, "%s: %s\n", ref->پیغام, strerror (ref->حالت));
  وإلا
    fprintf (بهر, "%s\n", ref->پیغام);

  حرر (ref->پیغام);
  حرر (ref);
  **err = عدم;

  errno = saved_errno;
}

فراغ
libcrun_error_write_warning_and_release (FILE *بهر, libcrun_error_t **err)
{
  أرجع crun_error_write_warning_and_release (بهر, err);
}

فراغ
libcrun_error_report_and_release (libcrun_error_t *err)
{
  libcrun_error_t ref;

  إذا (err == عدم || *err == عدم)
    أرجع;

  ref = *err;
  libcrun_error (ref->حالت, "%s", ref->پیغام);
  crun_error_release (err);
}

صحيح
crun_error_get_errno (libcrun_error_t *err)
{
  إذا (err == عدم || *err == عدم)
    أرجع 0;
  أرجع (*err)->حالت;
}

تعريفنوع حرف timestamp_t[64];

ساكن فراغ
get_timestamp (timestamp_t *timestamp, ثابت حرف *suffix)
{
  ثابت size_t بفر_اندازه = حجم (timestamp_t);
  حرف *بفر = (حرف *) timestamp;
  بنية timeval tv;
  size_t written;
  بنية tm now;

  gettimeofday (&tv, عدم);
  gmtime_r (&tv.tv_sec, &now);
  written = strftime (بفر, بفر_اندازه, "%Y-%m-%dT%H:%M:%S", &now);
  snprintf (بفر + written, بفر_اندازه - written, ".%06lldZ%.8s", (اوږد اوږد صحيح) tv.tv_usec, suffix);
}

/* Neither openlog(3) nor the journald handler copy the identifier they are
   given, and the global output handler outlives the caller's strings (e.g.
   the context is freed while errors are still reported through it), so keep
   an owned copy alive for as long as the handler can use it.  */
ساكن حرف *log_id;

ساكن ثابت حرف *
سېټ_log_id (ثابت حرف *id, حرف **old_id)
{
  *old_id = log_id;
  log_id = id ? xstrdup (id) : عدم;
  أرجع log_id;
}

ساكن فراغ *
init_syslog (ثابت حرف *id)
{
  openlog (id, 0, LOG_USER);
  أرجع عدم;
}

تعداد
{
  LOG_TYPE_FILE = 1,
  LOG_TYPE_SYSLOG = 2,
  LOG_TYPE_JOURNALD = 3
};

ساكن صحيح
get_log_type (ثابت حرف *لوګاریتم, ثابت حرف **معلومات)
{
  ثابت حرف *sep = strchr (لوګاریتم, ':');
  إذا (sep == عدم)
    {
      *معلومات = لوګاریتم;
      أرجع LOG_TYPE_FILE;
    }
  *معلومات = sep + 1;

  إذا (has_مختاړی (لوګاریتم, "syslog:"))
    أرجع LOG_TYPE_SYSLOG;
  إذا (has_مختاړی (لوګاریتم, "journald:"))
    أرجع LOG_TYPE_JOURNALD;
  إذا (has_مختاړی (لوګاریتم, "file:"))
    أرجع LOG_TYPE_FILE;

  أرجع -1;
}

صحيح
libcrun_init_logging (crun_output_handler *new_output_handler, فراغ **new_output_handler_arg, ثابت حرف *id,
                      ثابت حرف *لوګاریتم, libcrun_error_t *err)
{
  /* Released only after the new handler is installed.  */
  cleanup_free حرف *old_log_id = عدم;

  إذا (لوګاریتم == عدم)
    {
      *new_output_handler = log_write_منزل_stderr;
      *new_output_handler_arg = عدم;
    }
  وإلا
    {
      ثابت حرف *arg = عدم;
      صحيح log_type = get_log_type (لوګاریتم, &arg);

      إذا (log_type < 0)
        أرجع crun_make_error (err, errno, "unknown log type `%s`", لوګاریتم);

      بدل (log_type)
        {
        حالة LOG_TYPE_FILE:
          *new_output_handler = log_write_منزل_بهیر;
          *new_output_handler_arg = fopen (arg, "a+e");
          إذا (*new_output_handler_arg == عدم)
            أرجع crun_make_error (err, errno, "open log file `%s`", لوګاریتم);
          إذا (output_verbosity >= LIBCRUN_VERBOSITY_WARNING)
            setlinebuf (*new_output_handler_arg);
          اكسر;

        حالة LOG_TYPE_SYSLOG:
          *new_output_handler_arg = init_syslog (سېټ_log_id (arg[0] ? arg : id, &old_log_id));
          *new_output_handler = log_write_منزل_syslog;
          اكسر;

        حالة LOG_TYPE_JOURNALD:
          *new_output_handler = log_write_منزل_journald;
          *new_output_handler_arg = (فراغ *) سېټ_log_id (id, &old_log_id);
          اكسر;
        }
    }
  crun_سېټ_output_handler (*new_output_handler, *new_output_handler_arg);
  أرجع 0;
}

فراغ
log_write_منزل_بهیر (صحيح errno_, ثابت حرف *پیغام, صحيح verbosity, فراغ *arg)
{
  timestamp_t timestamp = {
    0,
  };
  FILE *بهیر = arg;
  صحيح tty = isatty (fileno (بهیر));
  ثابت حرف *color_begin = "";
  ثابت حرف *color_پای_ټکی = "";

  إذا (tty)
    {
      بدل (verbosity)
        {
        حالة LIBCRUN_VERBOSITY_DEBUG:
          color_begin = "\x1b[1;34m";
          color_پای_ټکی = "\x1b[0m";
          اكسر;
        حالة LIBCRUN_VERBOSITY_WARNING:
          color_begin = "\x1b[1;33m";
          color_پای_ټکی = "\x1b[0m";
          اكسر;
        حالة LIBCRUN_VERBOSITY_ERROR:
          color_begin = "\x1b[1;31m";
          color_پای_ټکی = "\x1b[0m";
          اكسر;
        }

      إذا (log_format == LOG_FORMAT_متن)
        get_timestamp (&timestamp, ": ");
    }

  إذا (errno_)
    fprintf (بهیر, "%s%s%s: %s%s\n", color_begin, timestamp, پیغام, strerror (errno_), color_پای_ټکی);
  وإلا
    fprintf (بهیر, "%s%s%s%s\n", color_begin, timestamp, پیغام, color_پای_ټکی);
}

فراغ
log_write_منزل_stderr (صحيح errno_, ثابت حرف *پیغام, صحيح verbosity, فراغ *arg arg_ناکارول_شوی)
{
  log_write_منزل_بهیر (errno_, پیغام, verbosity, stderr);
}

فراغ
log_write_منزل_syslog (صحيح errno_, ثابت حرف *پیغام, صحيح verbosity, فراغ *arg arg_ناکارول_شوی)
{
  صحيح لومړیتوب = LOG_ERR;
  بدل (verbosity)
    {
    حالة LIBCRUN_VERBOSITY_DEBUG:
      لومړیتوب = LOG_DEBUG;
      اكسر;
    حالة LIBCRUN_VERBOSITY_WARNING:
      لومړیتوب = LOG_WARNING;
      اكسر;
    حالة LIBCRUN_VERBOSITY_ERROR:
      لومړیتوب = LOG_ERR;
      اكسر;
    }
  إذا (errno_ == 0)
    syslog (لومړیتوب, "%s", پیغام);
  وإلا
    syslog (لومړیتوب, "%s: %s", پیغام, strerror (errno_));
}

فراغ
log_write_منزل_journald (صحيح errno_, ثابت حرف *پیغام, صحيح verbosity, فراغ *arg arg_ناکارول_شوی)
{
  (فراغ) errno_;
  (فراغ) پیغام;
  (فراغ) verbosity;
#که_تعریف_شوی HAVE_SYSTEMD
  صحيح لومړیتوب = LOG_ERR;
  بدل (verbosity)
    {
    حالة LIBCRUN_VERBOSITY_DEBUG:
      لومړیتوب = LOG_DEBUG;
      اكسر;
    حالة LIBCRUN_VERBOSITY_WARNING:
      لومړیتوب = LOG_WARNING;
      اكسر;
    حالة LIBCRUN_VERBOSITY_ERROR:
      لومړیتوب = LOG_ERR;
      اكسر;
    }
  إذا (errno_ == 0)
    sd_journal_send ("PRIORITY=%d", لومړیتوب, "MESSAGE=%s", پیغام, "ID=%s", arg, عدم);
  وإلا
    sd_journal_send ("PRIORITY=%d", لومړیتوب, "MESSAGE=%s: %s", پیغام, strerror (errno_), "ID=%s",
                     arg, عدم);
#د_که_پای
}

ساكن crun_output_handler output_handler = log_write_منزل_stderr;
ساكن فراغ *output_handler_arg = عدم;

فراغ
libcrun_سېټ_verbosity (صحيح verbosity)
{
  output_verbosity = verbosity;
}

صحيح
libcrun_get_verbosity ()
{
  أرجع output_verbosity;
}

فراغ
crun_سېټ_output_handler (crun_output_handler handler, فراغ *arg)
{
  output_handler = handler;
  output_handler_arg = arg;
}

ساكن حرف *
make_json_error (ثابت حرف *پیغام, صحيح errno_, صحيح verbosity)
{
  ثابت حرف *level;
  بدل (verbosity)
    {
    حالة LIBCRUN_VERBOSITY_DEBUG:
      level = "debug";
      اكسر;
    حالة LIBCRUN_VERBOSITY_WARNING:
      level = "warning";
      اكسر;
    حالة LIBCRUN_VERBOSITY_ERROR:
      level = "error";
      اكسر;
    }
  ثابت حرف *buf = عدم;
  json_gen_ctx *gen = عدم;
  حرف *ret = عدم;
  size_t buf_len;
  timestamp_t timestamp = {
    0,
  };

  إذا (! json_gen_init (&gen, عدم))
    أرجع عدم;

  get_timestamp (&timestamp, "");

  json_gen_map_open (gen);

  json_gen_string (gen, "msg", مزي_اوږدوالی ("msg"));
  إذا (errno_ == 0)
    json_gen_string (gen, پیغام, مزي_اوږدوالی (پیغام));
  وإلا
    {
      cleanup_free حرف *tmp = عدم;

      xasprintf (&tmp, "%s: %s", پیغام, strerror (errno_));
      json_gen_string (gen, tmp, مزي_اوږدوالی (tmp));
    }

  json_gen_string (gen, "level", مزي_اوږدوالی ("level"));
  json_gen_string (gen, level, مزي_اوږدوالی (level));

  json_gen_string (gen, "time", مزي_اوږدوالی ("time"));
  json_gen_string (gen, timestamp, مزي_اوږدوالی (timestamp));

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);
  إذا (buf)
    ret = strdup (buf);

  json_gen_free (gen);

  أرجع ret;
}

ساكن فراغ
write_log (صحيح errno_, صحيح verbosity, ثابت حرف *پیغام, va_list دلیلونه_list)
{
  صحيح ret;
  cleanup_free حرف *output = عدم;
  cleanup_free حرف *json = عدم;

  إذا (verbosity > output_verbosity)
    أرجع;

  ret = vasprintf (&output, پیغام, دلیلونه_list);
  إذا (UNLIKELY (ret < 0))
    OOM ();

  إذا (verbosity == LIBCRUN_VERBOSITY_ERROR && output_handler != log_write_منزل_stderr)
    log_write_منزل_stderr (errno_, output, LIBCRUN_VERBOSITY_ERROR, عدم);

  بدل (log_format)
    {
    حالة LOG_FORMAT_متن:
      output_handler (errno_, output, verbosity, output_handler_arg);
      اكسر;

    حالة LOG_FORMAT_JSON:
      json = make_json_error (output, errno_, verbosity);
      إذا (json)
        output_handler (0, json, verbosity, output_handler_arg);
      وإلا
        output_handler (errno_, output, verbosity, output_handler_arg);
      اكسر;
    }
}

فراغ
libcrun_debug (ثابت حرف *پیغام, ...)
{
  va_list دلیلونه_list;
  va_پیل (دلیلونه_list, پیغام);
  write_log (0, LIBCRUN_VERBOSITY_DEBUG, پیغام, دلیلونه_list);
  va_پای_ټکی (دلیلونه_list);
}

فراغ
libcrun_warning (ثابت حرف *پیغام, ...)
{
  va_list دلیلونه_list;
  va_پیل (دلیلونه_list, پیغام);
  write_log (0, LIBCRUN_VERBOSITY_WARNING, پیغام, دلیلونه_list);
  va_پای_ټکی (دلیلونه_list);
}

فراغ
libcrun_error (صحيح errno_, ثابت حرف *پیغام, ...)
{
  va_list دلیلونه_list;
  va_پیل (دلیلونه_list, پیغام);

  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, پیغام, دلیلونه_list);
  va_پای_ټکی (دلیلونه_list);
}

فراغ __attribute__ ((noreturn))
libcrun_fail_with_error (صحيح errno_, ثابت حرف *پیغام, ...)
{
  va_list دلیلونه_list;
  va_پیل (دلیلونه_list, پیغام);
  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, پیغام, دلیلونه_list);
  va_پای_ټکی (دلیلونه_list);
  وتل (EXIT_FAILURE);
}

صحيح
libcrun_سېټ_log_format (ثابت حرف *format, libcrun_error_t *err)
{
  إذا (strcmp (format, "text") == 0)
    log_format = LOG_FORMAT_متن;
  وإلا إذا (strcmp (format, "json") == 0)
    log_format = LOG_FORMAT_JSON;
  وإلا
    أرجع crun_make_error (err, 0, "unknown log format type `%s`", format);

  أرجع 0;
}

صحيح
json_gen_error_منزل_crun_error (صحيح حالت, libcrun_error_t *err)
{
  بدل (حالت)
    {
    حالة json_gen_حالت_ok:
      أرجع 0;

    افتراضي:
      أرجع crun_make_error (err, 0, "generate JSON document: error %d", حالت);
    }
}
