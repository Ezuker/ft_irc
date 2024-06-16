/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpretMode.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 18:37:19 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/16 05:36:35 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	Channel::isOperator(Client &cl)
{
	std::vector<Client *>::iterator it;
	
	it = find(this->_operators.begin(), this->_operators.end(), &cl);
	if (it != this->_operators.end())
		return (1);
	return (0);
}

void	Server::_interpretMode(Client &cl, std::string message)
{
	std::vector<std::string> splitted = split(message, ' ');
	std::string	mode;
	Channel *channel;
	char action;
	if (splitted.size() < 3)
		return ;
	mode = splitted[2];
	channel = this->_channelExists(splitted[1]);
	if (!channel)
		return (this->_sendMessageToClient(":" + this->_hostname + " " + ERR_NOSUCHCHANNEL(cl.getNickName(), channel->getChannelName()), &cl));
	if (!cl._isInChannel(*channel))
		return (this->_sendMessageToClient(":" + this->_hostname + " " + ERR_NOTONCHANNEL(cl.getNickName(), channel->getChannelName()), &cl));
	if (mode[0] == '-' || mode[0] == '+')
	{
		action = mode[0];
		mode = mode.substr(1);
	}
	else
		action = '+';
	for (size_t i = 0; i < mode.size(); i++)
	{
		if (mode[i] == 'i')
			channel->getMode().invitation = (mode[i] == 'i') ? (action == '+') : false;
		if (mode[i] == 't')
			channel->getMode().changeTopic = (mode[i] == 't') ? (action == '+') : false;
		if (mode[i] == 'k' && splitted.size() == 3)
			channel->getMode().password = splitted[3];
		if (mode[i] == 'o' && splitted.size() == 3)
		{
			std::vector<Client *>::iterator it = channel->getClients().begin();
			for (; it != channel->getClients().end(); ++it)
			{
				if ((*it)->getNickName() == splitted[3])
					break ;
			}
			if (it == channel->getClients().end())
			{
				std::string messageToSend = ":" + this->_hostname + " " + ERR_NOSUCHCHANNEL(cl.getNickName(), splitted[2]);
				send(cl.getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
				return ;
			}
			if (channel->isOperator(**it) && action == '-')
				channel->getOperators().erase(it);
			else if (channel->isOperator(**it) && action == '+')
				channel->getOperators().push_back(*it);
			it = channel->getClients().begin();
			std::string messageToSend = getMask(cl) + message + "\r\n";
			for (; it != channel->getClients().end(); ++it)
				send((*it)->getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
		}
		if (mode[i] == 'l')
			channel->getMode().userLimit = atoi(splitted[3].c_str());
	}
}
