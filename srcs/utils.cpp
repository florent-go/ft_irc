/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epfennig <epfennig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 16:45:05 by epfennig          #+#    #+#             */
/*   Updated: 2021/12/09 15:30:07 by epfennig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/irc.hpp"

void	exit_error(std::string error)
{
	std::cout << "[-] Error: "<< error << std::endl;
	exit(EXIT_FAILURE);
}

std::string	 ft_itos ( int Number )
{
	std::ostringstream	ss;
	ss << Number;
	return ss.str();
}

std::vector<std::string>	ft_split(std::string str, std::string delimiter, int times)
{
	size_t							pos;
	std::string						token;
	std::vector<std::string>		tab;

	while ((pos = str.find(delimiter)) != std::string::npos && times)
	{
		token = str.substr(0, pos);
		// if (pos != 0)
		tab.push_back(token);
		str.erase(0, pos + delimiter.length());
		times--;
	}
	tab.push_back(str);
	return (tab);
}

std::vector<std::string>	ft_split_c(std::string str, char	c)
{
	std::string						buff;
	std::stringstream				token(str);
	std::vector<std::string>		tab;

	while (getline(token, buff, c))
	{
		if (!buff.empty())
			tab.push_back(buff);
	}
	return (tab);
}

int		ft_strlen(char *str)
{
	int i = 0;
	for ( ; str[i] != '\0' ; )
		i++;
	return (i);
}