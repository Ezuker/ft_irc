/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   username.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 16:18:15 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/19 13:37:18 by ehalliez         ###   ########.fr       */
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
	
	if (!this->checkCommand("USER", message, cl))
		return ;
	toSet = strtrim(toSet);
	std::vector<std::string> splits = split(message, ' ');
	if (splits.size() != 5 || splits[4][0] != ':')
	{
		this->sendErrToClient(cl, ERR_NEEDMOREPARAMS(cl.getNickName(), "USER"));
		return ;
	}
	toSet = split(message, ' ')[1];
	if (usernameExist(this->_clients, toSet))
		this->sendErrToClient(cl, ERR_ALREADYREGISTERED(toSet));
	else
	{
		if (cl.getUserName().empty())
		{
			std::string toSend = ":" + this->_hostname + " 001 " + cl.getNickName() + " Welcome to TeamWithoutSpeak Server we know you as (" + cl.getNickName() + ")\r\n";
			send(cl.getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
		}
		cl.setUserName(toSet);
	}
	return ;
}
