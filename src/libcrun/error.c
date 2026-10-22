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

#నిర్వచించు GNU_ఆధారం
#అన్తర్భావ <config.h>
#అన్తర్భావ <stdarg.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ <sys/time.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ "utils.h"

#అన్తర్భావ <json-c/json.h>
#అన్తర్భావ <ocispec/json_common.h>

#నిర్వచితమైతే HAVE_SYSTEMD
#  include <systemd/sd-journal.h>
#అయితే_ముగింపు

గణన
{
  LOG_FORMAT_పాఠ్యం = 0,
  LOG_FORMAT_JSON,
};

స్థిర పూర్ణాంకం log_format;
స్థిర పూర్ణాంకం output_verbosity = LIBCRUN_VERBOSITY_ERROR;

పూర్ణాంకం
libcrun_make_error (libcrun_error_t *err, పూర్ణాంకం స్థితి_2, స్థిరరాశి అక్షరం *సందేశం, ...)
{
  va_list వాదనలు_list;
  libcrun_error_t ptr;

  va_ప్రారంభం (వాదనలు_list, సందేశం);
  *err = xmalloc (పరిమాణం (నిర్మాణం libcrun_error_s));
  ptr = *err;
  ptr->స్థితి_2 = స్థితి_2;
  ఒకవేళ (vasprintf (&(ptr->సందేశం), సందేశం, వాదనలు_list) < 0)
    OOM ();
  va_చివర (వాదనలు_list);

  తిరిగి -స్థితి_2 - 1;
}

పూర్ణాంకం
crun_error_wrap (libcrun_error_t *err, స్థిరరాశి అక్షరం *fmt, ...)
{
  cleanup_free అక్షరం *సందేశం = ఖాళీ;
  cleanup_free అక్షరం *tmp = ఖాళీ;
  va_list వాదనలు_list;
  అక్షరం *మార్పిడి;
  పూర్ణాంకం ret;

  ఒకవేళ (err == ఖాళీ || *err == ఖాళీ)
    {
      // Internal error
      తిరిగి 0;
    }

  ret = -(*err)->స్థితి_2 - 1;

  va_ప్రారంభం (వాదనలు_list, fmt);

  ఒకవేళ (vasprintf (&సందేశం, fmt, వాదనలు_list) < 0)
    {
      va_చివర (వాదనలు_list);
      సందేశం = ఖాళీ;
      తిరిగి ret;
    }
  va_చివర (వాదనలు_list);

  xasprintf (&tmp, "%s: %s", సందేశం, (*err)->సందేశం);
  మార్పిడి = tmp;
  tmp = (*err)->సందేశం;
  (*err)->సందేశం = మార్పిడి;

  తిరిగి ret;
}

పూర్ణాంకం
crun_error_release (libcrun_error_t *err)
{
  స్థిరరాశి పూర్ణాంకం saved_errno = errno;
  libcrun_error_t ptr;

  ఒకవేళ (err == ఖాళీ)
    తిరిగి 0;

  ptr = *err;
  ఒకవేళ (ptr == ఖాళీ)
    తిరిగి 0;

  విడుదల (ptr->సందేశం);
  విడుదల (ptr);
  *err = ఖాళీ;

  errno = saved_errno;
  తిరిగి 0;
}

పూర్ణాంకం
libcrun_error_release (libcrun_error_t *err)
{
  తిరిగి crun_error_release (err);
}

పూర్ణాంకం
libcrun_error_get_స్థితి (libcrun_error_t err)
{
  ఒకవేళ (err == ఖాళీ)
    తిరిగి 0;
  తిరిగి err->స్థితి_2;
}

స్థిరరాశి అక్షరం *
libcrun_error_get_సందేశం (libcrun_error_t err)
{
  ఒకవేళ (err == ఖాళీ)
    తిరిగి ఖాళీ;
  తిరిగి err->సందేశం;
}

శూన్య
crun_error_write_warning_and_release (FILE *బహిర్గతం, libcrun_error_t **err)
{
  స్థిరరాశి పూర్ణాంకం saved_errno = errno;
  libcrun_error_t ref;

  ఒకవేళ (బహిర్గతం == ఖాళీ)
    బహిర్గతం = stderr;
  ఒకవేళ (err == ఖాళీ || *err == ఖాళీ || **err == ఖాళీ)
    {
      // Internal error
      తిరిగి;
    }

  ref = **err;
  ఒకవేళ (ref->స్థితి_2)
    fprintf (బహిర్గతం, "%s: %s\n", ref->సందేశం, strerror (ref->స్థితి_2));
  లేకపోతే
    fprintf (బహిర్గతం, "%s\n", ref->సందేశం);

  విడుదల (ref->సందేశం);
  విడుదల (ref);
  **err = ఖాళీ;

  errno = saved_errno;
}

