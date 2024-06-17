/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleMessage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 03:56:02 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/17 11:19:13 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	Server::_getCommand(std::string str, Client *cl)
{
	if (!std::strncmp(str.c_str(), "NICK", 4))
		return (1);
	if (cl->getNickName().empty())
		return (0);
	if (!std::strncmp(str.c_str(), "USER", 4))
		return (2);
	if (cl->getUserName().empty())
		return (0);
	if (!std::strncmp(str.c_str(), "JOIN", 4))
		return (3);	
	if (!std::strncmp(str.c_str(), "PASS", 4))
		return (4);
	if (!std::strncmp(str.c_str(), "PRIVMSG", 7))
		return (5);
	if (!std::strncmp(str.c_str(), "KICK", 4))
		return (6);
	if (!std::strncmp(str.c_str(), "TOPIC", 5))
		return (7);
	if (!std::strncmp(str.c_str(), "PART", 4))
		return (8);
	if (!std::strncmp(str.c_str(), "QUIT", 4))
		return (9);
	if (!std::strncmp(str.c_str(), "INVITE", 6))
		return (10);
	if (!std::strncmp(str.c_str(), "MODE", 4))
		return (11);
	if (!std::strncmp(str.c_str(), "BOT", 3))
		return (12);
	return (0);
}

void	Server::_sendMessageToClient(const std::string & message, Client *client)
{
	send(client->getIdentifier(), message.c_str(), message.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
}

void	Server::_checkMessage(std::string message, unsigned int &i)
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
			this->_joinChannel(*this->_clients[i - 1], message);
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
		case 7: //TOPIC
		{
			this->_changeTopic(*this->_clients[i - 1], message);
			break ;
		}
		case 8: //PART
		{
			this->_partCase(*this->_clients[i - 1], message);
			break ;
		}
		case 9:
		{
			this->_removeClient(i, message);
			break ;
		}
		case 10:
		{
			this->_inviteCase(*this->_clients[i - 1], message);
			break ;
		}
		case 11:
		{
			this->_interpretMode(*this->_clients[i - 1], message);
			break ;
		}
		case 12:
		{
			this->_execMicroshell(*this->_clients[i - 1], message);
			break ;
		}
		default:
		{
			
			if (this->_clients[i - 1]->getNickName().empty())
			{
				this->_sendMessageToClient("451 : Please register a NICKNAME (NICK nickname) AND a USERNAME (USER username)\r\n", this->_clients[i - 1]);
				break ;
			}
			else if (this->_clients[i - 1]->getUserName().empty())
			{
				this->_sendMessageToClient("451 : Please register a USERNAME (USER username)\r\n", this->_clients[i - 1]);
				break ;
			}
			else
				this->sendErrToClient(*this->_clients[i - 1], ERR_UNKNOWNCOMMAND(message));
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
		this->_removeClient(i, "QUIT");
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
