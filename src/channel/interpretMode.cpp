/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpretMode.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 18:37:19 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/16 09:33:53 by bcarolle         ###   ########.fr       */
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

void	Server::_manageOperator(Client &cl, std::vector<std::string> splitted, char action, Channel *channel)
{
	std::vector<Client *>::iterator it = channel->getClients().begin();
	for (; it != channel->getClients().end(); ++it)
	{
		if ((*it)->getNickName() == splitted[3])
			break ;
	}
	if (it == channel->getClients().end())
	{
		std::string messageToSend = ":" + this->_hostname + " " + ERR_NOSUCHCHANNEL(cl.getNickName(), channel->getChannelName());
		send(cl.getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
		return ;
	}
	if (!channel->isOperator(cl))
		return ;
	// insuffi perm
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
}

void	Channel::sendMessageToClient(std::string const & messageToSend)
{
	std::vector<Client *>::iterator it = this->_clients.begin();

	for (; it != this->_clients.end(); ++it)
		send((*it)->getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
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
		{
			channel->getMode().invitation = (mode[i] == 'i') ? (action == '+') : false;
			if (action == '+')
				channel->sendMessageToClient(getMask(cl) + "MODE " + channel->getChannelName() + " +i" + "\r\n");
			else
				channel->sendMessageToClient(getMask(cl) + "MODE " + channel->getChannelName() + " -i" + "\r\n");
		}
		if (mode[i] == 't')
		{
			channel->getMode().changeTopic = (mode[i] == 't') ? (action == '+') : false;
			if (action == '+')
				channel->sendMessageToClient(getMask(cl) + "MODE " + channel->getChannelName() + " +t" + "\r\n");
			else
				channel->sendMessageToClient(getMask(cl) + "MODE " + channel->getChannelName() + " -t" + "\r\n");
		}
		if (mode[i] == 'k')
		{
			if (action == '+')
			{
				if (splitted.size() < 4)
				{
					this->_sendMessageToClient(":" + this->_hostname + " " + ERR_NEEDMOREPARAMS(cl.getNickName(), "MODE"), &cl);
					continue;
				}
				channel->sendMessageToClient(getMask(cl) + "MODE " + channel->getChannelName() + " +k " + splitted[3] + "\r\n");
				channel->getMode().password = splitted[3];
			}
			else
			{
				channel->sendMessageToClient(getMask(cl) + "MODE " + channel->getChannelName() + " -k " + splitted[3] + "\r\n");
				channel->getMode().password = "";
			}
			splitted.erase(find(splitted.begin(), splitted.end(), splitted[3]));
		}
		if (mode[i] == 'o')
		{
			if (splitted.size() < 4)
			{
				this->_sendMessageToClient(":" + this->_hostname + " " + ERR_NEEDMOREPARAMS(cl.getNickName(), "MODE"), &cl);
				continue;
			}
			this->_manageOperator(cl, splitted, action, channel);
			if (action == '+')
				channel->sendMessageToClient(getMask(cl) + "MODE " + channel->getChannelName() + " +o " + splitted[3] + "\r\n");
			else
				channel->sendMessageToClient(getMask(cl) + "MODE " + channel->getChannelName() + " -o " + splitted[3] + "\r\n");
			splitted.erase(find(splitted.begin(), splitted.end(), splitted[3]));
		}
		if (mode[i] == 'l')
		{
			if (action == '+')
			{
				if (splitted.size() < 4)
				{
					this->_sendMessageToClient(":" + this->_hostname + " " + ERR_NEEDMOREPARAMS(cl.getNickName(), "MODE"), &cl);
					continue;
				}
				channel->getMode().userLimit = atoi(splitted[3].c_str());
				channel->sendMessageToClient(getMask(cl) + "MODE " + channel->getChannelName() + " +l " + splitted[3] + "\r\n");
			}
			else
			{
				channel->getMode().userLimit = -1;
				channel->sendMessageToClient(getMask(cl) + "MODE " + channel->getChannelName() + " -l " + splitted[3] + "\r\n");
			}
			splitted.erase(find(splitted.begin(), splitted.end(), splitted[3]));
		}
	}
}
