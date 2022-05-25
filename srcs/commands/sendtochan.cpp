/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendtochan.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 09:42:23 by fgomez            #+#    #+#             */
/*   Updated: 2021/12/05 18:35:52 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/client.hpp"
#include "../classes/server.hpp"
#include "../classes/parser.hpp"

void	sendToChan(client* cl, channel* chan, std::string msg)
{
	if (chan)
	{
		std::vector<client *>::iterator it = chan->users.begin();
		std::vector<client *>::iterator ite = chan->users.end();

		for ( ; it != ite ; it++)
		{
			if (ft_split(msg, "!", 1)[0] != (":" + (*it)->getNickname()))
				send((*it)->getFd(), msg.c_str() , msg.length(), 0);
		}
	}
	else if (cl && cl->curr_chan)
	{
		std::vector<client *>::iterator it = cl->curr_chan->users.begin();
		std::vector<client *>::iterator ite = cl->curr_chan->users.end();

		for ( ; it != ite ; it++)
		{
			if ((*it)->getNickname() == cl->getNickname())
				continue ;
			send((*it)->getFd(), msg.c_str() , msg.length(), 0);
		}
	}
}
