/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 18:37:19 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/19 18:03:33 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

void	Bot::_init(char *addr, int port, std::string password)
{
    struct sockaddr_in bot_addr;

    this->_socket = socket(AF_INET, SOCK_STREAM, 0);
    bot_addr.sin_family = AF_INET;
	bot_addr.sin_port = htons(port);
	bot_addr.sin_addr.s_addr =  inet_addr(addr);
    if (connect(this->_socket, (sockaddr *)&bot_addr, sizeof(bot_addr)) < 0)
	{
		std::cerr << "Error: Connect() failed" << std::endl;
		close(this->_socket);
		this->_socket = -1;
		return ;
	}
    std::string toSend = "PASS " + password + "\r\n" + "NICK Bot\r\n" + "USER Bot 0 * :chatgpt\r\n";
    write(this->_socket, toSend.c_str(), toSend.size());
}

void Bot::_execMicroshell(std::string command, const std::string &name)
{
	if (!command.empty() && command[command.size() - 1] == '\n')
    {
        command.erase(command.size() - 1);
    }
	command = removeSingleQuotes(command);
    const char *microshellPath = "./command";
    const char *apiUrl = "https://api.openai.com/v1/chat/completions";
	std::ifstream	env;
    std::string response;
	env.open("./src/bot/.env");
    std::string apiKey;
	if (!env.is_open() || !std::getline(env, apiKey))
    {
        std::cerr << "Erreur lors de la lecture du fichier .env" << std::endl;
        return;
    }
    std::stringstream ss;
    ss << microshellPath << " /bin/curl -X POST -H \"Authorization: Bearer ";
	ss << apiKey << "\" -H \"Content-Type: application/json\" -d '{\"model\":\"gpt-3.5-turbo\",\"messages\":[{\"role\": \"user\", \"content\": \"";
	ss << command.substr(0, command.size() - 1); 
	ss << "\"}],\"max_tokens\":150}' \"";
	ss << apiUrl << "\"";
	std::string curlCommand = ss.str();
    FILE *pipe = popen(curlCommand.c_str(), "r");
    if (!pipe)
    {    
        std::cerr << "Erreur lors de l'exécution de curl" << std::endl;
		env.close();
        return;
    }

    char result[1024];
    std::string checker;
    while (fgets(result, sizeof(result), pipe) != NULL)
	{
        checker = result;
        if (checker.find("\"content\"") != std::string::npos)
        {
            response += result;
            break ;
        }
    }
    while (fgets(result, sizeof(result), pipe) != NULL)
    {
        checker = result;
        if (checker.find(" },") != std::string::npos)
            break ;
        response += result;
    }
    pclose(pipe);
    if (response.size())
    {
        response = cleanResponse(response);
    	std::string toSend = "PRIVMSG " + name + " :";
		toSend += response;
		toSend += "\r\n";
        write(this->_socket, toSend.c_str(), toSend.size());
		std::cout << "\033[1;91mBot sended a message.\033[0m" << std::endl;
		env.close();
    }
}
