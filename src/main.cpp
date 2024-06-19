/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:09:25 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/19 17:53:04 by ehalliez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "global.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <exception>


void	checkArgv(const char *port, const char *pw)
{
	std::string s_port = port;
	for (size_t i = 0; i < s_port.size(); i++)
		if (!std::isdigit(s_port[i]))
			throw std::logic_error("Need alpha character in port");
	if (strtold(port, 0x0) > 65535)
		throw std::logic_error("Port can't be superior to 65535");
	if (strtold(port, 0x0) < 1025)
		throw std::logic_error("Port can't be inferior to 1025");
	std::string s_pw = pw;
	if (s_pw.size() > 256)
		throw std::logic_error("Password can't be longer than 256");
}

bool signalTriggered;

void signalHandler(int signum)
{
	(void)signum;
	printServer("\033[1;91mInterrupt signal\033[0m");
	signalTriggered = true;
}
int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printServer("\033[1;91mNot enough args\033[0m");
		return (1);
	}
	int server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock < 0)
	{
		printServer("\033[1;91mError: socket() args\033[0m");
		return 1;
	}
	try
	{
		signal(SIGINT, signalHandler);
		checkArgv(argv[1], argv[2]);
		Server	myServer(atoi(argv[1]), argv[2], server_sock);
		myServer.startServer();
	}
	catch(const std::logic_error& e)
	{
		std::cerr << e.what() << '\n';
	}
	return 0;
}