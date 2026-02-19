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

#include "Parser.hpp"

Parser::Parser(const std::string data): _ss(data) {}

Parser::~Parser(void) {}

static unsigned int	ft_stoui(std::string& token) {
	int	output = 0;

	for (unsigned int i = 0; i < token.size(); i++) {
		output = output * 10 + (token.at(i) - '0');
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

static char	ft_toLowerChr(char c) {
	if (c > 64 && c < 91) {
		return (c += 32);
	}
	return (0);
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

static bool	ft_isNUM(std::string token) {
	for (size_t i = 0; i < token.length(); i++) {
		if (token.at(i) < '0' || token.at(i) > '9') {
			std::cerr << "ft_isNUM: token " << token << " is not a valid numeral string" << std::endl;
			return (false);
		}
	}
	return (true);
}

struct s_listen	Parser::parseListen(std::string token) {
	struct s_listen	output;
	int				port;

	//ip and port are the only data.
	//for simplicity, ip is assumed to be 0.0.0.0 and is not written in config
	output.ip = "0.0.0.0";
	output.protocol = "HTML";
	_ss >> token;
	//first int for port - else error handling
	if (!ft_isNUM(token)) {
		std::cerr << "from parseListen" << std::endl;
		output.port = -1;
		return (output);
	}
	port = ft_stoui(token);
	output.port = port;
	return (output);
}

std::vector<std::string>	Parser::parseIndex(void) {
	std::vector<std::string>	output;
	std::string					token;

	do { 
		_ss >> token;
		output.push_back(token);
	} while (token.at(token.size() - 1) != ';' && _ss.good());

	return (output);
}

unsigned int	Parser::parseBodySize(void) {
	unsigned int	output;
	std::string		token;

	_ss >> token;

	char	sizeUnit;
	if (token.at(token.size() - 1) < '0' && token.at(token.size() - 1) > '9') {
		sizeUnit = token.at(token.size() - 1);
		sizeUnit = ft_toLowerChr(sizeUnit);
		if (sizeUnit != 'k' && sizeUnit != 'm') {
			std::cerr << "parseBodySize: invalid body size unit " << sizeUnit << ". Expect none, 'k' or 'm'" << std::endl;
			return (0);
		}
		token.erase(token.end() - 1);
	}

	if (!ft_isNUM(token)) {
		std::cerr << "from parseBodySize" << std::endl;
		return (0);
	}
	
	output = ft_stoui(token);

	if (sizeUnit == 'k')
		output *= 1000;
	else if (sizeUnit == 'm')
		output *= 1000000;

	return (output);
}

std::map<int, std::string>	Parser::parseErrorPages(void) {
	std::map<int, std::string>	output;
	std::string					errNum;
	std::string					errPagePath;

	_ss >> errNum;
	if (errNum != "{"){
		std::cerr << "parseErrorPages: unexpected token " << errNum << std::endl;
		std::map<int, std::string>	fail;
		fail.insert(fail.begin(), std::make_pair(-1, "ERROR"));
		return (fail);
	}

	//space separated number and path
	//space is also a separator for each pair
	//always extract two from stream
	do {
	_ss >> errNum;
	if (errNum == "}")
		break ;
	_ss >> errPagePath;
	if (errPagePath == "}") {
		std::cerr << "parseErrorPages: closed error_pages block with an undefined page " << errNum << std::endl;
		std::map<int, std::string>	fail;
		fail.insert(fail.begin(), std::make_pair(-1, "ERROR"));
		return (fail);
	}

	if (!ft_isNUM(errNum)) {
		std::cerr << "from parseErrorPages" << std::endl;
		std::map<int, std::string>	fail;
		fail.insert(fail.begin(), std::make_pair(-1, "ERROR"));
		return (fail);
	}

	output.insert(output.end(), std::make_pair(ft_stoui(errNum), errPagePath));

	} while (_ss.good());

	return (output);
}

Server	Parser::parseServer(void) {
	struct s_server			servStruct;
	std::vector<Location>	locs;
	std::string				token;
	std::string				serverAllowed[] = {"server_name", "listen", "root", "index", "access_logs", "error_logs", "client_max_body_size", "error_pages", "location"};

	_ss >> token;
	if (token != "{") {
		std::cerr << "parseServer: unexpected token " << token << std::endl;
		servStruct.serverName = "ERROR";
		return (Server(servStruct,locs));
	}

	do {
		_ss >> token;
		if (token == "}")
			break ;
		
		int	i;
		for (i = 0; (unsigned int)i < sizeof(serverAllowed) / sizeof(std::string); i++) {
			if (token == serverAllowed[i]) {
				break ;
			}
		}
		switch (i)
		{
		case 0:
			_ss >> token;
			servStruct.serverName = token;
			break;

		case 1:
			servStruct.listen = parseListen(token);
			break;

		case 2:
			_ss >> token;
			servStruct.root = token;
			break;

		case 3:
			servStruct.index = parseIndex();
			break;

		case 4:
			_ss >> token;
			servStruct.access_logs = token;
			break;

		case 5:
			_ss >> token;
			servStruct.error_logs = token;
			break;

		case 6:
			servStruct.client_max_body_size = parseBodySize();
			break;

		case 7: //the full error page map could be filled with defaults and we replace specifically requested from config
			servStruct.error_pages = parseErrorPages();
			break;

		case 8:
			locs.push_back(parseLocation());
			break;

		default:
			servStruct.serverName = "ERROR";
			Server	output(servStruct, locs);
			return (output);
		}

	} while (_ss.good());
	//if locs & listen are not empty - else error handling
	Server	output(servStruct, locs);

	return (output);
}

std::vector<Server> Parser::initParser(void) {
    std::vector<Server> output;
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
		token.erase(token.end() - 1);
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

	std::string	key = token;
	std::string	value;

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
		if (token == "}")
			break ;
			
		for (i = 0; (unsigned int)i < sizeof(locationAllowed) / sizeof(std::string); i++) {
			if (token == locationAllowed[i]) {
				break ;
			}
		}
		// This switch statement should then check for ";" end line and handle errors
		// Maybe a return (handle location error) that returns an empty struct would be good
		switch (i)
		{
		case 0:
			_ss >> token;
			locStruct.route = token;
			break;

		case 1:
			_ss >> token;
			locStruct.root_path = token;
			break;

		case 2:
			_ss >> token;
			locStruct.alias = token;
			break;

		case 3:
			locStruct.limit_except = parseLimitExcept(token);
			break;

		case 4:
			_ss >> token;
			locStruct.autoindex = ft_stob(token);
			break;

		case 5:
			_ss >> token;
			locStruct.upload_path = token;
			break;

		case 6:
			locStruct.cgi_param = parseCgiParam(token);
			break;

		case 7:
			_ss >> token;
			locStruct.cgi_pass = token;
			break;

		default:
			locStruct.route = "ERROR";
			Location	output(locStruct);
			return (output);
		}
	} while (_ss.good());

	Location	output(locStruct);
	return (output);
}

