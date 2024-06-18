/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 16:15:20 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/18 15:06:57 by ehalliez         ###   ########.fr       */
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
	if (!this->checkCommand("KICK", message, cl))
		return ;
	std::vector<std::string> tokens = split(message, ' ');
	if (tokens.size() < 3)
		return (this->sendErrToClient(cl, ERR_NEEDMOREPARAMS(cl.getNickName(), "KICK")));
	std::string channelName = tokens[1];
	std::string userToKick = tokens[2];
	if (!this->_checkChannelName(channelName))
		return (this->sendErrToClient(cl, ERR_BADCHANMASK(cl.getNickName())));
	Channel *channel = this->_channelExists(channelName);
	if (!channel)
		return (this->sendErrToClient(cl, ERR_NOSUCHCHANNEL(channelName)));
	if (!cl._isInChannel(*channel))
		return (this->sendErrToClient(cl, ERR_NOTONCHANNEL(channel->getChannelName())));
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
			if (tokens.size() >= 4)
				messageToSend = getMask(cl) + message + "\r\n";
			else
				messageToSend = getMask(cl) + message + " :" + cl.getNickName() + "\r\n";
			std::vector<Client *>::iterator itSender = clientList.begin();
			for (; itSender != clientList.end(); ++itSender)
				send((*itSender)->getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
			clientList.erase(tmp);
		}
	}
	else
		this->sendErrToClient(cl, ERR_CHANOPRIVSNEEDED(cl.getUserName(), channel->getChannelName()));
}
