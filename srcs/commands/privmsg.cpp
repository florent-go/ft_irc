/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 09:42:17 by fgomez            #+#    #+#             */
/*   Updated: 2021/12/09 15:29:08 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/client.hpp"
#include "../classes/server.hpp"
#include "../classes/parser.hpp"

void	bot_send(client* cl)
{
	std::string msg;
	std::string	date;

	std::time_t		time;
	struct tm		*local;

	time = std::time(&time);
	local = localtime(&time);

	date = "What time is it ? -> " + ft_itos(local->tm_mday) + "/" + \
				  ft_itos(local->tm_mon + 1) + "/" + ft_itos(local->tm_year + 1900) + " - " + \
				  ft_itos(local->tm_hour) + ":" +  ft_itos(local->tm_min) + ":" + ft_itos(local->tm_sec) + ".";

	msg = ":" + cl->getNickname() + "!" + cl->getUsername() + "@127.0.0.1 " + date + "\r\n";
	send(cl->getFd(), msg.c_str(), msg.length(), 0);
}

void	cmd_privmsg(client* cl, std::vector<std::string> cmd,  server* serv)
{
	std::vector<std::string>	destinataire = ft_split(cmd[1], ",", 512);
	std::string msg;
	if (destinataire.size() > 14)
		send_error_code(cl->getFd(), cl->getNickname(), ":407 recipients. Too many targets.", "", "");
	else if (cmd.size() < 3)
		send_error_code(cl->getFd(), "481", cl->getNickname(), ":No text to send", "");
	else
	{
		if (cmd[1] == "TimeBot")
		{
			bot_send(cl);
			return ;
		}
		for (unsigned int i = 0; i < destinataire.size(); i++)
		{
			if (serv->findClientByName(destinataire[i]) == NULL && serv->findChannelByName(destinataire[i]) == NULL)
			{
				send_error_code(cl->getFd(), "401", cl->getNickname(), cmd[1], ":No such nick/channel");
				return ;
			}
		}
		client	*tmp_cl;
		channel	*tmp_chan;
		msg = ":" + cl->getNickname() + "!" + cl->getUsername() + "@127.0.0.1 " + cl->getCurrMsg();
		for (unsigned int i = 0 ; i < destinataire.size() ; i++)
		{
			tmp_cl		= serv->findClientByName(destinataire[i]);
			tmp_chan	= serv->findChannelByName(destinataire[i]);
			if (tmp_chan != NULL && tmp_chan->modes.find('n') != std::string::npos && cl->curr_chan != tmp_chan)
				send_error_code(cl->getFd(), "404", cl->getNickname(), tmp_chan->getName(), ":You cannot send external messages to this channel whilst the +n (noextmsg) mode is set.");
			else if ((destinataire[i][0] == '#' || destinataire[i][0] == '&') && tmp_chan->findIteratorStr(tmp_chan->muteList, cl->getNickname()) == tmp_chan->muteList.end())
				sendToChan(NULL, tmp_chan, msg);
			else if (tmp_cl != NULL && tmp_cl->isAccepted())
				send(tmp_cl->getFd(), msg.c_str(), msg.length(), 0);
		}
	}
}
