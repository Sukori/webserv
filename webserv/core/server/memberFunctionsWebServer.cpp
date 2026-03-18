/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memberFunctionsWebServer.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 17:46:35 by pberset           #+#    #+#             */
/*   Updated: 2026/03/18 17:46:38 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

/// @brief Initializes the server from its constructor
/// @param addrinfo 
/// @param server 
/// @return int 0 if success or > 0 if error
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

/// @brief closes asll server sockets
/// @param  none
void	WebServer::_closeServer(void) {
	for (std::map<int, const Server*>::iterator it = _sockets.begin(); it != _sockets.end(); ++it) {
		close(it->first);
	}
}

/// @brief handle a client request and prepares a response
/// @param client 
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

/// @brief accepts a new connexion from a client
/// @param fd 
/// @return int the client socket or < 0 on error
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
