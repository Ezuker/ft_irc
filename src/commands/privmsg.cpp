/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:34:20 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/16 09:01:44 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


// CHECK If the client is in the channel before send the message

void	Server::_privmsgCase(Client & client, std::string const & message)
{
	if (!this->checkCommand("PRIVMSG", message, client))
		return ;
	std::vector<std::string> splitted = split(message, ' ');
	std::string channel = splitted[1];

	Channel *toSend = this->_channelExists(channel);
	if (channel.size() && channel[0] == '#')
	{
		if (!toSend)
			return (this->_sendMessageToClient(":" + this->_hostname + " " + ERR_NOSUCHCHANNEL(client.getNickName(), channel), &client));
		if (!client._isInChannel(*toSend))
			return (this->_sendMessageToClient(":" + this->_hostname + " " + ERR_NOTONCHANNEL(client.getNickName(), toSend->getChannelName()), &client));
	}
	else
	{
		std::vector<Client *>::iterator it= this->_clients.begin();
		for (; it != this->_clients.end(); ++it)
		{
			if ((*it)->getNickName() == channel)
				break ;
		}
		if (it == this->_clients.end())
			return (this->_sendMessageToClient(":" + this->_hostname + " " + ERR_NOSUCHCHANNEL(client.getNickName(), channel), &client));
		std::string messageToSend = getMask(client) + message + "\r\n";
		send((*it)->getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
		return ;
	}
	std::string messageToSend;
	std::vector<Client *>::iterator it = toSend->getClients().begin();
	for (; it != toSend->getClients().end(); ++it)
	{
		if (*it == &client)
			continue ;
		messageToSend += getMask(client) + message  + "\r\n";
		send((*it)->getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
		messageToSend = ":";
	}
}
