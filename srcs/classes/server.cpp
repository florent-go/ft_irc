/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:44:37 by epfennig          #+#    #+#             */
/*   Updated: 2021/12/09 15:31:19 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

server::server() {}
server::~server() {}

void					server::setPassword(const std::string & pass) { this->password = pass; }
const std::string &		server::getPassword(void) const { return this->password; }
const std::string &		server::getOperPassword(void) const { return this->pass_oper; }

client*					server::findClientByFd(unsigned long Fd)
{
	std::vector<client *>::iterator	it = this->clients.begin();
	std::vector<client *>::iterator	ite = this->clients.end();

	for ( ; it != ite ; it++ ) {
		if ((*it)->getFd() == Fd)
			return (*it);
	}
	return (NULL);
}

client*					server::findClientByName(std::string name)
{
	std::vector<client *>::iterator	it = this->clients.begin();
	std::vector<client *>::iterator	ite = this->clients.end();

	for ( ; it != ite ; it++ ) {
		if ((*it)->getNickname() == name)
			return (*it);
	}
	return (NULL);
}

channel*				server::findChannelByName(std::string name)
{
	std::vector<channel *>::iterator	it = this->channels.begin();
	std::vector<channel *>::iterator	ite = this->channels.end();

	for ( ; it != ite ; it++ ) {
		if ((*it)->getName() == name)
			return (*it);
	}
	return (NULL);
}

int		server::acceptClient(int kq)
{
	struct sockaddr_in	client_addr;	// new struct for addr info client
	int addrlen = sizeof(client_addr);	// size of struct for accept()
	int cfd;							// new client fd
	(void)kq;
	// Si personne ne souhaite se connecter, le fd du server etant non bloquant,
	// dans ce cas on continue pour repartir du haute de la boucle !
	if ((cfd = accept(this->sfd, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen)) == -1)
		exit_error("Accept error");

	// Ajouter mon nouveau client à ma liste d'evenement en lui precisant le cfd
	// le client fd sera set de maniere non bloquante avec cfd
	this->change_list.resize(change_list.size() + 1);
	fcntl(cfd, F_SETFL, O_NONBLOCK);
	EV_SET(change_list.end().base() - 1, cfd, EVFILT_READ, EV_ADD, 0, 0, 0);

	client	*new_client = new client(cfd);
	this->clients.push_back(new_client);	// Add client to server list

	/* Add kevent struct to vector */
	this->event_list.resize(event_list.size() + 1);
	
	std::cout << "[+] Client[" << cfd << "] accepted !" << std::endl;
	send(new_client->getFd(), "PING :Hello\r\n", 13, 0);
	return (1);
}

void	server::recevMessage(int i)
{
	char	buffer[1024];

	bzero(buffer, 1024);
	recv(event_list[i].ident, buffer, 1024, 0);
	if (ft_strlen(buffer) > 511)
	{
		send(event_list[i].ident, "Limit message to 512 characteres\r\n", 34, 0);
		return ;
	}
	client* temp = findClientByFd(event_list[i].ident);
	if (temp == NULL)
		return ;
	else if (std::string(buffer) == "\r\n")
		return ;
	/* Rebuild string if ctrl+D is pressed by client */
	else if (std::string(buffer).find("\n") == std::string::npos)
	{
		temp->getCurrMsg() += std::string(buffer);
	}
	else
	{
		std::vector<std::string>	tab;
		if (!(temp->getCurrMsg().empty()))
		{
			if (!(std::string(buffer).empty()))
				temp->getCurrMsg() += std::string(buffer);
			tab = ft_split(temp->getCurrMsg(), "\r\n", 512);
			if (tab[0] == temp->getCurrMsg())
				tab = ft_split(temp->getCurrMsg(), "\n", 512);
		}
		else
		{
			temp->getCurrMsg() = std::string(buffer);
			tab = ft_split(temp->getCurrMsg(), "\r\n", 512);
			if (tab[0] == temp->getCurrMsg())
				tab = ft_split(temp->getCurrMsg(), "\n", 512);
		}
		/* Verif version agreger > 512 */
		for (unsigned long j = 0 ; !tab[j].empty() ; j++)
		{
			std::cout << "Recev_Msg $=> " << temp->getNickname() << " ->  |" << tab[j] << "|" << std::endl;
			temp->parser.parsing(temp, tab[j], this);
		}
		temp->getCurrMsg().clear();
	}
	return ;
}

void	server::parse_config_file()
{
	std::ifstream	ifs("config_irc.conf");
	if (ifs.fail())
		std::cout << "Error file" << std::endl;
	std::string		buff;

	std::vector<std::string>	lines;
	while (getline(ifs, buff)) {
		lines.push_back(buff);
	}

	for (unsigned int i = 0 ; i < lines.size() ; i++)
	{
		if (!strncmp(lines[i].c_str(), "serv_accept_connex:", 19))
		serv_accept_connex = ft_split(lines[i], ": ", 1)[1];
		else if (!strncmp(lines[i].c_str(), "location:", 9))
			location = ft_split(lines[i], ": ", 1)[1];
		else if (!strncmp(lines[i].c_str(), "oper:", 5))
			pass_oper = ft_split(lines[i], ": ", 1)[1];
		else if (!strncmp(lines[i].c_str(), "server_responsible:", 19))
			serv_responsible = ft_split(lines[i], ": ", 1)[1];
	}
}

void			server::deleteClient(int fd)
{
	client *tmp = findClientByFd(fd);

	std::vector<client *>::iterator	it	=	this->clients.begin();
	std::vector<client *>::iterator	ite =	this->clients.end();

	for ( ; it != ite ; it++ )
	{
		if (tmp == *it)
		{
			if ((*it)->curr_chan != NULL)
				(*it)->curr_chan->deleteClientFromChan(*it);
			this->clients.erase(it);
			delete tmp;
			tmp = NULL;
			return ;
		}
	}
}