/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 20:34:03 by epfennig          #+#    #+#             */
/*   Updated: 2021/12/05 18:30:04 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

#include <iostream>
#include <vector>
#include <utility>

class client;
class server;
class channel;

class parser
{
	public:
			enum msg_type {
				PASS,
				NICK,
				USER,
				OPER,
				JOIN,
				PRIVMSG,
				KILL,
				PART,
				MODE,
				KICK,
				LIST,
				TOPIC,
				INVITE,
				NAMES,
				PING,
				MSG
			};

			std::string		msg;		// Buffer envoyé au parser
			std::string		prefix;		// if prefix, print it in place of nickname
			std::string		tab[15];	// Tableau des commandes

			int				cmd_type;	// Categorie de commande

			parser();
			~parser(void);

			void	parsing(client* cli, std::string str, server* serv);
			int		whatIsCmd(std::string cmd);
};

/* ======= CMD FUNCTIONS ======= */
void						cmd_help(client* cl);
void						cmd_pass(client* cl, std::vector<std::string> cmd, server* serv);
void						cmd_user(client* cl, std::vector<std::string> cmd);
void						cmd_nick(client* cl, std::vector<std::string> cmd, server* serv);
void						cmd_oper(client* cl, std::vector<std::string> cmd, server* serv);
void						cmd_privmsg(client* cl, std::vector<std::string> cmd,  server* serv);
void						cmd_kill(client* cl, std::vector<std::string> cmd, server* serv);
void						cmd_join(client* cl, std::vector<std::string>	cmd, server* serv);
void						sendToChan(client* cl, channel* chan, std::string msg);
void						cmd_part(client* cl, std::vector<std::string> cmd, server* serv);
void						cmd_kick(client* cl, std::vector<std::string> cmd, server* serv);
void						cmd_mode(client* cl, std::vector<std::string> cmd, server* serv);
void						cmd_list(client* cl, std::vector<std::string> cmd, server* serv);
void						cmd_topic(client* cl, std::vector<std::string> cmd, server* serv);
void						cmd_invite(client* cl, std::vector<std::string> cmd, server* serv);
void						cmd_names(client* cl, std::vector<std::string> cmd, server* serv);
void						send_error_code(unsigned int fd, std::string error_code, std::string nickname, \
											std::string arg1, std::string error_msg);

#include "channel.hpp"
#include "client.hpp"
#include "../../includes/irc.hpp"

#endif