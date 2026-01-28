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
    std::vector<Server> output = {};
	std::string			token;

	while (_ss >> token) { //will stop at !_ss.good()
		if (token == "server") {
			output.push_back(parseServer());
		} else {
			std::cerr << "config file: unexpected token at root level: " << token << std::endl;
			//set a custom errno and return?
			break ;
		}
	}
	
	if (output.empty()) {
		std::cerr << "config file: no server definition found" << std::endl;
		//set a custom errno?
	}

	return (output);
}

Server	Parser::parseServer(void) {
	struct s_server			servStruct = {};
	std::vector<Location>	locs = {};
	std::string				token;
	std::string				serverAllowed[] = {"server_name", "root", "index", "access_logs", "error_logs", "client_max_body_size", "error"};

	_ss >> token;
	if (token != "{") {
		std::cerr << "server configuration: unexpected token: " << token << std::endl;
		//set a custom errno?
		return (Server(servStruct,locs));
	}

	do {
		_ss >> token;

		if (token == "listen") {
			servStruct.listen.push_back(parseListen());
			continue ;
		}

		if (token == "location") {
			locs.push_back(parseLocation());
			continue ;
		}

		

	} while (token != "}" && _ss.good());
	

	//if locs & listen are not empty - else error handling
	Server	output(servStruct, locs);

	return (output);
}

struct s_listen	parseListen(void) {
	struct s_listen	output;
	//ip and port are the only data.
	//for simplicity, ip is assumed to be 0.0.0.0 and is not written in config
	output.ip = "0.0.0.0";
	//first int for port - else error handling
	//output.port = token;
	//then protocol - if token == ';' - then default protocol to 'HTTP'
	//output.protocol = token;
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
