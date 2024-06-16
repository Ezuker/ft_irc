/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 16:18:56 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/16 09:52:00 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::_inviteCase(Client & cl, std::string const & message)
{
	if (!this->checkCommand("INVITE", message, cl))
		return ;
	std::vector<std::string> splitted = split(message, ' ');
	std::vector<Client *>::iterator findClient;
	std::vector<Channel *>::iterator findChannel;
	
	std::vector<Client *>::iterator it = this->_clients.begin();
	for (; it != this->_clients.end(); ++it)
	{
		if ((*it)->getNickName() == splitted[1])
			break ;
	}
	if (it == this->_clients.end())
		return (this->_sendMessageToClient(":" + this->_hostname + " " + ERR_NOSUCHCHANNEL(cl.getNickName(), splitted[2]), &cl));
	Channel *toSend = this->_channelExists(splitted[2]);
	if (!toSend)
		return (this->_sendMessageToClient(":" + this->_hostname + " " + ERR_NOSUCHCHANNEL(cl.getNickName(), splitted[2]), &cl));
	if (!cl._isInChannel(*toSend))
		return (this->_sendMessageToClient(":" + this->_hostname + " " + ERR_NOTONCHANNEL(cl.getNickName(), toSend->getChannelName()), &cl));
	std::string messageToSend = ":" + this->_hostname + " 341 " + (*it)->getNickName() + " " + cl.getNickName() + " " + splitted[2] + "\r\n";
	send(cl.getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
	messageToSend = getMask(cl) + message + "\r\n";
	send((*it)->getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
	toSend->getInvites().push_back((*it));
}
 