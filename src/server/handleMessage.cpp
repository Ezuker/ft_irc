/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleMessage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 03:56:02 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/15 05:37:33 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	Server::_getCommand(std::string str, Client *cl)
{
	if (!std::strncmp(str.c_str(), "NICK", 4))
		return (1);
	if (cl->getNickName().empty())
		return (7);
	if (!std::strncmp(str.c_str(), "USER ", 5))
		return (2);
	if (cl->getUserName().empty())
		return (7);
	if (!std::strncmp(str.c_str(), "JOIN ", 5))
		return (3);	
	if (!std::strncmp(str.c_str(), "PASS", 4))
		return (4);
	if (!std::strncmp(str.c_str(), "PRIVMSG ", 8))
		return (5);
	if (!std::strncmp(str.c_str(), "KICK ", 5))
		return (6);
	return (0);
}

void	Server::_sendMessageToClient(const std::string & message, Client *client)
{
	send(client->getIdentifier(), message.c_str(), message.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
}

void	Server::_checkMessage(std::string message, int i)
{
	std::cout << "message sent by client (hexchat): " << message << std::endl;
	switch (this->_getCommand(message, this->_clients[i - 1]))
	{
		case 1: // NICKNAME
		{
			this->_nicknameCase(*this->_clients[i - 1], message);
			break;
		}
		case 2: // USERNAME
		{
			this->_usernameCase(*this->_clients[i - 1], message);
			break ;
		}
		case 3: // JOIN	
		{
			this->joinChannel(*this->_clients[i - 1], message, i);
			break ;
		}
		case 4: //PASS
		{
			this->_passCase(*this->_clients[i - 1], message);
			break ;
		}
		case 5: //PRIVMSG
		{
			this->_privmsgCase(*this->_clients[i - 1], message);
			break ;
		}
		case 6: //KICK
		{
			this->_kickCase(*this->_clients[i - 1], message);
			break ;
		}
		default:
		{
			if (this->_clients[i - 1]->getNickName().empty())
			{
				this->_sendMessageToClient("451 : Please register a NICKNAME (NICK nickname) AND a USERNAME (USER username)\n", this->_clients[i - 1]);
				break ;
			}
			else if (this->_clients[i - 1]->getUserName().empty())
			{
				this->_sendMessageToClient("451 : Please register a USERNAME (USER username)\n", this->_clients[i - 1]);
				break ;
			}
			else
				this->_sendMessageToClient("Unknown command\n", this->_clients[i - 1]);
		}
	}
}

int	Server::_handleMessage(unsigned int &i)
{
	char buffer[MAX_BUFFER_SIZE];
	memset(buffer, 0, MAX_BUFFER_SIZE);
	int bytes_received = recv(this->_fds[i].fd, buffer, MAX_BUFFER_SIZE, 0);
	if (bytes_received <= 0)
	{
		this->_removeClient(i);
		return (0);
	}
	if (bytes_received > 0 && i > 0)
	{
		std::string message(buffer, bytes_received);
		std::vector<std::string> messages = split(message, '\n');
		for (size_t messageIndex = 0; messageIndex < messages.size(); messageIndex++)
		{
			messages[messageIndex] = strtrim(messages[messageIndex]);
			if (this->_clients[i - 1]->getAccess())
				this->_checkMessage(messages[messageIndex], i);
			else 
				this->_checkPassword(*this->_clients[i - 1], messages[messageIndex]);
		}
	}
	return (1);
}
