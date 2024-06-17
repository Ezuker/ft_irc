/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getMask.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 00:33:17 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/17 10:49:02 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "global.hpp"
#include "Server.hpp"

std::string	getMask(Client const &cl)
{
	return (":" + cl.getNickName() + "!" + cl.getUserName() + "@" + cl.getHostName() + " ");
}

void Server::sendErrToClient(Client &cl, std::string err)
{
	return (this->_sendMessageToClient(":" + this->_hostname + " " + err, &cl));
}
