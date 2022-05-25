/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 10:30:16 by epfennig          #+#    #+#             */
/*   Updated: 2021/12/08 10:30:18 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/client.hpp"
#include "../classes/server.hpp"
#include "../classes/parser.hpp"
#include "../../includes/irc.hpp"

void	cmd_list(client* cl, std::vector<std::string> cmd, server* serv)
{
	std::string msg;

	std::vector<channel *>::iterator it = serv->channels.begin();
	std::vector<channel *>::iterator ite = serv->channels.end();
	if (cmd.size() < 2) // nc sans l'espace a la fin
	{
		for ( ; it != ite; it++)
		{
			if ((*it)->modes.find('p') == std::string::npos && (*it)->modes.find('s') == std::string::npos)
				send_error_code(cl->getFd(), "322", cl->getNickname(), (*it)->getName() + " " + ft_itos((*it)->getNbuser()), ":[+" + (*it)->modes + "]");
			else if ((*it)->modes.find('p') != std::string::npos && (*it)->modes.find('s') == std::string::npos)
				send_error_code(cl->getFd(), "322", cl->getNickname(), "* " + ft_itos((*it)->getNbuser()), ":");
		}
	}
	else if (cmd.size() > 2)
	{
		std::vector<std::string> channel = ft_split(cmd[1], ",", 512);

		std::vector<std::string>::iterator it_name = channel.begin();
		std::vector<std::string>::iterator ite_name = channel.end();

		for ( ; it_name != ite_name; it_name++)
		{
			it = serv->channels.begin();
			for ( ; it != ite; it++) {
				if (*it_name == (*it)->getName() && (*it)->modes.find('p') == std::string::npos && (*it)->modes.find('s') == std::string::npos)
					send_error_code(cl->getFd(), "322", cl->getNickname(), (*it)->getName() + " " + ft_itos((*it)->getNbuser()), ":[+" + (*it)->modes + "]");
				else if (*it_name == (*it)->getName() && (*it)->modes.find('p') != std::string::npos && (*it)->modes.find('s') == std::string::npos)
					send_error_code(cl->getFd(), "322", cl->getNickname(), "* " + ft_itos((*it)->getNbuser()), ":");
			}
		}
	}
	send_error_code(cl->getFd(), "323", cl->getNickname(), ":End of channel list.", "");
}
