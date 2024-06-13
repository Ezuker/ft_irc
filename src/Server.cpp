/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:40:03 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/13 04:00:36 by ehalliez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "global.hpp"

Server::Server(unsigned short port, std::string password, int fdSocketServ) : _port(port), _password(password), _fdSocketServ(fdSocketServ)
{}

Server::~Server()
{
	std::vector<Client *>::iterator it = this->_clients.begin();
	for (; it != this->_clients.end(); ++it)
	{
		if (*it)
			delete *it;
	}
}
