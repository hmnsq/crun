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

#تعریف_کرو GNU_ماخذ

#شامل <config.h>
#شامل "net_device.h"
#شامل "utils.h"

#شامل <sys/socket.h>
#شامل <errno.h>
#شامل <sched.h>
#شامل <sys/wait.h>
#شامل <sys/xattr.h>
#شامل <linux/netlink.h>
#شامل <linux/rtnetlink.h>
#شامل <arpa/inet.h>
#شامل <net/if.h>
#اگر HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint متلون uint
#  endif
#اگر_ختم

ساخت ip_addr
{
  ساخت ifaddrmsg ifa;

  صحیح rta_len;
  حرف *rta;
};

ساخت nl_req
{
  ساخت nlmsghdr nlh;
  اتحاد
  {
    ساخت ifaddrmsg ifa;
    ساخت ifinfomsg ifi;
  };
};

ساکن atomic_uint nl_seq_شمار;

ساکن uint32_t
get_اگلا_seq ()
{
  واپس (uint32_t) ++nl_seq_شمار;
}

ساکن uint32_t
reset_request (ساخت nl_req *req, صحیح type, صحیح flags, صحیح پیغام_len)
{
  uint32_t seq = get_اگلا_seq ();

  memset (req, 0, حجم (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_لمبائی (پیغام_len);
  req->nlh.nlmsg_seq = seq;

  واپس seq;
}

ساکن خالی
cleanup_ip_addrsp (خالی *p)
{
  ساخت ip_addr **pp = (ساخت ip_addr **) p;
  ساخت ip_addr *ip;
  اگر (*pp == تہی)
    واپس;

  برائے (ip = *pp; ip->rta_len >= 0; ip++)
    آزاد (ip->rta);

  آزاد (*pp);
}

#تعریف_کرو cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

ساکن صحیح
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close صحیح sock = -1;
  ساخت sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  صحیح fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  اگر (sock < 0)
    واپس crun_make_error (err, errno, "netlink socket");

  اگر (bind (sock, (ساخت sockaddr *) &local, حجم (local)) < 0)
    واپس crun_make_error (err, errno, "bind");

  fd = sock;
  sock = -1;
  واپس fd;
}

ساکن صحیح
append_rtattr (ساخت nlmsghdr *n, size_t maxlen, صحیح type, ثابت خالی *ڈیٹا, size_t ڈیٹا_len, libcrun_error_t *err)
{
  صحیح len = RTA_لمبائی (ڈیٹا_len);
  ساخت rtattr *rta;

  اگر (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    واپس crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (ساخت rtattr *) (((حرف *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  اگر (ڈیٹا_len)
    حافظہ_نقل (RTA_ڈیٹا (rta), ڈیٹا, ڈیٹا_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  واپس 0;
}

ساکن صحیح
send_request (صحیح sock, ساخت nl_req *req, libcrun_error_t *err)
{
  صحیح ret;

  ret = عارضی_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "send");

  واپس 0;
}

/* if_nametoindex with an open netlink socket.  */
ساکن صحیح
name_مقصد_اشاریہ (صحیح sock, ثابت حرف *ifname, حرف *بفر, size_t بفر_سائز, libcrun_error_t *err)
{
  ساخت nlmsghdr *nlh;
  ساخت nl_req *req;
  uint32_t seq;
  ssize_t len;
  صحیح ret;

  req = (ساخت nl_req *) بفر;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, حجم (ساخت ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, بفر_سائز, IFLA_IFNAME, ifname, لڑی_لمبائی (ifname) + 1, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = send_request (sock, req, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  len = عارضی_FAILURE_RETRY (recv (sock, بفر, بفر_سائز, 0));
  اگر (UNLIKELY (len < 0))
    واپس crun_make_error (err, errno, "recv");

  برائے (nlh = (ساخت nlmsghdr *) بفر; NLMSG_OK (nlh, (غیرعلامتی صحیح) len); nlh = NLMSG_اگلا (nlh, len))
    {
      اگر (nlh->nlmsg_seq != seq)
        جاری;

      اگر (nlh->nlmsg_type == NLMSG_ERROR)
        {
          ساخت nlmsgerr *err_ڈیٹا = (ساخت nlmsgerr *) NLMSG_ڈیٹا (nlh);
          اگر (err_ڈیٹا->error == 0)
            جاری;
          واپس crun_make_error (err, -err_ڈیٹا->error, "netlink error while looking for interface `%s`", ifname);
        }

      اگر (nlh->nlmsg_type == RTM_NEWLINK)
        {
          ساخت ifinfomsg *ifi = NLMSG_ڈیٹا (nlh);
          واپس ifi->ifi_اشاریہ;
        }
    }

  واپس crun_make_error (err, 0, "could not find device `%s`", ifname);
}

ساکن صحیح
wait_for_ack (صحیح sock, uint32_t seq, حرف *recv_بفر, size_t recv_بفر_سائز, libcrun_error_t *err)
{
  ساخت nlmsghdr *nlh_recv = (ساخت nlmsghdr *) recv_بفر;
  ssize_t len;

  کرو
    {
      len = عارضی_FAILURE_RETRY (recv (sock, recv_بفر, recv_بفر_سائز, 0));
      اگر (len < 0)
        واپس crun_make_error (err, errno, "recv");
  } جب (nlh_recv->nlmsg_seq != seq);

  اگر (! NLMSG_OK (nlh_recv, len))
    واپس crun_make_error (err, 0, "received invalid packet");

  اگر (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      ساخت nlmsgerr *err_ڈیٹا = (ساخت nlmsgerr *) NLMSG_ڈیٹا (nlh_recv);
      اگر (err_ڈیٹا->error == 0)
        واپس 0;
      واپس crun_make_error (err, -err_ڈیٹا->error, "netlink error");
    }

  واپس crun_make_error (err, 0, "internal error: received unknown netlink packet type");
}

ساکن خالی
copy_ip_addr (ساخت nlmsghdr *nlh, ساخت ip_addr *ip)
{
  ساخت ifaddrmsg *ifa = (ساخت ifaddrmsg *) NLMSG_ڈیٹا (nlh);
  حافظہ_نقل (&ip->ifa, ifa, حجم (ساخت ifaddrmsg));
  ip->rta_len = IFA_PAYLOAD (nlh);
  ip->rta = xmalloc (ip->rta_len);
  حافظہ_نقل (ip->rta, IFA_RTA (ifa), ip->rta_len);
};

ساکن صحیح
get_ip_پتے (صحیح sock, uint32_t ifindex, ساخت ip_addr **برآمد_ips, حرف *بفر, size_t بفر_سائز, libcrun_error_t *err)
{
  ساخت nl_req *req = (ساخت nl_req *) بفر;
  cleanup_ip_addrs ساخت ip_addr *ips = تہی;
  size_t ips_len = 0;
  صحیح optval = 1;
  uint32_t seq;
  ssize_t len;
  صحیح ret;

#اگر_تعریف_شدہ NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, حجم (optval));
  اگر (ret < 0)
    {
      اگر (errno != ENOPROTOOPT)
        واپس crun_make_error (err, errno, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#اگر_ختم

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, حجم (ساخت ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_اشاریہ = ifindex;

  ret = send_request (sock, req, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  جب ((len = عارضی_FAILURE_RETRY (recv (sock, بفر, بفر_سائز, 0))) > 0)
    {
      ساخت nlmsghdr *nlh;

      برائے (nlh = (ساخت nlmsghdr *) بفر; NLMSG_OK (nlh, len); nlh = NLMSG_اگلا (nlh, len))
        {
          ساخت ifaddrmsg *ifa;

          اگر (nlh->nlmsg_seq != seq)
            جاری;

          اگر (nlh->nlmsg_type == NLMSG_مکمل)
            {
              *برآمد_ips = ips;
              ips = تہی;
              واپس 0;
            }

          اگر (nlh->nlmsg_type == NLMSG_ERROR)
            {
              ساخت nlmsgerr *err_ڈیٹا = (ساخت nlmsgerr *) NLMSG_ڈیٹا (nlh);
              اگر (err_ڈیٹا->error == 0)
                جاری;
              واپس crun_make_error (err, -err_ڈیٹا->error, "netlink error reading ip addresses");
            }

          ifa = (ساخت ifaddrmsg *) NLMSG_ڈیٹا (nlh);
          اگر (ifa->ifa_اشاریہ != ifindex)
            جاری;

          /* Copy only permanent, globally routable IP addresses.  */
          اگر (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            جاری;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, حجم (ساخت ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  اگر (UNLIKELY (len < 0))
    واپس crun_make_error (err, errno, "recv");
  واپس 0;
}

ساکن صحیح
configure_ip_پتے (صحیح sock, صحیح ifindex, حرف *بفر, size_t بفر_سائز, ثابت ساخت ip_addr *ips, libcrun_error_t *err)
{
  ساخت nl_req *req = (ساخت nl_req *) بفر;
  ثابت ساخت ip_addr *ip;
  صحیح ret;

  اگر (ips == تہی)
    واپس 0;

  برائے (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      صحیح rta_len = ip->rta_len;
      ساخت rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, حجم (ساخت ifaddrmsg));

      حافظہ_نقل (&req->ifa, &ip->ifa, حجم (ساخت ifaddrmsg));

      req->ifa.ifa_اشاریہ = ifindex;

      برائے (rta = (ساخت rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_اگلا (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), بفر_سائز, rta->rta_type, RTA_ڈیٹا (rta), RTA_PAYLOAD (rta), err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }

      ret = send_request (sock, req, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = wait_for_ack (sock, seq, بفر, بفر_سائز, err);
      اگر (ret < 0)
        واپس ret;
    }

  واپس 0;
}

ساکن صحیح
request_enable_interface_and_wait (صحیح sock, حرف *بفر, size_t بفر_سائز, صحیح اشاریہ, libcrun_error_t *err)
{
  ساخت nl_req *req = (ساخت nl_req *) بفر;
  uint32_t seq;
  صحیح ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, حجم (ساخت ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_اشاریہ = اشاریہ;

  req->ifi.ifi_flags = IFF_اوپر;
  req->ifi.ifi_change = IFF_اوپر;

  ret = send_request (sock, req, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس wait_for_ack (sock, seq, بفر, بفر_سائز, err);
}

ساکن صحیح
تیاری_جال_device_اندر_ns_helper (حرف *بفر, size_t بفر_سائز, صحیح netns_fd, ثابت حرف *newifname,
                                   ساخت ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close صحیح sock_اندر_ns = -1;
  صحیح new_ifindex;
  صحیح ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "change network namespace");

  sock_اندر_ns = open_netlink_fd (err);
  اگر (sock_اندر_ns < 0)
    واپس sock_اندر_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_مقصد_اشاریہ (sock_اندر_ns, newifname, بفر, بفر_سائز, err);
  اگر (UNLIKELY (new_ifindex < 0))
    واپس new_ifindex;

  ret = configure_ip_پتے (sock_اندر_ns, new_ifindex, بفر, بفر_سائز, ips, err);
  اگر (ret < 0)
    واپس ret;

  واپس request_enable_interface_and_wait (sock_اندر_ns, بفر, بفر_سائز, new_ifindex, err);
}

ساکن صحیح
do_move_link_مقصد_ns_and_wait (صحیح sock, حرف *بفر, size_t بفر_سائز, صحیح ifindex, صحیح netns_fd, ثابت حرف *newifname, libcrun_error_t *err)
{
  ساخت nl_req *req = (ساخت nl_req *) بفر;
  uint32_t seq;
  صحیح ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, حجم (ساخت ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_اشاریہ = ifindex;

  ret = append_rtattr (&req->nlh, بفر_سائز, IFLA_NET_NS_FD, &netns_fd, حجم (netns_fd), err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = append_rtattr (&req->nlh, بفر_سائز, IFLA_IFNAME, newifname, لڑی_لمبائی (newifname) + 1, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = send_request (sock, req, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس wait_for_ack (sock, seq, بفر, بفر_سائز, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
ساکن صحیح
تیاری_جال_device_اندر_ns (حرف *بفر, size_t بفر_سائز, صحیح netns_fd, ثابت حرف *newifname,
                            ساخت ip_addr *ips, libcrun_error_t *err)
{
  صحیح wait_حالت;
  pid_t pid;
  صحیح ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  اگر (UNLIKELY (pid < 0))
    واپس crun_make_error (err, errno, "vfork");

  اگر (pid == 0)
    {
      ret = تیاری_جال_device_اندر_ns_helper (بفر, بفر_سائز, netns_fd, newifname, ips, err);
      اگر (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          محفوظ_exit (-ret);
        }

      محفوظ_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_حالت, 0);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "waitpid for exec child pid");

  اگر (wait_حالت != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      واپس -get_process_exit_حالت (wait_حالت);
    }

  واپس 0;
}

صحیح
move_جال_device (ثابت حرف *ifname, ثابت حرف *newifname, صحیح netns_fd, libcrun_error_t *err)
{
  ثابت size_t بفر_سائز = 8192;
  cleanup_ip_addrs ساخت ip_addr *ips = تہی;
  cleanup_free حرف *بفر = xmalloc (بفر_سائز);
  cleanup_close صحیح sock = -1;
  صحیح ifindex;
  صحیح ret;

  sock = open_netlink_fd (err);
  اگر (sock < 0)
    واپس sock;

  ifindex = name_مقصد_اشاریہ (sock, ifname, بفر, بفر_سائز, err);
  اگر (UNLIKELY (ifindex < 0))
    واپس ifindex;

  ret = get_ip_پتے (sock, ifindex, &ips, بفر, بفر_سائز, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_مقصد_ns_and_wait (sock, بفر, بفر_سائز, ifindex, netns_fd, newifname, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس تیاری_جال_device_اندر_ns (بفر, بفر_سائز, netns_fd, newifname, ips, err);
}
