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

#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "net_device.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

#ಅನ್ತರ್ಭಾವ <sys/socket.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <sched.h>
#ಅನ್ತರ್ಭಾವ <sys/wait.h>
#ಅನ್ತರ್ಭಾವ <sys/xattr.h>
#ಅನ್ತರ್ಭಾವ <linux/netlink.h>
#ಅನ್ತರ್ಭಾವ <linux/rtnetlink.h>
#ಅನ್ತರ್ಭಾವ <arpa/inet.h>
#ಅನ್ತರ್ಭಾವ <net/if.h>
#ಆದರೆ HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint ಅಸ್ಥಿರ uint
#  endif
#ಆದರೆ_ಅಂತ್ಯ

ರಚನೆ ip_addr
{
  ರಚನೆ ifaddrmsg ifa;

  ಪೂರ್ಣಾಂಕ rta_len;
  ಅಕ್ಷರ *rta;
};

ರಚನೆ nl_req
{
  ರಚನೆ nlmsghdr nlh;
  ಒಕ್ಕೂಟ
  {
    ರಚನೆ ifaddrmsg ifa;
    ರಚನೆ ifinfomsg ifi;
  };
};

ಸ್ಥಿರ atomic_uint nl_seq_counter;

ಸ್ಥಿರ uint32_t
get_ಮುಂದಿನ_seq ()
{
  ಹಿಂತಿರುಗಿ (uint32_t) ++nl_seq_counter;
}