శూన్య
libcrun_error_write_warning_and_release (FILE *బహిర్గతం, libcrun_error_t **err)
{
  తిరిగి crun_error_write_warning_and_release (బహిర్గతం, err);
}

శూన్య
libcrun_error_report_and_release (libcrun_error_t *err)
{
  libcrun_error_t ref;

  ఒకవేళ (err == ఖాళీ || *err == ఖాళీ)
    తిరిగి;

  ref = *err;
  libcrun_error (ref->స్థితి_2, "%s", ref->సందేశం);
  crun_error_release (err);
}

పూర్ణాంకం
crun_error_get_errno (libcrun_error_t *err)
{
  ఒకవేళ (err == ఖాళీ || *err == ఖాళీ)
    తిరిగి 0;
  తిరిగి (*err)->స్థితి_2;
}

రకనిర్వచనం అక్షరం timestamp_t[64];

స్థిర శూన్య
get_timestamp (timestamp_t *timestamp, స్థిరరాశి అక్షరం *suffix)
{
  స్థిరరాశి size_t బఫరు_కొలత = పరిమాణం (timestamp_t);
  అక్షరం *బఫరు = (అక్షరం *) timestamp;
  నిర్మాణం timeval tv;
  size_t written;
  నిర్మాణం tm now;

  gettimeofday (&tv, ఖాళీ);
  gmtime_r (&tv.tv_sec, &now);
  written = strftime (బఫరు, బఫరు_కొలత, "%Y-%m-%dT%H:%M:%S", &now);
  snprintf (బఫరు + written, బఫరు_కొలత - written, ".%06lldZ%.8s", (దీర్ఘ దీర్ఘ పూర్ణాంకం) tv.tv_usec, suffix);
}

/* Neither openlog(3) nor the journald handler copy the identifier they are
   given, and the global output handler outlives the caller's strings (e.g.
   the context is freed while errors are still reported through it), so keep
   an owned copy alive for as long as the handler can use it.  */
స్థిర అక్షరం *log_id;

స్థిర స్థిరరాశి అక్షరం *
సమితి_log_id (స్థిరరాశి అక్షరం *id, అక్షరం **old_id)
{
  *old_id = log_id;
  log_id = id ? xstrdup (id) : ఖాళీ;
  తిరిగి log_id;
}

స్థిర శూన్య *
init_syslog (స్థిరరాశి అక్షరం *id)
{
  openlog (id, 0, LOG_USER);
  తిరిగి ఖాళీ;
}

గణన
{
  LOG_TYPE_FILE = 1,
  LOG_TYPE_SYSLOG = 2,
  LOG_TYPE_JOURNALD = 3
};

స్థిర పూర్ణాంకం
get_log_type (స్థిరరాశి అక్షరం *సంవర్గమానం, స్థిరరాశి అక్షరం **దత్తాంశం)
{
  స్థిరరాశి అక్షరం *sep = strchr (సంవర్గమానం, ':');
  ఒకవేళ (sep == ఖాళీ)
    {
      *దత్తాంశం = సంవర్గమానం;
      తిరిగి LOG_TYPE_FILE;
    }
  *దత్తాంశం = sep + 1;

  ఒకవేళ (has_ఉపసర్గ (సంవర్గమానం, "syslog:"))
    తిరిగి LOG_TYPE_SYSLOG;
  ఒకవేళ (has_ఉపసర్గ (సంవర్గమానం, "journald:"))
    తిరిగి LOG_TYPE_JOURNALD;
  ఒకవేళ (has_ఉపసర్గ (సంవర్గమానం, "file:"))
    తిరిగి LOG_TYPE_FILE;

  తిరిగి -1;
}

