/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memberFunctionsWebServer.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 17:46:35 by pberset           #+#    #+#             */
/*   Updated: 2026/04/01 19:16:20 by ylabussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <WebServer.hpp>

/// @brief Initializes the server from its constructor
/// @param addrinfo 
/// @param server 
/// @return int 0 if success or > 0 if error
int	WebServer::_initServer(const struct addrinfo* addrinfo, const Server* server) {
    std::cout << "init " + server->getName() << std::endl;

	int sockBuff = socket(AF_INET, SOCK_STREAM, 0);
	if (sockBuff < 0) {
		std::cerr << "create socket failed" << std::endl;
		return (1);
	}

	int	ctrlno = fcntl(sockBuff, F_GETFL);
	if (ctrlno < 0) {
		putLog("fcntl(GET): " + std::string(strerror(errno)));
		close(sockBuff);
		return (2);
	}
	ctrlno = fcntl(sockBuff, F_SETFL, ctrlno | O_NONBLOCK);
	if (ctrlno < 0) {
		putLog("fcntl(SET): " + std::string(strerror(errno)));
		close(sockBuff);
		return (3);
	}

	int optval = 1;
	ctrlno = setsockopt(sockBuff, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
	if (ctrlno < 0) {
		putLog("setsockopt: " + std::string(strerror(errno)));
		close(sockBuff);
		return (4);
	}

	if (bind(sockBuff, addrinfo->ai_addr, addrinfo->ai_addrlen) < 0) {
		std::cerr << "bind socket failed " << server->getName() << std::endl;
		close(sockBuff);
		return (5);
	}
	_serverSockets.insert(std::pair<int, const Server*>(sockBuff, server));
	return (0);
}

/// @brief closes asll server sockets
/// @param  none
void	WebServer::_closeServer(void) {
	for (std::map<int, const Server*>::iterator it = _serverSockets.begin(); it != _serverSockets.end(); ++it) {
		close(it->first);
	}
}

/// @brief handle a client request and prepares a response
/// @param client 
void	WebServer::_handleRequest(std::map<int, Client>::iterator& client, const Server* server) {

	int status;
	ByteString out;
	std::cerr << client->second.getRequest() << '\n';
	try {
		Http					req(client->second.getRequest());
		std::string				route = req.getStartLine().path;
		Location 				loc(server->getLocation(route));
		std::set<std::string>	methods = loc.getLimExcept();
		req.verifyMethod(methods);
		/* process normal */
		std::map<std::string, std::string>	bin;
		bin["py"] = "/usr/bin/python3";
		bin["php"] = "/usr/bin/php-cgi";
		out = req.getResponseBody(loc.getRoot(), bin, *server);
		status = 200;
	} catch (int s) {
		status = s;
		out = Http::buildErrorHtml(status, *server);
	}
	std::cout << "status " << status << std::endl;
	//ici il faut encore verifier le status. puis rebuild le header a append a la response

	client->second.setResponse(Http::buildResponse(out, status, server->getName()));
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
