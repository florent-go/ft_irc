/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 15:49:07 by epfennig          #+#    #+#             */
/*   Updated: 2021/12/05 18:42:46 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/client.hpp"
#include "../classes/server.hpp"
#include "../classes/parser.hpp"
#include "../classes/channel.hpp"

void	sendInfoClient(client* cl)
{
	// ==================== Confirm client join chan ===================
	std::string msg = ":" + cl->getNickname() + "!" + cl->getUsername() + "@127.0.0.1 " + cl->getCurrMsg();
	send(cl->getFd(), msg.c_str() , msg.length(), 0);
	// ==================== Message for topic ==========================
	msg = ":NiceIRC 332 " + cl->getNickname() + " " + cl->curr_chan->getName() + " :No Topic set\r\n";
	send(cl->getFd(), msg.c_str(), msg.length(), 0);
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

channel*	create_channel(client* cl, std::vector<std::string> cmd, server* serv)
{
	channel*	new_chan = new channel(cmd[1]);
	new_chan->addClient(cl, cmd);
	serv->channels.push_back(new_chan);
	cl->curr_chan = new_chan;
	cl->curr_chan->modes.push_back('n');
	cl->curr_chan->modes.push_back('t');
	return (new_chan);
}

void	cmd_join(client* cl, std::vector<std::string> cmd, server* serv)
{
	channel*	curr_chan = NULL;
	std::string	msg;
	if (cmd.size() < 2)
		send_error_code(cl->getFd(), "461", cl->getNickname(), cmd[0], ":Not enough parameters");
	else if (cmd[1][0] != '#' && cmd[1][0] != '&')
		send_error_code(cl->getFd(), "476", cl->getNickname(), cmd[1], ":Bad Channel Mask");
	else
	{
		curr_chan = serv->findChannelByName(cmd[1]);

		/* Leave Channel if Client is already in one */
		if (cl->curr_chan != NULL)
		{
			msg = ":" + cl->getNickname() + "!" + cl->getUsername() + "@127.0.0.1 PART :" + cl->curr_chan->getName() + "\r\n";
			send(cl->getFd(), msg.c_str(), msg.length(), 0);
			sendToChan(cl, NULL, msg);
			cl->curr_chan->deleteClientFromChan(cl);
			cl->curr_chan = NULL;
		}

		/* Create chan or join it if its exists */
		if (curr_chan == NULL)
		{
			curr_chan = create_channel(cl, cmd, serv);
			sendInfoClient(cl);
		}
		else
		{
			if (curr_chan->addClient(cl, cmd) == 0)
				return ;
			cl->curr_chan = curr_chan;
			msg = ":" + cl->getNickname() + "!" + cl->getUsername() + "@127.0.0.1 JOIN :" + cmd[1] + "\r\n";
			sendToChan(cl, NULL, msg);
			sendInfoClient(cl);

		}
	}
}
