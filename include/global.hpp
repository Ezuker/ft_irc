/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:31:35 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/16 00:41:14 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBAL_HPP
# define GLOBAL_HPP

# define MAX_BUFFER_SIZE 1024

/*                                   ERRORS                                   */
//PASS
# define ERR_PASSWDMISMATCH(n) "464 " + n + " :Passwod Dismatch\n"
# define ERROR_PW "Error (464): please enter the password 'PASS (PASSWORD)'\n"

//TOPIC

# define ERR_RPL_NOTOPIC(n, c) "331 " + n + " " + c + " :No topic set\n"

//NICK
# define ERR_NICKNAMEINUSE(n) "433 " + n + " this Nickname is already used\n"
# define ERR_NONICKNAMEGIVEN(n) "431 " + n + " :No nickname given in parameter\n"

//CHANNEL
# define ERR_NOSUCHCHANNEL(n, c) "403 " + n + " " + c + " :No such channel\n"
# define ERR_CHANOPRIVSNEEDED(n, c) "482 " + n + " " + c + " :No such permission\n"
# define ERR_NOTONCHANNEL(n, c) "442 " + n + " " + c + " :Not on channel\n"
# define ERR_BADCHANMASK(n) "476 " + n + " :The specified channel name is invalid.\n"

//GLOBAL
# define ERR_NEEDMOREPARAMS(n, c) "461 " + n + " " + c + " : Need more params\n"
# define ERR_ALREADYREGISTERED "Error (462): You are already registered\n"

/*                                                                            */

// readEvent
# include <vector>
# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <string>
# include <poll.h>
# include <algorithm>
# include <stdlib.h>
# include <cstring>
#include <sstream>
# include <ctime>

std::string	strtrim(std::string &s);
std::vector<std::string> split(std::string toSplit, char splitter);

#endif
