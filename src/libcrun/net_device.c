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

#include <config.h>
#include "net_device.h"
#include "utils.h"

#include <sys/socket.h>
#include <errno.h>
#include <sched.h>
#include <sys/wait.h>
#include <sys/xattr.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <arpa/inet.h>
#include <net/if.h>
#ถ้า HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint ca_ncala uint
#  endif
#endif

sa_nracanaa ip_addr
{
  sa_nracanaa ifaddrmsg ifa;

  จำนวนเต็ม rta_len;
  var_na *rta;
};

sa_nracanaa nl_req
{
  sa_nracanaa nlmsghdr nlh;
  sa_ngha
  {
    sa_nracanaa ifaddrmsg ifa;
    sa_nracanaa ifinfomsg ifi;
  };
};

static atomic_uint nl_seq_counter;

static uint32_t
get_next_seq ()
{
  คืน (uint32_t) ++nl_seq_counter;
}

static uint32_t
reset_request (sa_nracanaa nl_req *req, จำนวนเต็ม type, จำนวนเต็ม flags, จำนวนเต็ม msg_len)
{
  uint32_t seq = get_next_seq ();

  memset (req, 0, parimaa_na (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_LENGTH (msg_len);
  req->nlh.nlmsg_seq = seq;

  คืน seq;
}

static void
cleanup_ip_addrsp (void *p)
{
  sa_nracanaa ip_addr **pp = (sa_nracanaa ip_addr **) p;
  sa_nracanaa ip_addr *ip;
  ถ้า (*pp == NULL)
    คืน;

  สำหรับ (ip = *pp; ip->rta_len >= 0; ip++)
    free (ip->rta);

  free (*pp);
}

#define cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

static จำนวนเต็ม
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close จำนวนเต็ม sock = -1;
  sa_nracanaa sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  จำนวนเต็ม fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  ถ้า (sock < 0)
    คืน crun_make_error (err, errno, "netlink socket");

  ถ้า (bind (sock, (sa_nracanaa sockaddr *) &local, parimaa_na (local)) < 0)
    คืน crun_make_error (err, errno, "bind");

  fd = sock;
  sock = -1;
  คืน fd;
}

static จำนวนเต็ม
append_rtattr (sa_nracanaa nlmsghdr *n, size_t maxlen, จำนวนเต็ม type, const void *data, size_t data_len, libcrun_error_t *err)
{
  จำนวนเต็ม len = RTA_LENGTH (data_len);
  sa_nracanaa rtattr *rta;

  ถ้า (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    คืน crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (sa_nracanaa rtattr *) (((var_na *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  ถ้า (data_len)
    memcpy (RTA_DATA (rta), data, data_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  คืน 0;
}

static จำนวนเต็ม
send_request (จำนวนเต็ม sock, sa_nracanaa nl_req *req, libcrun_error_t *err)
{
  จำนวนเต็ม ret;

  ret = TEMP_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  ถ้า (UNLIKELY (ret < 0))
    คืน crun_make_error (err, errno, "send");

  คืน 0;
}

/* if_nametoindex with an open netlink socket.  */
static จำนวนเต็ม
name_to_index (จำนวนเต็ม sock, const var_na *ifname, var_na *buffer, size_t buffer_size, libcrun_error_t *err)
{
  sa_nracanaa nlmsghdr *nlh;
  sa_nracanaa nl_req *req;
  uint32_t seq;
  ssize_t len;
  จำนวนเต็ม ret;

  req = (sa_nracanaa nl_req *) buffer;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, parimaa_na (sa_nracanaa ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, buffer_size, IFLA_IFNAME, ifname, strlen (ifname) + 1, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  ret = send_request (sock, req, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0));
  ถ้า (UNLIKELY (len < 0))
    คืน crun_make_error (err, errno, "recv");

  สำหรับ (nlh = (sa_nracanaa nlmsghdr *) buffer; NLMSG_OK (nlh, (unsigned จำนวนเต็ม) len); nlh = NLMSG_NEXT (nlh, len))
    {
      ถ้า (nlh->nlmsg_seq != seq)
        continue;

      ถ้า (nlh->nlmsg_type == NLMSG_ERROR)
        {
          sa_nracanaa nlmsgerr *err_data = (sa_nracanaa nlmsgerr *) NLMSG_DATA (nlh);
          ถ้า (err_data->error == 0)
            continue;
          คืน crun_make_error (err, -err_data->error, "netlink error while looking for interface `%s`", ifname);
        }

      ถ้า (nlh->nlmsg_type == RTM_NEWLINK)
        {
          sa_nracanaa ifinfomsg *ifi = NLMSG_DATA (nlh);
          คืน ifi->ifi_index;
        }
    }

  คืน crun_make_error (err, 0, "could not find device `%s`", ifname);
}

static จำนวนเต็ม
wait_for_ack (จำนวนเต็ม sock, uint32_t seq, var_na *recv_buffer, size_t recv_buffer_size, libcrun_error_t *err)
{
  sa_nracanaa nlmsghdr *nlh_recv = (sa_nracanaa nlmsghdr *) recv_buffer;
  ssize_t len;

  do
    {
      len = TEMP_FAILURE_RETRY (recv (sock, recv_buffer, recv_buffer_size, 0));
      ถ้า (len < 0)
        คืน crun_make_error (err, errno, "recv");
  } ขณะ (nlh_recv->nlmsg_seq != seq);

  ถ้า (! NLMSG_OK (nlh_recv, len))
    คืน crun_make_error (err, 0, "received invalid packet");

  ถ้า (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      sa_nracanaa nlmsgerr *err_data = (sa_nracanaa nlmsgerr *) NLMSG_DATA (nlh_recv);
      ถ้า (err_data->error == 0)
        คืน 0;
      คืน crun_make_error (err, -err_data->error, "netlink error");
    }

  คืน crun_make_error (err, 0, "internal error: received unknown netlink packet type");
}

static void
copy_ip_addr (sa_nracanaa nlmsghdr *nlh, sa_nracanaa ip_addr *ip)
{
  sa_nracanaa ifaddrmsg *ifa = (sa_nracanaa ifaddrmsg *) NLMSG_DATA (nlh);
  memcpy (&ip->ifa, ifa, parimaa_na (sa_nracanaa ifaddrmsg));
  ip->rta_len = IFA_PAYLOAD (nlh);
  ip->rta = xmalloc (ip->rta_len);
  memcpy (ip->rta, IFA_RTA (ifa), ip->rta_len);
};

static จำนวนเต็ม
get_ip_addresses (จำนวนเต็ม sock, uint32_t ifindex, sa_nracanaa ip_addr **out_ips, var_na *buffer, size_t buffer_size, libcrun_error_t *err)
{
  sa_nracanaa nl_req *req = (sa_nracanaa nl_req *) buffer;
  cleanup_ip_addrs sa_nracanaa ip_addr *ips = NULL;
  size_t ips_len = 0;
  จำนวนเต็ม optval = 1;
  uint32_t seq;
  ssize_t len;
  จำนวนเต็ม ret;

#ifdef NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, parimaa_na (optval));
  ถ้า (ret < 0)
    {
      ถ้า (errno != ENOPROTOOPT)
        คืน crun_make_error (err, errno, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#endif

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, parimaa_na (sa_nracanaa ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_index = ifindex;

  ret = send_request (sock, req, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  ขณะ ((len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0))) > 0)
    {
      sa_nracanaa nlmsghdr *nlh;

      สำหรับ (nlh = (sa_nracanaa nlmsghdr *) buffer; NLMSG_OK (nlh, len); nlh = NLMSG_NEXT (nlh, len))
        {
          sa_nracanaa ifaddrmsg *ifa;

          ถ้า (nlh->nlmsg_seq != seq)
            continue;

          ถ้า (nlh->nlmsg_type == NLMSG_DONE)
            {
              *out_ips = ips;
              ips = NULL;
              คืน 0;
            }

          ถ้า (nlh->nlmsg_type == NLMSG_ERROR)
            {
              sa_nracanaa nlmsgerr *err_data = (sa_nracanaa nlmsgerr *) NLMSG_DATA (nlh);
              ถ้า (err_data->error == 0)
                continue;
              คืน crun_make_error (err, -err_data->error, "netlink error reading ip addresses");
            }

          ifa = (sa_nracanaa ifaddrmsg *) NLMSG_DATA (nlh);
          ถ้า (ifa->ifa_index != ifindex)
            continue;

          /* Copy only permanent, globally routable IP addresses.  */
          ถ้า (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            continue;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, parimaa_na (sa_nracanaa ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  ถ้า (UNLIKELY (len < 0))
    คืน crun_make_error (err, errno, "recv");
  คืน 0;
}

static จำนวนเต็ม
configure_ip_addresses (จำนวนเต็ม sock, จำนวนเต็ม ifindex, var_na *buffer, size_t buffer_size, const sa_nracanaa ip_addr *ips, libcrun_error_t *err)
{
  sa_nracanaa nl_req *req = (sa_nracanaa nl_req *) buffer;
  const sa_nracanaa ip_addr *ip;
  จำนวนเต็ม ret;

  ถ้า (ips == NULL)
    คืน 0;

  สำหรับ (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      จำนวนเต็ม rta_len = ip->rta_len;
      sa_nracanaa rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, parimaa_na (sa_nracanaa ifaddrmsg));

      memcpy (&req->ifa, &ip->ifa, parimaa_na (sa_nracanaa ifaddrmsg));

      req->ifa.ifa_index = ifindex;

      สำหรับ (rta = (sa_nracanaa rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_NEXT (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), buffer_size, rta->rta_type, RTA_DATA (rta), RTA_PAYLOAD (rta), err);
          ถ้า (UNLIKELY (ret < 0))
            คืน ret;
        }

      ret = send_request (sock, req, err);
      ถ้า (UNLIKELY (ret < 0))
        คืน ret;

      ret = wait_for_ack (sock, seq, buffer, buffer_size, err);
      ถ้า (ret < 0)
        คืน ret;
    }

  คืน 0;
}

static จำนวนเต็ม
request_enable_interface_and_wait (จำนวนเต็ม sock, var_na *buffer, size_t buffer_size, จำนวนเต็ม index, libcrun_error_t *err)
{
  sa_nracanaa nl_req *req = (sa_nracanaa nl_req *) buffer;
  uint32_t seq;
  จำนวนเต็ม ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, parimaa_na (sa_nracanaa ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = index;

  req->ifi.ifi_flags = IFF_UP;
  req->ifi.ifi_change = IFF_UP;

  ret = send_request (sock, req, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  คืน wait_for_ack (sock, seq, buffer, buffer_size, err);
}

static จำนวนเต็ม
setup_network_device_in_ns_helper (var_na *buffer, size_t buffer_size, จำนวนเต็ม netns_fd, const var_na *newifname,
                                   sa_nracanaa ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close จำนวนเต็ม sock_in_ns = -1;
  จำนวนเต็ม new_ifindex;
  จำนวนเต็ม ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  ถ้า (UNLIKELY (ret < 0))
    คืน crun_make_error (err, errno, "change network namespace");

  sock_in_ns = open_netlink_fd (err);
  ถ้า (sock_in_ns < 0)
    คืน sock_in_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_to_index (sock_in_ns, newifname, buffer, buffer_size, err);
  ถ้า (UNLIKELY (new_ifindex < 0))
    คืน new_ifindex;

  ret = configure_ip_addresses (sock_in_ns, new_ifindex, buffer, buffer_size, ips, err);
  ถ้า (ret < 0)
    คืน ret;

  คืน request_enable_interface_and_wait (sock_in_ns, buffer, buffer_size, new_ifindex, err);
}

static จำนวนเต็ม
do_move_link_to_ns_and_wait (จำนวนเต็ม sock, var_na *buffer, size_t buffer_size, จำนวนเต็ม ifindex, จำนวนเต็ม netns_fd, const var_na *newifname, libcrun_error_t *err)
{
  sa_nracanaa nl_req *req = (sa_nracanaa nl_req *) buffer;
  uint32_t seq;
  จำนวนเต็ม ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, parimaa_na (sa_nracanaa ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = ifindex;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_NET_NS_FD, &netns_fd, parimaa_na (netns_fd), err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_IFNAME, newifname, strlen (newifname) + 1, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  ret = send_request (sock, req, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  คืน wait_for_ack (sock, seq, buffer, buffer_size, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
static จำนวนเต็ม
setup_network_device_in_ns (var_na *buffer, size_t buffer_size, จำนวนเต็ม netns_fd, const var_na *newifname,
                            sa_nracanaa ip_addr *ips, libcrun_error_t *err)
{
  จำนวนเต็ม wait_status;
  pid_t pid;
  จำนวนเต็ม ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  ถ้า (UNLIKELY (pid < 0))
    คืน crun_make_error (err, errno, "vfork");

  ถ้า (pid == 0)
    {
      ret = setup_network_device_in_ns_helper (buffer, buffer_size, netns_fd, newifname, ips, err);
      ถ้า (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          _safe_exit (-ret);
        }

      _safe_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  ถ้า (UNLIKELY (ret < 0))
    คืน crun_make_error (err, errno, "waitpid for exec child pid");

  ถ้า (wait_status != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      คืน -get_process_exit_status (wait_status);
    }

  คืน 0;
}

จำนวนเต็ม
move_network_device (const var_na *ifname, const var_na *newifname, จำนวนเต็ม netns_fd, libcrun_error_t *err)
{
  const size_t buffer_size = 8192;
  cleanup_ip_addrs sa_nracanaa ip_addr *ips = NULL;
  cleanup_free var_na *buffer = xmalloc (buffer_size);
  cleanup_close จำนวนเต็ม sock = -1;
  จำนวนเต็ม ifindex;
  จำนวนเต็ม ret;

  sock = open_netlink_fd (err);
  ถ้า (sock < 0)
    คืน sock;

  ifindex = name_to_index (sock, ifname, buffer, buffer_size, err);
  ถ้า (UNLIKELY (ifindex < 0))
    คืน ifindex;

  ret = get_ip_addresses (sock, ifindex, &ips, buffer, buffer_size, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_to_ns_and_wait (sock, buffer, buffer_size, ifindex, netns_fd, newifname, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  คืน setup_network_device_in_ns (buffer, buffer_size, netns_fd, newifname, ips, err);
}
