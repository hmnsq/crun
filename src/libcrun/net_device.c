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

#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ "net_device.h"
#అన్తర్భావ "utils.h"

#అన్తర్భావ <sys/socket.h>
#అన్తర్భావ <errno.h>
#అన్తర్భావ <sched.h>
#అన్తర్భావ <sys/wait.h>
#అన్తర్భావ <sys/xattr.h>
#అన్తర్భావ <linux/netlink.h>
#అన్తర్భావ <linux/rtnetlink.h>
#అన్తర్భావ <arpa/inet.h>
#అన్తర్భావ <net/if.h>
#ఒకవేళ HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint అస్థిర uint
#  endif
#అయితే_ముగింపు

నిర్మాణం ip_addr
{
  నిర్మాణం ifaddrmsg ifa;

  పూర్ణాంకం rta_len;
  అక్షరం *rta;
};

నిర్మాణం nl_req
{
  నిర్మాణం nlmsghdr nlh;
  సంఘం
  {
    నిర్మాణం ifaddrmsg ifa;
    నిర్మాణం ifinfomsg ifi;
  };
};

స్థిర atomic_uint nl_seq_counter;

స్థిర uint32_t
get_తదుపరి_seq ()
{
  తిరిగి (uint32_t) ++nl_seq_counter;
}

