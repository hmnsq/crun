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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "net_device.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"

#ଅନ୍ତର୍ଭାଵ <sys/socket.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <sched.h>
#ଅନ୍ତର୍ଭାଵ <sys/wait.h>
#ଅନ୍ତର୍ଭାଵ <sys/xattr.h>
#ଅନ୍ତର୍ଭାଵ <linux/netlink.h>
#ଅନ୍ତର୍ଭାଵ <linux/rtnetlink.h>
#ଅନ୍ତର୍ଭାଵ <arpa/inet.h>
#ଅନ୍ତର୍ଭାଵ <net/if.h>
#ଯଦି HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint ଅସ୍ଥିର uint
#  endif
#endif

ସଂରଚନା ip_addr
{
  ସଂରଚନା ifaddrmsg ifa;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା rta_len;
  ଅକ୍ଷର *rta;
};

ସଂରଚନା nl_req
{
  ସଂରଚନା nlmsghdr nlh;
  ସଂଘ
  {
    ସଂରଚନା ifaddrmsg ifa;
    ସଂରଚନା ifinfomsg ifi;
  };
};

ସ୍ଥିର atomic_uint nl_seq_counter;

ସ୍ଥିର uint32_t
get_next_seq ()
{
  ଫେରନ୍ତୁ (uint32_t) ++nl_seq_counter;
}

ସ୍ଥିର uint32_t
reset_request (ସଂରଚନା nl_req *req, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ସନ୍ଦେଶ_len)
{
  uint32_t seq = get_next_seq ();

  memset (req, 0, ଆକାର (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_LENGTH (ସନ୍ଦେଶ_len);
  req->nlh.nlmsg_seq = seq;

  ଫେରନ୍ତୁ seq;
}

ସ୍ଥିର ଶୂନ୍ୟ
cleanup_ip_addrsp (ଶୂନ୍ୟ *p)
{
  ସଂରଚନା ip_addr **pp = (ସଂରଚନା ip_addr **) p;
  ସଂରଚନା ip_addr *ip;
  ଯଦି (*pp == ଖାଲି)
    ଫେରନ୍ତୁ;

  ପାଇଁ (ip = *pp; ip->rta_len >= 0; ip++)
    ମୁକ୍ତ (ip->rta);

  ମୁକ୍ତ (*pp);
}

#define cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା sock = -1;
  ସଂରଚନା sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  ଯଦି (sock < 0)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "netlink socket");

  ଯଦି (bind (sock, (ସଂରଚନା sockaddr *) &local, ଆକାର (local)) < 0)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "bind");

  fd = sock;
  sock = -1;
  ଫେରନ୍ତୁ fd;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
