/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2025 Giuseppe Scrivano <giuseppe@scrivano.org>
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

#ifndef NET_DEVICE_H
#define NET_DEVICE_H

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <ocispec/runtime_spec_schema_config_schema.h>
#অন্তর্ভাব "error.h"

পূর্ণাঙ্ক move_network_device (ধ্রুব বর্ণ *ifname, ধ্রুব বর্ণ *newifname, পূর্ণাঙ্ক netns_fd, libcrun_error_t *err);

#endif
