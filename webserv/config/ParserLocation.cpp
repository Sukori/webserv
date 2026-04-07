/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserLocation.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 17:09:31 by pberset           #+#    #+#             */
/*   Updated: 2026/04/07 15:44:04 by ylabussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "validatorConfig.hpp"

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

/// @brief parses the CGI param
/// @param token 
/// @return pair of string - string, variable - value
std::pair<std::string, std::string>	Parser::parseCgiParam(std::string& token) {
	std::pair<std::string, std::string>	output;
	std::string							varName = token;	
	std::string							varVal;

	_ss >> varVal;
	if (varVal.empty() || !varVal.compare("}")) {
		std::cerr << "parseCgiParam: closed CGI Params block with an undefined page or end of file. Got " << varVal << std::endl;
		output = std::make_pair(ERR_STR, ERR_STR);
		return (output);
	} else if (!varVal.compare(";")) {
		std::cerr << "parseCgiParam: closed CGI Params definition without a path. Got " << varVal << std::endl;
		output = std::make_pair(ERR_STR, ERR_STR);
		return (output);
	}

	_ss >> token;
	if (token.compare(";")){
		std::cerr << "parseCgiParam: expected \";\". Got " << token << std::endl;
		output = std::make_pair(ERR_STR, ERR_STR);
		return (output);
	}
	output = std::make_pair(varName, varVal);

	return (output);
}

/// @brief parses the elements of the cgi_params block
/// @param token 
/// @return map string string, all the variable - value pairs
std::map<std::string, std::string>	Parser::parseCgiParams(std::string& token) {
	std::map<std::string, std::string>	output;
	
	if (_ss.fail() || token.compare("{")) {
		std::cerr << "parseCgiParams: unexpected opening of block. Got " << token << std::endl;
		output.insert(std::make_pair(ERR_STR, ERR_STR));
		return (output);
	}

	do {
		_ss >> token;
		if (token.empty() || _ss.fail() || _ss.eof()) {
			std::cerr << "parseCgiParams: unexpected end of stream " << std::endl;
			output.insert(std::make_pair(ERR_STR, ERR_STR));
			return (output);
		}
		if (!token.compare("}")){
			break ;
		}
		output.insert(parseCgiParam(token));

		if (output.find(ERR_STR) != output.end() && !(output.find(ERR_STR)->first).compare(ERR_STR)) {
			std::cerr << "from parseCgiParams"<< std::endl;
			return (output);
		}

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
	std::string			locationAllowed[] = {"return", "root", "alias", "limit_except", "autoindex", "upload_path", "cgi_param", "cgi_pass"};

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

		case 6:
			_ss >> token;
			locStruct.cgi_param = parseCgiParams(token);
			break;

		case 7:
			_ss >> token;
			locStruct.cgi_pass = token;
			break;

		default:
			std::cerr << "parseLocation: unexpected token. Got " << token << std::endl;
			Location	output(locStruct);
			return (output);
		}

		/* Do we really invalidate a location if the cgi params are wrong ?

		if (!locStruct.cgi_param.empty() && locStruct.cgi_param.find(ERR_STR) != locStruct.cgi_param.end() && !locStruct.cgi_param.find(ERR_STR)->first.compare(ERR_STR)) {
			std::cerr << "from parseLocation" << std::endl;
			locStruct.route = ERR_STR;
			Location	output(locStruct);
			return (output);
		}*/

		/* If we got no limit except, it defaults to GET in the validator
		
		if (!locStruct.limit_except.empty() && !locStruct.limit_except[0].compare(ERR_STR)) {
			std::cerr << "from parseLocation" << std::endl;
			locStruct.route = ERR_STR;
			Location	output(locStruct);
			return (output);
		}*/

	} while (!_ss.fail());

	validateLocation(locStruct);

	Location	output(locStruct);
	return (output);
}
