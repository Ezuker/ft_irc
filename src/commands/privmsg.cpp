/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:34:20 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/16 01:50:39 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


// CHECK If the client is in the channel before send the message

void	Server::_privmsgCase(Client & client, std::string const & message)
{
	std::string channel = split(message, ' ')[1];

	Channel *toSend = this->_channelExists(channel);
	if (!toSend)
		return (this->_sendMessageToClient(":" + client.getHostName() + " " + ERR_NOSUCHCHANNEL(client.getNickName(), channel[1]), &client));
	if (!client._isInChannel(*toSend))
		return (this->_sendMessageToClient(":" + client.getHostName() + " " + ERR_NOTONCHANNEL(client.getNickName(), toSend->getChannelName()), &client));
	std::string messageToSend;
	std::vector<Client *>::iterator it = toSend->getClients().begin();
	for (; it != toSend->getClients().end(); ++it)
	{
		if (*it == &client)
			continue ;
		messageToSend += getMask(client) + message  + "\n";
		send((*it)->getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
		messageToSend = ":";
	}
}
