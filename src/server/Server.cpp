/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:40:03 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/19 16:17:26 by ehalliez         ###   ########.fr       */
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
	this->_clients.clear();
	std::vector<Channel *>::iterator itchannel = this->_channels.begin();
	for (; itchannel != this->_channels.end(); ++itchannel)
	{
		if (*itchannel)
			delete *itchannel;			
	}
	this->_channels.clear();
}
