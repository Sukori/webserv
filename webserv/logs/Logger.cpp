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
	if (_logs.is_open()) {
		_logs.flush();
		_logs.close();
	}
	if (_logs.is_open()) {
		_logs.flush();
		_logs.close();
	}
}

void	Logger::init(const std::string& fileName) {
	if (!fileName.empty()) {
    	_logs.open(fileName.c_str(), std::ios::app);
	}
}

void Logger::log(const std::string& message) {
    _logs << message << "\n";
    _logs.flush();
}
