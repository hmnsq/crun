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

#تعریف_کرو GNU_ماخذ
#شامل <config.h>
#شامل <stdarg.h>
#شامل <unistd.h>
#شامل <string.h>
#شامل <time.h>
#شامل <sys/time.h>
#شامل <stdio.h>
#شامل "utils.h"

#شامل <json-c/json.h>
#شامل <ocispec/json_common.h>

#اگر_تعریف_شدہ HAVE_SYSTEMD
#  include <systemd/sd-journal.h>
#اگر_ختم

شمار
{
  LOG_FORMAT_متن = 0,
  LOG_FORMAT_JSON,
};

ساکن صحیح log_format;
ساکن صحیح output_verbosity = LIBCRUN_VERBOSITY_ERROR;

صحیح
libcrun_make_error (libcrun_error_t *err, صحیح حالت_2, ثابت حرف *پیغام, ...)
{
  va_list دلائل_list;
  libcrun_error_t ptr;

  va_آغاز (دلائل_list, پیغام);
  *err = xmalloc (حجم (ساخت libcrun_error_s));
  ptr = *err;
  ptr->حالت_2 = حالت_2;
  اگر (vasprintf (&(ptr->پیغام), پیغام, دلائل_list) < 0)
    OOM ();
  va_انتہا (دلائل_list);

  واپس -حالت_2 - 1;
}

صحیح
crun_error_wrap (libcrun_error_t *err, ثابت حرف *fmt, ...)
{
  cleanup_free حرف *پیغام = تہی;
  cleanup_free حرف *tmp = تہی;
  va_list دلائل_list;
  حرف *تبادلہ;
  صحیح ret;

  اگر (err == تہی || *err == تہی)
    {
      // Internal error
      واپس 0;
    }

  ret = -(*err)->حالت_2 - 1;

  va_آغاز (دلائل_list, fmt);

  اگر (vasprintf (&پیغام, fmt, دلائل_list) < 0)
    {
      va_انتہا (دلائل_list);
      پیغام = تہی;
      واپس ret;
    }
  va_انتہا (دلائل_list);

  xasprintf (&tmp, "%s: %s", پیغام, (*err)->پیغام);
  تبادلہ = tmp;
  tmp = (*err)->پیغام;
  (*err)->پیغام = تبادلہ;

  واپس ret;
}

صحیح
crun_error_release (libcrun_error_t *err)
{
  ثابت صحیح saved_errno = errno;
  libcrun_error_t ptr;

  اگر (err == تہی)
    واپس 0;

  ptr = *err;
  اگر (ptr == تہی)
    واپس 0;

  آزاد (ptr->پیغام);
  آزاد (ptr);
  *err = تہی;

  errno = saved_errno;
  واپس 0;
}

صحیح
libcrun_error_release (libcrun_error_t *err)
{
  واپس crun_error_release (err);
}

صحیح
libcrun_error_get_حالت (libcrun_error_t err)
{
  اگر (err == تہی)
    واپس 0;
  واپس err->حالت_2;
}

ثابت حرف *
libcrun_error_get_پیغام (libcrun_error_t err)
{
  اگر (err == تہی)
    واپس تہی;
  واپس err->پیغام;
}

خالی
crun_error_write_warning_and_release (FILE *برآمد, libcrun_error_t **err)
{
  ثابت صحیح saved_errno = errno;
  libcrun_error_t ref;

  اگر (برآمد == تہی)
    برآمد = stderr;
  اگر (err == تہی || *err == تہی || **err == تہی)
    {
      // Internal error
      واپس;
    }

  ref = **err;
  اگر (ref->حالت_2)
    fprintf (برآمد, "%s: %s\n", ref->پیغام, strerror (ref->حالت_2));
  ورنہ
    fprintf (برآمد, "%s\n", ref->پیغام);

  آزاد (ref->پیغام);
  آزاد (ref);
  **err = تہی;

  errno = saved_errno;
}

خالی
libcrun_error_write_warning_and_release (FILE *برآمد, libcrun_error_t **err)
{
  واپس crun_error_write_warning_and_release (برآمد, err);
}

خالی
libcrun_error_report_and_release (libcrun_error_t *err)
{
  libcrun_error_t ref;

  اگر (err == تہی || *err == تہی)
    واپس;

  ref = *err;
  libcrun_error (ref->حالت_2, "%s", ref->پیغام);
  crun_error_release (err);
}

