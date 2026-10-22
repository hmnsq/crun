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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "net_device.h"
#অন্তর্ভাব "utils.h"

#অন্তর্ভাব <sys/socket.h>
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <sched.h>
#অন্তর্ভাব <sys/wait.h>
#অন্তর্ভাব <sys/xattr.h>
#অন্তর্ভাব <linux/netlink.h>
#অন্তর্ভাব <linux/rtnetlink.h>
#অন্তর্ভাব <arpa/inet.h>
#অন্তর্ভাব <net/if.h>
#যদি HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint অস্থির uint
#  endif
#endif

কাঠামো ip_addr
{
  কাঠামো ifaddrmsg ifa;

  পূর্ণসংখ্যা rta_len;
  অক্ষর *rta;
};

কাঠামো nl_req
{
  কাঠামো nlmsghdr nlh;
  সংঘ
  {
    কাঠামো ifaddrmsg ifa;
    কাঠামো ifinfomsg ifi;
  };
};

স্থির atomic_uint nl_seq_গণক;

স্থির uint32_t
get_পরবর্তী_seq ()
{
  ফেরত (uint32_t) ++nl_seq_গণক;
}

স্থির uint32_t
reset_request (কাঠামো nl_req *req, পূর্ণসংখ্যা type, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা বার্তা_len)
{
  uint32_t seq = get_পরবর্তী_seq ();

  memset (req, 0, আকার (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_দৈর্ঘ্য (বার্তা_len);
  req->nlh.nlmsg_seq = seq;

  ফেরত seq;
}

স্থির শূন্য
cleanup_ip_addrsp (শূন্য *p)
{
  কাঠামো ip_addr **pp = (কাঠামো ip_addr **) p;
  কাঠামো ip_addr *ip;
  যদি (*pp == ফাঁকা)
    ফেরত;

  জন্য (ip = *pp; ip->rta_len >= 0; ip++)
    মুক্ত (ip->rta);

  মুক্ত (*pp);
}

#define cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

স্থির পূর্ণসংখ্যা
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close পূর্ণসংখ্যা sock = -1;
  কাঠামো sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  পূর্ণসংখ্যা fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  যদি (sock < 0)
    ফেরত crun_make_error (err, errno, "netlink socket");

  যদি (bind (sock, (কাঠামো sockaddr *) &local, আকার (local)) < 0)
    ফেরত crun_make_error (err, errno, "bind");

  fd = sock;
  sock = -1;
  ফেরত fd;
}

স্থির পূর্ণসংখ্যা
append_rtattr (কাঠামো nlmsghdr *n, size_t maxlen, পূর্ণসংখ্যা type, ধ্রুবক শূন্য *তথ্য, size_t তথ্য_len, libcrun_error_t *err)
{
  পূর্ণসংখ্যা len = RTA_দৈর্ঘ্য (তথ্য_len);
  কাঠামো rtattr *rta;

  যদি (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    ফেরত crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (কাঠামো rtattr *) (((অক্ষর *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  যদি (তথ্য_len)
    memcpy (RTA_তথ্য (rta), তথ্য, তথ্য_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
send_request (পূর্ণসংখ্যা sock, কাঠামো nl_req *req, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;

  ret = অস্থায়ী_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "send");

  ফেরত 0;
}

/* if_nametoindex with an open netlink socket.  */
স্থির পূর্ণসংখ্যা
name_to_সূচক (পূর্ণসংখ্যা sock, ধ্রুবক অক্ষর *ifname, অক্ষর *বাফার, size_t বাফার_আকার, libcrun_error_t *err)
{
  কাঠামো nlmsghdr *nlh;
  কাঠামো nl_req *req;
  uint32_t seq;
  ssize_t len;
  পূর্ণসংখ্যা ret;

  req = (কাঠামো nl_req *) বাফার;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, আকার (কাঠামো ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, বাফার_আকার, IFLA_IFNAME, ifname, strlen (ifname) + 1, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = send_request (sock, req, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  len = অস্থায়ী_FAILURE_RETRY (recv (sock, বাফার, বাফার_আকার, 0));
  যদি (UNLIKELY (len < 0))
    ফেরত crun_make_error (err, errno, "recv");

  জন্য (nlh = (কাঠামো nlmsghdr *) বাফার; NLMSG_OK (nlh, (unsigned পূর্ণসংখ্যা) len); nlh = NLMSG_পরবর্তী (nlh, len))
    {
      যদি (nlh->nlmsg_seq != seq)
        চালু;

      যদি (nlh->nlmsg_type == NLMSG_ERROR)
        {
          কাঠামো nlmsgerr *err_তথ্য = (কাঠামো nlmsgerr *) NLMSG_তথ্য (nlh);
          যদি (err_তথ্য->error == 0)
            চালু;
          ফেরত crun_make_error (err, -err_তথ্য->error, "netlink error while looking for interface `%s`", ifname);
        }

      যদি (nlh->nlmsg_type == RTM_NEWLINK)
        {
          কাঠামো ifinfomsg *ifi = NLMSG_তথ্য (nlh);
          ফেরত ifi->ifi_সূচক;
        }
    }

  ফেরত crun_make_error (err, 0, "could not find device `%s`", ifname);
}

স্থির পূর্ণসংখ্যা
wait_for_ack (পূর্ণসংখ্যা sock, uint32_t seq, অক্ষর *recv_বাফার, size_t recv_বাফার_আকার, libcrun_error_t *err)
{
  কাঠামো nlmsghdr *nlh_recv = (কাঠামো nlmsghdr *) recv_বাফার;
  ssize_t len;

  করো
    {
      len = অস্থায়ী_FAILURE_RETRY (recv (sock, recv_বাফার, recv_বাফার_আকার, 0));
      যদি (len < 0)
        ফেরত crun_make_error (err, errno, "recv");
  } যখন (nlh_recv->nlmsg_seq != seq);

  যদি (! NLMSG_OK (nlh_recv, len))
    ফেরত crun_make_error (err, 0, "received invalid packet");

  যদি (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      কাঠামো nlmsgerr *err_তথ্য = (কাঠামো nlmsgerr *) NLMSG_তথ্য (nlh_recv);
      যদি (err_তথ্য->error == 0)
        ফেরত 0;
      ফেরত crun_make_error (err, -err_তথ্য->error, "netlink error");
    }

  ফেরত crun_make_error (err, 0, "internal error: received unknown netlink packet type");
}

স্থির শূন্য
copy_ip_addr (কাঠামো nlmsghdr *nlh, কাঠামো ip_addr *ip)
{
  কাঠামো ifaddrmsg *ifa = (কাঠামো ifaddrmsg *) NLMSG_তথ্য (nlh);
  memcpy (&ip->ifa, ifa, আকার (কাঠামো ifaddrmsg));
  ip->rta_len = IFA_PAYLOAD (nlh);
  ip->rta = xmalloc (ip->rta_len);
  memcpy (ip->rta, IFA_RTA (ifa), ip->rta_len);
};

স্থির পূর্ণসংখ্যা
get_ip_addresses (পূর্ণসংখ্যা sock, uint32_t ifindex, কাঠামো ip_addr **out_ips, অক্ষর *বাফার, size_t বাফার_আকার, libcrun_error_t *err)
{
  কাঠামো nl_req *req = (কাঠামো nl_req *) বাফার;
  cleanup_ip_addrs কাঠামো ip_addr *ips = ফাঁকা;
  size_t ips_len = 0;
  পূর্ণসংখ্যা optval = 1;
  uint32_t seq;
  ssize_t len;
  পূর্ণসংখ্যা ret;

#ifdef NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, আকার (optval));
  যদি (ret < 0)
    {
      যদি (errno != ENOPROTOOPT)
        ফেরত crun_make_error (err, errno, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#endif

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, আকার (কাঠামো ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_সূচক = ifindex;

  ret = send_request (sock, req, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  যখন ((len = অস্থায়ী_FAILURE_RETRY (recv (sock, বাফার, বাফার_আকার, 0))) > 0)
    {
      কাঠামো nlmsghdr *nlh;

      জন্য (nlh = (কাঠামো nlmsghdr *) বাফার; NLMSG_OK (nlh, len); nlh = NLMSG_পরবর্তী (nlh, len))
        {
          কাঠামো ifaddrmsg *ifa;

          যদি (nlh->nlmsg_seq != seq)
            চালু;

          যদি (nlh->nlmsg_type == NLMSG_DONE)
            {
              *out_ips = ips;
              ips = ফাঁকা;
              ফেরত 0;
            }

          যদি (nlh->nlmsg_type == NLMSG_ERROR)
            {
              কাঠামো nlmsgerr *err_তথ্য = (কাঠামো nlmsgerr *) NLMSG_তথ্য (nlh);
              যদি (err_তথ্য->error == 0)
                চালু;
              ফেরত crun_make_error (err, -err_তথ্য->error, "netlink error reading ip addresses");
            }

          ifa = (কাঠামো ifaddrmsg *) NLMSG_তথ্য (nlh);
          যদি (ifa->ifa_সূচক != ifindex)
            চালু;

          /* Copy only permanent, globally routable IP addresses.  */
          যদি (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            চালু;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, আকার (কাঠামো ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  যদি (UNLIKELY (len < 0))
    ফেরত crun_make_error (err, errno, "recv");
  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
configure_ip_addresses (পূর্ণসংখ্যা sock, পূর্ণসংখ্যা ifindex, অক্ষর *বাফার, size_t বাফার_আকার, ধ্রুবক কাঠামো ip_addr *ips, libcrun_error_t *err)
{
  কাঠামো nl_req *req = (কাঠামো nl_req *) বাফার;
  ধ্রুবক কাঠামো ip_addr *ip;
  পূর্ণসংখ্যা ret;

  যদি (ips == ফাঁকা)
    ফেরত 0;

  জন্য (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      পূর্ণসংখ্যা rta_len = ip->rta_len;
      কাঠামো rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, আকার (কাঠামো ifaddrmsg));

      memcpy (&req->ifa, &ip->ifa, আকার (কাঠামো ifaddrmsg));

      req->ifa.ifa_সূচক = ifindex;

      জন্য (rta = (কাঠামো rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_পরবর্তী (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), বাফার_আকার, rta->rta_type, RTA_তথ্য (rta), RTA_PAYLOAD (rta), err);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
        }

      ret = send_request (sock, req, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      ret = wait_for_ack (sock, seq, বাফার, বাফার_আকার, err);
      যদি (ret < 0)
        ফেরত ret;
    }

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
request_enable_interface_and_wait (পূর্ণসংখ্যা sock, অক্ষর *বাফার, size_t বাফার_আকার, পূর্ণসংখ্যা সূচক, libcrun_error_t *err)
{
  কাঠামো nl_req *req = (কাঠামো nl_req *) বাফার;
  uint32_t seq;
  পূর্ণসংখ্যা ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, আকার (কাঠামো ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_সূচক = সূচক;

  req->ifi.ifi_flags = IFF_UP;
  req->ifi.ifi_change = IFF_UP;

  ret = send_request (sock, req, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ফেরত wait_for_ack (sock, seq, বাফার, বাফার_আকার, err);
}

স্থির পূর্ণসংখ্যা
setup_network_device_in_ns_helper (অক্ষর *বাফার, size_t বাফার_আকার, পূর্ণসংখ্যা netns_fd, ধ্রুবক অক্ষর *newifname,
                                   কাঠামো ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close পূর্ণসংখ্যা sock_in_ns = -1;
  পূর্ণসংখ্যা new_ifindex;
  পূর্ণসংখ্যা ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "change network namespace");

  sock_in_ns = open_netlink_fd (err);
  যদি (sock_in_ns < 0)
    ফেরত sock_in_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_to_সূচক (sock_in_ns, newifname, বাফার, বাফার_আকার, err);
  যদি (UNLIKELY (new_ifindex < 0))
    ফেরত new_ifindex;

  ret = configure_ip_addresses (sock_in_ns, new_ifindex, বাফার, বাফার_আকার, ips, err);
  যদি (ret < 0)
    ফেরত ret;

  ফেরত request_enable_interface_and_wait (sock_in_ns, বাফার, বাফার_আকার, new_ifindex, err);
}

স্থির পূর্ণসংখ্যা
do_move_link_to_ns_and_wait (পূর্ণসংখ্যা sock, অক্ষর *বাফার, size_t বাফার_আকার, পূর্ণসংখ্যা ifindex, পূর্ণসংখ্যা netns_fd, ধ্রুবক অক্ষর *newifname, libcrun_error_t *err)
{
  কাঠামো nl_req *req = (কাঠামো nl_req *) বাফার;
  uint32_t seq;
  পূর্ণসংখ্যা ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, আকার (কাঠামো ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_সূচক = ifindex;

  ret = append_rtattr (&req->nlh, বাফার_আকার, IFLA_NET_NS_FD, &netns_fd, আকার (netns_fd), err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = append_rtattr (&req->nlh, বাফার_আকার, IFLA_IFNAME, newifname, strlen (newifname) + 1, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = send_request (sock, req, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ফেরত wait_for_ack (sock, seq, বাফার, বাফার_আকার, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
স্থির পূর্ণসংখ্যা
setup_network_device_in_ns (অক্ষর *বাফার, size_t বাফার_আকার, পূর্ণসংখ্যা netns_fd, ধ্রুবক অক্ষর *newifname,
                            কাঠামো ip_addr *ips, libcrun_error_t *err)
{
  পূর্ণসংখ্যা wait_status;
  pid_t pid;
  পূর্ণসংখ্যা ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  যদি (UNLIKELY (pid < 0))
    ফেরত crun_make_error (err, errno, "vfork");

  যদি (pid == 0)
    {
      ret = setup_network_device_in_ns_helper (বাফার, বাফার_আকার, netns_fd, newifname, ips, err);
      যদি (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          _safe_exit (-ret);
        }

      _safe_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "waitpid for exec child pid");

  যদি (wait_status != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      ফেরত -get_process_exit_status (wait_status);
    }

  ফেরত 0;
}

পূর্ণসংখ্যা
move_network_device (ধ্রুবক অক্ষর *ifname, ধ্রুবক অক্ষর *newifname, পূর্ণসংখ্যা netns_fd, libcrun_error_t *err)
{
  ধ্রুবক size_t বাফার_আকার = 8192;
  cleanup_ip_addrs কাঠামো ip_addr *ips = ফাঁকা;
  cleanup_free অক্ষর *বাফার = xmalloc (বাফার_আকার);
  cleanup_close পূর্ণসংখ্যা sock = -1;
  পূর্ণসংখ্যা ifindex;
  পূর্ণসংখ্যা ret;

  sock = open_netlink_fd (err);
  যদি (sock < 0)
    ফেরত sock;

  ifindex = name_to_সূচক (sock, ifname, বাফার, বাফার_আকার, err);
  যদি (UNLIKELY (ifindex < 0))
    ফেরত ifindex;

  ret = get_ip_addresses (sock, ifindex, &ips, বাফার, বাফার_আকার, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_to_ns_and_wait (sock, বাফার, বাফার_আকার, ifindex, netns_fd, newifname, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ফেরত setup_network_device_in_ns (বাফার, বাফার_আকার, netns_fd, newifname, ips, err);
}
