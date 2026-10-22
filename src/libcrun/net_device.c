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
#ᯀᯮᯝ HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint ca_ncala uint
#  endif
#endif

sa_nracanaa ip_addr
{
  sa_nracanaa ifaddrmsg ifa;

  puur_naa_nka rta_len;
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
  ᯀᯮᯞ (uint32_t) ++nl_seq_counter;
}

static uint32_t
reset_request (sa_nracanaa nl_req *req, puur_naa_nka type, puur_naa_nka flags, puur_naa_nka msg_len)
{
  uint32_t seq = get_next_seq ();

  memset (req, 0, parimaa_na (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_LENGTH (msg_len);
  req->nlh.nlmsg_seq = seq;

  ᯀᯮᯞ seq;
}

static void
cleanup_ip_addrsp (void *p)
{
  sa_nracanaa ip_addr **pp = (sa_nracanaa ip_addr **) p;
  sa_nracanaa ip_addr *ip;
  ᯀᯮᯝ (*pp == NULL)
    ᯀᯮᯞ;

  for (ip = *pp; ip->rta_len >= 0; ip++)
    free (ip->rta);

  free (*pp);
}

#define cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

static puur_naa_nka
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close puur_naa_nka sock = -1;
  sa_nracanaa sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  puur_naa_nka fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  ᯀᯮᯝ (sock < 0)
    ᯀᯮᯞ crun_make_error (err, errno, "netlink socket");

  ᯀᯮᯝ (bind (sock, (sa_nracanaa sockaddr *) &local, parimaa_na (local)) < 0)
    ᯀᯮᯞ crun_make_error (err, errno, "bind");

  fd = sock;
  sock = -1;
  ᯀᯮᯞ fd;
}

static puur_naa_nka
append_rtattr (sa_nracanaa nlmsghdr *n, size_t maxlen, puur_naa_nka type, const void *data, size_t data_len, libcrun_error_t *err)
{
  puur_naa_nka len = RTA_LENGTH (data_len);
  sa_nracanaa rtattr *rta;

  ᯀᯮᯝ (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    ᯀᯮᯞ crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (sa_nracanaa rtattr *) (((var_na *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  ᯀᯮᯝ (data_len)
    memcpy (RTA_DATA (rta), data, data_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  ᯀᯮᯞ 0;
}

static puur_naa_nka
send_request (puur_naa_nka sock, sa_nracanaa nl_req *req, libcrun_error_t *err)
{
  puur_naa_nka ret;

  ret = TEMP_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  ᯀᯮᯝ (UNLIKELY (ret < 0))
    ᯀᯮᯞ crun_make_error (err, errno, "send");

  ᯀᯮᯞ 0;
}

/* if_nametoindex with an open netlink socket.  */
static puur_naa_nka
name_to_index (puur_naa_nka sock, const var_na *ifname, var_na *buffer, size_t buffer_size, libcrun_error_t *err)
{
  sa_nracanaa nlmsghdr *nlh;
  sa_nracanaa nl_req *req;
  uint32_t seq;
  ssize_t len;
  puur_naa_nka ret;

  req = (sa_nracanaa nl_req *) buffer;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, parimaa_na (sa_nracanaa ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, buffer_size, IFLA_IFNAME, ifname, strlen (ifname) + 1, err);
  ᯀᯮᯝ (UNLIKELY (ret < 0))
    ᯀᯮᯞ ret;

  ret = send_request (sock, req, err);
  ᯀᯮᯝ (UNLIKELY (ret < 0))
    ᯀᯮᯞ ret;

  len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0));
  ᯀᯮᯝ (UNLIKELY (len < 0))
    ᯀᯮᯞ crun_make_error (err, errno, "recv");

  for (nlh = (sa_nracanaa nlmsghdr *) buffer; NLMSG_OK (nlh, (unsigned puur_naa_nka) len); nlh = NLMSG_NEXT (nlh, len))
    {
      ᯀᯮᯝ (nlh->nlmsg_seq != seq)
        continue;

      ᯀᯮᯝ (nlh->nlmsg_type == NLMSG_ERROR)
        {
          sa_nracanaa nlmsgerr *err_data = (sa_nracanaa nlmsgerr *) NLMSG_DATA (nlh);
          ᯀᯮᯝ (err_data->error == 0)
            continue;
          ᯀᯮᯞ crun_make_error (err, -err_data->error, "netlink error while looking for interface `%s`", ifname);
        }

      ᯀᯮᯝ (nlh->nlmsg_type == RTM_NEWLINK)
        {
          sa_nracanaa ifinfomsg *ifi = NLMSG_DATA (nlh);
          ᯀᯮᯞ ifi->ifi_index;
        }
    }

  ᯀᯮᯞ crun_make_error (err, 0, "could not find device `%s`", ifname);
}

static puur_naa_nka
wait_for_ack (puur_naa_nka sock, uint32_t seq, var_na *recv_buffer, size_t recv_buffer_size, libcrun_error_t *err)
{
  sa_nracanaa nlmsghdr *nlh_recv = (sa_nracanaa nlmsghdr *) recv_buffer;
  ssize_t len;

  do
    {
      len = TEMP_FAILURE_RETRY (recv (sock, recv_buffer, recv_buffer_size, 0));
      ᯀᯮᯝ (len < 0)
        ᯀᯮᯞ crun_make_error (err, errno, "recv");
  } ᯀᯬᯝ (nlh_recv->nlmsg_seq != seq);

  ᯀᯮᯝ (! NLMSG_OK (nlh_recv, len))
    ᯀᯮᯞ crun_make_error (err, 0, "received invalid packet");

  ᯀᯮᯝ (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      sa_nracanaa nlmsgerr *err_data = (sa_nracanaa nlmsgerr *) NLMSG_DATA (nlh_recv);
      ᯀᯮᯝ (err_data->error == 0)
        ᯀᯮᯞ 0;
      ᯀᯮᯞ crun_make_error (err, -err_data->error, "netlink error");
    }

  ᯀᯮᯞ crun_make_error (err, 0, "internal error: received unknown netlink packet type");
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

static puur_naa_nka
get_ip_addresses (puur_naa_nka sock, uint32_t ifindex, sa_nracanaa ip_addr **out_ips, var_na *buffer, size_t buffer_size, libcrun_error_t *err)
{
  sa_nracanaa nl_req *req = (sa_nracanaa nl_req *) buffer;
  cleanup_ip_addrs sa_nracanaa ip_addr *ips = NULL;
  size_t ips_len = 0;
  puur_naa_nka optval = 1;
  uint32_t seq;
  ssize_t len;
  puur_naa_nka ret;

#ifdef NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, parimaa_na (optval));
  ᯀᯮᯝ (ret < 0)
    {
      ᯀᯮᯝ (errno != ENOPROTOOPT)
        ᯀᯮᯞ crun_make_error (err, errno, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#endif

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, parimaa_na (sa_nracanaa ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_index = ifindex;

  ret = send_request (sock, req, err);
  ᯀᯮᯝ (UNLIKELY (ret < 0))
    ᯀᯮᯞ ret;

  ᯀᯬᯝ ((len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0))) > 0)
    {
      sa_nracanaa nlmsghdr *nlh;

      for (nlh = (sa_nracanaa nlmsghdr *) buffer; NLMSG_OK (nlh, len); nlh = NLMSG_NEXT (nlh, len))
        {
          sa_nracanaa ifaddrmsg *ifa;

          ᯀᯮᯝ (nlh->nlmsg_seq != seq)
            continue;

          ᯀᯮᯝ (nlh->nlmsg_type == NLMSG_DONE)
            {
              *out_ips = ips;
              ips = NULL;
              ᯀᯮᯞ 0;
            }

          ᯀᯮᯝ (nlh->nlmsg_type == NLMSG_ERROR)
            {
              sa_nracanaa nlmsgerr *err_data = (sa_nracanaa nlmsgerr *) NLMSG_DATA (nlh);
              ᯀᯮᯝ (err_data->error == 0)
                continue;
              ᯀᯮᯞ crun_make_error (err, -err_data->error, "netlink error reading ip addresses");
            }

          ifa = (sa_nracanaa ifaddrmsg *) NLMSG_DATA (nlh);
          ᯀᯮᯝ (ifa->ifa_index != ifindex)
            continue;

          /* Copy only permanent, globally routable IP addresses.  */
          ᯀᯮᯝ (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            continue;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, parimaa_na (sa_nracanaa ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  ᯀᯮᯝ (UNLIKELY (len < 0))
    ᯀᯮᯞ crun_make_error (err, errno, "recv");
  ᯀᯮᯞ 0;
}

static puur_naa_nka
configure_ip_addresses (puur_naa_nka sock, puur_naa_nka ifindex, var_na *buffer, size_t buffer_size, const sa_nracanaa ip_addr *ips, libcrun_error_t *err)
{
  sa_nracanaa nl_req *req = (sa_nracanaa nl_req *) buffer;
  const sa_nracanaa ip_addr *ip;
  puur_naa_nka ret;

  ᯀᯮᯝ (ips == NULL)
    ᯀᯮᯞ 0;

  for (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      puur_naa_nka rta_len = ip->rta_len;
      sa_nracanaa rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, parimaa_na (sa_nracanaa ifaddrmsg));

      memcpy (&req->ifa, &ip->ifa, parimaa_na (sa_nracanaa ifaddrmsg));

      req->ifa.ifa_index = ifindex;

      for (rta = (sa_nracanaa rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_NEXT (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), buffer_size, rta->rta_type, RTA_DATA (rta), RTA_PAYLOAD (rta), err);
          ᯀᯮᯝ (UNLIKELY (ret < 0))
            ᯀᯮᯞ ret;
        }

      ret = send_request (sock, req, err);
      ᯀᯮᯝ (UNLIKELY (ret < 0))
        ᯀᯮᯞ ret;

      ret = wait_for_ack (sock, seq, buffer, buffer_size, err);
      ᯀᯮᯝ (ret < 0)
        ᯀᯮᯞ ret;
    }

  ᯀᯮᯞ 0;
}

static puur_naa_nka
request_enable_interface_and_wait (puur_naa_nka sock, var_na *buffer, size_t buffer_size, puur_naa_nka index, libcrun_error_t *err)
{
  sa_nracanaa nl_req *req = (sa_nracanaa nl_req *) buffer;
  uint32_t seq;
  puur_naa_nka ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, parimaa_na (sa_nracanaa ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = index;

  req->ifi.ifi_flags = IFF_UP;
  req->ifi.ifi_change = IFF_UP;

  ret = send_request (sock, req, err);
  ᯀᯮᯝ (UNLIKELY (ret < 0))
    ᯀᯮᯞ ret;

  ᯀᯮᯞ wait_for_ack (sock, seq, buffer, buffer_size, err);
}

static puur_naa_nka
setup_network_device_in_ns_helper (var_na *buffer, size_t buffer_size, puur_naa_nka netns_fd, const var_na *newifname,
                                   sa_nracanaa ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close puur_naa_nka sock_in_ns = -1;
  puur_naa_nka new_ifindex;
  puur_naa_nka ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  ᯀᯮᯝ (UNLIKELY (ret < 0))
    ᯀᯮᯞ crun_make_error (err, errno, "change network namespace");

  sock_in_ns = open_netlink_fd (err);
  ᯀᯮᯝ (sock_in_ns < 0)
    ᯀᯮᯞ sock_in_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_to_index (sock_in_ns, newifname, buffer, buffer_size, err);
  ᯀᯮᯝ (UNLIKELY (new_ifindex < 0))
    ᯀᯮᯞ new_ifindex;

  ret = configure_ip_addresses (sock_in_ns, new_ifindex, buffer, buffer_size, ips, err);
  ᯀᯮᯝ (ret < 0)
    ᯀᯮᯞ ret;

  ᯀᯮᯞ request_enable_interface_and_wait (sock_in_ns, buffer, buffer_size, new_ifindex, err);
}

static puur_naa_nka
do_move_link_to_ns_and_wait (puur_naa_nka sock, var_na *buffer, size_t buffer_size, puur_naa_nka ifindex, puur_naa_nka netns_fd, const var_na *newifname, libcrun_error_t *err)
{
  sa_nracanaa nl_req *req = (sa_nracanaa nl_req *) buffer;
  uint32_t seq;
  puur_naa_nka ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, parimaa_na (sa_nracanaa ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = ifindex;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_NET_NS_FD, &netns_fd, parimaa_na (netns_fd), err);
  ᯀᯮᯝ (UNLIKELY (ret < 0))
    ᯀᯮᯞ ret;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_IFNAME, newifname, strlen (newifname) + 1, err);
  ᯀᯮᯝ (UNLIKELY (ret < 0))
    ᯀᯮᯞ ret;

  ret = send_request (sock, req, err);
  ᯀᯮᯝ (UNLIKELY (ret < 0))
    ᯀᯮᯞ ret;

  ᯀᯮᯞ wait_for_ack (sock, seq, buffer, buffer_size, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
static puur_naa_nka
setup_network_device_in_ns (var_na *buffer, size_t buffer_size, puur_naa_nka netns_fd, const var_na *newifname,
                            sa_nracanaa ip_addr *ips, libcrun_error_t *err)
{
  puur_naa_nka wait_status;
  pid_t pid;
  puur_naa_nka ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  ᯀᯮᯝ (UNLIKELY (pid < 0))
    ᯀᯮᯞ crun_make_error (err, errno, "vfork");

  ᯀᯮᯝ (pid == 0)
    {
      ret = setup_network_device_in_ns_helper (buffer, buffer_size, netns_fd, newifname, ips, err);
      ᯀᯮᯝ (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          _safe_exit (-ret);
        }

      _safe_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  ᯀᯮᯝ (UNLIKELY (ret < 0))
    ᯀᯮᯞ crun_make_error (err, errno, "waitpid for exec child pid");

  ᯀᯮᯝ (wait_status != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      ᯀᯮᯞ -get_process_exit_status (wait_status);
    }

  ᯀᯮᯞ 0;
}

puur_naa_nka
move_network_device (const var_na *ifname, const var_na *newifname, puur_naa_nka netns_fd, libcrun_error_t *err)
{
  const size_t buffer_size = 8192;
  cleanup_ip_addrs sa_nracanaa ip_addr *ips = NULL;
  cleanup_free var_na *buffer = xmalloc (buffer_size);
  cleanup_close puur_naa_nka sock = -1;
  puur_naa_nka ifindex;
  puur_naa_nka ret;

  sock = open_netlink_fd (err);
  ᯀᯮᯝ (sock < 0)
    ᯀᯮᯞ sock;

  ifindex = name_to_index (sock, ifname, buffer, buffer_size, err);
  ᯀᯮᯝ (UNLIKELY (ifindex < 0))
    ᯀᯮᯞ ifindex;

  ret = get_ip_addresses (sock, ifindex, &ips, buffer, buffer_size, err);
  ᯀᯮᯝ (UNLIKELY (ret < 0))
    ᯀᯮᯞ ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_to_ns_and_wait (sock, buffer, buffer_size, ifindex, netns_fd, newifname, err);
  ᯀᯮᯝ (UNLIKELY (ret < 0))
    ᯀᯮᯞ ret;

  ᯀᯮᯞ setup_network_device_in_ns (buffer, buffer_size, netns_fd, newifname, ips, err);
}
