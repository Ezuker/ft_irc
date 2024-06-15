/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manageClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 03:54:31 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/15 23:33:47 by bcarolle         ###   ########.fr       */
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
