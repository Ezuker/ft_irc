/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:40:03 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/12 19:25:17 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "global.hpp"

Server::Server(unsigned short port, std::string password, int identifier) : _port(port), _password(password), _identifier(identifier)
{
	
}

Server::~Server()
{
	std::vector<Client *>::iterator it = this->_clients.begin();
	for (; it != this->_clients.end(); ++it)
	{
		if (*it)
			delete *it;
	}
}

void	Server::socketHandle()
{
	
}

pollfd	Server::_createPollfd(int sock, short events, short revents)
{
	pollfd	result;

	result.fd = sock;
	result.events = events;
	result.revents = revents;
	return (result);
}

int     Server::_addClient()
{
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_sock = accept(this->_identifier, (sockaddr *)&client_addr, &client_addr_len);
    if (client_sock < 0)
    {
        std::cerr << "Error client failed" << std::endl;
        return (0);
    }
    this->_fds.push_back(this->_createPollfd(client_sock, POLLIN, 0));
    Client *newClient = new Client(client_sock);
    newClient->setHostName("Salut");
    newClient->setUserName("Salut");
    newClient->setNickName("Salut");
    this->_clients.push_back(newClient);
    return (1);
}

int     Server::startServer()
{
	int disconnected = 0;
	if (this->_identifier < 0)
	{
		std::cerr << "Error: socket() failed" << std::endl;
		return 1;
	}

	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(this->_port);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(this->_identifier, (sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		std::cerr << "Error: bind() failed" << std::endl;
		close(this->_identifier);
		return 1;
	}

	if (listen(this->_identifier, 5) < 0)
	{
		std::cerr << "Error: listen() failed" << std::endl;
		close(this->_identifier);
		return 1;
	}

	std::cout << "IRC server listening on port: " << this->_port << std::endl;


	this->_fds.push_back(this->_createPollfd(this->_identifier, POLLIN, 0));
	std::cout << this->_fds[0].fd << std::endl;
	std::cout << this->_fds[0].events << std::endl;
	std::cout << this->_fds[0].revents << std::endl;
	while (true)
	{
		int poll_count = poll(this->_fds.data(), this->_fds.size(), -1);
		if (poll_count < 0)
		{
			std::cerr << "Poll failed" << std::endl;
			break;
		}
		for (unsigned int i = 0; i < this->_fds.size(); i++)
		{
			if (this->_fds[i].revents & POLLIN)
			{
				if (this->_fds[i].fd == this->_identifier)
                {
                    if (!this->_addClient())
                        continue;
                }
				else
				{	
					char buffer[MAX_BUFFER_SIZE];
					memset(buffer, 0, MAX_BUFFER_SIZE);
					int bytes_received = recv(this->_fds[i].fd, buffer, MAX_BUFFER_SIZE, 0);
					if (bytes_received <= 0)
					{
						std::cerr << "Error: recv() failed or client disconnected" << std::endl;
						this->_fds.erase(this->_fds.begin() + i); //std::find(server._fds.begin(), server._fds.end(), i);
						close(this->_fds[i].fd);
						std::cout << "id = " << i << std::endl;
						Client *tmp = this->_clients[i - 1];
						this->_clients.erase(this->_clients.begin() + (i - 1));
						delete tmp;
						disconnected++;
						continue;
					}
					
					if (bytes_received > 0 && i > 0)
					{
						std::string message(buffer, bytes_received);
						std::cout << "Received message from " << ": " << message << std::endl;
						std::cout << "Receivec from " << this->_clients[i - 1]->getIdentifier() << std::endl; 
					}
				}
			}
			this->_fds[i].revents = 0;
		}
		if (disconnected > 4)
			break;
	}
	this->_fds.erase(this->_fds.begin());
	close(this->_identifier);
    return (1);
}
