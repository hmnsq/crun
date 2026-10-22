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

#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश "net_device.h"
#समावेश "utils.h"

#समावेश <sys/socket.h>
#समावेश <errno.h>
#समावेश <sched.h>
#समावेश <sys/wait.h>
#समावेश <sys/xattr.h>
#समावेश <linux/netlink.h>
#समावेश <linux/rtnetlink.h>
#समावेश <arpa/inet.h>
#समावेश <net/if.h>
#अगर HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint अस्थिर uint
#  endif
#पूर्ण_अगर

काष्ठा ip_addr
{
  काष्ठा ifaddrmsg ifa;

  पूर्णांक rta_len;
  अक्षर *rta;
};

काष्ठा nl_req
{
  काष्ठा nlmsghdr nlh;
  जोड़
  {
    काष्ठा ifaddrmsg ifa;
    काष्ठा ifinfomsg ifi;
  };
};

अटल atomic_uint nl_seq_गणक;

अटल uint32_t
get_अग्र_seq ()
{
  वापस (uint32_t) ++nl_seq_गणक;
}

अटल uint32_t
reset_request (काष्ठा nl_req *req, पूर्णांक type, पूर्णांक flags, पूर्णांक संदेश_len)
{
  uint32_t seq = get_अग्र_seq ();

  स_रखो (req, 0, माप (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_लंबाई (संदेश_len);
  req->nlh.nlmsg_seq = seq;

  वापस seq;
}

अटल व्योम
cleanup_ip_addrsp (व्योम *p)
{
  काष्ठा ip_addr **pp = (काष्ठा ip_addr **) p;
  काष्ठा ip_addr *ip;
  अगर (*pp == शून्य)
    वापस;

  क्रम (ip = *pp; ip->rta_len >= 0; ip++)
    मुक्त (ip->rta);

  मुक्त (*pp);
}

#घोषणा cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

अटल पूर्णांक
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close पूर्णांक sock = -1;
  काष्ठा sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  पूर्णांक fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  अगर (sock < 0)
    वापस crun_make_error (err, त्रुटि_सं, "netlink socket");

  अगर (bind (sock, (काष्ठा sockaddr *) &local, माप (local)) < 0)
    वापस crun_make_error (err, त्रुटि_सं, "bind");

  fd = sock;
  sock = -1;
  वापस fd;
}

अटल पूर्णांक
append_rtattr (काष्ठा nlmsghdr *n, माप_प्रकार maxlen, पूर्णांक type, स्थिर व्योम *तथ्य, माप_प्रकार तथ्य_len, libcrun_error_t *err)
{
  पूर्णांक len = RTA_लंबाई (तथ्य_len);
  काष्ठा rtattr *rta;

  अगर (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    वापस crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (काष्ठा rtattr *) (((अक्षर *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  अगर (तथ्य_len)
    स_नकल (RTA_तथ्य (rta), तथ्य, तथ्य_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  वापस 0;
}

अटल पूर्णांक
send_request (पूर्णांक sock, काष्ठा nl_req *req, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = अस्थायी_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "send");

  वापस 0;
}

/* if_nametoindex with an open netlink socket.  */
अटल पूर्णांक
name_to_सूचकांक (पूर्णांक sock, स्थिर अक्षर *ifname, अक्षर *बफर, माप_प्रकार बफर_आकार, libcrun_error_t *err)
{
  काष्ठा nlmsghdr *nlh;
  काष्ठा nl_req *req;
  uint32_t seq;
  ssize_t len;
  पूर्णांक ret;

  req = (काष्ठा nl_req *) बफर;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, माप (काष्ठा ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, बफर_आकार, IFLA_IFNAME, ifname, म_माप (ifname) + 1, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = send_request (sock, req, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  len = अस्थायी_FAILURE_RETRY (recv (sock, बफर, बफर_आकार, 0));
  अगर (UNLIKELY (len < 0))
    वापस crun_make_error (err, त्रुटि_सं, "recv");

  क्रम (nlh = (काष्ठा nlmsghdr *) बफर; NLMSG_OK (nlh, (अचिन्हित पूर्णांक) len); nlh = NLMSG_अग्र (nlh, len))
    {
      अगर (nlh->nlmsg_seq != seq)
        जारी;

      अगर (nlh->nlmsg_type == NLMSG_ERROR)
        {
          काष्ठा nlmsgerr *err_तथ्य = (काष्ठा nlmsgerr *) NLMSG_तथ्य (nlh);
          अगर (err_तथ्य->error == 0)
            जारी;
          वापस crun_make_error (err, -err_तथ्य->error, "netlink error while looking for interface `%s`", ifname);
        }

      अगर (nlh->nlmsg_type == RTM_NEWLINK)
        {
          काष्ठा ifinfomsg *ifi = NLMSG_तथ्य (nlh);
          वापस ifi->ifi_सूचकांक;
        }
    }

  वापस crun_make_error (err, 0, "could not find device `%s`", ifname);
}

अटल पूर्णांक
wait_for_ack (पूर्णांक sock, uint32_t seq, अक्षर *recv_बफर, माप_प्रकार recv_बफर_आकार, libcrun_error_t *err)
{
  काष्ठा nlmsghdr *nlh_recv = (काष्ठा nlmsghdr *) recv_बफर;
  ssize_t len;

  करो
    {
      len = अस्थायी_FAILURE_RETRY (recv (sock, recv_बफर, recv_बफर_आकार, 0));
      अगर (len < 0)
        वापस crun_make_error (err, त्रुटि_सं, "recv");
  } जबतक (nlh_recv->nlmsg_seq != seq);

  अगर (! NLMSG_OK (nlh_recv, len))
    वापस crun_make_error (err, 0, "received invalid packet");

  अगर (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      काष्ठा nlmsgerr *err_तथ्य = (काष्ठा nlmsgerr *) NLMSG_तथ्य (nlh_recv);
      अगर (err_तथ्य->error == 0)
        वापस 0;
      वापस crun_make_error (err, -err_तथ्य->error, "netlink error");
    }

  वापस crun_make_error (err, 0, "internal error: received unknown netlink packet type");
}

अटल व्योम
copy_ip_addr (काष्ठा nlmsghdr *nlh, काष्ठा ip_addr *ip)
{
  काष्ठा ifaddrmsg *ifa = (काष्ठा ifaddrmsg *) NLMSG_तथ्य (nlh);
  स_नकल (&ip->ifa, ifa, माप (काष्ठा ifaddrmsg));
  ip->rta_len = IFA_PAYLOAD (nlh);
  ip->rta = xmalloc (ip->rta_len);
  स_नकल (ip->rta, IFA_RTA (ifa), ip->rta_len);
};

अटल पूर्णांक
get_ip_addresses (पूर्णांक sock, uint32_t ifindex, काष्ठा ip_addr **out_ips, अक्षर *बफर, माप_प्रकार बफर_आकार, libcrun_error_t *err)
{
  काष्ठा nl_req *req = (काष्ठा nl_req *) बफर;
  cleanup_ip_addrs काष्ठा ip_addr *ips = शून्य;
  माप_प्रकार ips_len = 0;
  पूर्णांक optval = 1;
  uint32_t seq;
  ssize_t len;
  पूर्णांक ret;

#अगर_घोषित NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, माप (optval));
  अगर (ret < 0)
    {
      अगर (त्रुटि_सं != ENOPROTOOPT)
        वापस crun_make_error (err, त्रुटि_सं, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#पूर्ण_अगर

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, माप (काष्ठा ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_सूचकांक = ifindex;

  ret = send_request (sock, req, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  जबतक ((len = अस्थायी_FAILURE_RETRY (recv (sock, बफर, बफर_आकार, 0))) > 0)
    {
      काष्ठा nlmsghdr *nlh;

      क्रम (nlh = (काष्ठा nlmsghdr *) बफर; NLMSG_OK (nlh, len); nlh = NLMSG_अग्र (nlh, len))
        {
          काष्ठा ifaddrmsg *ifa;

          अगर (nlh->nlmsg_seq != seq)
            जारी;

          अगर (nlh->nlmsg_type == NLMSG_DONE)
            {
              *out_ips = ips;
              ips = शून्य;
              वापस 0;
            }

          अगर (nlh->nlmsg_type == NLMSG_ERROR)
            {
              काष्ठा nlmsgerr *err_तथ्य = (काष्ठा nlmsgerr *) NLMSG_तथ्य (nlh);
              अगर (err_तथ्य->error == 0)
                जारी;
              वापस crun_make_error (err, -err_तथ्य->error, "netlink error reading ip addresses");
            }

          ifa = (काष्ठा ifaddrmsg *) NLMSG_तथ्य (nlh);
          अगर (ifa->ifa_सूचकांक != ifindex)
            जारी;

          /* Copy only permanent, globally routable IP addresses.  */
          अगर (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            जारी;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, माप (काष्ठा ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  अगर (UNLIKELY (len < 0))
    वापस crun_make_error (err, त्रुटि_सं, "recv");
  वापस 0;
}

अटल पूर्णांक
configure_ip_addresses (पूर्णांक sock, पूर्णांक ifindex, अक्षर *बफर, माप_प्रकार बफर_आकार, स्थिर काष्ठा ip_addr *ips, libcrun_error_t *err)
{
  काष्ठा nl_req *req = (काष्ठा nl_req *) बफर;
  स्थिर काष्ठा ip_addr *ip;
  पूर्णांक ret;

  अगर (ips == शून्य)
    वापस 0;

  क्रम (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      पूर्णांक rta_len = ip->rta_len;
      काष्ठा rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, माप (काष्ठा ifaddrmsg));

      स_नकल (&req->ifa, &ip->ifa, माप (काष्ठा ifaddrmsg));

      req->ifa.ifa_सूचकांक = ifindex;

      क्रम (rta = (काष्ठा rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_अग्र (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), बफर_आकार, rta->rta_type, RTA_तथ्य (rta), RTA_PAYLOAD (rta), err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }

      ret = send_request (sock, req, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = wait_for_ack (sock, seq, बफर, बफर_आकार, err);
      अगर (ret < 0)
        वापस ret;
    }

  वापस 0;
}

अटल पूर्णांक
request_enable_interface_and_wait (पूर्णांक sock, अक्षर *बफर, माप_प्रकार बफर_आकार, पूर्णांक सूचकांक, libcrun_error_t *err)
{
  काष्ठा nl_req *req = (काष्ठा nl_req *) बफर;
  uint32_t seq;
  पूर्णांक ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, माप (काष्ठा ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_सूचकांक = सूचकांक;

  req->ifi.ifi_flags = IFF_UP;
  req->ifi.ifi_change = IFF_UP;

  ret = send_request (sock, req, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस wait_for_ack (sock, seq, बफर, बफर_आकार, err);
}

अटल पूर्णांक
setup_network_device_in_ns_helper (अक्षर *बफर, माप_प्रकार बफर_आकार, पूर्णांक netns_fd, स्थिर अक्षर *newifname,
                                   काष्ठा ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close पूर्णांक sock_in_ns = -1;
  पूर्णांक new_ifindex;
  पूर्णांक ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "change network namespace");

  sock_in_ns = open_netlink_fd (err);
  अगर (sock_in_ns < 0)
    वापस sock_in_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_to_सूचकांक (sock_in_ns, newifname, बफर, बफर_आकार, err);
  अगर (UNLIKELY (new_ifindex < 0))
    वापस new_ifindex;

  ret = configure_ip_addresses (sock_in_ns, new_ifindex, बफर, बफर_आकार, ips, err);
  अगर (ret < 0)
    वापस ret;

  वापस request_enable_interface_and_wait (sock_in_ns, बफर, बफर_आकार, new_ifindex, err);
}

अटल पूर्णांक
do_move_link_to_ns_and_wait (पूर्णांक sock, अक्षर *बफर, माप_प्रकार बफर_आकार, पूर्णांक ifindex, पूर्णांक netns_fd, स्थिर अक्षर *newifname, libcrun_error_t *err)
{
  काष्ठा nl_req *req = (काष्ठा nl_req *) बफर;
  uint32_t seq;
  पूर्णांक ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, माप (काष्ठा ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_सूचकांक = ifindex;

  ret = append_rtattr (&req->nlh, बफर_आकार, IFLA_NET_NS_FD, &netns_fd, माप (netns_fd), err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = append_rtattr (&req->nlh, बफर_आकार, IFLA_IFNAME, newifname, म_माप (newifname) + 1, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = send_request (sock, req, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस wait_for_ack (sock, seq, बफर, बफर_आकार, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
अटल पूर्णांक
setup_network_device_in_ns (अक्षर *बफर, माप_प्रकार बफर_आकार, पूर्णांक netns_fd, स्थिर अक्षर *newifname,
                            काष्ठा ip_addr *ips, libcrun_error_t *err)
{
  पूर्णांक wait_status;
  pid_t pid;
  पूर्णांक ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  अगर (UNLIKELY (pid < 0))
    वापस crun_make_error (err, त्रुटि_सं, "vfork");

  अगर (pid == 0)
    {
      ret = setup_network_device_in_ns_helper (बफर, बफर_आकार, netns_fd, newifname, ips, err);
      अगर (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          _safe_exit (-ret);
        }

      _safe_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "waitpid for exec child pid");

  अगर (wait_status != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      वापस -get_process_exit_status (wait_status);
    }

  वापस 0;
}

पूर्णांक
move_network_device (स्थिर अक्षर *ifname, स्थिर अक्षर *newifname, पूर्णांक netns_fd, libcrun_error_t *err)
{
  स्थिर माप_प्रकार बफर_आकार = 8192;
  cleanup_ip_addrs काष्ठा ip_addr *ips = शून्य;
  cleanup_free अक्षर *बफर = xmalloc (बफर_आकार);
  cleanup_close पूर्णांक sock = -1;
  पूर्णांक ifindex;
  पूर्णांक ret;

  sock = open_netlink_fd (err);
  अगर (sock < 0)
    वापस sock;

  ifindex = name_to_सूचकांक (sock, ifname, बफर, बफर_आकार, err);
  अगर (UNLIKELY (ifindex < 0))
    वापस ifindex;

  ret = get_ip_addresses (sock, ifindex, &ips, बफर, बफर_आकार, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_to_ns_and_wait (sock, बफर, बफर_आकार, ifindex, netns_fd, newifname, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस setup_network_device_in_ns (बफर, बफर_आकार, netns_fd, newifname, ips, err);
}
