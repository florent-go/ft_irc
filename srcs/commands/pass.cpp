/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 17:55:43 by epfennig          #+#    #+#             */
/*   Updated: 2021/12/03 17:55:44 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/client.hpp"
#include "../classes/server.hpp"
#include "../classes/parser.hpp"

void	cmd_pass(client* cl, std::vector<std::string> cmd, server* serv)
{
	std::string	msg;
	if (cmd.size() < 2)
		send_error_code(cl->getFd(), "461", cl->getNickname(), cmd[0], ":Not enough parameters");
	else if (cl->isAccepted() == true)
		send_error_code(cl->getFd(), "462", cl->getNickname(), ":Unauthorized command (already registered)", "");
	else if (cl->isAccepted() == false)
	{
		/* Check for ':' some client are using it */
		if (cmd[1][0] == ':')
			cmd[1] = cmd[1].substr(1);
		if (cmd[1] == serv->getPassword())
			cl->pass = true;
		else
			send_error_code(cl->getFd(), "464", cl->getNickname(), ":Password incorrect", "");
	}
	else
		cl->pass = false;
	
	if (!cl->isAccepted() && cl->nick == true && cl->pass == true && cl->user == true)
	{
		std::string msg = ":NiceIRC 001 " + cl->getNickname() + " :Welcome to the Internet Relay Network " + cl->getNickname() + "!" + cl->getUsername() + "@127.0.0.1\r\n";
		send(cl->getFd(), msg.c_str(), msg.length(), 0);
		cl->AcceptClient();
	}
}