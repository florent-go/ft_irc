/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 11:59:30 by epfennig          #+#    #+#             */
/*   Updated: 2021/12/06 12:35:00 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/client.hpp"
#include "../classes/server.hpp"
#include "../classes/parser.hpp"

void	cmd_nick(client* cl, std::vector<std::string> cmd,  server* serv)
{
	std::string msg;
	if  (cmd.size() < 2)
		send_error_code(cl->getFd(), "431", cl->getNickname(), ":No nickname given", "");
	else if (serv->findClientByName(cmd[1]) != NULL)
		send_error_code(cl->getFd(), "443", cl->getNickname(), cmd[1], ":Nickname is already in use");
	else if (cmd[1] == "TimeBot")
		send_error_code(cl->getFd(), "443", cl->getNickname(), cmd[1], ":Nickname is already in use");
	else if (cmd[1].length() == 0)
		send_error_code(cl->getFd(), "431", cl->getNickname(), ":No nickname given", "");
	else
	{
		if (cl->nick == true)
		{
			msg = ":" + cl->getNickname() + "!" + cl->getUsername() + "127.0.0.1 " + "NICK " + ":" + cmd[1] + "\r\n";
			send(cl->getFd(), msg.c_str(), msg.length(), 0);
		}
		cl->setNickname(cmd[1]);
		cl->nick = true;
		cl->nickname_history.push_back(cmd[1]);
	}

	if (!cl->isAccepted() && cl->nick == true && cl->pass == true && cl->user == true)
	{
		msg = ":NiceIRC 001 " + cl->getNickname() + " :Welcome to the Internet Relay Network " + cl->getNickname() + "!" + cl->getUsername() + "@127.0.0.1\r\n";
		send(cl->getFd(), msg.c_str(), msg.length(), 0);
		cl->AcceptClient();
	}
}
