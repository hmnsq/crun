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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ERROR_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ERROR_H
#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_ERROR_H
#  include <error.h>
#ਨਹੀਂਤਾਂ
#  define error(ਸਥਿਤੀ, errno, fmt, ...)                      \
    ਕਰੋ                                                        \
      {                                                       \
        ਜੇ (errno == 0)                                       \
          fprintf (stderr, "crun: " fmt "\n", ##__VA_ARGS__); \
        ਨਹੀਂਤਾਂ                                                  \
          {                                                   \
            fprintf (stderr, "crun: " fmt, ##__VA_ARGS__);    \
            fprintf (stderr, ": %s\n", strerror (errno));     \
          }                                                   \
        ਜੇ (ਸਥਿਤੀ)                                           \
          ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);                                      \
    } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <syslog.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>

ਬਣਤਰ libcrun_error_s
{
  ਪੂਰਨ ਸਥਿਤੀ;
  ਅੱਖਰ *ਸੁਨੇਹਾ;
};
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ libcrun_error_s *libcrun_error_t;

/* Coverage support: flush coverage data before _exit() in forked processes */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_COVERAGE
ਬਾਹਰੀ ਖਾਲੀ __gcov_dump (ਖਾਲੀ);
#  define GCOV_DUMP() __gcov_dump ()
#ਨਹੀਂਤਾਂ
#  define GCOV_DUMP()
#ਜੇ_ਅੰਤ

/* Use this instead of _exit() to ensure coverage data is flushed */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੁਰੱਖਿਅਤ_exit(ਸਥਿਤੀ) \
  ਕਰੋ                       \
    {                      \
      GCOV_DUMP ();        \
      _exit (ਸਥਿਤੀ);      \
  } ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OOM()                            \
  ਕਰੋ                                     \
    {                                    \
      fprintf (stderr, "out of memory"); \
      ਸੁਰੱਖਿਅਤ_exit (EXIT_FAILURE);         \
  } ਜਦੋਂ (0)

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*crun_output_handler) (ਪੂਰਨ errno_, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ਪੂਰਨ verbosity, ਖਾਲੀ *arg);

ਖਾਲੀ crun_ਸੈੱਟ_output_handler (crun_output_handler handler, ਖਾਲੀ *arg);

ਖਾਲੀ log_write_ਮੰਜ਼ਿਲ_journald (ਪੂਰਨ errno_, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ਪੂਰਨ verbosity, ਖਾਲੀ *arg);

ਖਾਲੀ log_write_ਮੰਜ਼ਿਲ_syslog (ਪੂਰਨ errno_, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ਪੂਰਨ verbosity, ਖਾਲੀ *arg);

ਖਾਲੀ log_write_ਮੰਜ਼ਿਲ_ਧਾਰਾ (ਪੂਰਨ errno_, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ਪੂਰਨ verbosity, ਖਾਲੀ *arg);

ਖਾਲੀ log_write_ਮੰਜ਼ਿਲ_stderr (ਪੂਰਨ errno_, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ਪੂਰਨ verbosity, ਖਾਲੀ *arg);

ਪੂਰਨ crun_error_wrap (libcrun_error_t *err, ਅਚਲ ਅੱਖਰ *fmt, ...) __attribute__ ((format (ਲਿਖੋ, 2, 3)));

ਪੂਰਨ crun_error_get_errno (libcrun_error_t *err);

ਪੂਰਨ crun_error_release (libcrun_error_t *err);

ਖਾਲੀ crun_error_write_warning_and_release (FILE *ਬਾਹਰਲਾ, libcrun_error_t **err);

LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_debug (ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ...) __attribute__ ((format (ਲਿਖੋ, 1, 2)));

LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_warning (ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ...) __attribute__ ((format (ਲਿਖੋ, 1, 2)));

LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_error (ਪੂਰਨ errno_, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ...) __attribute__ ((format (ਲਿਖੋ, 2, 3)));

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_make_error (libcrun_error_t *err, ਪੂਰਨ ਸਥਿਤੀ, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ...) __attribute__ ((format (ਲਿਖੋ, 3, 4)));

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ crun_make_error libcrun_make_error

LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_error_write_warning_and_release (FILE *ਬਾਹਰਲਾ, libcrun_error_t **err);

LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_error_report_and_release (libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_fail_with_error (ਪੂਰਨ errno_, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ...) __attribute__ ((noreturn)) __attribute__ ((format (ਲਿਖੋ, 2, 3)));

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_ਸੈੱਟ_log_format (ਅਚਲ ਅੱਖਰ *format, libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_init_logging (crun_output_handler *output_handler, ਖਾਲੀ **output_handler_arg, ਅਚਲ ਅੱਖਰ *id,
                                         ਅਚਲ ਅੱਖਰ *ਲਘੂਗਣਕ, libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_error_release (libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_error_get_ਸਥਿਤੀ (libcrun_error_t err);

LIBCRUN_ਜਨਤਕ ਅਚਲ ਅੱਖਰ *libcrun_error_get_ਸੁਨੇਹਾ (libcrun_error_t err);

ਪੂਰਨ json_gen_error_ਮੰਜ਼ਿਲ_crun_error (ਪੂਰਨ gen_ਸਥਿਤੀ, libcrun_error_t *err);

ਗਿਣਤੀ
{
  LIBCRUN_VERBOSITY_ERROR = 0,
  LIBCRUN_VERBOSITY_WARNING,
  LIBCRUN_VERBOSITY_DEBUG,
};

LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_ਸੈੱਟ_verbosity (ਪੂਰਨ verbosity);
LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_get_verbosity ();

#ਜੇ_ਅੰਤ
