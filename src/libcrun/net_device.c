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
#якщо HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint volatile uint
#  endif
#endif

struct ip_addr
{
  struct ifaddrmsg ifa;

  ціле rta_len;
  char *rta;
};

struct nl_req
{
  struct nlmsghdr nlh;
  union
  {
    struct ifaddrmsg ifa;
    struct ifinfomsg ifi;
  };
};

static atomic_uint nl_seq_counter;

static uint32_t
get_next_seq ()
{
  повернути (uint32_t) ++nl_seq_counter;
}

static uint32_t
reset_request (struct nl_req *req, ціле type, ціле flags, ціле msg_len)
{
  uint32_t seq = get_next_seq ();

  memset (req, 0, sizeof (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_LENGTH (msg_len);
  req->nlh.nlmsg_seq = seq;

  повернути seq;
}

static void
cleanup_ip_addrsp (void *p)
{
  struct ip_addr **pp = (struct ip_addr **) p;
  struct ip_addr *ip;
  якщо (*pp == NULL)
    повернути;

  для (ip = *pp; ip->rta_len >= 0; ip++)
    free (ip->rta);

  free (*pp);
}

#define cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

static ціле
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close ціле sock = -1;
  struct sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  ціле fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  якщо (sock < 0)
    повернути crun_make_error (err, errno, "netlink socket");

  якщо (bind (sock, (struct sockaddr *) &local, sizeof (local)) < 0)
    повернути crun_make_error (err, errno, "bind");

  fd = sock;
  sock = -1;
  повернути fd;
}

static ціле
append_rtattr (struct nlmsghdr *n, size_t maxlen, ціле type, const void *data, size_t data_len, libcrun_error_t *err)
{
  ціле len = RTA_LENGTH (data_len);
  struct rtattr *rta;

  якщо (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    повернути crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (struct rtattr *) (((char *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  якщо (data_len)
    memcpy (RTA_DATA (rta), data, data_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  повернути 0;
}

static ціле
send_request (ціле sock, struct nl_req *req, libcrun_error_t *err)
{
  ціле ret;

  ret = TEMP_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "send");

  повернути 0;
}

/* if_nametoindex with an open netlink socket.  */
static ціле
name_to_index (ціле sock, const char *ifname, char *buffer, size_t buffer_size, libcrun_error_t *err)
{
  struct nlmsghdr *nlh;
  struct nl_req *req;
  uint32_t seq;
  ssize_t len;
  ціле ret;

  req = (struct nl_req *) buffer;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, sizeof (struct ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, buffer_size, IFLA_IFNAME, ifname, strlen (ifname) + 1, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = send_request (sock, req, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0));
  якщо (UNLIKELY (len < 0))
    повернути crun_make_error (err, errno, "recv");

  для (nlh = (struct nlmsghdr *) buffer; NLMSG_OK (nlh, (unsigned ціле) len); nlh = NLMSG_NEXT (nlh, len))
    {
      якщо (nlh->nlmsg_seq != seq)
        continue;

      якщо (nlh->nlmsg_type == NLMSG_ERROR)
        {
          struct nlmsgerr *err_data = (struct nlmsgerr *) NLMSG_DATA (nlh);
          якщо (err_data->error == 0)
            continue;
          повернути crun_make_error (err, -err_data->error, "netlink error while looking for interface `%s`", ifname);
        }

      якщо (nlh->nlmsg_type == RTM_NEWLINK)
        {
          struct ifinfomsg *ifi = NLMSG_DATA (nlh);
          повернути ifi->ifi_index;
        }
    }

  повернути crun_make_error (err, 0, "could not find device `%s`", ifname);
}

static ціле
wait_for_ack (ціле sock, uint32_t seq, char *recv_buffer, size_t recv_buffer_size, libcrun_error_t *err)
{
  struct nlmsghdr *nlh_recv = (struct nlmsghdr *) recv_buffer;
  ssize_t len;

  do
    {
      len = TEMP_FAILURE_RETRY (recv (sock, recv_buffer, recv_buffer_size, 0));
      якщо (len < 0)
        повернути crun_make_error (err, errno, "recv");
  } поки (nlh_recv->nlmsg_seq != seq);

  якщо (! NLMSG_OK (nlh_recv, len))
    повернути crun_make_error (err, 0, "received invalid packet");

  якщо (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      struct nlmsgerr *err_data = (struct nlmsgerr *) NLMSG_DATA (nlh_recv);
      якщо (err_data->error == 0)
        повернути 0;
      повернути crun_make_error (err, -err_data->error, "netlink error");
    }

  повернути crun_make_error (err, 0, "internal error: received unknown netlink packet type");
}

static void
copy_ip_addr (struct nlmsghdr *nlh, struct ip_addr *ip)
{
  struct ifaddrmsg *ifa = (struct ifaddrmsg *) NLMSG_DATA (nlh);
  memcpy (&ip->ifa, ifa, sizeof (struct ifaddrmsg));
  ip->rta_len = IFA_PAYLOAD (nlh);
  ip->rta = xmalloc (ip->rta_len);
  memcpy (ip->rta, IFA_RTA (ifa), ip->rta_len);
};

static ціле
get_ip_addresses (ціле sock, uint32_t ifindex, struct ip_addr **out_ips, char *buffer, size_t buffer_size, libcrun_error_t *err)
{
  struct nl_req *req = (struct nl_req *) buffer;
  cleanup_ip_addrs struct ip_addr *ips = NULL;
  size_t ips_len = 0;
  ціле optval = 1;
  uint32_t seq;
  ssize_t len;
  ціле ret;

#ifdef NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, sizeof (optval));
  якщо (ret < 0)
    {
      якщо (errno != ENOPROTOOPT)
        повернути crun_make_error (err, errno, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#endif

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, sizeof (struct ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_index = ifindex;

  ret = send_request (sock, req, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  поки ((len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0))) > 0)
    {
      struct nlmsghdr *nlh;

      для (nlh = (struct nlmsghdr *) buffer; NLMSG_OK (nlh, len); nlh = NLMSG_NEXT (nlh, len))
        {
          struct ifaddrmsg *ifa;

          якщо (nlh->nlmsg_seq != seq)
            continue;

          якщо (nlh->nlmsg_type == NLMSG_DONE)
            {
              *out_ips = ips;
              ips = NULL;
              повернути 0;
            }

          якщо (nlh->nlmsg_type == NLMSG_ERROR)
            {
              struct nlmsgerr *err_data = (struct nlmsgerr *) NLMSG_DATA (nlh);
              якщо (err_data->error == 0)
                continue;
              повернути crun_make_error (err, -err_data->error, "netlink error reading ip addresses");
            }

          ifa = (struct ifaddrmsg *) NLMSG_DATA (nlh);
          якщо (ifa->ifa_index != ifindex)
            continue;

          /* Copy only permanent, globally routable IP addresses.  */
          якщо (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            continue;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, sizeof (struct ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  якщо (UNLIKELY (len < 0))
    повернути crun_make_error (err, errno, "recv");
  повернути 0;
}

static ціле
configure_ip_addresses (ціле sock, ціле ifindex, char *buffer, size_t buffer_size, const struct ip_addr *ips, libcrun_error_t *err)
{
  struct nl_req *req = (struct nl_req *) buffer;
  const struct ip_addr *ip;
  ціле ret;

  якщо (ips == NULL)
    повернути 0;

  для (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      ціле rta_len = ip->rta_len;
      struct rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, sizeof (struct ifaddrmsg));

      memcpy (&req->ifa, &ip->ifa, sizeof (struct ifaddrmsg));

      req->ifa.ifa_index = ifindex;

      для (rta = (struct rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_NEXT (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), buffer_size, rta->rta_type, RTA_DATA (rta), RTA_PAYLOAD (rta), err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
        }

      ret = send_request (sock, req, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      ret = wait_for_ack (sock, seq, buffer, buffer_size, err);
      якщо (ret < 0)
        повернути ret;
    }

  повернути 0;
}

static ціле
request_enable_interface_and_wait (ціле sock, char *buffer, size_t buffer_size, ціле index, libcrun_error_t *err)
{
  struct nl_req *req = (struct nl_req *) buffer;
  uint32_t seq;
  ціле ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, sizeof (struct ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = index;

  req->ifi.ifi_flags = IFF_UP;
  req->ifi.ifi_change = IFF_UP;

  ret = send_request (sock, req, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  повернути wait_for_ack (sock, seq, buffer, buffer_size, err);
}

static ціле
setup_network_device_in_ns_helper (char *buffer, size_t buffer_size, ціле netns_fd, const char *newifname,
                                   struct ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close ціле sock_in_ns = -1;
  ціле new_ifindex;
  ціле ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "change network namespace");

  sock_in_ns = open_netlink_fd (err);
  якщо (sock_in_ns < 0)
    повернути sock_in_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_to_index (sock_in_ns, newifname, buffer, buffer_size, err);
  якщо (UNLIKELY (new_ifindex < 0))
    повернути new_ifindex;

  ret = configure_ip_addresses (sock_in_ns, new_ifindex, buffer, buffer_size, ips, err);
  якщо (ret < 0)
    повернути ret;

  повернути request_enable_interface_and_wait (sock_in_ns, buffer, buffer_size, new_ifindex, err);
}

static ціле
do_move_link_to_ns_and_wait (ціле sock, char *buffer, size_t buffer_size, ціле ifindex, ціле netns_fd, const char *newifname, libcrun_error_t *err)
{
  struct nl_req *req = (struct nl_req *) buffer;
  uint32_t seq;
  ціле ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, sizeof (struct ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = ifindex;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_NET_NS_FD, &netns_fd, sizeof (netns_fd), err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_IFNAME, newifname, strlen (newifname) + 1, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = send_request (sock, req, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  повернути wait_for_ack (sock, seq, buffer, buffer_size, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
static ціле
setup_network_device_in_ns (char *buffer, size_t buffer_size, ціле netns_fd, const char *newifname,
                            struct ip_addr *ips, libcrun_error_t *err)
{
  ціле wait_status;
  pid_t pid;
  ціле ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  якщо (UNLIKELY (pid < 0))
    повернути crun_make_error (err, errno, "vfork");

  якщо (pid == 0)
    {
      ret = setup_network_device_in_ns_helper (buffer, buffer_size, netns_fd, newifname, ips, err);
      якщо (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          _safe_exit (-ret);
        }

      _safe_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "waitpid for exec child pid");

  якщо (wait_status != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      повернути -get_process_exit_status (wait_status);
    }

  повернути 0;
}

ціле
move_network_device (const char *ifname, const char *newifname, ціле netns_fd, libcrun_error_t *err)
{
  const size_t buffer_size = 8192;
  cleanup_ip_addrs struct ip_addr *ips = NULL;
  cleanup_free char *buffer = xmalloc (buffer_size);
  cleanup_close ціле sock = -1;
  ціле ifindex;
  ціле ret;

  sock = open_netlink_fd (err);
  якщо (sock < 0)
    повернути sock;

  ifindex = name_to_index (sock, ifname, buffer, buffer_size, err);
  якщо (UNLIKELY (ifindex < 0))
    повернути ifindex;

  ret = get_ip_addresses (sock, ifindex, &ips, buffer, buffer_size, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_to_ns_and_wait (sock, buffer, buffer_size, ifindex, netns_fd, newifname, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  повернути setup_network_device_in_ns (buffer, buffer_size, netns_fd, newifname, ips, err);
}
