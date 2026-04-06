/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 17:09:15 by pberset           #+#    #+#             */
/*   Updated: 2026/04/02 20:18:56 by ylabussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Parser.hpp>
#include <validatorConfig.hpp>

/// @brief parses an error page code and path
/// @param token 
/// @return pair of error number and path to error page
std::pair<int, std::string>	Parser::parseErrorPage(std::string& token) {
	std::pair<int, std::string>	output;
	unsigned int	errNum;	
	std::string	errPagePath;

	if (!ft_isNUM(token)) {
		std::cerr << "parseErrorPages: error number is not an unsigned int. Got " << token << std::endl;
		output = std::make_pair(0, ERR_STR);
		return (output);
	} else {
		errNum = ft_stoui(token);
	}
	_ss >> errPagePath;
	if (errPagePath.empty() || !errPagePath.compare("}")) {
		std::cerr << "parseErrorPages: closed error_pages block with an undefined page or end of file. Got " << errPagePath << std::endl;
		output = std::make_pair(0, ERR_STR);
		return (output);
	} else if (!errPagePath.compare(";")) {
		std::cerr << "parseErrorPages: closed error_pages definition without a path. Got " << errPagePath << std::endl;
		output = std::make_pair(0, ERR_STR);
		return (output);
	}

	_ss >> token;
	if (token.compare(";")){
		std::cerr << "parseErrorPages: expected \";\". Got " << token << std::endl;
		output = std::make_pair(0, ERR_STR);
		return (output);
	}
	output = std::make_pair(errNum, errPagePath);

	return (output);
}

/// @brief parses an error_pages block
/// @param token 
/// @return map of error numbers and pages paths
std::map<int, std::string>	Parser::parseErrorPages(std::string& token) {
	std::map<int, std::string>	output;
	
	if (_ss.fail() || token.compare("{")) {
		std::cerr << "parseErrorPages: unexpected opening of block. Got " << token << std::endl;
		output.insert(std::make_pair(0, ERR_STR));
		return (output);
	}

	do {
		_ss >> token;
		if (!token.compare("}")){
			break ;
		}
		output.insert(parseErrorPage(token));

	} while(!_ss.fail() && token.compare("}"));
	
	return (output);
}

/// @brief parses the client_max_body_size parameter
/// @param  none
/// @return unsigned int size
unsigned int	Parser::parseBodySize(void) {
	unsigned int	output;
	std::string		token;

	_ss >> token;
	char	sizeUnit = '\0';
	if (token.at(token.size() - 1) < '0' || token.at(token.size() - 1) > '9') {
		sizeUnit = token.at(token.size() - 1);
		ft_toLowerChr(&sizeUnit);
		if (sizeUnit != 'k' && sizeUnit != 'm') {
			std::cerr << "parseBodySize: invalid body size unit. Got " << sizeUnit << ". Expect none, 'k' or 'm'" << std::endl;
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

/// @brief parses the indexes found in the stringstream
/// @param  none
/// @return vector of strings
std::vector<std::string>	Parser::parseIndex(void) {
	std::vector<std::string>	output;
	std::string					token;

	_ss >> token;
	while (token != ";" && !_ss.fail()){
		output.push_back(token);
		_ss >> token;
	}

	return (output);
}

/// @brief parses tokens for the "listen" parameter
/// @param token
/// @return struct s_listen
struct s_listen	Parser::parseListen(std::string token) {
	struct s_listen	output;
	unsigned int	port;

	output.ip = "127.0.0.1";
	output.protocol = "HTML";
	_ss >> token;

	if (!ft_isNUM(token)) {
		std::cerr << "from parseListen" << std::endl;
		output.port = -1;
		return (output);
	}
	port = ft_stoui(token);
	output.port = port;
	return (output);
}

/* idk where to put this */
static std::string&	trimLastSlash(std::string& s) {
	if (!s.empty() && s[s.length() - 1] == '/')
		s.erase(s.length() - 1);
	return s;
}

/// @brief parses the tokens found int the server block
/// @param  none
/// @return Server object
Server	Parser::parseServer(void) {
	struct s_server			servStruct;
	std::vector<Location>	locs; //not initialized at 234 ...
	std::string				token;
	std::string				serverAllowed[] = {"server_name", "listen", "root", "index", "access_logs", "error_logs", "client_max_body_size", "error_pages", "location"};

	servStruct.valid = false;
	_ss >> token;
	if (token.compare("{")) {
		std::cerr << "parseServer: unexpected token. Got " << token << std::endl;
		servStruct.serverName = ERR_STR;
		return (Server(servStruct,locs));
	}
	
	do {
		_ss >> token;
		if (token.empty() || _ss.fail() || _ss.eof()) {
			std::cerr << "parseServer: unexpected end of stream" << std::endl;
			servStruct.serverName = ERR_STR;
			Server	output(servStruct, locs);
			return (output);
		}

		if (!token.compare("}")) {
			break ;
		}
		
		int	i;
		for (i = 0; (unsigned int)i < sizeof(serverAllowed) / sizeof(std::string); i++) {
			if (!token.compare(serverAllowed[i])) {
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
				servStruct.root = "." + trimLastSlash(token); // need to remove trailing '/' if there is one
				break;

			case 3:
				servStruct.index = parseIndex();
				break;

			case 4:
				_ss >> token;
				servStruct.access_logs = "." + token;
				break;

			case 5:
				_ss >> token;
				servStruct.error_logs = "." + token;
				break;

			case 6:
				servStruct.client_max_body_size = parseBodySize();
				break;

			case 7:
				_ss >> token;
				servStruct.error_pages = parseErrorPages(token);
				break;

			case 8:
				locs.push_back(parseLocation());
				break;

			default:
				std::cerr << "parseServer: unexpected token. Got " << token << std::endl;
				Server	output(servStruct, locs);
				return (output);
		}

	} while (!_ss.fail());

	validateServer(servStruct);

	Server	output(servStruct, locs);

	return (output);
}
