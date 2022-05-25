/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 20:34:11 by epfennig          #+#    #+#             */
/*   Updated: 2021/12/05 18:29:26 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"

parser::parser()
{
	this->tab[0] =	"PASS";		this->tab[1] =	"NICK";
	this->tab[2] =	"USER";		this->tab[3] =	"OPER";		
	this->tab[4] =	"JOIN";		this->tab[5] =	"PRIVMSG";	
	this->tab[6] =	"KILL";		this->tab[7] =	"PART";		
	this->tab[8] =	"MODE";		this->tab[9] =	"KICK";		
	this->tab[10] =	"LIST";		this->tab[11] =	"TOPIC";
	this->tab[12] =	"INVITE";	this->tab[13] =	"NAMES";
	this->tab[14] =	"PING";
}

parser::~parser() { }

void	parser::parsing(client* cli, std::string msg, server* serv)
{
	if (cli == NULL)
		return ;
	this->msg	= msg;

	std::vector<std::string>	cmd = ft_split_c(this->msg, ' ');
	/* Checking for prefix in case it's not an operator */
	if (cmd[0][0] == ':' && (cmd[0].substr(1, cmd[0].find(' ')) != cli->getNickname()) && !cli->isOpe())
	{
		send(cli->getFd(), "Server: You canno't prefix a message this way (use your nickname or do not prefix)\r\n", 84, 0);
		return ;
	}
	/* Store prefix in case it's an operator */
	else if (cmd[0][0] == ':' && (cmd[0].substr(1, cmd[0].find(' ')) != cli->getNickname()))
	{
		this->prefix	= cmd[0].substr(1, cmd[0].find(' '));
		this->msg		= msg.substr(cmd[0].substr(1, cmd[0].find(' ')).length() + 2, msg.length());
		cmd = ft_split_c(this->msg, ' ');
	}
	else if (cmd[0][0] == ':' && (cmd[0].substr(1, cmd[0].find(' ')) == cli->getNickname()))
	{
		this->prefix	= cmd[0].substr(1, cmd[0].find(' '));
		this->msg		= msg.substr(cmd[0].substr(1, cmd[0].find(' ')).length() + 2, msg.length());
		cmd = ft_split_c(this->msg, ' ');
	}

	/* Get What is Cmd */
	cmd_type = this->whatIsCmd(cmd[0]);

	/*
	**	HELP -> Proposer cette commande lorsqu'une commande est mal utilisée
	**	PASS <mot de passe>
	**	USER <nom d'utilisateur> <hôte> <nom de serveur> :<nom réel>
	**	NICK <pseudonyme>
	**	OPER <utilisateur> <mot de passe>
	**	JOIN <canal> [<mdp>]
	**	PRIVMSG <destinataire> <texte à envoyer>
	*/

	if (cmd_type == PASS)
		cmd_pass(cli, cmd, serv);
	else if (cmd_type == NICK)
		cmd_nick(cli, cmd, serv);
	else if (cmd_type == USER)
		cmd_user(cli, cmd);
	else if (cmd_type == OPER && cli->isAccepted())
		cmd_oper(cli, cmd, serv);
	else if (cmd_type == JOIN && cli->isAccepted())
		cmd_join(cli, cmd, serv);
	else if ((cmd_type == PRIVMSG || cmd[0] == "NOTICE" || cmd[0] == "/NOTICE") && cli->isAccepted())
		cmd_privmsg(cli, cmd, serv);
	else if ((cmd_type == KILL || cmd[0] == "kill") && cli->isAccepted())
		cmd_kill(cli, cmd, serv);
	else if (cmd_type == PART && cli->isAccepted())
		cmd_part(cli, cmd, serv);
	else if (cmd_type == MODE && cli->isAccepted())
		cmd_mode(cli, cmd, serv);
	else if (cmd_type == KICK && cli->isAccepted())
		cmd_kick(cli, cmd, serv);
	else if (cmd_type == LIST && cli->isAccepted())
		cmd_list(cli, cmd, serv);
	else if (cmd_type == TOPIC && cli->isAccepted())
		cmd_topic(cli, cmd, serv);
	else if (cmd_type == INVITE && cli->isAccepted())
		cmd_invite(cli, cmd, serv);
	else if (cmd_type == NAMES && cli->isAccepted())
		cmd_names(cli, cmd, serv);
	else if (cmd_type == PING && cli->isAccepted())
		send(cli->getFd(), "PONG :NiceIRC\r\n", 15, 0); // :*.freenode.net PONG *.freenode.net :sunshine.freenode.net
	else if (cmd_type == MSG && cli->isAccepted())
		sendToChan(cli, NULL, cli->getCurrMsg());

	// Reinitialize data between message
	this->prefix.clear();
	this->msg.clear();
	this->cmd_type = -1;
}

int		parser::whatIsCmd(std::string cmd)
{
	for (int i = 0; i < MSG ; i++)
	{
		if (cmd == tab[i] || cmd == "/" + tab[i])
			return (i);
	}
	return (MSG);
}
