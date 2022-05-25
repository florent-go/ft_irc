/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 14:03:44 by fgomez            #+#    #+#             */
/*   Updated: 2021/12/05 18:54:00 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../classes/client.hpp"
#include "../classes/server.hpp"
#include "../classes/parser.hpp"

void	cmd_kill(client* cl, std::vector<std::string> cmd, server* serv)
{
	std::string msg;
	if (cl->isOpe() == false)
		send_error_code(cl->getFd(), "481", cl->getNickname(), ":Permission Denied- You're not an IRC operator", "");
	else if (cmd.size() < 3)
		send_error_code(cl->getFd(), "461", cl->getNickname(), cmd[0], ":Not enough parameters");
	else if (serv->findClientByName(cmd[1]) == NULL)
		send_error_code(cl->getFd(), "401", cl->getNickname(), cmd[1], ":No such nick/channel");
	else
	{
		client *tmp = serv->findClientByName(cmd[1]);
		if (tmp->getNickname() == cl->getNickname())
			send_error_code(cl->getFd(), "401", cl->getNickname(), cmd[0], ":You cannot kill yourself");
		else if (ft_split(cl->getCurrMsg(), ":", 1).size() == 1)
			send_error_code(cl->getFd(), "461", cl->getNickname(), cmd[0], ":Not enough parameters");
		else
		{
			msg = ":" + cl->getNickname() + "!" + cl->getUsername() + "127.0.0.1 " + "KILL " + ":" + cmd[1] + "\r\n";
			send(tmp->getFd(), msg.c_str(), msg.length(), 0);
			if (tmp->curr_chan)
			{
				msg = ":" + tmp->getNickname() + "!" + tmp->getUsername() + "@127.0.0.1 PART :" + tmp->curr_chan->getName() + "\r\n";
				send(tmp->getFd(), msg.c_str(), msg.length(), 0);
				sendToChan(tmp, NULL, msg);
			}
			close(tmp->getFd());
			serv->deleteClient(tmp->getFd());
		}
	}
}
