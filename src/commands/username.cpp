/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   username.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 16:18:15 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/14 16:18:30 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

void	usernameCase(Client & cl, std::string const & message)
{
	std::string toSet;
	
	if (cl.getNickName().size() == 0 || cl.getUserName().size() != 0)
		return ;
	toSet = split(message, ' ')[1]; //USERNAME bcarolle 0 *: realname
	cl.setUserName(toSet);
	std::cout << "Username set to : " << toSet << std::endl;
}
