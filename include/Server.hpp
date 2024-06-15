/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:21:17 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/15 22:15:05 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
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
        void    _checkPassword(Client &cl, std::string message);
        void    _checkMessage(std::string, int i);
		void	_sendMessageToClient(const std::string & message, Client *client);

		int		_getCommand(std::string str, Client *cl);
		int		_authClients();
		int		_saveClient(int client_sock);
		void	_nicknameCase(Client & cl, std::string const & message);
		void	_usernameCase(Client & cl, std::string const & message);
		void	_privmsgCase(Client & cl, std::string const & message);
		void	_kickCase(Client & cl, std::string const & message);
		void	_passCase(Client &cl, std::string const & message);
		void	_quitCase(Client &cl, std::string const & message);

		//	Channel
		void	joinChannel(Client & cl, std::string const & message, int const & i);
		Channel	* _channelExists(std::string channel);
		int		_createChannel(std::string channel, int i);
		void 	refreshList(Channel *channel);
		bool	_checkChannelName(std::string const & name);
		void	_changeTopic(Client & cl, std::string name);
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
