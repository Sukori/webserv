/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 14:36:34 by pberset           #+#    #+#             */
/*   Updated: 2026/01/28 14:36:45 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser(const std::string data): _ss(data) {}

Parser::~Parser(void) {}

std::vector<Server> Parser::initParser(void) {
    std::vector<Server> output;

	//while _ss >> token //(_ss.good())
	//if token is "server"
	output.push_back(parseServer());
	//else error handling

	return (output);
}

Server	Parser::parseServer(void) {
	struct s_server			servStruct;
	std::vector<Location>	locs;

	//if opening token is '{' - else error handling
	//while token != '}' or eof - error handling if eof
	//if token is a server token - else error handling
	//populate struct
	//if token is "location"
	locs.push_back(parseLocation());
	//if location is not empty or has minimal necessary - else error handling
	Server	output(servStruct, locs);

	return (output);
}

Location	Parser::parseLocation(void) {
	struct s_location	locStruct;

	//if token is here valid /path/ - else error handling
	//if token if '{' - else error handling
	//while token != '}' or eof - error handling if eof
	//if token is a location token - else error handling
	//populate struct

	Location	output(locStruct);
	return (output);
}
