/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 20:34:31 by epfennig          #+#    #+#             */
/*   Updated: 2021/12/03 12:34:20 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

/* C++ Includes */
#include <iostream>
#include <string>
#include <cstring>
#include <map>
#include <sstream>
#include <fstream>

/* TCP / IP */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/event.h>
#include <netdb.h>
#include <arpa/inet.h>

/* C Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>		
#include <unistd.h>
#include <fcntl.h>		// fcntl (fd non bloquant)
#include <errno.h>		// errno


/* ***************************************************************************

====================================
**	=== Server TCP / IP ===
**	Socket()
**	Bind()
**	Listen()
**	Accept()
**	Write() / Read() -> (server <--> client)
**	Close()
====================================

struct sockaddr_in
{
	__uint8_t       sin_len;							// unsigned char  0 - 255
	sa_family_t     sin_family;							// unsigned char  0 - 255
	in_port_t       sin_port;							// unsigned short 0	- 65535
	struct  in_addr sin_addr;		sin_addr.s_addr;	// unsigned int   (0xffffffff) address
	char            sin_zero[8];
};

[XSI] Structure used by kernel to store most addresses.
struct sockaddr
{
	__uint8_t       sa_len;         total length		// unsigned char
	sa_family_t     sa_family;      address family		// unsigned char
	char            sa_data[14];    addr value (actually larger)
};

struct in_addr
{
	in_addr_t s_addr;  base type for internet address
};

in_addr_t == uint32_t

struct kevent {
            uintptr_t  ident;        identifier for this event
            uint32_t   filter;       filter for event
            uint32_t   flags;        action flags for kqueue
            uint32_t   fflags;       filter flag value
            int64_t    data;         filter data value
            void       *udata;        opaque user data identifier
};

*************************************************************************** */

class server;

void	tcp_protocol(server* serv, char* cport);
void	server_loop(server* serv, int kq);
int		accept_client(int sfd, int kq, struct kevent change_list, std::map<int, int>* client, int* id);
void	recev_message(char *buffer, std::map<int, int> client, struct kevent event_list[64], int i);

/* === UTILS FUNCTIONS === */

void						exit_error(std::string error);
std::string					ft_itos(int Number);
std::vector<std::string>	ft_split(std::string str, std::string delimiter, int times);
std::vector<std::string>	ft_split_c(std::string str, char	c);
int							ft_strlen(char *str);

#include "../srcs/classes/server.hpp"


#endif