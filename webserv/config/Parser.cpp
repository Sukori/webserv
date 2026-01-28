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

Parser::Parser(void) {}

Parser::~Parser(void) {}

void	Parser::initParser(void) {
	//in case, otherwise just a step
	parseServer();
}

void	Parser::parseServer(void) {
	//token
	parseLocation();
	//once done parsing one server, push back in Configuration::_servers
}

void	Parser::parseLocation(void) {
	//token
	//Once done parsing one location, puch_back in Server::_locations
}