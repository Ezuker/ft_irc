/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:21:17 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/16 00:36:49 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <poll.h>
# include "Channel.hpp"

struct clientData {
	std::string nickName;
	std::string hostName;
	std::string userName;
};

class Channel;
class Client
{
	public:
		Client(int32_t id) : _identifier(id) {std::cout << "Client :" << id << " saved." << std::endl;};
		~Client() {};
		void		setNickName(std::string nickName)	{this->_names.nickName = nickName;};
		void		setHostName(std::string hostName)	{this->_names.hostName = hostName;};
		void		setUserName(std::string userName)	{this->_names.userName = userName;};
		void		setAccess(bool access)				{this->_access = access;};
		void		setBelongChannel(std::vector<Channel *> &belongChannel) {this->_belongChannel = belongChannel;};
		bool		getAccess(void) const				{return this->_access;};
		std::string	getUserName(void) const				{return this->_names.userName;};
		std::string	getHostName(void) const				{return this->_names.hostName;};
		std::string	getNickName(void) const				{return this->_names.nickName;};
		std::vector<Channel *>	&getBelongChannel(void) {return this->_belongChannel;};
		int				_isInChannel(Channel const & channel);
		unsigned int	getIdentifier(void) const		{return _identifier;};
	private:
		clientData      _names;
		unsigned int    _identifier;
		bool            _access;
		std::vector<Channel *>	_belongChannel;
};

std::string	getMask(Client const &cl);
