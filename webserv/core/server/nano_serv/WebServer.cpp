/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 14:11:12 by pberset           #+#    #+#             */
/*   Updated: 2026/01/19 14:11:19 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

//_clients.insert(std::make_pair(new_socket_fd, Client(new_socket_fd)));
#include "WebServer.hpp"
#include "../../../config/Configuration.hpp" 

void	putLog(const std::string& message) {
	std::cout << message << std::endl;
}

void	exitWithError(const std::string& funct, const std::string& message) {
	putLog(funct +": " + message);
	exit(1);
}

WebServer::WebServer(const Configuration& config) : _config(config) {
    std::cout << "Config WebServer constructor" << std::endl;
	
	_socketAddress.sin_family = AF_INET;
	_socketAddress.sin_port = htons(_config._servers[0]._listen[0].port);
	_socketAddress.sin_addr.s_addr = inet_addr(_config._servers[0]._listen[0].ip.c_str()); //not allowed, must find alt

	if (_initServer() != 0) {
		std::ostringstream ss;
		ss << "failed to initialize server with PORT: " << ntohs(_socketAddress.sin_port);
		putLog(ss.str());
	}
}

WebServer::~WebServer(void) {
    std::cout << "WebServer destructor" << std::endl;
	_closeServer();
}

int	WebServer::_initServer(void) {
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0) {
		exitWithError("_initServer", "create socket failed");
		return (1);
	}
	if (bind(_socket, (sockaddr *)&_socketAddress, sizeof(_socketAddress)) < 0) {
		exitWithError("_initServer", "failed to bind socket");
		return (2);
	}
	return (0);
}

int	WebServer::_closeServer(void) {
	close(_socket);
	close(_newSocket);
	exit(0);
}

void	WebServer::run(void) {

	int	ctrlno;

	ctrlno = listen(_socket, BACKLOG);
	if (ctrlno < 0) {
		exitWithError("listen", strerror(errno));
	}

	_fds.clear();
	struct pollfd	pfd;
	pfd.fd = _socket;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_fds.push_back(pfd);

	while (true) {
		ctrlno = poll(&_fds[0], _fds.size(), -1);
		if (ctrlno < 0) {
			exitWithError("poll", strerror(errno));
		} else if (ctrlno == 0) {
			continue ;
		}

		for (int i = 0; i < _fds.size(); i++) {
			if (_fds[i].revents == 0) {
				continue ;
			}
			if (_fds[i].fd == _socket) {
				_acceptConnection(_newSocket);
			} else {
				//client.readRequest();
				std::cout << "existing client is reading" << std::endl;
			}
		}
	}
}

void	WebServer::_acceptConnection(int& newSocket) {
	newSocket = accept(_socket, (sockaddr *)&_socketAddress, (socklen_t *)sizeof(_socketAddress));
	if (newSocket < 0) {
		putLog("accept: " + std::string(strerror(errno)));
		return ;
	}
}

//int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
//int poll(struct pollfd *fds, nfds_t nfds, int timeout);
