/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleChannel.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 05:29:21 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/14 01:39:25 by bcarolle         ###   ########.fr       */
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
	this->_channels.push_back(newChannel);
	message = ":" + this->_clients[i - 1]->getHostName() + " MODE #" + name + " +tn\n";
	this->_sendMessageToClient(message, i);
	// message = ":" + this->_clients[i - 1]->getNickName() + "!" + this->_clients[i - 1]->getUserName() + "@" + this->_clients[i - 1]->getHostName() + " JOIN :#" + name + "\n";
	// this->_sendMessageToClient(message, i);
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
		
		// CLIENT LIST
		toSend = ":" + clients[i]->getHostName() + " 353 " + clients[i]->getNickName() + " = #" + channel->getChannelName() + " " + channel->getMaskList2() + "\n";
		send(clients[i]->getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
		toSend = ":" + clients[i]->getHostName() + " 366 " + clients[i]->getNickName() + " #" + channel->getChannelName() + " :End of /NAMES list.\n";
		send(clients[i]->getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);

		// // OPERATOR LIST 
		
		// toSend = ":" + clients[i]->getHostName() + " 353 " + clients[i]->getNickName() + " = #" + channel->getChannelName() + " " + channel->getOperatorList() + "\n";
		// send(clients[i]->getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
		// toSend = ":" + clients[i]->getHostName() + " 366 " + clients[i]->getNickName() + " #" + channel->getChannelName() + " :End of /NAMES list.\n";
		// send(clients[i]->getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
		
		// toSend = ":" + clients[i]->getHostName() + " 353 " + clients[i]->getNickName() + " @ #" + channel->getChannelName() + " " + channel->getMaskList2() + "\n";
		// send(clients[i]->getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
		// toSend = ":" + clients[i]->getHostName() + " 324 " + clients[i]->getNickName() + " #" + channel->getChannelName() + " tn\n";
		// send(clients[i]->getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
	}
}

// void	Server::_joinChannel(Channel *channel, int i)
// {
// 	std::string message;
// 	std::vector<Client *> clients;

// 	std::string list;
	
// 	// response = ":" + this->_clients[i - 1]->getHostName() + " 366 " + this->_clients[i - 1]->getNickName() + " #" + channel->getChannelName() + " :End of /NAMES list.\n";
// 	// this->_sendMessageToClient(response, i);
//  }

void Server::_sendMessageToChannelClients(Client *sender, const std::string &message)
{
    std::string senderNick = sender->getNickName();
    std::string senderUser = sender->getUserName();
    std::string senderHost = sender->getHostName();
    std::string senderChannel = sender->getCurrentChannel();

    std::string messageToSend = ":" + senderNick + "!~" + senderUser + "@" + senderHost + " " + message + "\n";

    Channel *channel = this->_channelExists(senderChannel);
    if (!channel) {
        return; // Return if channel doesn't exist
    }

    std::vector<Client *> channelClients = channel->getClients();
    std::vector<Client *>::iterator it;
    for (it = channelClients.begin(); it != channelClients.end(); ++it)
    {
        if (*it != sender)
        {
			std::string toSend = "PRIVMSG #" + senderChannel + ":" + message; 
			send((*it)->getIdentifier(), toSend.c_str(), toSend.size(), 0);
        }
    }
}
