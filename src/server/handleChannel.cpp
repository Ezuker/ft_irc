/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleChannel.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 05:29:21 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/13 18:52:21 by ehalliez         ###   ########.fr       */
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
	message = ":" + this->_clients[i - 1]->getNickName() + "!" + this->_clients[i - 1]->getUserName() + "@" + this->_clients[i - 1]->getHostName() + " JOIN :#" + name + "\n";
	this->_sendMessageToClient(message, i);
	this->_clients[i - 1]->setCurrentChannel(name);
	this->_joinChannel(newChannel, i);
	std::string response = ":" + this->_clients[i - 1]->getHostName() + " 331 " + this->_clients[i - 1]->getNickName() + " TOPIC #" + name + " :No topic is set\n";
	this->_sendMessageToClient(response, i);
	response = ":" + this->_clients[i - 1]->getHostName() + " 353 " + this->_clients[i - 1]->getNickName() + " @ #" + name + " :" + this->_clients[i - 1]->getNickName() + "\n";
	this->_sendMessageToClient(response, i);
	response = ":" + this->_clients[i - 1]->getHostName() + " 366 " + this->_clients[i - 1]->getNickName() + " #" + name + " :End of /NAMES list.\n";
	this->_sendMessageToClient(response, i);
	response = ":" + this->_clients[i - 1]->getHostName() + " 353 " + this->_clients[i - 1]->getNickName() + " @ #" + name + " :@" + this->_clients[i - 1]->getNickName() + "\n";
	this->_sendMessageToClient(response, i);
	response = ":" + this->_clients[i - 1]->getHostName() + " 366 " + this->_clients[i - 1]->getNickName() + " #" + name + " :End of /NAMES list.\n";
	this->_sendMessageToClient(response, i);
	response = ":" + this->_clients[i - 1]->getHostName() + " 324 " + this->_clients[i - 1]->getNickName() + " #" + name + " tn\n";
	this->_sendMessageToClient(response, i);
	return (1);
}

void	Server::_joinChannel(Channel *channel, int i)
{
	std::string message;
	std::vector<Client *> clients;

	clients = channel->getClients();
	clients.push_back(this->_clients[i - 1]);
	channel->setClients(clients);
	this->_clients[i - 1]->setCurrentChannel(channel->getChannelName());
	std::string list;
	channel->getMaskList();
	message = ":" + this->_clients[i - 1]->getNickName() + this->_clients[i - 1]->getUserName() + this->_clients[i - 1]->getHostName() + " JOIN :#" + channel->getChannelName() + "\n";
	this->_sendMessageToClient(message, i);
 }

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
