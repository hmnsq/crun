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

#परिभाषय _GNU_SOURCE

#अन्तर्भाव <config.h>
#अन्तर्भाव "net_device.h"
#अन्तर्भाव "utils.h"

#अन्तर्भाव <sys/socket.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <sched.h>
#अन्तर्भाव <sys/wait.h>
#अन्तर्भाव <sys/xattr.h>
#अन्तर्भाव <linux/netlink.h>
#अन्तर्भाव <linux/rtnetlink.h>
#अन्तर्भाव <arpa/inet.h>
#अन्तर्भाव <net/if.h>
#यदि HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint चञ्चल uint
#  endif
#यदि_अन्तः

संरचना ip_addr
{
  संरचना ifaddrmsg ifa;

  पूर्णाङ्क rta_len;
  वर्ण *rta;
};

संरचना nl_req
{
  संरचना nlmsghdr nlh;
  सङ्घ
  {
    संरचना ifaddrmsg ifa;
    संरचना ifinfomsg ifi;
  };
};

स्थिर atomic_uint nl_seq_गणक;

स्थिर uint32_t
get_अग्र_seq ()
{
  प्रत्यागम (uint32_t) ++nl_seq_गणक;
}

स्थिर uint32_t
reset_request (संरचना nl_req *req, पूर्णाङ्क type, पूर्णाङ्क flags, पूर्णाङ्क सन्देशः_len)
{
  uint32_t seq = get_अग्र_seq ();

  स्मृतिं_पूरय (req, 0, परिमाण (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_दीर्घता (सन्देशः_len);
  req->nlh.nlmsg_seq = seq;

  प्रत्यागम seq;
}

स्थिर शून्य
cleanup_ip_addrsp (शून्य *p)
{
  संरचना ip_addr **pp = (संरचना ip_addr **) p;
  संरचना ip_addr *ip;
  यदि (*pp == रिक्त)
    प्रत्यागम;

  प्रति (ip = *pp; ip->rta_len >= 0; ip++)
    मुक्त (ip->rta);

  मुक्त (*pp);
}

#परिभाषय cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

स्थिर पूर्णाङ्क
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क sock = -1;
  संरचना sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  पूर्णाङ्क fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  यदि (sock < 0)
    प्रत्यागम crun_make_error (err, errno, "netlink socket");

  यदि (bind (sock, (संरचना sockaddr *) &local, परिमाण (local)) < 0)
    प्रत्यागम crun_make_error (err, errno, "bind");

  fd = sock;
  sock = -1;
  प्रत्यागम fd;
}

स्थिर पूर्णाङ्क
append_rtattr (संरचना nlmsghdr *n, size_t maxlen, पूर्णाङ्क type, ध्रुव शून्य *दत्त, size_t दत्त_len, libcrun_error_t *err)
{
  पूर्णाङ्क len = RTA_दीर्घता (दत्त_len);
  संरचना rtattr *rta;

  यदि (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    प्रत्यागम crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (संरचना rtattr *) (((वर्ण *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  यदि (दत्त_len)
    स्मृतिं_प्रतिलिख (RTA_दत्त (rta), दत्त, दत्त_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
send_request (पूर्णाङ्क sock, संरचना nl_req *req, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = अस्थायी_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "send");

  प्रत्यागम 0;
}

/* if_nametoindex with an open netlink socket.  */
स्थिर पूर्णाङ्क
name_गन्तव्यम्_सूचकांक (पूर्णाङ्क sock, ध्रुव वर्ण *ifname, वर्ण *बफर, size_t बफर_आकार, libcrun_error_t *err)
{
  संरचना nlmsghdr *nlh;
  संरचना nl_req *req;
  uint32_t seq;
  ssize_t len;
  पूर्णाङ्क ret;

  req = (संरचना nl_req *) बफर;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, परिमाण (संरचना ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, बफर_आकार, IFLA_IFNAME, ifname, वाक्यदैर्घ्यम् (ifname) + 1, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = send_request (sock, req, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  len = अस्थायी_FAILURE_RETRY (recv (sock, बफर, बफर_आकार, 0));
  यदि (UNLIKELY (len < 0))
    प्रत्यागम crun_make_error (err, errno, "recv");

  प्रति (nlh = (संरचना nlmsghdr *) बफर; NLMSG_OK (nlh, (अचिह्नित पूर्णाङ्क) len); nlh = NLMSG_अग्र (nlh, len))
    {
      यदि (nlh->nlmsg_seq != seq)
        अनुवर्त;

      यदि (nlh->nlmsg_type == NLMSG_ERROR)
        {
          संरचना nlmsgerr *err_दत्त = (संरचना nlmsgerr *) NLMSG_दत्त (nlh);
          यदि (err_दत्त->error == 0)
            अनुवर्त;
          प्रत्यागम crun_make_error (err, -err_दत्त->error, "netlink error while looking for interface `%s`", ifname);
        }

      यदि (nlh->nlmsg_type == RTM_NEWLINK)
        {
          संरचना ifinfomsg *ifi = NLMSG_दत्त (nlh);
          प्रत्यागम ifi->ifi_सूचकांक;
        }
    }

  प्रत्यागम crun_make_error (err, 0, "could not find device `%s`", ifname);
}

स्थिर पूर्णाङ्क
wait_for_ack (पूर्णाङ्क sock, uint32_t seq, वर्ण *recv_बफर, size_t recv_बफर_आकार, libcrun_error_t *err)
{
  संरचना nlmsghdr *nlh_recv = (संरचना nlmsghdr *) recv_बफर;
  ssize_t len;

  कुरु
    {
      len = अस्थायी_FAILURE_RETRY (recv (sock, recv_बफर, recv_बफर_आकार, 0));
      यदि (len < 0)
        प्रत्यागम crun_make_error (err, errno, "recv");
  } यावत् (nlh_recv->nlmsg_seq != seq);

  यदि (! NLMSG_OK (nlh_recv, len))
    प्रत्यागम crun_make_error (err, 0, "received invalid packet");

  यदि (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      संरचना nlmsgerr *err_दत्त = (संरचना nlmsgerr *) NLMSG_दत्त (nlh_recv);
      यदि (err_दत्त->error == 0)
        प्रत्यागम 0;
      प्रत्यागम crun_make_error (err, -err_दत्त->error, "netlink error");
    }

  प्रत्यागम crun_make_error (err, 0, "internal error: received unknown netlink packet type");
}

स्थिर शून्य
copy_ip_addr (संरचना nlmsghdr *nlh, संरचना ip_addr *ip)
{
  संरचना ifaddrmsg *ifa = (संरचना ifaddrmsg *) NLMSG_दत्त (nlh);
  स्मृतिं_प्रतिलिख (&ip->ifa, ifa, परिमाण (संरचना ifaddrmsg));
  ip->rta_len = IFA_PAYLOAD (nlh);
  ip->rta = xmalloc (ip->rta_len);
  स्मृतिं_प्रतिलिख (ip->rta, IFA_RTA (ifa), ip->rta_len);
};

स्थिर पूर्णाङ्क
get_ip_स्थानाङ्काः (पूर्णाङ्क sock, uint32_t ifindex, संरचना ip_addr **बहिर्गमनम्_ips, वर्ण *बफर, size_t बफर_आकार, libcrun_error_t *err)
{
  संरचना nl_req *req = (संरचना nl_req *) बफर;
  cleanup_ip_addrs संरचना ip_addr *ips = रिक्त;
  size_t ips_len = 0;
  पूर्णाङ्क optval = 1;
  uint32_t seq;
  ssize_t len;
  पूर्णाङ्क ret;

#यदि_परिभाषितम् NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, परिमाण (optval));
  यदि (ret < 0)
    {
      यदि (errno != ENOPROTOOPT)
        प्रत्यागम crun_make_error (err, errno, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#यदि_अन्तः

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, परिमाण (संरचना ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_सूचकांक = ifindex;

  ret = send_request (sock, req, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यावत् ((len = अस्थायी_FAILURE_RETRY (recv (sock, बफर, बफर_आकार, 0))) > 0)
    {
      संरचना nlmsghdr *nlh;

      प्रति (nlh = (संरचना nlmsghdr *) बफर; NLMSG_OK (nlh, len); nlh = NLMSG_अग्र (nlh, len))
        {
          संरचना ifaddrmsg *ifa;

          यदि (nlh->nlmsg_seq != seq)
            अनुवर्त;

          यदि (nlh->nlmsg_type == NLMSG_समाप्तम्)
            {
              *बहिर्गमनम्_ips = ips;
              ips = रिक्त;
              प्रत्यागम 0;
            }

          यदि (nlh->nlmsg_type == NLMSG_ERROR)
            {
              संरचना nlmsgerr *err_दत्त = (संरचना nlmsgerr *) NLMSG_दत्त (nlh);
              यदि (err_दत्त->error == 0)
                अनुवर्त;
              प्रत्यागम crun_make_error (err, -err_दत्त->error, "netlink error reading ip addresses");
            }

          ifa = (संरचना ifaddrmsg *) NLMSG_दत्त (nlh);
          यदि (ifa->ifa_सूचकांक != ifindex)
            अनुवर्त;

          /* Copy only permanent, globally routable IP addresses.  */
          यदि (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            अनुवर्त;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, परिमाण (संरचना ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  यदि (UNLIKELY (len < 0))
    प्रत्यागम crun_make_error (err, errno, "recv");
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
configure_ip_स्थानाङ्काः (पूर्णाङ्क sock, पूर्णाङ्क ifindex, वर्ण *बफर, size_t बफर_आकार, ध्रुव संरचना ip_addr *ips, libcrun_error_t *err)
{
  संरचना nl_req *req = (संरचना nl_req *) बफर;
  ध्रुव संरचना ip_addr *ip;
  पूर्णाङ्क ret;

  यदि (ips == रिक्त)
    प्रत्यागम 0;

  प्रति (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      पूर्णाङ्क rta_len = ip->rta_len;
      संरचना rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, परिमाण (संरचना ifaddrmsg));

      स्मृतिं_प्रतिलिख (&req->ifa, &ip->ifa, परिमाण (संरचना ifaddrmsg));

      req->ifa.ifa_सूचकांक = ifindex;

      प्रति (rta = (संरचना rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_अग्र (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), बफर_आकार, rta->rta_type, RTA_दत्त (rta), RTA_PAYLOAD (rta), err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }

      ret = send_request (sock, req, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = wait_for_ack (sock, seq, बफर, बफर_आकार, err);
      यदि (ret < 0)
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
request_enable_interface_and_wait (पूर्णाङ्क sock, वर्ण *बफर, size_t बफर_आकार, पूर्णाङ्क सूचकांक, libcrun_error_t *err)
{
  संरचना nl_req *req = (संरचना nl_req *) बफर;
  uint32_t seq;
  पूर्णाङ्क ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, परिमाण (संरचना ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_सूचकांक = सूचकांक;

  req->ifi.ifi_flags = IFF_ऊर्ध्वम्;
  req->ifi.ifi_change = IFF_ऊर्ध्वम्;

  ret = send_request (sock, req, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम wait_for_ack (sock, seq, बफर, बफर_आकार, err);
}

स्थिर पूर्णाङ्क
सज्जता_जालम्_device_अन्तर्_ns_helper (वर्ण *बफर, size_t बफर_आकार, पूर्णाङ्क netns_fd, ध्रुव वर्ण *newifname,
                                   संरचना ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क sock_अन्तर्_ns = -1;
  पूर्णाङ्क new_ifindex;
  पूर्णाङ्क ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "change network namespace");

  sock_अन्तर्_ns = open_netlink_fd (err);
  यदि (sock_अन्तर्_ns < 0)
    प्रत्यागम sock_अन्तर्_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_गन्तव्यम्_सूचकांक (sock_अन्तर्_ns, newifname, बफर, बफर_आकार, err);
  यदि (UNLIKELY (new_ifindex < 0))
    प्रत्यागम new_ifindex;

  ret = configure_ip_स्थानाङ्काः (sock_अन्तर्_ns, new_ifindex, बफर, बफर_आकार, ips, err);
  यदि (ret < 0)
    प्रत्यागम ret;

  प्रत्यागम request_enable_interface_and_wait (sock_अन्तर्_ns, बफर, बफर_आकार, new_ifindex, err);
}

स्थिर पूर्णाङ्क
do_move_link_गन्तव्यम्_ns_and_wait (पूर्णाङ्क sock, वर्ण *बफर, size_t बफर_आकार, पूर्णाङ्क ifindex, पूर्णाङ्क netns_fd, ध्रुव वर्ण *newifname, libcrun_error_t *err)
{
  संरचना nl_req *req = (संरचना nl_req *) बफर;
  uint32_t seq;
  पूर्णाङ्क ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, परिमाण (संरचना ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_सूचकांक = ifindex;

  ret = append_rtattr (&req->nlh, बफर_आकार, IFLA_NET_NS_FD, &netns_fd, परिमाण (netns_fd), err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = append_rtattr (&req->nlh, बफर_आकार, IFLA_IFNAME, newifname, वाक्यदैर्घ्यम् (newifname) + 1, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = send_request (sock, req, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम wait_for_ack (sock, seq, बफर, बफर_आकार, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
स्थिर पूर्णाङ्क
सज्जता_जालम्_device_अन्तर्_ns (वर्ण *बफर, size_t बफर_आकार, पूर्णाङ्क netns_fd, ध्रुव वर्ण *newifname,
                            संरचना ip_addr *ips, libcrun_error_t *err)
{
  पूर्णाङ्क wait_स्थितिः;
  pid_t pid;
  पूर्णाङ्क ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  यदि (UNLIKELY (pid < 0))
    प्रत्यागम crun_make_error (err, errno, "vfork");

  यदि (pid == 0)
    {
      ret = सज्जता_जालम्_device_अन्तर्_ns_helper (बफर, बफर_आकार, netns_fd, newifname, ips, err);
      यदि (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          _safe_exit (-ret);
        }

      _safe_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_स्थितिः, 0);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "waitpid for exec child pid");

  यदि (wait_स्थितिः != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      प्रत्यागम -get_process_exit_स्थितिः (wait_स्थितिः);
    }

  प्रत्यागम 0;
}

पूर्णाङ्क
move_जालम्_device (ध्रुव वर्ण *ifname, ध्रुव वर्ण *newifname, पूर्णाङ्क netns_fd, libcrun_error_t *err)
{
  ध्रुव size_t बफर_आकार = 8192;
  cleanup_ip_addrs संरचना ip_addr *ips = रिक्त;
  cleanup_free वर्ण *बफर = xmalloc (बफर_आकार);
  cleanup_close पूर्णाङ्क sock = -1;
  पूर्णाङ्क ifindex;
  पूर्णाङ्क ret;

  sock = open_netlink_fd (err);
  यदि (sock < 0)
    प्रत्यागम sock;

  ifindex = name_गन्तव्यम्_सूचकांक (sock, ifname, बफर, बफर_आकार, err);
  यदि (UNLIKELY (ifindex < 0))
    प्रत्यागम ifindex;

  ret = get_ip_स्थानाङ्काः (sock, ifindex, &ips, बफर, बफर_आकार, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_गन्तव्यम्_ns_and_wait (sock, बफर, बफर_आकार, ifindex, netns_fd, newifname, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम सज्जता_जालम्_device_अन्तर्_ns (बफर, बफर_आकार, netns_fd, newifname, ips, err);
}
