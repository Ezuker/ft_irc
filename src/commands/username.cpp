/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   username.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 16:18:15 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/16 01:00:25 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

static	Client	*usernameExist(std::vector<Client *> &client, std::string nickname)
{
	for (std::vector<Client *>::iterator it = client.begin(); it != client.end(); ++it)
	{
		if ((*it)->getUserName() == nickname)
			return (*it);
	}
	return (NULL);
}

void	Server::_usernameCase(Client & cl, std::string const & message)
{
	std::string toSet = message;
	
	if (!std::strncmp(message.c_str(), "USER ", 5))
	{
		toSet = strtrim(toSet);
		toSet = split(message, ' ')[1];
		if (usernameExist(this->_clients, toSet))
			this->_sendMessageToClient(ERR_ALREADYREGISTERED, &cl);
		else
		{
			cl.setUserName(toSet);
			std::cout << "Username set to : " << toSet << std::endl;
		}
	}
	else if (message.size() == 4)
		this->_sendMessageToClient(":" + cl.getHostName() + " " + ERR_NEEDMOREPARAMS(cl.getNickName(), "USER"), &cl);
	else
		this->_sendMessageToClient("UNKNOWN COMMAND\n", &cl);
	return ;
}
