/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 18:05:23 by epfennig          #+#    #+#             */
/*   Updated: 2021/12/05 18:28:48 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/client.hpp"
#include "../classes/server.hpp"
#include "../classes/parser.hpp"

void	cmd_kick(client* cl, std::vector<std::string> cmd, server* serv)
{
	std::string	msg;
	if (cmd.size() < 4)
		send_error_code(cl->getFd(), "461", cl->getNickname(), cmd[0], ":Not enough parameters");
	else if (cmd[1][0] != '#' && cmd[1][0] != '&')
		send_error_code(cl->getFd(), "476", cl->getNickname(), cmd[1], ":Bad Channel Mask");
	else if (serv->findChannelByName(cmd[1]) == NULL)
		send_error_code(cl->getFd(), "403", cl->getNickname(), cmd[1], ":No such channel");
	else if (serv->findClientByName(cmd[2]) == NULL)
		send_error_code(cl->getFd(), "401", cl->getNickname(), cmd[2], ":No such nick/channel");
	else if (cl->curr_chan == NULL || serv->findChannelByName(cmd[1]) != cl->curr_chan)
		send_error_code(cl->getFd(), "442", cl->getNickname(), cmd[1], ":You're not on that channel");
	else if (cl->curr_chan != NULL && serv->findClientByName(cmd[2])->curr_chan != cl->curr_chan)
		send_error_code(cl->getFd(), "441", cl->getNickname(), cmd[2], cmd[1] + " :They aren't on that channel");
	else if (cl->curr_chan->isOperator(cl->getNickname()) == false)
		send_error_code(cl->getFd(), "482", cl->getNickname(), cmd[1], ":You must be a channel operator");
	else
	{
		msg = ":" + cl->getNickname() + "!" + cl->getUsername() + "@127.0.0.1 KICK " + cmd[1] + " " + cmd[2] + " :" + ft_split(cl->getCurrMsg(), ":", 1)[1];
		send(cl->getFd(), msg.c_str(), msg.length(), 0);
		sendToChan(cl, NULL, msg);
		serv->findChannelByName(cmd[1])->deleteClientFromChan(serv->findClientByName(cmd[2]));
		serv->findClientByName(cmd[2])->curr_chan = NULL;
	}
}
