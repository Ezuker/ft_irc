/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manageClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 03:54:31 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/13 07:46:02 by ehalliez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "global.hpp"

int	Server::_addClient()
{
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    
    int client_sock = accept(this->_fdSocketServ, (sockaddr *)&client_addr, &client_addr_len);
    if (client_sock < 0)
    {
        std::cerr << "Error client failed" << std::endl;
        return (0);
    }
    char ipstr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, ipstr, sizeof(ipstr));
    this->_fds.push_back(this->_createPollfd(client_sock, POLLIN, 0));
	Client *newClient = new Client(client_sock);
	newClient->setAccess(false);
    newClient->setHostName(ipstr);
    this->_clients.push_back(newClient);
    return (1);
}

void	Server::_removeClient(unsigned int &index)
{
	std::cerr << "Error: recv() failed or client disconnected" << std::endl;
	close(this->_fds[index].fd);
	this->_fds.erase(this->_fds.begin() + index);
	Client *tmp = this->_clients[index - 1];
	this->_clients.erase(this->_clients.begin() + (index - 1));
	if (index > 0)
		index--;
	delete tmp;
}
