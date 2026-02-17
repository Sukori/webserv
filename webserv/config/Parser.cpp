/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 14:36:34 by pberset           #+#    #+#             */
/*   Updated: 2026/02/14 10:18:35 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include <cmath>

Parser::Parser(const std::string data): _ss(data) {}

Parser::~Parser(void) {}

static int	ft_stoui(std::string& token) {
	int	output;

	for (unsigned int i = 0; i < token.size(); i++) {
		char	c = token.at(i);
		if (c - '0' < 0 && c - '0' > 9) {
			std::cerr << "ft_stoui: invalid char " << c << std::endl;
			return (-1);
		}
		output += (c - '0') * std::pow(10, (token.size() - i));
	}
	return (output);
}

static void	ft_toLower(std::string& str) {
	for (int i = 0; (unsigned int)i < str.size(); i++) {
		if (str.at(i) > 64 && str.at(i) < 91) {
			str.at(i) += 32;
		}
	}
}

static bool	ft_stob(std::string& token) {
	ft_toLower(token);
	if (token == "true") {
		return (true);
	} else if (token == "false") {
		return (false);
	} else {
		std::cerr << "ft_stob: invalid token " << token << std::endl;
		return (false);
	}
}

std::vector<Server> Parser::initParser(void) {
    std::vector<Server> output = {};
	std::string			token;

	while (_ss >> token) { //will stop at !_ss.good()
		if (token == "server") {
			output.push_back(parseServer());
		} else {
			std::cerr << "initParser: unexpected token at root level " << token << std::endl;
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

	servStruct.error_pages.insert(std::make_pair(404, "/www/error_pages/404.html"));
	_ss >> token;
	if (token != "{") {
		std::cerr << "parseServer: unexpected token " << token << std::endl;
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

		/*
			set the other allowed things
		*/
	} while (token != "}" && _ss.good());
	

	//if locs & listen are not empty - else error handling
	Server	output(servStruct, locs);

	return (output);
}

struct s_listen	Parser::parseListen(void) {
	struct s_listen	output = {};
	std::string		token;
	int				port;
	//ip and port are the only data.
	//for simplicity, ip is assumed to be 0.0.0.0 and is not written in config
	output.ip = "0.0.0.0";
	_ss >> token;
	port = ft_stoui(token);
	if (port < 8000) {
		//error invalid port
		std::cerr << "port: invalid port " << port << std::endl;
		return (output);
	}
	//first int for port - else error handling
	if (token.at(token.size() - 1) != ';') {
		std::cerr << "parseListen: unexpected token " << token << std::endl;
		//set a custom errno ?
		output.port = -1;
	}
	output.port = port;
	_ss >> token;
	//output.protocol = token; //actually, we don't accept anything else than "http"
	return (output);
}

//Single line declaration of all allowed methods only, terminated by semicolon
std::vector<std::string>	Parser::parseLimitExcept(std::string token) {
	std::vector<std::string>	output;

	while (token.at(token.size() - 1) != ';') {
		if (!_ss.good()) {
			std::cerr << "parseLimitExcept: unexpected end of line " << token << std::endl;
			output.insert(output.begin(), std::string("ERROR"));
			return (output);
		}
		output.push_back(token);
		_ss >> token;
	}
	if (token.size() != 1) {
		token.at(token.size() - 1) = '\0';
		output.push_back(token);
	}
	return (output);
}

std::map<std::string, std::string>	Parser::parseCgiParam(std::string& token) {
	std::map<std::string, std::string>	output;
	
	if (!_ss.good() || token == "}") {
		std::cerr << "parseCgiParam: [key] unexpected end of line " << token << std::endl;
		output.insert(output.begin(), std::make_pair("ERROR", "ERROR"));
		return (output);
	}

	std::string							key = token;
	std::string							value;

	do {
		_ss >> value;
		if (!_ss.good() || value == "}") {
			std::cerr << "parseCgiParam: [value] unexpected end of line " << value << std::endl;
			output.insert(output.begin(), std::make_pair("ERROR", "ERROR"));
			return (output);
		}
		output.insert(output.end(), std::make_pair(key, value));

		_ss >> key;
		if (!_ss.good()) {
			std::cerr << "parseCgiParam: [key] unexpected end of line " << key << std::endl;
			output.insert(output.begin(), std::make_pair("ERROR", "ERROR"));
			return (output);
		}
		if (key == "}")
			break;
	} while (_ss.good());
	return (output);
}

Location	Parser::parseLocation(void) {
	struct s_location	locStruct;
	std::string			token;
	std::string			locationAllowed[] = {"route", "root_path", "alias", "limit_except", "autoindex", "upload_path", "cgi_param", "cgi_pass"};

	locStruct.autoindex = true;
	locStruct.root_path = "/";
	locStruct.upload_path = "/uploads/";
	//locStruct.error_path # NO NEED ?

	//if token is here valid /path/ - else error handling
	_ss >> token;
	if (token.at(0) != '/') {
		std::cerr << "parseLocation: path not starting from root " << token << std::endl;
		locStruct.route = "ERROR";
		Location	error(locStruct);
		return (error);
	}
	locStruct.route = token;
	_ss >> token;

	//if token is '{' - else error handling
	if (token.at(0) != '{') {
		std::cerr << "parseLocation: unexpected token after route " << token << std::endl;
		Location	error(locStruct);
		return (error);
	}

	//while token != '}' or eof - error handling if eof
	//if token is a location token - else error handling
	//populate struct
	int	i;
	do {
		_ss >> token;

		for (i = 0; (unsigned int)i < locationAllowed->size(); i++) {
			if (token == locationAllowed[i]) {
				_ss >> token;
				if (token.at(token.size() - 1) == ';')
					token.at(token.size() - 1) = '\0';
				if (token.size() < 1) {
					std::cerr << "parseLocation: unexpected token of size 0" << std::endl;
					locStruct.route = "ERROR";
					Location	error(locStruct);
					return (error);
				}
				break ;
			}
		}
		// This switch statement should then check for ";" end line and handle errors
		// Maybe a return (handle location error) that returns an empty struct would be good
		switch (i)
		{
		case 0:
			locStruct.route = token;
			break;

		case 1:
			locStruct.root_path = token;
			break;

		case 2:
			locStruct.alias = token;
			break;

		case 3:
			locStruct.limit_except = parseLimitExcept(token);
			break;

		case 4:
			locStruct.autoindex = ft_stob(token);
			break;

		case 5:
			locStruct.upload_path = token;
			break;

		case 6:
			locStruct.cgi_param = parseCgiParam(token);
			break;

		case 7:
			locStruct.cgi_pass = token;
			break;

		default:
			Location	output(locStruct);
			return (output);
		}
	} while (token != "}" && _ss.good());

	Location	output(locStruct);
	return (output);
}
