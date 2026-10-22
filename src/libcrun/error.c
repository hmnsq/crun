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

#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ
#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdarg.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <sys/time.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ "utils.h"

#ಅನ್ತರ್ಭಾವ <json-c/json.h>
#ಅನ್ತರ್ಭಾವ <ocispec/json_common.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYSTEMD
#  include <systemd/sd-journal.h>
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ
{
  LOG_FORMAT_ಪಠ್ಯ = 0,
  LOG_FORMAT_JSON,
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ log_format;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ output_verbosity = LIBCRUN_VERBOSITY_ERROR;

ಪೂರ್ಣಾಂಕ
libcrun_make_error (libcrun_error_t *err, ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ...)
{
  va_list ವಾದಗಳು_list;
  libcrun_error_t ptr;

  va_ಆರಂಭ (ವಾದಗಳು_list, ಸಂದೇಶ);
  *err = xmalloc (ಗಾತ್ರ (ರಚನೆ libcrun_error_s));
  ptr = *err;
  ptr->ಸ್ಥಿತಿ_2 = ಸ್ಥಿತಿ_2;
  ಆದರೆ (vasprintf (&(ptr->ಸಂದೇಶ), ಸಂದೇಶ, ವಾದಗಳು_list) < 0)
    OOM ();
  va_ಕೊನೆ (ವಾದಗಳು_list);

  ಹಿಂತಿರುಗಿ -ಸ್ಥಿತಿ_2 - 1;
}

ಪೂರ್ಣಾಂಕ
crun_error_wrap (libcrun_error_t *err, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
  cleanup_free ಅಕ್ಷರ *ಸಂದೇಶ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *tmp = ಖಾಲಿ;
  va_list ವಾದಗಳು_list;
  ಅಕ್ಷರ *ಅದಲುಬದಲು;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (err == ಖಾಲಿ || *err == ಖಾಲಿ)
    {
      // Internal error
      ಹಿಂತಿರುಗಿ 0;
    }

  ret = -(*err)->ಸ್ಥಿತಿ_2 - 1;

  va_ಆರಂಭ (ವಾದಗಳು_list, fmt);

  ಆದರೆ (vasprintf (&ಸಂದೇಶ, fmt, ವಾದಗಳು_list) < 0)
    {
      va_ಕೊನೆ (ವಾದಗಳು_list);
      ಸಂದೇಶ = ಖಾಲಿ;
      ಹಿಂತಿರುಗಿ ret;
    }
  va_ಕೊನೆ (ವಾದಗಳು_list);

  xasprintf (&tmp, "%s: %s", ಸಂದೇಶ, (*err)->ಸಂದೇಶ);
  ಅದಲುಬದಲು = tmp;
  tmp = (*err)->ಸಂದೇಶ;
  (*err)->ಸಂದೇಶ = ಅದಲುಬದಲು;

  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
crun_error_release (libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ saved_errno = errno;
  libcrun_error_t ptr;

  ಆದರೆ (err == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ptr = *err;
  ಆದರೆ (ptr == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ಬಿಡುಗಡೆ (ptr->ಸಂದೇಶ);
  ಬಿಡುಗಡೆ (ptr);
  *err = ಖಾಲಿ;

  errno = saved_errno;
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_error_release (libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ crun_error_release (err);
}

ಪೂರ್ಣಾಂಕ
libcrun_error_get_ಸ್ಥಿತಿ (libcrun_error_t err)
{
  ಆದರೆ (err == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;
  ಹಿಂತಿರುಗಿ err->ಸ್ಥಿತಿ_2;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
libcrun_error_get_ಸಂದೇಶ (libcrun_error_t err)
{
  ಆದರೆ (err == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
  ಹಿಂತಿರುಗಿ err->ಸಂದೇಶ;
}

ಶೂನ್ಯ
crun_error_write_warning_and_release (FILE *ಹೊರಹೋಗು, libcrun_error_t **err)
{
  ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ saved_errno = errno;
  libcrun_error_t ref;

  ಆದರೆ (ಹೊರಹೋಗು == ಖಾಲಿ)
    ಹೊರಹೋಗು = stderr;
  ಆದರೆ (err == ಖಾಲಿ || *err == ಖಾಲಿ || **err == ಖಾಲಿ)
    {
      // Internal error
      ಹಿಂತಿರುಗಿ;
    }

  ref = **err;
  ಆದರೆ (ref->ಸ್ಥಿತಿ_2)
    fprintf (ಹೊರಹೋಗು, "%s: %s\n", ref->ಸಂದೇಶ, strerror (ref->ಸ್ಥಿತಿ_2));
  ಇಲ್ಲದಿದ್ದರೆ
    fprintf (ಹೊರಹೋಗು, "%s\n", ref->ಸಂದೇಶ);

  ಬಿಡುಗಡೆ (ref->ಸಂದೇಶ);
  ಬಿಡುಗಡೆ (ref);
  **err = ಖಾಲಿ;

  errno = saved_errno;
}

ಶೂನ್ಯ
libcrun_error_write_warning_and_release (FILE *ಹೊರಹೋಗು, libcrun_error_t **err)
{
  ಹಿಂತಿರುಗಿ crun_error_write_warning_and_release (ಹೊರಹೋಗು, err);
}

ಶೂನ್ಯ
libcrun_error_report_and_release (libcrun_error_t *err)
{
  libcrun_error_t ref;

  ಆದರೆ (err == ಖಾಲಿ || *err == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;

  ref = *err;
  libcrun_error (ref->ಸ್ಥಿತಿ_2, "%s", ref->ಸಂದೇಶ);
  crun_error_release (err);
}

ಪೂರ್ಣಾಂಕ
crun_error_get_errno (libcrun_error_t *err)
{
  ಆದರೆ (err == ಖಾಲಿ || *err == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;
  ಹಿಂತಿರುಗಿ (*err)->ಸ್ಥಿತಿ_2;
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಅಕ್ಷರ timestamp_t[64];

ಸ್ಥಿರ ಶೂನ್ಯ
get_timestamp (timestamp_t *timestamp, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *suffix)
{
  ಸ್ಥಿರಾಂಕ size_t ಬಫರ್_ಗಾತ್ರ = ಗಾತ್ರ (timestamp_t);
  ಅಕ್ಷರ *ಬಫರ್ = (ಅಕ್ಷರ *) timestamp;
  ರಚನೆ timeval tv;
  size_t written;
  ರಚನೆ tm now;

  gettimeofday (&tv, ಖಾಲಿ);
  gmtime_r (&tv.tv_sec, &now);
  written = strftime (ಬಫರ್, ಬಫರ್_ಗಾತ್ರ, "%Y-%m-%dT%H:%M:%S", &now);
  snprintf (ಬಫರ್ + written, ಬಫರ್_ಗಾತ್ರ - written, ".%06lldZ%.8s", (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ) tv.tv_usec, suffix);
}

/* Neither openlog(3) nor the journald handler copy the identifier they are
   given, and the global output handler outlives the caller's strings (e.g.
   the context is freed while errors are still reported through it), so keep
   an owned copy alive for as long as the handler can use it.  */
ಸ್ಥಿರ ಅಕ್ಷರ *log_id;

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
ಗಣ_log_id (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಅಕ್ಷರ **old_id)
{
  *old_id = log_id;
  log_id = id ? xstrdup (id) : ಖಾಲಿ;
  ಹಿಂತಿರುಗಿ log_id;
}

ಸ್ಥಿರ ಶೂನ್ಯ *
init_syslog (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id)
{
  openlog (id, 0, LOG_USER);
  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಎಣಿಕೆ
{
  LOG_TYPE_FILE = 1,
  LOG_TYPE_SYSLOG = 2,
  LOG_TYPE_JOURNALD = 3
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_log_type (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಲಘುಗಣಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ದತ್ತಾಂಶ)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sep = strchr (ಲಘುಗಣಕ, ':');
  ಆದರೆ (sep == ಖಾಲಿ)
    {
      *ದತ್ತಾಂಶ = ಲಘುಗಣಕ;
      ಹಿಂತಿರುಗಿ LOG_TYPE_FILE;
    }
  *ದತ್ತಾಂಶ = sep + 1;

  ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (ಲಘುಗಣಕ, "syslog:"))
    ಹಿಂತಿರುಗಿ LOG_TYPE_SYSLOG;
  ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (ಲಘುಗಣಕ, "journald:"))
    ಹಿಂತಿರುಗಿ LOG_TYPE_JOURNALD;
  ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (ಲಘುಗಣಕ, "file:"))
    ಹಿಂತಿರುಗಿ LOG_TYPE_FILE;

  ಹಿಂತಿರುಗಿ -1;
}

ಪೂರ್ಣಾಂಕ
libcrun_init_logging (crun_output_handler *new_output_handler, ಶೂನ್ಯ **new_output_handler_arg, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id,
                      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಲಘುಗಣಕ, libcrun_error_t *err)
{
  /* Released only after the new handler is installed.  */
  cleanup_free ಅಕ್ಷರ *old_log_id = ಖಾಲಿ;

  ಆದರೆ (ಲಘುಗಣಕ == ಖಾಲಿ)
    {
      *new_output_handler = log_write_ಗಮ್ಯ_stderr;
      *new_output_handler_arg = ಖಾಲಿ;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg = ಖಾಲಿ;
      ಪೂರ್ಣಾಂಕ log_type = get_log_type (ಲಘುಗಣಕ, &arg);

      ಆದರೆ (log_type < 0)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "unknown log type `%s`", ಲಘುಗಣಕ);

      ಆಯ್ಕೆ (log_type)
        {
        ಸ್ಥಿತಿ LOG_TYPE_FILE:
          *new_output_handler = log_write_ಗಮ್ಯ_ಹರಿವು;
          *new_output_handler_arg = fopen (arg, "a+e");
          ಆದರೆ (*new_output_handler_arg == ಖಾಲಿ)
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open log file `%s`", ಲಘುಗಣಕ);
          ಆದರೆ (output_verbosity >= LIBCRUN_VERBOSITY_WARNING)
            setlinebuf (*new_output_handler_arg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LOG_TYPE_SYSLOG:
          *new_output_handler_arg = init_syslog (ಗಣ_log_id (arg[0] ? arg : id, &old_log_id));
          *new_output_handler = log_write_ಗಮ್ಯ_syslog;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ LOG_TYPE_JOURNALD:
          *new_output_handler = log_write_ಗಮ್ಯ_journald;
          *new_output_handler_arg = (ಶೂನ್ಯ *) ಗಣ_log_id (id, &old_log_id);
          ನಿಲ್ಲಿಸು;
        }
    }
  crun_ಗಣ_output_handler (*new_output_handler, *new_output_handler_arg);
  ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ
log_write_ಗಮ್ಯ_ಹರಿವು (ಪೂರ್ಣಾಂಕ errno_, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ಪೂರ್ಣಾಂಕ verbosity, ಶೂನ್ಯ *arg)
{
  timestamp_t timestamp = {
    0,
  };
  FILE *ಹರಿವು = arg;
  ಪೂರ್ಣಾಂಕ tty = isatty (fileno (ಹರಿವು));
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *color_begin = "";
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *color_ಕೊನೆ = "";

  ಆದರೆ (tty)
    {
      ಆಯ್ಕೆ (verbosity)
        {
        ಸ್ಥಿತಿ LIBCRUN_VERBOSITY_DEBUG:
          color_begin = "\x1b[1;34m";
          color_ಕೊನೆ = "\x1b[0m";
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LIBCRUN_VERBOSITY_WARNING:
          color_begin = "\x1b[1;33m";
          color_ಕೊನೆ = "\x1b[0m";
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LIBCRUN_VERBOSITY_ERROR:
          color_begin = "\x1b[1;31m";
          color_ಕೊನೆ = "\x1b[0m";
          ನಿಲ್ಲಿಸು;
        }

      ಆದರೆ (log_format == LOG_FORMAT_ಪಠ್ಯ)
        get_timestamp (&timestamp, ": ");
    }

  ಆದರೆ (errno_)
    fprintf (ಹರಿವು, "%s%s%s: %s%s\n", color_begin, timestamp, ಸಂದೇಶ, strerror (errno_), color_ಕೊನೆ);
  ಇಲ್ಲದಿದ್ದರೆ
    fprintf (ಹರಿವು, "%s%s%s%s\n", color_begin, timestamp, ಸಂದೇಶ, color_ಕೊನೆ);
}

ಶೂನ್ಯ
log_write_ಗಮ್ಯ_stderr (ಪೂರ್ಣಾಂಕ errno_, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ಪೂರ್ಣಾಂಕ verbosity, ಶೂನ್ಯ *arg arg_ಬಳಸದ)
{
  log_write_ಗಮ್ಯ_ಹರಿವು (errno_, ಸಂದೇಶ, verbosity, stderr);
}

ಶೂನ್ಯ
log_write_ಗಮ್ಯ_syslog (ಪೂರ್ಣಾಂಕ errno_, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ಪೂರ್ಣಾಂಕ verbosity, ಶೂನ್ಯ *arg arg_ಬಳಸದ)
{
  ಪೂರ್ಣಾಂಕ ಆದ್ಯತೆ = LOG_ERR;
  ಆಯ್ಕೆ (verbosity)
    {
    ಸ್ಥಿತಿ LIBCRUN_VERBOSITY_DEBUG:
      ಆದ್ಯತೆ = LOG_DEBUG;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ LIBCRUN_VERBOSITY_WARNING:
      ಆದ್ಯತೆ = LOG_WARNING;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ LIBCRUN_VERBOSITY_ERROR:
      ಆದ್ಯತೆ = LOG_ERR;
      ನಿಲ್ಲಿಸು;
    }
  ಆದರೆ (errno_ == 0)
    syslog (ಆದ್ಯತೆ, "%s", ಸಂದೇಶ);
  ಇಲ್ಲದಿದ್ದರೆ
    syslog (ಆದ್ಯತೆ, "%s: %s", ಸಂದೇಶ, strerror (errno_));
}

ಶೂನ್ಯ
log_write_ಗಮ್ಯ_journald (ಪೂರ್ಣಾಂಕ errno_, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ಪೂರ್ಣಾಂಕ verbosity, ಶೂನ್ಯ *arg arg_ಬಳಸದ)
{
  (ಶೂನ್ಯ) errno_;
  (ಶೂನ್ಯ) ಸಂದೇಶ;
  (ಶೂನ್ಯ) verbosity;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYSTEMD
  ಪೂರ್ಣಾಂಕ ಆದ್ಯತೆ = LOG_ERR;
  ಆಯ್ಕೆ (verbosity)
    {
    ಸ್ಥಿತಿ LIBCRUN_VERBOSITY_DEBUG:
      ಆದ್ಯತೆ = LOG_DEBUG;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ LIBCRUN_VERBOSITY_WARNING:
      ಆದ್ಯತೆ = LOG_WARNING;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ LIBCRUN_VERBOSITY_ERROR:
      ಆದ್ಯತೆ = LOG_ERR;
      ನಿಲ್ಲಿಸು;
    }
  ಆದರೆ (errno_ == 0)
    sd_journal_send ("PRIORITY=%d", ಆದ್ಯತೆ, "MESSAGE=%s", ಸಂದೇಶ, "ID=%s", arg, ಖಾಲಿ);
  ಇಲ್ಲದಿದ್ದರೆ
    sd_journal_send ("PRIORITY=%d", ಆದ್ಯತೆ, "MESSAGE=%s: %s", ಸಂದೇಶ, strerror (errno_), "ID=%s",
                     arg, ಖಾಲಿ);
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ crun_output_handler output_handler = log_write_ಗಮ್ಯ_stderr;
ಸ್ಥಿರ ಶೂನ್ಯ *output_handler_arg = ಖಾಲಿ;

ಶೂನ್ಯ
libcrun_ಗಣ_verbosity (ಪೂರ್ಣಾಂಕ verbosity)
{
  output_verbosity = verbosity;
}

ಪೂರ್ಣಾಂಕ
libcrun_get_verbosity ()
{
  ಹಿಂತಿರುಗಿ output_verbosity;
}

ಶೂನ್ಯ
crun_ಗಣ_output_handler (crun_output_handler handler, ಶೂನ್ಯ *arg)
{
  output_handler = handler;
  output_handler_arg = arg;
}

ಸ್ಥಿರ ಅಕ್ಷರ *
make_json_error (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ಪೂರ್ಣಾಂಕ errno_, ಪೂರ್ಣಾಂಕ verbosity)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *level;
  ಆಯ್ಕೆ (verbosity)
    {
    ಸ್ಥಿತಿ LIBCRUN_VERBOSITY_DEBUG:
      level = "debug";
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ LIBCRUN_VERBOSITY_WARNING:
      level = "warning";
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ LIBCRUN_VERBOSITY_ERROR:
      level = "error";
      ನಿಲ್ಲಿಸು;
    }
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *buf = ಖಾಲಿ;
  json_gen_ctx *gen = ಖಾಲಿ;
  ಅಕ್ಷರ *ret = ಖಾಲಿ;
  size_t buf_len;
  timestamp_t timestamp = {
    0,
  };

  ಆದರೆ (! json_gen_init (&gen, ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  get_timestamp (&timestamp, "");

  json_gen_map_open (gen);

  json_gen_string (gen, "msg", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("msg"));
  ಆದರೆ (errno_ == 0)
    json_gen_string (gen, ಸಂದೇಶ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಸಂದೇಶ));
  ಇಲ್ಲದಿದ್ದರೆ
    {
      cleanup_free ಅಕ್ಷರ *tmp = ಖಾಲಿ;

      xasprintf (&tmp, "%s: %s", ಸಂದೇಶ, strerror (errno_));
      json_gen_string (gen, tmp, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (tmp));
    }

  json_gen_string (gen, "level", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("level"));
  json_gen_string (gen, level, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (level));

  json_gen_string (gen, "time", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("time"));
  json_gen_string (gen, timestamp, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (timestamp));

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);
  ಆದರೆ (buf)
    ret = strdup (buf);

  json_gen_free (gen);

  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ
write_log (ಪೂರ್ಣಾಂಕ errno_, ಪೂರ್ಣಾಂಕ verbosity, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, va_list ವಾದಗಳು_list)
{
  ಪೂರ್ಣಾಂಕ ret;
  cleanup_free ಅಕ್ಷರ *output = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *json = ಖಾಲಿ;

  ಆದರೆ (verbosity > output_verbosity)
    ಹಿಂತಿರುಗಿ;

  ret = vasprintf (&output, ಸಂದೇಶ, ವಾದಗಳು_list);
  ಆದರೆ (UNLIKELY (ret < 0))
    OOM ();

  ಆದರೆ (verbosity == LIBCRUN_VERBOSITY_ERROR && output_handler != log_write_ಗಮ್ಯ_stderr)
    log_write_ಗಮ್ಯ_stderr (errno_, output, LIBCRUN_VERBOSITY_ERROR, ಖಾಲಿ);

  ಆಯ್ಕೆ (log_format)
    {
    ಸ್ಥಿತಿ LOG_FORMAT_ಪಠ್ಯ:
      output_handler (errno_, output, verbosity, output_handler_arg);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ LOG_FORMAT_JSON:
      json = make_json_error (output, errno_, verbosity);
      ಆದರೆ (json)
        output_handler (0, json, verbosity, output_handler_arg);
      ಇಲ್ಲದಿದ್ದರೆ
        output_handler (errno_, output, verbosity, output_handler_arg);
      ನಿಲ್ಲಿಸು;
    }
}

ಶೂನ್ಯ
libcrun_debug (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ...)
{
  va_list ವಾದಗಳು_list;
  va_ಆರಂಭ (ವಾದಗಳು_list, ಸಂದೇಶ);
  write_log (0, LIBCRUN_VERBOSITY_DEBUG, ಸಂದೇಶ, ವಾದಗಳು_list);
  va_ಕೊನೆ (ವಾದಗಳು_list);
}

ಶೂನ್ಯ
libcrun_warning (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ...)
{
  va_list ವಾದಗಳು_list;
  va_ಆರಂಭ (ವಾದಗಳು_list, ಸಂದೇಶ);
  write_log (0, LIBCRUN_VERBOSITY_WARNING, ಸಂದೇಶ, ವಾದಗಳು_list);
  va_ಕೊನೆ (ವಾದಗಳು_list);
}

ಶೂನ್ಯ
libcrun_error (ಪೂರ್ಣಾಂಕ errno_, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ...)
{
  va_list ವಾದಗಳು_list;
  va_ಆರಂಭ (ವಾದಗಳು_list, ಸಂದೇಶ);

  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, ಸಂದೇಶ, ವಾದಗಳು_list);
  va_ಕೊನೆ (ವಾದಗಳು_list);
}

ಶೂನ್ಯ __attribute__ ((noreturn))
libcrun_fail_with_error (ಪೂರ್ಣಾಂಕ errno_, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ...)
{
  va_list ವಾದಗಳು_list;
  va_ಆರಂಭ (ವಾದಗಳು_list, ಸಂದೇಶ);
  write_log (errno_, LIBCRUN_VERBOSITY_ERROR, ಸಂದೇಶ, ವಾದಗಳು_list);
  va_ಕೊನೆ (ವಾದಗಳು_list);
  ನಿರ್ಗಮಿಸು (EXIT_FAILURE);
}

ಪೂರ್ಣಾಂಕ
libcrun_ಗಣ_log_format (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format, libcrun_error_t *err)
{
  ಆದರೆ (strcmp (format, "text") == 0)
    log_format = LOG_FORMAT_ಪಠ್ಯ;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (format, "json") == 0)
    log_format = LOG_FORMAT_JSON;
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "unknown log format type `%s`", format);

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
json_gen_error_ಗಮ್ಯ_crun_error (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2, libcrun_error_t *err)
{
  ಆಯ್ಕೆ (ಸ್ಥಿತಿ_2)
    {
    ಸ್ಥಿತಿ json_gen_ಸ್ಥಿತಿ_ok:
      ಹಿಂತಿರುಗಿ 0;

    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "generate JSON document: error %d", ಸ್ಥಿತಿ_2);
    }
}
