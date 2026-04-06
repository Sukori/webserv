/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 14:36:34 by pberset           #+#    #+#             */
/*   Updated: 2026/02/18 12:00:21 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <Parser.hpp>

Parser::Parser(const std::string data): _ss(data) {}

Parser::~Parser(void) {}

/// @brief entry point of the parser. uses the input file saved as stringstream
/// @param  none
/// @return vector of servers detected in the config file
std::vector<Server> Parser::initParser(void) {
    std::vector<Server> output;
	std::string			token;

	do {
		_ss >> token;
		if (_ss.fail())
			break;
		if (!token.compare("server")) {
			Server	tmp(parseServer());
			if (tmp.isValid()) {
				output.push_back(tmp);
			}
		} else {
			std::cerr << "initParser: unexpected token at root level. Got " << token << std::endl;
			//set a custom errno and return?
			break ;
		}
	} while (!_ss.fail());


	
	if (output.empty()) {
		std::cerr << "config file: no server definition found" << std::endl;
	}

	return (output);
}
