/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 10:27:23 by epfennig          #+#    #+#             */
/*   Updated: 2021/11/23 11:17:09 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/irc.hpp"

int	main(int ac, char **av)
{
	server	serv;
	int		i = 0;

	if (ac >= 3 && ac < 5)
	{
		if (ac == 3 && (i = 1))
			serv.setPassword(std::string(av[2]));
		else if (ac == 4 && (i = 2))
			serv.setPassword(std::string(av[3]));
		serv.parse_config_file();
		tcp_protocol(&serv, av[i]);
	}
	else
		std::cout << "Usage: ./ircserv [host:port_network:password_network] <port> <password>" << std::endl;
}
