/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 20:34:22 by epfennig          #+#    #+#             */
/*   Updated: 2021/12/03 18:43:48 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"

channel::channel(std::string Name)
{
	this->name		= Name;
	this->max_user	= 10;
	this->status	= 'n';	/* No status yet */
	this->Nbuser	= 0;
}

channel::~channel() {}

int		channel::addClient(client* cl, std::vector<std::string> cmd)  // JOIN d'un client, Verif, etc
{
	if (this->operators.size() == 0 && this->users.size() == 0) /* It means it's a new channel */
	{
		this->Nbuser += 1;
		this->operators.push_back(cl);
		this->users.push_back(cl);
		return (1);
	}

	if (this->max_user == this->Nbuser)
	{
		send_error_code(cl->getFd(), "471", cl->getNickname(), this->name, ":Cannot join channel (channel is full)");
		return (0);
	}

	/* Handle b mode */
	else if (this->black_list.size() != 0)
	{
		if (this->findIteratorStr(black_list, cl->getNickname()) != black_list.end())
		{
			send_error_code(cl->getFd(), "474", cl->getNickname(), this->name, ":Cannot join channel (you're banned)");
			return (0);
		}
	}
	
	else if (cl->invited == this->name)
	{
		cl->invited.clear();
		this->users.push_back(cl);
		this->Nbuser += 1;
		return (1);
	}

	/* Handle i mode */
	else if (this->modes.find('i') != std::string::npos)
	{
		if (cl->invited != this->name)
		{
			send_error_code(cl->getFd(), "473", cl->getNickname(), cmd[1], ":Cannot join channel (invite only)");
			return (0);
		}
	}

	/* Handle k mode */
	else if (this->modes.find('k') != std::string::npos && cmd.size() >= 3)
	{
		if (cmd[2] == this->password)
		{
			this->users.push_back(cl);
			this->Nbuser += 1;
			return (1);
		}
		else
		{
			send_error_code(cl->getFd(), "475", cl->getNickname(), cmd[1], ":Cannot join channel (incorrect channel key)");
			return (0);
		}
	}

	else
	{
		this->Nbuser += 1;
		this->users.push_back(cl);
	}

	return (1);
}


bool					channel::deleteClientFromChan(client *cl)
{
	std::vector<client *>::iterator	it	=	this->users.begin();
	std::vector<client *>::iterator	ite	=	this->users.end();

	std::vector<client *>::iterator	it2		=	this->operators.begin();
	std::vector<client *>::iterator	ite2	=	this->operators.end();
	
	for ( ; it != ite ; it++ )
	{
		if ((*it) == cl)
		{
			if (isOperator(cl->getNickname()) == true)
			{
				for ( ; it2 != ite2 ; it2++ ) {
					if (*it2 == cl) {
						this->operators.erase(it2);
						break ;
					}
				}
			}
			this->Nbuser -= 1;
			this->users.erase(it);
			return (true);
		}
	}
	return (false);
}

const std::string&		channel::getName(void) const { return this->name; }
const std::string&		channel::getPassword(void) const { return this->name; }
const std::string&		channel::getTopic(void) const { return this->topic; }
const unsigned int&		channel::getNbuser(void) const { return this->Nbuser; }
void					channel::setPassword(std::string pass) { password = pass; }

void					channel::setTopic(std::string const & Topic) { this->topic = Topic; }

bool					channel::isOperator(std::string user)
{
	std::vector<client *>::iterator	it	= this->operators.begin();
	std::vector<client *>::iterator	ite	= this->operators.end();

	for ( ; it != ite ; it++)
	{
		if ((*it)->getNickname() == user)
			return (true);
	}
	return false;
}

char		 			channel::checkStatus(void) const
{
	return this->status;
}

bool 					channel::checkMode(char mode) const
{
	(void)mode;
	return false;
}

bool					channel::checkBlackList(std::string user) const
{
	(void)user;
	return false;
}

bool					channel::checkMaxUser(void) const { return false; }

client*					channel::findClientByName(std::string nickname)
{
	std::vector<client *>::iterator	it	=	this->users.begin();
	std::vector<client *>::iterator	ite	=	this->users.end();

	for ( ; it != ite ; it++) {
		if ((*it)->getNickname() == nickname)
			return (*it);
	}
	return NULL;
}

std::vector<std::string>::iterator	channel::findIteratorStr(std::vector<std::string>& vec, std::string str)
{
	std::vector<std::string>::iterator	it = vec.begin();
	std::vector<std::string>::iterator	ite = vec.end();

	for ( ; it != ite ; it++)
	{
		if (*it == str)
			return (it);
	}
	return (ite);
}

std::vector<client *>::iterator	channel::findIteratorClient(std::vector<client *>& vec, std::string str)
{
	std::vector<client *>::iterator	it = vec.begin();
	std::vector<client *>::iterator	ite = vec.end();

	for ( ; it != ite ; it++)
	{
		if ((*it)->getNickname() == str)
			return (it);
	}
	return (ite);
}