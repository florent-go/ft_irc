/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:44:43 by epfennig          #+#    #+#             */
/*   Updated: 2021/11/29 18:32:10 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include "channel.hpp"
#include "client.hpp"
// #include ""
#include "../../includes/irc.hpp"

class server
{
	private:
			/* Config File Data */
			std::string	serv_accept_connex;
			std::string	location;
			std::string	pass_oper;
			std::string	serv_responsible;
			/* Serv Data */
			std::string					password;
	public:
			std::vector<channel *>		channels;
			std::vector<client *>		clients;
			int							sfd;

			std::vector<struct kevent>		change_list;
			std::vector<struct kevent>		event_list;
	public:
			server();
			~server();

			void	parse_config_file();

			int						acceptClient(int kq);
			void					recevMessage(int i);
			void					setPassword(const std::string & pass);
			const std::string &		getPassword(void) const;
			const std::string &		getOperPassword(void) const;


			client*					findClientByFd(unsigned long Fd);
			client*					findClientByName(std::string name);

			channel*				findChannelByName(std::string name);

			void					deleteClient(int fd);

};


#endif
