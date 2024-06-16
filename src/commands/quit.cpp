/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 22:14:02 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/16 09:13:30 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::_removeClient(unsigned int &index, std::string message)
{
	this->_sendMessageToClient(message, this->_clients[index - 1]);
	std::cerr << "Error: recv() failed or client disconnected" << std::endl;
	close(this->_fds[index].fd);
	this->_fds.erase(this->_fds.begin() + index);
	Client *tmp = this->_clients[index - 1];
	this->_clients.erase(this->_clients.begin() + (index - 1));
	if (index > 0)
		index--;

	std::vector<Channel *>::iterator it = tmp->getBelongChannel().begin();
	std::vector<Client *>::iterator itFindClient;
	for (; it != tmp->getBelongChannel().end(); ++it)
	{
		itFindClient = find((*it)->getClients().begin(), (*it)->getClients().end(), tmp);
		if (itFindClient != (*it)->getClients().end())
			(*it)->getClients().erase(itFindClient);
		itFindClient = find((*it)->getOperators().begin(), (*it)->getOperators().end(), tmp);
		if (itFindClient != (*it)->getOperators().end())
			(*it)->getOperators().erase(itFindClient);
	}

	
	std::vector<Channel *>::iterator itChan;
	std::vector<Channel *>::iterator itFindChan;
	for (itChan = tmp->getBelongChannel().begin() ; itChan != tmp->getBelongChannel().end(); ++itChan)
	{
		itFindChan = find(this->_channels.begin(), this->_channels.end(), (*itChan));
		if (itFindChan != this->_channels.end() && (*itFindChan)->getClients().empty())
		{
			delete (*itFindChan);
			this->_channels.erase(itFindChan);
		}
	}
	delete tmp;
}
