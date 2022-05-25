/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 18:03:33 by epfennig          #+#    #+#             */
/*   Updated: 2021/12/03 15:08:09 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>
#include <vector>

class	client;

class channel
{
	private:
				std::string						name;
				std::string						password;	// if private
				char							status;		// private ; secret ; invitation

				std::string						topic;
	
	public:
				std::vector<client *>			operators;	// operator list in the channel
				std::vector<client *>			users;		// users list in the channel
				
				std::vector<std::string>		black_list;	// mode +b
				std::vector<std::string>		muteList;	// mode +v
				std::string						modes;		// channels modes
				unsigned int					Nbuser;
				unsigned int					max_user;

	public:
				channel(std::string Name);
				~channel();

				int						addClient(client* cl, std::vector<std::string> cmd); // /JOIN d'un client, toutes les etapes de verifications du channel etc
				bool					deleteClientFromChan(client *cl);

				const std::string&		getName(void) const;
				const std::string&		getPassword(void) const;
				const std::string&		getTopic(void) const;

				void					setTopic(std::string const & Topic);

				bool					isOperator(std::string user);

				void					setPassword(std::string pass);
				char 					checkStatus(void) const;
				bool 					checkMode(char mode) const;
				bool					checkBlackList(std::string user) const;
				bool					checkMaxUser(void) const;

				const unsigned int&		getNbuser(void) const;
				client*					findClientByName(std::string nickname);

				std::vector<std::string>::iterator	findIteratorStr(std::vector<std::string>& vec, std::string str);
				std::vector<client *>::iterator		findIteratorClient(std::vector<client *>& vec, std::string str);

};

#include "client.hpp"

#endif