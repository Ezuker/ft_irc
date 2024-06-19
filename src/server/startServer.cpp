/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   startServer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 03:58:23 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/19 16:15:53 by ehalliez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "global.hpp"

extern bool signalTriggered;

int     Server::startServer()
{
	if (!this->_initServer())
		return (0);
    std::stringstream ss;
	ss << "\033[1;96mServer is listening on port " << this->_port << "...\033[0m";
	printServer(ss.str());
	while (!signalTriggered)
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
	