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
#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ "spec.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "cgroup.h"
#ಅನ್ತರ್ಭಾವ "cgroup-utils.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ spec_file[] = "\
{\n\
	\"oci_ಆವೃತ್ತಿ\": \"1.0.0\",\n\
	\"process\": {\n\
		\"terminal\": true,\n\
		\"user\": {\n\
			\"uid\": 0,\n\
			\"gid\": 0\n\
		},\n\
		\"ವಾದಗಳು\": [\n\
			\"sh\"\n\
		],\n\
		\"env\": [\n\
			\"ಮಾರ್ಗ_3=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin\",\n\
			\"ಪದ=xterm\"\n\
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
	\"ಬೇರು\": {\n\
		\"ಮಾರ್ಗ\": \"rootfs\",\n\
		\"readonly\": true\n\
	},\n\
	\"hostname\": \"crun\",\n\
	\"mounts\": [\n\
		{\n\
			\"destination\": \"/proc\",\n\
			\"type\": \"proc\",\n\
			\"ಮೂಲ\": \"proc\"\n\
		},\n\
		{\n\
			\"destination\": \"/dev\",\n\
			\"type\": \"tmpfs\",\n\
			\"ಮೂಲ\": \"tmpfs\",\n\
			\"options\": [\n\
				\"nosuid\",\n\
				\"strictatime\",\n\
				\"ಬಹುಲಕ=755\",\n\
				\"ಗಾತ್ರ_2=65536k\"\n\
			]\n\
		},\n\
		{\n\
			\"destination\": \"/dev/pts\",\n\
			\"type\": \"devpts\",\n\
			\"ಮೂಲ\": \"devpts\",\n\
			\"options\": [\n\
				\"nosuid\",\n\
				\"noexec\",\n\
				\"newinstance\",\n\
				\"ptmxmode=0666\",\n\
				\"ಬಹುಲಕ=0620\"\
%s\
			]\n\
		},\n\
		{\n\
			\"destination\": \"/dev/shm\",\n\
			\"type\": \"tmpfs\",\n\
			\"ಮೂಲ\": \"shm\",\n\
			\"options\": [\n\
				\"nosuid\",\n\
				\"noexec\",\n\
				\"nodev\",\n\
				\"ಬಹುಲಕ=1777\",\n\
				\"ಗಾತ್ರ_2=65536k\"\n\
			]\n\
		},\n\
		{\n\
			\"destination\": \"/dev/mqueue\",\n\
			\"type\": \"mqueue\",\n\
			\"ಮೂಲ\": \"mqueue\",\n\
			\"options\": [\n\
				\"nosuid\",\n\
				\"noexec\",\n\
				\"nodev\"\n\
			]\n\
		},\n\
		{\n\
			\"destination\": \"/sys\",\n\
			\"type\": \"sysfs\",\n\
			\"ಮೂಲ\": \"sysfs\",\n\
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
			\"ಮೂಲ\": \"cgroup\",\n\
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
				\"type\": \"ಜಾಲ\"\n\
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
			\"/proc/ಕೀಲಿಗಳು\",\n\
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

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *spec_pts_tty_ಗುಂಪು = ",\n\
				\"gid=5\"\n";

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *spec_user = "\
			{\n\
				\"type\": \"user\"\n\
			},\n";

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *spec_cgroupns = "\
			{\n\
				\"type\": \"cgroup\"\n\
			},\n";

ಪೂರ್ಣಾಂಕ
libcrun_container_spec (bool ಬೇರು, FILE *ಹೊರಹೋಗು, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  ret = fprintf (ಹೊರಹೋಗು, spec_file,
                 ಬೇರು ? spec_pts_tty_ಗುಂಪು : "\n",
                 ಬೇರು ? "" : spec_user,
                 cgroup_ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED ? spec_cgroupns : "");
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fprintf failed");

  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
libcrun_container_spec_json (bool ಬೇರು, ಅಕ್ಷರ **ಹೊರಹೋಗು, libcrun_error_t *err)
{
  ಅಕ್ಷರ *ಬಫರ್ = ಖಾಲಿ;
  size_t len = 0;
  FILE *f;
  ಪೂರ್ಣಾಂಕ ret;

  f = open_memstream (&ಬಫರ್, &len);
  ಆದರೆ (f == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_spec (ಬೇರು, f, err);
  fclose (f);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಬಿಡುಗಡೆ (ಬಫರ್);
      ಹಿಂತಿರುಗಿ ret;
    }

  *ಹೊರಹೋಗು = ಬಫರ್;
  ಹಿಂತಿರುಗಿ 0;
}
