/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:12:10 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/15 06:55:22 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "global.hpp"
#include <sstream>

std::vector<std::string> split(std::string toSplit, char splitter)
{
	std::vector<std::string> result;
	std::string word;
	std::istringstream iss(toSplit);
	while (std::getline(iss, word, splitter) && word[0] != ':') {
		result.push_back(word);
	}
	return result;
}
