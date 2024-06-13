/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strtrim.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:00:52 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/13 16:05:39 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "global.hpp"

std::string	strtrim(std::string &s)
{
	std::string strtrim(std::string s);
	{
		// Find the position of the first non-whitespace character
		size_t first = s.find_first_not_of(" \t\n\r\f\v");

		// If the string is empty or contains only whitespace, return an empty string
		if (first == std::string::npos)
			return "";

		// Find the position of the last non-whitespace character
		size_t last = s.find_last_not_of(" \t\n\r\f\v");

		// Extract the trimmed substring
		return s.substr(first, last - first + 1);
	}
}
