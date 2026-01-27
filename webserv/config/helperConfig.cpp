/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helperConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:55:22 by pberset           #+#    #+#             */
/*   Updated: 2026/01/06 18:06:24 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "helperConfig.hpp"

HelperConfig::HelperConfig(void) {}

HelperConfig::~HelperConfig(void) {}

void	HelperConfig::initParser(void) {
	//in case, otherwise just a step
	parseServer();
}

void	HelperConfig::parseServer(void) {
	//token
	parseLocation();
	//once done parsing one server, push back in Configuration::_servers
}

void	HelperConfig::parseLocation(void) {
	//token
	//Once done parsing one location, puch_back in Server::_locations
}

std::string	readFile(const char* file) {
	std::ifstream	fs(file);
	if (!fs.is_open()) {
		std::cerr << "file: failed to open" << std::endl;
		return ("");
	}
	
	std::ostringstream	ss;
	std::string			output;

	ss << fs.rdbuf();
	output = ss.str();
	return (output);
}

std::string	filterComments(std::string& str) {
	std::string			output;
	std::istringstream	is(str);
	std::string			line;

	while (std::getline(is, line)) {

		for (unsigned int i = 0; i < line.size(); i++) {
			if (line[i] != '#') {
				output += line[i];
			}
			else
				break;
		}
		output += '\n';
		line.clear();
	}
	return (output);
}

std::string	insertSpaces(std::string& str) {
	std::string	output;
	
	for (unsigned int i = 0; i < str.size(); i++) {
		if (str[i] == '{' || str[i] == '}' || str[i] == ';') {
				output += " ";
				output += str[i];
				output += " ";
		} else {
			output += str[i];
		}
	}

	return (output);
}

