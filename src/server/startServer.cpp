/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   startServer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 03:58:23 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/13 16:26:30 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "global.hpp"

int     Server::startServer()
{
	if (!this->_initServer())
		return (0);
	std::cout << "IRC server listening on port: " << this->_port << std::endl;
	while (true)
	{
		int poll_count = poll(this->_fds.data(), this->_fds.size(), -1);
		if (poll_count < 0)
		{
			std::cerr << "Poll failed" << std::endl;
			break;
		}
		for (unsigned int i = 0; i < this->_fds.size(); i++)
		{
			if (this->_fds[i].revents & POLLIN)
			{
				if (this->_fds[i].fd == this->_fdSocketServ)
				{
					if (!this->_addClient())
						continue;
				}
				else if (!this->_handleMessage(i))
                        continue;
			}
			this->_fds[i].revents = 0;
		}
	}
	this->_fds.erase(this->_fds.begin());
	close(this->_fdSocketServ);
    return (1);
}
	