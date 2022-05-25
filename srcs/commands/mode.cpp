/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 10:30:10 by epfennig          #+#    #+#             */
/*   Updated: 2021/12/08 10:30:12 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/client.hpp"
#include "../classes/server.hpp"
#include "../classes/parser.hpp"
#include "../classes/channel.hpp"

void	plusParamMode(client* cl, char m, std::vector<std::string> cmd, channel *curr_chan, server* serv)
{
	/* [ +k <key> ] [ +l <max_user> ] [ +b <users_banlist> ] [ +t <topic> ] [ +v <user> ] [ +o <user> ] */
	if (m == 'k')
		curr_chan->setPassword(cmd[3]);
	else if (m == 'l')
		curr_chan->max_user = std::stoi(cmd[3]);
	if (m == 'b')
	{
		if (cmd.size() > 3)
			curr_chan->black_list.push_back(cmd[3]);
		else
		{
			std::vector<std::string>::iterator it = curr_chan->black_list.begin();
			std::vector<std::string>::iterator ite = curr_chan->black_list.end();
			for ( ; it != ite; it++)
				send_error_code(cl->getFd(), "367", cl->getNickname(), curr_chan->getName() + " " + *it + "!*@127.0.0.1 " +  cl->getNickname(), ":");
		}
	}
	else if (m == 't')
		;
	else if (m == 'v')
	{
		if (cmd.size() > 3)
			curr_chan->muteList.push_back(cmd[3]);
		else
		{
			std::vector<std::string>::iterator it = curr_chan->muteList.begin();
			std::vector<std::string>::iterator ite = curr_chan->muteList.end();
			for ( ; it != ite; it++)
				send_error_code(cl->getFd(), "367", cl->getNickname(), curr_chan->getName() + " " + *it + "!*@127.0.0.1 " +  cl->getNickname(), ":");
		}
	}
	else if (m == 'o')
		curr_chan->operators.push_back(serv->findClientByName(cmd[3]));
}

void	minusParamMode(char m, std::vector<std::string> cmd, channel *curr_chan)
{
	/* [ +k <key> ] [ +l <max_user> ] [ +b <users_banlist> ] [ +t <topic> ] [ +v <user> ] [ +o <user> ] */
	if (m == 'k')
	{
		curr_chan->setPassword("");
		curr_chan->modes.erase(curr_chan->modes.find(m), 1);
	}
	else if (m == 'l')
	{
		curr_chan->max_user = 10; /* Reset to default */
		curr_chan->modes.erase(curr_chan->modes.find(m), 1);
	}
	else if (m == 't')
		curr_chan->modes.erase(curr_chan->modes.find(m), 1);
	else if (m == 'b' && cmd.size() < 4)
	{
		curr_chan->black_list.clear();
		curr_chan->modes.erase(curr_chan->modes.find(m), 1);
	}
	else if (m == 'v' && cmd.size() < 4)
	{
		curr_chan->muteList.clear();
		curr_chan->modes.erase(curr_chan->modes.find(m), 1);
	}
	else if (m == 'b')
	{
		std::vector<std::string>::iterator	it = curr_chan->findIteratorStr(curr_chan->black_list, cmd[3]);
		if (it != curr_chan->black_list.end())
			curr_chan->black_list.erase(curr_chan->findIteratorStr(curr_chan->black_list, cmd[3]));
	}
	else if (m == 'v')
	{
		std::vector<std::string>::iterator	it = curr_chan->findIteratorStr(curr_chan->muteList, cmd[3]);
		if (it != curr_chan->muteList.end())
			curr_chan->muteList.erase(it);
	}
	else if (m == 'o' && cmd.size() >= 4)
	{
		if (curr_chan->isOperator(cmd[3]))
		{
			std::vector<client *>::iterator	it = curr_chan->findIteratorClient(curr_chan->operators, cmd[3]);
			if (it != curr_chan->operators.end())
				curr_chan->operators.erase(it);
		}
	}
}

int check_mode(std::vector<std::string>& cmd, client* cl)
{
	std::string		msg;
	bool			error = false;
	for (unsigned int j = 1 ; j < cmd[2].length() ; j++)
	{
		if (cmd[2][j] == 'o' || cmd[2][j] == 'k' || cmd[2][j] == 'v' || cmd[2][j] == 'l' || cmd[2][j] == 'i' || \
			cmd[2][j] == 's' || cmd[2][j] == 'p' || cmd[2][j] == 'b' || cmd[2][j] == 'n' || cmd[2][j] == 't' || cmd[2][j] == 'm')
			;
		else
		{
			if (error == false) {
				msg = ":NiceIRC 472 " + cl->getNickname() + " " + cmd[2][j] + " :is unknown mode char to me for " + cmd[1] + "\r\n";
				send(cl->getFd(), msg.c_str(), msg.length(), 0);
				error = true;
			}
			std::string::iterator	it = cmd[2].begin() + cmd[2].find(cmd[2][j]);
			cmd[2].erase(it);
		}
	}
	return (0);
}

