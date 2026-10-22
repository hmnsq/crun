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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "net_device.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

#ಅನ್ತರ್ಭಾವ <sys/socket.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <sched.h>
#ಅನ್ತರ್ಭಾವ <sys/wait.h>
#ಅನ್ತರ್ಭಾವ <sys/xattr.h>
#ಅನ್ತರ್ಭಾವ <linux/netlink.h>
#ಅನ್ತರ್ಭಾವ <linux/rtnetlink.h>
#ಅನ್ತರ್ಭಾವ <arpa/inet.h>
#ಅನ್ತರ್ಭಾವ <net/if.h>
#ಯದಿ HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint ಚಞ್ಚಲ uint
#  endif
#endif

ಸಂರಚನಾ ip_addr
{
  ಸಂರಚನಾ ifaddrmsg ifa;

  ಪೂರ್ಣಾಙ್ಕ rta_len;
  ವರ್ಣ *rta;
};

ಸಂರಚನಾ nl_req
{
  ಸಂರಚನಾ nlmsghdr nlh;
  ಸಙ್ಘ
  {
    ಸಂರಚನಾ ifaddrmsg ifa;
    ಸಂರಚನಾ ifinfomsg ifi;
  };
};

ಸ್ಥಿರ atomic_uint nl_seq_counter;

ಸ್ಥಿರ uint32_t
get_next_seq ()
{
  ಪ್ರತ್ಯಾಗಮ (uint32_t) ++nl_seq_counter;
}

