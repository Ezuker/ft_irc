/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehalliez <ehalliez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 18:37:19 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/19 18:01:09 by ehalliez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

std::string	strtrim(std::string s)
{
	std::string strtrim(std::string s);
	size_t first = s.find_first_not_of(" \t\n\r\f\v");

	if (first == std::string::npos)
		return "";

	size_t last = s.find_last_not_of(" \t\n\r\f\v");

	return s.substr(first, last - first + 1);
}


std::string cleanResponse(std::string & response)
{
	size_t startpos;
    std::string content = "\"content\": \"";
    for (startpos = 0; startpos < response.size() && response[0] == ' '; startpos++)
		response = response.substr(1);
	return (response.substr(content.size(), response.size() - content.size() - 2));
}

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

std::string removeSingleQuotes(const std::string& input)
{
    std::string output;
    for (size_t i = 0; i < input.length(); ++i)
    {
        if (input[i] != '\'')
        {
            output += input[i];
        }
    }
    return output;
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

void	Bot::parse_message(std::string message)
{
	if (message.find("PRIVMSG ") == std::string::npos)
		return ;
	size_t start_len;
	std::string response;
	
	response = message;
	_targetName = "";
	if (response[0] == ':')
		response.substr(1);
	for (size_t i = 0; i < response.size(); i++)
	{
		if (response[0] == '!')
		{
			response = response.substr(1);
			break ;
		}
		_targetName += response.substr(0, 1);
		response = response.substr(1);
	}
	for (start_len = 0; start_len < response.size(); start_len++)
	{
		if (!response.substr(0, 8).compare("PRIVMSG "))
			break ;
		response = response.substr(1);
	}	
	while (response.size() && response[0] != ':')
		response = response.substr(1);
	this->_execMicroshell(response.substr(1), _targetName.substr(1));
}

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
