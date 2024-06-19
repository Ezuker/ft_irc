/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_message.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 18:02:56 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/19 18:03:21 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"


void	Bot::parse_message(std::string message)
{
	if (message.find("PRIVMSG ") == std::string::npos)
		return ;
	size_t start_len;
	std::string response;
	
	response = message;
	_targetName = "";
	if (response[0] == ':')
		response.substr(1);
	for (size_t i = 0; i < response.size(); i++)
	{
		if (response[0] == '!')
		{
			response = response.substr(1);
			break ;
		}
		_targetName += response.substr(0, 1);
		response = response.substr(1);
	}
	for (start_len = 0; start_len < response.size(); start_len++)
	{
		if (!response.substr(0, 8).compare("PRIVMSG "))
			break ;
		response = response.substr(1);
	}	
	while (response.size() && response[0] != ':')
		response = response.substr(1);
	this->_execMicroshell(response.substr(1), _targetName.substr(1));
}
