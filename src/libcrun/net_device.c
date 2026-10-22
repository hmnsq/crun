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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ "net_device.h"
#অন্তর্ভাৱ "utils.h"

#অন্তর্ভাৱ <sys/socket.h>
#অন্তর্ভাৱ <errno.h>
#অন্তর্ভাৱ <sched.h>
#অন্তর্ভাৱ <sys/wait.h>
#অন্তর্ভাৱ <sys/xattr.h>
#অন্তর্ভাৱ <linux/netlink.h>
#অন্তর্ভাৱ <linux/rtnetlink.h>
#অন্তর্ভাৱ <arpa/inet.h>
#অন্তর্ভাৱ <net/if.h>
#যদি HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint চঞ্চল uint
#  endif
#endif

সংরচনা ip_addr
{
  সংরচনা ifaddrmsg ifa;

  পূর্ণাঙ্ক rta_len;
  ৱর্ণ *rta;
};

সংরচনা nl_req
{
  সংরচনা nlmsghdr nlh;
  সঙ্ঘ
  {
    সংরচনা ifaddrmsg ifa;
    সংরচনা ifinfomsg ifi;
  };
};

স্থির atomic_uint nl_seq_counter;

স্থির uint32_t
get_next_seq ()
{
  ঘূৰাই (uint32_t) ++nl_seq_counter;
}

