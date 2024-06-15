/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 16:15:20 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/15 07:38:52 by ehalliez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool	Server::_checkChannelName(std::string const & name)
{
	if (name.size() < 2 || name.size() > 200)
		return (false);
	for (unsigned int i = 0; i < name.size(); i++)
		if (std::isspace(name[i]) || !std::isprint(name[i]) || name[i] == ':' || name[i] == ',')
			return (false);
	return (true);
}

int	Client::_isInChannel(Channel const & channel)
{
	std::vector<Channel *>::iterator it = this->_belongChannel.begin();
	for (; it != this->_belongChannel.end(); ++it)
	{
		if (*it == &channel)
			return (1);	
	}
	return (0);
}

void	Server::_kickCase(Client & cl, std::string const & message)
{
	std::vector<std::string> tokens = split(message, ' ');
	if (tokens.size() < 3)
		return (this->_sendMessageToClient(":" + cl.getHostName() + " " + ERR_NEEDMOREPARAMS(cl.getNickName(), "KICK"), &cl));
	std::string channelName = tokens[1];
	std::string userToKick = tokens[2];
	if (!this->_checkChannelName(channelName))
		return (this->_sendMessageToClient(":" + cl.getHostName() + " " + ERR_BADCHANMASK(cl.getNickName()), &cl));
	Channel *channel = this->_channelExists(channelName);
	if (!channel)
		return (this->_sendMessageToClient(":" + cl.getHostName() + " " + ERR_NOSUCHCHANNEL(cl.getNickName(), channelName), &cl));
	if (!cl._isInChannel(*channel))
		return (this->_sendMessageToClient(":" + cl.getHostName() + " " + ERR_NOTONCHANNEL(cl.getNickName(), channel->getChannelName()), &cl));
	std::vector<Client *> operatorList = channel->getOperators();
	std::vector<Client *>::iterator toFind;

	toFind = find(operatorList.begin(), operatorList.end(), &cl);
	if (toFind != operatorList.end())
	{
		std::vector<Client *> clientList = channel->getClients();
		std::vector<Client *>::iterator it = clientList.begin();
		for (; it != clientList.end(); ++it)
		{
			if ((*it)->getNickName() == userToKick)
				break ;
		}
		if (it != clientList.end())
		{
			std::string messageToSend;
			std::vector<Client *>::iterator tmp = it;
			std::cout << tokens.size() << std::endl;
			std::cout << message << std::endl;
			if (tokens.size() >= 4)
				messageToSend = ":" + cl.getNickName() + "!" + cl.getUserName() + "@" + cl.getHostName() + " " + message + "\n";
			else
				messageToSend = ":" + cl.getNickName() + "!" + cl.getUserName() + "@" + cl.getHostName() + " " + message + " :" + cl.getNickName() + "\n";
			std::vector<Client *>::iterator itSender = clientList.begin();
			for (; itSender != clientList.end(); ++itSender)
				send((*itSender)->getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
			clientList.erase(tmp);
		}
	}
	else
		this->_sendMessageToClient(":" + cl.getHostName() + " " + ERR_CHANOPRIVSNEEDED(cl.getNickName(), channel->getChannelName()), &cl);
}
