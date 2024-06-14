/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 16:16:50 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/14 16:18:35 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "global.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

void	nicknameCase(Client & cl, std::string const & message);
void	usernameCase(Client & cl, std::string const & message);
