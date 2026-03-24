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
	std::vector<Server>	servers = _config.getServers();
	
	struct addrinfo hints;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_TCP;
	
	struct addrinfo*	addrinfo;
	
	for (size_t i = 0; i < servers.size(); i++) {
		service << servers[i].getListen().port;
		errnum = getaddrinfo(servers[i].getListen().ip.c_str(), service.str().c_str(), &hints, &addrinfo);
		service.str("");
		if (errnum != 0) {
			std::cerr << "WebServer constructor getaddrinfo: " << gai_strerror(errnum) << std::endl
			<< "skipped " << servers[i].getName() << std::endl;
			freeaddrinfo(addrinfo);
			continue ;
		}
		if (_initServer(addrinfo, &servers[i]) != 0) {
			freeaddrinfo(addrinfo);
			std::cerr << "WebServer constructor _initServer: failed init" << std::endl
			<< "skipped " << servers[i].getName() << std::endl;
			continue ;
		}
		std::cout << "Initialized server with IP: " <<servers[i].getListen().ip << " | PORT: " << servers[i].getListen().port << std::endl;
		freeaddrinfo(addrinfo);
	}
}

WebServer::~WebServer(void) {
    std::cout << "WebServer destructor" << std::endl;
	closeAllSockets(_sockets);
	_closeServer();
}
