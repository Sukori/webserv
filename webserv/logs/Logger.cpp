/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 11:22:42 by pberset           #+#    #+#             */
/*   Updated: 2026/04/15 11:23:46 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

Logger::Logger(void){}

Logger::~Logger() {
}

void	Logger::init(const std::string& fileName) {
	(void)fileName; // unused
}

void Logger::log(const std::string& message) {
	std::ostringstream os;
	time_t	log = time(NULL);
	std::string	date = ctime(&log);
	if (date.at(date.size() - 1) == '\n')
		date = date.erase(date.size() - 1);
	os << '[' << date << "] " << message << '\n';
	std::cout << os.str();
}
