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

	if (listen(_socket, 20) < 0) {
		exitWithError("run", "listen failed");
	}
	std::ostringstream ss;
	ss << "Listening on ADDRESS: " << inet_ntoa(_socketAddress.sin_addr) //not allowed must fnd alt
	<< " | PORT: " << ntohs(_socketAddress.sin_port);
	putLog(ss.str());

	_fds[0].fd = _socket;
	_fds[0].events = POLLIN;
	int	timeout = 3 * 60 * 1000;
	ssize_t	bytesReceived;

	do {
		putLog("=====LISTENING=====");
		_acceptConnection(_newSocket);
		Client	client(_newSocket);
		_clients.insert(std::pair<int,Client>(_newSocket,client));

		char	buffer[BUFFER_SIZE] = {0};

		bytesReceived = _clients[0].readRequest();
	} while (_newSocket != -1)
}

void	WebServer::_acceptConnection(int& newSocket) {
	newSocket = accept(_socket, (sockaddr *)&_socketAddress, (socklen_t *)sizeof(_socketAddress));
	if (newSocket < 0) {
		std::ostringstream oss;

		oss << "server failed to accept connection on ADDRESS: " << inet_ntoa(_socketAddress.sin_addr) //not allowed must fnd alt
		<< " | PORT: " << ntohs(_socketAddress.sin_port);
		exitWithError("acceptConnection", oss.str());
	}
}

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
