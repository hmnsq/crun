/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2020 Adrian Reber <areber@redhat.com>
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CRIU_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CRIU_H

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "container.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "error.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

#ਜੇ HAVE_CRIU && HAVE_DLOPEN

ਪੂਰਨ libcrun_container_checkpoint_linux_criu (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, libcrun_container_t *container,
                                             libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

ਪੂਰਨ libcrun_container_restore_linux_criu (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, libcrun_container_t *container,
                                          libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

#ਨਹੀਂਤਾਂ

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
libcrun_container_checkpoint_linux_criu (arg_ਅਣਵਰਤਿਆ libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ,
                                         arg_ਅਣਵਰਤਿਆ libcrun_container_t *container,
                                         arg_ਅਣਵਰਤਿਆ libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  ਵਾਪਸ crun_make_error (err, 0, "compiled without CRIU support. Checkpointing not available");
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
libcrun_container_restore_linux_criu (arg_ਅਣਵਰਤਿਆ libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ,
                                      arg_ਅਣਵਰਤਿਆ libcrun_container_t *container,
                                      arg_ਅਣਵਰਤਿਆ libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  ਵਾਪਸ crun_make_error (err, 0, "compiled without CRIU support. Restore not available");
}

#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
