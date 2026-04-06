/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 14:36:55 by pberset           #+#    #+#             */
/*   Updated: 2026/02/18 11:34:08 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <vector>
# include <map>
# include <Configuration.hpp>
# include <helperParser.hpp>
# include <validatorConfig.hpp>

class Parser {
public:
	Parser(const std::string data);
	~Parser(void);

	std::vector<Server>					initParser(void);
	Server								parseServer(void);
	std::vector<std::string>			parseIndex(void);
	unsigned int						parseBodySize(void);
	std::pair<int, std::string>			parseErrorPage(std::string& token);
	std::map<int, std::string>			parseErrorPages(std::string& token);
	Location							parseLocation(void);
	struct s_listen						parseListen(std::string token);
	std::pair<int, std::string>			parseReturn(std::string& token);
	std::map<int, std::string>			parseReturns(std::string& token);
	std::set<std::string>				parseLimitExcept(std::string token);
	std::pair<std::string, std::string>	parseCgiParam(std::string& token);
	std::map<std::string, std::string>	parseCgiParams(std::string& token);

private:
	Parser(void);
	std::stringstream		_ss;
	std::vector<s_location>	_locations;
	std::vector<s_server>	_servers;
};

#endif
