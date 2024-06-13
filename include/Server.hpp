/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:21:17 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/14 00:59:46 by bcarolle         ###   ########.fr       */
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
        pollfd	_createPollfd(int sock, short events, short revents);
		// HANDLE CLIENT
        int     _handleMessage(unsigned int &i);
        int     _addClient();
        void	_removeClient(unsigned int &index);
        void    _checkPassword(std::string message, int i);
        void    _checkMessage(std::string message, int i);
		void	_sendMessageToClient(std::string message, int i);
		int		_getCommand(std::string str);
		int		_authClients();
		int		_saveClient(int client_sock);
		//	Channel
		// void	_joinChannel(Channel *channel, int i);
		Channel	* _channelExists(std::string channel);
		int		_createChannel(std::string channel, int i);
		void 	refreshList(Channel *channel);
		void	_sendMessageToChannelClients(Client *sender, const std::string &message);
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
