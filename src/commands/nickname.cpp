/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nickname.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 16:15:20 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/14 16:22:43 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

void	nicknameCase(Client & cl, std::string const & message)
{
	std::string toSet;
	
	if (cl.getNickName().size() != 0)
		return ;
	toSet = message.substr(5, message.size() - 5);
	
	cl.setNickName(toSet);
	std::cout << "Nickname set to : " << toSet << std::endl;
	return ;
}
