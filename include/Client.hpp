/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:21:17 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/13 14:11:29 by ehalliez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <poll.h>

struct clientData {
	std::string nickName;
	std::string hostName;
	std::string userName;
};

class Client
{
	public:
		Client(int32_t id) : _identifier(id), _currentChannel("")	{std::cout << "Client :" << id << " saved." << std::endl;};
		~Client() {};
		void		setNickName(std::string nickName)	{this->_names.nickName = nickName;};
		void		setHostName(std::string hostName)	{this->_names.hostName = hostName;};
		void		setUserName(std::string userName)	{this->_names.userName = userName;};
		void		setCurrentChannel(std::string currentChannel){this->_currentChannel = currentChannel;};
		void		setAccess(bool access)			{this->_access = access;};
		bool		getAccess(void) const			{return this->_access;};
		std::string	getCurrentChannel(void) const		{return this->_currentChannel;};
		std::string	getUserName(void) const			{return this->_names.userName;};
		std::string	getHostName(void) const			{return this->_names.hostName;};
		std::string	getNickName(void) const			{return this->_names.nickName;};
		unsigned int     getIdentifier(void) const		{return _identifier;};
	private:
		clientData      _names;
		unsigned int    _identifier;
		bool            _access;
		std::string		_currentChannel;
};

#endif
