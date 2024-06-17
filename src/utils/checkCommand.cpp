/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 08:45:09 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/17 11:17:29 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool	Server::checkCommand(std::string commandName, std::string message, Client &cl)
{
	if (message == commandName)
	{
		this->sendErrToClient(cl, ERR_NEEDMOREPARAMS(cl.getNickName(), commandName));
		return (false);
	}
	commandName += " ";
	if (std::strncmp(message.c_str(), commandName.c_str(), commandName.size()))
	{
		this->sendErrToClient(cl, ERR_UNKNOWNCOMMAND(message));
		return (false);
	}
	return (true);
}
