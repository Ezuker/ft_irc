/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 08:45:09 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/16 08:52:09 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool	Server::checkCommand(std::string commandName, std::string message, Client &cl)
{
	if (message == commandName)
	{
		this->_sendMessageToClient(":" + this->_hostname + " " + ERR_NEEDMOREPARAMS(cl.getNickName(), commandName), &cl);
		return (false);
	}
	commandName += " ";
	if (std::strncmp(message.c_str(), commandName.c_str(), commandName.size()))
	{
		this->_sendMessageToClient("UKNOWN COMMAND\r\n", &cl);
		return (false);
	}
	return (true);
}
