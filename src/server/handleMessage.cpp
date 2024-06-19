/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleMessage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 03:56:02 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/19 17:24:20 by ehalliez         ###   ########.fr       */
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
	if (!std::strncmp(str.c_str(), "WHO", 3))
		return (12);
	return (0);
}

void	Server::_sendMessageToClient(const std::string & message, Client *client)
{
	send(client->getIdentifier(), message.c_str(), message.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
}

void	Server::_checkMessage(std::string message, unsigned int &i)
{
	switch (this->_getCommand(message, this->_clients[i - 1]))
	{
		case 1: // NICKNAME
		{
			printServer("\033[1;95mCommand NICK triggered ");
			this->_nicknameCase(*this->_clients[i - 1], message);
			break;
		}
		case 2: // USERNAME
		{
			printServer("\033[1;95mCommand USER triggered ");
			this->_usernameCase(*this->_clients[i - 1], message);
			break ;
		}
		case 3: // JOIN	
		{
			printServer("\033[1;95mCommand JOIN triggered ");
			this->_joinChannel(*this->_clients[i - 1], message);
			break ;
		}
		case 4: //PASS
		{
			printServer("\033[1;95mCommand PASS triggered ");
			this->_passCase(*this->_clients[i - 1], message);
			break ;
		}
		case 5: //PRIVMSG
		{
			printServer("\033[1;95mCommand PRIVMSG triggered ");
			this->_privmsgCase(*this->_clients[i - 1], message);
			break ;
		}
		case 6: //KICK
		{
			printServer("\033[1;95mCommand KICK triggered ");
			this->_kickCase(*this->_clients[i - 1], message);
			break ;
		}
		case 7: //TOPIC
		{
			printServer("\033[1;95mCommand TOPIC triggered ");
			this->_changeTopic(*this->_clients[i - 1], message);
			break ;
		}
		case 8: //PART
		{
			printServer("\033[1;95mCommand PART triggered ");
			this->_partCase(*this->_clients[i - 1], message);
			break ;
		}
		case 9:
		{
			printServer("\033[1;95mCommand QUIT triggered ");
			this->_removeClient(i, message);
			break ;
		}
		case 10:
		{
			printServer("\033[1;95mCommand INVITE triggered ");
			this->_inviteCase(*this->_clients[i - 1], message);
			break ;
		}
		case 11:
		{
			this->_interpretMode(*this->_clients[i - 1], message);
			printServer("\033[1;95mCommand MODE triggered ");
			break ;
		}
		case 12:
		{
			break ;
		}
		default:
		{
			
			if (this->_clients[i - 1]->getNickName().empty())
			{
				this->_sendMessageToClient("451 : Please register a NICKNAME (NICK nickname) first\r\n", this->_clients[i - 1]);
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
			if (message.find("\n") == std::string::npos)
			{
				this->_clients[i - 1]->getBuffer() += message;
				continue;
			}
			messages[messageIndex] = this->_clients[i - 1]->getBuffer() + messages[messageIndex];
			this->_clients[i - 1]->getBuffer() = "";
			messages[messageIndex] = strtrim(messages[messageIndex]);
			if (this->_clients[i - 1]->getAccess())
				this->_checkMessage(messages[messageIndex], i);
			else 
				this->_checkPassword(*this->_clients[i - 1], messages[messageIndex]);
		}
	}
	return (1);
}
