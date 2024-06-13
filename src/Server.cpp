/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:40:03 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/13 02:58:48 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "global.hpp"

Server::Server(unsigned short port, std::string password, int fdSocketServ) : _port(port), _password(password), _fdSocketServ(fdSocketServ)
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

pollfd	Server::_createPollfd(int sock, short events, short revents)
{
	pollfd	result;

	result.fd = sock;
	result.events = events;
	result.revents = revents;
	return (result);
}

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
    this->_fds.push_back(this->_createPollfd(client_sock, POLLIN, 0));
	Client *newClient = new Client(client_sock);
	newClient->setAccess(false);
    this->_clients.push_back(newClient);
    return (1);
}


void	Server::_removeClient(unsigned int &index)
{
	std::cerr << "Error: recv() failed or client disconnected" << std::endl;
	this->_fds.erase(this->_fds.begin() + index);
	close(this->_fds[index].fd);
	Client *tmp = this->_clients[index - 1];
	this->_clients.erase(this->_clients.begin() + (index - 1));
	if (index > 0)
		index--;
	std::cout << index << " after remove" << std::endl;
	delete tmp;
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
	std::cout << this->_fds[0].fd << std::endl;
	std::cout << this->_fds[0].events << std::endl;
	std::cout << this->_fds[0].revents << std::endl;
	return (1);
}

int	Server::_handleMessage(unsigned int &i)
{
	char buffer[MAX_BUFFER_SIZE];
	memset(buffer, 0, MAX_BUFFER_SIZE);
	int bytes_received = recv(this->_fds[i].fd, buffer, MAX_BUFFER_SIZE, 0);
	if (bytes_received <= 0)
	{
		this->_removeClient(i);
		std::cout << i << " after remove in the handle" << std::endl;
		return (0);
	}
	if (bytes_received > 0 && i > 0)
	{
		std::string message(buffer, bytes_received);
		if (this->_clients[i - 1]->getAccess())
		{
			std::cout << message.substr(0, message.size() - 1) << std::endl;
		}
		else 
		{
			if (!std::strncmp(message.c_str(), "PASS ", 5))
			{
				std::string p_try = message.substr(5, message.size() - 6);
				std::cout << "PW attempt : [" << p_try << "]" << std::endl;
				std::cout << "size ptry :" << p_try.size() << std::endl << "size password :" << this->_password.size() << std::endl;
				if (p_try == this->_password)
				{
					std::cout << "HAHAHAHAH" << std::endl;
					this->_clients[i - 1]->setAccess(true);
				}
			}
			else if (i > 0)
			{
				std::string errorMess = "Error (464): please enter de password 'PASS (PASSWORD)'\n";
				send(this->_clients[i - 1]->getIdentifier(), errorMess.c_str(), errorMess.size(), 0);
			}
		}
	}
	return (1);
}

int     Server::startServer()
{
	if (!this->_initServer())
		return (0);
	std::cout << "IRC server listening on port: " << this->_port << std::endl;
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
				if (this->_fds[i].fd == this->_fdSocketServ)
				{
					if (!this->_addClient())
						continue;
				}
				else if (!this->_handleMessage(i))
                        continue;
			}
			std::cout << i << " after remove in the loop" << std::endl;
			this->_fds[i].revents = 0;
		}
	}
	this->_fds.erase(this->_fds.begin());
	close(this->_fdSocketServ);
    return (1);
}
