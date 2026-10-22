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
#जर HAVE_STDATOMIC_H
#  include <stdatomic.h>
#  ifndef HAVE_ATOMIC_INT
#    define atomic_uint अस्थिर uint
#  endif
#endif

संरचना ip_addr
{
  संरचना ifaddrmsg ifa;

  पूर्णांक rta_len;
  अक्षर *rta;
};

संरचना nl_req
{
  संरचना nlmsghdr nlh;
  संघ
  {
    संरचना ifaddrmsg ifa;
    संरचना ifinfomsg ifi;
  };
};

स्थिर atomic_uint nl_seq_counter;

स्थिर uint32_t
get_next_seq ()
{
  परत (uint32_t) ++nl_seq_counter;
}

स्थिर uint32_t
reset_request (संरचना nl_req *req, पूर्णांक type, पूर्णांक flags, पूर्णांक संदेश_len)
{
  uint32_t seq = get_next_seq ();

  स_रखो (req, 0, आकार (*req));
  req->nlh.nlmsg_type = type;
  req->nlh.nlmsg_flags = flags;
  req->nlh.nlmsg_len = NLMSG_LENGTH (संदेश_len);
  req->nlh.nlmsg_seq = seq;

  परत seq;
}

स्थिर शून्य
cleanup_ip_addrsp (शून्य *p)
{
  संरचना ip_addr **pp = (संरचना ip_addr **) p;
  संरचना ip_addr *ip;
  जर (*pp == रिक्त)
    परत;

  साठी (ip = *pp; ip->rta_len >= 0; ip++)
    मुक्त (ip->rta);

  मुक्त (*pp);
}

#define cleanup_ip_addrs __attribute__ ((cleanup (cleanup_ip_addrsp)))

स्थिर पूर्णांक
open_netlink_fd (libcrun_error_t *err)
{
  cleanup_close पूर्णांक sock = -1;
  संरचना sockaddr_nl local = {
    .nl_family = AF_NETLINK
  };
  पूर्णांक fd;

  sock = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  जर (sock < 0)
    परत crun_make_error (err, त्रुटि_सं, "netlink socket");

  जर (bind (sock, (संरचना sockaddr *) &local, आकार (local)) < 0)
    परत crun_make_error (err, त्रुटि_सं, "bind");

  fd = sock;
  sock = -1;
  परत fd;
}

स्थिर पूर्णांक
append_rtattr (संरचना nlmsghdr *n, माप_प्रकार maxlen, पूर्णांक type, स्थिरांक शून्य *data, माप_प्रकार data_len, libcrun_error_t *err)
{
  पूर्णांक len = RTA_LENGTH (data_len);
  संरचना rtattr *rta;

  जर (NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len) > maxlen)
    परत crun_make_error (err, E2BIG, "internal error: buffer too small");

  rta = (संरचना rtattr *) (((अक्षर *) n) + NLMSG_ALIGN (n->nlmsg_len));
  rta->rta_type = type;
  rta->rta_len = len;
  जर (data_len)
    स_नकल (RTA_DATA (rta), data, data_len);
  n->nlmsg_len = NLMSG_ALIGN (n->nlmsg_len) + RTA_ALIGN (len);
  परत 0;
}

स्थिर पूर्णांक
send_request (पूर्णांक sock, संरचना nl_req *req, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = TEMP_FAILURE_RETRY (send (sock, req, req->nlh.nlmsg_len, 0));
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "send");

  परत 0;
}

