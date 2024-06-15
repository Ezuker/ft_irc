/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 16:15:20 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/15 05:51:07 by bcarolle         ###   ########.fr       */
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
	{
		this->_sendMessageToClient(ERR_NEEDMOREPARAMS, &cl);
		return ;
	}
	std::string channelName = tokens[1];
	std::string userToKick = tokens[2];
	if (channelName.empty())
	{
		this->_sendMessageToClient(ERR_BADCHANMASK, &cl);
		return ;
	}
	Channel *channel = this->_channelExists(channelName.substr(1, channelName.size() - 1));
	if (!channel)
	{
		this->_sendMessageToClient(ERR_NOSUCHCHANNEL, &cl);
		return ;
	}
	if (!cl._isInChannel(*channel))
	{
		this->_sendMessageToClient(ERR_NOTONCHANNEL, &cl);
		return ;
	}
	std::vector<Client *> operatorList = channel->getOperators();
	std::vector<Client *>::iterator it;

	it = find(operatorList.begin(), operatorList.end(), &cl);
	if (it != operatorList.end())
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
			std::vector<Client *>::iterator tmp = it;
			std::string messageToSend = ":" + cl.getNickName() + "!" + cl.getUserName() + "@" + cl.getHostName() + " " + message + " :" + cl.getNickName() + "\n";
			std::vector<Client *>::iterator itmachin = clientList.begin();
			for (; itmachin != clientList.end(); ++itmachin)
				send((*itmachin)->getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
			clientList.erase(tmp);
		}
	}
}