صحیح
crun_error_get_errno (libcrun_error_t *err)
{
  اگر (err == تہی || *err == تہی)
    واپس 0;
  واپس (*err)->حالت_2;
}

قسمتعریف حرف timestamp_t[64];

ساکن خالی
get_timestamp (timestamp_t *timestamp, ثابت حرف *suffix)
{
  ثابت size_t بفر_سائز = حجم (timestamp_t);
  حرف *بفر = (حرف *) timestamp;
  ساخت timeval tv;
  size_t written;
  ساخت tm now;

  gettimeofday (&tv, تہی);
  gmtime_r (&tv.tv_sec, &now);
  written = strftime (بفر, بفر_سائز, "%Y-%m-%dT%H:%M:%S", &now);
  snprintf (بفر + written, بفر_سائز - written, ".%06lldZ%.8s", (طویل طویل صحیح) tv.tv_usec, suffix);
}

/* Neither openlog(3) nor the journald handler copy the identifier they are
   given, and the global output handler outlives the caller's strings (e.g.
   the context is freed while errors are still reported through it), so keep
   an owned copy alive for as long as the handler can use it.  */
ساکن حرف *log_id;

ساکن ثابت حرف *
سیٹ_log_id (ثابت حرف *id, حرف **old_id)
{
  *old_id = log_id;
  log_id = id ? xstrdup (id) : تہی;
  واپس log_id;
}

ساکن خالی *
init_syslog (ثابت حرف *id)
{
  openlog (id, 0, LOG_USER);
  واپس تہی;
}

شمار
{
  LOG_TYPE_FILE = 1,
  LOG_TYPE_SYSLOG = 2,
  LOG_TYPE_JOURNALD = 3
};

ساکن صحیح
get_log_type (ثابت حرف *لوگارتھم, ثابت حرف **ڈیٹا)
{
  ثابت حرف *sep = strchr (لوگارتھم, ':');
  اگر (sep == تہی)
    {
      *ڈیٹا = لوگارتھم;
      واپس LOG_TYPE_FILE;
    }
  *ڈیٹا = sep + 1;

  اگر (has_سابقہ (لوگارتھم, "syslog:"))
    واپس LOG_TYPE_SYSLOG;
  اگر (has_سابقہ (لوگارتھم, "journald:"))
    واپس LOG_TYPE_JOURNALD;
  اگر (has_سابقہ (لوگارتھم, "file:"))
    واپس LOG_TYPE_FILE;

  واپس -1;
}

صحیح
libcrun_init_logging (crun_output_handler *new_output_handler, خالی **new_output_handler_arg, ثابت حرف *id,
                      ثابت حرف *لوگارتھم, libcrun_error_t *err)
{
  /* Released only after the new handler is installed.  */
  cleanup_free حرف *old_log_id = تہی;

  اگر (لوگارتھم == تہی)
    {
      *new_output_handler = log_write_مقصد_stderr;
      *new_output_handler_arg = تہی;
    }
  ورنہ
    {
      ثابت حرف *arg = تہی;
      صحیح log_type = get_log_type (لوگارتھم, &arg);

      اگر (log_type < 0)
        واپس crun_make_error (err, errno, "unknown log type `%s`", لوگارتھم);

      انتخاب (log_type)
        {
        حالت LOG_TYPE_FILE:
          *new_output_handler = log_write_مقصد_دھارا;
          *new_output_handler_arg = fopen (arg, "a+e");
          اگر (*new_output_handler_arg == تہی)
            واپس crun_make_error (err, errno, "open log file `%s`", لوگارتھم);
          اگر (output_verbosity >= LIBCRUN_VERBOSITY_WARNING)
            setlinebuf (*new_output_handler_arg);
          توڑو;

        حالت LOG_TYPE_SYSLOG:
          *new_output_handler_arg = init_syslog (سیٹ_log_id (arg[0] ? arg : id, &old_log_id));
          *new_output_handler = log_write_مقصد_syslog;
          توڑو;

        حالت LOG_TYPE_JOURNALD:
          *new_output_handler = log_write_مقصد_journald;
          *new_output_handler_arg = (خالی *) سیٹ_log_id (id, &old_log_id);
          توڑو;
        }
    }
  crun_سیٹ_output_handler (*new_output_handler, *new_output_handler_arg);
  واپس 0;
}