/* if_nametoindex with an open netlink socket.  */
स्थिर पूर्णांक
name_to_index (पूर्णांक sock, स्थिरांक अक्षर *ifname, अक्षर *buffer, माप_प्रकार buffer_size, libcrun_error_t *err)
{
  संरचना nlmsghdr *nlh;
  संरचना nl_req *req;
  uint32_t seq;
  ssize_t len;
  पूर्णांक ret;

  req = (संरचना nl_req *) buffer;

  nlh = &req->nlh;

  seq = reset_request (req, RTM_GETLINK, NLM_F_REQUEST, आकार (संरचना ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;

  ret = append_rtattr (nlh, buffer_size, IFLA_IFNAME, ifname, म_माप (ifname) + 1, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = send_request (sock, req, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0));
  जर (UNLIKELY (len < 0))
    परत crun_make_error (err, त्रुटि_सं, "recv");

  साठी (nlh = (संरचना nlmsghdr *) buffer; NLMSG_OK (nlh, (अचिन्हित पूर्णांक) len); nlh = NLMSG_NEXT (nlh, len))
    {
      जर (nlh->nlmsg_seq != seq)
        सुरूठेवा;

      जर (nlh->nlmsg_type == NLMSG_ERROR)
        {
          संरचना nlmsgerr *err_data = (संरचना nlmsgerr *) NLMSG_DATA (nlh);
          जर (err_data->error == 0)
            सुरूठेवा;
          परत crun_make_error (err, -err_data->error, "netlink error while looking for interface `%s`", ifname);
        }

      जर (nlh->nlmsg_type == RTM_NEWLINK)
        {
          संरचना ifinfomsg *ifi = NLMSG_DATA (nlh);
          परत ifi->ifi_index;
        }
    }

  परत crun_make_error (err, 0, "could not find device `%s`", ifname);
}

स्थिर पूर्णांक
wait_for_ack (पूर्णांक sock, uint32_t seq, अक्षर *recv_buffer, माप_प्रकार recv_buffer_size, libcrun_error_t *err)
{
  संरचना nlmsghdr *nlh_recv = (संरचना nlmsghdr *) recv_buffer;
  ssize_t len;

  करा
    {
      len = TEMP_FAILURE_RETRY (recv (sock, recv_buffer, recv_buffer_size, 0));
      जर (len < 0)
        परत crun_make_error (err, त्रुटि_सं, "recv");
  } पर्यंत (nlh_recv->nlmsg_seq != seq);

  जर (! NLMSG_OK (nlh_recv, len))
    परत crun_make_error (err, 0, "received invalid packet");

  जर (nlh_recv->nlmsg_type == NLMSG_ERROR)
    {
      संरचना nlmsgerr *err_data = (संरचना nlmsgerr *) NLMSG_DATA (nlh_recv);
      जर (err_data->error == 0)
        परत 0;
      परत crun_make_error (err, -err_data->error, "netlink error");
    }

  परत crun_make_error (err, 0, "internal error: received unknown netlink packet type");
}

स्थिर शून्य
copy_ip_addr (संरचना nlmsghdr *nlh, संरचना ip_addr *ip)
{
  संरचना ifaddrmsg *ifa = (संरचना ifaddrmsg *) NLMSG_DATA (nlh);
  स_नकल (&ip->ifa, ifa, आकार (संरचना ifaddrmsg));
  ip->rta_len = IFA_PAYLOAD (nlh);
  ip->rta = xmalloc (ip->rta_len);
  स_नकल (ip->rta, IFA_RTA (ifa), ip->rta_len);
};

स्थिर पूर्णांक
get_ip_addresses (पूर्णांक sock, uint32_t ifindex, संरचना ip_addr **out_ips, अक्षर *buffer, माप_प्रकार buffer_size, libcrun_error_t *err)
{
  संरचना nl_req *req = (संरचना nl_req *) buffer;
  cleanup_ip_addrs संरचना ip_addr *ips = रिक्त;
  माप_प्रकार ips_len = 0;
  पूर्णांक optval = 1;
  uint32_t seq;
  ssize_t len;
  पूर्णांक ret;

#ifdef NETLINK_GET_STRICT_CHK
  ret = setsockopt (sock, SOL_NETLINK, NETLINK_GET_STRICT_CHK, &optval, आकार (optval));
  जर (ret < 0)
    {
      जर (त्रुटि_सं != ENOPROTOOPT)
        परत crun_make_error (err, त्रुटि_सं, "setsockopt (NETLINK_GET_STRICT_CHK)");

      /* NETLINK_GET_STRICT_CHK not supported by this kernel, continue without strict checking.   */
    }
#endif

  seq = reset_request (req, RTM_GETADDR, NLM_F_DUMP | NLM_F_REQUEST, आकार (संरचना ifaddrmsg));
  req->ifa.ifa_family = AF_UNSPEC;
  req->ifa.ifa_index = ifindex;

  ret = send_request (sock, req, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  पर्यंत ((len = TEMP_FAILURE_RETRY (recv (sock, buffer, buffer_size, 0))) > 0)
    {
      संरचना nlmsghdr *nlh;

      साठी (nlh = (संरचना nlmsghdr *) buffer; NLMSG_OK (nlh, len); nlh = NLMSG_NEXT (nlh, len))
        {
          संरचना ifaddrmsg *ifa;

          जर (nlh->nlmsg_seq != seq)
            सुरूठेवा;

          जर (nlh->nlmsg_type == NLMSG_DONE)
            {
              *out_ips = ips;
              ips = रिक्त;
              परत 0;
            }

          जर (nlh->nlmsg_type == NLMSG_ERROR)
            {
              संरचना nlmsgerr *err_data = (संरचना nlmsgerr *) NLMSG_DATA (nlh);
              जर (err_data->error == 0)
                सुरूठेवा;
              परत crun_make_error (err, -err_data->error, "netlink error reading ip addresses");
            }

          ifa = (संरचना ifaddrmsg *) NLMSG_DATA (nlh);
          जर (ifa->ifa_index != ifindex)
            सुरूठेवा;

          /* Copy only permanent, globally routable IP addresses.  */
          जर (! (ifa->ifa_flags & IFA_F_PERMANENT) || (ifa->ifa_scope != RT_SCOPE_UNIVERSE))
            सुरूठेवा;

          /* Always append an empty struct.  */
          ips = xrealloc (ips, आकार (संरचना ip_addr) * (++ips_len + 1));
          /* Mark the end of the array.  */
          ips[ips_len].rta_len = -1;

          copy_ip_addr (nlh, &ips[ips_len - 1]);
        }
    }
  जर (UNLIKELY (len < 0))
    परत crun_make_error (err, त्रुटि_सं, "recv");
  परत 0;
}

स्थिर पूर्णांक
configure_ip_addresses (पूर्णांक sock, पूर्णांक ifindex, अक्षर *buffer, माप_प्रकार buffer_size, स्थिरांक संरचना ip_addr *ips, libcrun_error_t *err)
{
  संरचना nl_req *req = (संरचना nl_req *) buffer;
  स्थिरांक संरचना ip_addr *ip;
  पूर्णांक ret;

  जर (ips == रिक्त)
    परत 0;

  साठी (ip = ips; ip->rta_len >= 0; ip++)
    {
      /* RTA_NEXT modifies the argument, so use a copy.  */
      पूर्णांक rta_len = ip->rta_len;
      संरचना rtattr *rta;
      uint32_t seq;

      seq = reset_request (req, RTM_NEWADDR, NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE | NLM_F_ACK, आकार (संरचना ifaddrmsg));

      स_नकल (&req->ifa, &ip->ifa, आकार (संरचना ifaddrmsg));

      req->ifa.ifa_index = ifindex;

      साठी (rta = (संरचना rtattr *) ip->rta; RTA_OK (rta, rta_len); rta = RTA_NEXT (rta, rta_len))
        {
          ret = append_rtattr (&(req->nlh), buffer_size, rta->rta_type, RTA_DATA (rta), RTA_PAYLOAD (rta), err);
          जर (UNLIKELY (ret < 0))
            परत ret;
        }

      ret = send_request (sock, req, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = wait_for_ack (sock, seq, buffer, buffer_size, err);
      जर (ret < 0)
        परत ret;
    }

  परत 0;
}

स्थिर पूर्णांक
request_enable_interface_and_wait (पूर्णांक sock, अक्षर *buffer, माप_प्रकार buffer_size, पूर्णांक index, libcrun_error_t *err)
{
  संरचना nl_req *req = (संरचना nl_req *) buffer;
  uint32_t seq;
  पूर्णांक ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, आकार (संरचना ifinfomsg));

  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = index;

  req->ifi.ifi_flags = IFF_UP;
  req->ifi.ifi_change = IFF_UP;

  ret = send_request (sock, req, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत wait_for_ack (sock, seq, buffer, buffer_size, err);
}

स्थिर पूर्णांक
setup_network_device_in_ns_helper (अक्षर *buffer, माप_प्रकार buffer_size, पूर्णांक netns_fd, स्थिरांक अक्षर *newifname,
                                   संरचना ip_addr *ips, libcrun_error_t *err)
{
  cleanup_close पूर्णांक sock_in_ns = -1;
  पूर्णांक new_ifindex;
  पूर्णांक ret;

  ret = setns (netns_fd, CLONE_NEWNET);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "change network namespace");

  sock_in_ns = open_netlink_fd (err);
  जर (sock_in_ns < 0)
    परत sock_in_ns;

  /* we could ask for a specific index with IFLA_NEW_IFINDEX, and apparently the kernel tries anyway to
     reuse the existing one, but asking for a specific index could cause conflicts if the
     target network namespace already exists, so avoid doing it and lookup the device again.  */
  new_ifindex = name_to_index (sock_in_ns, newifname, buffer, buffer_size, err);
  जर (UNLIKELY (new_ifindex < 0))
    परत new_ifindex;

  ret = configure_ip_addresses (sock_in_ns, new_ifindex, buffer, buffer_size, ips, err);
  जर (ret < 0)
    परत ret;

  परत request_enable_interface_and_wait (sock_in_ns, buffer, buffer_size, new_ifindex, err);
}

स्थिर पूर्णांक
do_move_link_to_ns_and_wait (पूर्णांक sock, अक्षर *buffer, माप_प्रकार buffer_size, पूर्णांक ifindex, पूर्णांक netns_fd, स्थिरांक अक्षर *newifname, libcrun_error_t *err)
{
  संरचना nl_req *req = (संरचना nl_req *) buffer;
  uint32_t seq;
  पूर्णांक ret;

  seq = reset_request (req, RTM_NEWLINK, NLM_F_REQUEST | NLM_F_ACK, आकार (संरचना ifinfomsg));
  req->ifi.ifi_family = AF_UNSPEC;
  req->ifi.ifi_index = ifindex;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_NET_NS_FD, &netns_fd, आकार (netns_fd), err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = append_rtattr (&req->nlh, buffer_size, IFLA_IFNAME, newifname, म_माप (newifname) + 1, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = send_request (sock, req, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत wait_for_ack (sock, seq, buffer, buffer_size, err);
}

/* Run the setup in the target namespace.  It is done in a separate function
   so that no variable of the caller is live across the vfork.  */
स्थिर पूर्णांक
setup_network_device_in_ns (अक्षर *buffer, माप_प्रकार buffer_size, पूर्णांक netns_fd, स्थिरांक अक्षर *newifname,
                            संरचना ip_addr *ips, libcrun_error_t *err)
{
  पूर्णांक wait_status;
  pid_t pid;
  पूर्णांक ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  जर (UNLIKELY (pid < 0))
    परत crun_make_error (err, त्रुटि_सं, "vfork");

  जर (pid == 0)
    {
      ret = setup_network_device_in_ns_helper (buffer, buffer_size, netns_fd, newifname, ips, err);
      जर (UNLIKELY (ret < 0))
        {
          /* Do not release the error as it will be used by the parent proc.  */
          _safe_exit (-ret);
        }

      _safe_exit (0);
    }

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "waitpid for exec child pid");

  जर (wait_status != 0)
    {
      /* Reuse the error from the child proc.
         In other words, do not call crun_make_error() here. */
      परत -get_process_exit_status (wait_status);
    }

  परत 0;
}

पूर्णांक
move_network_device (स्थिरांक अक्षर *ifname, स्थिरांक अक्षर *newifname, पूर्णांक netns_fd, libcrun_error_t *err)
{
  स्थिरांक माप_प्रकार buffer_size = 8192;
  cleanup_ip_addrs संरचना ip_addr *ips = रिक्त;
  cleanup_free अक्षर *buffer = xmalloc (buffer_size);
  cleanup_close पूर्णांक sock = -1;
  पूर्णांक ifindex;
  पूर्णांक ret;

  sock = open_netlink_fd (err);
  जर (sock < 0)
    परत sock;

  ifindex = name_to_index (sock, ifname, buffer, buffer_size, err);
  जर (UNLIKELY (ifindex < 0))
    परत ifindex;

  ret = get_ip_addresses (sock, ifindex, &ips, buffer, buffer_size, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  /* Move the device to the target network namespace.  */
  ret = do_move_link_to_ns_and_wait (sock, buffer, buffer_size, ifindex, netns_fd, newifname, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत setup_network_device_in_ns (buffer, buffer_size, netns_fd, newifname, ips, err);
}
