/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpretMode.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 18:37:19 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/17 22:29:15 by bcarolle         ###   ########.fr       */
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

void	Server::_manageOperator(Client &cl, std::vector<std::string> &splitted, char action, Channel *channel)
{
	if (splitted.size() < 4)
	{
		this->sendErrToClient(cl, ERR_NEEDMOREPARAMS(cl.getNickName(), "MODE"));
		return;
	}
	std::vector<Client *>::iterator it = channel->getClients().begin();
	if (action == '-')
	{
		std::vector<Client *>::iterator itOp = channel->getOperators().begin();
		for (; itOp != channel->getOperators().end(); ++itOp)
		{
			if ((*itOp)->getNickName() == splitted[3])
				break ;
		}
		if (itOp != channel->getOperators().end())
			channel->getOperators().erase(itOp);
	}
	else if (action == '+')
		channel->getOperators().push_back(*it);
	channel->toggleChannelMode(cl, "o " + splitted[3], action);
	splitted.erase(find(splitted.begin(), splitted.end(), splitted[3]));
}

void	Channel::sendMessageToClient(std::string const & messageToSend)
{
	std::vector<Client *>::iterator it = this->_clients.begin();

	for (; it != this->_clients.end(); ++it)
		send((*it)->getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
}

void	Channel::toggleChannelMode(Client &cl, std::string message, char action)
{
	if (action == '+')
		this->sendMessageToClient(getMask(cl) + "MODE " + this->getChannelName() + " +" + message + "\r\n");
	else
		this->sendMessageToClient(getMask(cl) + "MODE " + this->getChannelName() + " -" + message + "\r\n");
}

bool	Server::_checkMode(Client &cl, Channel *channel, std::vector<std::string> splitted)
{
	/* Check if channel exists */
	std::vector<Channel *>::iterator itChan = this->_channels.begin();
	for (; itChan != this->_channels.end(); ++itChan)
	{
		if ((*itChan)->getChannelName() == splitted[1])
			break;
	}
	if (itChan == this->_channels.end())
	{
		this->sendErrToClient(cl, ERR_NOSUCHCHANNEL(splitted[1]));
		return false;
	}

	/* Check if client is an operator */
	if (!channel->isOperator(cl))
	{
		this->sendErrToClient(cl, ERR_CHANOPRIVSNEEDED(cl.getUserName(), channel->getChannelName()));
		return false;
	}
	return true;
}

void	Server::_interpretMode(Client &cl, std::string message)
{
	if (!this->checkCommand("MODE", message, cl))
		return ;
	std::vector<std::string> splitted = split(message, ' ');
	std::string	mode;
	Channel *channel;
	char action;
	if (splitted.size() < 3)
		return ;
	mode = splitted[2];
	channel = this->_channelExists(splitted[1]);
	if (!channel)
		return (this->sendErrToClient(cl, ERR_NOSUCHCHANNEL(channel->getChannelName())));
	if (!cl._isInChannel(*channel))
		return (this->sendErrToClient(cl, ERR_NOTONCHANNEL(channel->getChannelName())));
	if (mode[0] == '-' || mode[0] == '+')
	{
		action = mode[0];
		mode = mode.substr(1);
	}
	else
		action = '+';
	for (size_t i = 0; i < mode.size(); i++)
	{
		if (!this->_checkMode(cl, channel, splitted))
		{
			if (splitted.size() > 3)
				splitted.erase(find(splitted.begin(), splitted.end(), splitted[3]));
			continue ;
		}
		switch (mode[i])
		{
			case 'i':
				channel->getMode().invitation = action == '+';
				channel->toggleChannelMode(cl, "i", action);
				break;
			case 't':
				channel->getMode().changeTopic = action == '+';
				channel->toggleChannelMode(cl, "t", action);
				break;
			case 'k':
				if (splitted.size() < 4 && action == '+')
				{
					this->sendErrToClient(cl, ERR_NEEDMOREPARAMS(cl.getNickName(), "MODE"));
					continue;
				}
				if (channel->getMode().password != "" && action == '+')
				{
					this->sendErrToClient(cl, ERR_KEYSET(channel->getChannelName()));
					continue;
				}
				channel->getMode().password = (action == '+') ? splitted[3] : "";
				if (action == '+')
					channel->sendMessageToClient(getMask(cl) + "MODE " + channel->getChannelName() + " +k " + splitted[3] + "\r\n");
				else
					channel->sendMessageToClient(getMask(cl) + "MODE " + channel->getChannelName() + " -k\r\n");
				splitted.erase(find(splitted.begin(), splitted.end(), splitted[3]));
				break;
			case 'o':
				this->_manageOperator(cl, splitted, action, channel);
				break;
			case 'l':
				if (splitted.size() < 4 && action == '+')
				{
					this->sendErrToClient(cl, ERR_NEEDMOREPARAMS(cl.getNickName(), "MODE"));
					continue;
				}
				channel->getMode().userLimit = (action == '+') ? atoi(splitted[3].c_str()) : -1;
				if (action == '+')
					channel->sendMessageToClient(getMask(cl) + "MODE " + channel->getChannelName() + " +l " + splitted[3] + "\r\n");
				else
					channel->sendMessageToClient(getMask(cl) + "MODE " + channel->getChannelName() + " -l\r\n");
				splitted.erase(find(splitted.begin(), splitted.end(), splitted[3]));
				break;
			default:
				this->sendErrToClient(cl, ERR_UMODEUNKNOWNFLAG);
		}
	}
}

