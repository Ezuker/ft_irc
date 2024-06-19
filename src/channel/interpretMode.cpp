/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpretMode.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 18:37:19 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/18 19:13:59 by ehalliez         ###   ########.fr       */
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
	std::cout << action << std::endl;
	if (splitted.size() < 4)
	{
		this->sendErrToClient(cl, ERR_NEEDMOREPARAMS(cl.getNickName(), "MODE"));
		return;
	}
	std::vector<Client *>::iterator itOp = channel->getClients().begin();
	std::cout << splitted[1] << std::endl;
	std::cout << splitted[2] << std::endl;
	std::cout << splitted[3] << std::endl;
	for (; itOp != channel->getClients().end(); ++itOp)
	{
		if ((*itOp)->getNickName() == splitted[3])
			break ;
	}
	if (itOp == channel->getClients().end())
		return ;
	if (action == '-')
	{
		if (channel->isOperator(**itOp))
		{
			std::vector<Client *>::iterator it;
	
			it = find(channel->getOperators().begin(), channel->getOperators().end(), *itOp);
			if (it != channel->getOperators().end())
				channel->getOperators().erase(it);
		}
		else
			return ;
	}
	else if (action == '+')
	{
		if (channel->isOperator(**itOp))
			return ;
		channel->getOperators().push_back(*itOp);
	}
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

std::string Channel::getCreation() const
{
	std::ostringstream oss;
	
	oss << this->_CreationDate;
	std::string myString2 = oss.str();
	return (myString2);
}

void	Server::_interpretMode(Client &cl, std::string message)
{
	if (!this->checkCommand("MODE", message, cl))
		return ;
	std::vector<std::string> splitted = split(message, ' ');
	std::string	mode;
	Channel *channel;
	char action;
	channel = this->_channelExists(splitted[1]);
	if (!channel)
		return (this->sendErrToClient(cl, ERR_NOSUCHCHANNEL(splitted[1])));
	if (!cl._isInChannel(*channel))
		return (this->sendErrToClient(cl, ERR_NOTONCHANNEL(channel->getChannelName())));
	if (!channel->isOperator(cl))
		this->sendErrToClient(cl, ERR_CHANOPRIVSNEEDED(cl.getUserName(), channel->getChannelName()));
	if (splitted.size() < 3)
	{
		this->_sendMessageToClient(":" + this->_hostname + " 329 " + cl.getNickName() + " " + splitted[1] + " "  + channel->getCreation() + "\r\n", &cl);
		return ;
	}
	mode = splitted[2];
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
			// INVITATION CASE
			case 'i':
				channel->getMode().invitation = action == '+';
				channel->toggleChannelMode(cl, "i", action);
				break;
			// TOPIC CASE
			case 't':
				channel->getMode().changeTopic = action == '+';
				channel->toggleChannelMode(cl, "t", action);
				break;
			// PASSWORD CASE
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
			// OPERATOR CASE
			case 'o':
				this->_manageOperator(cl, splitted, action, channel);
				break;
			// LIMIT CASE
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

