/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 02:50:08 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/18 04:30:23 by ehalliez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

# include <iostream>
# include <arpa/inet.h>
# include <unistd.h>
# include <string>
# include <stdlib.h>
# include <cstring>
# include <fstream>
# include <sstream>

class Bot
{
	public:
		Bot(char *addr, int port, std::string password) {this->_init(addr, port, password);};
		~Bot() 											{};
		int			getSocket(void) const 				{return (this->_socket);};
		std::string	getTargetName(void) const 			{return (this->_targetName);};
		void			parse_message(std::string message);
		void			_init(char *addr, int port, std::string password);
		void			_execMicroshell(std::string command, const std::string &name);
	private:
		Bot () 											{};
		std::string _targetName;
		int			_socket;
};