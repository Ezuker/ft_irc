/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   username.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 16:18:15 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/14 18:30:38 by bcarolle         ###   ########.fr       */
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
	std::string toSet;
	
	if (cl.getNickName().size() == 0|| cl.getUserName().size() > 0)
		return ;
	toSet = split(message, ' ')[1];
	if (usernameExist(this->_clients, toSet))
	{
		std::string err = "462 " + toSet + " :Already registered";
		_sendMessageToClient(err, &cl);
	}
	else
	{
		cl.setUserName(toSet);
		std::cout << "Username set to : " << toSet << std::endl;
	}
	return ;
}
