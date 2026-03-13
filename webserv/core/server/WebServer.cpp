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

void	closeAllSockets(std::map<int, const Server*> sockets) {
	for (std::map<int, const Server*>::iterator it = sockets.begin(); it != sockets.end(); ++it) {
		close(it->first);
	}
}

void	closeAllPollFds(std::vector<pollfd> fds) {
	for (size_t i = 0; i < fds.size(); i++) {
		close(fds[i].fd);
	}
}

void	exitWithError(const std::string& funct, const std::string& message) {
	putLog(funct +": " + message);
	exit(1);
}
\

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

int	WebServer::_initServer(const struct addrinfo* addrinfo, const Server* server) {
    std::cout << "initServer" << std::endl;

	int sockBuff = socket(AF_INET, SOCK_STREAM, 0);
	if (sockBuff < 0) {
		std::cerr << "create socket failed" << std::endl;
		closeAllSockets(_sockets);
		exitWithError("_initServer", server->getName());
	}

	int	ctrlno = fcntl(sockBuff, F_GETFL);
	if (ctrlno < 0) {
		putLog("fcntl(GET): " + std::string(strerror(errno)));
		close(sockBuff);
		closeAllSockets(_sockets);
		exitWithError("_initServer", server->getName());
	}
	ctrlno = fcntl(sockBuff, F_SETFL, ctrlno | O_NONBLOCK);
	if (ctrlno < 0) {
		putLog("fcntl(SET): " + std::string(strerror(errno)));
		close(sockBuff);
		closeAllSockets(_sockets);
		exitWithError("_initServer", server->getName());
	}

	int optval = 1;
	ctrlno = setsockopt(sockBuff, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
	if (ctrlno < 0) {
		putLog("setsockopt: " + std::string(strerror(errno)));
		close(sockBuff);
		closeAllSockets(_sockets);
		exitWithError("_initServer", server->getName());
	}

	_sockets.insert(std::pair<int, const Server*>(sockBuff, server));
	if (bind(sockBuff, addrinfo->ai_addr, addrinfo->ai_addrlen) < 0) {
		std::cerr << "bind socket failed" << std::endl;
		closeAllSockets(_sockets);
		exitWithError("_initServer", server->getName());
	}
	return (0);
}

void	WebServer::_closeServer(void) {
	for (std::map<int, const Server*>::iterator it = _sockets.begin(); it != _sockets.end(); ++it) {
		close(it->first);
	}
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

int	WebServer::_acceptConnection(int fd) {

	int newSocket = accept(fd, NULL, NULL);
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

void	WebServer::run(void) {

    std::cout << "run" << std::endl;
	int	ctrlno;
	_fds.clear();

	//listen to each sockets and populate the pollfd struct foreach sockets
	for (std::map<int, const Server*>::iterator it = _sockets.begin(); it != _sockets.end(); ++it) {
		ctrlno = listen(it->first, BACKLOG);
		if (ctrlno < 0) {
			exitWithError("listen", strerror(errno));
		}
		struct pollfd	pfd;
		pfd.fd = it->first;
		pfd.events = POLLIN;
		pfd.revents = 0;
		_fds.push_back(pfd);
	}

	//Main loop
	while (true) {
    std::cout << "===== Listening =====" << std::endl;
		ctrlno = poll(&_fds[0], _fds.size(), -1);
		if (ctrlno < 0) {
			closeAllSockets(_sockets);
			closeAllPollFds(_fds);
			exitWithError("poll", strerror(errno));
		} else if (ctrlno == 0) {
			continue ;
		}

		for (size_t i = 0; i < _fds.size(); i++) {
			if (_fds[i].revents == 0) {
				continue ;
			}
			if (_sockets.find(_fds[i].fd) != _sockets.end()) {
				int newSocket = _acceptConnection(_fds[i].fd);
				if (newSocket > 0) {
					_clients.insert(std::pair<int, Client>(newSocket, Client(newSocket)));
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
								std::cout << it->second.getRequestIn() << std::endl;
								std::cout << (int)_socketAddress.sin_port << std::endl;
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
