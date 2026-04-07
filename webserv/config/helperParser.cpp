/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helperParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 17:13:30 by pberset           #+#    #+#             */
/*   Updated: 2026/03/18 17:13:37 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <helperParser.hpp>

/// @brief converts string into an unsigned int
/// @param token 
/// @return unsigned int conversion of token
unsigned int	ft_stoui(std::string& token) {
	int	output = 0;

	for (unsigned int i = 0; i < token.size(); i++) {
		output = output * 10 + (token.at(i) - '0');
	}
	return (output);
}

/// @brief lowercases string
/// @param str 
void	ft_toLower(std::string& str) {
	for (int i = 0; (unsigned int)i < str.size(); i++) {
		str.at(i) = std::tolower((unsigned char)str.at(i));
	}
}

/// @brief lowercases a cstring
/// @param c 
void	ft_toLowerChr(char* c) {
	*c = std::tolower((unsigned char)*c);
}

/// @brief converts a boolean string
/// @param token 
/// @return bool equivalent of "true" or "false"
bool	ft_stob(std::string& token) {
	ft_toLower(token);
	if (!token.compare("true")) {
		return (true);
	} else if (!token.compare("false")) {
		return (false);
	} else {
		std::cerr << "ft_stob: invalid token. Got " << token << std::endl;
		return (false);
	}
}

/// @brief checks if string contains an unsigned int representation
/// @param token 
/// @return bool true or false
bool	ft_isNUM(std::string token) {
	for (size_t i = 0; i < token.length(); i++) {
		if (token.at(i) < '0' || token.at(i) > '9') {
			std::cerr << "ft_isNUM: token. Got " << token << " is not a valid unsigned numeral string" << std::endl;
			return (false);
		}
	}
	return (true);
}
