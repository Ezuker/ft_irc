/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getMask.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 18:37:19 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/14 01:34:13 by bcarolle         ###   ########.fr       */
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
	std::string				result = ":";
	std::vector<Client *>::iterator it = this->_clients.begin();
	for (; it != this->_clients.end(); ++it)
	{
		if (std::find(this->_operators.begin(), this->_operators.end(), *it) != this->_operators.end())
			result += " @" + (*it)->getNickName();
		else
			result += " " + (*it)->getNickName();
	}
	std::cout << result << std::endl;
	return (result);
}

std::string Channel::getClientList(void)
{
	std::vector<Client *>::iterator it = this->_clients.begin();
	std::string				result = ":" + this->_clients.front()->getNickName();
	it++;
	for (; it != this->_clients.end(); ++it)
	{
		result += " " + (*it)->getNickName();
	}
	std::cout << result << std::endl;
	return (result);
}

std::string Channel::getOperatorList(void)
{
	std::vector<Client *>::iterator it = this->_operators.begin();
	std::string				result = ":@" + this->_operators.front()->getNickName();
	it++;
	for (; it != this->_operators.end(); ++it)
	{
		result += " @" + (*it)->getNickName();
	}
	std::cout << result << std::endl;
	return (result);
}
