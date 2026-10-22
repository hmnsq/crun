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
#    define atomic_uint فرار uint
#  endif
#endif

ساختار ip_addr
{
  ساختار ifaddrmsg ifa;

  صحیح rta_len;
  نویسه *rta;
};

ساختار nl_req
{
  ساختار nlmsghdr nlh;
  اجتماع
  {
    ساختار ifaddrmsg ifa;
    ساختار ifinfomsg ifi;
  };
};

ایستا atomic_uint nl_seq_counter;

ایستا uint32_t
get_next_seq ()
{
  بازگشت (uint32_t) ++nl_seq_counter;
}

ایستا uint32_t
reset_request (ساختار nl_req *req, صحیح type, صحیح flags, صحیح پیام_len)
{
  uint32_t seq = get_next_seq ();

  memset (req, 0, اندازه (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_LENGTH (پیام_len);
  req->nlh.nlmsg_seq = seq;

  بازگشت seq;
}

ایستا تهی
cleanup_ip_addrsp (تهی *p)
{
  ساختار ip_addr **pp = (ساختار ip_addr **) p;
  ساختار ip_addr *ip;
  اگر (*pp == پوچ)
    بازگشت;

  برای (ip = *pp; ip->rta_len >= 0; ip++)
    آزاد (ip->rta);

  آزاد (*pp);
}

#define cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

ایستا صحیح
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close صحیح sock = -1;
  ساختار sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  صحیح fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  اگر (sock < 0)
    بازگشت crun_make_error (err, errno, "netlink socket");

  اگر (bind (sock, (ساختار sockaddr *) &local, اندازه (local)) < 0)
    بازگشت crun_make_error (err, errno, "bind");

  fd = sock;
  sock = -1;
  بازگشت fd;
}

ایستا صحیح
append_rtattr (ساختار nlmsghdr *n, size_t maxlen, صحیح type, ثابت تهی *data, size_t data_len, libcrun_error_t *err)
{
  صحیح len = RTA_LENGTH (data_len);
  ساختار rtattr *rta;

  اگر (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    بازگشت crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (ساختار rtattr *) (((نویسه *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  اگر (data_len)
    memcpy (RTA_DATA (rta), data, data_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  بازگشت 0;
}

ایستا صحیح
send_request (صحیح sock, ساختار nl_req *req, libcrun_error_t *err)
{
  صحیح ret;

  ret = TEMP_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "send");

  بازگشت 0;
}

/* if_nametoindex with an open netlink socket.  */
ایستا صحیح
name_to_index (صحیح sock, ثابت نویسه *ifname, نویسه *buffer, size_t buffer_size, libcrun_error_t *err)
{
  ساختار nlmsghdr *nlh;
  ساختار nl_req *req;
  uint32_t seq;
  ssize_t len;
  صحیح ret;

  req = (ساختار nl_req *) buffer;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, اندازه (ساختار ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, buffer_size, IFLA_IFNAME, ifname, strlen (ifname) + 1, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = send_request (sock, req, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0));
  اگر (UNLIKELY (len < 0))
    بازگشت crun_make_error (err, errno, "recv");

  برای (nlh = (ساختار nlmsghdr *) buffer; NLMSG_OK (nlh, (unsigned صحیح) len); nlh = NLMSG_NEXT (nlh, len))
    {
      اگر (nlh->nlmsg_seq != seq)
        ادامه;

      اگر (nlh->nlmsg_type == NLMSG_ERROR)
        {
          ساختار nlmsgerr *err_data = (ساختار nlmsgerr *) NLMSG_DATA (nlh);
          اگر (err_data->error == 0)
            ادامه;
          بازگشت crun_make_error (err, -err_data->error, "netlink error while looking for interface `%s`", ifname);
        }

      اگر (nlh->nlmsg_type == RTM_NEWLINK)
        {
          ساختار ifinfomsg *ifi = NLMSG_DATA (nlh);
          بازگشت ifi->ifi_index;
        }
    }

  بازگشت crun_make_error (err, 0, "could not find device `%s`", ifname);
}

ایستا صحیح
wait_for_ack (صحیح sock, uint32_t seq, نویسه *recv_buffer, size_t recv_buffer_size, libcrun_error_t *err)
{
  ساختار nlmsghdr *nlh_recv = (ساختار nlmsghdr *) recv_buffer;
  ssize_t len;

  انجامبده
    {
      len = TEMP_FAILURE_RETRY (recv (sock, recv_buffer, recv_buffer_size, 0));
      اگر (len < 0)
        بازگشت crun_make_error (err, errno, "recv");
  } وقتی (nlh_recv->nlmsg_seq != seq);

  اگر (! NLMSG_OK (nlh_recv, len))
    بازگشت crun_make_error (err, 0, "received invalid packet");

  اگر (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      ساختار nlmsgerr *err_data = (ساختار nlmsgerr *) NLMSG_DATA (nlh_recv);
      اگر (err_data->error == 0)
        بازگشت 0;
      بازگشت crun_make_error (err, -err_data->error, "netlink error");
    }

  بازگشت crun_make_error (err, 0, "internal error: received unknown netlink packet type");
}

ایستا تهی
copy_ip_addr (ساختار nlmsghdr *nlh, ساختار ip_addr *ip)
{
  ساختار ifaddrmsg *ifa = (ساختار ifaddrmsg *) NLMSG_DATA (nlh);
  memcpy (&ip->ifa, ifa, اندازه (ساختار ifaddrmsg));
  ip->rta_len = IFA_PAYLOAD (nlh);
  ip->rta = xmalloc (ip->rta_len);
  memcpy (ip->rta, IFA_RTA (ifa), ip->rta_len);
};

ایستا صحیح
get_ip_addresses (صحیح sock, uint32_t ifindex, ساختار ip_addr **out_ips, نویسه *buffer, size_t buffer_size, libcrun_error_t *err)
{
  ساختار nl_req *req = (ساختار nl_req *) buffer;
  cleanup_ip_addrs ساختار ip_addr *ips = پوچ;
  size_t ips_len = 0;
  صحیح optval = 1;
  uint32_t seq;
  ssize_t len;
  صحیح ret;

#ifdef NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, اندازه (optval));
  اگر (ret < 0)
    {
      اگر (errno != ENOPROTOOPT)
        بازگشت crun_make_error (err, errno, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#endif

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, اندازه (ساختار ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_index = ifindex;

  ret = send_request (sock, req, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  وقتی ((len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0))) > 0)
    {
      ساختار nlmsghdr *nlh;

      برای (nlh = (ساختار nlmsghdr *) buffer; NLMSG_OK (nlh, len); nlh = NLMSG_NEXT (nlh, len))
        {
          ساختار ifaddrmsg *ifa;

          اگر (nlh->nlmsg_seq != seq)
            ادامه;

          اگر (nlh->nlmsg_type == NLMSG_DONE)
            {
              *out_ips = ips;
              ips = پوچ;
              بازگشت 0;
            }

          اگر (nlh->nlmsg_type == NLMSG_ERROR)
            {
              ساختار nlmsgerr *err_data = (ساختار nlmsgerr *) NLMSG_DATA (nlh);
              اگر (err_data->error == 0)
                ادامه;
              بازگشت crun_make_error (err, -err_data->error, "netlink error reading ip addresses");
            }

          ifa = (ساختار ifaddrmsg *) NLMSG_DATA (nlh);
          اگر (ifa->ifa_index != ifindex)
            ادامه;

          /* Copy only permanent, globally routable IP addresses.  */
          اگر (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            ادامه;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, اندازه (ساختار ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  اگر (UNLIKELY (len < 0))
    بازگشت crun_make_error (err, errno, "recv");
  بازگشت 0;
}

ایستا صحیح
configure_ip_addresses (صحیح sock, صحیح ifindex, نویسه *buffer, size_t buffer_size, ثابت ساختار ip_addr *ips, libcrun_error_t *err)
{
  ساختار nl_req *req = (ساختار nl_req *) buffer;
  ثابت ساختار ip_addr *ip;
  صحیح ret;

  اگر (ips == پوچ)
    بازگشت 0;

  برای (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      صحیح rta_len = ip->rta_len;
      ساختار rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, اندازه (ساختار ifaddrmsg));

      memcpy (&req->ifa, &ip->ifa, اندازه (ساختار ifaddrmsg));

      req->ifa.ifa_index = ifindex;

      برای (rta = (ساختار rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_NEXT (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), buffer_size, rta->rta_type, RTA_DATA (rta), RTA_PAYLOAD (rta), err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }

      ret = send_request (sock, req, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = wait_for_ack (sock, seq, buffer, buffer_size, err);
      اگر (ret < 0)
        بازگشت ret;
    }

  بازگشت 0;
}

ایستا صحیح
request_enable_interface_and_wait (صحیح sock, نویسه *buffer, size_t buffer_size, صحیح index, libcrun_error_t *err)
{
  ساختار nl_req *req = (ساختار nl_req *) buffer;
  uint32_t seq;
  صحیح ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, اندازه (ساختار ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = index;

  req->ifi.ifi_flags = IFF_UP;
  req->ifi.ifi_change = IFF_UP;

  ret = send_request (sock, req, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت wait_for_ack (sock, seq, buffer, buffer_size, err);
}

ایستا صحیح
setup_network_device_in_ns_helper (نویسه *buffer, size_t buffer_size, صحیح netns_fd, ثابت نویسه *newifname,
                                   ساختار ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close صحیح sock_in_ns = -1;
  صحیح new_ifindex;
  صحیح ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "change network namespace");

  sock_in_ns = open_netlink_fd (err);
  اگر (sock_in_ns < 0)
    بازگشت sock_in_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_to_index (sock_in_ns, newifname, buffer, buffer_size, err);
  اگر (UNLIKELY (new_ifindex < 0))
    بازگشت new_ifindex;

  ret = configure_ip_addresses (sock_in_ns, new_ifindex, buffer, buffer_size, ips, err);
  اگر (ret < 0)
    بازگشت ret;

  بازگشت request_enable_interface_and_wait (sock_in_ns, buffer, buffer_size, new_ifindex, err);
}

ایستا صحیح
do_move_link_to_ns_and_wait (صحیح sock, نویسه *buffer, size_t buffer_size, صحیح ifindex, صحیح netns_fd, ثابت نویسه *newifname, libcrun_error_t *err)
{
  ساختار nl_req *req = (ساختار nl_req *) buffer;
  uint32_t seq;
  صحیح ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, اندازه (ساختار ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = ifindex;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_NET_NS_FD, &netns_fd, اندازه (netns_fd), err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_IFNAME, newifname, strlen (newifname) + 1, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = send_request (sock, req, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت wait_for_ack (sock, seq, buffer, buffer_size, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
ایستا صحیح
setup_network_device_in_ns (نویسه *buffer, size_t buffer_size, صحیح netns_fd, ثابت نویسه *newifname,
                            ساختار ip_addr *ips, libcrun_error_t *err)
{
  صحیح wait_status;
  pid_t pid;
  صحیح ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  اگر (UNLIKELY (pid < 0))
    بازگشت crun_make_error (err, errno, "vfork");

  اگر (pid == 0)
    {
      ret = setup_network_device_in_ns_helper (buffer, buffer_size, netns_fd, newifname, ips, err);
      اگر (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          _safe_exit (-ret);
        }

      _safe_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "waitpid for exec child pid");

  اگر (wait_status != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      بازگشت -get_process_exit_status (wait_status);
    }

  بازگشت 0;
}

صحیح
move_network_device (ثابت نویسه *ifname, ثابت نویسه *newifname, صحیح netns_fd, libcrun_error_t *err)
{
  ثابت size_t buffer_size = 8192;
  cleanup_ip_addrs ساختار ip_addr *ips = پوچ;
  cleanup_free نویسه *buffer = xmalloc (buffer_size);
  cleanup_close صحیح sock = -1;
  صحیح ifindex;
  صحیح ret;

  sock = open_netlink_fd (err);
  اگر (sock < 0)
    بازگشت sock;

  ifindex = name_to_index (sock, ifname, buffer, buffer_size, err);
  اگر (UNLIKELY (ifindex < 0))
    بازگشت ifindex;

  ret = get_ip_addresses (sock, ifindex, &ips, buffer, buffer_size, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_to_ns_and_wait (sock, buffer, buffer_size, ifindex, netns_fd, newifname, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت setup_network_device_in_ns (buffer, buffer_size, netns_fd, newifname, ips, err);
}
