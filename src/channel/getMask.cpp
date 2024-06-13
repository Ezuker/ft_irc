/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getMask.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 18:37:19 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/13 19:34:55 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

std::string Channel::getMaskList(void)
{
	std::string				result;
	std::vector<Client *> clients = this->_clients;
	std::vector<Client *>::iterator it = this->_clients.begin();
	for (; it != this->_clients.end(); ++it)
	{
		if (it == this->_clients.begin())
			result += ":@" + (*it)->getNickName() + "!" + (*it)->getUserName() + "@" + (*it)->getHostName();
		else
			result += " " + (*it)->getNickName() + "!" + (*it)->getUserName() + "@" + (*it)->getHostName();
		
	}
	std::cout << result << std::endl;
	return (result);
}
