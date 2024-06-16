/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 16:18:56 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/16 10:03:47 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::_joinChannel(Client & cl, std::string & message, int const & i)
{
	std::vector<std::string> toSet;

	if (!this->checkCommand("JOIN", message, cl))
		return ;
	toSet = split(message, ' ');
	if (toSet[1][0] != '#')
		return (this->_sendMessageToClient(":" + this->_hostname + " " + ERR_BADCHANMASK(cl.getNickName()), &cl));
	Channel *channelCheck = this->_channelExists(toSet[1]);
	if (channelCheck)
	{
		if (channelCheck->getMode().invitation)
		{
			std::vector<Client *>::iterator findInvite;

			findInvite = find(channelCheck->getInvites().begin(), channelCheck->getInvites().end(), &cl);
			std::string messageToSend = ":" + this->_hostname + " 473 " + cl.getNickName() + " " + channelCheck->getChannelName() + ": Cannot join channel (Invite only)\r\n";
			if (findInvite == channelCheck->getInvites().end())
			{
				send(cl.getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
				return ;
			}
			channelCheck->getInvites().erase(findInvite);
		}
		if (channelCheck->getMode().userLimit > 0 && static_cast<size_t>(channelCheck->getMode().userLimit) >= channelCheck->getClients().size())
			return (this->_sendMessageToClient(":" + this->_hostname + " " + ERR_CHANNELISFULL(toSet[1]), &cl));
		if (!channelCheck->getMode().password.empty() && channelCheck->getMode().password != toSet[2])
			return (this->_sendMessageToClient(":" + this->_hostname + " " + ERR_BADCHANNELKEY(toSet[1]), &cl));
		channelCheck->getClients().push_back(&cl);
		cl.getBelongChannel().push_back(channelCheck);
		this->_refreshList(channelCheck);
	}
	else
		this->_createChannel(toSet[1], i);
	return ;
}

void	Server::_refreshList(Channel *channel)
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
	this->_refreshList(newChannel);
	return (1);
}
