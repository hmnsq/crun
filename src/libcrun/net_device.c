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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "net_device.h"
#અન્તર્ભાવ "utils.h"

#અન્તર્ભાવ <sys/socket.h>
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <sched.h>
#અન્તર્ભાવ <sys/wait.h>
#અન્તર્ભાવ <sys/xattr.h>
#અન્તર્ભાવ <linux/netlink.h>
#અન્તર્ભાવ <linux/rtnetlink.h>
#અન્તર્ભાવ <arpa/inet.h>
#અન્તર્ભાવ <net/if.h>
#જો HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint અસ્થિર uint
#  endif
#endif

સંરચના ip_addr
{
  સંરચના ifaddrmsg ifa;

  પૂર્ણાંક rta_len;
  અક્ષર *rta;
};

સંરચના nl_req
{
  સંરચના nlmsghdr nlh;
  સંઘ
  {
    સંરચના ifaddrmsg ifa;
    સંરચના ifinfomsg ifi;
  };
};

સ્થિર atomic_uint nl_seq_counter;

સ્થિર uint32_t
get_next_seq ()
{
  પાછા (uint32_t) ++nl_seq_counter;
}

સ્થિર uint32_t
reset_request (સંરચના nl_req *req, પૂર્ણાંક type, પૂર્ણાંક flags, પૂર્ણાંક સંદેશ_len)
{
  uint32_t seq = get_next_seq ();

  memset (req, 0, કદ (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_LENGTH (સંદેશ_len);
  req->nlh.nlmsg_seq = seq;

  પાછા seq;
}

સ્થિર શૂન્ય
cleanup_ip_addrsp (શૂન્ય *p)
{
  સંરચના ip_addr **pp = (સંરચના ip_addr **) p;
  સંરચના ip_addr *ip;
  જો (*pp == ખાલી)
    પાછા;

  માટે (ip = *pp; ip->rta_len >= 0; ip++)
    મુક્ત (ip->rta);

  મુક્ત (*pp);
}

#define cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

સ્થિર પૂર્ણાંક
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક sock = -1;
  સંરચના sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  પૂર્ણાંક fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  જો (sock < 0)
    પાછા crun_make_error (err, errno, "netlink socket");

  જો (bind (sock, (સંરચના sockaddr *) &local, કદ (local)) < 0)
    પાછા crun_make_error (err, errno, "bind");

  fd = sock;
  sock = -1;
  પાછા fd;
}

સ્થિર પૂર્ણાંક
append_rtattr (સંરચના nlmsghdr *n, size_t maxlen, પૂર્ણાંક type, અચલ શૂન્ય *data, size_t data_len, libcrun_error_t *err)
{
  પૂર્ણાંક len = RTA_LENGTH (data_len);
  સંરચના rtattr *rta;

  જો (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    પાછા crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (સંરચના rtattr *) (((અક્ષર *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  જો (data_len)
    memcpy (RTA_DATA (rta), data, data_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  પાછા 0;
}

સ્થિર પૂર્ણાંક
send_request (પૂર્ણાંક sock, સંરચના nl_req *req, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  ret = TEMP_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "send");

  પાછા 0;
}

/* if_nametoindex with an open netlink socket.  */
સ્થિર પૂર્ણાંક
name_to_index (પૂર્ણાંક sock, અચલ અક્ષર *ifname, અક્ષર *buffer, size_t buffer_size, libcrun_error_t *err)
{
  સંરચના nlmsghdr *nlh;
  સંરચના nl_req *req;
  uint32_t seq;
  ssize_t len;
  પૂર્ણાંક ret;

  req = (સંરચના nl_req *) buffer;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, કદ (સંરચના ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, buffer_size, IFLA_IFNAME, ifname, strlen (ifname) + 1, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = send_request (sock, req, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0));
  જો (UNLIKELY (len < 0))
    પાછા crun_make_error (err, errno, "recv");

  માટે (nlh = (સંરચના nlmsghdr *) buffer; NLMSG_OK (nlh, (unsigned પૂર્ણાંક) len); nlh = NLMSG_NEXT (nlh, len))
    {
      જો (nlh->nlmsg_seq != seq)
        ચાલુ;

      જો (nlh->nlmsg_type == NLMSG_ERROR)
        {
          સંરચના nlmsgerr *err_data = (સંરચના nlmsgerr *) NLMSG_DATA (nlh);
          જો (err_data->error == 0)
            ચાલુ;
          પાછા crun_make_error (err, -err_data->error, "netlink error while looking for interface `%s`", ifname);
        }

      જો (nlh->nlmsg_type == RTM_NEWLINK)
        {
          સંરચના ifinfomsg *ifi = NLMSG_DATA (nlh);
          પાછા ifi->ifi_index;
        }
    }

  પાછા crun_make_error (err, 0, "could not find device `%s`", ifname);
}

સ્થિર પૂર્ણાંક
wait_for_ack (પૂર્ણાંક sock, uint32_t seq, અક્ષર *recv_buffer, size_t recv_buffer_size, libcrun_error_t *err)
{
  સંરચના nlmsghdr *nlh_recv = (સંરચના nlmsghdr *) recv_buffer;
  ssize_t len;

  કરો
    {
      len = TEMP_FAILURE_RETRY (recv (sock, recv_buffer, recv_buffer_size, 0));
      જો (len < 0)
        પાછા crun_make_error (err, errno, "recv");
  } જ્યારે (nlh_recv->nlmsg_seq != seq);

  જો (! NLMSG_OK (nlh_recv, len))
    પાછા crun_make_error (err, 0, "received invalid packet");

  જો (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      સંરચના nlmsgerr *err_data = (સંરચના nlmsgerr *) NLMSG_DATA (nlh_recv);
      જો (err_data->error == 0)
        પાછા 0;
      પાછા crun_make_error (err, -err_data->error, "netlink error");
    }

  પાછા crun_make_error (err, 0, "internal error: received unknown netlink packet type");
}

સ્થિર શૂન્ય
copy_ip_addr (સંરચના nlmsghdr *nlh, સંરચના ip_addr *ip)
{
  સંરચના ifaddrmsg *ifa = (સંરચના ifaddrmsg *) NLMSG_DATA (nlh);
  memcpy (&ip->ifa, ifa, કદ (સંરચના ifaddrmsg));
  ip->rta_len = IFA_PAYLOAD (nlh);
  ip->rta = xmalloc (ip->rta_len);
  memcpy (ip->rta, IFA_RTA (ifa), ip->rta_len);
};

સ્થિર પૂર્ણાંક
get_ip_addresses (પૂર્ણાંક sock, uint32_t ifindex, સંરચના ip_addr **out_ips, અક્ષર *buffer, size_t buffer_size, libcrun_error_t *err)
{
  સંરચના nl_req *req = (સંરચના nl_req *) buffer;
  cleanup_ip_addrs સંરચના ip_addr *ips = ખાલી;
  size_t ips_len = 0;
  પૂર્ણાંક optval = 1;
  uint32_t seq;
  ssize_t len;
  પૂર્ણાંક ret;

#ifdef NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, કદ (optval));
  જો (ret < 0)
    {
      જો (errno != ENOPROTOOPT)
        પાછા crun_make_error (err, errno, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#endif

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, કદ (સંરચના ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_index = ifindex;

  ret = send_request (sock, req, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જ્યારે ((len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0))) > 0)
    {
      સંરચના nlmsghdr *nlh;

      માટે (nlh = (સંરચના nlmsghdr *) buffer; NLMSG_OK (nlh, len); nlh = NLMSG_NEXT (nlh, len))
        {
          સંરચના ifaddrmsg *ifa;

          જો (nlh->nlmsg_seq != seq)
            ચાલુ;

          જો (nlh->nlmsg_type == NLMSG_DONE)
            {
              *out_ips = ips;
              ips = ખાલી;
              પાછા 0;
            }

          જો (nlh->nlmsg_type == NLMSG_ERROR)
            {
              સંરચના nlmsgerr *err_data = (સંરચના nlmsgerr *) NLMSG_DATA (nlh);
              જો (err_data->error == 0)
                ચાલુ;
              પાછા crun_make_error (err, -err_data->error, "netlink error reading ip addresses");
            }

          ifa = (સંરચના ifaddrmsg *) NLMSG_DATA (nlh);
          જો (ifa->ifa_index != ifindex)
            ચાલુ;

          /* Copy only permanent, globally routable IP addresses.  */
          જો (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            ચાલુ;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, કદ (સંરચના ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  જો (UNLIKELY (len < 0))
    પાછા crun_make_error (err, errno, "recv");
  પાછા 0;
}

સ્થિર પૂર્ણાંક
configure_ip_addresses (પૂર્ણાંક sock, પૂર્ણાંક ifindex, અક્ષર *buffer, size_t buffer_size, અચલ સંરચના ip_addr *ips, libcrun_error_t *err)
{
  સંરચના nl_req *req = (સંરચના nl_req *) buffer;
  અચલ સંરચના ip_addr *ip;
  પૂર્ણાંક ret;

  જો (ips == ખાલી)
    પાછા 0;

  માટે (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      પૂર્ણાંક rta_len = ip->rta_len;
      સંરચના rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, કદ (સંરચના ifaddrmsg));

      memcpy (&req->ifa, &ip->ifa, કદ (સંરચના ifaddrmsg));

      req->ifa.ifa_index = ifindex;

      માટે (rta = (સંરચના rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_NEXT (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), buffer_size, rta->rta_type, RTA_DATA (rta), RTA_PAYLOAD (rta), err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }

      ret = send_request (sock, req, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = wait_for_ack (sock, seq, buffer, buffer_size, err);
      જો (ret < 0)
        પાછા ret;
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
request_enable_interface_and_wait (પૂર્ણાંક sock, અક્ષર *buffer, size_t buffer_size, પૂર્ણાંક index, libcrun_error_t *err)
{
  સંરચના nl_req *req = (સંરચના nl_req *) buffer;
  uint32_t seq;
  પૂર્ણાંક ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, કદ (સંરચના ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = index;

  req->ifi.ifi_flags = IFF_UP;
  req->ifi.ifi_change = IFF_UP;

  ret = send_request (sock, req, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા wait_for_ack (sock, seq, buffer, buffer_size, err);
}

સ્થિર પૂર્ણાંક
setup_network_device_in_ns_helper (અક્ષર *buffer, size_t buffer_size, પૂર્ણાંક netns_fd, અચલ અક્ષર *newifname,
                                   સંરચના ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક sock_in_ns = -1;
  પૂર્ણાંક new_ifindex;
  પૂર્ણાંક ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "change network namespace");

  sock_in_ns = open_netlink_fd (err);
  જો (sock_in_ns < 0)
    પાછા sock_in_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_to_index (sock_in_ns, newifname, buffer, buffer_size, err);
  જો (UNLIKELY (new_ifindex < 0))
    પાછા new_ifindex;

  ret = configure_ip_addresses (sock_in_ns, new_ifindex, buffer, buffer_size, ips, err);
  જો (ret < 0)
    પાછા ret;

  પાછા request_enable_interface_and_wait (sock_in_ns, buffer, buffer_size, new_ifindex, err);
}

સ્થિર પૂર્ણાંક
do_move_link_to_ns_and_wait (પૂર્ણાંક sock, અક્ષર *buffer, size_t buffer_size, પૂર્ણાંક ifindex, પૂર્ણાંક netns_fd, અચલ અક્ષર *newifname, libcrun_error_t *err)
{
  સંરચના nl_req *req = (સંરચના nl_req *) buffer;
  uint32_t seq;
  પૂર્ણાંક ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, કદ (સંરચના ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = ifindex;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_NET_NS_FD, &netns_fd, કદ (netns_fd), err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_IFNAME, newifname, strlen (newifname) + 1, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = send_request (sock, req, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા wait_for_ack (sock, seq, buffer, buffer_size, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
સ્થિર પૂર્ણાંક
setup_network_device_in_ns (અક્ષર *buffer, size_t buffer_size, પૂર્ણાંક netns_fd, અચલ અક્ષર *newifname,
                            સંરચના ip_addr *ips, libcrun_error_t *err)
{
  પૂર્ણાંક wait_status;
  pid_t pid;
  પૂર્ણાંક ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  જો (UNLIKELY (pid < 0))
    પાછા crun_make_error (err, errno, "vfork");

  જો (pid == 0)
    {
      ret = setup_network_device_in_ns_helper (buffer, buffer_size, netns_fd, newifname, ips, err);
      જો (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          _safe_exit (-ret);
        }

      _safe_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "waitpid for exec child pid");

  જો (wait_status != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      પાછા -get_process_exit_status (wait_status);
    }

  પાછા 0;
}

પૂર્ણાંક
move_network_device (અચલ અક્ષર *ifname, અચલ અક્ષર *newifname, પૂર્ણાંક netns_fd, libcrun_error_t *err)
{
  અચલ size_t buffer_size = 8192;
  cleanup_ip_addrs સંરચના ip_addr *ips = ખાલી;
  cleanup_free અક્ષર *buffer = xmalloc (buffer_size);
  cleanup_close પૂર્ણાંક sock = -1;
  પૂર્ણાંક ifindex;
  પૂર્ણાંક ret;

  sock = open_netlink_fd (err);
  જો (sock < 0)
    પાછા sock;

  ifindex = name_to_index (sock, ifname, buffer, buffer_size, err);
  જો (UNLIKELY (ifindex < 0))
    પાછા ifindex;

  ret = get_ip_addresses (sock, ifindex, &ips, buffer, buffer_size, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_to_ns_and_wait (sock, buffer, buffer_size, ifindex, netns_fd, newifname, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા setup_network_device_in_ns (buffer, buffer_size, netns_fd, newifname, ips, err);
}
