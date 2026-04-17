/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 17:09:15 by pberset           #+#    #+#             */
/*   Updated: 2026/04/15 16:01:58 by ylabussi         ###   ########.fr       */
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

/// @brief parses the CGI bin
/// @param token 
/// @return pair of string - string, variable - value
std::pair<std::string, std::string>	Parser::parseCgiBin(std::string& token) {
	std::pair<std::string, std::string>	output;
	std::string							varName = token;	
	std::string							varVal;

	_ss >> varVal;
	if (varVal.empty() || !varVal.compare("}")) {
		std::cerr << "parseCgiBin: closed CGI Bins block with an undefined page or end of file. Got " << varVal << std::endl;
		output = std::make_pair(ERR_STR, ERR_STR);
		return (output);
	} else if (!varVal.compare(";")) {
		std::cerr << "parseCgiBin: closed CGI Bins definition without a path. Got " << varVal << std::endl;
		output = std::make_pair(ERR_STR, ERR_STR);
		return (output);
	}

	_ss >> token;
	if (token.compare(";")){
		std::cerr << "parseCgiBin: expected \";\". Got " << token << std::endl;
		output = std::make_pair(ERR_STR, ERR_STR);
		return (output);
	}
	output = std::make_pair(varName, varVal);
	return (output);
}

/// @brief parses the elements of the cgi_bin block
/// @param token 
/// @return map string string, all the variable - value pairs
std::map<std::string, std::string>	Parser::parseCgiBins(std::string& token) {
	std::map<std::string, std::string>	output;
	
	if (_ss.fail() || token.compare("{")) {
		std::cerr << "parseCgiBins: unexpected opening of block. Got " << token << std::endl;
		output.insert(std::make_pair(ERR_STR, ERR_STR));
		return (output);
	}

	do {
		_ss >> token;
		if (token.empty() || _ss.fail() || _ss.eof()) {
			std::cerr << "parseCgiBins: unexpected end of stream " << std::endl;
			output.insert(std::make_pair(ERR_STR, ERR_STR));
			return (output);
		}
		if (!token.compare("}")){
			break ;
		}
		output.insert(parseCgiBin(token));

		if (output.find(ERR_STR) != output.end() && !(output.find(ERR_STR)->first).compare(ERR_STR)) {
			std::cerr << "from parseCgiBins"<< std::endl;
			return (output);
		}

	} while(!_ss.fail() && token.compare("}"));
	
	return (output);
}

/// @brief parses tokens for the "listen" parameter
/// @param token
/// @return struct s_listen
struct s_listen	Parser::parseListen(std::string token) {
	struct s_listen	output;
	size_t			separatorPos = 0;
	
	output.protocol = "HTML";
	_ss >> token;
	if ((separatorPos = token.find(':')) == std::string::npos) {
		std::cerr << "parseListen: No separator found between interface and port. expected \':\'" << std::endl;
		std::cerr << "\tcritical error in config, set -127. Please review your config!!!" << std::endl;
		output.ip = "-127.0.0.1";
	} else {
		output.ip = token.substr(0, separatorPos);
	}

	std::string s_port = token.substr(separatorPos + 1);
	
	if (!ft_isNUM(s_port)) {
		std::cerr << "from parseListen" << std::endl;
		output.port = -1;
		return (output);
	}
	
	unsigned int port = ft_stoui(s_port);
	output.port = port;
	return (output);
}

/// @brief parses the tokens found int the server block
/// @param  none
/// @return Server object
Server	Parser::parseServer(void) {
	struct s_server			servStruct;
	std::vector<Location>	locs;
	std::string				token;
	std::string				serverAllowed[] = {"server_name", "listen", "root", "index", "client_max_body_size", "error_pages", "location", "cgi_bin"};

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
				servStruct.root = "." + token;
				break;

			case 3:
				servStruct.index = parseIndex();
				break;

			case 4:
				servStruct.client_max_body_size = parseBodySize();
				break;

			case 5:
				_ss >> token;
				servStruct.error_pages = parseErrorPages(token);
				break;

			case 6:
				locs.push_back(parseLocation(servStruct.root));
				break;

			case 7:
				_ss >> token;
				servStruct.cgi_bins = parseCgiBins(token);
				break;

			default:
				std::cerr << "parseServer: unexpected token. Got " << token << std::endl;
				Server	output(servStruct, locs);
				return (output);
		}

	} while (!_ss.fail());

	trimLastSlash(servStruct.root);
	std::cout << "Parsed server " << servStruct.serverName << "\n" << std::endl;
	servStruct.access_logs = servStruct.root + servStruct.access_logs;
	servStruct.error_logs = servStruct.root + servStruct.error_logs;
	validateServer(servStruct);
	Server	output(servStruct, locs);

	return (output);
}
