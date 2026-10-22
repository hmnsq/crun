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

#ضمن <config.h>
#ضمن "net_device.h"
#ضمن "utils.h"

#ضمن <sys/socket.h>
#ضمن <errno.h>
#ضمن <sched.h>
#ضمن <sys/wait.h>
#ضمن <sys/xattr.h>
#ضمن <linux/netlink.h>
#ضمن <linux/rtnetlink.h>
#ضمن <arpa/inet.h>
#ضمن <net/if.h>
#إذا HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint متطاير uint
#  endif
#endif

بنية ip_addr
{
  بنية ifaddrmsg ifa;

  صحيح rta_len;
  حرف *rta;
};

بنية nl_req
{
  بنية nlmsghdr nlh;
  اتحاد
  {
    بنية ifaddrmsg ifa;
    بنية ifinfomsg ifi;
  };
};

ساكن atomic_uint nl_seq_counter;

ساكن uint32_t
get_next_seq ()
{
  أرجع (uint32_t) ++nl_seq_counter;
}

ساكن uint32_t
reset_request (بنية nl_req *req, صحيح type, صحيح flags, صحيح پیغام_len)
{
  uint32_t seq = get_next_seq ();

  memset (req, 0, حجم (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_LENGTH (پیغام_len);
  req->nlh.nlmsg_seq = seq;

  أرجع seq;
}

ساكن فراغ
cleanup_ip_addrsp (فراغ *p)
{
  بنية ip_addr **pp = (بنية ip_addr **) p;
  بنية ip_addr *ip;
  إذا (*pp == عدم)
    أرجع;

  لكل (ip = *pp; ip->rta_len >= 0; ip++)
    حرر (ip->rta);

  حرر (*pp);
}

#define cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

ساكن صحيح
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close صحيح sock = -1;
  بنية sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  صحيح fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  إذا (sock < 0)
    أرجع crun_make_error (err, errno, "netlink socket");

  إذا (bind (sock, (بنية sockaddr *) &local, حجم (local)) < 0)
    أرجع crun_make_error (err, errno, "bind");

  fd = sock;
  sock = -1;
  أرجع fd;
}

ساكن صحيح
append_rtattr (بنية nlmsghdr *n, size_t maxlen, صحيح type, ثابت فراغ *data, size_t data_len, libcrun_error_t *err)
{
  صحيح len = RTA_LENGTH (data_len);
  بنية rtattr *rta;

  إذا (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    أرجع crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (بنية rtattr *) (((حرف *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  إذا (data_len)
    memcpy (RTA_DATA (rta), data, data_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  أرجع 0;
}

ساكن صحيح
send_request (صحيح sock, بنية nl_req *req, libcrun_error_t *err)
{
  صحيح ret;

  ret = TEMP_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "send");

  أرجع 0;
}

/* if_nametoindex with an open netlink socket.  */
ساكن صحيح
name_to_index (صحيح sock, ثابت حرف *ifname, حرف *buffer, size_t buffer_size, libcrun_error_t *err)
{
  بنية nlmsghdr *nlh;
  بنية nl_req *req;
  uint32_t seq;
  ssize_t len;
  صحيح ret;

  req = (بنية nl_req *) buffer;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, حجم (بنية ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, buffer_size, IFLA_IFNAME, ifname, strlen (ifname) + 1, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = send_request (sock, req, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0));
  إذا (UNLIKELY (len < 0))
    أرجع crun_make_error (err, errno, "recv");

  لكل (nlh = (بنية nlmsghdr *) buffer; NLMSG_OK (nlh, (unsigned صحيح) len); nlh = NLMSG_NEXT (nlh, len))
    {
      إذا (nlh->nlmsg_seq != seq)
        استمر;

      إذا (nlh->nlmsg_type == NLMSG_ERROR)
        {
          بنية nlmsgerr *err_data = (بنية nlmsgerr *) NLMSG_DATA (nlh);
          إذا (err_data->error == 0)
            استمر;
          أرجع crun_make_error (err, -err_data->error, "netlink error while looking for interface `%s`", ifname);
        }

      إذا (nlh->nlmsg_type == RTM_NEWLINK)
        {
          بنية ifinfomsg *ifi = NLMSG_DATA (nlh);
          أرجع ifi->ifi_index;
        }
    }

  أرجع crun_make_error (err, 0, "could not find device `%s`", ifname);
}

ساكن صحيح
wait_for_ack (صحيح sock, uint32_t seq, حرف *recv_buffer, size_t recv_buffer_size, libcrun_error_t *err)
{
  بنية nlmsghdr *nlh_recv = (بنية nlmsghdr *) recv_buffer;
  ssize_t len;

  افعل
    {
      len = TEMP_FAILURE_RETRY (recv (sock, recv_buffer, recv_buffer_size, 0));
      إذا (len < 0)
        أرجع crun_make_error (err, errno, "recv");
  } طالما (nlh_recv->nlmsg_seq != seq);

  إذا (! NLMSG_OK (nlh_recv, len))
    أرجع crun_make_error (err, 0, "received invalid packet");

  إذا (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      بنية nlmsgerr *err_data = (بنية nlmsgerr *) NLMSG_DATA (nlh_recv);
      إذا (err_data->error == 0)
        أرجع 0;
      أرجع crun_make_error (err, -err_data->error, "netlink error");
    }

  أرجع crun_make_error (err, 0, "internal error: received unknown netlink packet type");
}

ساكن فراغ
copy_ip_addr (بنية nlmsghdr *nlh, بنية ip_addr *ip)
{
  بنية ifaddrmsg *ifa = (بنية ifaddrmsg *) NLMSG_DATA (nlh);
  memcpy (&ip->ifa, ifa, حجم (بنية ifaddrmsg));
  ip->rta_len = IFA_PAYLOAD (nlh);
  ip->rta = xmalloc (ip->rta_len);
  memcpy (ip->rta, IFA_RTA (ifa), ip->rta_len);
};

ساكن صحيح
get_ip_addresses (صحيح sock, uint32_t ifindex, بنية ip_addr **out_ips, حرف *buffer, size_t buffer_size, libcrun_error_t *err)
{
  بنية nl_req *req = (بنية nl_req *) buffer;
  cleanup_ip_addrs بنية ip_addr *ips = عدم;
  size_t ips_len = 0;
  صحيح optval = 1;
  uint32_t seq;
  ssize_t len;
  صحيح ret;

#ifdef NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, حجم (optval));
  إذا (ret < 0)
    {
      إذا (errno != ENOPROTOOPT)
        أرجع crun_make_error (err, errno, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#endif

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, حجم (بنية ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_index = ifindex;

  ret = send_request (sock, req, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  طالما ((len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0))) > 0)
    {
      بنية nlmsghdr *nlh;

      لكل (nlh = (بنية nlmsghdr *) buffer; NLMSG_OK (nlh, len); nlh = NLMSG_NEXT (nlh, len))
        {
          بنية ifaddrmsg *ifa;

          إذا (nlh->nlmsg_seq != seq)
            استمر;

          إذا (nlh->nlmsg_type == NLMSG_DONE)
            {
              *out_ips = ips;
              ips = عدم;
              أرجع 0;
            }

          إذا (nlh->nlmsg_type == NLMSG_ERROR)
            {
              بنية nlmsgerr *err_data = (بنية nlmsgerr *) NLMSG_DATA (nlh);
              إذا (err_data->error == 0)
                استمر;
              أرجع crun_make_error (err, -err_data->error, "netlink error reading ip addresses");
            }

          ifa = (بنية ifaddrmsg *) NLMSG_DATA (nlh);
          إذا (ifa->ifa_index != ifindex)
            استمر;

          /* Copy only permanent, globally routable IP addresses.  */
          إذا (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            استمر;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, حجم (بنية ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  إذا (UNLIKELY (len < 0))
    أرجع crun_make_error (err, errno, "recv");
  أرجع 0;
}

ساكن صحيح
configure_ip_addresses (صحيح sock, صحيح ifindex, حرف *buffer, size_t buffer_size, ثابت بنية ip_addr *ips, libcrun_error_t *err)
{
  بنية nl_req *req = (بنية nl_req *) buffer;
  ثابت بنية ip_addr *ip;
  صحيح ret;

  إذا (ips == عدم)
    أرجع 0;

  لكل (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      صحيح rta_len = ip->rta_len;
      بنية rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, حجم (بنية ifaddrmsg));

      memcpy (&req->ifa, &ip->ifa, حجم (بنية ifaddrmsg));

      req->ifa.ifa_index = ifindex;

      لكل (rta = (بنية rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_NEXT (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), buffer_size, rta->rta_type, RTA_DATA (rta), RTA_PAYLOAD (rta), err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }

      ret = send_request (sock, req, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = wait_for_ack (sock, seq, buffer, buffer_size, err);
      إذا (ret < 0)
        أرجع ret;
    }

  أرجع 0;
}

ساكن صحيح
request_enable_interface_and_wait (صحيح sock, حرف *buffer, size_t buffer_size, صحيح index, libcrun_error_t *err)
{
  بنية nl_req *req = (بنية nl_req *) buffer;
  uint32_t seq;
  صحيح ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, حجم (بنية ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = index;

  req->ifi.ifi_flags = IFF_UP;
  req->ifi.ifi_change = IFF_UP;

  ret = send_request (sock, req, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع wait_for_ack (sock, seq, buffer, buffer_size, err);
}

ساكن صحيح
setup_network_device_in_ns_helper (حرف *buffer, size_t buffer_size, صحيح netns_fd, ثابت حرف *newifname,
                                   بنية ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close صحيح sock_in_ns = -1;
  صحيح new_ifindex;
  صحيح ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "change network namespace");

  sock_in_ns = open_netlink_fd (err);
  إذا (sock_in_ns < 0)
    أرجع sock_in_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_to_index (sock_in_ns, newifname, buffer, buffer_size, err);
  إذا (UNLIKELY (new_ifindex < 0))
    أرجع new_ifindex;

  ret = configure_ip_addresses (sock_in_ns, new_ifindex, buffer, buffer_size, ips, err);
  إذا (ret < 0)
    أرجع ret;

  أرجع request_enable_interface_and_wait (sock_in_ns, buffer, buffer_size, new_ifindex, err);
}

ساكن صحيح
do_move_link_to_ns_and_wait (صحيح sock, حرف *buffer, size_t buffer_size, صحيح ifindex, صحيح netns_fd, ثابت حرف *newifname, libcrun_error_t *err)
{
  بنية nl_req *req = (بنية nl_req *) buffer;
  uint32_t seq;
  صحيح ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, حجم (بنية ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = ifindex;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_NET_NS_FD, &netns_fd, حجم (netns_fd), err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_IFNAME, newifname, strlen (newifname) + 1, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = send_request (sock, req, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع wait_for_ack (sock, seq, buffer, buffer_size, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
ساكن صحيح
setup_network_device_in_ns (حرف *buffer, size_t buffer_size, صحيح netns_fd, ثابت حرف *newifname,
                            بنية ip_addr *ips, libcrun_error_t *err)
{
  صحيح wait_status;
  pid_t pid;
  صحيح ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  إذا (UNLIKELY (pid < 0))
    أرجع crun_make_error (err, errno, "vfork");

  إذا (pid == 0)
    {
      ret = setup_network_device_in_ns_helper (buffer, buffer_size, netns_fd, newifname, ips, err);
      إذا (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          _safe_exit (-ret);
        }

      _safe_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "waitpid for exec child pid");

  إذا (wait_status != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      أرجع -get_process_exit_status (wait_status);
    }

  أرجع 0;
}

صحيح
move_network_device (ثابت حرف *ifname, ثابت حرف *newifname, صحيح netns_fd, libcrun_error_t *err)
{
  ثابت size_t buffer_size = 8192;
  cleanup_ip_addrs بنية ip_addr *ips = عدم;
  cleanup_free حرف *buffer = xmalloc (buffer_size);
  cleanup_close صحيح sock = -1;
  صحيح ifindex;
  صحيح ret;

  sock = open_netlink_fd (err);
  إذا (sock < 0)
    أرجع sock;

  ifindex = name_to_index (sock, ifname, buffer, buffer_size, err);
  إذا (UNLIKELY (ifindex < 0))
    أرجع ifindex;

  ret = get_ip_addresses (sock, ifindex, &ips, buffer, buffer_size, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_to_ns_and_wait (sock, buffer, buffer_size, ifindex, netns_fd, newifname, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع setup_network_device_in_ns (buffer, buffer_size, netns_fd, newifname, ips, err);
}
