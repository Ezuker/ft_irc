/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getMask.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 00:33:17 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/16 00:36:36 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "global.hpp"
#include "Client.hpp"

std::string	getMask(Client const &cl)
{
	return (":" + cl.getNickName() + "!" + cl.getUserName() + "@" + cl.getHostName() + " ");
}
