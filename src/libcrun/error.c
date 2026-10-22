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

#تعریف_کن GNU_منبع
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

#اگر_تعریف_شده HAVE_SYSTEMD
#  include <systemd/sd-journal.h>
#پایان_اگر

تعداد
{
  LOG_FORMAT_متن = 0,
  LOG_FORMAT_JSON,
};

ساكن صحيح log_format;
ساكن صحيح output_verbosity = LIBCRUN_VERBOSITY_ERROR;

صحيح
libcrun_make_error (libcrun_error_t *err, صحيح وضعیت, ثابت حرف *پیام, ...)
{
  va_list آرگومانها_list;
  libcrun_error_t ptr;

  va_شروع (آرگومانها_list, پیام);
  *err = xmalloc (حجم (بنية libcrun_error_s));
  ptr = *err;
  ptr->وضعیت = وضعیت;
  إذا (vasprintf (&(ptr->پیام), پیام, آرگومانها_list) < 0)
    OOM ();
  va_انتها (آرگومانها_list);

  أرجع -وضعیت - 1;
}

صحيح
crun_error_wrap (libcrun_error_t *err, ثابت حرف *fmt, ...)
{
  cleanup_free حرف *پیام = عدم;
  cleanup_free حرف *tmp = عدم;
  va_list آرگومانها_list;
  حرف *جابجا;
  صحيح ret;

  إذا (err == عدم || *err == عدم)
    {
      // Internal error
      أرجع 0;
    }

  ret = -(*err)->وضعیت - 1;

  va_شروع (آرگومانها_list, fmt);

  إذا (vasprintf (&پیام, fmt, آرگومانها_list) < 0)
    {
      va_انتها (آرگومانها_list);
      پیام = عدم;
      أرجع ret;
    }
  va_انتها (آرگومانها_list);

  xasprintf (&tmp, "%s: %s", پیام, (*err)->پیام);
  جابجا = tmp;
  tmp = (*err)->پیام;
  (*err)->پیام = جابجا;

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

  حرر (ptr->پیام);
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
libcrun_error_get_وضعیت (libcrun_error_t err)
{
  إذا (err == عدم)
    أرجع 0;
  أرجع err->وضعیت;
}

ثابت حرف *
libcrun_error_get_پیام (libcrun_error_t err)
{
  إذا (err == عدم)
    أرجع عدم;
  أرجع err->پیام;
}

فراغ
crun_error_write_warning_and_release (FILE *بیرون, libcrun_error_t **err)
{
  ثابت صحيح saved_errno = errno;
  libcrun_error_t ref;

  إذا (بیرون == عدم)
    بیرون = stderr;
  إذا (err == عدم || *err == عدم || **err == عدم)
    {
      // Internal error
      أرجع;
    }

  ref = **err;
  إذا (ref->وضعیت)
    fprintf (بیرون, "%s: %s\n", ref->پیام, strerror (ref->وضعیت));
  وإلا
    fprintf (بیرون, "%s\n", ref->پیام);

  حرر (ref->پیام);
  حرر (ref);
  **err = عدم;

  errno = saved_errno;
}

فراغ
libcrun_error_write_warning_and_release (FILE *بیرون, libcrun_error_t **err)
{
  أرجع crun_error_write_warning_and_release (بیرون, err);
}

فراغ
libcrun_error_report_and_release (libcrun_error_t *err)
{
  libcrun_error_t ref;

  إذا (err == عدم || *err == عدم)
    أرجع;

  ref = *err;
  libcrun_error (ref->وضعیت, "%s", ref->پیام);
  crun_error_release (err);
}

صحيح
crun_error_get_errno (libcrun_error_t *err)
{
  إذا (err == عدم || *err == عدم)
    أرجع 0;
  أرجع (*err)->وضعیت;
}

تعريفنوع حرف timestamp_t[64];

ساكن فراغ
get_timestamp (timestamp_t *timestamp, ثابت حرف *suffix)
{
  ثابت size_t میانگیر_اندازه = حجم (timestamp_t);
  حرف *میانگیر = (حرف *) timestamp;
  بنية timeval tv;
  size_t written;
  بنية tm now;

  gettimeofday (&tv, عدم);
  gmtime_r (&tv.tv_sec, &now);
  written = strftime (میانگیر, میانگیر_اندازه, "%Y-%m-%dT%H:%M:%S", &now);
  snprintf (میانگیر + written, میانگیر_اندازه - written, ".%06lldZ%.8s", (دراز دراز صحيح) tv.tv_usec, suffix);
}

/* Neither openlog(3) nor the journald handler copy the identifier they are
   given, and the global output handler outlives the caller's strings (e.g.
   the context is freed while errors are still reported through it), so keep
   an owned copy alive for as long as the handler can use it.  */
ساكن حرف *log_id;

ساكن ثابت حرف *
مجموعه_log_id (ثابت حرف *id, حرف **old_id)
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
get_log_type (ثابت حرف *لگاریتم, ثابت حرف **داده)
{
  ثابت حرف *sep = strchr (لگاریتم, ':');
  إذا (sep == عدم)
    {
      *داده = لگاریتم;
      أرجع LOG_TYPE_FILE;
    }
  *داده = sep + 1;

  إذا (has_پیشوند (لگاریتم, "syslog:"))
    أرجع LOG_TYPE_SYSLOG;
  إذا (has_پیشوند (لگاریتم, "journald:"))
    أرجع LOG_TYPE_JOURNALD;
  إذا (has_پیشوند (لگاریتم, "file:"))
    أرجع LOG_TYPE_FILE;

  أرجع -1;
}

صحيح
libcrun_init_logging (crun_output_handler *new_output_handler, فراغ **new_output_handler_arg, ثابت حرف *id,
                      ثابت حرف *لگاریتم, libcrun_error_t *err)
{
  /* Released only after the new handler is installed.  */
  cleanup_free حرف *old_log_id = عدم;

  إذا (لگاریتم == عدم)
    {
      *new_output_handler = log_write_مقصد_stderr;
      *new_output_handler_arg = عدم;
    }
  وإلا
    {
      ثابت حرف *arg = عدم;
      صحيح log_type = get_log_type (لگاریتم, &arg);

      إذا (log_type < 0)
        أرجع crun_make_error (err, errno, "unknown log type `%s`", لگاریتم);

      بدل (log_type)
        {
        حالة LOG_TYPE_FILE:
          *new_output_handler = log_write_مقصد_جریان;
          *new_output_handler_arg = fopen (arg, "a+e");
          إذا (*new_output_handler_arg == عدم)
            أرجع crun_make_error (err, errno, "open log file `%s`", لگاریتم);
          إذا (output_verbosity >= LIBCRUN_VERBOSITY_WARNING)
            setlinebuf (*new_output_handler_arg);
          اكسر;

        حالة LOG_TYPE_SYSLOG:
          *new_output_handler_arg = init_syslog (مجموعه_log_id (arg[0] ? arg : id, &old_log_id));
          *new_output_handler = log_write_مقصد_syslog;
          اكسر;

        حالة LOG_TYPE_JOURNALD:
          *new_output_handler = log_write_مقصد_journald;
          *new_output_handler_arg = (فراغ *) مجموعه_log_id (id, &old_log_id);
          اكسر;
        }
    }
  crun_مجموعه_output_handler (*new_output_handler, *new_output_handler_arg);
  أرجع 0;
}

فراغ
log_write_مقصد_جریان (صحيح errno_, ثابت حرف *پیام, صحيح verbosity, فراغ *arg)
{
  timestamp_t timestamp = {
    0,
  };
  FILE *جریان = arg;
  صحيح tty = isatty (fileno (جریان));
  ثابت حرف *color_begin = "";
  ثابت حرف *color_انتها = "";

  إذا (tty)
    {
      بدل (verbosity)
        {
        حالة LIBCRUN_VERBOSITY_DEBUG:
          color_begin = "\x1b[1;34m";
          color_انتها = "\x1b[0m";
          اكسر;
        حالة LIBCRUN_VERBOSITY_WARNING:
          color_begin = "\x1b[1;33m";
          color_انتها = "\x1b[0m";
          اكسر;
        حالة LIBCRUN_VERBOSITY_ERROR:
          color_begin = "\x1b[1;31m";
          color_انتها = "\x1b[0m";
          اكسر;
        }

      إذا (log_format == LOG_FORMAT_متن)
        get_timestamp (&timestamp, ": ");
    }

  إذا (errno_)
    fprintf (جریان, "%s%s%s: %s%s\n", color_begin, timestamp, پیام, strerror (errno_), color_انتها);
  وإلا
    fprintf (جریان, "%s%s%s%s\n", color_begin, timestamp, پیام, color_انتها);
}

فراغ
log_write_مقصد_stderr (صحيح errno_, ثابت حرف *پیام, صحيح verbosity, فراغ *arg arg_استفادهنشده)
{
  log_write_مقصد_جریان (errno_, پیام, verbosity, stderr);
}

فراغ
log_write_مقصد_syslog (صحيح errno_, ثابت حرف *پیام, صحيح verbosity, فراغ *arg arg_استفادهنشده)
{
  صحيح اولویت = LOG_ERR;
  بدل (verbosity)
    {
    حالة LIBCRUN_VERBOSITY_DEBUG:
      اولویت = LOG_DEBUG;
      اكسر;
    حالة LIBCRUN_VERBOSITY_WARNING:
      اولویت = LOG_WARNING;
      اكسر;
    حالة LIBCRUN_VERBOSITY_ERROR:
      اولویت = LOG_ERR;
      اكسر;
    }
  إذا (errno_ == 0)
    syslog (اولویت, "%s", پیام);
  وإلا
    syslog (اولویت, "%s: %s", پیام, strerror (errno_));
}

فراغ
log_write_مقصد_journald (صحيح errno_, ثابت حرف *پیام, صحيح verbosity, فراغ *arg arg_استفادهنشده)
{
  (فراغ) errno_;
  (فراغ) پیام;
  (فراغ) verbosity;
#اگر_تعریف_شده HAVE_SYSTEMD
  صحيح اولویت = LOG_ERR;
  بدل (verbosity)
    {
    حالة LIBCRUN_VERBOSITY_DEBUG:
      اولویت = LOG_DEBUG;
      اكسر;
    حالة LIBCRUN_VERBOSITY_WARNING:
      اولویت = LOG_WARNING;
      اكسر;
    حالة LIBCRUN_VERBOSITY_ERROR:
      اولویت = LOG_ERR;
      اكسر;
    }
  إذا (errno_ == 0)
    sd_journal_send ("PRIORITY=%d", اولویت, "MESSAGE=%s", پیام, "ID=%s", arg, عدم);
  وإلا
    sd_journal_send ("PRIORITY=%d", اولویت, "MESSAGE=%s: %s", پیام, strerror (errno_), "ID=%s",
                     arg, عدم);
#پایان_اگر
}

ساكن crun_output_handler output_handler = log_write_مقصد_stderr;
ساكن فراغ *output_handler_arg = عدم;

فراغ
libcrun_مجموعه_verbosity (صحيح verbosity)
{
  output_verbosity = verbosity;
}

صحيح
libcrun_get_verbosity ()
{
  أرجع output_verbosity;
}

فراغ
crun_مجموعه_output_handler (crun_output_handler handler, فراغ *arg)
{
  output_handler = handler;
  output_handler_arg = arg;
}

ساكن حرف *
make_json_error (ثابت حرف *پیام, صحيح errno_, صحيح verbosity)
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

  json_gen_string (gen, "msg", طول_رشته ("msg"));
  إذا (errno_ == 0)
    json_gen_string (gen, پیام, طول_رشته (پیام));
  وإلا
    {
      cleanup_free حرف *tmp = عدم;

      xasprintf (&tmp, "%s: %s", پیام, strerror (errno_));
      json_gen_string (gen, tmp, طول_رشته (tmp));
    }

  json_gen_string (gen, "level", طول_رشته ("level"));
  json_gen_string (gen, level, طول_رشته (level));

  json_gen_string (gen, "time", طول_رشته ("time"));
  json_gen_string (gen, timestamp, طول_رشته (timestamp));

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);
  إذا (buf)
    ret = strdup (buf);

  json_gen_free (gen);

  أرجع ret;
}

