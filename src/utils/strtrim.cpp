/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strtrim.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 16:00:52 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/16 04:02:03 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "global.hpp"

std::string	strtrim(std::string s)
{
	std::string strtrim(std::string s);
	size_t first = s.find_first_not_of(" \t\n\r\f\v");

	if (first == std::string::npos)
		return "";

	size_t last = s.find_last_not_of(" \t\n\r\f\v");

	return s.substr(first, last - first + 1);
}
