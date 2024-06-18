/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nickname.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 16:15:20 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/18 16:38:11 by ehalliez         ###   ########.fr       */
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
	
	if ("NICK" == message)
		return (this->sendErrToClient(cl, ERR_NONICKNAMEGIVEN));
	if (!this->checkCommand("NICK", message, cl))
		return ;
	toSet = message.substr(5, message.size() - 5);
	if (nicknameExist(this->_clients, toSet))
		this->sendErrToClient(cl, ERR_NICKNAMEINUSE(cl.getNickName()));
	else if (isValidName(toSet))
	{
		std::string messageToSend = getMask(cl) + message + "\r\n";
		if (cl.getNickName().empty())
		{
			send(cl.getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
			cl.setNickName(toSet);
		}
		else
		{
			std::vector<Channel *>::iterator it = cl.getBelongChannel().begin();
			for (; it != cl.getBelongChannel().end(); ++it)
			{
				(*it)->sendMessageToClient(messageToSend);
			}
		}
	}
	else if (!toSet.size())
		this->sendErrToClient(cl, ERR_NONICKNAMEGIVEN);
	else
		this->sendErrToClient(cl, ERR_ERRONEUSNICKNAME(toSet));
	return ;
}
