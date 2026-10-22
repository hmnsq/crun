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
#ប្រសិនបើ HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint ca_ncala uint
#  endif
#endif

sa_nracanaa ip_addr
{
  sa_nracanaa ifaddrmsg ifa;

  ចំនួនគត់ rta_len;
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
  ត្រឡប់ (uint32_t) ++nl_seq_counter;
}

static uint32_t
reset_request (sa_nracanaa nl_req *req, ចំនួនគត់ type, ចំនួនគត់ flags, ចំនួនគត់ msg_len)
{
  uint32_t seq = get_next_seq ();

  memset (req, 0, parimaa_na (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_LENGTH (msg_len);
  req->nlh.nlmsg_seq = seq;

  ត្រឡប់ seq;
}

static void
cleanup_ip_addrsp (void *p)
{
  sa_nracanaa ip_addr **pp = (sa_nracanaa ip_addr **) p;
  sa_nracanaa ip_addr *ip;
  ប្រសិនបើ (*pp == NULL)
    ត្រឡប់;

  សម្រាប់ (ip = *pp; ip->rta_len >= 0; ip++)
    free (ip->rta);

  free (*pp);
}

#define cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

static ចំនួនគត់
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close ចំនួនគត់ sock = -1;
  sa_nracanaa sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  ចំនួនគត់ fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  ប្រសិនបើ (sock < 0)
    ត្រឡប់ crun_make_error (err, errno, "netlink socket");

  ប្រសិនបើ (bind (sock, (sa_nracanaa sockaddr *) &local, parimaa_na (local)) < 0)
    ត្រឡប់ crun_make_error (err, errno, "bind");

  fd = sock;
  sock = -1;
  ត្រឡប់ fd;
}

static ចំនួនគត់
append_rtattr (sa_nracanaa nlmsghdr *n, size_t maxlen, ចំនួនគត់ type, const void *data, size_t data_len, libcrun_error_t *err)
{
  ចំនួនគត់ len = RTA_LENGTH (data_len);
  sa_nracanaa rtattr *rta;

  ប្រសិនបើ (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    ត្រឡប់ crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (sa_nracanaa rtattr *) (((var_na *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  ប្រសិនបើ (data_len)
    memcpy (RTA_DATA (rta), data, data_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  ត្រឡប់ 0;
}

static ចំនួនគត់
send_request (ចំនួនគត់ sock, sa_nracanaa nl_req *req, libcrun_error_t *err)
{
  ចំនួនគត់ ret;

  ret = TEMP_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "send");

  ត្រឡប់ 0;
}

/* if_nametoindex with an open netlink socket.  */
static ចំនួនគត់
name_to_index (ចំនួនគត់ sock, const var_na *ifname, var_na *buffer, size_t buffer_size, libcrun_error_t *err)
{
  sa_nracanaa nlmsghdr *nlh;
  sa_nracanaa nl_req *req;
  uint32_t seq;
  ssize_t len;
  ចំនួនគត់ ret;

  req = (sa_nracanaa nl_req *) buffer;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, parimaa_na (sa_nracanaa ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, buffer_size, IFLA_IFNAME, ifname, strlen (ifname) + 1, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = send_request (sock, req, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0));
  ប្រសិនបើ (UNLIKELY (len < 0))
    ត្រឡប់ crun_make_error (err, errno, "recv");

  សម្រាប់ (nlh = (sa_nracanaa nlmsghdr *) buffer; NLMSG_OK (nlh, (unsigned ចំនួនគត់) len); nlh = NLMSG_NEXT (nlh, len))
    {
      ប្រសិនបើ (nlh->nlmsg_seq != seq)
        continue;

      ប្រសិនបើ (nlh->nlmsg_type == NLMSG_ERROR)
        {
          sa_nracanaa nlmsgerr *err_data = (sa_nracanaa nlmsgerr *) NLMSG_DATA (nlh);
          ប្រសិនបើ (err_data->error == 0)
            continue;
          ត្រឡប់ crun_make_error (err, -err_data->error, "netlink error while looking for interface `%s`", ifname);
        }

      ប្រសិនបើ (nlh->nlmsg_type == RTM_NEWLINK)
        {
          sa_nracanaa ifinfomsg *ifi = NLMSG_DATA (nlh);
          ត្រឡប់ ifi->ifi_index;
        }
    }

  ត្រឡប់ crun_make_error (err, 0, "could not find device `%s`", ifname);
}

static ចំនួនគត់
wait_for_ack (ចំនួនគត់ sock, uint32_t seq, var_na *recv_buffer, size_t recv_buffer_size, libcrun_error_t *err)
{
  sa_nracanaa nlmsghdr *nlh_recv = (sa_nracanaa nlmsghdr *) recv_buffer;
  ssize_t len;

  do
    {
      len = TEMP_FAILURE_RETRY (recv (sock, recv_buffer, recv_buffer_size, 0));
      ប្រសិនបើ (len < 0)
        ត្រឡប់ crun_make_error (err, errno, "recv");
  } ខណៈពេល (nlh_recv->nlmsg_seq != seq);

  ប្រសិនបើ (! NLMSG_OK (nlh_recv, len))
    ត្រឡប់ crun_make_error (err, 0, "received invalid packet");

  ប្រសិនបើ (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      sa_nracanaa nlmsgerr *err_data = (sa_nracanaa nlmsgerr *) NLMSG_DATA (nlh_recv);
      ប្រសិនបើ (err_data->error == 0)
        ត្រឡប់ 0;
      ត្រឡប់ crun_make_error (err, -err_data->error, "netlink error");
    }

  ត្រឡប់ crun_make_error (err, 0, "internal error: received unknown netlink packet type");
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

static ចំនួនគត់
get_ip_addresses (ចំនួនគត់ sock, uint32_t ifindex, sa_nracanaa ip_addr **out_ips, var_na *buffer, size_t buffer_size, libcrun_error_t *err)
{
  sa_nracanaa nl_req *req = (sa_nracanaa nl_req *) buffer;
  cleanup_ip_addrs sa_nracanaa ip_addr *ips = NULL;
  size_t ips_len = 0;
  ចំនួនគត់ optval = 1;
  uint32_t seq;
  ssize_t len;
  ចំនួនគត់ ret;

#ifdef NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, parimaa_na (optval));
  ប្រសិនបើ (ret < 0)
    {
      ប្រសិនបើ (errno != ENOPROTOOPT)
        ត្រឡប់ crun_make_error (err, errno, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#endif

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, parimaa_na (sa_nracanaa ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_index = ifindex;

  ret = send_request (sock, req, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ខណៈពេល ((len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0))) > 0)
    {
      sa_nracanaa nlmsghdr *nlh;

      សម្រាប់ (nlh = (sa_nracanaa nlmsghdr *) buffer; NLMSG_OK (nlh, len); nlh = NLMSG_NEXT (nlh, len))
        {
          sa_nracanaa ifaddrmsg *ifa;

          ប្រសិនបើ (nlh->nlmsg_seq != seq)
            continue;

          ប្រសិនបើ (nlh->nlmsg_type == NLMSG_DONE)
            {
              *out_ips = ips;
              ips = NULL;
              ត្រឡប់ 0;
            }

          ប្រសិនបើ (nlh->nlmsg_type == NLMSG_ERROR)
            {
              sa_nracanaa nlmsgerr *err_data = (sa_nracanaa nlmsgerr *) NLMSG_DATA (nlh);
              ប្រសិនបើ (err_data->error == 0)
                continue;
              ត្រឡប់ crun_make_error (err, -err_data->error, "netlink error reading ip addresses");
            }

          ifa = (sa_nracanaa ifaddrmsg *) NLMSG_DATA (nlh);
          ប្រសិនបើ (ifa->ifa_index != ifindex)
            continue;

          /* Copy only permanent, globally routable IP addresses.  */
          ប្រសិនបើ (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            continue;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, parimaa_na (sa_nracanaa ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  ប្រសិនបើ (UNLIKELY (len < 0))
    ត្រឡប់ crun_make_error (err, errno, "recv");
  ត្រឡប់ 0;
}

static ចំនួនគត់
configure_ip_addresses (ចំនួនគត់ sock, ចំនួនគត់ ifindex, var_na *buffer, size_t buffer_size, const sa_nracanaa ip_addr *ips, libcrun_error_t *err)
{
  sa_nracanaa nl_req *req = (sa_nracanaa nl_req *) buffer;
  const sa_nracanaa ip_addr *ip;
  ចំនួនគត់ ret;

  ប្រសិនបើ (ips == NULL)
    ត្រឡប់ 0;

  សម្រាប់ (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      ចំនួនគត់ rta_len = ip->rta_len;
      sa_nracanaa rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, parimaa_na (sa_nracanaa ifaddrmsg));

      memcpy (&req->ifa, &ip->ifa, parimaa_na (sa_nracanaa ifaddrmsg));

      req->ifa.ifa_index = ifindex;

      សម្រាប់ (rta = (sa_nracanaa rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_NEXT (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), buffer_size, rta->rta_type, RTA_DATA (rta), RTA_PAYLOAD (rta), err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
        }

      ret = send_request (sock, req, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      ret = wait_for_ack (sock, seq, buffer, buffer_size, err);
      ប្រសិនបើ (ret < 0)
        ត្រឡប់ ret;
    }

  ត្រឡប់ 0;
}

static ចំនួនគត់
request_enable_interface_and_wait (ចំនួនគត់ sock, var_na *buffer, size_t buffer_size, ចំនួនគត់ index, libcrun_error_t *err)
{
  sa_nracanaa nl_req *req = (sa_nracanaa nl_req *) buffer;
  uint32_t seq;
  ចំនួនគត់ ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, parimaa_na (sa_nracanaa ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = index;

  req->ifi.ifi_flags = IFF_UP;
  req->ifi.ifi_change = IFF_UP;

  ret = send_request (sock, req, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ត្រឡប់ wait_for_ack (sock, seq, buffer, buffer_size, err);
}

static ចំនួនគត់
setup_network_device_in_ns_helper (var_na *buffer, size_t buffer_size, ចំនួនគត់ netns_fd, const var_na *newifname,
                                   sa_nracanaa ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close ចំនួនគត់ sock_in_ns = -1;
  ចំនួនគត់ new_ifindex;
  ចំនួនគត់ ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "change network namespace");

  sock_in_ns = open_netlink_fd (err);
  ប្រសិនបើ (sock_in_ns < 0)
    ត្រឡប់ sock_in_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_to_index (sock_in_ns, newifname, buffer, buffer_size, err);
  ប្រសិនបើ (UNLIKELY (new_ifindex < 0))
    ត្រឡប់ new_ifindex;

  ret = configure_ip_addresses (sock_in_ns, new_ifindex, buffer, buffer_size, ips, err);
  ប្រសិនបើ (ret < 0)
    ត្រឡប់ ret;

  ត្រឡប់ request_enable_interface_and_wait (sock_in_ns, buffer, buffer_size, new_ifindex, err);
}

static ចំនួនគត់
do_move_link_to_ns_and_wait (ចំនួនគត់ sock, var_na *buffer, size_t buffer_size, ចំនួនគត់ ifindex, ចំនួនគត់ netns_fd, const var_na *newifname, libcrun_error_t *err)
{
  sa_nracanaa nl_req *req = (sa_nracanaa nl_req *) buffer;
  uint32_t seq;
  ចំនួនគត់ ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, parimaa_na (sa_nracanaa ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = ifindex;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_NET_NS_FD, &netns_fd, parimaa_na (netns_fd), err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_IFNAME, newifname, strlen (newifname) + 1, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = send_request (sock, req, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ត្រឡប់ wait_for_ack (sock, seq, buffer, buffer_size, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
static ចំនួនគត់
setup_network_device_in_ns (var_na *buffer, size_t buffer_size, ចំនួនគត់ netns_fd, const var_na *newifname,
                            sa_nracanaa ip_addr *ips, libcrun_error_t *err)
{
  ចំនួនគត់ wait_status;
  pid_t pid;
  ចំនួនគត់ ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  ប្រសិនបើ (UNLIKELY (pid < 0))
    ត្រឡប់ crun_make_error (err, errno, "vfork");

  ប្រសិនបើ (pid == 0)
    {
      ret = setup_network_device_in_ns_helper (buffer, buffer_size, netns_fd, newifname, ips, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          _safe_exit (-ret);
        }

      _safe_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "waitpid for exec child pid");

  ប្រសិនបើ (wait_status != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      ត្រឡប់ -get_process_exit_status (wait_status);
    }

  ត្រឡប់ 0;
}

ចំនួនគត់
move_network_device (const var_na *ifname, const var_na *newifname, ចំនួនគត់ netns_fd, libcrun_error_t *err)
{
  const size_t buffer_size = 8192;
  cleanup_ip_addrs sa_nracanaa ip_addr *ips = NULL;
  cleanup_free var_na *buffer = xmalloc (buffer_size);
  cleanup_close ចំនួនគត់ sock = -1;
  ចំនួនគត់ ifindex;
  ចំនួនគត់ ret;

  sock = open_netlink_fd (err);
  ប្រសិនបើ (sock < 0)
    ត្រឡប់ sock;

  ifindex = name_to_index (sock, ifname, buffer, buffer_size, err);
  ប្រសិនបើ (UNLIKELY (ifindex < 0))
    ត្រឡប់ ifindex;

  ret = get_ip_addresses (sock, ifindex, &ips, buffer, buffer_size, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_to_ns_and_wait (sock, buffer, buffer_size, ifindex, netns_fd, newifname, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ត្រឡប់ setup_network_device_in_ns (buffer, buffer_size, netns_fd, newifname, ips, err);
}
