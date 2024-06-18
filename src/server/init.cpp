/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 03:55:30 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/17 22:14:02 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "global.hpp"

pollfd	Server::_createPollfd(int sock, short events, short revents)
{
	pollfd	result;

	result.fd = sock;
	result.events = events;
	result.revents = revents;
	return (result);
}


int	Server::_initServer()
{
	if (this->_fdSocketServ < 0)
	{
		std::cerr << "Error: socket() failed" << std::endl;
		return (0);
	}
	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(this->_port);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(this->_fdSocketServ, (sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		std::cerr << "Error: bind() failed" << std::endl;
		close(this->_fdSocketServ);
		return (0);
	}
	if (listen(this->_fdSocketServ, 5) < 0)
	{
		std::cerr << "Error: listen() failed" << std::endl;
		close(this->_fdSocketServ);
		return (0);
	}
	this->_fds.push_back(this->_createPollfd(this->_fdSocketServ, POLLIN, 0));
	return (1);
}
