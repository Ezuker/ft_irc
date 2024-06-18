/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:32:46 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/18 16:28:53 by ehalliez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "global.hpp"
# include "Client.hpp"

struct mode
{
	bool		invitation;
	bool		changeTopic;
	std::string password;
	int			userLimit;
	std::string all;
};

class Client;
class Channel
{
	public:
		Channel(std::string name, Client* op) : _channelName(name) {
			_clients.push_back(op);
			_operators.push_back(op);
			_CreationDate = time(0);
		};
		~Channel() {};
		std::string				getChannelName(void) const						{return (this->_channelName);};
		std::string				getTopic(void) const							{return (this->_topic);};
		std::string 			getLastChange(void)								{return (this->_lastChange);};
		bool					&getIsTopic(void)								{return (this->_isTopic);};
		void					setChannelName(std::string name)				{this->_channelName = name;};
		std::vector<Client *>	&getClients(void)								{return (this->_clients);};
		std::vector<Client *>	&getOperators(void)								{return (this->_operators);};
		std::vector<Client *>	&getInvites(void)								{return (this->_invites);};
		struct mode				&getMode(void)									{return (this->_mode);};
		void					setClients(std::vector<Client *> clients)		{this->_clients = clients;};
		void					setOperators(std::vector<Client *> operators)	{this->_operators = operators;};
		void					setLastChange(std::string const & lastChange)	{this->_lastChange = lastChange;_lastTopicChange = time(0);};
		void					setTopic(std::string const & topic)				{this->_topic = topic;};
		void					setIsTopic(bool value)							{this->_isTopic = value;};
		std::string 			getMaskList(void);
		std::string 			getMaskList2(void);
		std::string 			getClientList(void);
		std::string 			getOperatorList(void);
		std::string 			getLastTopicChangeTime(void) const;
		std::string 			getCreation(void) const;
		int						isOperator(Client &cl);
		void					sendMessageToClient(std::string const & messageToSend);
		void					toggleChannelMode(Client &cl, std::string message, char action);

	private:
		std::vector<Client *>	_clients;
		std::vector<Client *>	_operators;
		std::vector<Client *>	_invites;
		std::string				_channelName;
		std::string				_topic;
		bool					_isTopic;
		std::string				_lastChange;
		time_t					_lastTopicChange;
		time_t					_CreationDate;
		struct mode				_mode;
};
