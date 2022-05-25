/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 17:10:21 by epfennig          #+#    #+#             */
/*   Updated: 2021/11/24 18:50:48 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

client::client(int Fd)
{
	this->fd		= Fd;
	this->curr_chan	= NULL;
	this->accepted	= false;
	this->ope		= false;
	this->pass		= false;
	this->nick		= false;
	this->user		= false;
}

client::client(const client& copy)
{
	*this = copy;
}

client&		client::operator=(const client& rhs)
{
	this->nickname			= rhs.nickname;
	this->username			= rhs.username;
	this->realname			= rhs.realname;
	this->nickname_history	= rhs.nickname_history;
	
	this->fd				= rhs.fd;
	this->curr_chan			= rhs.curr_chan;
	this->accepted			= rhs.accepted;
	this->ope				= rhs.ope;
	this->pass				= rhs.pass;
	this->nick				= rhs.nick;
	this->user				= rhs.user;
	return (*this);
}

client::~client()
{
	// this->nickname.clear();
	// this->username.clear();
	// this->realname.clear();
	// this->nickname_history.clear();

	// this->accepted	= false;
	// this->pass		= false;
	// this->nick		= false;
	// this->user		= false;
}

bool				client::isAccepted() const { return this->accepted; }
void				client::AcceptClient() { this->accepted = true; }

void				client::setUsername(const std::string & Username) { this->username = Username; }
void				client::setNickname(const std::string & Nickname) { this->nickname = Nickname; }
void				client::setRealname(const std::string & Realname) { this->realname = Realname; }

const std::string & client::getNickname(void) const { return this->nickname; }
const std::string & client::getUsername(void) const { return this->username; }
const std::string &	client::getRealname(void) const { return this->realname; }

std::string&		client::getCurrMsg(void) { return this->curr_msg; }

unsigned long 		client::getFd() const { return this->fd; }

bool				client::isOpe(void) { return this->ope; }

void				client::joinChannel(channel chan)
{
	(void)chan;
}
