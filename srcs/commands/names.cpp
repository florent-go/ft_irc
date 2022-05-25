/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 10:30:41 by epfennig          #+#    #+#             */
/*   Updated: 2021/12/08 10:30:42 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/client.hpp"
#include "../classes/server.hpp"
#include "../classes/parser.hpp"

void	tmp_cmd_who(client* cl)
{
	// ==================== Confirm client join chan ===================
	std::string msg = ":" + cl->getNickname() + "!" + cl->getUsername() + "@127.0.0.1 " + cl->getCurrMsg();
	send(cl->getFd(), msg.c_str() , msg.length(), 0);
	// ================== List Name ====================================
	std::vector<client *>::iterator it = cl->curr_chan->users.begin();
	std::vector<client *>::iterator ite = cl->curr_chan->users.end();

	msg = ":NiceIRC 353 " + cl->getNickname() + " = " + cl->curr_chan->getName() + " :";
	for ( ; it != ite ; it++)
	{
		if ((*it)->curr_chan->isOperator((*it)->getNickname()) == true)
            msg += "@";
		msg += (*it)->getNickname() + " ";
	}
	msg.erase(msg.length() - 1);
	send(cl->getFd(), (msg + "\r\n").c_str(), msg.length() + 2, 0);
	// ====================== RPL_END_OF_NAMES =========================
	msg = ":NiceIRC 366 " + cl->getNickname() + " " + cl->curr_chan->getName() + " :End of NAMES list\r\n";
	send(cl->getFd(), msg.c_str(), msg.length(), 0);
}

void	cmd_names(client* cl, std::vector<std::string> cmd, server* serv)
{
	std::string	msg;
	channel*	curr_chan = NULL;
	if (cl->curr_chan == NULL)
		send_error_code(cl->getFd(), "442", cl->getNickname(), cmd[2], ":You're not on that channel!");
	else if (cmd.size() < 2)
		send_error_code(cl->getFd(), "461", cl->getNickname(), cmd[0], ":Not enough parameters");
	else if (cmd.size() == 2)
	{
		curr_chan = serv->findChannelByName(cmd[1]);
		if (curr_chan != NULL)
			tmp_cmd_who(cl);
	}
}