పూర్ణాంకం
libcrun_init_logging (crun_output_handler *new_output_handler, శూన్య **new_output_handler_arg, స్థిరరాశి అక్షరం *id,
                      స్థిరరాశి అక్షరం *సంవర్గమానం, libcrun_error_t *err)
{
  /* Released only after the new handler is installed.  */
  cleanup_free అక్షరం *old_log_id = ఖాళీ;

  ఒకవేళ (సంవర్గమానం == ఖాళీ)
    {
      *new_output_handler = log_write_గమ్యం_stderr;
      *new_output_handler_arg = ఖాళీ;
    }
  లేకపోతే
    {
      స్థిరరాశి అక్షరం *arg = ఖాళీ;
      పూర్ణాంకం log_type = get_log_type (సంవర్గమానం, &arg);

      ఒకవేళ (log_type < 0)
        తిరిగి crun_make_error (err, errno, "unknown log type `%s`", సంవర్గమానం);

      ఎంపిక (log_type)
        {
        స్థితి LOG_TYPE_FILE:
          *new_output_handler = log_write_గమ్యం_ప్రవాహం;
          *new_output_handler_arg = fopen (arg, "a+e");
          ఒకవేళ (*new_output_handler_arg == ఖాళీ)
            తిరిగి crun_make_error (err, errno, "open log file `%s`", సంవర్గమానం);
          ఒకవేళ (output_verbosity >= LIBCRUN_VERBOSITY_WARNING)
            setlinebuf (*new_output_handler_arg);
          ఆపు;

        స్థితి LOG_TYPE_SYSLOG:
          *new_output_handler_arg = init_syslog (సమితి_log_id (arg[0] ? arg : id, &old_log_id));
          *new_output_handler = log_write_గమ్యం_syslog;
          ఆపు;

        స్థితి LOG_TYPE_JOURNALD:
          *new_output_handler = log_write_గమ్యం_journald;
          *new_output_handler_arg = (శూన్య *) సమితి_log_id (id, &old_log_id);
          ఆపు;
        }
    }
  crun_సమితి_output_handler (*new_output_handler, *new_output_handler_arg);
  తిరిగి 0;
}

శూన్య
log_write_గమ్యం_ప్రవాహం (పూర్ణాంకం errno_, స్థిరరాశి అక్షరం *సందేశం, పూర్ణాంకం verbosity, శూన్య *arg)
{
  timestamp_t timestamp = {
    0,
  };
  FILE *ప్రవాహం = arg;
  పూర్ణాంకం tty = isatty (fileno (ప్రవాహం));
  స్థిరరాశి అక్షరం *color_begin = "";
  స్థిరరాశి అక్షరం *color_చివర = "";

  ఒకవేళ (tty)
    {
      ఎంపిక (verbosity)
        {
        స్థితి LIBCRUN_VERBOSITY_DEBUG:
          color_begin = "\x1b[1;34m";
          color_చివర = "\x1b[0m";
          ఆపు;
        స్థితి LIBCRUN_VERBOSITY_WARNING:
          color_begin = "\x1b[1;33m";
          color_చివర = "\x1b[0m";
          ఆపు;
        స్థితి LIBCRUN_VERBOSITY_ERROR:
          color_begin = "\x1b[1;31m";
          color_చివర = "\x1b[0m";
          ఆపు;
        }

      ఒకవేళ (log_format == LOG_FORMAT_పాఠ్యం)
        get_timestamp (&timestamp, ": ");
    }

  ఒకవేళ (errno_)
    fprintf (ప్రవాహం, "%s%s%s: %s%s\n", color_begin, timestamp, సందేశం, strerror (errno_), color_చివర);
  లేకపోతే
    fprintf (ప్రవాహం, "%s%s%s%s\n", color_begin, timestamp, సందేశం, color_చివర);
}

శూన్య
log_write_గమ్యం_stderr (పూర్ణాంకం errno_, స్థిరరాశి అక్షరం *సందేశం, పూర్ణాంకం verbosity, శూన్య *arg arg_వాడని)
{
  log_write_గమ్యం_ప్రవాహం (errno_, సందేశం, verbosity, stderr);
}

