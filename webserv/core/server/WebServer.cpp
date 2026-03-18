/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 14:11:12 by pberset           #+#    #+#             */
/*   Updated: 2026/01/25 15:17:15 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

WebServer::WebServer(const Configuration& config) : _config(config) {
    std::cout << "Config WebServer constructor" << std::endl;
	int					errnum;
	std::ostringstream	service;
	
	struct addrinfo hints;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_TCP;
	
	struct addrinfo*	addrinfo;
	
	for (size_t i = 0; i < _config.getServers().size(); i++) {
		service << _config.getServers()[i].getListens().front().port;
		errnum = getaddrinfo(_config.getServers()[i].getListens().front().ip.c_str(), service.str().c_str(), &hints, &addrinfo);
		if (errnum != 0) {
			std::cerr << "getaddrinfo: " << gai_strerror(errnum);
			freeaddrinfo(addrinfo);
			exitWithError("from WebServer", "WebServer constructor");
		}
		if (_initServer(addrinfo, &_config.getServers()[i]) != 0) {
			freeaddrinfo(addrinfo);
			exitWithError("WebServer constructor", "failed to initialize server");
		}
		std::cout << "Initialized server with IP: " <<_config.getServers()[i].getListens().front().ip << " | PORT: " << _config.getServers()[i].getListens().front().port << std::endl;
		service.str("");
		freeaddrinfo(addrinfo);
	}
}

WebServer::~WebServer(void) {
    std::cout << "WebServer destructor" << std::endl;
	closeAllSockets(_sockets);
	_closeServer();
}
