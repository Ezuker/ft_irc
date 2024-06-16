/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 18:37:19 by ehalliez          #+#    #+#             */
/*   Updated: 2024/06/16 11:17:44 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <sstream>
#include <cstdio>

void Server::_execMicroshell(Client &client, const std::string &message)
{
    std::string command = message.substr(10); // Supposez que "MICROSHELL " fait 10 caractères
    const char *microshellPath = "./command";
    const char *apiUrl = "https://api.openai.com/v1/completions";

    // Construire la commande pour `curl` avec l'API de complétion de OpenAI
    std::stringstream ss;
    ss << microshellPath << " /bin/curl -X POST -H \"Authorization: Bearer " << apiKey << "\" -H \"Content-Type: application/json\" -d '{\"model\":\"gpt-3.5-turbo\",\"prompt\":\"" << command << "\",\"max_tokens\":150}' \"" << apiUrl << "\"";
    std::string curlCommand = ss.str();

    FILE *pipe = popen(curlCommand.c_str(), "r");
    if (!pipe) {
        std::cerr << "Erreur lors de l'exécution de curl" << std::endl;
        return;
    }

    char result[1024];
    std::string response;
    while (fgets(result, sizeof(result), pipe) != NULL) {
        response += result;
    }

    pclose(pipe);

    // Envoi de la réponse au client
    this->_sendMessageToClient(response, &client);
}

