/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:34:20 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/15 01:57:33 by ehalliez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


// CHECK If the client is in the channel before send the message

void	Server::_privmsgCase(Client & client, std::string const & message)
{
	std::string channel = split(message, ' ')[1];

	if (channel.size() && channel[0] != '#')
	{
		std::cout << "envoyer une erreur" << std::endl;
		return ;
	}
	channel = channel.substr(1, channel.size() - 1);
	std::cout << channel << std::endl;
	Channel *toSend = this->_channelExists(channel);
	if (!toSend)
		return ;
	std::string messageToSend = ":";
	std::vector<Client *>::iterator it = toSend->getClients().begin();
	for (; it != toSend->getClients().end(); ++it)
	{
		if (*it == &client)
			continue ;
		messageToSend += client.getNickName() + "!" + client.getUserName() + "@" + client.getHostName() + " " + message  + "\n";
		send((*it)->getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
		messageToSend = ":";
	}
}