خالی
log_write_مقصد_دھارا (صحیح errno_, ثابت حرف *پیغام, صحیح verbosity, خالی *arg)
{
  timestamp_t timestamp = {
    0,
  };
  FILE *دھارا = arg;
  صحیح tty = isatty (fileno (دھارا));
  ثابت حرف *color_begin = "";
  ثابت حرف *color_انتہا = "";

  اگر (tty)
    {
      انتخاب (verbosity)
        {
        حالت LIBCRUN_VERBOSITY_DEBUG:
          color_begin = "\x1b[1;34m";
          color_انتہا = "\x1b[0m";
          توڑو;
        حالت LIBCRUN_VERBOSITY_WARNING:
          color_begin = "\x1b[1;33m";
          color_انتہا = "\x1b[0m";
          توڑو;
        حالت LIBCRUN_VERBOSITY_ERROR:
          color_begin = "\x1b[1;31m";
          color_انتہا = "\x1b[0m";
          توڑو;
        }

      اگر (log_format == LOG_FORMAT_متن)
        get_timestamp (&timestamp, ": ");
    }

  اگر (errno_)
    fprintf (دھارا, "%s%s%s: %s%s\n", color_begin, timestamp, پیغام, strerror (errno_), color_انتہا);
  ورنہ
    fprintf (دھارا, "%s%s%s%s\n", color_begin, timestamp, پیغام, color_انتہا);
}

خالی
log_write_مقصد_stderr (صحیح errno_, ثابت حرف *پیغام, صحیح verbosity, خالی *arg arg_غیر_مستعمل)
{
  log_write_مقصد_دھارا (errno_, پیغام, verbosity, stderr);
}

خالی
log_write_مقصد_syslog (صحیح errno_, ثابت حرف *پیغام, صحیح verbosity, خالی *arg arg_غیر_مستعمل)
{
  صحیح ترجیح = LOG_ERR;
  انتخاب (verbosity)
    {
    حالت LIBCRUN_VERBOSITY_DEBUG:
      ترجیح = LOG_DEBUG;
      توڑو;
    حالت LIBCRUN_VERBOSITY_WARNING:
      ترجیح = LOG_WARNING;
      توڑو;
    حالت LIBCRUN_VERBOSITY_ERROR:
      ترجیح = LOG_ERR;
      توڑو;
    }
  اگر (errno_ == 0)
    syslog (ترجیح, "%s", پیغام);
  ورنہ
    syslog (ترجیح, "%s: %s", پیغام, strerror (errno_));
}

خالی
log_write_مقصد_journald (صحیح errno_, ثابت حرف *پیغام, صحیح verbosity, خالی *arg arg_غیر_مستعمل)
{
  (خالی) errno_;
  (خالی) پیغام;
  (خالی) verbosity;
#اگر_تعریف_شدہ HAVE_SYSTEMD
  صحیح ترجیح = LOG_ERR;
  انتخاب (verbosity)
    {
    حالت LIBCRUN_VERBOSITY_DEBUG:
      ترجیح = LOG_DEBUG;
      توڑو;
    حالت LIBCRUN_VERBOSITY_WARNING:
      ترجیح = LOG_WARNING;
      توڑو;
    حالت LIBCRUN_VERBOSITY_ERROR:
      ترجیح = LOG_ERR;
      توڑو;
    }
  اگر (errno_ == 0)
    sd_journal_send ("PRIORITY=%d", ترجیح, "MESSAGE=%s", پیغام, "ID=%s", arg, تہی);
  ورنہ
    sd_journal_send ("PRIORITY=%d", ترجیح, "MESSAGE=%s: %s", پیغام, strerror (errno_), "ID=%s",
                     arg, تہی);
#اگر_ختم
}

ساکن crun_output_handler output_handler = log_write_مقصد_stderr;
ساکن خالی *output_handler_arg = تہی;

خالی
libcrun_سیٹ_verbosity (صحیح verbosity)
{
  output_verbosity = verbosity;
}

صحیح
libcrun_get_verbosity ()
{
  واپس output_verbosity;
}

خالی
crun_سیٹ_output_handler (crun_output_handler handler, خالی *arg)
{
  output_handler = handler;
  output_handler_arg = arg;
}

