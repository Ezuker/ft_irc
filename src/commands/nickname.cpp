/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nickname.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 16:15:20 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/14 18:17:03 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

static	Client	*nicknameExist(std::vector<Client *> &client, std::string nickname)
{
	for (std::vector<Client *>::iterator it = client.begin(); it != client.end(); ++it)
	{
		if ((*it)->getNickName() == nickname)
			return (*it);
	}
	return (NULL);
}

void	Server::_nicknameCase(Client & cl, std::string const & message)
{
	std::string toSet;
	
	toSet = message.substr(5, message.size() - 5);
	if (nicknameExist(this->_clients, toSet))
	{
		std::string err = "443 " + toSet + " Nickname already in use";
		_sendMessageToClient(err, &cl);
	}
	else
	{
		cl.setNickName(toSet);
		std::cout << "Nickname set to : " << toSet << std::endl;
	}
	return ;
}
