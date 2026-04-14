/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neon-05 <neon-05@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 14:11:12 by pberset           #+#    #+#             */
/*   Updated: 2026/04/14 17:36:35 by neon-05          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <WebServer.hpp>

volatile sig_atomic_t	WebServer::_stopRequested = 0;

void	WebServer::_handleSignal(int sig) {
	if (sig == SIGINT || sig == SIGTERM) {
		_stopRequested = 1;
	} else if (sig == SIGPIPE) {
		std::cout << "send(): Connection closed\n";
	}
}

void	WebServer::installSignalHandlers(void) {
	struct sigaction	sa;

	std::memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &WebServer::_handleSignal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGPIPE, &sa, NULL);
}

WebServer::WebServer(const Configuration& config) : _config(config) {
    std::cout << "Config WebServer constructor" << std::endl;
	int							errnum;
	std::ostringstream			service;
	const std::vector<Server>&	servers = _config.getServers();
	
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
	closeAllSockets(_serverSockets);
	_closeServer();
}
