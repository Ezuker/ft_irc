/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleMessage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 03:56:02 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/14 16:20:31 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

int	Server::_getCommand(std::string str)
{
	if (!std::strncmp(str.c_str(), "NICK ", 5))
		return (1);		
	if (!std::strncmp(str.c_str(), "USER ", 5))
		return (2);
	if (!std::strncmp(str.c_str(), "JOIN ", 5))
		return (3);	
	if (!std::strncmp(str.c_str(), "PASS ", 5))
		return (4);
	return (0);
}

void	Server::_sendMessageToClient(std::string message, int i)
{
	send(this->_clients[i - 1]->getIdentifier(), message.c_str(), message.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
}

void	Server::_checkMessage(std::string message, int i)
{
	std::string toSet;
	std::string messageToSend;
	switch (this->_getCommand(message))
	{
		case 1: // NICKNAME
		{
			nicknameCase(*this->_clients[i - 1], message);
			break;
		}
		case 2: // USERNAME
		{
			usernameCase(*this->_clients[i - 1], message);
			break ;
		}
		case 3: // JOIN	
		{
			this->joinChannel(*this->_clients[i - 1], message, i);
			break ;
		}
		default:
		{
			if (this->_clients[i - 1]->getNickName().size() == 0)
			{
				this->_sendMessageToClient("451 : Please register a NICKNAME (NICK nickname) AND a USERNAME (USER username)\n", i);
				break;
			}
			if (this->_clients[i - 1]->getUserName().size() == 0)
				this->_sendMessageToClient("451 : Please register a USERNAME (USER username)\n", i);
		}
	}
}

void	Server::_checkPassword(std::string message, int i)
{
	if (!std::strncmp(message.c_str(), "PASS ", 5))
	{
		std::string p_try = message.substr(5, message.size() - 6);
		if (p_try == this->_password)
			this->_clients[i - 1]->setAccess(true);
		else
		{
			p_try = message.substr(5, message.size() - 7);
			if (p_try == this->_password)
				this->_clients[i - 1]->setAccess(true);
			else
				this->_sendMessageToClient(ERROR_PW_MISMATCH, i);
		}
	}
	else
		this->_sendMessageToClient(ERROR_PW, i);
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
				this->_checkPassword(messages[messageIndex], i);
		}
	}
	return (1);
}
