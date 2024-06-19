/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 16:18:56 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/18 18:33:24 by ehalliez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::_joinChannel(Client & cl, std::string & message)
{
	std::vector<std::string> toSet;

	if (!this->checkCommand("JOIN", message, cl))
		return ;
	toSet = split(message, ' ');
	if (toSet[1][0] != '#')
		return (this->sendErrToClient(cl, ERR_BADCHANMASK(cl.getNickName())));
	Channel *channelCheck = this->_channelExists(toSet[1]);
	if (channelCheck)
	{
		std::vector<Client *>::iterator findInvite;
		if (channelCheck->getMode().invitation)
		{

			findInvite = find(channelCheck->getInvites().begin(), channelCheck->getInvites().end(), &cl);
			if (findInvite == channelCheck->getInvites().end())
			{
				this->sendErrToClient(cl, ERR_INVITEONLYCHAN(toSet[1]));
				return ;
			}
		}
		if (channelCheck->getMode().userLimit > 0 && static_cast<size_t>(channelCheck->getMode().userLimit) <= channelCheck->getClients().size())
			return (this->sendErrToClient(cl, ERR_CHANNELISFULL(toSet[1])));
		if (!channelCheck->getMode().password.empty() && channelCheck->getMode().password != toSet[2])
			return (this->sendErrToClient(cl, ERR_BADCHANNELKEY(toSet[1])));
		channelCheck->getClients().push_back(&cl);
		cl.getBelongChannel().push_back(channelCheck);
		if (channelCheck->getMode().invitation)
			channelCheck->getInvites().erase(findInvite);
		this->_refreshList(channelCheck, cl);
	}
	else
		this->_createChannel(cl, toSet[1]);
	return ;
}

void	Server::_refreshList(Channel *channel, Client & cl)
{
	std::vector<Client *> clients = channel->getClients();
	std::string	toSend;
	channel->sendMessageToClient(getMask(cl) + "JOIN :" + channel->getChannelName() + "\r\n");
	send(cl.getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
	toSend = ":" + this->_hostname + " 331 " + cl.getNickName() + " TOPIC " + channel->getChannelName() + " :No topic set\r\n";
	send(cl.getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
	
	toSend = ":" + this->_hostname + " 353 " + cl.getNickName() + " = " + channel->getChannelName() + " " + channel->getMaskList2() + "\r\n";
	send(cl.getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
	toSend = ":" + this->_hostname + " 366 " + cl.getNickName() + " " + channel->getChannelName() + " :End of /NAMES list.\r\n";
	send(cl.getIdentifier(), toSend.c_str(), toSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
}

int	Server::_createChannel(Client & cl, std::string name)
{
	std::string message;
	
	if (!this->_checkChannelName(name))
	{
		this->sendErrToClient(cl, ERR_BADCHANMASK(name));
		return 0;
	}
	Channel *newChannel = new Channel(name, &cl);
	newChannel->getMode().userLimit = -1;
	newChannel->getMode().password = "";
	newChannel->getMode().changeTopic = true;
	newChannel->getMode().invitation = false;
	cl.getBelongChannel().push_back(newChannel);
	this->_channels.push_back(newChannel);
	message = ":" + this->_hostname + " MODE " + name + " +tn\r\n";
	this->_sendMessageToClient(message, &cl);
	this->_refreshList(newChannel, cl);
	return (1);
}
