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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "net_device.h"
#அந்தர்பாவ "utils.h"

#அந்தர்பாவ <sys/socket.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <sched.h>
#அந்தர்பாவ <sys/wait.h>
#அந்தர்பாவ <sys/xattr.h>
#அந்தர்பாவ <linux/netlink.h>
#அந்தர்பாவ <linux/rtnetlink.h>
#அந்தர்பாவ <arpa/inet.h>
#அந்தர்பாவ <net/if.h>
#எனில் HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint நிலையற்ற uint
#  endif
#endif

கட்டமைப்பு ip_addr
{
  கட்டமைப்பு ifaddrmsg ifa;

  முழு rta_len;
  எழுத்து *rta;
};

கட்டமைப்பு nl_req
{
  கட்டமைப்பு nlmsghdr nlh;
  ஒன்றிப்பு
  {
    கட்டமைப்பு ifaddrmsg ifa;
    கட்டமைப்பு ifinfomsg ifi;
  };
};

நிலையான atomic_uint nl_seq_counter;

நிலையான uint32_t
get_next_seq ()
{
  திரும்பு (uint32_t) ++nl_seq_counter;
}

நிலையான uint32_t
reset_request (கட்டமைப்பு nl_req *req, முழு type, முழு flags, முழு செய்தி_len)
{
  uint32_t seq = get_next_seq ();

  memset (req, 0, அளவு (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_LENGTH (செய்தி_len);
  req->nlh.nlmsg_seq = seq;

  திரும்பு seq;
}

நிலையான வெறுமை
cleanup_ip_addrsp (வெறுமை *p)
{
  கட்டமைப்பு ip_addr **pp = (கட்டமைப்பு ip_addr **) p;
  கட்டமைப்பு ip_addr *ip;
  எனில் (*pp == வெற்று)
    திரும்பு;

  க்கு (ip = *pp; ip->rta_len >= 0; ip++)
    விடுவி (ip->rta);

  விடுவி (*pp);
}

#define cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

நிலையான முழு
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close முழு sock = -1;
  கட்டமைப்பு sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  முழு fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  எனில் (sock < 0)
    திரும்பு crun_make_error (err, errno, "netlink socket");

  எனில் (bind (sock, (கட்டமைப்பு sockaddr *) &local, அளவு (local)) < 0)
    திரும்பு crun_make_error (err, errno, "bind");

  fd = sock;
  sock = -1;
  திரும்பு fd;
}

நிலையான முழு
append_rtattr (கட்டமைப்பு nlmsghdr *n, size_t maxlen, முழு type, மாறிலி வெறுமை *data, size_t data_len, libcrun_error_t *err)
{
  முழு len = RTA_LENGTH (data_len);
  கட்டமைப்பு rtattr *rta;

  எனில் (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    திரும்பு crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (கட்டமைப்பு rtattr *) (((எழுத்து *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  எனில் (data_len)
    memcpy (RTA_DATA (rta), data, data_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  திரும்பு 0;
}

நிலையான முழு
send_request (முழு sock, கட்டமைப்பு nl_req *req, libcrun_error_t *err)
{
  முழு ret;

  ret = TEMP_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "send");

  திரும்பு 0;
}

/* if_nametoindex with an open netlink socket.  */
நிலையான முழு
name_to_index (முழு sock, மாறிலி எழுத்து *ifname, எழுத்து *buffer, size_t buffer_size, libcrun_error_t *err)
{
  கட்டமைப்பு nlmsghdr *nlh;
  கட்டமைப்பு nl_req *req;
  uint32_t seq;
  ssize_t len;
  முழு ret;

  req = (கட்டமைப்பு nl_req *) buffer;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, அளவு (கட்டமைப்பு ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, buffer_size, IFLA_IFNAME, ifname, strlen (ifname) + 1, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = send_request (sock, req, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0));
  எனில் (UNLIKELY (len < 0))
    திரும்பு crun_make_error (err, errno, "recv");

  க்கு (nlh = (கட்டமைப்பு nlmsghdr *) buffer; NLMSG_OK (nlh, (unsigned முழு) len); nlh = NLMSG_NEXT (nlh, len))
    {
      எனில் (nlh->nlmsg_seq != seq)
        தொடர்;

      எனில் (nlh->nlmsg_type == NLMSG_ERROR)
        {
          கட்டமைப்பு nlmsgerr *err_data = (கட்டமைப்பு nlmsgerr *) NLMSG_DATA (nlh);
          எனில் (err_data->error == 0)
            தொடர்;
          திரும்பு crun_make_error (err, -err_data->error, "netlink error while looking for interface `%s`", ifname);
        }

      எனில் (nlh->nlmsg_type == RTM_NEWLINK)
        {
          கட்டமைப்பு ifinfomsg *ifi = NLMSG_DATA (nlh);
          திரும்பு ifi->ifi_index;
        }
    }

  திரும்பு crun_make_error (err, 0, "could not find device `%s`", ifname);
}

நிலையான முழு
wait_for_ack (முழு sock, uint32_t seq, எழுத்து *recv_buffer, size_t recv_buffer_size, libcrun_error_t *err)
{
  கட்டமைப்பு nlmsghdr *nlh_recv = (கட்டமைப்பு nlmsghdr *) recv_buffer;
  ssize_t len;

  செய்
    {
      len = TEMP_FAILURE_RETRY (recv (sock, recv_buffer, recv_buffer_size, 0));
      எனில் (len < 0)
        திரும்பு crun_make_error (err, errno, "recv");
  } போது (nlh_recv->nlmsg_seq != seq);

  எனில் (! NLMSG_OK (nlh_recv, len))
    திரும்பு crun_make_error (err, 0, "received invalid packet");

  எனில் (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      கட்டமைப்பு nlmsgerr *err_data = (கட்டமைப்பு nlmsgerr *) NLMSG_DATA (nlh_recv);
      எனில் (err_data->error == 0)
        திரும்பு 0;
      திரும்பு crun_make_error (err, -err_data->error, "netlink error");
    }

  திரும்பு crun_make_error (err, 0, "internal error: received unknown netlink packet type");
}

நிலையான வெறுமை
copy_ip_addr (கட்டமைப்பு nlmsghdr *nlh, கட்டமைப்பு ip_addr *ip)
{
  கட்டமைப்பு ifaddrmsg *ifa = (கட்டமைப்பு ifaddrmsg *) NLMSG_DATA (nlh);
  memcpy (&ip->ifa, ifa, அளவு (கட்டமைப்பு ifaddrmsg));
  ip->rta_len = IFA_PAYLOAD (nlh);
  ip->rta = xmalloc (ip->rta_len);
  memcpy (ip->rta, IFA_RTA (ifa), ip->rta_len);
};

நிலையான முழு
get_ip_addresses (முழு sock, uint32_t ifindex, கட்டமைப்பு ip_addr **out_ips, எழுத்து *buffer, size_t buffer_size, libcrun_error_t *err)
{
  கட்டமைப்பு nl_req *req = (கட்டமைப்பு nl_req *) buffer;
  cleanup_ip_addrs கட்டமைப்பு ip_addr *ips = வெற்று;
  size_t ips_len = 0;
  முழு optval = 1;
  uint32_t seq;
  ssize_t len;
  முழு ret;

#ifdef NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, அளவு (optval));
  எனில் (ret < 0)
    {
      எனில் (errno != ENOPROTOOPT)
        திரும்பு crun_make_error (err, errno, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#endif

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, அளவு (கட்டமைப்பு ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_index = ifindex;

  ret = send_request (sock, req, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  போது ((len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0))) > 0)
    {
      கட்டமைப்பு nlmsghdr *nlh;

      க்கு (nlh = (கட்டமைப்பு nlmsghdr *) buffer; NLMSG_OK (nlh, len); nlh = NLMSG_NEXT (nlh, len))
        {
          கட்டமைப்பு ifaddrmsg *ifa;

          எனில் (nlh->nlmsg_seq != seq)
            தொடர்;

          எனில் (nlh->nlmsg_type == NLMSG_DONE)
            {
              *out_ips = ips;
              ips = வெற்று;
              திரும்பு 0;
            }

          எனில் (nlh->nlmsg_type == NLMSG_ERROR)
            {
              கட்டமைப்பு nlmsgerr *err_data = (கட்டமைப்பு nlmsgerr *) NLMSG_DATA (nlh);
              எனில் (err_data->error == 0)
                தொடர்;
              திரும்பு crun_make_error (err, -err_data->error, "netlink error reading ip addresses");
            }

          ifa = (கட்டமைப்பு ifaddrmsg *) NLMSG_DATA (nlh);
          எனில் (ifa->ifa_index != ifindex)
            தொடர்;

          /* Copy only permanent, globally routable IP addresses.  */
          எனில் (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            தொடர்;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, அளவு (கட்டமைப்பு ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  எனில் (UNLIKELY (len < 0))
    திரும்பு crun_make_error (err, errno, "recv");
  திரும்பு 0;
}

நிலையான முழு
configure_ip_addresses (முழு sock, முழு ifindex, எழுத்து *buffer, size_t buffer_size, மாறிலி கட்டமைப்பு ip_addr *ips, libcrun_error_t *err)
{
  கட்டமைப்பு nl_req *req = (கட்டமைப்பு nl_req *) buffer;
  மாறிலி கட்டமைப்பு ip_addr *ip;
  முழு ret;

  எனில் (ips == வெற்று)
    திரும்பு 0;

  க்கு (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      முழு rta_len = ip->rta_len;
      கட்டமைப்பு rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, அளவு (கட்டமைப்பு ifaddrmsg));

      memcpy (&req->ifa, &ip->ifa, அளவு (கட்டமைப்பு ifaddrmsg));

      req->ifa.ifa_index = ifindex;

      க்கு (rta = (கட்டமைப்பு rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_NEXT (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), buffer_size, rta->rta_type, RTA_DATA (rta), RTA_PAYLOAD (rta), err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }

      ret = send_request (sock, req, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = wait_for_ack (sock, seq, buffer, buffer_size, err);
      எனில் (ret < 0)
        திரும்பு ret;
    }

  திரும்பு 0;
}

நிலையான முழு
request_enable_interface_and_wait (முழு sock, எழுத்து *buffer, size_t buffer_size, முழு index, libcrun_error_t *err)
{
  கட்டமைப்பு nl_req *req = (கட்டமைப்பு nl_req *) buffer;
  uint32_t seq;
  முழு ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, அளவு (கட்டமைப்பு ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = index;

  req->ifi.ifi_flags = IFF_UP;
  req->ifi.ifi_change = IFF_UP;

  ret = send_request (sock, req, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு wait_for_ack (sock, seq, buffer, buffer_size, err);
}

நிலையான முழு
setup_network_device_in_ns_helper (எழுத்து *buffer, size_t buffer_size, முழு netns_fd, மாறிலி எழுத்து *newifname,
                                   கட்டமைப்பு ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close முழு sock_in_ns = -1;
  முழு new_ifindex;
  முழு ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "change network namespace");

  sock_in_ns = open_netlink_fd (err);
  எனில் (sock_in_ns < 0)
    திரும்பு sock_in_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_to_index (sock_in_ns, newifname, buffer, buffer_size, err);
  எனில் (UNLIKELY (new_ifindex < 0))
    திரும்பு new_ifindex;

  ret = configure_ip_addresses (sock_in_ns, new_ifindex, buffer, buffer_size, ips, err);
  எனில் (ret < 0)
    திரும்பு ret;

  திரும்பு request_enable_interface_and_wait (sock_in_ns, buffer, buffer_size, new_ifindex, err);
}

நிலையான முழு
do_move_link_to_ns_and_wait (முழு sock, எழுத்து *buffer, size_t buffer_size, முழு ifindex, முழு netns_fd, மாறிலி எழுத்து *newifname, libcrun_error_t *err)
{
  கட்டமைப்பு nl_req *req = (கட்டமைப்பு nl_req *) buffer;
  uint32_t seq;
  முழு ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, அளவு (கட்டமைப்பு ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = ifindex;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_NET_NS_FD, &netns_fd, அளவு (netns_fd), err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_IFNAME, newifname, strlen (newifname) + 1, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = send_request (sock, req, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு wait_for_ack (sock, seq, buffer, buffer_size, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
நிலையான முழு
setup_network_device_in_ns (எழுத்து *buffer, size_t buffer_size, முழு netns_fd, மாறிலி எழுத்து *newifname,
                            கட்டமைப்பு ip_addr *ips, libcrun_error_t *err)
{
  முழு wait_status;
  pid_t pid;
  முழு ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  எனில் (UNLIKELY (pid < 0))
    திரும்பு crun_make_error (err, errno, "vfork");

  எனில் (pid == 0)
    {
      ret = setup_network_device_in_ns_helper (buffer, buffer_size, netns_fd, newifname, ips, err);
      எனில் (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          _safe_exit (-ret);
        }

      _safe_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "waitpid for exec child pid");

  எனில் (wait_status != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      திரும்பு -get_process_exit_status (wait_status);
    }

  திரும்பு 0;
}

முழு
move_network_device (மாறிலி எழுத்து *ifname, மாறிலி எழுத்து *newifname, முழு netns_fd, libcrun_error_t *err)
{
  மாறிலி size_t buffer_size = 8192;
  cleanup_ip_addrs கட்டமைப்பு ip_addr *ips = வெற்று;
  cleanup_free எழுத்து *buffer = xmalloc (buffer_size);
  cleanup_close முழு sock = -1;
  முழு ifindex;
  முழு ret;

  sock = open_netlink_fd (err);
  எனில் (sock < 0)
    திரும்பு sock;

  ifindex = name_to_index (sock, ifname, buffer, buffer_size, err);
  எனில் (UNLIKELY (ifindex < 0))
    திரும்பு ifindex;

  ret = get_ip_addresses (sock, ifindex, &ips, buffer, buffer_size, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_to_ns_and_wait (sock, buffer, buffer_size, ifindex, netns_fd, newifname, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு setup_network_device_in_ns (buffer, buffer_size, netns_fd, newifname, ips, err);
}
