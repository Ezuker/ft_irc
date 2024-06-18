/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:21:17 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/18 16:54:45 by ehalliez         ###   ########.fr       */
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
		void	sendErrToClient(Client &cl, std::string err);
        int		startServer();
		
	private:
	// Function
        int     _initServer();
        pollfd	_createPollfd(int sock, short events, short revents);
		// HANDLE CLIENT
        int     _handleMessage(unsigned int &i);
        int     _addClient();
        void	_removeClient(unsigned int &index, std::string message);
        void    _checkPassword(Client &cl, std::string message);
        void    _checkMessage(std::string, unsigned int &i);
		void	_sendMessageToClient(const std::string & message, Client *client);

		int		_getCommand(std::string str, Client *cl);
		int		_authClients();
		int		_saveClient(int client_sock);
		void	_nicknameCase(Client & cl, std::string const & message);
		void	_usernameCase(Client & cl, std::string const & message);
		void	_privmsgCase(Client & cl, std::string const & message);
		void	_kickCase(Client & cl, std::string const & message);
		void	_passCase(Client &cl, std::string const & message);
		void	_partCase(Client & cl, std::string const & message);
		void	_inviteCase(Client & cl, std::string const & message);
		bool	checkCommand(std::string commandName, std::string message, Client &cl);
		
		// MODE
		void	_interpretMode(Client &cl, std::string mode);
		void	_manageOperator(Client &cl, std::vector<std::string> &splitted, char action, Channel *channel);
		bool	_checkMode(Client &cl, Channel *channel, std::vector<std::string> splitted);

		//	Channel
		void	_joinChannel(Client & cl, std::string & message);
		Channel	* _channelExists(std::string channel);
		int		_createChannel(Client & cl, std::string name);
		void 	_refreshList(Channel *channel, Client & cl);
		bool	_checkChannelName(std::string const & name);
		void	_changeTopic(Client & cl, std::string name);
		void	_execMicroshell(Client &client, const std::string &message);
	// Attributs
		// Vector
		std::vector<Client *>	_clients;
        std::vector<pollfd>     _fds;
		std::vector<Channel *>	_channels;
		// 
		unsigned short			_port;
		std::string				_password;
		int                     _fdSocketServ;
		const std::string		_hostname;
};
