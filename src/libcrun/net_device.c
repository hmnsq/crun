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
#nếu HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint volatile uint
#  endif
#endif

struct ip_addr
{
  struct ifaddrmsg ifa;

  số_nguyên rta_len;
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
  trả_về (uint32_t) ++nl_seq_counter;
}

static uint32_t
reset_request (struct nl_req *req, số_nguyên type, số_nguyên flags, số_nguyên msg_len)
{
  uint32_t seq = get_next_seq ();

  memset (req, 0, sizeof (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_LENGTH (msg_len);
  req->nlh.nlmsg_seq = seq;

  trả_về seq;
}

static void
cleanup_ip_addrsp (void *p)
{
  struct ip_addr **pp = (struct ip_addr **) p;
  struct ip_addr *ip;
  nếu (*pp == NULL)
    trả_về;

  cho (ip = *pp; ip->rta_len >= 0; ip++)
    free (ip->rta);

  free (*pp);
}

#define cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

static số_nguyên
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close số_nguyên sock = -1;
  struct sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  số_nguyên fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  nếu (sock < 0)
    trả_về crun_make_error (err, errno, "netlink socket");

  nếu (bind (sock, (struct sockaddr *) &local, sizeof (local)) < 0)
    trả_về crun_make_error (err, errno, "bind");

  fd = sock;
  sock = -1;
  trả_về fd;
}

static số_nguyên
append_rtattr (struct nlmsghdr *n, size_t maxlen, số_nguyên type, const void *data, size_t data_len, libcrun_error_t *err)
{
  số_nguyên len = RTA_LENGTH (data_len);
  struct rtattr *rta;

  nếu (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    trả_về crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (struct rtattr *) (((char *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  nếu (data_len)
    memcpy (RTA_DATA (rta), data, data_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  trả_về 0;
}

static số_nguyên
send_request (số_nguyên sock, struct nl_req *req, libcrun_error_t *err)
{
  số_nguyên ret;

  ret = TEMP_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "send");

  trả_về 0;
}

/* if_nametoindex with an open netlink socket.  */
static số_nguyên
name_to_index (số_nguyên sock, const char *ifname, char *buffer, size_t buffer_size, libcrun_error_t *err)
{
  struct nlmsghdr *nlh;
  struct nl_req *req;
  uint32_t seq;
  ssize_t len;
  số_nguyên ret;

  req = (struct nl_req *) buffer;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, sizeof (struct ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, buffer_size, IFLA_IFNAME, ifname, strlen (ifname) + 1, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  ret = send_request (sock, req, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0));
  nếu (UNLIKELY (len < 0))
    trả_về crun_make_error (err, errno, "recv");

  cho (nlh = (struct nlmsghdr *) buffer; NLMSG_OK (nlh, (unsigned số_nguyên) len); nlh = NLMSG_NEXT (nlh, len))
    {
      nếu (nlh->nlmsg_seq != seq)
        continue;

      nếu (nlh->nlmsg_type == NLMSG_ERROR)
        {
          struct nlmsgerr *err_data = (struct nlmsgerr *) NLMSG_DATA (nlh);
          nếu (err_data->error == 0)
            continue;
          trả_về crun_make_error (err, -err_data->error, "netlink error while looking for interface `%s`", ifname);
        }

      nếu (nlh->nlmsg_type == RTM_NEWLINK)
        {
          struct ifinfomsg *ifi = NLMSG_DATA (nlh);
          trả_về ifi->ifi_index;
        }
    }

  trả_về crun_make_error (err, 0, "could not find device `%s`", ifname);
}

static số_nguyên
wait_for_ack (số_nguyên sock, uint32_t seq, char *recv_buffer, size_t recv_buffer_size, libcrun_error_t *err)
{
  struct nlmsghdr *nlh_recv = (struct nlmsghdr *) recv_buffer;
  ssize_t len;

  do
    {
      len = TEMP_FAILURE_RETRY (recv (sock, recv_buffer, recv_buffer_size, 0));
      nếu (len < 0)
        trả_về crun_make_error (err, errno, "recv");
  } trong_khi (nlh_recv->nlmsg_seq != seq);

  nếu (! NLMSG_OK (nlh_recv, len))
    trả_về crun_make_error (err, 0, "received invalid packet");

  nếu (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      struct nlmsgerr *err_data = (struct nlmsgerr *) NLMSG_DATA (nlh_recv);
      nếu (err_data->error == 0)
        trả_về 0;
      trả_về crun_make_error (err, -err_data->error, "netlink error");
    }

  trả_về crun_make_error (err, 0, "internal error: received unknown netlink packet type");
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

static số_nguyên
get_ip_addresses (số_nguyên sock, uint32_t ifindex, struct ip_addr **out_ips, char *buffer, size_t buffer_size, libcrun_error_t *err)
{
  struct nl_req *req = (struct nl_req *) buffer;
  cleanup_ip_addrs struct ip_addr *ips = NULL;
  size_t ips_len = 0;
  số_nguyên optval = 1;
  uint32_t seq;
  ssize_t len;
  số_nguyên ret;

#ifdef NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, sizeof (optval));
  nếu (ret < 0)
    {
      nếu (errno != ENOPROTOOPT)
        trả_về crun_make_error (err, errno, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#endif

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, sizeof (struct ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_index = ifindex;

  ret = send_request (sock, req, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  trong_khi ((len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0))) > 0)
    {
      struct nlmsghdr *nlh;

      cho (nlh = (struct nlmsghdr *) buffer; NLMSG_OK (nlh, len); nlh = NLMSG_NEXT (nlh, len))
        {
          struct ifaddrmsg *ifa;

          nếu (nlh->nlmsg_seq != seq)
            continue;

          nếu (nlh->nlmsg_type == NLMSG_DONE)
            {
              *out_ips = ips;
              ips = NULL;
              trả_về 0;
            }

          nếu (nlh->nlmsg_type == NLMSG_ERROR)
            {
              struct nlmsgerr *err_data = (struct nlmsgerr *) NLMSG_DATA (nlh);
              nếu (err_data->error == 0)
                continue;
              trả_về crun_make_error (err, -err_data->error, "netlink error reading ip addresses");
            }

          ifa = (struct ifaddrmsg *) NLMSG_DATA (nlh);
          nếu (ifa->ifa_index != ifindex)
            continue;

          /* Copy only permanent, globally routable IP addresses.  */
          nếu (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            continue;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, sizeof (struct ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  nếu (UNLIKELY (len < 0))
    trả_về crun_make_error (err, errno, "recv");
  trả_về 0;
}

static số_nguyên
configure_ip_addresses (số_nguyên sock, số_nguyên ifindex, char *buffer, size_t buffer_size, const struct ip_addr *ips, libcrun_error_t *err)
{
  struct nl_req *req = (struct nl_req *) buffer;
  const struct ip_addr *ip;
  số_nguyên ret;

  nếu (ips == NULL)
    trả_về 0;

  cho (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      số_nguyên rta_len = ip->rta_len;
      struct rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, sizeof (struct ifaddrmsg));

      memcpy (&req->ifa, &ip->ifa, sizeof (struct ifaddrmsg));

      req->ifa.ifa_index = ifindex;

      cho (rta = (struct rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_NEXT (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), buffer_size, rta->rta_type, RTA_DATA (rta), RTA_PAYLOAD (rta), err);
          nếu (UNLIKELY (ret < 0))
            trả_về ret;
        }

      ret = send_request (sock, req, err);
      nếu (UNLIKELY (ret < 0))
        trả_về ret;

      ret = wait_for_ack (sock, seq, buffer, buffer_size, err);
      nếu (ret < 0)
        trả_về ret;
    }

  trả_về 0;
}

static số_nguyên
request_enable_interface_and_wait (số_nguyên sock, char *buffer, size_t buffer_size, số_nguyên index, libcrun_error_t *err)
{
  struct nl_req *req = (struct nl_req *) buffer;
  uint32_t seq;
  số_nguyên ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, sizeof (struct ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = index;

  req->ifi.ifi_flags = IFF_UP;
  req->ifi.ifi_change = IFF_UP;

  ret = send_request (sock, req, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  trả_về wait_for_ack (sock, seq, buffer, buffer_size, err);
}

static số_nguyên
setup_network_device_in_ns_helper (char *buffer, size_t buffer_size, số_nguyên netns_fd, const char *newifname,
                                   struct ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close số_nguyên sock_in_ns = -1;
  số_nguyên new_ifindex;
  số_nguyên ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "change network namespace");

  sock_in_ns = open_netlink_fd (err);
  nếu (sock_in_ns < 0)
    trả_về sock_in_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_to_index (sock_in_ns, newifname, buffer, buffer_size, err);
  nếu (UNLIKELY (new_ifindex < 0))
    trả_về new_ifindex;

  ret = configure_ip_addresses (sock_in_ns, new_ifindex, buffer, buffer_size, ips, err);
  nếu (ret < 0)
    trả_về ret;

  trả_về request_enable_interface_and_wait (sock_in_ns, buffer, buffer_size, new_ifindex, err);
}

static số_nguyên
do_move_link_to_ns_and_wait (số_nguyên sock, char *buffer, size_t buffer_size, số_nguyên ifindex, số_nguyên netns_fd, const char *newifname, libcrun_error_t *err)
{
  struct nl_req *req = (struct nl_req *) buffer;
  uint32_t seq;
  số_nguyên ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, sizeof (struct ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = ifindex;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_NET_NS_FD, &netns_fd, sizeof (netns_fd), err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_IFNAME, newifname, strlen (newifname) + 1, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  ret = send_request (sock, req, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  trả_về wait_for_ack (sock, seq, buffer, buffer_size, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
static số_nguyên
setup_network_device_in_ns (char *buffer, size_t buffer_size, số_nguyên netns_fd, const char *newifname,
                            struct ip_addr *ips, libcrun_error_t *err)
{
  số_nguyên wait_status;
  pid_t pid;
  số_nguyên ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  nếu (UNLIKELY (pid < 0))
    trả_về crun_make_error (err, errno, "vfork");

  nếu (pid == 0)
    {
      ret = setup_network_device_in_ns_helper (buffer, buffer_size, netns_fd, newifname, ips, err);
      nếu (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          _safe_exit (-ret);
        }

      _safe_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "waitpid for exec child pid");

  nếu (wait_status != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      trả_về -get_process_exit_status (wait_status);
    }

  trả_về 0;
}

số_nguyên
move_network_device (const char *ifname, const char *newifname, số_nguyên netns_fd, libcrun_error_t *err)
{
  const size_t buffer_size = 8192;
  cleanup_ip_addrs struct ip_addr *ips = NULL;
  cleanup_free char *buffer = xmalloc (buffer_size);
  cleanup_close số_nguyên sock = -1;
  số_nguyên ifindex;
  số_nguyên ret;

  sock = open_netlink_fd (err);
  nếu (sock < 0)
    trả_về sock;

  ifindex = name_to_index (sock, ifname, buffer, buffer_size, err);
  nếu (UNLIKELY (ifindex < 0))
    trả_về ifindex;

  ret = get_ip_addresses (sock, ifindex, &ips, buffer, buffer_size, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_to_ns_and_wait (sock, buffer, buffer_size, ifindex, netns_fd, newifname, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  trả_về setup_network_device_in_ns (buffer, buffer_size, netns_fd, newifname, ips, err);
}
