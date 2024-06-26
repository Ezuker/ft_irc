/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleChannel.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 05:29:21 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/14 16:19:36 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "global.hpp"

Channel	*Server::_channelExists(std::string name)
{
	std::vector<Channel *>::iterator it = this->_channels.begin();
	for (; it != this->_channels.end(); ++it)
	{
		if ((*it)->getChannelName() == name)
			return (*it);
	}
	return (NULL);
}
