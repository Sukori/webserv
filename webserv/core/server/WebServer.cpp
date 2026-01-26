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
	_socketAddress.sin_port = htons(_config.getServers().front().getListens().front().port);
	_socketAddress.sin_addr.s_addr = inet_addr(_config.getServers().front().getListens().front().ip.c_str()); //not allowed, must find alt

	if (_initServer() != 0) {
		std::ostringstream ss;
		ss << "failed to initialize server with PORT: " << ntohs(_socketAddress.sin_port);
		putLog(ss.str());
	}
    std::cout << "Initialized server with IP: " <<_config.getServers().front().getListens().front().ip << " | PORT: " << _config.getServers().front().getListens().front().port << std::endl;
}

WebServer::~WebServer(void) {
    std::cout << "WebServer destructor" << std::endl;
	_closeServer();
}

int	WebServer::_initServer(void) {
    std::cout << "initServer" << std::endl;
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
	exit(0);
}

void	WebServer::_handleRequest(Client& client) {
	//parse request in
	// check config, file, cgi
	
	// for now: hello world
	std::string	body = "<html><body><h1>Hello from Poll Server!</h1></body></html>";

	std::ostringstream	oss;
	oss << "HTTP/1.1 200 OK\r\n"
	<< "Content-Type: text/html\r\n"
	<< "Content-Length: " << body.size() << "\r\n"
	<< "\r\n" //!! CR LF !! RFC 2.2, 4.1 - 19.3 tolerant only, but server doesn't know
	<< body;

	client.setResponse(oss.str());
}

void	WebServer::run(void) {

    std::cout << "run" << std::endl;
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
    std::cout << "===== Listening =====" << std::endl;
		ctrlno = poll(&_fds[0], _fds.size(), -1);
		if (ctrlno < 0) {
			exitWithError("poll", strerror(errno));
		} else if (ctrlno == 0) {
			continue ;
		}

		for (size_t i = 0; i < _fds.size(); i++) {
			if (_fds[i].revents == 0) {
				continue ;
			}
			if (_fds[i].fd == _socket) {
				int newSocket = _acceptConnection();
				if (newSocket > 0) {
					_clients.insert(std::make_pair(newSocket, Client(newSocket)));
					struct pollfd	npfd;
					npfd.fd = newSocket;
					npfd.events = POLLIN;
					npfd.revents = 0;
					_fds.push_back(npfd);

					putLog("New client");
				} else {
					continue ;
				}
			} else {
				std::map<int, Client>::iterator	it = _clients.find(_fds[i].fd);

				if (it != _clients.end()) {
					if (_fds[i].revents & POLLIN) {
						ssize_t read_bytes = it->second.readRequest();
	
							if (read_bytes <= 0) {
								close(_fds[i].fd);
								_clients.erase(it);
								_fds.erase(_fds.begin() + i);
								i--;
								continue ;
							}
	
							if (it->second.isRequestComplete()) {
								_handleRequest(it->second);
								_fds[i].events = POLLOUT;
							}

					} else if (_fds[i].revents & POLLOUT) {
						if (it->second.writeResponse()) { //for now, we close
							close(_fds[i].fd);
							_clients.erase(it);
							_fds.erase(_fds.begin() + i);
							i--;
							/*personal assumption
							 * it->second.events = POLLIN;
							 * clear the read write containers*/
						}
					}
				}
			}
		}
	}
}

int	WebServer::_acceptConnection(void) {

	socklen_t	addrLen = sizeof(_socketAddress);

	int newSocket = accept(_socket, (sockaddr *)&_socketAddress, &addrLen);
	if (newSocket < 0) {
		putLog("accept: " + std::string(strerror(errno)));
		return (-1);
	}
	
	int	ctrlno = fcntl(newSocket, F_SETFL, O_NONBLOCK);
	if (ctrlno < 0) {
		putLog("fcntl: " + std::string(strerror(errno)));
		close(newSocket);
		return (-2);
	}
	return (newSocket);
}

