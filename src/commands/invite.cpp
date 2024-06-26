/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 16:18:56 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/19 17:55:38 by ehalliez         ###   ########.fr       */
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
		return (this->sendErrToClient(cl, ERR_NOSUCHCHANNEL(splitted[2])));
	Channel *toSend = this->_channelExists(splitted[2]);
	if (!toSend)
		return (this->sendErrToClient(cl, ERR_NOSUCHCHANNEL(splitted[2])));
	if (!cl._isInChannel(*toSend))
		return (this->sendErrToClient(cl, ERR_NOTONCHANNEL(toSend->getChannelName())));
	if ((*it)->_isInChannel(*toSend))
		return (this->sendErrToClient(cl, ERR_INVITECL(cl.getNickName(), (*it)->getNickName(), (*toSend).getChannelName())));
	if (toSend->getMode().userLimit > 0 && static_cast<int>(toSend->getClients().size()) + 1 > toSend->getMode().userLimit)
		return (this->sendErrToClient(cl, ERR_CHANNELISFULL((*toSend).getChannelName())));
	std::string messageToSend = ":" + this->_hostname + " 341 " + cl.getNickName() + " " + (*it)->getNickName() + " " + splitted[2] + "\r\n";
	send(cl.getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
	messageToSend = getMask(cl) + message + "\r\n";
	send((*it)->getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
	toSend->getInvites().push_back((*it));
}
