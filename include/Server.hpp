/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:21:17 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/13 02:54:29 by bcarolle         ###   ########.fr       */
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
		Server(unsigned short port, std::string password, int fdSocketServ);
		~Server();
        int     startServer();
	private:
	// Function
        int     _initServer();
        int     _addClient();
        int     _handleMessage(unsigned int &i);
        void	_removeClient(unsigned int &index);
        pollfd	_createPollfd(int sock, short events, short revents);
		int		_authClients();
		int		_isGranted(unsigned int fd);
		int		_saveClient(int client_sock);
		
	// Attributs
		// Vector
		std::vector<Client *>	_clients;
        std::vector<pollfd>     _fds;
		std::vector<Channel *>	_channels;
		// 
		unsigned short			_port;
		std::string				_password;
		int                     _fdSocketServ;
};

#endif