శూన్య
log_write_గమ్యం_syslog (పూర్ణాంకం errno_, స్థిరరాశి అక్షరం *సందేశం, పూర్ణాంకం verbosity, శూన్య *arg arg_వాడని)
{
  పూర్ణాంకం ప్రాధాన్యత = LOG_ERR;
  ఎంపిక (verbosity)
    {
    స్థితి LIBCRUN_VERBOSITY_DEBUG:
      ప్రాధాన్యత = LOG_DEBUG;
      ఆపు;
    స్థితి LIBCRUN_VERBOSITY_WARNING:
      ప్రాధాన్యత = LOG_WARNING;
      ఆపు;
    స్థితి LIBCRUN_VERBOSITY_ERROR:
      ప్రాధాన్యత = LOG_ERR;
      ఆపు;
    }
  ఒకవేళ (errno_ == 0)
    syslog (ప్రాధాన్యత, "%s", సందేశం);
  లేకపోతే
    syslog (ప్రాధాన్యత, "%s: %s", సందేశం, strerror (errno_));
}

శూన్య
log_write_గమ్యం_journald (పూర్ణాంకం errno_, స్థిరరాశి అక్షరం *సందేశం, పూర్ణాంకం verbosity, శూన్య *arg arg_వాడని)
{
  (శూన్య) errno_;
  (శూన్య) సందేశం;
  (శూన్య) verbosity;
#నిర్వచితమైతే HAVE_SYSTEMD
  పూర్ణాంకం ప్రాధాన్యత = LOG_ERR;
  ఎంపిక (verbosity)
    {
    స్థితి LIBCRUN_VERBOSITY_DEBUG:
      ప్రాధాన్యత = LOG_DEBUG;
      ఆపు;
    స్థితి LIBCRUN_VERBOSITY_WARNING:
      ప్రాధాన్యత = LOG_WARNING;
      ఆపు;
    స్థితి LIBCRUN_VERBOSITY_ERROR:
      ప్రాధాన్యత = LOG_ERR;
      ఆపు;
    }
  ఒకవేళ (errno_ == 0)
    sd_journal_send ("PRIORITY=%d", ప్రాధాన్యత, "MESSAGE=%s", సందేశం, "ID=%s", arg, ఖాళీ);
  లేకపోతే
    sd_journal_send ("PRIORITY=%d", ప్రాధాన్యత, "MESSAGE=%s: %s", సందేశం, strerror (errno_), "ID=%s",
                     arg, ఖాళీ);
#అయితే_ముగింపు
}

స్థిర crun_output_handler output_handler = log_write_గమ్యం_stderr;
స్థిర శూన్య *output_handler_arg = ఖాళీ;

శూన్య
libcrun_సమితి_verbosity (పూర్ణాంకం verbosity)
{
  output_verbosity = verbosity;
}

పూర్ణాంకం
libcrun_get_verbosity ()
{
  తిరిగి output_verbosity;
}

శూన్య
crun_సమితి_output_handler (crun_output_handler handler, శూన్య *arg)
{
  output_handler = handler;
  output_handler_arg = arg;
}

స్థిర అక్షరం *
make_json_error (స్థిరరాశి అక్షరం *సందేశం, పూర్ణాంకం errno_, పూర్ణాంకం verbosity)
{
  స్థిరరాశి అక్షరం *level;
  ఎంపిక (verbosity)
    {
    స్థితి LIBCRUN_VERBOSITY_DEBUG:
      level = "debug";
      ఆపు;
    స్థితి LIBCRUN_VERBOSITY_WARNING:
      level = "warning";
      ఆపు;
    స్థితి LIBCRUN_VERBOSITY_ERROR:
      level = "error";
      ఆపు;
    }
  స్థిరరాశి అక్షరం *buf = ఖాళీ;
  json_gen_ctx *gen = ఖాళీ;
  అక్షరం *ret = ఖాళీ;
  size_t buf_len;
  timestamp_t timestamp = {
    0,
  };

  ఒకవేళ (! json_gen_init (&gen, ఖాళీ))
    తిరిగి ఖాళీ;

  get_timestamp (&timestamp, "");

  json_gen_map_open (gen);

  json_gen_string (gen, "msg", అక్షరశ్రేణి_పొడవు ("msg"));
  ఒకవేళ (errno_ == 0)
    json_gen_string (gen, సందేశం, అక్షరశ్రేణి_పొడవు (సందేశం));
  లేకపోతే
    {
      cleanup_free అక్షరం *tmp = ఖాళీ;

      xasprintf (&tmp, "%s: %s", సందేశం, strerror (errno_));
      json_gen_string (gen, tmp, అక్షరశ్రేణి_పొడవు (tmp));
    }

  json_gen_string (gen, "level", అక్షరశ్రేణి_పొడవు ("level"));
  json_gen_string (gen, level, అక్షరశ్రేణి_పొడవు (level));

  json_gen_string (gen, "time", అక్షరశ్రేణి_పొడవు ("time"));
  json_gen_string (gen, timestamp, అక్షరశ్రేణి_పొడవు (timestamp));

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);
  ఒకవేళ (buf)
    ret = strdup (buf);

  json_gen_free (gen);

  తిరిగి ret;
}