స్థిర uint32_t
reset_request (నిర్మాణం nl_req *req, పూర్ణాంకం type, పూర్ణాంకం flags, పూర్ణాంకం సందేశం_len)
{
  uint32_t seq = get_తదుపరి_seq ();

  memset (req, 0, పరిమాణం (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_నిడివి (సందేశం_len);
  req->nlh.nlmsg_seq = seq;

  తిరిగి seq;
}

స్థిర శూన్య
cleanup_ip_addrsp (శూన్య *p)
{
  నిర్మాణం ip_addr **pp = (నిర్మాణం ip_addr **) p;
  నిర్మాణం ip_addr *ip;
  ఒకవేళ (*pp == ఖాళీ)
    తిరిగి;

  కొరకు (ip = *pp; ip->rta_len >= 0; ip++)
    విడుదల (ip->rta);

  విడుదల (*pp);
}

#నిర్వచించు cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

స్థిర పూర్ణాంకం
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం sock = -1;
  నిర్మాణం sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  పూర్ణాంకం fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  ఒకవేళ (sock < 0)
    తిరిగి crun_make_error (err, errno, "netlink socket");

  ఒకవేళ (bind (sock, (నిర్మాణం sockaddr *) &local, పరిమాణం (local)) < 0)
    తిరిగి crun_make_error (err, errno, "bind");

  fd = sock;
  sock = -1;
  తిరిగి fd;
}

స్థిర పూర్ణాంకం
append_rtattr (నిర్మాణం nlmsghdr *n, size_t maxlen, పూర్ణాంకం type, స్థిరరాశి శూన్య *దత్తాంశం, size_t దత్తాంశం_len, libcrun_error_t *err)
{
  పూర్ణాంకం len = RTA_నిడివి (దత్తాంశం_len);
  నిర్మాణం rtattr *rta;

  ఒకవేళ (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    తిరిగి crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (నిర్మాణం rtattr *) (((అక్షరం *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  ఒకవేళ (దత్తాంశం_len)
    జ్ఞాపకం_నకలు (RTA_దత్తాంశం (rta), దత్తాంశం, దత్తాంశం_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
send_request (పూర్ణాంకం sock, నిర్మాణం nl_req *req, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ret = తాత్కాలిక_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "send");

  తిరిగి 0;
}

/* if_nametoindex with an open netlink socket.  */
స్థిర పూర్ణాంకం
name_గమ్యం_సూచీ (పూర్ణాంకం sock, స్థిరరాశి అక్షరం *ifname, అక్షరం *బఫరు, size_t బఫరు_కొలత, libcrun_error_t *err)
{
  నిర్మాణం nlmsghdr *nlh;
  నిర్మాణం nl_req *req;
  uint32_t seq;
  ssize_t len;
  పూర్ణాంకం ret;

  req = (నిర్మాణం nl_req *) బఫరు;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, పరిమాణం (నిర్మాణం ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, బఫరు_కొలత, IFLA_IFNAME, ifname, అక్షరశ్రేణి_పొడవు (ifname) + 1, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = send_request (sock, req, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  len = తాత్కాలిక_FAILURE_RETRY (recv (sock, బఫరు, బఫరు_కొలత, 0));
  ఒకవేళ (UNLIKELY (len < 0))
    తిరిగి crun_make_error (err, errno, "recv");

  కొరకు (nlh = (నిర్మాణం nlmsghdr *) బఫరు; NLMSG_OK (nlh, (గుర్తులేని పూర్ణాంకం) len); nlh = NLMSG_తదుపరి (nlh, len))
    {
      ఒకవేళ (nlh->nlmsg_seq != seq)
        కొనసాగు;

      ఒకవేళ (nlh->nlmsg_type == NLMSG_ERROR)
        {
          నిర్మాణం nlmsgerr *err_దత్తాంశం = (నిర్మాణం nlmsgerr *) NLMSG_దత్తాంశం (nlh);
          ఒకవేళ (err_దత్తాంశం->error == 0)
            కొనసాగు;
          తిరిగి crun_make_error (err, -err_దత్తాంశం->error, "netlink error while looking for interface `%s`", ifname);
        }

      ఒకవేళ (nlh->nlmsg_type == RTM_NEWLINK)
        {
          నిర్మాణం ifinfomsg *ifi = NLMSG_దత్తాంశం (nlh);
          తిరిగి ifi->ifi_సూచీ;
        }
    }

  తిరిగి crun_make_error (err, 0, "could not find device `%s`", ifname);
}

స్థిర పూర్ణాంకం
wait_for_ack (పూర్ణాంకం sock, uint32_t seq, అక్షరం *recv_బఫరు, size_t recv_బఫరు_కొలత, libcrun_error_t *err)
{
  నిర్మాణం nlmsghdr *nlh_recv = (నిర్మాణం nlmsghdr *) recv_బఫరు;
  ssize_t len;

  చేయి
    {
      len = తాత్కాలిక_FAILURE_RETRY (recv (sock, recv_బఫరు, recv_బఫరు_కొలత, 0));
      ఒకవేళ (len < 0)
        తిరిగి crun_make_error (err, errno, "recv");
  } అయితే (nlh_recv->nlmsg_seq != seq);

  ఒకవేళ (! NLMSG_OK (nlh_recv, len))
    తిరిగి crun_make_error (err, 0, "received invalid packet");

  ఒకవేళ (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      నిర్మాణం nlmsgerr *err_దత్తాంశం = (నిర్మాణం nlmsgerr *) NLMSG_దత్తాంశం (nlh_recv);
      ఒకవేళ (err_దత్తాంశం->error == 0)
        తిరిగి 0;
      తిరిగి crun_make_error (err, -err_దత్తాంశం->error, "netlink error");
    }

  తిరిగి crun_make_error (err, 0, "internal error: received unknown netlink packet type");
}

స్థిర శూన్య
copy_ip_addr (నిర్మాణం nlmsghdr *nlh, నిర్మాణం ip_addr *ip)
{
  నిర్మాణం ifaddrmsg *ifa = (నిర్మాణం ifaddrmsg *) NLMSG_దత్తాంశం (nlh);
  జ్ఞాపకం_నకలు (&ip->ifa, ifa, పరిమాణం (నిర్మాణం ifaddrmsg));
  ip->rta_len = IFA_PAYLOAD (nlh);
  ip->rta = xmalloc (ip->rta_len);
  జ్ఞాపకం_నకలు (ip->rta, IFA_RTA (ifa), ip->rta_len);
};

స్థిర పూర్ణాంకం
get_ip_చిరునామాలు (పూర్ణాంకం sock, uint32_t ifindex, నిర్మాణం ip_addr **బహిర్గతం_ips, అక్షరం *బఫరు, size_t బఫరు_కొలత, libcrun_error_t *err)
{
  నిర్మాణం nl_req *req = (నిర్మాణం nl_req *) బఫరు;
  cleanup_ip_addrs నిర్మాణం ip_addr *ips = ఖాళీ;
  size_t ips_len = 0;
  పూర్ణాంకం optval = 1;
  uint32_t seq;
  ssize_t len;
  పూర్ణాంకం ret;

#నిర్వచితమైతే NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, పరిమాణం (optval));
  ఒకవేళ (ret < 0)
    {
      ఒకవేళ (errno != ENOPROTOOPT)
        తిరిగి crun_make_error (err, errno, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#అయితే_ముగింపు

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, పరిమాణం (నిర్మాణం ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_సూచీ = ifindex;

  ret = send_request (sock, req, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  అయితే ((len = తాత్కాలిక_FAILURE_RETRY (recv (sock, బఫరు, బఫరు_కొలత, 0))) > 0)
    {
      నిర్మాణం nlmsghdr *nlh;

      కొరకు (nlh = (నిర్మాణం nlmsghdr *) బఫరు; NLMSG_OK (nlh, len); nlh = NLMSG_తదుపరి (nlh, len))
        {
          నిర్మాణం ifaddrmsg *ifa;

          ఒకవేళ (nlh->nlmsg_seq != seq)
            కొనసాగు;

          ఒకవేళ (nlh->nlmsg_type == NLMSG_పూర్తి)
            {
              *బహిర్గతం_ips = ips;
              ips = ఖాళీ;
              తిరిగి 0;
            }

          ఒకవేళ (nlh->nlmsg_type == NLMSG_ERROR)
            {
              నిర్మాణం nlmsgerr *err_దత్తాంశం = (నిర్మాణం nlmsgerr *) NLMSG_దత్తాంశం (nlh);
              ఒకవేళ (err_దత్తాంశం->error == 0)
                కొనసాగు;
              తిరిగి crun_make_error (err, -err_దత్తాంశం->error, "netlink error reading ip addresses");
            }

          ifa = (నిర్మాణం ifaddrmsg *) NLMSG_దత్తాంశం (nlh);
          ఒకవేళ (ifa->ifa_సూచీ != ifindex)
            కొనసాగు;

          /* Copy only permanent, globally routable IP addresses.  */
          ఒకవేళ (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            కొనసాగు;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, పరిమాణం (నిర్మాణం ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  ఒకవేళ (UNLIKELY (len < 0))
    తిరిగి crun_make_error (err, errno, "recv");
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
configure_ip_చిరునామాలు (పూర్ణాంకం sock, పూర్ణాంకం ifindex, అక్షరం *బఫరు, size_t బఫరు_కొలత, స్థిరరాశి నిర్మాణం ip_addr *ips, libcrun_error_t *err)
{
  నిర్మాణం nl_req *req = (నిర్మాణం nl_req *) బఫరు;
  స్థిరరాశి నిర్మాణం ip_addr *ip;
  పూర్ణాంకం ret;

  ఒకవేళ (ips == ఖాళీ)
    తిరిగి 0;

  కొరకు (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      పూర్ణాంకం rta_len = ip->rta_len;
      నిర్మాణం rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, పరిమాణం (నిర్మాణం ifaddrmsg));

      జ్ఞాపకం_నకలు (&req->ifa, &ip->ifa, పరిమాణం (నిర్మాణం ifaddrmsg));

      req->ifa.ifa_సూచీ = ifindex;

      కొరకు (rta = (నిర్మాణం rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_తదుపరి (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), బఫరు_కొలత, rta->rta_type, RTA_దత్తాంశం (rta), RTA_PAYLOAD (rta), err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }

      ret = send_request (sock, req, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = wait_for_ack (sock, seq, బఫరు, బఫరు_కొలత, err);
      ఒకవేళ (ret < 0)
        తిరిగి ret;
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
request_enable_interface_and_wait (పూర్ణాంకం sock, అక్షరం *బఫరు, size_t బఫరు_కొలత, పూర్ణాంకం సూచీ, libcrun_error_t *err)
{
  నిర్మాణం nl_req *req = (నిర్మాణం nl_req *) బఫరు;
  uint32_t seq;
  పూర్ణాంకం ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, పరిమాణం (నిర్మాణం ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_సూచీ = సూచీ;

  req->ifi.ifi_flags = IFF_పైకి;
  req->ifi.ifi_change = IFF_పైకి;

  ret = send_request (sock, req, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి wait_for_ack (sock, seq, బఫరు, బఫరు_కొలత, err);
}

స్థిర పూర్ణాంకం
అమరిక_జాలం_device_లోపల_ns_helper (అక్షరం *బఫరు, size_t బఫరు_కొలత, పూర్ణాంకం netns_fd, స్థిరరాశి అక్షరం *newifname,
                                   నిర్మాణం ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం sock_లోపల_ns = -1;
  పూర్ణాంకం new_ifindex;
  పూర్ణాంకం ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "change network namespace");

  sock_లోపల_ns = open_netlink_fd (err);
  ఒకవేళ (sock_లోపల_ns < 0)
    తిరిగి sock_లోపల_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_గమ్యం_సూచీ (sock_లోపల_ns, newifname, బఫరు, బఫరు_కొలత, err);
  ఒకవేళ (UNLIKELY (new_ifindex < 0))
    తిరిగి new_ifindex;

  ret = configure_ip_చిరునామాలు (sock_లోపల_ns, new_ifindex, బఫరు, బఫరు_కొలత, ips, err);
  ఒకవేళ (ret < 0)
    తిరిగి ret;

  తిరిగి request_enable_interface_and_wait (sock_లోపల_ns, బఫరు, బఫరు_కొలత, new_ifindex, err);
}

స్థిర పూర్ణాంకం
do_move_link_గమ్యం_ns_and_wait (పూర్ణాంకం sock, అక్షరం *బఫరు, size_t బఫరు_కొలత, పూర్ణాంకం ifindex, పూర్ణాంకం netns_fd, స్థిరరాశి అక్షరం *newifname, libcrun_error_t *err)
{
  నిర్మాణం nl_req *req = (నిర్మాణం nl_req *) బఫరు;
  uint32_t seq;
  పూర్ణాంకం ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, పరిమాణం (నిర్మాణం ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_సూచీ = ifindex;

  ret = append_rtattr (&req->nlh, బఫరు_కొలత, IFLA_NET_NS_FD, &netns_fd, పరిమాణం (netns_fd), err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = append_rtattr (&req->nlh, బఫరు_కొలత, IFLA_IFNAME, newifname, అక్షరశ్రేణి_పొడవు (newifname) + 1, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = send_request (sock, req, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి wait_for_ack (sock, seq, బఫరు, బఫరు_కొలత, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
స్థిర పూర్ణాంకం
అమరిక_జాలం_device_లోపల_ns (అక్షరం *బఫరు, size_t బఫరు_కొలత, పూర్ణాంకం netns_fd, స్థిరరాశి అక్షరం *newifname,
                            నిర్మాణం ip_addr *ips, libcrun_error_t *err)
{
  పూర్ణాంకం wait_స్థితి;
  pid_t pid;
  పూర్ణాంకం ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  ఒకవేళ (UNLIKELY (pid < 0))
    తిరిగి crun_make_error (err, errno, "vfork");

  ఒకవేళ (pid == 0)
    {
      ret = అమరిక_జాలం_device_లోపల_ns_helper (బఫరు, బఫరు_కొలత, netns_fd, newifname, ips, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          సురక్షిత_exit (-ret);
        }

      సురక్షిత_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_స్థితి, 0);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "waitpid for exec child pid");

  ఒకవేళ (wait_స్థితి != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      తిరిగి -get_process_exit_స్థితి (wait_స్థితి);
    }

  తిరిగి 0;
}

పూర్ణాంకం
move_జాలం_device (స్థిరరాశి అక్షరం *ifname, స్థిరరాశి అక్షరం *newifname, పూర్ణాంకం netns_fd, libcrun_error_t *err)
{
  స్థిరరాశి size_t బఫరు_కొలత = 8192;
  cleanup_ip_addrs నిర్మాణం ip_addr *ips = ఖాళీ;
  cleanup_free అక్షరం *బఫరు = xmalloc (బఫరు_కొలత);
  cleanup_close పూర్ణాంకం sock = -1;
  పూర్ణాంకం ifindex;
  పూర్ణాంకం ret;

  sock = open_netlink_fd (err);
  ఒకవేళ (sock < 0)
    తిరిగి sock;

  ifindex = name_గమ్యం_సూచీ (sock, ifname, బఫరు, బఫరు_కొలత, err);
  ఒకవేళ (UNLIKELY (ifindex < 0))
    తిరిగి ifindex;

  ret = get_ip_చిరునామాలు (sock, ifindex, &ips, బఫరు, బఫరు_కొలత, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_గమ్యం_ns_and_wait (sock, బఫరు, బఫరు_కొలత, ifindex, netns_fd, newifname, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి అమరిక_జాలం_device_లోపల_ns (బఫరు, బఫరు_కొలత, netns_fd, newifname, ips, err);
}
