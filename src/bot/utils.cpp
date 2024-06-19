/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 18:01:57 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/19 18:02:40 by bcarolle         ###   ########.fr       */
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
