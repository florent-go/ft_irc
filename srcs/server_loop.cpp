/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_loop.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:44:59 by epfennig          #+#    #+#             */
/*   Updated: 2021/12/09 15:31:09 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/irc.hpp"

void	server_loop(server* serv, int kq)
{
	int		n_ev;

	while (1)
	{
		serv->event_list.clear();
		serv->event_list.resize(1);
		if ((n_ev = kevent(kq, serv->change_list.begin().base(), serv->change_list.size(), serv->event_list.begin().base(), serv->event_list.size(), NULL)) < 0 )
			exit_error("kevent failed");
		serv->change_list.clear();
		if (n_ev > 0)
		{
			/* Boucler sur le nombre de nouveaux events */
			for (int i = 0; i < n_ev ; i++)
			{
				/* Handle kevent error */
				if (serv->event_list[i].flags & EV_ERROR)
					exit_error("kevent: EV_ERROR");

				/* Client disconnect */
				if (serv->event_list[i].flags & EV_EOF)
				{	
					serv->deleteClient(serv->event_list[i].ident);
					close(serv->event_list[i].ident);
					std::cout << "[-] Client[" << serv->event_list[i].ident << "] disconnected !" << std::endl;
				}
				/* Accept new clients */
				else if (serv->event_list[i].ident == (unsigned long)serv->sfd)
					serv->acceptClient(kq);
				/* Read client messages that are already accepted */
				else if (serv->event_list[i].filter & EVFILT_READ)
					serv->recevMessage(i);
			}
		}
	}
}
