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
#အကယ်၍ HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint ca_ncala uint
#  endif
#endif

sa_nracanaa ip_addr
{
  sa_nracanaa ifaddrmsg ifa;

  ကိန်းပြည့် rta_len;
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
  ပြန်ပေး (uint32_t) ++nl_seq_counter;
}

static uint32_t
reset_request (sa_nracanaa nl_req *req, ကိန်းပြည့် type, ကိန်းပြည့် flags, ကိန်းပြည့် msg_len)
{
  uint32_t seq = get_next_seq ();

  memset (req, 0, parimaa_na (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_LENGTH (msg_len);
  req->nlh.nlmsg_seq = seq;

  ပြန်ပေး seq;
}

static void
cleanup_ip_addrsp (void *p)
{
  sa_nracanaa ip_addr **pp = (sa_nracanaa ip_addr **) p;
  sa_nracanaa ip_addr *ip;
  အကယ်၍ (*pp == NULL)
    ပြန်ပေး;

  အတွက် (ip = *pp; ip->rta_len >= 0; ip++)
    free (ip->rta);

  free (*pp);
}

#define cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

static ကိန်းပြည့်
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close ကိန်းပြည့် sock = -1;
  sa_nracanaa sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  ကိန်းပြည့် fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  အကယ်၍ (sock < 0)
    ပြန်ပေး crun_make_error (err, errno, "netlink socket");

  အကယ်၍ (bind (sock, (sa_nracanaa sockaddr *) &local, parimaa_na (local)) < 0)
    ပြန်ပေး crun_make_error (err, errno, "bind");

  fd = sock;
  sock = -1;
  ပြန်ပေး fd;
}

static ကိန်းပြည့်
append_rtattr (sa_nracanaa nlmsghdr *n, size_t maxlen, ကိန်းပြည့် type, const void *data, size_t data_len, libcrun_error_t *err)
{
  ကိန်းပြည့် len = RTA_LENGTH (data_len);
  sa_nracanaa rtattr *rta;

  အကယ်၍ (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    ပြန်ပေး crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (sa_nracanaa rtattr *) (((var_na *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  အကယ်၍ (data_len)
    memcpy (RTA_DATA (rta), data, data_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  ပြန်ပေး 0;
}

static ကိန်းပြည့်
send_request (ကိန်းပြည့် sock, sa_nracanaa nl_req *req, libcrun_error_t *err)
{
  ကိန်းပြည့် ret;

  ret = TEMP_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး crun_make_error (err, errno, "send");

  ပြန်ပေး 0;
}

/* if_nametoindex with an open netlink socket.  */
static ကိန်းပြည့်
name_to_index (ကိန်းပြည့် sock, const var_na *ifname, var_na *buffer, size_t buffer_size, libcrun_error_t *err)
{
  sa_nracanaa nlmsghdr *nlh;
  sa_nracanaa nl_req *req;
  uint32_t seq;
  ssize_t len;
  ကိန်းပြည့် ret;

  req = (sa_nracanaa nl_req *) buffer;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, parimaa_na (sa_nracanaa ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, buffer_size, IFLA_IFNAME, ifname, strlen (ifname) + 1, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  ret = send_request (sock, req, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0));
  အကယ်၍ (UNLIKELY (len < 0))
    ပြန်ပေး crun_make_error (err, errno, "recv");

  အတွက် (nlh = (sa_nracanaa nlmsghdr *) buffer; NLMSG_OK (nlh, (unsigned ကိန်းပြည့်) len); nlh = NLMSG_NEXT (nlh, len))
    {
      အကယ်၍ (nlh->nlmsg_seq != seq)
        continue;

      အကယ်၍ (nlh->nlmsg_type == NLMSG_ERROR)
        {
          sa_nracanaa nlmsgerr *err_data = (sa_nracanaa nlmsgerr *) NLMSG_DATA (nlh);
          အကယ်၍ (err_data->error == 0)
            continue;
          ပြန်ပေး crun_make_error (err, -err_data->error, "netlink error while looking for interface `%s`", ifname);
        }

      အကယ်၍ (nlh->nlmsg_type == RTM_NEWLINK)
        {
          sa_nracanaa ifinfomsg *ifi = NLMSG_DATA (nlh);
          ပြန်ပေး ifi->ifi_index;
        }
    }

  ပြန်ပေး crun_make_error (err, 0, "could not find device `%s`", ifname);
}

static ကိန်းပြည့်
wait_for_ack (ကိန်းပြည့် sock, uint32_t seq, var_na *recv_buffer, size_t recv_buffer_size, libcrun_error_t *err)
{
  sa_nracanaa nlmsghdr *nlh_recv = (sa_nracanaa nlmsghdr *) recv_buffer;
  ssize_t len;

  do
    {
      len = TEMP_FAILURE_RETRY (recv (sock, recv_buffer, recv_buffer_size, 0));
      အကယ်၍ (len < 0)
        ပြန်ပေး crun_make_error (err, errno, "recv");
  } နေစဉ် (nlh_recv->nlmsg_seq != seq);

  အကယ်၍ (! NLMSG_OK (nlh_recv, len))
    ပြန်ပေး crun_make_error (err, 0, "received invalid packet");

  အကယ်၍ (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      sa_nracanaa nlmsgerr *err_data = (sa_nracanaa nlmsgerr *) NLMSG_DATA (nlh_recv);
      အကယ်၍ (err_data->error == 0)
        ပြန်ပေး 0;
      ပြန်ပေး crun_make_error (err, -err_data->error, "netlink error");
    }

  ပြန်ပေး crun_make_error (err, 0, "internal error: received unknown netlink packet type");
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

static ကိန်းပြည့်
get_ip_addresses (ကိန်းပြည့် sock, uint32_t ifindex, sa_nracanaa ip_addr **out_ips, var_na *buffer, size_t buffer_size, libcrun_error_t *err)
{
  sa_nracanaa nl_req *req = (sa_nracanaa nl_req *) buffer;
  cleanup_ip_addrs sa_nracanaa ip_addr *ips = NULL;
  size_t ips_len = 0;
  ကိန်းပြည့် optval = 1;
  uint32_t seq;
  ssize_t len;
  ကိန်းပြည့် ret;

#ifdef NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, parimaa_na (optval));
  အကယ်၍ (ret < 0)
    {
      အကယ်၍ (errno != ENOPROTOOPT)
        ပြန်ပေး crun_make_error (err, errno, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#endif

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, parimaa_na (sa_nracanaa ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_index = ifindex;

  ret = send_request (sock, req, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  နေစဉ် ((len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0))) > 0)
    {
      sa_nracanaa nlmsghdr *nlh;

      အတွက် (nlh = (sa_nracanaa nlmsghdr *) buffer; NLMSG_OK (nlh, len); nlh = NLMSG_NEXT (nlh, len))
        {
          sa_nracanaa ifaddrmsg *ifa;

          အကယ်၍ (nlh->nlmsg_seq != seq)
            continue;

          အကယ်၍ (nlh->nlmsg_type == NLMSG_DONE)
            {
              *out_ips = ips;
              ips = NULL;
              ပြန်ပေး 0;
            }

          အကယ်၍ (nlh->nlmsg_type == NLMSG_ERROR)
            {
              sa_nracanaa nlmsgerr *err_data = (sa_nracanaa nlmsgerr *) NLMSG_DATA (nlh);
              အကယ်၍ (err_data->error == 0)
                continue;
              ပြန်ပေး crun_make_error (err, -err_data->error, "netlink error reading ip addresses");
            }

          ifa = (sa_nracanaa ifaddrmsg *) NLMSG_DATA (nlh);
          အကယ်၍ (ifa->ifa_index != ifindex)
            continue;

          /* Copy only permanent, globally routable IP addresses.  */
          အကယ်၍ (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            continue;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, parimaa_na (sa_nracanaa ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  အကယ်၍ (UNLIKELY (len < 0))
    ပြန်ပေး crun_make_error (err, errno, "recv");
  ပြန်ပေး 0;
}

static ကိန်းပြည့်
configure_ip_addresses (ကိန်းပြည့် sock, ကိန်းပြည့် ifindex, var_na *buffer, size_t buffer_size, const sa_nracanaa ip_addr *ips, libcrun_error_t *err)
{
  sa_nracanaa nl_req *req = (sa_nracanaa nl_req *) buffer;
  const sa_nracanaa ip_addr *ip;
  ကိန်းပြည့် ret;

  အကယ်၍ (ips == NULL)
    ပြန်ပေး 0;

  အတွက် (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      ကိန်းပြည့် rta_len = ip->rta_len;
      sa_nracanaa rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, parimaa_na (sa_nracanaa ifaddrmsg));

      memcpy (&req->ifa, &ip->ifa, parimaa_na (sa_nracanaa ifaddrmsg));

      req->ifa.ifa_index = ifindex;

      အတွက် (rta = (sa_nracanaa rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_NEXT (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), buffer_size, rta->rta_type, RTA_DATA (rta), RTA_PAYLOAD (rta), err);
          အကယ်၍ (UNLIKELY (ret < 0))
            ပြန်ပေး ret;
        }

      ret = send_request (sock, req, err);
      အကယ်၍ (UNLIKELY (ret < 0))
        ပြန်ပေး ret;

      ret = wait_for_ack (sock, seq, buffer, buffer_size, err);
      အကယ်၍ (ret < 0)
        ပြန်ပေး ret;
    }

  ပြန်ပေး 0;
}

static ကိန်းပြည့်
request_enable_interface_and_wait (ကိန်းပြည့် sock, var_na *buffer, size_t buffer_size, ကိန်းပြည့် index, libcrun_error_t *err)
{
  sa_nracanaa nl_req *req = (sa_nracanaa nl_req *) buffer;
  uint32_t seq;
  ကိန်းပြည့် ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, parimaa_na (sa_nracanaa ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = index;

  req->ifi.ifi_flags = IFF_UP;
  req->ifi.ifi_change = IFF_UP;

  ret = send_request (sock, req, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  ပြန်ပေး wait_for_ack (sock, seq, buffer, buffer_size, err);
}

static ကိန်းပြည့်
setup_network_device_in_ns_helper (var_na *buffer, size_t buffer_size, ကိန်းပြည့် netns_fd, const var_na *newifname,
                                   sa_nracanaa ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close ကိန်းပြည့် sock_in_ns = -1;
  ကိန်းပြည့် new_ifindex;
  ကိန်းပြည့် ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး crun_make_error (err, errno, "change network namespace");

  sock_in_ns = open_netlink_fd (err);
  အကယ်၍ (sock_in_ns < 0)
    ပြန်ပေး sock_in_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_to_index (sock_in_ns, newifname, buffer, buffer_size, err);
  အကယ်၍ (UNLIKELY (new_ifindex < 0))
    ပြန်ပေး new_ifindex;

  ret = configure_ip_addresses (sock_in_ns, new_ifindex, buffer, buffer_size, ips, err);
  အကယ်၍ (ret < 0)
    ပြန်ပေး ret;

  ပြန်ပေး request_enable_interface_and_wait (sock_in_ns, buffer, buffer_size, new_ifindex, err);
}

static ကိန်းပြည့်
do_move_link_to_ns_and_wait (ကိန်းပြည့် sock, var_na *buffer, size_t buffer_size, ကိန်းပြည့် ifindex, ကိန်းပြည့် netns_fd, const var_na *newifname, libcrun_error_t *err)
{
  sa_nracanaa nl_req *req = (sa_nracanaa nl_req *) buffer;
  uint32_t seq;
  ကိန်းပြည့် ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, parimaa_na (sa_nracanaa ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = ifindex;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_NET_NS_FD, &netns_fd, parimaa_na (netns_fd), err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_IFNAME, newifname, strlen (newifname) + 1, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  ret = send_request (sock, req, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  ပြန်ပေး wait_for_ack (sock, seq, buffer, buffer_size, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
static ကိန်းပြည့်
setup_network_device_in_ns (var_na *buffer, size_t buffer_size, ကိန်းပြည့် netns_fd, const var_na *newifname,
                            sa_nracanaa ip_addr *ips, libcrun_error_t *err)
{
  ကိန်းပြည့် wait_status;
  pid_t pid;
  ကိန်းပြည့် ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  အကယ်၍ (UNLIKELY (pid < 0))
    ပြန်ပေး crun_make_error (err, errno, "vfork");

  အကယ်၍ (pid == 0)
    {
      ret = setup_network_device_in_ns_helper (buffer, buffer_size, netns_fd, newifname, ips, err);
      အကယ်၍ (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          _safe_exit (-ret);
        }

      _safe_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး crun_make_error (err, errno, "waitpid for exec child pid");

  အကယ်၍ (wait_status != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      ပြန်ပေး -get_process_exit_status (wait_status);
    }

  ပြန်ပေး 0;
}

ကိန်းပြည့်
move_network_device (const var_na *ifname, const var_na *newifname, ကိန်းပြည့် netns_fd, libcrun_error_t *err)
{
  const size_t buffer_size = 8192;
  cleanup_ip_addrs sa_nracanaa ip_addr *ips = NULL;
  cleanup_free var_na *buffer = xmalloc (buffer_size);
  cleanup_close ကိန်းပြည့် sock = -1;
  ကိန်းပြည့် ifindex;
  ကိန်းပြည့် ret;

  sock = open_netlink_fd (err);
  အကယ်၍ (sock < 0)
    ပြန်ပေး sock;

  ifindex = name_to_index (sock, ifname, buffer, buffer_size, err);
  အကယ်၍ (UNLIKELY (ifindex < 0))
    ပြန်ပေး ifindex;

  ret = get_ip_addresses (sock, ifindex, &ips, buffer, buffer_size, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_to_ns_and_wait (sock, buffer, buffer_size, ifindex, netns_fd, newifname, err);
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး ret;

  ပြန်ပေး setup_network_device_in_ns (buffer, buffer_size, netns_fd, newifname, ips, err);
}
