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
#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <errno.h>
#అన్తర్భావ "spec.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "cgroup.h"
#అన్తర్భావ "cgroup-utils.h"

స్థిర స్థిరరాశి అక్షరం spec_file[] = "\
{\n\
	\"oci_సంస్కరణ\": \"1.0.0\",\n\
	\"process\": {\n\
		\"terminal\": true,\n\
		\"user\": {\n\
			\"uid\": 0,\n\
			\"gid\": 0\n\
		},\n\
		\"వాదనలు\": [\n\
			\"sh\"\n\
		],\n\
		\"env\": [\n\
			\"మార్గం_3=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin\",\n\
			\"పదం=xterm\"\n\
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
	\"మూలం\": {\n\
		\"మార్గం\": \"rootfs\",\n\
		\"readonly\": true\n\
	},\n\
	\"hostname\": \"crun\",\n\
	\"mounts\": [\n\
		{\n\
			\"destination\": \"/proc\",\n\
			\"type\": \"proc\",\n\
			\"ఆధారం\": \"proc\"\n\
		},\n\
		{\n\
			\"destination\": \"/dev\",\n\
			\"type\": \"tmpfs\",\n\
			\"ఆధారం\": \"tmpfs\",\n\
			\"options\": [\n\
				\"nosuid\",\n\
				\"strictatime\",\n\
				\"బహుళకం=755\",\n\
				\"కొలత=65536k\"\n\
			]\n\
		},\n\
		{\n\
			\"destination\": \"/dev/pts\",\n\
			\"type\": \"devpts\",\n\
			\"ఆధారం\": \"devpts\",\n\
			\"options\": [\n\
				\"nosuid\",\n\
				\"noexec\",\n\
				\"newinstance\",\n\
				\"ptmxmode=0666\",\n\
				\"బహుళకం=0620\"\
%s\
			]\n\
		},\n\
		{\n\
			\"destination\": \"/dev/shm\",\n\
			\"type\": \"tmpfs\",\n\
			\"ఆధారం\": \"shm\",\n\
			\"options\": [\n\
				\"nosuid\",\n\
				\"noexec\",\n\
				\"nodev\",\n\
				\"బహుళకం=1777\",\n\
				\"కొలత=65536k\"\n\
			]\n\
		},\n\
		{\n\
			\"destination\": \"/dev/mqueue\",\n\
			\"type\": \"mqueue\",\n\
			\"ఆధారం\": \"mqueue\",\n\
			\"options\": [\n\
				\"nosuid\",\n\
				\"noexec\",\n\
				\"nodev\"\n\
			]\n\
		},\n\
		{\n\
			\"destination\": \"/sys\",\n\
			\"type\": \"sysfs\",\n\
			\"ఆధారం\": \"sysfs\",\n\
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
			\"ఆధారం\": \"cgroup\",\n\
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
				\"type\": \"జాలం\"\n\
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
			\"/proc/కీలకాలు\",\n\
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

స్థిర స్థిరరాశి అక్షరం *spec_pts_tty_సమూహం = ",\n\
				\"gid=5\"\n";

స్థిర స్థిరరాశి అక్షరం *spec_user = "\
			{\n\
				\"type\": \"user\"\n\
			},\n";

స్థిర స్థిరరాశి అక్షరం *spec_cgroupns = "\
			{\n\
				\"type\": \"cgroup\"\n\
			},\n";

పూర్ణాంకం
libcrun_container_spec (bool మూలం, FILE *బహిర్గతం, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  పూర్ణాంకం cgroup_బహుళకం;

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;

  ret = fprintf (బహిర్గతం, spec_file,
                 మూలం ? spec_pts_tty_సమూహం : "\n",
                 మూలం ? "" : spec_user,
                 cgroup_బహుళకం == CGROUP_బహుళకం_UNIFIED ? spec_cgroupns : "");
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "fprintf failed");

  తిరిగి ret;
}

పూర్ణాంకం
libcrun_container_spec_json (bool మూలం, అక్షరం **బహిర్గతం, libcrun_error_t *err)
{
  అక్షరం *బఫరు = ఖాళీ;
  size_t len = 0;
  FILE *f;
  పూర్ణాంకం ret;

  f = open_memstream (&బఫరు, &len);
  ఒకవేళ (f == ఖాళీ)
    తిరిగి crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_spec (మూలం, f, err);
  fclose (f);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      విడుదల (బఫరు);
      తిరిగి ret;
    }

  *బహిర్గతం = బఫరు;
  తిరిగి 0;
}