append_rtattr (ସଂରଚନା nlmsghdr *n, size_t maxlen, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, ଅଚଳ ଶୂନ୍ୟ *data, size_t data_len, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = RTA_LENGTH (data_len);
  ସଂରଚନା rtattr *rta;

  ଯଦି (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    ଫେରନ୍ତୁ crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (ସଂରଚନା rtattr *) (((ଅକ୍ଷର *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  ଯଦି (data_len)
    memcpy (RTA_DATA (rta), data, data_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
send_request (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sock, ସଂରଚନା nl_req *req, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = TEMP_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "send");

  ଫେରନ୍ତୁ 0;
}

/* if_nametoindex with an open netlink socket.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
name_to_index (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sock, ଅଚଳ ଅକ୍ଷର *ifname, ଅକ୍ଷର *buffer, size_t buffer_size, libcrun_error_t *err)
{
  ସଂରଚନା nlmsghdr *nlh;
  ସଂରଚନା nl_req *req;
  uint32_t seq;
  ssize_t len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  req = (ସଂରଚନା nl_req *) buffer;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, ଆକାର (ସଂରଚନା ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, buffer_size, IFLA_IFNAME, ifname, strlen (ifname) + 1, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = send_request (sock, req, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0));
  ଯଦି (UNLIKELY (len < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "recv");

  ପାଇଁ (nlh = (ସଂରଚନା nlmsghdr *) buffer; NLMSG_OK (nlh, (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) len); nlh = NLMSG_NEXT (nlh, len))
    {
      ଯଦି (nlh->nlmsg_seq != seq)
        ଜାରି;

      ଯଦି (nlh->nlmsg_type == NLMSG_ERROR)
        {
          ସଂରଚନା nlmsgerr *err_data = (ସଂରଚନା nlmsgerr *) NLMSG_DATA (nlh);
          ଯଦି (err_data->error == 0)
            ଜାରି;
          ଫେରନ୍ତୁ crun_make_error (err, -err_data->error, "netlink error while looking for interface `%s`", ifname);
        }

      ଯଦି (nlh->nlmsg_type == RTM_NEWLINK)
        {
          ସଂରଚନା ifinfomsg *ifi = NLMSG_DATA (nlh);
          ଫେରନ୍ତୁ ifi->ifi_index;
        }
    }

  ଫେରନ୍ତୁ crun_make_error (err, 0, "could not find device `%s`", ifname);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
wait_for_ack (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sock, uint32_t seq, ଅକ୍ଷର *recv_buffer, size_t recv_buffer_size, libcrun_error_t *err)
{
  ସଂରଚନା nlmsghdr *nlh_recv = (ସଂରଚନା nlmsghdr *) recv_buffer;
  ssize_t len;

  କର
    {
      len = TEMP_FAILURE_RETRY (recv (sock, recv_buffer, recv_buffer_size, 0));
      ଯଦି (len < 0)
        ଫେରନ୍ତୁ crun_make_error (err, errno, "recv");
  } ଯେତେବେଳେ (nlh_recv->nlmsg_seq != seq);

  ଯଦି (! NLMSG_OK (nlh_recv, len))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "received invalid packet");

  ଯଦି (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      ସଂରଚନା nlmsgerr *err_data = (ସଂରଚନା nlmsgerr *) NLMSG_DATA (nlh_recv);
      ଯଦି (err_data->error == 0)
        ଫେରନ୍ତୁ 0;
      ଫେରନ୍ତୁ crun_make_error (err, -err_data->error, "netlink error");
    }

  ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: received unknown netlink packet type");
}

ସ୍ଥିର ଶୂନ୍ୟ
copy_ip_addr (ସଂରଚନା nlmsghdr *nlh, ସଂରଚନା ip_addr *ip)
{
  ସଂରଚନା ifaddrmsg *ifa = (ସଂରଚନା ifaddrmsg *) NLMSG_DATA (nlh);
  memcpy (&ip->ifa, ifa, ଆକାର (ସଂରଚନା ifaddrmsg));
  ip->rta_len = IFA_PAYLOAD (nlh);
  ip->rta = xmalloc (ip->rta_len);
  memcpy (ip->rta, IFA_RTA (ifa), ip->rta_len);
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_ip_addresses (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sock, uint32_t ifindex, ସଂରଚନା ip_addr **out_ips, ଅକ୍ଷର *buffer, size_t buffer_size, libcrun_error_t *err)
{
  ସଂରଚନା nl_req *req = (ସଂରଚନା nl_req *) buffer;
  cleanup_ip_addrs ସଂରଚନା ip_addr *ips = ଖାଲି;
  size_t ips_len = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optval = 1;
  uint32_t seq;
  ssize_t len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

#ifdef NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, ଆକାର (optval));
  ଯଦି (ret < 0)
    {
      ଯଦି (errno != ENOPROTOOPT)
        ଫେରନ୍ତୁ crun_make_error (err, errno, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#endif

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, ଆକାର (ସଂରଚନା ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_index = ifindex;

  ret = send_request (sock, req, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯେତେବେଳେ ((len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0))) > 0)
    {
      ସଂରଚନା nlmsghdr *nlh;

      ପାଇଁ (nlh = (ସଂରଚନା nlmsghdr *) buffer; NLMSG_OK (nlh, len); nlh = NLMSG_NEXT (nlh, len))
        {
          ସଂରଚନା ifaddrmsg *ifa;

          ଯଦି (nlh->nlmsg_seq != seq)
            ଜାରି;

          ଯଦି (nlh->nlmsg_type == NLMSG_DONE)
            {
              *out_ips = ips;
              ips = ଖାଲି;
              ଫେରନ୍ତୁ 0;
            }

          ଯଦି (nlh->nlmsg_type == NLMSG_ERROR)
            {
              ସଂରଚନା nlmsgerr *err_data = (ସଂରଚନା nlmsgerr *) NLMSG_DATA (nlh);
              ଯଦି (err_data->error == 0)
                ଜାରି;
              ଫେରନ୍ତୁ crun_make_error (err, -err_data->error, "netlink error reading ip addresses");
            }

          ifa = (ସଂରଚନା ifaddrmsg *) NLMSG_DATA (nlh);
          ଯଦି (ifa->ifa_index != ifindex)
            ଜାରି;

          /* Copy only permanent, globally routable IP addresses.  */
          ଯଦି (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            ଜାରି;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, ଆକାର (ସଂରଚନା ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  ଯଦି (UNLIKELY (len < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "recv");
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
configure_ip_addresses (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sock, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ifindex, ଅକ୍ଷର *buffer, size_t buffer_size, ଅଚଳ ସଂରଚନା ip_addr *ips, libcrun_error_t *err)
{
  ସଂରଚନା nl_req *req = (ସଂରଚନା nl_req *) buffer;
  ଅଚଳ ସଂରଚନା ip_addr *ip;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (ips == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  ପାଇଁ (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା rta_len = ip->rta_len;
      ସଂରଚନା rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, ଆକାର (ସଂରଚନା ifaddrmsg));

      memcpy (&req->ifa, &ip->ifa, ଆକାର (ସଂରଚନା ifaddrmsg));

      req->ifa.ifa_index = ifindex;

      ପାଇଁ (rta = (ସଂରଚନା rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_NEXT (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), buffer_size, rta->rta_type, RTA_DATA (rta), RTA_PAYLOAD (rta), err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }

      ret = send_request (sock, req, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = wait_for_ack (sock, seq, buffer, buffer_size, err);
      ଯଦି (ret < 0)
        ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
request_enable_interface_and_wait (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sock, ଅକ୍ଷର *buffer, size_t buffer_size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା index, libcrun_error_t *err)
{
  ସଂରଚନା nl_req *req = (ସଂରଚନା nl_req *) buffer;
  uint32_t seq;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, ଆକାର (ସଂରଚନା ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = index;

  req->ifi.ifi_flags = IFF_UP;
  req->ifi.ifi_change = IFF_UP;

  ret = send_request (sock, req, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ wait_for_ack (sock, seq, buffer, buffer_size, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
setup_network_device_in_ns_helper (ଅକ୍ଷର *buffer, size_t buffer_size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା netns_fd, ଅଚଳ ଅକ୍ଷର *newifname,
                                   ସଂରଚନା ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା sock_in_ns = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_ifindex;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "change network namespace");

  sock_in_ns = open_netlink_fd (err);
  ଯଦି (sock_in_ns < 0)
    ଫେରନ୍ତୁ sock_in_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_to_index (sock_in_ns, newifname, buffer, buffer_size, err);
  ଯଦି (UNLIKELY (new_ifindex < 0))
    ଫେରନ୍ତୁ new_ifindex;

  ret = configure_ip_addresses (sock_in_ns, new_ifindex, buffer, buffer_size, ips, err);
  ଯଦି (ret < 0)
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ request_enable_interface_and_wait (sock_in_ns, buffer, buffer_size, new_ifindex, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
do_move_link_to_ns_and_wait (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sock, ଅକ୍ଷର *buffer, size_t buffer_size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ifindex, ପୂର୍ଣ୍ଣସଂଖ୍ୟା netns_fd, ଅଚଳ ଅକ୍ଷର *newifname, libcrun_error_t *err)
{
  ସଂରଚନା nl_req *req = (ସଂରଚନା nl_req *) buffer;
  uint32_t seq;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, ଆକାର (ସଂରଚନା ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = ifindex;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_NET_NS_FD, &netns_fd, ଆକାର (netns_fd), err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_IFNAME, newifname, strlen (newifname) + 1, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = send_request (sock, req, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ wait_for_ack (sock, seq, buffer, buffer_size, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
setup_network_device_in_ns (ଅକ୍ଷର *buffer, size_t buffer_size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା netns_fd, ଅଚଳ ଅକ୍ଷର *newifname,
                            ସଂରଚନା ip_addr *ips, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା wait_status;
  pid_t pid;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  ଯଦି (UNLIKELY (pid < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "vfork");

  ଯଦି (pid == 0)
    {
      ret = setup_network_device_in_ns_helper (buffer, buffer_size, netns_fd, newifname, ips, err);
      ଯଦି (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          _safe_exit (-ret);
        }

      _safe_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "waitpid for exec child pid");

  ଯଦି (wait_status != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      ଫେରନ୍ତୁ -get_process_exit_status (wait_status);
    }

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
move_network_device (ଅଚଳ ଅକ୍ଷର *ifname, ଅଚଳ ଅକ୍ଷର *newifname, ପୂର୍ଣ୍ଣସଂଖ୍ୟା netns_fd, libcrun_error_t *err)
{
  ଅଚଳ size_t buffer_size = 8192;
  cleanup_ip_addrs ସଂରଚନା ip_addr *ips = ଖାଲି;
  cleanup_free ଅକ୍ଷର *buffer = xmalloc (buffer_size);
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା sock = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ifindex;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  sock = open_netlink_fd (err);
  ଯଦି (sock < 0)
    ଫେରନ୍ତୁ sock;

  ifindex = name_to_index (sock, ifname, buffer, buffer_size, err);
  ଯଦି (UNLIKELY (ifindex < 0))
    ଫେରନ୍ତୁ ifindex;

  ret = get_ip_addresses (sock, ifindex, &ips, buffer, buffer_size, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_to_ns_and_wait (sock, buffer, buffer_size, ifindex, netns_fd, newifname, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ setup_network_device_in_ns (buffer, buffer_size, netns_fd, newifname, ips, err);
}
