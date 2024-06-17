/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 02:56:26 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/17 11:19:34 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::_passCase(Client &cl, const std::string &message)
{
	if (cl.getAccess())
		this->sendErrToClient(cl, ERR_ALREADYREGISTERED(cl.getNickName()));
	else 
		this->_checkPassword(cl, message);
}

void	Server::_checkPassword(Client &cl, std::string message)
{
	if (!std::strncmp(message.c_str(), "PASS", 4))
	{
		if (message == "PASS")
		{
			this->sendErrToClient(cl, ERR_NEEDMOREPARAMS(cl.getNickName(), "PASS"));
			return ;
		}
		else if (std::strncmp(message.c_str(), "PASS ", 5))
		{
			this->sendErrToClient(cl, ERR_UNKNOWNCOMMAND(message));
			return ;
		}
		std::string p_try = message.substr(5, message.size() - 5);
		p_try = strtrim(p_try);
		if (!p_try.size())
			this->sendErrToClient(cl, ERR_NEEDMOREPARAMS(cl.getNickName(), "PASS"));
		else if (p_try == this->_password)
			cl.setAccess(true);
		else
			this->sendErrToClient(cl, ERR_PASSWDMISMATCH(cl.getNickName()));
	}
	else
		this->sendErrToClient(cl, ERROR_PW);
}
