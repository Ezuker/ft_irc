/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 02:56:26 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/16 05:26:24 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::_passCase(Client &cl, const std::string &message)
{
	if (cl.getAccess())
		this->_sendMessageToClient(ERR_ALREADYREGISTERED(cl.getNickName()), &cl);
	else 
		this->_checkPassword(cl, message);
}

void	Server::_checkPassword(Client &cl, std::string message)
{
	if (!std::strncmp(message.c_str(), "PASS", 4))
	{
		if (message == "PASS")
		{
			this->_sendMessageToClient(":" + this->_hostname + " " + ERR_NEEDMOREPARAMS(cl.getNickName(), "PASS"), &cl);
			return ;
		}
		else if (std::strncmp(message.c_str(), "PASS ", 5))
		{
			this->_sendMessageToClient("UNKNOWN COMMAND\r\n", &cl);
			return ;
		}
		std::string p_try = message.substr(5, message.size() - 5);
		p_try = strtrim(p_try);
		if (!p_try.size())
			this->_sendMessageToClient(":" + this->_hostname + " " + ERR_NEEDMOREPARAMS(cl.getNickName(), "PASS"), &cl);
		else if (p_try == this->_password)
			cl.setAccess(true);
		else
			this->_sendMessageToClient(ERR_PASSWDMISMATCH(cl.getNickName()), &cl);
	}
	else
		this->_sendMessageToClient(ERROR_PW, &cl);
}
