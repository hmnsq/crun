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

#اگر_غیر_تعریف_شدہ NET_DEVICE_H
#تعریف_کرو NET_DEVICE_H

#شامل <config.h>
#شامل <ocispec/runtime_spec_schema_config_schema.h>
#شامل "error.h"

صحیح move_جال_device (ثابت حرف *ifname, ثابت حرف *newifname, صحیح netns_fd, libcrun_error_t *err);

#اگر_ختم
