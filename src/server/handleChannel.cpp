/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleChannel.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 05:29:21 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/14 02:20:35 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "global.hpp"

Channel	*Server::_channelExists(std::string name)
{
	std::vector<Channel *>::iterator it = this->_channels.begin();
	for (; it != this->_channels.end(); ++it)
	{
		if ((*it)->getChannelName() == name)
			return (*it);
	}
	return (NULL);
}

int	Server::_createChannel(std::string name, int i)
{
	std::string message;
	
	Channel *newChannel = new Channel(name, this->_clients[i - 1]);
	this->_clients[i - 1]->getBelongChannel().push_back(newChannel);
	this->_channels.push_back(newChannel);
	message = ":" + this->_clients[i - 1]->getHostName() + " MODE #" + name + " +tn\n";
	this->_sendMessageToClient(message, i);
	this->refreshList(newChannel);
	return (1);
}

void	Server::refreshList(Channel *channel)
{
	std::vector<Client *> clients = channel->getClients();
	std::string	toSend;
	for (unsigned int i = 0; i < channel->getClients().size(); i++)
	{
		toSend = ":" + clients[i]->getNickName() + "!" + clients[i]->getUserName() + "@" + clients[i]->getHostName() + " JOIN :#" + channel->getChannelName() + "\n";
		send(clients[i]->getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
		toSend = ":" + clients[i]->getHostName() + " 331 " + clients[i]->getNickName() + " TOPIC #" + channel->getChannelName() + " :No topic set\n";
		send(clients[i]->getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
		
		toSend = ":" + clients[i]->getHostName() + " 353 " + clients[i]->getNickName() + " = #" + channel->getChannelName() + " " + channel->getMaskList2() + "\n";
		send(clients[i]->getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
		toSend = ":" + clients[i]->getHostName() + " 366 " + clients[i]->getNickName() + " #" + channel->getChannelName() + " :End of /NAMES list.\n";
		send(clients[i]->getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
	}
}
