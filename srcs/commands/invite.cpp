/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 10:30:28 by epfennig          #+#    #+#             */
/*   Updated: 2021/12/09 15:46:55 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/client.hpp"
#include "../classes/server.hpp"
#include "../classes/parser.hpp"

void	cmd_invite(client* cl, std::vector<std::string> cmd, server* serv)
{
	std::string	msg;

	if (cmd.size() < 3)
		send_error_code(cl->getFd(), "461", cl->getNickname(), cmd[0], ":Not enough parameters");
	else
	{
		channel*	curr_chan = NULL;
		client*		curr_client = NULL;

		curr_chan =	serv->findChannelByName(cmd[2]);
		curr_client	= serv->findClientByName(cmd[1]);

		if (cl->curr_chan == NULL)
			return ;
		if (curr_chan == NULL)
			send_error_code(cl->getFd(), "403", cl->getNickname(), cmd[2], ":No such channel");
		else if (curr_client == NULL)
			send_error_code(cl->getFd(), "401", cl->getNickname(), cmd[1], ":No such nick");
		else if (cl->curr_chan->getName() != curr_chan->getName())
			send_error_code(cl->getFd(), "442", cl->getNickname(), cmd[2], ":You're not on that channel!");
		else if (curr_chan->isOperator(cl->getNickname()) == false)
			send_error_code(cl->getFd(), "482", cl->getNickname(), cmd[2], ":You're not channel operator");
		else if (curr_client->curr_chan != NULL && curr_client->curr_chan->getName() == curr_chan->getName())
			send_error_code(cl->getFd(), "443", cl->getNickname(), cmd[1] + " " + cmd[2], ":is already on channel");
		else
		{
			send_error_code(cl->getFd(), "341", cl->getNickname(), cmd[1], ":" + cmd[2]);
			msg = ":" + cl->getNickname() + "!" + cl->getUsername() + "@127.0.0.1 INVITE :" + cmd[1] + ":" + cmd[2] + "\r\n";
			send(curr_client->getFd(), msg.c_str(), msg.length(), 0);
			curr_client->invited = cmd[2];
		}
	}
}