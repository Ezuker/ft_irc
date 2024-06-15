/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 22:31:33 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/15 22:40:29 by ehalliez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::_partCase(Client & cl, std::string const & message)
{
	std::vector<std::string> tokens = split(message, ' ');
	Channel *channel;

	if (tokens.size() < 2)
		return ;
	channel = this->_channelExists(tokens[1]);
	std::vector<Client *> clients = channel->getClients();
	std::vector<Client *>::iterator it = clients.begin();
	std::string messageToSend = ":" + cl.getNickName() + "!" + cl.getUserName() + "@" + cl.getHostName() + " " + message + "\n";
	for (; it != clients.end(); ++it)
	{
		send((*it)->getIdentifier(), messageToSend.c_str(), messageToSend.size(), MSG_NOSIGNAL | MSG_DONTWAIT);	
	}
	// :ehalliez__!ehalliezUse@Pony-q988mu.20.unyc.it PART #t :"Leaving"
	
}
