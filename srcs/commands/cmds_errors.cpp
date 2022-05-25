/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_errors.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 10:30:33 by epfennig          #+#    #+#             */
/*   Updated: 2021/12/08 10:30:33 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/client.hpp"
#include "../classes/server.hpp"
#include "../classes/parser.hpp"
#include "../classes/channel.hpp"

void	send_error_code(unsigned int fd, std::string error_code, std::string nickname, std::string arg, std::string error_msg)
{
	std::string		msg;
	msg = ":NiceIRC " + error_code + " " + nickname + " " + arg + " " + error_msg + "\r\n";
	send(fd, msg.c_str(), msg.length(), 0);
}
