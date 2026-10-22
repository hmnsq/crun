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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "net_device.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/socket.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sched.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/wait.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/xattr.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/netlink.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/rtnetlink.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <arpa/inet.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <net/if.h>
#ਜੇ HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint ਅਸਥਿਰ uint
#  endif
#ਜੇ_ਅੰਤ

ਬਣਤਰ ip_addr
{
  ਬਣਤਰ ifaddrmsg ifa;

  ਪੂਰਨ rta_len;
  ਅੱਖਰ *rta;
};

ਬਣਤਰ nl_req
{
  ਬਣਤਰ nlmsghdr nlh;
  ਸੰਘ
  {
    ਬਣਤਰ ifaddrmsg ifa;
    ਬਣਤਰ ifinfomsg ifi;
  };
};

ਸਥਿਰ atomic_uint nl_seq_counter;

ਸਥਿਰ uint32_t
get_ਅਗਲਾ_seq ()
{
  ਵਾਪਸ (uint32_t) ++nl_seq_counter;
}

ਸਥਿਰ uint32_t
reset_request (ਬਣਤਰ nl_req *req, ਪੂਰਨ type, ਪੂਰਨ flags, ਪੂਰਨ ਸੁਨੇਹਾ_len)
{
  uint32_t seq = get_ਅਗਲਾ_seq ();

  memset (req, 0, ਆਕਾਰ (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_ਲੰਮਾਈ (ਸੁਨੇਹਾ_len);
  req->nlh.nlmsg_seq = seq;

  ਵਾਪਸ seq;
}

ਸਥਿਰ ਖਾਲੀ
cleanup_ip_addrsp (ਖਾਲੀ *p)
{
  ਬਣਤਰ ip_addr **pp = (ਬਣਤਰ ip_addr **) p;
  ਬਣਤਰ ip_addr *ip;
  ਜੇ (*pp == ਨਿਰਾ)
    ਵਾਪਸ;

  ਲਈ (ip = *pp; ip->rta_len >= 0; ip++)
    ਮੁਕਤ (ip->rta);

  ਮੁਕਤ (*pp);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

ਸਥਿਰ ਪੂਰਨ
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ sock = -1;
  ਬਣਤਰ sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  ਪੂਰਨ fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  ਜੇ (sock < 0)
    ਵਾਪਸ crun_make_error (err, errno, "netlink socket");

  ਜੇ (bind (sock, (ਬਣਤਰ sockaddr *) &local, ਆਕਾਰ (local)) < 0)
    ਵਾਪਸ crun_make_error (err, errno, "bind");

  fd = sock;
  sock = -1;
  ਵਾਪਸ fd;
}

ਸਥਿਰ ਪੂਰਨ
append_rtattr (ਬਣਤਰ nlmsghdr *n, size_t maxlen, ਪੂਰਨ type, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, size_t ਅੰਕੜੇ_len, libcrun_error_t *err)
{
  ਪੂਰਨ len = RTA_ਲੰਮਾਈ (ਅੰਕੜੇ_len);
  ਬਣਤਰ rtattr *rta;

  ਜੇ (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    ਵਾਪਸ crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (ਬਣਤਰ rtattr *) (((ਅੱਖਰ *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  ਜੇ (ਅੰਕੜੇ_len)
    ਸਮ੍ਰਿਤੀ_ਨਕਲ (RTA_ਅੰਕੜੇ (rta), ਅੰਕੜੇ, ਅੰਕੜੇ_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
send_request (ਪੂਰਨ sock, ਬਣਤਰ nl_req *req, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "send");

  ਵਾਪਸ 0;
}

/* if_nametoindex with an open netlink socket.  */
ਸਥਿਰ ਪੂਰਨ
name_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ (ਪੂਰਨ sock, ਅਚਲ ਅੱਖਰ *ifname, ਅੱਖਰ *ਬਫਰ, size_t ਬਫਰ_ਮਾਪ, libcrun_error_t *err)
{
  ਬਣਤਰ nlmsghdr *nlh;
  ਬਣਤਰ nl_req *req;
  uint32_t seq;
  ssize_t len;
  ਪੂਰਨ ret;

  req = (ਬਣਤਰ nl_req *) ਬਫਰ;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, ਆਕਾਰ (ਬਣਤਰ ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, ਬਫਰ_ਮਾਪ, IFLA_IFNAME, ifname, ਲੜੀ_ਲੰਬਾਈ (ifname) + 1, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = send_request (sock, req, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  len = ਆਰਜ਼ੀ_FAILURE_RETRY (recv (sock, ਬਫਰ, ਬਫਰ_ਮਾਪ, 0));
  ਜੇ (UNLIKELY (len < 0))
    ਵਾਪਸ crun_make_error (err, errno, "recv");

  ਲਈ (nlh = (ਬਣਤਰ nlmsghdr *) ਬਫਰ; NLMSG_OK (nlh, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) len); nlh = NLMSG_ਅਗਲਾ (nlh, len))
    {
      ਜੇ (nlh->nlmsg_seq != seq)
        ਜਾਰੀ;

      ਜੇ (nlh->nlmsg_type == NLMSG_ERROR)
        {
          ਬਣਤਰ nlmsgerr *err_ਅੰਕੜੇ = (ਬਣਤਰ nlmsgerr *) NLMSG_ਅੰਕੜੇ (nlh);
          ਜੇ (err_ਅੰਕੜੇ->error == 0)
            ਜਾਰੀ;
          ਵਾਪਸ crun_make_error (err, -err_ਅੰਕੜੇ->error, "netlink error while looking for interface `%s`", ifname);
        }

      ਜੇ (nlh->nlmsg_type == RTM_NEWLINK)
        {
          ਬਣਤਰ ifinfomsg *ifi = NLMSG_ਅੰਕੜੇ (nlh);
          ਵਾਪਸ ifi->ifi_ਸੂਚਕਾਂਕ;
        }
    }

  ਵਾਪਸ crun_make_error (err, 0, "could not find device `%s`", ifname);
}

ਸਥਿਰ ਪੂਰਨ
wait_for_ack (ਪੂਰਨ sock, uint32_t seq, ਅੱਖਰ *recv_ਬਫਰ, size_t recv_ਬਫਰ_ਮਾਪ, libcrun_error_t *err)
{
  ਬਣਤਰ nlmsghdr *nlh_recv = (ਬਣਤਰ nlmsghdr *) recv_ਬਫਰ;
  ssize_t len;

  ਕਰੋ
    {
      len = ਆਰਜ਼ੀ_FAILURE_RETRY (recv (sock, recv_ਬਫਰ, recv_ਬਫਰ_ਮਾਪ, 0));
      ਜੇ (len < 0)
        ਵਾਪਸ crun_make_error (err, errno, "recv");
  } ਜਦੋਂ (nlh_recv->nlmsg_seq != seq);

  ਜੇ (! NLMSG_OK (nlh_recv, len))
    ਵਾਪਸ crun_make_error (err, 0, "received invalid packet");

  ਜੇ (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      ਬਣਤਰ nlmsgerr *err_ਅੰਕੜੇ = (ਬਣਤਰ nlmsgerr *) NLMSG_ਅੰਕੜੇ (nlh_recv);
      ਜੇ (err_ਅੰਕੜੇ->error == 0)
        ਵਾਪਸ 0;
      ਵਾਪਸ crun_make_error (err, -err_ਅੰਕੜੇ->error, "netlink error");
    }

  ਵਾਪਸ crun_make_error (err, 0, "internal error: received unknown netlink packet type");
}

ਸਥਿਰ ਖਾਲੀ
copy_ip_addr (ਬਣਤਰ nlmsghdr *nlh, ਬਣਤਰ ip_addr *ip)
{
  ਬਣਤਰ ifaddrmsg *ifa = (ਬਣਤਰ ifaddrmsg *) NLMSG_ਅੰਕੜੇ (nlh);
  ਸਮ੍ਰਿਤੀ_ਨਕਲ (&ip->ifa, ifa, ਆਕਾਰ (ਬਣਤਰ ifaddrmsg));
  ip->rta_len = IFA_PAYLOAD (nlh);
  ip->rta = xmalloc (ip->rta_len);
  ਸਮ੍ਰਿਤੀ_ਨਕਲ (ip->rta, IFA_RTA (ifa), ip->rta_len);
};

ਸਥਿਰ ਪੂਰਨ
get_ip_ਪਤੇ (ਪੂਰਨ sock, uint32_t ifindex, ਬਣਤਰ ip_addr **ਬਾਹਰਲਾ_ips, ਅੱਖਰ *ਬਫਰ, size_t ਬਫਰ_ਮਾਪ, libcrun_error_t *err)
{
  ਬਣਤਰ nl_req *req = (ਬਣਤਰ nl_req *) ਬਫਰ;
  cleanup_ip_addrs ਬਣਤਰ ip_addr *ips = ਨਿਰਾ;
  size_t ips_len = 0;
  ਪੂਰਨ optval = 1;
  uint32_t seq;
  ssize_t len;
  ਪੂਰਨ ret;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, ਆਕਾਰ (optval));
  ਜੇ (ret < 0)
    {
      ਜੇ (errno != ENOPROTOOPT)
        ਵਾਪਸ crun_make_error (err, errno, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#ਜੇ_ਅੰਤ

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, ਆਕਾਰ (ਬਣਤਰ ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_ਸੂਚਕਾਂਕ = ifindex;

  ret = send_request (sock, req, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜਦੋਂ ((len = ਆਰਜ਼ੀ_FAILURE_RETRY (recv (sock, ਬਫਰ, ਬਫਰ_ਮਾਪ, 0))) > 0)
    {
      ਬਣਤਰ nlmsghdr *nlh;

      ਲਈ (nlh = (ਬਣਤਰ nlmsghdr *) ਬਫਰ; NLMSG_OK (nlh, len); nlh = NLMSG_ਅਗਲਾ (nlh, len))
        {
          ਬਣਤਰ ifaddrmsg *ifa;

          ਜੇ (nlh->nlmsg_seq != seq)
            ਜਾਰੀ;

          ਜੇ (nlh->nlmsg_type == NLMSG_ਪੂਰਾ)
            {
              *ਬਾਹਰਲਾ_ips = ips;
              ips = ਨਿਰਾ;
              ਵਾਪਸ 0;
            }

          ਜੇ (nlh->nlmsg_type == NLMSG_ERROR)
            {
              ਬਣਤਰ nlmsgerr *err_ਅੰਕੜੇ = (ਬਣਤਰ nlmsgerr *) NLMSG_ਅੰਕੜੇ (nlh);
              ਜੇ (err_ਅੰਕੜੇ->error == 0)
                ਜਾਰੀ;
              ਵਾਪਸ crun_make_error (err, -err_ਅੰਕੜੇ->error, "netlink error reading ip addresses");
            }

          ifa = (ਬਣਤਰ ifaddrmsg *) NLMSG_ਅੰਕੜੇ (nlh);
          ਜੇ (ifa->ifa_ਸੂਚਕਾਂਕ != ifindex)
            ਜਾਰੀ;

          /* Copy only permanent, globally routable IP addresses.  */
          ਜੇ (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            ਜਾਰੀ;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, ਆਕਾਰ (ਬਣਤਰ ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  ਜੇ (UNLIKELY (len < 0))
    ਵਾਪਸ crun_make_error (err, errno, "recv");
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
configure_ip_ਪਤੇ (ਪੂਰਨ sock, ਪੂਰਨ ifindex, ਅੱਖਰ *ਬਫਰ, size_t ਬਫਰ_ਮਾਪ, ਅਚਲ ਬਣਤਰ ip_addr *ips, libcrun_error_t *err)
{
  ਬਣਤਰ nl_req *req = (ਬਣਤਰ nl_req *) ਬਫਰ;
  ਅਚਲ ਬਣਤਰ ip_addr *ip;
  ਪੂਰਨ ret;

  ਜੇ (ips == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ਲਈ (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      ਪੂਰਨ rta_len = ip->rta_len;
      ਬਣਤਰ rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, ਆਕਾਰ (ਬਣਤਰ ifaddrmsg));

      ਸਮ੍ਰਿਤੀ_ਨਕਲ (&req->ifa, &ip->ifa, ਆਕਾਰ (ਬਣਤਰ ifaddrmsg));

      req->ifa.ifa_ਸੂਚਕਾਂਕ = ifindex;

      ਲਈ (rta = (ਬਣਤਰ rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_ਅਗਲਾ (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), ਬਫਰ_ਮਾਪ, rta->rta_type, RTA_ਅੰਕੜੇ (rta), RTA_PAYLOAD (rta), err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }

      ret = send_request (sock, req, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = wait_for_ack (sock, seq, ਬਫਰ, ਬਫਰ_ਮਾਪ, err);
      ਜੇ (ret < 0)
        ਵਾਪਸ ret;
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
request_enable_interface_and_wait (ਪੂਰਨ sock, ਅੱਖਰ *ਬਫਰ, size_t ਬਫਰ_ਮਾਪ, ਪੂਰਨ ਸੂਚਕਾਂਕ, libcrun_error_t *err)
{
  ਬਣਤਰ nl_req *req = (ਬਣਤਰ nl_req *) ਬਫਰ;
  uint32_t seq;
  ਪੂਰਨ ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, ਆਕਾਰ (ਬਣਤਰ ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ;

  req->ifi.ifi_flags = IFF_ਉੱਪਰ;
  req->ifi.ifi_change = IFF_ਉੱਪਰ;

  ret = send_request (sock, req, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ wait_for_ack (sock, seq, ਬਫਰ, ਬਫਰ_ਮਾਪ, err);
}

ਸਥਿਰ ਪੂਰਨ
ਤਿਆਰੀ_ਜਾਲ_device_ਵਿੱਚ_ns_helper (ਅੱਖਰ *ਬਫਰ, size_t ਬਫਰ_ਮਾਪ, ਪੂਰਨ netns_fd, ਅਚਲ ਅੱਖਰ *newifname,
                                   ਬਣਤਰ ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ sock_ਵਿੱਚ_ns = -1;
  ਪੂਰਨ new_ifindex;
  ਪੂਰਨ ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "change network namespace");

  sock_ਵਿੱਚ_ns = open_netlink_fd (err);
  ਜੇ (sock_ਵਿੱਚ_ns < 0)
    ਵਾਪਸ sock_ਵਿੱਚ_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ (sock_ਵਿੱਚ_ns, newifname, ਬਫਰ, ਬਫਰ_ਮਾਪ, err);
  ਜੇ (UNLIKELY (new_ifindex < 0))
    ਵਾਪਸ new_ifindex;

  ret = configure_ip_ਪਤੇ (sock_ਵਿੱਚ_ns, new_ifindex, ਬਫਰ, ਬਫਰ_ਮਾਪ, ips, err);
  ਜੇ (ret < 0)
    ਵਾਪਸ ret;

  ਵਾਪਸ request_enable_interface_and_wait (sock_ਵਿੱਚ_ns, ਬਫਰ, ਬਫਰ_ਮਾਪ, new_ifindex, err);
}

ਸਥਿਰ ਪੂਰਨ
do_move_link_ਮੰਜ਼ਿਲ_ns_and_wait (ਪੂਰਨ sock, ਅੱਖਰ *ਬਫਰ, size_t ਬਫਰ_ਮਾਪ, ਪੂਰਨ ifindex, ਪੂਰਨ netns_fd, ਅਚਲ ਅੱਖਰ *newifname, libcrun_error_t *err)
{
  ਬਣਤਰ nl_req *req = (ਬਣਤਰ nl_req *) ਬਫਰ;
  uint32_t seq;
  ਪੂਰਨ ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, ਆਕਾਰ (ਬਣਤਰ ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_ਸੂਚਕਾਂਕ = ifindex;

  ret = append_rtattr (&req->nlh, ਬਫਰ_ਮਾਪ, IFLA_NET_NS_FD, &netns_fd, ਆਕਾਰ (netns_fd), err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = append_rtattr (&req->nlh, ਬਫਰ_ਮਾਪ, IFLA_IFNAME, newifname, ਲੜੀ_ਲੰਬਾਈ (newifname) + 1, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = send_request (sock, req, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ wait_for_ack (sock, seq, ਬਫਰ, ਬਫਰ_ਮਾਪ, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
ਸਥਿਰ ਪੂਰਨ
ਤਿਆਰੀ_ਜਾਲ_device_ਵਿੱਚ_ns (ਅੱਖਰ *ਬਫਰ, size_t ਬਫਰ_ਮਾਪ, ਪੂਰਨ netns_fd, ਅਚਲ ਅੱਖਰ *newifname,
                            ਬਣਤਰ ip_addr *ips, libcrun_error_t *err)
{
  ਪੂਰਨ wait_ਸਥਿਤੀ;
  pid_t pid;
  ਪੂਰਨ ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  ਜੇ (UNLIKELY (pid < 0))
    ਵਾਪਸ crun_make_error (err, errno, "vfork");

  ਜੇ (pid == 0)
    {
      ret = ਤਿਆਰੀ_ਜਾਲ_device_ਵਿੱਚ_ns_helper (ਬਫਰ, ਬਫਰ_ਮਾਪ, netns_fd, newifname, ips, err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          ਸੁਰੱਖਿਅਤ_exit (-ret);
        }

      ਸੁਰੱਖਿਅਤ_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_ਸਥਿਤੀ, 0);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "waitpid for exec child pid");

  ਜੇ (wait_ਸਥਿਤੀ != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      ਵਾਪਸ -get_process_exit_ਸਥਿਤੀ (wait_ਸਥਿਤੀ);
    }

  ਵਾਪਸ 0;
}

ਪੂਰਨ
move_ਜਾਲ_device (ਅਚਲ ਅੱਖਰ *ifname, ਅਚਲ ਅੱਖਰ *newifname, ਪੂਰਨ netns_fd, libcrun_error_t *err)
{
  ਅਚਲ size_t ਬਫਰ_ਮਾਪ = 8192;
  cleanup_ip_addrs ਬਣਤਰ ip_addr *ips = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *ਬਫਰ = xmalloc (ਬਫਰ_ਮਾਪ);
  cleanup_close ਪੂਰਨ sock = -1;
  ਪੂਰਨ ifindex;
  ਪੂਰਨ ret;

  sock = open_netlink_fd (err);
  ਜੇ (sock < 0)
    ਵਾਪਸ sock;

  ifindex = name_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ (sock, ifname, ਬਫਰ, ਬਫਰ_ਮਾਪ, err);
  ਜੇ (UNLIKELY (ifindex < 0))
    ਵਾਪਸ ifindex;

  ret = get_ip_ਪਤੇ (sock, ifindex, &ips, ਬਫਰ, ਬਫਰ_ਮਾਪ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_ਮੰਜ਼ਿਲ_ns_and_wait (sock, ਬਫਰ, ਬਫਰ_ਮਾਪ, ifindex, netns_fd, newifname, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ ਤਿਆਰੀ_ਜਾਲ_device_ਵਿੱਚ_ns (ਬਫਰ, ਬਫਰ_ਮਾਪ, netns_fd, newifname, ips, err);
}
