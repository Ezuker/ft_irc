/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isValidName.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 02:47:51 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/16 03:04:47 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "global.hpp"
#include <regex.h>

bool isValidName(const std::string& nickname) {
	const char* nicknameRegex = "^[A-Za-z\\x5B-\\x60\\x7B-\\x7D][A-Za-z0-9_\\x5B-\\x60\\x7B-\\x7D-]{0,8}$";
	regex_t regexCompiled;
	int result = regcomp(&regexCompiled, nicknameRegex, REG_EXTENDED | REG_NOSUB);
	if (result != 0) {
		regfree(&regexCompiled);
		return false;
	}

	result = regexec(&regexCompiled, nickname.c_str(), 0, 0x00, 0);
	regfree(&regexCompiled);

	return result == 0;
}
