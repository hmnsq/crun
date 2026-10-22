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
#ଅନ୍ତର୍ଭାଵ <config.h>
#ifdef HAVE_ERROR_H
#  include <error.h>
#ଅନ୍ୟଥା
#  define error(status, errno, fmt, ...)                      \
    କର                                                        \
      {                                                       \
        ଯଦି (errno == 0)                                       \
          fprintf (stderr, "crun: " fmt "\n", ##__VA_ARGS__); \
        ଅନ୍ୟଥା                                                  \
          {                                                   \
            fprintf (stderr, "crun: " fmt, ##__VA_ARGS__);    \
            fprintf (stderr, ": %s\n", strerror (errno));     \
          }                                                   \
        ଯଦି (status)                                           \
          exit (status);                                      \
    } ଯେତେବେଳେ (0)
#endif
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdbool.h>
#ଅନ୍ତର୍ଭାଵ <syslog.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>

ସଂରଚନା libcrun_error_s
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା status;
  ଅକ୍ଷର *ସନ୍ଦେଶ;
};
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା libcrun_error_s *libcrun_error_t;

/* Coverage support: flush coverage data before _exit() in forked processes */
#ifdef HAVE_COVERAGE
ବାହ୍ୟ ଶୂନ୍ୟ __gcov_dump (ଶୂନ୍ୟ);
#  define GCOV_DUMP() __gcov_dump ()
#ଅନ୍ୟଥା
#  define GCOV_DUMP()
#endif

/* Use this instead of _exit() to ensure coverage data is flushed */
#define _safe_exit(status) \
  କର                       \
    {                      \
      GCOV_DUMP ();        \
      _exit (status);      \
  } ଯେତେବେଳେ (0)

#define OOM()                            \
  କର                                     \
    {                                    \
      fprintf (stderr, "out of memory"); \
      _safe_exit (EXIT_FAILURE);         \
  } ଯେତେବେଳେ (0)

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*crun_output_handler) (ପୂର୍ଣ୍ଣସଂଖ୍ୟା errno_, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity, ଶୂନ୍ୟ *arg);

ଶୂନ୍ୟ crun_set_output_handler (crun_output_handler handler, ଶୂନ୍ୟ *arg);

ଶୂନ୍ୟ log_write_to_journald (ପୂର୍ଣ୍ଣସଂଖ୍ୟା errno_, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity, ଶୂନ୍ୟ *arg);

ଶୂନ୍ୟ log_write_to_syslog (ପୂର୍ଣ୍ଣସଂଖ୍ୟା errno_, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity, ଶୂନ୍ୟ *arg);

ଶୂନ୍ୟ log_write_to_stream (ପୂର୍ଣ୍ଣସଂଖ୍ୟା errno_, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity, ଶୂନ୍ୟ *arg);

ଶୂନ୍ୟ log_write_to_stderr (ପୂର୍ଣ୍ଣସଂଖ୍ୟା errno_, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity, ଶୂନ୍ୟ *arg);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା crun_error_wrap (libcrun_error_t *err, ଅଚଳ ଅକ୍ଷର *fmt, ...) __attribute__ ((format (printf, 2, 3)));

ପୂର୍ଣ୍ଣସଂଖ୍ୟା crun_error_get_errno (libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା crun_error_release (libcrun_error_t *err);

ଶୂନ୍ୟ crun_error_write_warning_and_release (FILE *out, libcrun_error_t **err);

LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_debug (ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ...) __attribute__ ((format (printf, 1, 2)));

LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_warning (ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ...) __attribute__ ((format (printf, 1, 2)));

LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_error (ପୂର୍ଣ୍ଣସଂଖ୍ୟା errno_, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ...) __attribute__ ((format (printf, 2, 3)));

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_make_error (libcrun_error_t *err, ପୂର୍ଣ୍ଣସଂଖ୍ୟା status, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ...) __attribute__ ((format (printf, 3, 4)));

#define crun_make_error libcrun_make_error

LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_error_write_warning_and_release (FILE *out, libcrun_error_t **err);

LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_error_report_and_release (libcrun_error_t *err);

LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_fail_with_error (ପୂର୍ଣ୍ଣସଂଖ୍ୟା errno_, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ...) __attribute__ ((noreturn)) __attribute__ ((format (printf, 2, 3)));

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_set_log_format (ଅଚଳ ଅକ୍ଷର *format, libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_init_logging (crun_output_handler *output_handler, ଶୂନ୍ୟ **output_handler_arg, ଅଚଳ ଅକ୍ଷର *id,
                                         ଅଚଳ ଅକ୍ଷର *log, libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_error_release (libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_error_get_status (libcrun_error_t err);

LIBCRUN_PUBLIC ଅଚଳ ଅକ୍ଷର *libcrun_error_get_message (libcrun_error_t err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା json_gen_error_to_crun_error (ପୂର୍ଣ୍ଣସଂଖ୍ୟା gen_status, libcrun_error_t *err);

ଗଣନା
{
  LIBCRUN_VERBOSITY_ERROR = 0,
  LIBCRUN_VERBOSITY_WARNING,
  LIBCRUN_VERBOSITY_DEBUG,
};

LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_set_verbosity (ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity);
LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_get_verbosity ();

#endif
