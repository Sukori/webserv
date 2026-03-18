/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserLocation.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 17:09:31 by pberset           #+#    #+#             */
/*   Updated: 2026/03/18 17:09:33 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "validatorConfig.hpp"

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
		output = std::make_pair("ERROR", "ERROR");
		return (output);
	} else if (!varVal.compare(";")) {
		std::cerr << "parseCgiParam: closed CGI Params definition without a path. Got " << varVal << std::endl;
		output = std::make_pair("ERROR", "ERROR");
		return (output);
	}

	_ss >> token;
	if (token.compare(";")){
		std::cerr << "parseCgiParam: expected \";\". Got " << token << std::endl;
		output = std::make_pair("ERROR", "ERROR");
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
		output.insert(std::make_pair("ERROR", "ERROR"));
		return (output);
	}

	do {
		_ss >> token;
		if (token.empty() || _ss.fail() || _ss.eof()) {
			std::cerr << "parseCgiParams: unexpected end of stream " << std::endl;
			output.insert(std::make_pair("ERROR", "ERROR"));
			return (output);
		}
		if (!token.compare("}")){
			break ;
		}
		output.insert(parseCgiParam(token));

		if (output.find("ERROR") != output.end() && !(output.find("ERROR")->first).compare("ERROR")) {
			std::cerr << "from parseCgiParams"<< std::endl;
			return (output);
		}

	} while(!_ss.fail() && token.compare("}"));
	
	return (output);
}

/// @brief parses the limitExcept parameter list
/// @param token 
/// @return vector of string for allowed methods. Defaults to "GET" if no config
std::vector<std::string>	Parser::parseLimitExcept(std::string token) {
	std::vector<std::string>	output;

	while (token != ";") {
		if (_ss.fail()) {
			std::cerr << "parseLimitExcept: unexpected end of line. Got " << token << std::endl;
			output.insert(output.begin(), std::string("ERROR"));
			return (output);
		}
		output.push_back(token);
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
	std::string			locationAllowed[] = {"route", "root", "alias", "limit_except", "autoindex", "upload_path", "cgi_param", "cgi_pass"};

	_ss >> token;
	if (token.empty()) {
		std::cerr << "parseLocation: unexpected end of stream" << std::endl;
		locStruct.route = "ERROR";
		Location	error(locStruct);
		return (error);
	}
	
	if (token.at(0) != '/') {
		std::cerr << "parseLocation: path not starting from root. Got " << token << std::endl;
		locStruct.route = "ERROR";
		Location	error(locStruct);
		return (error);
	}
	locStruct.route = token;
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
			_ss >> token;
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
			_ss >> token;
			locStruct.cgi_param = parseCgiParams(token);
			break;

		case 7:
			_ss >> token;
			locStruct.cgi_pass = token;
			break;

		default:
			std::cerr << "parseLocation: unexpected token. Got " << token << std::endl;
			locStruct.route = "ERROR";
			Location	output(locStruct);
			return (output);
		}

		if (!locStruct.cgi_param.empty() && locStruct.cgi_param.find("ERROR") != locStruct.cgi_param.end() && !locStruct.cgi_param.find("ERROR")->first.compare("ERROR")) {
			std::cerr << "from parseLocation" << std::endl;
			locStruct.route = "ERROR";
			Location	output(locStruct);
			return (output);
		}

		if (!locStruct.limit_except.empty() && !locStruct.limit_except[0].compare("ERROR")) {
			std::cerr << "from parseLocation" << std::endl;
			locStruct.route = "ERROR";
			Location	output(locStruct);
			return (output);
		}

	} while (!_ss.fail());

	validateLocation(locStruct);

	Location	output(locStruct);
	return (output);
}
