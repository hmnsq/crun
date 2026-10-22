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

#define _GNU_SOURCE

#अन्तर्भाव <config.h>
#अन्तर्भाव "net_device.h"
#अन्तर्भाव "utils.h"

#अन्तर्भाव <sys/socket.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <sched.h>
#अन्तर्भाव <sys/wait.h>
#अन्तर्भाव <sys/xattr.h>
#अन्तर्भाव <linux/netlink.h>
#अन्तर्भाव <linux/rtnetlink.h>
#अन्तर्भाव <arpa/inet.h>
#अन्तर्भाव <net/if.h>
#सचे HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint चञ्चल uint
#  endif
#endif

संरचना ip_addr
{
  संरचना ifaddrmsg ifa;

  पूर्णाङ्क rta_len;
  वर्ण *rta;
};

संरचना nl_req
{
  संरचना nlmsghdr nlh;
  सङ्घ
  {
    संरचना ifaddrmsg ifa;
    संरचना ifinfomsg ifi;
  };
};

स्थिर atomic_uint nl_seq_counter;

स्थिर uint32_t
get_next_seq ()
{
  पटिनिवत्त (uint32_t) ++nl_seq_counter;
}

स्थिर uint32_t
reset_request (संरचना nl_req *req, पूर्णाङ्क type, पूर्णाङ्क flags, पूर्णाङ्क सन्देशः_len)
{
  uint32_t seq = get_next_seq ();

  memset (req, 0, परिमाण (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_LENGTH (सन्देशः_len);
  req->nlh.nlmsg_seq = seq;

  पटिनिवत्त seq;
}

स्थिर शून्य
cleanup_ip_addrsp (शून्य *p)
{
  संरचना ip_addr **pp = (संरचना ip_addr **) p;
  संरचना ip_addr *ip;
  सचे (*pp == रिक्त)
    पटिनिवत्त;

  अत्थाय (ip = *pp; ip->rta_len >= 0; ip++)
    मुक्त (ip->rta);

  मुक्त (*pp);
}

#define cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

स्थिर पूर्णाङ्क
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क sock = -1;
  संरचना sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  पूर्णाङ्क fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  सचे (sock < 0)
    पटिनिवत्त crun_make_error (err, errno, "netlink socket");

  सचे (bind (sock, (संरचना sockaddr *) &local, परिमाण (local)) < 0)
    पटिनिवत्त crun_make_error (err, errno, "bind");

  fd = sock;
  sock = -1;
  पटिनिवत्त fd;
}

स्थिर पूर्णाङ्क
append_rtattr (संरचना nlmsghdr *n, size_t maxlen, पूर्णाङ्क type, ध्रुव शून्य *data, size_t data_len, libcrun_error_t *err)
{
  पूर्णाङ्क len = RTA_LENGTH (data_len);
  संरचना rtattr *rta;

  सचे (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    पटिनिवत्त crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (संरचना rtattr *) (((वर्ण *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  सचे (data_len)
    memcpy (RTA_DATA (rta), data, data_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
send_request (पूर्णाङ्क sock, संरचना nl_req *req, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = TEMP_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "send");

  पटिनिवत्त 0;
}

/* if_nametoindex with an open netlink socket.  */
स्थिर पूर्णाङ्क
name_to_index (पूर्णाङ्क sock, ध्रुव वर्ण *ifname, वर्ण *buffer, size_t buffer_size, libcrun_error_t *err)
{
  संरचना nlmsghdr *nlh;
  संरचना nl_req *req;
  uint32_t seq;
  ssize_t len;
  पूर्णाङ्क ret;

  req = (संरचना nl_req *) buffer;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, परिमाण (संरचना ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, buffer_size, IFLA_IFNAME, ifname, strlen (ifname) + 1, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = send_request (sock, req, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0));
  सचे (UNLIKELY (len < 0))
    पटिनिवत्त crun_make_error (err, errno, "recv");

  अत्थाय (nlh = (संरचना nlmsghdr *) buffer; NLMSG_OK (nlh, (unsigned पूर्णाङ्क) len); nlh = NLMSG_NEXT (nlh, len))
    {
      सचे (nlh->nlmsg_seq != seq)
        अनुवर्त;

      सचे (nlh->nlmsg_type == NLMSG_ERROR)
        {
          संरचना nlmsgerr *err_data = (संरचना nlmsgerr *) NLMSG_DATA (nlh);
          सचे (err_data->error == 0)
            अनुवर्त;
          पटिनिवत्त crun_make_error (err, -err_data->error, "netlink error while looking for interface `%s`", ifname);
        }

      सचे (nlh->nlmsg_type == RTM_NEWLINK)
        {
          संरचना ifinfomsg *ifi = NLMSG_DATA (nlh);
          पटिनिवत्त ifi->ifi_index;
        }
    }

  पटिनिवत्त crun_make_error (err, 0, "could not find device `%s`", ifname);
}

स्थिर पूर्णाङ्क
wait_for_ack (पूर्णाङ्क sock, uint32_t seq, वर्ण *recv_buffer, size_t recv_buffer_size, libcrun_error_t *err)
{
  संरचना nlmsghdr *nlh_recv = (संरचना nlmsghdr *) recv_buffer;
  ssize_t len;

  करोहि
    {
      len = TEMP_FAILURE_RETRY (recv (sock, recv_buffer, recv_buffer_size, 0));
      सचे (len < 0)
        पटिनिवत्त crun_make_error (err, errno, "recv");
  } याव (nlh_recv->nlmsg_seq != seq);

  सचे (! NLMSG_OK (nlh_recv, len))
    पटिनिवत्त crun_make_error (err, 0, "received invalid packet");

  सचे (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      संरचना nlmsgerr *err_data = (संरचना nlmsgerr *) NLMSG_DATA (nlh_recv);
      सचे (err_data->error == 0)
        पटिनिवत्त 0;
      पटिनिवत्त crun_make_error (err, -err_data->error, "netlink error");
    }

  पटिनिवत्त crun_make_error (err, 0, "internal error: received unknown netlink packet type");
}

स्थिर शून्य
copy_ip_addr (संरचना nlmsghdr *nlh, संरचना ip_addr *ip)
{
  संरचना ifaddrmsg *ifa = (संरचना ifaddrmsg *) NLMSG_DATA (nlh);
  memcpy (&ip->ifa, ifa, परिमाण (संरचना ifaddrmsg));
  ip->rta_len = IFA_PAYLOAD (nlh);
  ip->rta = xmalloc (ip->rta_len);
  memcpy (ip->rta, IFA_RTA (ifa), ip->rta_len);
};

स्थिर पूर्णाङ्क
get_ip_addresses (पूर्णाङ्क sock, uint32_t ifindex, संरचना ip_addr **out_ips, वर्ण *buffer, size_t buffer_size, libcrun_error_t *err)
{
  संरचना nl_req *req = (संरचना nl_req *) buffer;
  cleanup_ip_addrs संरचना ip_addr *ips = रिक्त;
  size_t ips_len = 0;
  पूर्णाङ्क optval = 1;
  uint32_t seq;
  ssize_t len;
  पूर्णाङ्क ret;

#ifdef NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, परिमाण (optval));
  सचे (ret < 0)
    {
      सचे (errno != ENOPROTOOPT)
        पटिनिवत्त crun_make_error (err, errno, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#endif

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, परिमाण (संरचना ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_index = ifindex;

  ret = send_request (sock, req, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  याव ((len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0))) > 0)
    {
      संरचना nlmsghdr *nlh;

      अत्थाय (nlh = (संरचना nlmsghdr *) buffer; NLMSG_OK (nlh, len); nlh = NLMSG_NEXT (nlh, len))
        {
          संरचना ifaddrmsg *ifa;

          सचे (nlh->nlmsg_seq != seq)
            अनुवर्त;

          सचे (nlh->nlmsg_type == NLMSG_DONE)
            {
              *out_ips = ips;
              ips = रिक्त;
              पटिनिवत्त 0;
            }

          सचे (nlh->nlmsg_type == NLMSG_ERROR)
            {
              संरचना nlmsgerr *err_data = (संरचना nlmsgerr *) NLMSG_DATA (nlh);
              सचे (err_data->error == 0)
                अनुवर्त;
              पटिनिवत्त crun_make_error (err, -err_data->error, "netlink error reading ip addresses");
            }

          ifa = (संरचना ifaddrmsg *) NLMSG_DATA (nlh);
          सचे (ifa->ifa_index != ifindex)
            अनुवर्त;

          /* Copy only permanent, globally routable IP addresses.  */
          सचे (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            अनुवर्त;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, परिमाण (संरचना ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  सचे (UNLIKELY (len < 0))
    पटिनिवत्त crun_make_error (err, errno, "recv");
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
configure_ip_addresses (पूर्णाङ्क sock, पूर्णाङ्क ifindex, वर्ण *buffer, size_t buffer_size, ध्रुव संरचना ip_addr *ips, libcrun_error_t *err)
{
  संरचना nl_req *req = (संरचना nl_req *) buffer;
  ध्रुव संरचना ip_addr *ip;
  पूर्णाङ्क ret;

  सचे (ips == रिक्त)
    पटिनिवत्त 0;

  अत्थाय (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      पूर्णाङ्क rta_len = ip->rta_len;
      संरचना rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, परिमाण (संरचना ifaddrmsg));

      memcpy (&req->ifa, &ip->ifa, परिमाण (संरचना ifaddrmsg));

      req->ifa.ifa_index = ifindex;

      अत्थाय (rta = (संरचना rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_NEXT (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), buffer_size, rta->rta_type, RTA_DATA (rta), RTA_PAYLOAD (rta), err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }

      ret = send_request (sock, req, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = wait_for_ack (sock, seq, buffer, buffer_size, err);
      सचे (ret < 0)
        पटिनिवत्त ret;
    }

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
request_enable_interface_and_wait (पूर्णाङ्क sock, वर्ण *buffer, size_t buffer_size, पूर्णाङ्क index, libcrun_error_t *err)
{
  संरचना nl_req *req = (संरचना nl_req *) buffer;
  uint32_t seq;
  पूर्णाङ्क ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, परिमाण (संरचना ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = index;

  req->ifi.ifi_flags = IFF_UP;
  req->ifi.ifi_change = IFF_UP;

  ret = send_request (sock, req, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त wait_for_ack (sock, seq, buffer, buffer_size, err);
}

स्थिर पूर्णाङ्क
setup_network_device_in_ns_helper (वर्ण *buffer, size_t buffer_size, पूर्णाङ्क netns_fd, ध्रुव वर्ण *newifname,
                                   संरचना ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क sock_in_ns = -1;
  पूर्णाङ्क new_ifindex;
  पूर्णाङ्क ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "change network namespace");

  sock_in_ns = open_netlink_fd (err);
  सचे (sock_in_ns < 0)
    पटिनिवत्त sock_in_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_to_index (sock_in_ns, newifname, buffer, buffer_size, err);
  सचे (UNLIKELY (new_ifindex < 0))
    पटिनिवत्त new_ifindex;

  ret = configure_ip_addresses (sock_in_ns, new_ifindex, buffer, buffer_size, ips, err);
  सचे (ret < 0)
    पटिनिवत्त ret;

  पटिनिवत्त request_enable_interface_and_wait (sock_in_ns, buffer, buffer_size, new_ifindex, err);
}

स्थिर पूर्णाङ्क
do_move_link_to_ns_and_wait (पूर्णाङ्क sock, वर्ण *buffer, size_t buffer_size, पूर्णाङ्क ifindex, पूर्णाङ्क netns_fd, ध्रुव वर्ण *newifname, libcrun_error_t *err)
{
  संरचना nl_req *req = (संरचना nl_req *) buffer;
  uint32_t seq;
  पूर्णाङ्क ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, परिमाण (संरचना ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = ifindex;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_NET_NS_FD, &netns_fd, परिमाण (netns_fd), err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_IFNAME, newifname, strlen (newifname) + 1, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = send_request (sock, req, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त wait_for_ack (sock, seq, buffer, buffer_size, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
स्थिर पूर्णाङ्क
setup_network_device_in_ns (वर्ण *buffer, size_t buffer_size, पूर्णाङ्क netns_fd, ध्रुव वर्ण *newifname,
                            संरचना ip_addr *ips, libcrun_error_t *err)
{
  पूर्णाङ्क wait_status;
  pid_t pid;
  पूर्णाङ्क ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  सचे (UNLIKELY (pid < 0))
    पटिनिवत्त crun_make_error (err, errno, "vfork");

  सचे (pid == 0)
    {
      ret = setup_network_device_in_ns_helper (buffer, buffer_size, netns_fd, newifname, ips, err);
      सचे (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          _safe_exit (-ret);
        }

      _safe_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "waitpid for exec child pid");

  सचे (wait_status != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      पटिनिवत्त -get_process_exit_status (wait_status);
    }

  पटिनिवत्त 0;
}

पूर्णाङ्क
move_network_device (ध्रुव वर्ण *ifname, ध्रुव वर्ण *newifname, पूर्णाङ्क netns_fd, libcrun_error_t *err)
{
  ध्रुव size_t buffer_size = 8192;
  cleanup_ip_addrs संरचना ip_addr *ips = रिक्त;
  cleanup_free वर्ण *buffer = xmalloc (buffer_size);
  cleanup_close पूर्णाङ्क sock = -1;
  पूर्णाङ्क ifindex;
  पूर्णाङ्क ret;

  sock = open_netlink_fd (err);
  सचे (sock < 0)
    पटिनिवत्त sock;

  ifindex = name_to_index (sock, ifname, buffer, buffer_size, err);
  सचे (UNLIKELY (ifindex < 0))
    पटिनिवत्त ifindex;

  ret = get_ip_addresses (sock, ifindex, &ips, buffer, buffer_size, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_to_ns_and_wait (sock, buffer, buffer_size, ifindex, netns_fd, newifname, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त setup_network_device_in_ns (buffer, buffer_size, netns_fd, newifname, ips, err);
}