ಸ್ಥಿರ uint32_t
reset_request (ರಚನೆ nl_req *req, ಪೂರ್ಣಾಂಕ type, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ ಸಂದೇಶ_len)
{
  uint32_t seq = get_ಮುಂದಿನ_seq ();

  memset (req, 0, ಗಾತ್ರ (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_ಉದ್ದಳತೆ (ಸಂದೇಶ_len);
  req->nlh.nlmsg_seq = seq;

  ಹಿಂತಿರುಗಿ seq;
}

ಸ್ಥಿರ ಶೂನ್ಯ
cleanup_ip_addrsp (ಶೂನ್ಯ *p)
{
  ರಚನೆ ip_addr **pp = (ರಚನೆ ip_addr **) p;
  ರಚನೆ ip_addr *ip;
  ಆದರೆ (*pp == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;

  ಗಾಗಿ (ip = *pp; ip->rta_len >= 0; ip++)
    ಬಿಡುಗಡೆ (ip->rta);

  ಬಿಡುಗಡೆ (*pp);
}

#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ sock = -1;
  ರಚನೆ sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  ಪೂರ್ಣಾಂಕ fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  ಆದರೆ (sock < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "netlink socket");

  ಆದರೆ (bind (sock, (ರಚನೆ sockaddr *) &local, ಗಾತ್ರ (local)) < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "bind");

  fd = sock;
  sock = -1;
  ಹಿಂತಿರುಗಿ fd;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
append_rtattr (ರಚನೆ nlmsghdr *n, size_t maxlen, ಪೂರ್ಣಾಂಕ type, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ, size_t ದತ್ತಾಂಶ_len, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ len = RTA_ಉದ್ದಳತೆ (ದತ್ತಾಂಶ_len);
  ರಚನೆ rtattr *rta;

  ಆದರೆ (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    ಹಿಂತಿರುಗಿ crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (ರಚನೆ rtattr *) (((ಅಕ್ಷರ *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  ಆದರೆ (ದತ್ತಾಂಶ_len)
    ಸ್ಮರಣೆ_ನಕಲು (RTA_ದತ್ತಾಂಶ (rta), ದತ್ತಾಂಶ, ದತ್ತಾಂಶ_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
send_request (ಪೂರ್ಣಾಂಕ sock, ರಚನೆ nl_req *req, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "send");

  ಹಿಂತಿರುಗಿ 0;
}

/* if_nametoindex with an open netlink socket.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
name_ಗಮ್ಯ_ಸೂಚ್ಯಂಕ (ಪೂರ್ಣಾಂಕ sock, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ifname, ಅಕ್ಷರ *ಬಫರ್, size_t ಬಫರ್_ಗಾತ್ರ, libcrun_error_t *err)
{
  ರಚನೆ nlmsghdr *nlh;
  ರಚನೆ nl_req *req;
  uint32_t seq;
  ssize_t len;
  ಪೂರ್ಣಾಂಕ ret;

  req = (ರಚನೆ nl_req *) ಬಫರ್;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, ಗಾತ್ರ (ರಚನೆ ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, ಬಫರ್_ಗಾತ್ರ, IFLA_IFNAME, ifname, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ifname) + 1, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = send_request (sock, req, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  len = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (recv (sock, ಬಫರ್, ಬಫರ್_ಗಾತ್ರ, 0));
  ಆದರೆ (UNLIKELY (len < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "recv");

  ಗಾಗಿ (nlh = (ರಚನೆ nlmsghdr *) ಬಫರ್; NLMSG_OK (nlh, (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) len); nlh = NLMSG_ಮುಂದಿನ (nlh, len))
    {
      ಆದರೆ (nlh->nlmsg_seq != seq)
        ಮುಂದುವರಿ;

      ಆದರೆ (nlh->nlmsg_type == NLMSG_ERROR)
        {
          ರಚನೆ nlmsgerr *err_ದತ್ತಾಂಶ = (ರಚನೆ nlmsgerr *) NLMSG_ದತ್ತಾಂಶ (nlh);
          ಆದರೆ (err_ದತ್ತಾಂಶ->error == 0)
            ಮುಂದುವರಿ;
          ಹಿಂತಿರುಗಿ crun_make_error (err, -err_ದತ್ತಾಂಶ->error, "netlink error while looking for interface `%s`", ifname);
        }

      ಆದರೆ (nlh->nlmsg_type == RTM_NEWLINK)
        {
          ರಚನೆ ifinfomsg *ifi = NLMSG_ದತ್ತಾಂಶ (nlh);
          ಹಿಂತಿರುಗಿ ifi->ifi_ಸೂಚ್ಯಂಕ;
        }
    }

  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not find device `%s`", ifname);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
wait_for_ack (ಪೂರ್ಣಾಂಕ sock, uint32_t seq, ಅಕ್ಷರ *recv_ಬಫರ್, size_t recv_ಬಫರ್_ಗಾತ್ರ, libcrun_error_t *err)
{
  ರಚನೆ nlmsghdr *nlh_recv = (ರಚನೆ nlmsghdr *) recv_ಬಫರ್;
  ssize_t len;

  ಮಾಡು
    {
      len = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (recv (sock, recv_ಬಫರ್, recv_ಬಫರ್_ಗಾತ್ರ, 0));
      ಆದರೆ (len < 0)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "recv");
  } ಆಗ (nlh_recv->nlmsg_seq != seq);

  ಆದರೆ (! NLMSG_OK (nlh_recv, len))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "received invalid packet");

  ಆದರೆ (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      ರಚನೆ nlmsgerr *err_ದತ್ತಾಂಶ = (ರಚನೆ nlmsgerr *) NLMSG_ದತ್ತಾಂಶ (nlh_recv);
      ಆದರೆ (err_ದತ್ತಾಂಶ->error == 0)
        ಹಿಂತಿರುಗಿ 0;
      ಹಿಂತಿರುಗಿ crun_make_error (err, -err_ದತ್ತಾಂಶ->error, "netlink error");
    }

  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: received unknown netlink packet type");
}

ಸ್ಥಿರ ಶೂನ್ಯ
copy_ip_addr (ರಚನೆ nlmsghdr *nlh, ರಚನೆ ip_addr *ip)
{
  ರಚನೆ ifaddrmsg *ifa = (ರಚನೆ ifaddrmsg *) NLMSG_ದತ್ತಾಂಶ (nlh);
  ಸ್ಮರಣೆ_ನಕಲು (&ip->ifa, ifa, ಗಾತ್ರ (ರಚನೆ ifaddrmsg));
  ip->rta_len = IFA_PAYLOAD (nlh);
  ip->rta = xmalloc (ip->rta_len);
  ಸ್ಮರಣೆ_ನಕಲು (ip->rta, IFA_RTA (ifa), ip->rta_len);
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_ip_ವಿಳಾಸಗಳು (ಪೂರ್ಣಾಂಕ sock, uint32_t ifindex, ರಚನೆ ip_addr **ಹೊರಹೋಗು_ips, ಅಕ್ಷರ *ಬಫರ್, size_t ಬಫರ್_ಗಾತ್ರ, libcrun_error_t *err)
{
  ರಚನೆ nl_req *req = (ರಚನೆ nl_req *) ಬಫರ್;
  cleanup_ip_addrs ರಚನೆ ip_addr *ips = ಖಾಲಿ;
  size_t ips_len = 0;
  ಪೂರ್ಣಾಂಕ optval = 1;
  uint32_t seq;
  ssize_t len;
  ಪೂರ್ಣಾಂಕ ret;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, ಗಾತ್ರ (optval));
  ಆದರೆ (ret < 0)
    {
      ಆದರೆ (errno != ENOPROTOOPT)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#ಆದರೆ_ಅಂತ್ಯ

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, ಗಾತ್ರ (ರಚನೆ ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_ಸೂಚ್ಯಂಕ = ifindex;

  ret = send_request (sock, req, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆಗ ((len = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (recv (sock, ಬಫರ್, ಬಫರ್_ಗಾತ್ರ, 0))) > 0)
    {
      ರಚನೆ nlmsghdr *nlh;

      ಗಾಗಿ (nlh = (ರಚನೆ nlmsghdr *) ಬಫರ್; NLMSG_OK (nlh, len); nlh = NLMSG_ಮುಂದಿನ (nlh, len))
        {
          ರಚನೆ ifaddrmsg *ifa;

          ಆದರೆ (nlh->nlmsg_seq != seq)
            ಮುಂದುವರಿ;

          ಆದರೆ (nlh->nlmsg_type == NLMSG_ಮುಗಿದ)
            {
              *ಹೊರಹೋಗು_ips = ips;
              ips = ಖಾಲಿ;
              ಹಿಂತಿರುಗಿ 0;
            }

          ಆದರೆ (nlh->nlmsg_type == NLMSG_ERROR)
            {
              ರಚನೆ nlmsgerr *err_ದತ್ತಾಂಶ = (ರಚನೆ nlmsgerr *) NLMSG_ದತ್ತಾಂಶ (nlh);
              ಆದರೆ (err_ದತ್ತಾಂಶ->error == 0)
                ಮುಂದುವರಿ;
              ಹಿಂತಿರುಗಿ crun_make_error (err, -err_ದತ್ತಾಂಶ->error, "netlink error reading ip addresses");
            }

          ifa = (ರಚನೆ ifaddrmsg *) NLMSG_ದತ್ತಾಂಶ (nlh);
          ಆದರೆ (ifa->ifa_ಸೂಚ್ಯಂಕ != ifindex)
            ಮುಂದುವರಿ;

          /* Copy only permanent, globally routable IP addresses.  */
          ಆದರೆ (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            ಮುಂದುವರಿ;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, ಗಾತ್ರ (ರಚನೆ ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  ಆದರೆ (UNLIKELY (len < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "recv");
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
configure_ip_ವಿಳಾಸಗಳು (ಪೂರ್ಣಾಂಕ sock, ಪೂರ್ಣಾಂಕ ifindex, ಅಕ್ಷರ *ಬಫರ್, size_t ಬಫರ್_ಗಾತ್ರ, ಸ್ಥಿರಾಂಕ ರಚನೆ ip_addr *ips, libcrun_error_t *err)
{
  ರಚನೆ nl_req *req = (ರಚನೆ nl_req *) ಬಫರ್;
  ಸ್ಥಿರಾಂಕ ರಚನೆ ip_addr *ip;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (ips == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ಗಾಗಿ (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      ಪೂರ್ಣಾಂಕ rta_len = ip->rta_len;
      ರಚನೆ rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, ಗಾತ್ರ (ರಚನೆ ifaddrmsg));

      ಸ್ಮರಣೆ_ನಕಲು (&req->ifa, &ip->ifa, ಗಾತ್ರ (ರಚನೆ ifaddrmsg));

      req->ifa.ifa_ಸೂಚ್ಯಂಕ = ifindex;

      ಗಾಗಿ (rta = (ರಚನೆ rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_ಮುಂದಿನ (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), ಬಫರ್_ಗಾತ್ರ, rta->rta_type, RTA_ದತ್ತಾಂಶ (rta), RTA_PAYLOAD (rta), err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }

      ret = send_request (sock, req, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = wait_for_ack (sock, seq, ಬಫರ್, ಬಫರ್_ಗಾತ್ರ, err);
      ಆದರೆ (ret < 0)
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
request_enable_interface_and_wait (ಪೂರ್ಣಾಂಕ sock, ಅಕ್ಷರ *ಬಫರ್, size_t ಬಫರ್_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ, libcrun_error_t *err)
{
  ರಚನೆ nl_req *req = (ರಚನೆ nl_req *) ಬಫರ್;
  uint32_t seq;
  ಪೂರ್ಣಾಂಕ ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, ಗಾತ್ರ (ರಚನೆ ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_ಸೂಚ್ಯಂಕ = ಸೂಚ್ಯಂಕ;

  req->ifi.ifi_flags = IFF_ಮೇಲೆ;
  req->ifi.ifi_change = IFF_ಮೇಲೆ;

  ret = send_request (sock, req, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ wait_for_ack (sock, seq, ಬಫರ್, ಬಫರ್_ಗಾತ್ರ, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಸಿದ್ಧತೆ_ಜಾಲ_device_ಒಳ_ns_helper (ಅಕ್ಷರ *ಬಫರ್, size_t ಬಫರ್_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ netns_fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *newifname,
                                   ರಚನೆ ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ sock_ಒಳ_ns = -1;
  ಪೂರ್ಣಾಂಕ new_ifindex;
  ಪೂರ್ಣಾಂಕ ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "change network namespace");

  sock_ಒಳ_ns = open_netlink_fd (err);
  ಆದರೆ (sock_ಒಳ_ns < 0)
    ಹಿಂತಿರುಗಿ sock_ಒಳ_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_ಗಮ್ಯ_ಸೂಚ್ಯಂಕ (sock_ಒಳ_ns, newifname, ಬಫರ್, ಬಫರ್_ಗಾತ್ರ, err);
  ಆದರೆ (UNLIKELY (new_ifindex < 0))
    ಹಿಂತಿರುಗಿ new_ifindex;

  ret = configure_ip_ವಿಳಾಸಗಳು (sock_ಒಳ_ns, new_ifindex, ಬಫರ್, ಬಫರ್_ಗಾತ್ರ, ips, err);
  ಆದರೆ (ret < 0)
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ request_enable_interface_and_wait (sock_ಒಳ_ns, ಬಫರ್, ಬಫರ್_ಗಾತ್ರ, new_ifindex, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_move_link_ಗಮ್ಯ_ns_and_wait (ಪೂರ್ಣಾಂಕ sock, ಅಕ್ಷರ *ಬಫರ್, size_t ಬಫರ್_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ ifindex, ಪೂರ್ಣಾಂಕ netns_fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *newifname, libcrun_error_t *err)
{
  ರಚನೆ nl_req *req = (ರಚನೆ nl_req *) ಬಫರ್;
  uint32_t seq;
  ಪೂರ್ಣಾಂಕ ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, ಗಾತ್ರ (ರಚನೆ ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_ಸೂಚ್ಯಂಕ = ifindex;

  ret = append_rtattr (&req->nlh, ಬಫರ್_ಗಾತ್ರ, IFLA_NET_NS_FD, &netns_fd, ಗಾತ್ರ (netns_fd), err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = append_rtattr (&req->nlh, ಬಫರ್_ಗಾತ್ರ, IFLA_IFNAME, newifname, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (newifname) + 1, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = send_request (sock, req, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ wait_for_ack (sock, seq, ಬಫರ್, ಬಫರ್_ಗಾತ್ರ, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಸಿದ್ಧತೆ_ಜಾಲ_device_ಒಳ_ns (ಅಕ್ಷರ *ಬಫರ್, size_t ಬಫರ್_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ netns_fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *newifname,
                            ರಚನೆ ip_addr *ips, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ wait_ಸ್ಥಿತಿ;
  pid_t pid;
  ಪೂರ್ಣಾಂಕ ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  ಆದರೆ (UNLIKELY (pid < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "vfork");

  ಆದರೆ (pid == 0)
    {
      ret = ಸಿದ್ಧತೆ_ಜಾಲ_device_ಒಳ_ns_helper (ಬಫರ್, ಬಫರ್_ಗಾತ್ರ, netns_fd, newifname, ips, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          ಸುರಕ್ಷಿತ_exit (-ret);
        }

      ಸುರಕ್ಷಿತ_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_ಸ್ಥಿತಿ, 0);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "waitpid for exec child pid");

  ಆದರೆ (wait_ಸ್ಥಿತಿ != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      ಹಿಂತಿರುಗಿ -get_process_exit_ಸ್ಥಿತಿ (wait_ಸ್ಥಿತಿ);
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
move_ಜಾಲ_device (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ifname, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *newifname, ಪೂರ್ಣಾಂಕ netns_fd, libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ size_t ಬಫರ್_ಗಾತ್ರ = 8192;
  cleanup_ip_addrs ರಚನೆ ip_addr *ips = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *ಬಫರ್ = xmalloc (ಬಫರ್_ಗಾತ್ರ);
  cleanup_close ಪೂರ್ಣಾಂಕ sock = -1;
  ಪೂರ್ಣಾಂಕ ifindex;
  ಪೂರ್ಣಾಂಕ ret;

  sock = open_netlink_fd (err);
  ಆದರೆ (sock < 0)
    ಹಿಂತಿರುಗಿ sock;

  ifindex = name_ಗಮ್ಯ_ಸೂಚ್ಯಂಕ (sock, ifname, ಬಫರ್, ಬಫರ್_ಗಾತ್ರ, err);
  ಆದರೆ (UNLIKELY (ifindex < 0))
    ಹಿಂತಿರುಗಿ ifindex;

  ret = get_ip_ವಿಳಾಸಗಳು (sock, ifindex, &ips, ಬಫರ್, ಬಫರ್_ಗಾತ್ರ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_ಗಮ್ಯ_ns_and_wait (sock, ಬಫರ್, ಬಫರ್_ಗಾತ್ರ, ifindex, netns_fd, newifname, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ ಸಿದ್ಧತೆ_ಜಾಲ_device_ಒಳ_ns (ಬಫರ್, ಬಫರ್_ಗಾತ್ರ, netns_fd, newifname, ips, err);
}