স্থির uint32_t
reset_request (সংরচনা nl_req *req, পূর্ণাঙ্ক type, পূর্ণাঙ্ক flags, পূর্ণাঙ্ক বাৰ্তা_len)
{
  uint32_t seq = get_next_seq ();

  memset (req, 0, পরিমাণ (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_LENGTH (বাৰ্তা_len);
  req->nlh.nlmsg_seq = seq;

  ঘূৰাই seq;
}

স্থির শূন্য
cleanup_ip_addrsp (শূন্য *p)
{
  সংরচনা ip_addr **pp = (সংরচনা ip_addr **) p;
  সংরচনা ip_addr *ip;
  যদি (*pp == রিক্ত)
    ঘূৰাই;

  বাবে (ip = *pp; ip->rta_len >= 0; ip++)
    মুক্ত (ip->rta);

  মুক্ত (*pp);
}

#define cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

স্থির পূর্ণাঙ্ক
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক sock = -1;
  সংরচনা sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  পূর্ণাঙ্ক fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  যদি (sock < 0)
    ঘূৰাই crun_make_error (err, errno, "netlink socket");

  যদি (bind (sock, (সংরচনা sockaddr *) &local, পরিমাণ (local)) < 0)
    ঘূৰাই crun_make_error (err, errno, "bind");

  fd = sock;
  sock = -1;
  ঘূৰাই fd;
}

স্থির পূর্ণাঙ্ক
append_rtattr (সংরচনা nlmsghdr *n, size_t maxlen, পূর্ণাঙ্ক type, ধ্রুৱ শূন্য *data, size_t data_len, libcrun_error_t *err)
{
  পূর্ণাঙ্ক len = RTA_LENGTH (data_len);
  সংরচনা rtattr *rta;

  যদি (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    ঘূৰাই crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (সংরচনা rtattr *) (((ৱর্ণ *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  যদি (data_len)
    memcpy (RTA_DATA (rta), data, data_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
send_request (পূর্ণাঙ্ক sock, সংরচনা nl_req *req, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  ret = TEMP_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "send");

  ঘূৰাই 0;
}

/* if_nametoindex with an open netlink socket.  */
স্থির পূর্ণাঙ্ক
name_to_index (পূর্ণাঙ্ক sock, ধ্রুৱ ৱর্ণ *ifname, ৱর্ণ *buffer, size_t buffer_size, libcrun_error_t *err)
{
  সংরচনা nlmsghdr *nlh;
  সংরচনা nl_req *req;
  uint32_t seq;
  ssize_t len;
  পূর্ণাঙ্ক ret;

  req = (সংরচনা nl_req *) buffer;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, পরিমাণ (সংরচনা ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, buffer_size, IFLA_IFNAME, ifname, strlen (ifname) + 1, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = send_request (sock, req, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0));
  যদি (UNLIKELY (len < 0))
    ঘূৰাই crun_make_error (err, errno, "recv");

  বাবে (nlh = (সংরচনা nlmsghdr *) buffer; NLMSG_OK (nlh, (unsigned পূর্ণাঙ্ক) len); nlh = NLMSG_NEXT (nlh, len))
    {
      যদি (nlh->nlmsg_seq != seq)
        অনুৱর্ত;

      যদি (nlh->nlmsg_type == NLMSG_ERROR)
        {
          সংরচনা nlmsgerr *err_data = (সংরচনা nlmsgerr *) NLMSG_DATA (nlh);
          যদি (err_data->error == 0)
            অনুৱর্ত;
          ঘূৰাই crun_make_error (err, -err_data->error, "netlink error while looking for interface `%s`", ifname);
        }

      যদি (nlh->nlmsg_type == RTM_NEWLINK)
        {
          সংরচনা ifinfomsg *ifi = NLMSG_DATA (nlh);
          ঘূৰাই ifi->ifi_index;
        }
    }

  ঘূৰাই crun_make_error (err, 0, "could not find device `%s`", ifname);
}

স্থির পূর্ণাঙ্ক
wait_for_ack (পূর্ণাঙ্ক sock, uint32_t seq, ৱর্ণ *recv_buffer, size_t recv_buffer_size, libcrun_error_t *err)
{
  সংরচনা nlmsghdr *nlh_recv = (সংরচনা nlmsghdr *) recv_buffer;
  ssize_t len;

  কৰা
    {
      len = TEMP_FAILURE_RETRY (recv (sock, recv_buffer, recv_buffer_size, 0));
      যদি (len < 0)
        ঘূৰাই crun_make_error (err, errno, "recv");
  } যেতিয়ালৈ (nlh_recv->nlmsg_seq != seq);

  যদি (! NLMSG_OK (nlh_recv, len))
    ঘূৰাই crun_make_error (err, 0, "received invalid packet");

  যদি (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      সংরচনা nlmsgerr *err_data = (সংরচনা nlmsgerr *) NLMSG_DATA (nlh_recv);
      যদি (err_data->error == 0)
        ঘূৰাই 0;
      ঘূৰাই crun_make_error (err, -err_data->error, "netlink error");
    }

  ঘূৰাই crun_make_error (err, 0, "internal error: received unknown netlink packet type");
}

স্থির শূন্য
copy_ip_addr (সংরচনা nlmsghdr *nlh, সংরচনা ip_addr *ip)
{
  সংরচনা ifaddrmsg *ifa = (সংরচনা ifaddrmsg *) NLMSG_DATA (nlh);
  memcpy (&ip->ifa, ifa, পরিমাণ (সংরচনা ifaddrmsg));
  ip->rta_len = IFA_PAYLOAD (nlh);
  ip->rta = xmalloc (ip->rta_len);
  memcpy (ip->rta, IFA_RTA (ifa), ip->rta_len);
};

স্থির পূর্ণাঙ্ক
get_ip_addresses (পূর্ণাঙ্ক sock, uint32_t ifindex, সংরচনা ip_addr **out_ips, ৱর্ণ *buffer, size_t buffer_size, libcrun_error_t *err)
{
  সংরচনা nl_req *req = (সংরচনা nl_req *) buffer;
  cleanup_ip_addrs সংরচনা ip_addr *ips = রিক্ত;
  size_t ips_len = 0;
  পূর্ণাঙ্ক optval = 1;
  uint32_t seq;
  ssize_t len;
  পূর্ণাঙ্ক ret;

#ifdef NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, পরিমাণ (optval));
  যদি (ret < 0)
    {
      যদি (errno != ENOPROTOOPT)
        ঘূৰাই crun_make_error (err, errno, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#endif

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, পরিমাণ (সংরচনা ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_index = ifindex;

  ret = send_request (sock, req, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যেতিয়ালৈ ((len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0))) > 0)
    {
      সংরচনা nlmsghdr *nlh;

      বাবে (nlh = (সংরচনা nlmsghdr *) buffer; NLMSG_OK (nlh, len); nlh = NLMSG_NEXT (nlh, len))
        {
          সংরচনা ifaddrmsg *ifa;

          যদি (nlh->nlmsg_seq != seq)
            অনুৱর্ত;

          যদি (nlh->nlmsg_type == NLMSG_DONE)
            {
              *out_ips = ips;
              ips = রিক্ত;
              ঘূৰাই 0;
            }

          যদি (nlh->nlmsg_type == NLMSG_ERROR)
            {
              সংরচনা nlmsgerr *err_data = (সংরচনা nlmsgerr *) NLMSG_DATA (nlh);
              যদি (err_data->error == 0)
                অনুৱর্ত;
              ঘূৰাই crun_make_error (err, -err_data->error, "netlink error reading ip addresses");
            }

          ifa = (সংরচনা ifaddrmsg *) NLMSG_DATA (nlh);
          যদি (ifa->ifa_index != ifindex)
            অনুৱর্ত;

          /* Copy only permanent, globally routable IP addresses.  */
          যদি (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            অনুৱর্ত;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, পরিমাণ (সংরচনা ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  যদি (UNLIKELY (len < 0))
    ঘূৰাই crun_make_error (err, errno, "recv");
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
configure_ip_addresses (পূর্ণাঙ্ক sock, পূর্ণাঙ্ক ifindex, ৱর্ণ *buffer, size_t buffer_size, ধ্রুৱ সংরচনা ip_addr *ips, libcrun_error_t *err)
{
  সংরচনা nl_req *req = (সংরচনা nl_req *) buffer;
  ধ্রুৱ সংরচনা ip_addr *ip;
  পূর্ণাঙ্ক ret;

  যদি (ips == রিক্ত)
    ঘূৰাই 0;

  বাবে (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      পূর্ণাঙ্ক rta_len = ip->rta_len;
      সংরচনা rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, পরিমাণ (সংরচনা ifaddrmsg));

      memcpy (&req->ifa, &ip->ifa, পরিমাণ (সংরচনা ifaddrmsg));

      req->ifa.ifa_index = ifindex;

      বাবে (rta = (সংরচনা rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_NEXT (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), buffer_size, rta->rta_type, RTA_DATA (rta), RTA_PAYLOAD (rta), err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }

      ret = send_request (sock, req, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = wait_for_ack (sock, seq, buffer, buffer_size, err);
      যদি (ret < 0)
        ঘূৰাই ret;
    }

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
request_enable_interface_and_wait (পূর্ণাঙ্ক sock, ৱর্ণ *buffer, size_t buffer_size, পূর্ণাঙ্ক index, libcrun_error_t *err)
{
  সংরচনা nl_req *req = (সংরচনা nl_req *) buffer;
  uint32_t seq;
  পূর্ণাঙ্ক ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, পরিমাণ (সংরচনা ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = index;

  req->ifi.ifi_flags = IFF_UP;
  req->ifi.ifi_change = IFF_UP;

  ret = send_request (sock, req, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ঘূৰাই wait_for_ack (sock, seq, buffer, buffer_size, err);
}

স্থির পূর্ণাঙ্ক
setup_network_device_in_ns_helper (ৱর্ণ *buffer, size_t buffer_size, পূর্ণাঙ্ক netns_fd, ধ্রুৱ ৱর্ণ *newifname,
                                   সংরচনা ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক sock_in_ns = -1;
  পূর্ণাঙ্ক new_ifindex;
  পূর্ণাঙ্ক ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "change network namespace");

  sock_in_ns = open_netlink_fd (err);
  যদি (sock_in_ns < 0)
    ঘূৰাই sock_in_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_to_index (sock_in_ns, newifname, buffer, buffer_size, err);
  যদি (UNLIKELY (new_ifindex < 0))
    ঘূৰাই new_ifindex;

  ret = configure_ip_addresses (sock_in_ns, new_ifindex, buffer, buffer_size, ips, err);
  যদি (ret < 0)
    ঘূৰাই ret;

  ঘূৰাই request_enable_interface_and_wait (sock_in_ns, buffer, buffer_size, new_ifindex, err);
}

স্থির পূর্ণাঙ্ক
do_move_link_to_ns_and_wait (পূর্ণাঙ্ক sock, ৱর্ণ *buffer, size_t buffer_size, পূর্ণাঙ্ক ifindex, পূর্ণাঙ্ক netns_fd, ধ্রুৱ ৱর্ণ *newifname, libcrun_error_t *err)
{
  সংরচনা nl_req *req = (সংরচনা nl_req *) buffer;
  uint32_t seq;
  পূর্ণাঙ্ক ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, পরিমাণ (সংরচনা ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = ifindex;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_NET_NS_FD, &netns_fd, পরিমাণ (netns_fd), err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_IFNAME, newifname, strlen (newifname) + 1, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = send_request (sock, req, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ঘূৰাই wait_for_ack (sock, seq, buffer, buffer_size, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
স্থির পূর্ণাঙ্ক
setup_network_device_in_ns (ৱর্ণ *buffer, size_t buffer_size, পূর্ণাঙ্ক netns_fd, ধ্রুৱ ৱর্ণ *newifname,
                            সংরচনা ip_addr *ips, libcrun_error_t *err)
{
  পূর্ণাঙ্ক wait_status;
  pid_t pid;
  পূর্ণাঙ্ক ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  যদি (UNLIKELY (pid < 0))
    ঘূৰাই crun_make_error (err, errno, "vfork");

  যদি (pid == 0)
    {
      ret = setup_network_device_in_ns_helper (buffer, buffer_size, netns_fd, newifname, ips, err);
      যদি (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          _safe_exit (-ret);
        }

      _safe_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "waitpid for exec child pid");

  যদি (wait_status != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      ঘূৰাই -get_process_exit_status (wait_status);
    }

  ঘূৰাই 0;
}

পূর্ণাঙ্ক
move_network_device (ধ্রুৱ ৱর্ণ *ifname, ধ্রুৱ ৱর্ণ *newifname, পূর্ণাঙ্ক netns_fd, libcrun_error_t *err)
{
  ধ্রুৱ size_t buffer_size = 8192;
  cleanup_ip_addrs সংরচনা ip_addr *ips = রিক্ত;
  cleanup_free ৱর্ণ *buffer = xmalloc (buffer_size);
  cleanup_close পূর্ণাঙ্ক sock = -1;
  পূর্ণাঙ্ক ifindex;
  পূর্ণাঙ্ক ret;

  sock = open_netlink_fd (err);
  যদি (sock < 0)
    ঘূৰাই sock;

  ifindex = name_to_index (sock, ifname, buffer, buffer_size, err);
  যদি (UNLIKELY (ifindex < 0))
    ঘূৰাই ifindex;

  ret = get_ip_addresses (sock, ifindex, &ips, buffer, buffer_size, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_to_ns_and_wait (sock, buffer, buffer_size, ifindex, netns_fd, newifname, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ঘূৰাই setup_network_device_in_ns (buffer, buffer_size, netns_fd, newifname, ips, err);
}
