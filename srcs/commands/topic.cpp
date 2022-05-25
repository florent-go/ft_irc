/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 10:30:57 by epfennig          #+#    #+#             */
/*   Updated: 2021/12/08 10:30:58 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/client.hpp"
#include "../classes/server.hpp"
#include "../classes/parser.hpp"
#include "../../includes/irc.hpp"

void	cmd_topic(client* cl, std::vector<std::string> cmd, server* serv)
{
	std::string msg;
	channel*	curr_chan = NULL;

	if (cmd.size() == 1 && cl->curr_chan == NULL) // nc sans l'espace a la fin
	{
		send_error_code(cl->getFd(), "461", cl->getNickname(), cmd[0], ":Not enough parameters");
		send_error_code(cl->getFd(), "650", cl->getNickname(), cmd[0], ":<channel> [:<topic>]");
	}
	else if (cmd.size() == 1 && cl->curr_chan)
		send_error_code(cl->getFd(), "461", cl->getNickname(), cmd[0], ":Not enough parameters");
	else if (cmd[1].length() == 0 && cl->curr_chan == NULL) // irssi avec l'espace a la fin 
	{
		send_error_code(cl->getFd(), "461", cl->getNickname(), cmd[0], ":Not enough parameters");
		send_error_code(cl->getFd(), "650", cl->getNickname(), cmd[0], ":<channel> [:<topic>]");
	}
	else if (cmd.size() == 2 && cl->curr_chan == NULL)
	{
		curr_chan = serv->findChannelByName(cmd[1]);
		if (curr_chan == NULL)
			send_error_code(cl->getFd(), "403", cl->getNickname(), cmd[1], ":No such channel");
		else if (curr_chan->getTopic().empty())
			send_error_code(cl->getFd(), "331", cl->getNickname(), cmd[1], ":No topic is set.");
		else if (!(curr_chan->getTopic().empty()))
			send_error_code(cl->getFd(), "332", cl->getNickname(), cmd[1], curr_chan->getTopic());
	}
	else if (cmd.size() == 2 && cl->curr_chan)
	{
		curr_chan = serv->findChannelByName(cmd[1]);
		if (curr_chan == NULL)
			send_error_code(cl->getFd(), "403", cl->getNickname(), cmd[1], ":No such channel");
		else if (curr_chan->getTopic().empty())
			send_error_code(cl->getFd(), "331", cl->getNickname(), cmd[1], ":No topic is set.");
		else if (!(curr_chan->getTopic().empty()))
			send_error_code(cl->getFd(), "332", cl->getNickname(), cmd[1], curr_chan->getTopic());
	}
	else if (cmd.size() >= 3)
	{
		curr_chan = serv->findChannelByName(cmd[1]);
		if (curr_chan == NULL)
			send_error_code(cl->getFd(), "403", cl->getNickname(), cmd[1], ":No such channel");
		else if (cl->curr_chan == NULL)
			send_error_code(cl->getFd(), "442", cl->getNickname(), cmd[1], ":You're not on that channel!");
		else if (cl->curr_chan)
		{
			std::string topic;
			topic = ft_split(cl->getCurrMsg(), ":", 1)[1];
			curr_chan->setTopic(topic);
			msg = ":" + cl->getNickname() + "!" + cl->getUsername() + "@127.0.0.1 TOPIC " + cmd[1] + " :" + topic;
			send(cl->getFd(), msg.c_str(), msg.length(), 0);
		}
	}
}
