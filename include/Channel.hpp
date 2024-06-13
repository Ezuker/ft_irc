/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:32:46 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/13 06:24:35 by ehalliez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "global.hpp"
# include "Client.hpp"

class Channel
{
	public:
		Channel(std::string name, Client* op) : _channelName(name) {
			_clients.push_back(op);
			_operators.push_back(op);
		};
		~Channel() {};
		std::string				getChannelName() const							{return (this->_channelName);};
		void					setChannelName(std::string name)				{this->_channelName = name;};
		std::vector<Client *>	getClients(void) const							{return (this->_clients);};
		std::vector<Client *>	getOperators(void) const						{return (this->_operators);};
		void					setClients(std::vector<Client *> clients)		{this->_clients = clients;};
		void					setOperators(std::vector<Client *> operators)	{this->_operators = operators;};
	private:
		std::vector<Client *>	_clients;
		std::vector<Client *>	_operators;
		std::string				_channelName;
};

#endif
