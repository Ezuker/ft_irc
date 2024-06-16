/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:40:03 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/16 05:21:27 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "global.hpp"

Server::Server(unsigned short port, std::string password, int fdSocketServ) : _port(port), _password(password), _fdSocketServ(fdSocketServ), _hostname("teamspeak-without-speak.org")
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