ಸ್ಥಿರ uint32_t
reset_request (ಸಂರಚನಾ nl_req *req, ಪೂರ್ಣಾಙ್ಕ type, ಪೂರ್ಣಾಙ್ಕ flags, ಪೂರ್ಣಾಙ್ಕ ಸಂದೇಶ_len)
{
  uint32_t seq = get_next_seq ();

  memset (req, 0, ಪರಿಮಾಣ (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_LENGTH (ಸಂದೇಶ_len);
  req->nlh.nlmsg_seq = seq;

  ಪ್ರತ್ಯಾಗಮ seq;
}

ಸ್ಥಿರ ಶೂನ್ಯ
cleanup_ip_addrsp (ಶೂನ್ಯ *p)
{
  ಸಂರಚನಾ ip_addr **pp = (ಸಂರಚನಾ ip_addr **) p;
  ಸಂರಚನಾ ip_addr *ip;
  ಯದಿ (*pp == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;

  ಪ್ರತಿ (ip = *pp; ip->rta_len >= 0; ip++)
    ಮುಕ್ತ (ip->rta);

  ಮುಕ್ತ (*pp);
}

#define cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ sock = -1;
  ಸಂರಚನಾ sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  ಪೂರ್ಣಾಙ್ಕ fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  ಯದಿ (sock < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "netlink socket");

  ಯದಿ (bind (sock, (ಸಂರಚನಾ sockaddr *) &local, ಪರಿಮಾಣ (local)) < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "bind");

  fd = sock;
  sock = -1;
  ಪ್ರತ್ಯಾಗಮ fd;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
append_rtattr (ಸಂರಚನಾ nlmsghdr *n, size_t maxlen, ಪೂರ್ಣಾಙ್ಕ type, ಧ್ರುವ ಶೂನ್ಯ *data, size_t data_len, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ len = RTA_LENGTH (data_len);
  ಸಂರಚನಾ rtattr *rta;

  ಯದಿ (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (ಸಂರಚನಾ rtattr *) (((ವರ್ಣ *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  ಯದಿ (data_len)
    memcpy (RTA_DATA (rta), data, data_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
send_request (ಪೂರ್ಣಾಙ್ಕ sock, ಸಂರಚನಾ nl_req *req, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = TEMP_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "send");

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* if_nametoindex with an open netlink socket.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
name_to_index (ಪೂರ್ಣಾಙ್ಕ sock, ಧ್ರುವ ವರ್ಣ *ifname, ವರ್ಣ *buffer, size_t buffer_size, libcrun_error_t *err)
{
  ಸಂರಚನಾ nlmsghdr *nlh;
  ಸಂರಚನಾ nl_req *req;
  uint32_t seq;
  ssize_t len;
  ಪೂರ್ಣಾಙ್ಕ ret;

  req = (ಸಂರಚನಾ nl_req *) buffer;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, ಪರಿಮಾಣ (ಸಂರಚನಾ ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, buffer_size, IFLA_IFNAME, ifname, strlen (ifname) + 1, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = send_request (sock, req, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0));
  ಯದಿ (UNLIKELY (len < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "recv");

  ಪ್ರತಿ (nlh = (ಸಂರಚನಾ nlmsghdr *) buffer; NLMSG_OK (nlh, (unsigned ಪೂರ್ಣಾಙ್ಕ) len); nlh = NLMSG_NEXT (nlh, len))
    {
      ಯದಿ (nlh->nlmsg_seq != seq)
        ಅನುವರ್ತ;

      ಯದಿ (nlh->nlmsg_type == NLMSG_ERROR)
        {
          ಸಂರಚನಾ nlmsgerr *err_data = (ಸಂರಚನಾ nlmsgerr *) NLMSG_DATA (nlh);
          ಯದಿ (err_data->error == 0)
            ಅನುವರ್ತ;
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -err_data->error, "netlink error while looking for interface `%s`", ifname);
        }

      ಯದಿ (nlh->nlmsg_type == RTM_NEWLINK)
        {
          ಸಂರಚನಾ ifinfomsg *ifi = NLMSG_DATA (nlh);
          ಪ್ರತ್ಯಾಗಮ ifi->ifi_index;
        }
    }

  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not find device `%s`", ifname);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
wait_for_ack (ಪೂರ್ಣಾಙ್ಕ sock, uint32_t seq, ವರ್ಣ *recv_buffer, size_t recv_buffer_size, libcrun_error_t *err)
{
  ಸಂರಚನಾ nlmsghdr *nlh_recv = (ಸಂರಚನಾ nlmsghdr *) recv_buffer;
  ssize_t len;

  ಕುರು
    {
      len = TEMP_FAILURE_RETRY (recv (sock, recv_buffer, recv_buffer_size, 0));
      ಯದಿ (len < 0)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "recv");
  } ಯಾವತ್ (nlh_recv->nlmsg_seq != seq);

  ಯದಿ (! NLMSG_OK (nlh_recv, len))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "received invalid packet");

  ಯದಿ (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      ಸಂರಚನಾ nlmsgerr *err_data = (ಸಂರಚನಾ nlmsgerr *) NLMSG_DATA (nlh_recv);
      ಯದಿ (err_data->error == 0)
        ಪ್ರತ್ಯಾಗಮ 0;
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -err_data->error, "netlink error");
    }

  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: received unknown netlink packet type");
}

ಸ್ಥಿರ ಶೂನ್ಯ
copy_ip_addr (ಸಂರಚನಾ nlmsghdr *nlh, ಸಂರಚನಾ ip_addr *ip)
{
  ಸಂರಚನಾ ifaddrmsg *ifa = (ಸಂರಚನಾ ifaddrmsg *) NLMSG_DATA (nlh);
  memcpy (&ip->ifa, ifa, ಪರಿಮಾಣ (ಸಂರಚನಾ ifaddrmsg));
  ip->rta_len = IFA_PAYLOAD (nlh);
  ip->rta = xmalloc (ip->rta_len);
  memcpy (ip->rta, IFA_RTA (ifa), ip->rta_len);
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_ip_addresses (ಪೂರ್ಣಾಙ್ಕ sock, uint32_t ifindex, ಸಂರಚನಾ ip_addr **out_ips, ವರ್ಣ *buffer, size_t buffer_size, libcrun_error_t *err)
{
  ಸಂರಚನಾ nl_req *req = (ಸಂರಚನಾ nl_req *) buffer;
  cleanup_ip_addrs ಸಂರಚನಾ ip_addr *ips = ರಿಕ್ತ;
  size_t ips_len = 0;
  ಪೂರ್ಣಾಙ್ಕ optval = 1;
  uint32_t seq;
  ssize_t len;
  ಪೂರ್ಣಾಙ್ಕ ret;

#ifdef NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, ಪರಿಮಾಣ (optval));
  ಯದಿ (ret < 0)
    {
      ಯದಿ (errno != ENOPROTOOPT)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#endif

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, ಪರಿಮಾಣ (ಸಂರಚನಾ ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_index = ifindex;

  ret = send_request (sock, req, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯಾವತ್ ((len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0))) > 0)
    {
      ಸಂರಚನಾ nlmsghdr *nlh;

      ಪ್ರತಿ (nlh = (ಸಂರಚನಾ nlmsghdr *) buffer; NLMSG_OK (nlh, len); nlh = NLMSG_NEXT (nlh, len))
        {
          ಸಂರಚನಾ ifaddrmsg *ifa;

          ಯದಿ (nlh->nlmsg_seq != seq)
            ಅನುವರ್ತ;

          ಯದಿ (nlh->nlmsg_type == NLMSG_DONE)
            {
              *out_ips = ips;
              ips = ರಿಕ್ತ;
              ಪ್ರತ್ಯಾಗಮ 0;
            }

          ಯದಿ (nlh->nlmsg_type == NLMSG_ERROR)
            {
              ಸಂರಚನಾ nlmsgerr *err_data = (ಸಂರಚನಾ nlmsgerr *) NLMSG_DATA (nlh);
              ಯದಿ (err_data->error == 0)
                ಅನುವರ್ತ;
              ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -err_data->error, "netlink error reading ip addresses");
            }

          ifa = (ಸಂರಚನಾ ifaddrmsg *) NLMSG_DATA (nlh);
          ಯದಿ (ifa->ifa_index != ifindex)
            ಅನುವರ್ತ;

          /* Copy only permanent, globally routable IP addresses.  */
          ಯದಿ (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            ಅನುವರ್ತ;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, ಪರಿಮಾಣ (ಸಂರಚನಾ ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  ಯದಿ (UNLIKELY (len < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "recv");
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
configure_ip_addresses (ಪೂರ್ಣಾಙ್ಕ sock, ಪೂರ್ಣಾಙ್ಕ ifindex, ವರ್ಣ *buffer, size_t buffer_size, ಧ್ರುವ ಸಂರಚನಾ ip_addr *ips, libcrun_error_t *err)
{
  ಸಂರಚನಾ nl_req *req = (ಸಂರಚನಾ nl_req *) buffer;
  ಧ್ರುವ ಸಂರಚನಾ ip_addr *ip;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (ips == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಪ್ರತಿ (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      ಪೂರ್ಣಾಙ್ಕ rta_len = ip->rta_len;
      ಸಂರಚನಾ rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, ಪರಿಮಾಣ (ಸಂರಚನಾ ifaddrmsg));

      memcpy (&req->ifa, &ip->ifa, ಪರಿಮಾಣ (ಸಂರಚನಾ ifaddrmsg));

      req->ifa.ifa_index = ifindex;

      ಪ್ರತಿ (rta = (ಸಂರಚನಾ rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_NEXT (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), buffer_size, rta->rta_type, RTA_DATA (rta), RTA_PAYLOAD (rta), err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }

      ret = send_request (sock, req, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = wait_for_ack (sock, seq, buffer, buffer_size, err);
      ಯದಿ (ret < 0)
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
request_enable_interface_and_wait (ಪೂರ್ಣಾಙ್ಕ sock, ವರ್ಣ *buffer, size_t buffer_size, ಪೂರ್ಣಾಙ್ಕ index, libcrun_error_t *err)
{
  ಸಂರಚನಾ nl_req *req = (ಸಂರಚನಾ nl_req *) buffer;
  uint32_t seq;
  ಪೂರ್ಣಾಙ್ಕ ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, ಪರಿಮಾಣ (ಸಂರಚನಾ ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = index;

  req->ifi.ifi_flags = IFF_UP;
  req->ifi.ifi_change = IFF_UP;

  ret = send_request (sock, req, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ wait_for_ack (sock, seq, buffer, buffer_size, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
setup_network_device_in_ns_helper (ವರ್ಣ *buffer, size_t buffer_size, ಪೂರ್ಣಾಙ್ಕ netns_fd, ಧ್ರುವ ವರ್ಣ *newifname,
                                   ಸಂರಚನಾ ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ sock_in_ns = -1;
  ಪೂರ್ಣಾಙ್ಕ new_ifindex;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "change network namespace");

  sock_in_ns = open_netlink_fd (err);
  ಯದಿ (sock_in_ns < 0)
    ಪ್ರತ್ಯಾಗಮ sock_in_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_to_index (sock_in_ns, newifname, buffer, buffer_size, err);
  ಯದಿ (UNLIKELY (new_ifindex < 0))
    ಪ್ರತ್ಯಾಗಮ new_ifindex;

  ret = configure_ip_addresses (sock_in_ns, new_ifindex, buffer, buffer_size, ips, err);
  ಯದಿ (ret < 0)
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ request_enable_interface_and_wait (sock_in_ns, buffer, buffer_size, new_ifindex, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_move_link_to_ns_and_wait (ಪೂರ್ಣಾಙ್ಕ sock, ವರ್ಣ *buffer, size_t buffer_size, ಪೂರ್ಣಾಙ್ಕ ifindex, ಪೂರ್ಣಾಙ್ಕ netns_fd, ಧ್ರುವ ವರ್ಣ *newifname, libcrun_error_t *err)
{
  ಸಂರಚನಾ nl_req *req = (ಸಂರಚನಾ nl_req *) buffer;
  uint32_t seq;
  ಪೂರ್ಣಾಙ್ಕ ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, ಪರಿಮಾಣ (ಸಂರಚನಾ ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = ifindex;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_NET_NS_FD, &netns_fd, ಪರಿಮಾಣ (netns_fd), err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_IFNAME, newifname, strlen (newifname) + 1, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = send_request (sock, req, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ wait_for_ack (sock, seq, buffer, buffer_size, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
setup_network_device_in_ns (ವರ್ಣ *buffer, size_t buffer_size, ಪೂರ್ಣಾಙ್ಕ netns_fd, ಧ್ರುವ ವರ್ಣ *newifname,
                            ಸಂರಚನಾ ip_addr *ips, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ wait_status;
  pid_t pid;
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  ಯದಿ (UNLIKELY (pid < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "vfork");

  ಯದಿ (pid == 0)
    {
      ret = setup_network_device_in_ns_helper (buffer, buffer_size, netns_fd, newifname, ips, err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          _safe_exit (-ret);
        }

      _safe_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "waitpid for exec child pid");

  ಯದಿ (wait_status != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      ಪ್ರತ್ಯಾಗಮ -get_process_exit_status (wait_status);
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
move_network_device (ಧ್ರುವ ವರ್ಣ *ifname, ಧ್ರುವ ವರ್ಣ *newifname, ಪೂರ್ಣಾಙ್ಕ netns_fd, libcrun_error_t *err)
{
  ಧ್ರುವ size_t buffer_size = 8192;
  cleanup_ip_addrs ಸಂರಚನಾ ip_addr *ips = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *buffer = xmalloc (buffer_size);
  cleanup_close ಪೂರ್ಣಾಙ್ಕ sock = -1;
  ಪೂರ್ಣಾಙ್ಕ ifindex;
  ಪೂರ್ಣಾಙ್ಕ ret;

  sock = open_netlink_fd (err);
  ಯದಿ (sock < 0)
    ಪ್ರತ್ಯಾಗಮ sock;

  ifindex = name_to_index (sock, ifname, buffer, buffer_size, err);
  ಯದಿ (UNLIKELY (ifindex < 0))
    ಪ್ರತ್ಯಾಗಮ ifindex;

  ret = get_ip_addresses (sock, ifindex, &ips, buffer, buffer_size, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_to_ns_and_wait (sock, buffer, buffer_size, ifindex, netns_fd, newifname, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ setup_network_device_in_ns (buffer, buffer_size, netns_fd, newifname, ips, err);
}
