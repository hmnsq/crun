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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ TERMINAL_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TERMINAL_H
#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "container.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <termios.h>

ਖਾਲੀ cleanup_terminalp (ਖਾਲੀ *p);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_terminal __attribute__ ((cleanup (cleanup_terminalp)))

ਪੂਰਨ libcrun_new_terminal (ਅੱਖਰ **pty, libcrun_error_t *err);

ਪੂਰਨ libcrun_ਸੈੱਟ_stdio (ਪੂਰਨ ptmx_fd, ਅੱਖਰ *pty, libcrun_error_t *err);

ਪੂਰਨ libcrun_ਸੈੱਟ_raw (ਪੂਰਨ fd, ਖਾਲੀ **current_ਸਥਿਤੀ, libcrun_error_t *err);

ਪੂਰਨ libcrun_clear_onlcr (ਪੂਰਨ fd, libcrun_error_t *err);

ਪੂਰਨ libcrun_terminal_ਤਿਆਰੀ_ਮਾਪ (ਪੂਰਨ fd, ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ ਪੰਕਤੀਆਂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ ਖਾਨੇ_ਗਿਣਤੀ, libcrun_error_t *err);

#ਜੇ_ਅੰਤ
