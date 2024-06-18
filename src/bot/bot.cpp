/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 18:37:19 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/18 04:39:29 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <sstream>
#include <cstdio>

std::string cleanResponse(const std::string& response)
{
    std::string content = "\"content\": \"";
    // if (response.find(content) != std::string::npos)
    //     return ""; // No content found
    // size_t size_pos = content.size();
    return (response.substr(content.size(), content.size() - 1));
}

void Server::_execMicroshell(Client &client, const std::string &message)
{
    std::string command = message.substr(4);
    const char *microshellPath = "./command";
    const char *apiUrl = "https://api.openai.com/v1/chat/completions";

    // Construire la commande pour `curl` avec l'API de complétion de OpenAI
    std::stringstream ss;
    ss << microshellPath << " /bin/curl -X POST -H \"Authorization: Bearer " << apiKey << "\" -H \"Content-Type: application/json\" -d '{\"model\":\"gpt-3.5-turbo\",\"messages\":[{\"role\": \"user\", \"content\": \"" << command << "\"}],\"max_tokens\":150}' \"" << apiUrl << "\"";
    std::string curlCommand = ss.str();

    FILE *pipe = popen(curlCommand.c_str(), "r");
    if (!pipe) {    
        std::cerr << "Erreur lors de l'exécution de curl" << std::endl;
        return;
    }

    char result[1024];
    std::string response;
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


    this->_sendMessageToClient(cleanResponse(response), &client);
    // std::string test = " \"content\": \"Il était une fois une petite fille nommée Lily, qui vivait dans un petit village en bordure de la forêt. Un jour, en se promenant dans les bois, elle découvrit un mystérieux livre ancien caché sous un arbre. Intriguée, elle l'ouvrit et fut transportée dans un monde magique et enchanté.\n\nDans ce monde, les arbres parlaient, les animaux dansaient et les fleurs chantaient. Lily se fit de nombreux amis et apprit de nombreuses choses sur la nature et la magie. Elle était tellement captivée par ce nouveau monde qu'elle ne voulait plus en partir.\n\nMais un jour, le\"";
    // cleanResponse(test);
}

