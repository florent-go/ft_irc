/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 17:55:34 by epfennig          #+#    #+#             */
/*   Updated: 2021/12/05 18:28:56 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/client.hpp"
#include "../classes/server.hpp"
#include "../classes/parser.hpp"
#include "../classes/channel.hpp"

void	cmd_part(client* cl, std::vector<std::string> cmd, server* serv)
{
	channel*	curr_chan = NULL;
	std::string msg;
	if (cmd.size() < 2)
	{
		send_error_code(cl->getFd(), "461", cl->getNickname(), cmd[0], ":Not enough parameters");
		return ;
	}

	curr_chan = serv->findChannelByName(cmd[1]);
	if (curr_chan == NULL)
		send_error_code(cl->getFd(), "401", cl->getNickname(), cmd[1], ":No such nick/channel");
	else if ((curr_chan != NULL && cl->curr_chan && cl->curr_chan->getName() != cmd[1]) || cl->curr_chan == NULL)
		send_error_code(cl->getFd(), "442", cl->getNickname(), cmd[1], ":You're not on that channel");
	else if (curr_chan != NULL && curr_chan->getName() ==  cmd[1])
	{
		if (cl->getCurrMsg().find(':') != std::string::npos)
			msg = ":" + cl->getNickname() + "!" + cl->getUsername() + "@127.0.0.1 PART " + cmd[1] + " :" + ft_split(cl->getCurrMsg(), ":", 1)[1];
		else
			msg = ":" + cl->getNickname() + "!" + cl->getUsername() + "@127.0.0.1 PART :" + cmd[1] + "\r\n";
		send(cl->getFd(), msg.c_str(), msg.length(), 0);
		sendToChan(cl, NULL, msg);
		curr_chan->deleteClientFromChan(cl);
		cl->curr_chan = NULL;
		return ;
	}
}