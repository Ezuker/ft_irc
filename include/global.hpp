/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:31:35 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/14 02:27:35 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBAL_HPP
# define GLOBAL_HPP

# define MAX_BUFFER_SIZE 1024

// ERROR
# define ERROR_PW_MISMATCH "Error (464): this password is incorrect\n"
# define ERROR_PW "Error (464): please enter the password 'PASS (PASSWORD)'\n"

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
