/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:21:17 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/12 19:09:47 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "global.hpp"
# include "Channel.hpp"
# include "Client.hpp"
# include <poll.h>

class	Server
{
	public:
		Server(unsigned short port, std::string password, int identifier);
		~Server();
        int     startServer();
		void socketHandle();
		std::vector<Client *>	_clients;
        std::vector<pollfd>     _fds;
	private:
        int     _addClient();
        pollfd	_createPollfd(int sock, short events, short revents);
		std::vector<Channel *>	_channels;
		unsigned short			_port;
		std::string				_password;
		int                     _identifier;
};

#endif
