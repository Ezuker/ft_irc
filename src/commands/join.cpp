/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 16:18:56 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/16 03:32:12 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::joinChannel(Client & cl, std::string const & message, int const & i)
{
	std::string toSet;
	
	if (cl.getNickName().size() == 0 || cl.getUserName().size() == 0)
		return ;
	if (message.size() >= 6 && message[5] != '#')
		return ;
	// envoyer une erreur
	toSet = message.substr(5, message.size() - 5);
	Channel *channelCheck = this->_channelExists(toSet);
	if (channelCheck)
	{
		channelCheck->getClients().push_back(&cl);
		cl.getBelongChannel().push_back(channelCheck);
		this->refreshList(channelCheck);
	}
	else
		this->_createChannel(toSet, i);
	return ;
}

void	Server::refreshList(Channel *channel)
{
	std::vector<Client *> clients = channel->getClients();
	std::string	toSend;
	for (unsigned int i = 0; i < channel->getClients().size(); i++)
	{
		toSend = getMask(*clients[i]) + "JOIN :" + channel->getChannelName() + "\n";
		send(clients[i]->getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
		toSend = ":" + clients[i]->getHostName() + " 331 " + clients[i]->getNickName() + " TOPIC " + channel->getChannelName() + " :No topic set\n";
		send(clients[i]->getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
		
		toSend = ":" + clients[i]->getHostName() + " 353 " + clients[i]->getNickName() + " = " + channel->getChannelName() + " " + channel->getMaskList2() + "\n";
		send(clients[i]->getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
		toSend = ":" + clients[i]->getHostName() + " 366 " + clients[i]->getNickName() + " " + channel->getChannelName() + " :End of /NAMES list.\n";
		send(clients[i]->getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
	}
}

int	Server::_createChannel(std::string name, int i)
{
	std::string message;
	
	if (!this->_checkChannelName(name))
	{
		std::string errMessage = ":" + this->_clients[i - 1]->getHostName() + " " + ERR_BADCHANMASK(name);
		this->_sendMessageToClient(message, this->_clients[i - 1]);
		return 0;
	}
	Channel *newChannel = new Channel(name, this->_clients[i - 1]);
	newChannel->getMode().userLimit = -1;
	newChannel->getMode().password = "";
	newChannel->getMode().changeTopic = true;
	newChannel->getMode().invitation = false;
	this->_clients[i - 1]->getBelongChannel().push_back(newChannel);
	this->_channels.push_back(newChannel);
	message = ":" + this->_clients[i - 1]->getHostName() + " MODE #" + name + " +tn\n";
	this->_sendMessageToClient(message, this->_clients[i - 1]);
	this->refreshList(newChannel);
	return (1);
}
