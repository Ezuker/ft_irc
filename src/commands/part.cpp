/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 22:31:33 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/17 13:46:50 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::_partCase(Client & cl, std::string const & message)
{
	if (!this->checkCommand("PART", message, cl))
		return ;
	std::vector<std::string> tokens = split(message, ' ');
	Channel *channel;

	if (tokens.size() < 2)
		return ;
	channel = this->_channelExists(tokens[1]);
	if (!channel)
		return (this->sendErrToClient(cl, ERR_NOSUCHCHANNEL(tokens[1])));
	if (!cl._isInChannel(*channel))
		return (this->sendErrToClient(cl, ERR_NOTONCHANNEL(channel->getChannelName())));
	std::vector<Client *> clients = channel->getClients();
	std::vector<Client *>::iterator it = clients.begin();
	std::string messageToSend = getMask(cl) + message + "\r\n";
	for (; it != clients.end(); ++it)
	{
		send((*it)->getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);	
	}
	std::vector<Client *>::iterator findClient;
	std::vector<Client *>::iterator findOperator;
	std::vector<Channel *>::iterator findChannel;
	
	findClient = find(channel->getClients().begin(), channel->getClients().end(), &cl);
	if (findClient != channel->getClients().end())
		channel->getClients().erase(findClient);
	findOperator = find(channel->getOperators().begin(), channel->getOperators().end(), &cl);
	if (findOperator != channel->getOperators().end())
		channel->getOperators().erase(findOperator);
	findChannel = find(cl.getBelongChannel().begin(), cl.getBelongChannel().end(), channel);
	if (findChannel != cl.getBelongChannel().end())
		cl.getBelongChannel().erase(findChannel);
	if (channel->getClients().empty())
	{
		findChannel = find(this->_channels.begin(), this->_channels.end(), channel);
		if (findChannel != cl.getBelongChannel().end())
		{
			delete *findChannel;
			this->_channels.erase(findChannel);
		}
	}
		
}
