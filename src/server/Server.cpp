/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:40:03 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/17 22:14:33 by bcarolle         ###   ########.fr       */
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
