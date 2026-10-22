/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019, 2020 Giuseppe Scrivano <giuseppe@scrivano.org>
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
#تعریف_کرو GNU_سروت

#ضمن <config.h>
#ضمن <stdbool.h>
#ضمن <stdio.h>
#ضمن <errno.h>
#ضمن "spec.h"
#ضمن "utils.h"
#ضمن "cgroup.h"
#ضمن "cgroup-utils.h"

ساكن ثابت حرف spec_file[] = "\
{\n\
	\"oci_نسخہ\": \"1.0.0\",\n\
	\"process\": {\n\
		\"terminal\": true,\n\
		\"user\": {\n\
			\"uid\": 0,\n\
			\"gid\": 0\n\
		},\n\
		\"دلیلاں\": [\n\
			\"sh\"\n\
		],\n\
		\"env\": [\n\
			\"رستہ_3=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin\",\n\
			\"رکن=xterm\"\n\
		],\n\
		\"cwd\": \"/\",\n\
		\"capabilities\": {\n\
			\"bounding\": [\n\
				\"CAP_AUDIT_WRITE\",\n\
				\"CAP_KILL\",\n\
				\"CAP_NET_BIND_SERVICE\"\n\
			],\n\
			\"effective\": [\n\
				\"CAP_AUDIT_WRITE\",\n\
				\"CAP_KILL\",\n\
				\"CAP_NET_BIND_SERVICE\"\n\
			],\n\
			\"inheritable\": [\n\
			],\n\
			\"permitted\": [\n\
				\"CAP_AUDIT_WRITE\",\n\
				\"CAP_KILL\",\n\
				\"CAP_NET_BIND_SERVICE\"\n\
			],\n\
			\"ambient\": [\n\
				\"CAP_AUDIT_WRITE\",\n\
				\"CAP_KILL\",\n\
				\"CAP_NET_BIND_SERVICE\"\n\
			]\n\
		},\n\
		\"rlimits\": [\n\
			{\n\
				\"type\": \"RLIMIT_NOFILE\",\n\
				\"hard\": 1024,\n\
				\"soft\": 1024\n\
			}\n\
		],\n\
		\"noNewPrivileges\": true\n\
	},\n\
	\"جڑ\": {\n\
		\"رستہ\": \"rootfs\",\n\
		\"readonly\": true\n\
	},\n\
	\"hostname\": \"crun\",\n\
	\"mounts\": [\n\
		{\n\
			\"destination\": \"/proc\",\n\
			\"type\": \"proc\",\n\
			\"سروت\": \"proc\"\n\
		},\n\
		{\n\
			\"destination\": \"/dev\",\n\
			\"type\": \"tmpfs\",\n\
			\"سروت\": \"tmpfs\",\n\
			\"options\": [\n\
				\"nosuid\",\n\
				\"strictatime\",\n\
				\"منوال=755\",\n\
				\"ناپ=65536k\"\n\
			]\n\
		},\n\
		{\n\
			\"destination\": \"/dev/pts\",\n\
			\"type\": \"devpts\",\n\
			\"سروت\": \"devpts\",\n\
			\"options\": [\n\
				\"nosuid\",\n\
				\"noexec\",\n\
				\"newinstance\",\n\
				\"ptmxmode=0666\",\n\
				\"منوال=0620\"\
%s\
			]\n\
		},\n\
		{\n\
			\"destination\": \"/dev/shm\",\n\
			\"type\": \"tmpfs\",\n\
			\"سروت\": \"shm\",\n\
			\"options\": [\n\
				\"nosuid\",\n\
				\"noexec\",\n\
				\"nodev\",\n\
				\"منوال=1777\",\n\
				\"ناپ=65536k\"\n\
			]\n\
		},\n\
		{\n\
			\"destination\": \"/dev/mqueue\",\n\
			\"type\": \"mqueue\",\n\
			\"سروت\": \"mqueue\",\n\
			\"options\": [\n\
				\"nosuid\",\n\
				\"noexec\",\n\
				\"nodev\"\n\
			]\n\
		},\n\
		{\n\
			\"destination\": \"/sys\",\n\
			\"type\": \"sysfs\",\n\
			\"سروت\": \"sysfs\",\n\
			\"options\": [\n\
				\"nosuid\",\n\
				\"noexec\",\n\
				\"nodev\",\n\
				\"ro\"\n\
			]\n\
		},\n\
		{\n\
			\"destination\": \"/sys/fs/cgroup\",\n\
			\"type\": \"cgroup\",\n\
			\"سروت\": \"cgroup\",\n\
			\"options\": [\n\
				\"nosuid\",\n\
				\"noexec\",\n\
				\"nodev\",\n\
				\"relatime\",\n\
				\"ro\"\n\
			]\n\
		}\n\
	],\n\
	\"linux\": {\n\
		\"resources\": {\n\
			\"devices\": [\n\
				{\n\
					\"allow\": false,\n\
					\"access\": \"rwm\"\n\
				}\n\
			]\n\
		},\n\
		\"namespaces\": [\n\
			{\n\
				\"type\": \"pid\"\n\
			},\n\
			{\n\
				\"type\": \"جال\"\n\
			},\n\
			{\n\
				\"type\": \"ipc\"\n\
			},\n\
			{\n\
				\"type\": \"uts\"\n\
			},\n\
%s\
%s\
			{\n\
				\"type\": \"mount\"\n\
			}\n\
		],\n\
		\"maskedPaths\": [\n\
			\"/proc/acpi\",\n\
			\"/proc/asound\",\n\
			\"/proc/kcore\",\n\
			\"/proc/کلیدیں\",\n\
			\"/proc/latency_stats\",\n\
			\"/proc/timer_list\",\n\
			\"/proc/timer_stats\",\n\
			\"/proc/sched_debug\",\n\
			\"/sys/firmware\",\n\
			\"/proc/scsi\"\n\
		],\n\
		\"readonlyPaths\": [\n\
			\"/proc/bus\",\n\
			\"/proc/fs\",\n\
			\"/proc/irq\",\n\
			\"/proc/sys\",\n\
			\"/proc/sysrq-trigger\"\n\
		]\n\
	}\n\
}\n";

ساكن ثابت حرف *spec_pts_tty_گروہ = ",\n\
				\"gid=5\"\n";

ساكن ثابت حرف *spec_user = "\
			{\n\
				\"type\": \"user\"\n\
			},\n";

ساكن ثابت حرف *spec_cgroupns = "\
			{\n\
				\"type\": \"cgroup\"\n\
			},\n";

صحيح
libcrun_container_spec (bool جڑ, FILE *باہرلا, libcrun_error_t *err)
{
  صحيح ret;
  صحيح cgroup_منوال;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  إذا (UNLIKELY (cgroup_منوال < 0))
    أرجع cgroup_منوال;

  ret = fprintf (باہرلا, spec_file,
                 جڑ ? spec_pts_tty_گروہ : "\n",
                 جڑ ? "" : spec_user,
                 cgroup_منوال == CGROUP_منوال_UNIFIED ? spec_cgroupns : "");
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "fprintf failed");

  أرجع ret;
}

صحيح
libcrun_container_spec_json (bool جڑ, حرف **باہرلا, libcrun_error_t *err)
{
  حرف *بفر = عدم;
  size_t len = 0;
  FILE *f;
  صحيح ret;

  f = open_memstream (&بفر, &len);
  إذا (f == عدم)
    أرجع crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_spec (جڑ, f, err);
  fclose (f);
  إذا (UNLIKELY (ret < 0))
    {
      حرر (بفر);
      أرجع ret;
    }

  *باہرلا = بفر;
  أرجع 0;
}
