/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 07:44:42 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/17 22:17:51 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string Channel::getLastTopicChangeTime() const
{
	std::ostringstream oss;
	
	oss << this->_lastTopicChange;
	std::string myString2 = oss.str();
	return (myString2);
}

void	Server::_changeTopic(Client & cl, std::string message)
{
	if (!this->checkCommand("TOPIC", message, cl))
		return ;
	std::vector<std::string> tokens = split(message, ' ');
	Channel *channel;

	channel = this->_channelExists(tokens[1]);
	if (!channel)
		return (this->sendErrToClient(cl, ERR_NOSUCHCHANNEL(tokens[1])));
	if (!cl._isInChannel(*channel))
		return (this->sendErrToClient(cl, ERR_NOTONCHANNEL(channel->getChannelName())));
	std::vector<Client *> operatorList = channel->getOperators();
	std::vector<Client *>::iterator toFind;

	toFind = find(operatorList.begin(), operatorList.end(), &cl);
	if (channel->getMode().changeTopic == false || toFind != operatorList.end())
	{
		std::vector<Client *> clientList = channel->getClients();
		std::vector<Client *>::iterator it = clientList.begin();
		if (tokens.size() == 2)
		{
			if (channel && channel->getTopic().empty())
				return (this->sendErrToClient(cl, ERR_RPL_NOTOPIC(cl.getNickName(), channel->getChannelName())));
			if (channel)
			{
				channel->setIsTopic(!channel->getIsTopic());
				if (channel->getLastChange().size())
				{
					std::string messageToSend = ":" + this->_hostname + " 332 " +  cl.getNickName() + " " + channel->getChannelName() + " " + channel->getTopic() + "\r\n";
					messageToSend += ":" + this->_hostname + " 333 " +  cl.getNickName() + " " + channel->getChannelName() + " " + channel->getLastChange() + channel->getLastTopicChangeTime() + "\r\n";
					channel->setIsTopic(true);
					send(cl.getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);	
				}
			}
			return ;
		}
		else
		{
			std::string messageToSend = getMask(cl) + message + "\r\n";
			channel->setIsTopic(true);
			if (tokens.size() > 2)
				channel->setTopic(tokens[2]);
			for (; it != clientList.end(); ++it)
				send((*it)->getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
			channel->setLastChange(getMask(cl));
		}
	}
	else
		this->sendErrToClient(cl, ERR_CHANOPRIVSNEEDED(cl.getUserName(), channel->getChannelName()));
	
}