ساکن حرف *
make_json_error (ثابت حرف *پیغام, صحیح errno_, صحیح verbosity)
{
  ثابت حرف *level;
  انتخاب (verbosity)
    {
    حالت LIBCRUN_VERBOSITY_DEBUG:
      level = "debug";
      توڑو;
    حالت LIBCRUN_VERBOSITY_WARNING:
      level = "warning";
      توڑو;
    حالت LIBCRUN_VERBOSITY_ERROR:
      level = "error";
      توڑو;
    }
  ثابت حرف *buf = تہی;
  json_gen_ctx *gen = تہی;
  حرف *ret = تہی;
  size_t buf_len;
  timestamp_t timestamp = {
    0,
  };

  اگر (! json_gen_init (&gen, تہی))
    واپس تہی;

  get_timestamp (&timestamp, "");

  json_gen_map_open (gen);

  json_gen_string (gen, "msg", لڑی_لمبائی ("msg"));
  اگر (errno_ == 0)
    json_gen_string (gen, پیغام, لڑی_لمبائی (پیغام));
  ورنہ
    {
      cleanup_free حرف *tmp = تہی;

      xasprintf (&tmp, "%s: %s", پیغام, strerror (errno_));
      json_gen_string (gen, tmp, لڑی_لمبائی (tmp));
    }

  json_gen_string (gen, "level", لڑی_لمبائی ("level"));
  json_gen_string (gen, level, لڑی_لمبائی (level));

  json_gen_string (gen, "time", لڑی_لمبائی ("time"));
  json_gen_string (gen, timestamp, لڑی_لمبائی (timestamp));

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);
  اگر (buf)
    ret = strdup (buf);

  json_gen_free (gen);

  واپس ret;
}

ساکن خالی
write_log (صحیح errno_, صحیح verbosity, ثابت حرف *پیغام, va_list دلائل_list)
{
  صحیح ret;
  cleanup_free حرف *output = تہی;
  cleanup_free حرف *json = تہی;

  اگر (verbosity > output_verbosity)
    واپس;

  ret = vasprintf (&output, پیغام, دلائل_list);
  اگر (UNLIKELY (ret < 0))
    OOM ();

  اگر (verbosity == LIBCRUN_VERBOSITY_ERROR && output_handler != log_write_مقصد_stderr)
    log_write_مقصد_stderr (errno_, output, LIBCRUN_VERBOSITY_ERROR, تہی);

  انتخاب (log_format)
    {
    حالت LOG_FORMAT_متن:
      output_handler (errno_, output, verbosity, output_handler_arg);
      توڑو;

    حالت LOG_FORMAT_JSON:
      json = make_json_error (output, errno_, verbosity);
      اگر (json)
        output_handler (0, json, verbosity, output_handler_arg);
      ورنہ
        output_handler (errno_, output, verbosity, output_handler_arg);
      توڑو;
    }
}

خالی
libcrun_debug (ثابت حرف *پیغام, ...)
{
  va_list دلائل_list;
  va_آغاز (دلائل_list, پیغام);
  write_log (0, LIBCRUN_VERBOSITY_DEBUG, پیغام, دلائل_list);
  va_انتہا (دلائل_list);
}

خالی
libcrun_warning (ثابت حرف *پیغام, ...)
{
  va_list دلائل_list;
  va_آغاز (دلائل_list, پیغام);
  write_log (0, LIBCRUN_VERBOSITY_WARNING, پیغام, دلائل_list);
  va_انتہا (دلائل_list);
}

خالی
libcrun_error (صحیح errno_, ثابت حرف *پیغام, ...)
{
  va_list دلائل_list;
  va_آغاز (دلائل_list, پیغام);

  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, پیغام, دلائل_list);
  va_انتہا (دلائل_list);
}

خالی __attribute__ ((noreturn))
libcrun_fail_with_error (صحیح errno_, ثابت حرف *پیغام, ...)
{
  va_list دلائل_list;
  va_آغاز (دلائل_list, پیغام);
  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, پیغام, دلائل_list);
  va_انتہا (دلائل_list);
  باہر_نکلو (EXIT_FAILURE);
}

صحیح
libcrun_سیٹ_log_format (ثابت حرف *format, libcrun_error_t *err)
{
  اگر (strcmp (format, "text") == 0)
    log_format = LOG_FORMAT_متن;
  ورنہ اگر (strcmp (format, "json") == 0)
    log_format = LOG_FORMAT_JSON;
  ورنہ
    واپس crun_make_error (err, 0, "unknown log format type `%s`", format);

  واپس 0;
}

صحیح
json_gen_error_مقصد_crun_error (صحیح حالت_2, libcrun_error_t *err)
{
  انتخاب (حالت_2)
    {
    حالت json_gen_حالت_ok:
      واپس 0;

    طےشدہ:
      واپس crun_make_error (err, 0, "generate JSON document: error %d", حالت_2);
    }
}