bool	removeMode(client* cl, std::vector<std::string> cmd, channel* curr_chan)
{
	size_t	pos;
	bool	mode_arg = false;
	for (unsigned int i = 1; i < cmd[2].length(); i++) {
		if ((curr_chan->modes.find(cmd[2][i]) != std::string::npos) || cmd[2][i] == 'o' || cmd[2][i] == 'b')
		{
			if (!mode_arg && (cmd[2][i] == 'k' || cmd[2][i] == 'l' || cmd[2][i] == 'b' || \
							cmd[2][i] == 't' || cmd[2][i] == 'v' || cmd[2][i] == 'o'))
			{
				minusParamMode(cmd[2][i], cmd, curr_chan);
				mode_arg = true;
			}
			else if (mode_arg && (cmd[2][i] == 'k' || cmd[2][i] == 'l' || cmd[2][i] == 'b' || \
								cmd[2][i] == 't' || cmd[2][i] == 'v' || cmd[2][i] == 'o'))
			{
				send_error_code(cl->getFd(), "666", cl->getNickname(), std::string(1, cmd[2][i]), ":Only one mode with parameter can be taken.");
				return false;
			}
			else if (!(cmd[2][i] == 'k' || cmd[2][i] == 'l' || cmd[2][i] == 'b' || \
						cmd[2][i] == 't' || cmd[2][i] == 'v' || cmd[2][i] == 'o') && \
						(pos = curr_chan->modes.find(cmd[2][i])) != std::string::npos)
				curr_chan->modes.erase(pos, 1);
		}
	}
	return true;
}

bool	addMode(client* cl, std::vector<std::string> cmd, channel* curr_chan, server* serv)
{
	bool	mode_arg = false;
	for (unsigned int i = 1; i < cmd[2].length(); i++)
	{
		if (!mode_arg && (cmd[2][i] == 'k' || cmd[2][i] == 'l' || cmd[2][i] == 'b' || \
						cmd[2][i] == 't' || cmd[2][i] == 'v' || cmd[2][i] == 'o'))
		{
			/* handle parameters for mode */
			if ((cmd[2][i] == 'k' || cmd[2][i] == 'l' || \
						cmd[2][i] == 't' || cmd[2][i] == 'o') && cmd.size() < 4)
			{
				send_error_code(cl->getFd(), "696", cl->getNickname(), std::string(1, cmd[2][i]), ":You must specify a parameter for this mode.");
				return false;
			}
			plusParamMode(cl, cmd[2][i], cmd, curr_chan, serv);
			mode_arg = true;
			if (curr_chan->modes.find(cmd[2][i]) == std::string::npos && cmd[2][i] != 'o' && cmd[2][i] != 'b' && cmd[2][i] != 'v')
				curr_chan->modes.push_back(cmd[2][i]);
		}
		else if (mode_arg && (cmd[2][i] == 'k' || cmd[2][i] == 'l' || cmd[2][i] == 'b' || \
							cmd[2][i] == 't' || cmd[2][i] == 'v' || cmd[2][i] == 'o'))
		{
			send_error_code(cl->getFd(), "666", cl->getNickname(), std::string(1, cmd[2][i]), ":Only one mode with parameter can be taken.");
			return false;
		}
		
		else if (curr_chan->modes.find(cmd[2][i]) == std::string::npos)
			curr_chan->modes.push_back(cmd[2][i]);
	}
	return true;
}

void	cmd_mode(client* cl, std::vector<std::string> cmd, server* serv)
{
	std::string	msg;

	if (cmd.size() < 2)
		send_error_code(cl->getFd(), "461", cl->getNickname(), cmd[0], ":Not enough parameters");
	else if (cmd[1][0] != '#' && cmd[1][0] != '&')
		send_error_code(cl->getFd(), "666", cl->getNickname(), cmd[0], ":We do not handle client modes !");
	else if (serv->findChannelByName(cmd[1]) == NULL)
		send_error_code(cl->getFd(), "403", cl->getNickname(), cmd[1], ":No such channel");
	else if (cmd.size() == 2)
		send_error_code(cl->getFd(), "324", cl->getNickname(), serv->findChannelByName(cmd[1])->getName() + " :+" + serv->findChannelByName(cmd[1])->modes, "");
	else if (serv->findChannelByName(cmd[1])->isOperator(cl->getNickname()) == false)
		send_error_code(cl->getFd(), "482", cl->getNickname(), cmd[1], ":You must have channel op access or above to set channel modes.");
	else
	{
		channel		*curr_chan = serv->findChannelByName(cmd[1]);
		check_mode(cmd, cl); // ARG MODES -> [ +k <key> ] [ +l <max_user> ] [ +b <users_banlist> ] [ +t <topic> ] [ +v <user> ] [ +o <user> ]
		if (cmd[2][0] == '+')
		{
			if (addMode(cl, cmd, curr_chan, serv)) {
				msg = ":" + cl->getNickname() + "!" + cl->getUsername() + "@127.0.0.1 " + cmd[0] + " " + cmd[1] + " :" + cmd[2] + "\r\n";
				send(cl->getFd(), msg.c_str(), msg.length(), 0);
			}
		}
		else if (cmd[2][0] == '-')
		{
			if (removeMode(cl, cmd, curr_chan)) {
				msg = ":" + cl->getNickname() + "!" + cl->getUsername() + "@127.0.0.1 " + cmd[0] + " " + cmd[1] + " :" + cmd[2] + "\r\n";
				send(cl->getFd(), msg.c_str(), msg.length(), 0);
			}
		}
	}
}
