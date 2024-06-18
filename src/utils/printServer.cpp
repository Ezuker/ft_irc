/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printServer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:40:03 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/17 22:38:08 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "global.hpp"

static std::string get_real_time(void)
{
	time_t		now = time(0);
	struct tm	tstruct;
	char		buf[80];

	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%a %b %d %H:%M:%S %Y ", &tstruct);
	return (buf);
}

void    printServer(std::string message)
{
    std::cout << "\033[1;90m" << get_real_time() << " \033[0m" << message << "\033[0m" << std::endl;
}