ساكن فراغ
write_log (صحيح errno_, صحيح verbosity, ثابت حرف *پیام, va_list آرگومانها_list)
{
  صحيح ret;
  cleanup_free حرف *output = عدم;
  cleanup_free حرف *json = عدم;

  إذا (verbosity > output_verbosity)
    أرجع;

  ret = vasprintf (&output, پیام, آرگومانها_list);
  إذا (UNLIKELY (ret < 0))
    OOM ();

  إذا (verbosity == LIBCRUN_VERBOSITY_ERROR && output_handler != log_write_مقصد_stderr)
    log_write_مقصد_stderr (errno_, output, LIBCRUN_VERBOSITY_ERROR, عدم);

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
libcrun_debug (ثابت حرف *پیام, ...)
{
  va_list آرگومانها_list;
  va_شروع (آرگومانها_list, پیام);
  write_log (0, LIBCRUN_VERBOSITY_DEBUG, پیام, آرگومانها_list);
  va_انتها (آرگومانها_list);
}

فراغ
libcrun_warning (ثابت حرف *پیام, ...)
{
  va_list آرگومانها_list;
  va_شروع (آرگومانها_list, پیام);
  write_log (0, LIBCRUN_VERBOSITY_WARNING, پیام, آرگومانها_list);
  va_انتها (آرگومانها_list);
}

فراغ
libcrun_error (صحيح errno_, ثابت حرف *پیام, ...)
{
  va_list آرگومانها_list;
  va_شروع (آرگومانها_list, پیام);

  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, پیام, آرگومانها_list);
  va_انتها (آرگومانها_list);
}

فراغ __attribute__ ((noreturn))
libcrun_fail_with_error (صحيح errno_, ثابت حرف *پیام, ...)
{
  va_list آرگومانها_list;
  va_شروع (آرگومانها_list, پیام);
  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, پیام, آرگومانها_list);
  va_انتها (آرگومانها_list);
  خروج (EXIT_FAILURE);
}

صحيح
libcrun_مجموعه_log_format (ثابت حرف *format, libcrun_error_t *err)
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
json_gen_error_مقصد_crun_error (صحيح وضعیت, libcrun_error_t *err)
{
  بدل (وضعیت)
    {
    حالة json_gen_وضعیت_ok:
      أرجع 0;

    افتراضي:
      أرجع crun_make_error (err, 0, "generate JSON document: error %d", وضعیت);
    }
}
