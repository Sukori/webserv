/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserLocation.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 17:09:31 by pberset           #+#    #+#             */
/*   Updated: 2026/04/07 18:59:57 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <Parser.hpp>
#include <validatorConfig.hpp>

/// @brief parses a return code and redirection path
/// @param token 
/// @return pair of return code and a redirection path
std::pair<int, std::string>	Parser::parseReturn(std::string& token) {
	std::pair<int, std::string>	output;
	unsigned int				retCode;	
	std::string					retPath;

	if (ft_isNUM(token)) {
		retCode = ft_stoui(token);
		_ss >> retPath;
	} else {
		std::cerr << "parseReturn: set default redirection code " << DFT_REDIR << std::endl;
		retCode = DFT_REDIR;
		retPath = token;
	}

	if (retPath.empty() || !retPath.compare("}")) {
		std::cerr << "parseReturn: closed return block with an undefined page or end of file. Got " << retPath << std::endl;
		output = std::make_pair(EN_REDIR, ERR_STR);
		return (output);
	} else if (!retPath.compare(";")) {
		std::cerr << "parseReturn: closed return definition without a path. Got code " << retCode << std::endl
		<< "Please ensure this is what you intend to do" << std::endl;
		output = std::make_pair(retCode, "");
		return (output);
	}

	_ss >> token;
	if (token.compare(";")){
		std::cerr << "parseReturn: expected \";\". Got " << token << std::endl;
		output = std::make_pair(EN_REDIR, ERR_STR);
		return (output);
	}

	output = std::make_pair(retCode, retPath);

	return (output);
}

/// @brief parses a return block
/// @param token 
/// @return map of return codes and redirection paths
std::map<int, std::string>	Parser::parseReturns(std::string& token) {
	std::map<int, std::string>	output;
	
	if (_ss.fail() || token.compare("{")) {
		std::cerr << "parseReturns: unexpected opening of block. Got " << token << std::endl;
		output.insert(std::make_pair(EN_REDIR, ERR_STR));
		return (output);
	}

	do {
		_ss >> token;
		if (!token.compare("}")){
			break ;
		}
		output.insert(parseReturn(token));

	} while(!_ss.fail() && token.compare("}"));

	return (output);
}

/// @brief parses the limitExcept parameter list
/// @param token 
/// @return vector of string for allowed methods. Defaults to "GET" if no config
std::set<std::string>	Parser::parseLimitExcept(std::string token) {
	std::set<std::string>	output;

	while (token != ";") {
		if (_ss.fail()) {
			std::cerr << "parseLimitExcept: unexpected end of line. Got " << token << std::endl;
			output.insert(output.begin(), std::string(ERR_STR));
			return (output);
		}
		output.insert(token);
		_ss >> token;
	}
	return (output);
}

/// @brief parses a location block
/// @param  none
/// @return Location object
Location	Parser::parseLocation(void) {
	struct s_location	locStruct;
	std::string			token;
	std::string			locationAllowed[] = {"return", "root", "alias", "limit_except", "autoindex", "upload_path"};

	locStruct.valid = false;
	_ss >> token;
	if (token.empty()) {
		std::cerr << "parseLocation: unexpected end of stream" << std::endl;
		locStruct.route = ERR_STR;
		Location	error(locStruct);
		return (error);
	}
	
	if (token.at(0) != '/') {
		std::cerr << "parseLocation: route not starting from '/'. Got " << token << std::endl;
		locStruct.route = ERR_STR;
		Location	error(locStruct);
		return (error);
	}

	locStruct.route = trimLastSlash(token);
	_ss >> token;
	
	if (token.empty() || token.at(0) != '{') {
		std::cerr << "parseLocation: unexpected token after route. Got " << token << std::endl;
		Location	error(locStruct);
		return (error);
	}
	
	int	i;
	do {
		_ss >> token;
		if (token.empty()) {
			std::cerr << "parseLocation: empty token\nExit" << std::endl;
			break;
		}
		if (!token.compare("}")) {
			break ;
		}
			
		for (i = 0; (unsigned int)i < sizeof(locationAllowed) / sizeof(std::string); i++) {
			if (!token.compare(locationAllowed[i])) {
				break ;
			}
		}

		switch (i)
		{
		case 0:
			_ss >> token;
			locStruct.locReturn = parseReturns(token);
			break;

		case 1:
			_ss >> token;
			locStruct.root_path = trimLastSlash(token); // no need to prepend '.' here
			break;

		case 2:
			_ss >> token;
			locStruct.alias = token;
			break;

		case 3:
			_ss >> token;
			locStruct.limit_except = parseLimitExcept(token);
			break;

		case 4:
			_ss >> token;
			locStruct.autoindex = ft_stob(token);
			break;

		case 5:
			_ss >> token;
			locStruct.upload_path = "." + token;
			break;

		default:
			std::cerr << "parseLocation: unexpected token. Got " << token << std::endl;
			Location	output(locStruct);
			return (output);
		}
	} while (!_ss.fail());

	std::cout << "Parsed Location " << locStruct.route << "\n" << std::endl;

	validateLocation(locStruct);

	Location	output(locStruct);
	return (output);
}