స్థిర శూన్య
write_log (పూర్ణాంకం errno_, పూర్ణాంకం verbosity, స్థిరరాశి అక్షరం *సందేశం, va_list వాదనలు_list)
{
  పూర్ణాంకం ret;
  cleanup_free అక్షరం *output = ఖాళీ;
  cleanup_free అక్షరం *json = ఖాళీ;

  ఒకవేళ (verbosity > output_verbosity)
    తిరిగి;

  ret = vasprintf (&output, సందేశం, వాదనలు_list);
  ఒకవేళ (UNLIKELY (ret < 0))
    OOM ();

  ఒకవేళ (verbosity == LIBCRUN_VERBOSITY_ERROR && output_handler != log_write_గమ్యం_stderr)
    log_write_గమ్యం_stderr (errno_, output, LIBCRUN_VERBOSITY_ERROR, ఖాళీ);

  ఎంపిక (log_format)
    {
    స్థితి LOG_FORMAT_పాఠ్యం:
      output_handler (errno_, output, verbosity, output_handler_arg);
      ఆపు;

    స్థితి LOG_FORMAT_JSON:
      json = make_json_error (output, errno_, verbosity);
      ఒకవేళ (json)
        output_handler (0, json, verbosity, output_handler_arg);
      లేకపోతే
        output_handler (errno_, output, verbosity, output_handler_arg);
      ఆపు;
    }
}

శూన్య
libcrun_debug (స్థిరరాశి అక్షరం *సందేశం, ...)
{
  va_list వాదనలు_list;
  va_ప్రారంభం (వాదనలు_list, సందేశం);
  write_log (0, LIBCRUN_VERBOSITY_DEBUG, సందేశం, వాదనలు_list);
  va_చివర (వాదనలు_list);
}

శూన్య
libcrun_warning (స్థిరరాశి అక్షరం *సందేశం, ...)
{
  va_list వాదనలు_list;
  va_ప్రారంభం (వాదనలు_list, సందేశం);
  write_log (0, LIBCRUN_VERBOSITY_WARNING, సందేశం, వాదనలు_list);
  va_చివర (వాదనలు_list);
}

శూన్య
libcrun_error (పూర్ణాంకం errno_, స్థిరరాశి అక్షరం *సందేశం, ...)
{
  va_list వాదనలు_list;
  va_ప్రారంభం (వాదనలు_list, సందేశం);

  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, సందేశం, వాదనలు_list);
  va_చివర (వాదనలు_list);
}

శూన్య __attribute__ ((noreturn))
libcrun_fail_with_error (పూర్ణాంకం errno_, స్థిరరాశి అక్షరం *సందేశం, ...)
{
  va_list వాదనలు_list;
  va_ప్రారంభం (వాదనలు_list, సందేశం);
  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, సందేశం, వాదనలు_list);
  va_చివర (వాదనలు_list);
  నిష్క్రమించు (EXIT_FAILURE);
}

పూర్ణాంకం
libcrun_సమితి_log_format (స్థిరరాశి అక్షరం *format, libcrun_error_t *err)
{
  ఒకవేళ (strcmp (format, "text") == 0)
    log_format = LOG_FORMAT_పాఠ్యం;
  లేకపోతే ఒకవేళ (strcmp (format, "json") == 0)
    log_format = LOG_FORMAT_JSON;
  లేకపోతే
    తిరిగి crun_make_error (err, 0, "unknown log format type `%s`", format);

  తిరిగి 0;
}

పూర్ణాంకం
json_gen_error_గమ్యం_crun_error (పూర్ణాంకం స్థితి_2, libcrun_error_t *err)
{
  ఎంపిక (స్థితి_2)
    {
    స్థితి json_gen_స్థితి_ok:
      తిరిగి 0;

    అప్రమేయ:
      తిరిగి crun_make_error (err, 0, "generate JSON document: error %d", స్థితి_2);
    }
}
