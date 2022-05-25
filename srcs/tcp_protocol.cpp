/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp_protocol.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:44:18 by epfennig          #+#    #+#             */
/*   Updated: 2021/12/09 15:34:52 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/irc.hpp"

void	tcp_protocol(server* serv, char* cport)
{
	int					port;							// server port
	int					enable = 1;
	socklen_t			addrlen;						// sizeof la structure sockaddr_in
	struct sockaddr_in	server_addr;					// structure, données pour bind(), accept(), etc

	if(!(serv->sfd = socket(AF_INET, SOCK_STREAM, 0)))
		exit_error("Socket Error");

	std::cout << "[+] Server socket successfully created." << std::endl;

	/* Optionnel mais permet d'éviter le bind error à cause de la socket deja utilisé */
	if (setsockopt(serv->sfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		exit_error("Setsockopt Error");

	port = atoi(cport);
	addrlen = sizeof(server_addr);

	/* Remplire la structure pour donner les informations au bind */
	server_addr.sin_family			= AF_INET;				// famille ipv4
	server_addr.sin_port			= htons(port);			// port
	server_addr.sin_addr.s_addr		= htonl(INADDR_ANY);	// 0x00000000 pour accepter toutes les addresses

	/* Cast de la grande structure sockaddr_in avec une plus petite structure sockaddr */
	if (bind(serv->sfd, (struct sockaddr *)&server_addr, addrlen) < 0)
		exit_error("Bind Error");

	// Rendre le fd du server non bloquant, ce qui va permettre d'eviter de bloquer
	// sur le accept dans la boucle si aucun client n'essaie de se connecter
	fcntl(serv->sfd, F_SETFL, O_NONBLOCK);

	if (listen(serv->sfd, 42) < 0)
		exit_error("Listen Error: Failed set soket to passive socket");

	std::cout << "[+] Server listening for clients connexion." << std::endl;

	// KQUEUE VARIABLES //
	int					kq;
	serv->change_list.resize(1);

	// initialiser kqueue
	if ((kq = kqueue()) == -1)
		exit_error("Kqueue failed");

	// Set structure kevent selon nos flags
	EV_SET(serv->change_list.begin().base(), serv->sfd, EVFILT_READ, EV_ADD, 0, 0, 0);

	server_loop(serv, kq);
}
