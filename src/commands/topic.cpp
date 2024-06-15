/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 07:44:42 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/15 22:30:12 by ehalliez         ###   ########.fr       */
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
	std::vector<std::string> tokens = split(message, ' ');
	Channel *channel;

	channel = this->_channelExists(tokens[1]);
	if (!channel)
		return (this->_sendMessageToClient(":" + cl.getHostName() + " " + ERR_NOSUCHCHANNEL(cl.getNickName(), tokens[1]), &cl));
	if (!cl._isInChannel(*channel))
		return (this->_sendMessageToClient(":" + cl.getHostName() + " " + ERR_NOTONCHANNEL(cl.getNickName(), channel->getChannelName()), &cl));
	std::vector<Client *> operatorList = channel->getOperators();
	std::vector<Client *>::iterator toFind;

	toFind = find(operatorList.begin(), operatorList.end(), &cl);
	if (toFind != operatorList.end())
	{
		std::vector<Client *> clientList = channel->getClients();
		std::vector<Client *>::iterator it = clientList.begin();
		if (tokens.size() == 2)
		{
			if (channel && channel->getTopic().empty())
				return (this->_sendMessageToClient(":" + cl.getHostName() + " " + ERR_RPL_NOTOPIC(cl.getNickName(), channel->getChannelName()), &cl));
			if (channel)
			{
				channel->setIsTopic(!channel->getIsTopic());
				if (channel->getLastChange().size())
				{
					std::string messageToSend = ":" + cl.getHostName() + " 332 " +  cl.getNickName() + " " + channel->getChannelName() + " " + channel->getTopic() + "\n";
					messageToSend += ":" + cl.getHostName() + " 333 " +  cl.getNickName() + " " + channel->getChannelName() + " " + channel->getLastChange() + " " + channel->getLastTopicChangeTime() + "\n";
					channel->setIsTopic(true);
					for (; it != clientList.end(); ++it)
						send((*it)->getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);	
				}
			}
			return ;
		}
		else
		{
			std::string messageToSend = ":" + cl.getNickName() + "!" +  cl.getUserName() + "@" + cl.getHostName() + " " + message + "\n";
			channel->setIsTopic(true);
			if (tokens.size() > 2)
				channel->setTopic(tokens[2]);
			for (; it != clientList.end(); ++it)
				send((*it)->getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
			channel->setLastChange(cl.getNickName() + "!" +  cl.getUserName() + "@" + cl.getHostName());
		}
	}
	else
		this->_sendMessageToClient(":" + cl.getHostName() + " " + ERR_CHANOPRIVSNEEDED(cl.getNickName(), channel->getChannelName()), &cl);
	
}
