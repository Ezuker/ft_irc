/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   username.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 16:18:15 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/17 11:16:44 by bcarolle         ###   ########.fr       */
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
	toSet = split(message, ' ')[1];
	if (usernameExist(this->_clients, toSet))
		this->sendErrToClient(cl, ERR_ALREADYREGISTERED(toSet));
	else
	{
		cl.setUserName(toSet);
		std::cout << "Username set to : " << toSet << std::endl;
	}
	return ;
}
