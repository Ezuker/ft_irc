/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 18:03:38 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/19 18:03:47 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		std::cout << "\033[1;91mERROR: Bad execution\033[0m" << std::endl;
		std::cout << "\033[1;93m	./bot [IP] [PORT] [PASSWORD]" << std::endl;
		return (1);
	}
	Bot ChatBot(argv[1], atoi(argv[2]), argv[3]);
	char buffer[1024];
	memset(buffer, 0, 1024);
	std::cout << "\033[1;90mBot connected.\033[0m" << std::endl;
	while (true)
	{
		int bytes_received = recv(ChatBot.getSocket(), buffer, 1024, 0);
		if (bytes_received > 0)
        {
			std::string message(buffer, bytes_received);	
			ChatBot.parse_message(message);
            if (std::string(buffer).find("PING") != std::string::npos)
            {
                std::string pongResponse = "PONG " + std::string(buffer).substr(std::string(buffer).find(":") + 1);
                send(ChatBot.getSocket(), pongResponse.c_str(), pongResponse.length(), 0);
            }
        }
        else if (bytes_received == 0)
        {
            std::cerr << "Connexion fermée par le serveur" << std::endl;
			close(ChatBot.getSocket());
            return (1);
        }
        else
        {
            std::cerr << "Erreur lors de la réception" << std::endl;
            return (1);
	    }
	}
	close(ChatBot.getSocket());
    return (1);
}
