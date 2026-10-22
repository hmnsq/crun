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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "net_device.h"
#അന്തര്ഭാവ "utils.h"

#അന്തര്ഭാവ <sys/socket.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <sched.h>
#അന്തര്ഭാവ <sys/wait.h>
#അന്തര്ഭാവ <sys/xattr.h>
#അന്തര്ഭാവ <linux/netlink.h>
#അന്തര്ഭാവ <linux/rtnetlink.h>
#അന്തര്ഭാവ <arpa/inet.h>
#അന്തര്ഭാവ <net/if.h>
#എങ്കിൽ HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint അസ്ഥിര uint
#  endif
#endif

ഘടന ip_addr
{
  ഘടന ifaddrmsg ifa;

  പൂർണ്ണസംഖ്യ rta_len;
  അക്ഷരം *rta;
};

ഘടന nl_req
{
  ഘടന nlmsghdr nlh;
  സംഘം
  {
    ഘടന ifaddrmsg ifa;
    ഘടന ifinfomsg ifi;
  };
};

സ്ഥിതം atomic_uint nl_seq_counter;

സ്ഥിതം uint32_t
get_next_seq ()
{
  തിരികെ (uint32_t) ++nl_seq_counter;
}

സ്ഥിതം uint32_t
reset_request (ഘടന nl_req *req, പൂർണ്ണസംഖ്യ type, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ സന്ദേശം_len)
{
  uint32_t seq = get_next_seq ();

  memset (req, 0, വലുപ്പം (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_LENGTH (സന്ദേശം_len);
  req->nlh.nlmsg_seq = seq;

  തിരികെ seq;
}

സ്ഥിതം ശൂന്യം
cleanup_ip_addrsp (ശൂന്യം *p)
{
  ഘടന ip_addr **pp = (ഘടന ip_addr **) p;
  ഘടന ip_addr *ip;
  എങ്കിൽ (*pp == ശൂന്യ)
    തിരികെ;

  വേണ്ടി (ip = *pp; ip->rta_len >= 0; ip++)
    സ്വതന്ത്രം (ip->rta);

  സ്വതന്ത്രം (*pp);
}

#define cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

സ്ഥിതം പൂർണ്ണസംഖ്യ
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ sock = -1;
  ഘടന sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  പൂർണ്ണസംഖ്യ fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  എങ്കിൽ (sock < 0)
    തിരികെ crun_make_error (err, errno, "netlink socket");

  എങ്കിൽ (bind (sock, (ഘടന sockaddr *) &local, വലുപ്പം (local)) < 0)
    തിരികെ crun_make_error (err, errno, "bind");

  fd = sock;
  sock = -1;
  തിരികെ fd;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
append_rtattr (ഘടന nlmsghdr *n, size_t maxlen, പൂർണ്ണസംഖ്യ type, സ്ഥിരം ശൂന്യം *data, size_t data_len, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ len = RTA_LENGTH (data_len);
  ഘടന rtattr *rta;

  എങ്കിൽ (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    തിരികെ crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (ഘടന rtattr *) (((അക്ഷരം *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  എങ്കിൽ (data_len)
    memcpy (RTA_DATA (rta), data, data_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
send_request (പൂർണ്ണസംഖ്യ sock, ഘടന nl_req *req, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  ret = TEMP_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "send");

  തിരികെ 0;
}

/* if_nametoindex with an open netlink socket.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
name_to_index (പൂർണ്ണസംഖ്യ sock, സ്ഥിരം അക്ഷരം *ifname, അക്ഷരം *buffer, size_t buffer_size, libcrun_error_t *err)
{
  ഘടന nlmsghdr *nlh;
  ഘടന nl_req *req;
  uint32_t seq;
  ssize_t len;
  പൂർണ്ണസംഖ്യ ret;

  req = (ഘടന nl_req *) buffer;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, വലുപ്പം (ഘടന ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, buffer_size, IFLA_IFNAME, ifname, strlen (ifname) + 1, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = send_request (sock, req, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0));
  എങ്കിൽ (UNLIKELY (len < 0))
    തിരികെ crun_make_error (err, errno, "recv");

  വേണ്ടി (nlh = (ഘടന nlmsghdr *) buffer; NLMSG_OK (nlh, (unsigned പൂർണ്ണസംഖ്യ) len); nlh = NLMSG_NEXT (nlh, len))
    {
      എങ്കിൽ (nlh->nlmsg_seq != seq)
        തുടരുക;

      എങ്കിൽ (nlh->nlmsg_type == NLMSG_ERROR)
        {
          ഘടന nlmsgerr *err_data = (ഘടന nlmsgerr *) NLMSG_DATA (nlh);
          എങ്കിൽ (err_data->error == 0)
            തുടരുക;
          തിരികെ crun_make_error (err, -err_data->error, "netlink error while looking for interface `%s`", ifname);
        }

      എങ്കിൽ (nlh->nlmsg_type == RTM_NEWLINK)
        {
          ഘടന ifinfomsg *ifi = NLMSG_DATA (nlh);
          തിരികെ ifi->ifi_index;
        }
    }

  തിരികെ crun_make_error (err, 0, "could not find device `%s`", ifname);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
wait_for_ack (പൂർണ്ണസംഖ്യ sock, uint32_t seq, അക്ഷരം *recv_buffer, size_t recv_buffer_size, libcrun_error_t *err)
{
  ഘടന nlmsghdr *nlh_recv = (ഘടന nlmsghdr *) recv_buffer;
  ssize_t len;

  ചെയ്യുക
    {
      len = TEMP_FAILURE_RETRY (recv (sock, recv_buffer, recv_buffer_size, 0));
      എങ്കിൽ (len < 0)
        തിരികെ crun_make_error (err, errno, "recv");
  } ആകുമ്പോൾ (nlh_recv->nlmsg_seq != seq);

  എങ്കിൽ (! NLMSG_OK (nlh_recv, len))
    തിരികെ crun_make_error (err, 0, "received invalid packet");

  എങ്കിൽ (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      ഘടന nlmsgerr *err_data = (ഘടന nlmsgerr *) NLMSG_DATA (nlh_recv);
      എങ്കിൽ (err_data->error == 0)
        തിരികെ 0;
      തിരികെ crun_make_error (err, -err_data->error, "netlink error");
    }

  തിരികെ crun_make_error (err, 0, "internal error: received unknown netlink packet type");
}

സ്ഥിതം ശൂന്യം
copy_ip_addr (ഘടന nlmsghdr *nlh, ഘടന ip_addr *ip)
{
  ഘടന ifaddrmsg *ifa = (ഘടന ifaddrmsg *) NLMSG_DATA (nlh);
  memcpy (&ip->ifa, ifa, വലുപ്പം (ഘടന ifaddrmsg));
  ip->rta_len = IFA_PAYLOAD (nlh);
  ip->rta = xmalloc (ip->rta_len);
  memcpy (ip->rta, IFA_RTA (ifa), ip->rta_len);
};

സ്ഥിതം പൂർണ്ണസംഖ്യ
get_ip_addresses (പൂർണ്ണസംഖ്യ sock, uint32_t ifindex, ഘടന ip_addr **out_ips, അക്ഷരം *buffer, size_t buffer_size, libcrun_error_t *err)
{
  ഘടന nl_req *req = (ഘടന nl_req *) buffer;
  cleanup_ip_addrs ഘടന ip_addr *ips = ശൂന്യ;
  size_t ips_len = 0;
  പൂർണ്ണസംഖ്യ optval = 1;
  uint32_t seq;
  ssize_t len;
  പൂർണ്ണസംഖ്യ ret;

#ifdef NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, വലുപ്പം (optval));
  എങ്കിൽ (ret < 0)
    {
      എങ്കിൽ (errno != ENOPROTOOPT)
        തിരികെ crun_make_error (err, errno, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#endif

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, വലുപ്പം (ഘടന ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_index = ifindex;

  ret = send_request (sock, req, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ആകുമ്പോൾ ((len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0))) > 0)
    {
      ഘടന nlmsghdr *nlh;

      വേണ്ടി (nlh = (ഘടന nlmsghdr *) buffer; NLMSG_OK (nlh, len); nlh = NLMSG_NEXT (nlh, len))
        {
          ഘടന ifaddrmsg *ifa;

          എങ്കിൽ (nlh->nlmsg_seq != seq)
            തുടരുക;

          എങ്കിൽ (nlh->nlmsg_type == NLMSG_DONE)
            {
              *out_ips = ips;
              ips = ശൂന്യ;
              തിരികെ 0;
            }

          എങ്കിൽ (nlh->nlmsg_type == NLMSG_ERROR)
            {
              ഘടന nlmsgerr *err_data = (ഘടന nlmsgerr *) NLMSG_DATA (nlh);
              എങ്കിൽ (err_data->error == 0)
                തുടരുക;
              തിരികെ crun_make_error (err, -err_data->error, "netlink error reading ip addresses");
            }

          ifa = (ഘടന ifaddrmsg *) NLMSG_DATA (nlh);
          എങ്കിൽ (ifa->ifa_index != ifindex)
            തുടരുക;

          /* Copy only permanent, globally routable IP addresses.  */
          എങ്കിൽ (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            തുടരുക;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, വലുപ്പം (ഘടന ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  എങ്കിൽ (UNLIKELY (len < 0))
    തിരികെ crun_make_error (err, errno, "recv");
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
configure_ip_addresses (പൂർണ്ണസംഖ്യ sock, പൂർണ്ണസംഖ്യ ifindex, അക്ഷരം *buffer, size_t buffer_size, സ്ഥിരം ഘടന ip_addr *ips, libcrun_error_t *err)
{
  ഘടന nl_req *req = (ഘടന nl_req *) buffer;
  സ്ഥിരം ഘടന ip_addr *ip;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (ips == ശൂന്യ)
    തിരികെ 0;

  വേണ്ടി (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      പൂർണ്ണസംഖ്യ rta_len = ip->rta_len;
      ഘടന rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, വലുപ്പം (ഘടന ifaddrmsg));

      memcpy (&req->ifa, &ip->ifa, വലുപ്പം (ഘടന ifaddrmsg));

      req->ifa.ifa_index = ifindex;

      വേണ്ടി (rta = (ഘടന rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_NEXT (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), buffer_size, rta->rta_type, RTA_DATA (rta), RTA_PAYLOAD (rta), err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }

      ret = send_request (sock, req, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = wait_for_ack (sock, seq, buffer, buffer_size, err);
      എങ്കിൽ (ret < 0)
        തിരികെ ret;
    }

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
request_enable_interface_and_wait (പൂർണ്ണസംഖ്യ sock, അക്ഷരം *buffer, size_t buffer_size, പൂർണ്ണസംഖ്യ index, libcrun_error_t *err)
{
  ഘടന nl_req *req = (ഘടന nl_req *) buffer;
  uint32_t seq;
  പൂർണ്ണസംഖ്യ ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, വലുപ്പം (ഘടന ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = index;

  req->ifi.ifi_flags = IFF_UP;
  req->ifi.ifi_change = IFF_UP;

  ret = send_request (sock, req, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ wait_for_ack (sock, seq, buffer, buffer_size, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
setup_network_device_in_ns_helper (അക്ഷരം *buffer, size_t buffer_size, പൂർണ്ണസംഖ്യ netns_fd, സ്ഥിരം അക്ഷരം *newifname,
                                   ഘടന ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ sock_in_ns = -1;
  പൂർണ്ണസംഖ്യ new_ifindex;
  പൂർണ്ണസംഖ്യ ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "change network namespace");

  sock_in_ns = open_netlink_fd (err);
  എങ്കിൽ (sock_in_ns < 0)
    തിരികെ sock_in_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_to_index (sock_in_ns, newifname, buffer, buffer_size, err);
  എങ്കിൽ (UNLIKELY (new_ifindex < 0))
    തിരികെ new_ifindex;

  ret = configure_ip_addresses (sock_in_ns, new_ifindex, buffer, buffer_size, ips, err);
  എങ്കിൽ (ret < 0)
    തിരികെ ret;

  തിരികെ request_enable_interface_and_wait (sock_in_ns, buffer, buffer_size, new_ifindex, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
do_move_link_to_ns_and_wait (പൂർണ്ണസംഖ്യ sock, അക്ഷരം *buffer, size_t buffer_size, പൂർണ്ണസംഖ്യ ifindex, പൂർണ്ണസംഖ്യ netns_fd, സ്ഥിരം അക്ഷരം *newifname, libcrun_error_t *err)
{
  ഘടന nl_req *req = (ഘടന nl_req *) buffer;
  uint32_t seq;
  പൂർണ്ണസംഖ്യ ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, വലുപ്പം (ഘടന ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = ifindex;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_NET_NS_FD, &netns_fd, വലുപ്പം (netns_fd), err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_IFNAME, newifname, strlen (newifname) + 1, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = send_request (sock, req, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ wait_for_ack (sock, seq, buffer, buffer_size, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
setup_network_device_in_ns (അക്ഷരം *buffer, size_t buffer_size, പൂർണ്ണസംഖ്യ netns_fd, സ്ഥിരം അക്ഷരം *newifname,
                            ഘടന ip_addr *ips, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ wait_status;
  pid_t pid;
  പൂർണ്ണസംഖ്യ ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  എങ്കിൽ (UNLIKELY (pid < 0))
    തിരികെ crun_make_error (err, errno, "vfork");

  എങ്കിൽ (pid == 0)
    {
      ret = setup_network_device_in_ns_helper (buffer, buffer_size, netns_fd, newifname, ips, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          _safe_exit (-ret);
        }

      _safe_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "waitpid for exec child pid");

  എങ്കിൽ (wait_status != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      തിരികെ -get_process_exit_status (wait_status);
    }

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
move_network_device (സ്ഥിരം അക്ഷരം *ifname, സ്ഥിരം അക്ഷരം *newifname, പൂർണ്ണസംഖ്യ netns_fd, libcrun_error_t *err)
{
  സ്ഥിരം size_t buffer_size = 8192;
  cleanup_ip_addrs ഘടന ip_addr *ips = ശൂന്യ;
  cleanup_free അക്ഷരം *buffer = xmalloc (buffer_size);
  cleanup_close പൂർണ്ണസംഖ്യ sock = -1;
  പൂർണ്ണസംഖ്യ ifindex;
  പൂർണ്ണസംഖ്യ ret;

  sock = open_netlink_fd (err);
  എങ്കിൽ (sock < 0)
    തിരികെ sock;

  ifindex = name_to_index (sock, ifname, buffer, buffer_size, err);
  എങ്കിൽ (UNLIKELY (ifindex < 0))
    തിരികെ ifindex;

  ret = get_ip_addresses (sock, ifindex, &ips, buffer, buffer_size, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_to_ns_and_wait (sock, buffer, buffer_size, ifindex, netns_fd, newifname, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ setup_network_device_in_ns (buffer, buffer_size, netns_fd, newifname, ips, err);
}
