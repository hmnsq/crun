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
#ifndef ERROR_H
#define ERROR_H
#અન્તર્ભાવ <config.h>
#ifdef HAVE_ERROR_H
#  include <error.h>
#નહીંતર
#  define error(status, errno, fmt, ...)                      \
    કરો                                                        \
      {                                                       \
        જો (errno == 0)                                       \
          fprintf (stderr, "crun: " fmt "\n", ##__VA_ARGS__); \
        નહીંતર                                                  \
          {                                                   \
            fprintf (stderr, "crun: " fmt, ##__VA_ARGS__);    \
            fprintf (stderr, ": %s\n", strerror (errno));     \
          }                                                   \
        જો (status)                                           \
          exit (status);                                      \
    } જ્યારે (0)
#endif
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdbool.h>
#અન્તર્ભાવ <syslog.h>
#અન્તર્ભાવ <unistd.h>

સંરચના libcrun_error_s
{
  પૂર્ણાંક status;
  અક્ષર *સંદેશ;
};
પ્રકારવ્યાખ્યા સંરચના libcrun_error_s *libcrun_error_t;

/* Coverage support: flush coverage data before _exit() in forked processes */
#ifdef HAVE_COVERAGE
બાહ્ય શૂન્ય __gcov_dump (શૂન્ય);
#  define GCOV_DUMP() __gcov_dump ()
#નહીંતર
#  define GCOV_DUMP()
#endif

/* Use this instead of _exit() to ensure coverage data is flushed */
#define _safe_exit(status) \
  કરો                       \
    {                      \
      GCOV_DUMP ();        \
      _exit (status);      \
  } જ્યારે (0)

#define OOM()                            \
  કરો                                     \
    {                                    \
      fprintf (stderr, "out of memory"); \
      _safe_exit (EXIT_FAILURE);         \
  } જ્યારે (0)

પ્રકારવ્યાખ્યા શૂન્ય (*crun_output_handler) (પૂર્ણાંક errno_, અચલ અક્ષર *સંદેશ, પૂર્ણાંક verbosity, શૂન્ય *arg);

શૂન્ય crun_set_output_handler (crun_output_handler handler, શૂન્ય *arg);

શૂન્ય log_write_to_journald (પૂર્ણાંક errno_, અચલ અક્ષર *સંદેશ, પૂર્ણાંક verbosity, શૂન્ય *arg);

શૂન્ય log_write_to_syslog (પૂર્ણાંક errno_, અચલ અક્ષર *સંદેશ, પૂર્ણાંક verbosity, શૂન્ય *arg);

શૂન્ય log_write_to_stream (પૂર્ણાંક errno_, અચલ અક્ષર *સંદેશ, પૂર્ણાંક verbosity, શૂન્ય *arg);

શૂન્ય log_write_to_stderr (પૂર્ણાંક errno_, અચલ અક્ષર *સંદેશ, પૂર્ણાંક verbosity, શૂન્ય *arg);

પૂર્ણાંક crun_error_wrap (libcrun_error_t *err, અચલ અક્ષર *fmt, ...) __attribute__ ((format (printf, 2, 3)));

પૂર્ણાંક crun_error_get_errno (libcrun_error_t *err);

પૂર્ણાંક crun_error_release (libcrun_error_t *err);

શૂન્ય crun_error_write_warning_and_release (FILE *out, libcrun_error_t **err);

LIBCRUN_PUBLIC શૂન્ય libcrun_debug (અચલ અક્ષર *સંદેશ, ...) __attribute__ ((format (printf, 1, 2)));

LIBCRUN_PUBLIC શૂન્ય libcrun_warning (અચલ અક્ષર *સંદેશ, ...) __attribute__ ((format (printf, 1, 2)));

LIBCRUN_PUBLIC શૂન્ય libcrun_error (પૂર્ણાંક errno_, અચલ અક્ષર *સંદેશ, ...) __attribute__ ((format (printf, 2, 3)));

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_make_error (libcrun_error_t *err, પૂર્ણાંક status, અચલ અક્ષર *સંદેશ, ...) __attribute__ ((format (printf, 3, 4)));

#define crun_make_error libcrun_make_error

LIBCRUN_PUBLIC શૂન્ય libcrun_error_write_warning_and_release (FILE *out, libcrun_error_t **err);

LIBCRUN_PUBLIC શૂન્ય libcrun_error_report_and_release (libcrun_error_t *err);

LIBCRUN_PUBLIC શૂન્ય libcrun_fail_with_error (પૂર્ણાંક errno_, અચલ અક્ષર *સંદેશ, ...) __attribute__ ((noreturn)) __attribute__ ((format (printf, 2, 3)));

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_set_log_format (અચલ અક્ષર *format, libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_init_logging (crun_output_handler *output_handler, શૂન્ય **output_handler_arg, અચલ અક્ષર *id,
                                         અચલ અક્ષર *log, libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_error_release (libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_error_get_status (libcrun_error_t err);

LIBCRUN_PUBLIC અચલ અક્ષર *libcrun_error_get_message (libcrun_error_t err);

પૂર્ણાંક json_gen_error_to_crun_error (પૂર્ણાંક gen_status, libcrun_error_t *err);

ગણના
{
  LIBCRUN_VERBOSITY_ERROR = 0,
  LIBCRUN_VERBOSITY_WARNING,
  LIBCRUN_VERBOSITY_DEBUG,
};

LIBCRUN_PUBLIC શૂન્ય libcrun_set_verbosity (પૂર્ણાંક verbosity);
LIBCRUN_PUBLIC પૂર્ણાંક libcrun_get_verbosity ();

#endif
