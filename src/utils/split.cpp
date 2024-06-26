/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:12:10 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/15 21:16:08 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "global.hpp"

std::vector<std::string> split(std::string toSplit, char splitter)
{
	std::vector<std::string> result;
	std::string word;
	std::istringstream iss(toSplit);
	while (std::getline(iss, word, splitter)) {
		result.push_back(word);
	}
	return result;
}
