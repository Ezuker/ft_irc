/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 16:18:56 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/16 06:53:35 by ehalliez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::joinChannel(Client & cl, std::string & message, int const & i)
{
	std::string toSet;

	if (std::strncmp(message.c_str(), "JOIN ", 5))
		return (this->_sendMessageToClient(":" + this->_hostname + " " + ERR_NEEDMOREPARAMS(cl.getNickName(), "JOIN"), &cl));
	toSet = message.substr(5, message.size() - 5);
	if (toSet.empty())
		return (this->_sendMessageToClient(":" + this->_hostname + " " + ERR_NEEDMOREPARAMS(cl.getNickName(), "JOIN"), &cl));
	if (toSet[0] != '#')
		return (this->_sendMessageToClient(":" + this->_hostname + " " + ERR_BADCHANMASK(cl.getNickName()), &cl));
	Channel *channelCheck = this->_channelExists(toSet);
	if (channelCheck)
	{
		if (channelCheck->getMode().userLimit > 0 && static_cast<size_t>(channelCheck->getMode().userLimit) >= channelCheck->getClients().size())
			return ;
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
		toSend = getMask(*clients[i]) + "JOIN :" + channel->getChannelName() + "\r\n";
		send(clients[i]->getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
		toSend = ":" + this->_hostname + " 331 " + clients[i]->getNickName() + " TOPIC " + channel->getChannelName() + " :No topic set\r\n";
		send(clients[i]->getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
		
		toSend = ":" + this->_hostname + " 353 " + clients[i]->getNickName() + " = " + channel->getChannelName() + " " + channel->getMaskList2() + "\r\n";
		send(clients[i]->getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
		toSend = ":" + this->_hostname + " 366 " + clients[i]->getNickName() + " " + channel->getChannelName() + " :End of /NAMES list.\r\n";
		send(clients[i]->getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
	}
}

int	Server::_createChannel(std::string name, int i)
{
	std::string message;
	
	if (!this->_checkChannelName(name))
	{
		std::string errMessage = ":" + this->_hostname + " " + ERR_BADCHANMASK(name);
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
	message = ":" + this->_hostname + " MODE #" + name + " +tn\r\n";
	this->_sendMessageToClient(message, this->_clients[i - 1]);
	this->refreshList(newChannel);
	return (1);
}
