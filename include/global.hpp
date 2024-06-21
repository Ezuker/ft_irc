/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:31:35 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/19 18:18:56 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBAL_HPP
# define GLOBAL_HPP

# define MAX_BUFFER_SIZE 1024

/*                                   ERRORS                                   */
//GLOBAL
# define ERR_NEEDMOREPARAMS(n, c) "461 " + n + " " + c + " : Need more params\r\n"
# define ERR_ALREADYREGISTERED(c) "462 " + c + " : You are already registered\r\n"
# define ERR_UNKNOWNCOMMAND(c) "421 " + c + " :Unknown command\r\n"

//PASS
# define ERR_PASSWDMISMATCH "464 :Password incorrect\r\n"

//TOPIC

# define ERR_RPL_NOTOPIC(n, c) "331 " + n + " " + c + " :No topic set\r\n"

//NICK
# define ERR_NICKNAMEINUSE(n) "433 " + n + " :Nickname is already in use\r\n"
# define ERR_NONICKNAMEGIVEN "431 :No nickname given\r\n"
# define ERR_ERRONEUSNICKNAME(n) "432 " + n + " :Erroneous nickname\r\n"

//CHANNEL
# define ERR_NOSUCHCHANNEL(c) "403 " + c + " :No such channel\r\n"
# define ERR_NOSUCHNICK(c) "401 " + c + " :No such nick/channel\r\n"
# define ERR_CHANOPRIVSNEEDED(ch, cl) "482 "+ ch + " " + cl + " :No such permission\r\n"
# define ERR_NOTONCHANNEL(c) "442 " + c + " :You're not on that channel\r\n"
# define ERR_BADCHANMASK(n) "476 " + n + " :The specified channel name is invalid.\r\n"
# define ERR_CHANNELISFULL(c) "471 " + c + " :Cannot join channel (+l)\r\n"
# define ERR_INVITEONLYCHAN(c) "473 " + c + " :Cannot join channel (+i)\r\n"
# define ERR_BADCHANNELKEY(c) "475 " + c + " :Cannot join channel (+k)\r\n"
# define ERR_KEYSET(c) "467 " + c + " :Channel key already set\r\n"
# define ERR_INVITECL(c, c2, ch) "443 " + c + " " + c2 + " " + ch + " :is already on channel\r\n"

//MODE
# define ERR_UMODEUNKNOWNFLAG "501 :Unknown MODE flag\r\n"

//INVITE
# define ERR_USERONCHANNEL(n, c) "443 " + n + " " + c + " :is already on channel\r\n"
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
# include <sstream>
# include <ctime>
# include <signal.h>

std::string	strtrim(std::string s);
std::vector<std::string> split(std::string toSplit, char splitter);
bool isValidName(const std::string& nickname);
void    printServer(std::string message);

#endif
