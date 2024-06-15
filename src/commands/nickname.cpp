/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nickname.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 16:15:20 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/15 05:55:09 by bcarolle         ###   ########.fr       */
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
	std::string toSet = message;
	
	if (!std::strncmp(message.c_str(), "NICK ", 5))
	{
		toSet = message.substr(5, message.size() - 5);
		if (nicknameExist(this->_clients, toSet))
			this->_sendMessageToClient(ERR_NICKNAMEINUSE, &cl);
		else
		{
			cl.setNickName(toSet);
			std::cout << "Nickname set to : " << toSet << std::endl;
		}
	}
	else if (message.size() == 4)
		this->_sendMessageToClient(ERR_NONICKNAMEGIVEN, &cl);
	else
		this->_sendMessageToClient("UNKNOWN COMMAND\n", &cl);
	return ;
}
