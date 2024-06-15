/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:31:35 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/15 05:55:09 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBAL_HPP
# define GLOBAL_HPP

# define MAX_BUFFER_SIZE 1024

/*                                   ERRORS                                   */
//PASS
# define ERROR_PW_MISMATCH "Error (464): this password is incorrect\n"
# define ERROR_PW "Error (464): please enter the password 'PASS (PASSWORD)'\n"

//NICK
# define ERR_NICKNAMEINUSE "Error (433): this Nickname is already used\n"
# define ERR_NONICKNAMEGIVEN "Error (431): No nickname given in parameter\n"

//CHANNEL
# define ERR_NOSUCHCHANNEL "Error (403): No such channel\n"
# define ERR_CHANOPRIVSNEEDED "Error (482): No such permission\n"
# define ERR_NOTONCHANNEL "Error (442): Not on channel\n"
# define ERR_BADCHANMASK "Error (476): The specified channel name is invalid.\n"

//GLOBAL
# define ERR_NEEDMOREPARAMS "Error (461): Need more params\n"
# define ERR_ALREADYREGISTERED "Error (462): You are already registered\n"

/*                                                                            */

// readEvent
# include <vector>
# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <string.h>
# include <poll.h>
# include <algorithm>
# include <stdlib.h>
# include <cstring>

std::string	strtrim(std::string &s);
std::vector<std::string> split(std::string toSplit, char splitter);

#endif
