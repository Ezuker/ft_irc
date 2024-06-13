/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getMask.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 18:37:19 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/14 00:50:22 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

std::string Channel::getMaskList(void)
{
	std::string				result;
	std::vector<Client *>::iterator it = this->_clients.begin();
	for (; it != this->_clients.end(); ++it)
	{
		if (std::find(this->_operators.begin(), this->_operators.end(), *it) != this->_operators.end())
			result += ":@" + (*it)->getNickName() + "!" + (*it)->getUserName() + "@" + (*it)->getHostName();
		else
			result += " " + (*it)->getNickName() + "!" + (*it)->getUserName() + "@" + (*it)->getHostName();
	}
	std::cout << result << std::endl;
	return (result);
}

std::string Channel::getMaskList2(void)
{
	std::string				result;
	std::vector<Client *>::iterator it = this->_clients.begin();
	for (; it != this->_clients.end(); ++it)
	{
		if (std::find(this->_operators.begin(), this->_operators.end(), *it) != this->_operators.end())
			result += ":@" + (*it)->getNickName();
		else
			result += " " + (*it)->getNickName();
	}
	std::cout << result << std::endl;
	return (result);
}
