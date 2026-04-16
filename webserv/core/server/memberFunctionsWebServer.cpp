/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memberFunctionsWebServer.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 17:46:35 by pberset           #+#    #+#             */
/*   Updated: 2026/04/15 16:07:21 by ylabussi         ###   ########.fr       */
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

/// @brief handle a client request and gets the resource
/// @param client
/// @param server
Resource	WebServer::_handleRequest(Client& client, const Server& server) {
	int			status;
	Resource	out;
	try {
		Http				req(client.getRequest());
		const std::string&	route = req.getStartLine().path;
		const Location&		loc = server.getLocation(route);
		if (server.getMaxBodySize() > 0 && req.getRequestBody().length() > server.getMaxBodySize()) {
			std:: cerr << "body size: " << req.getRequestBody().length() << '\n';
			throw 413; // Request Entity Too Large
		}
		req.verifyMethod(loc.getLimExcept()); // check for 405 Method Not Allowed
		if (req.isChunked())
			req.dechunk();
		out = req.getResponseBody(loc, server, status);
	} catch (int s) {
		status = s;
		out = Http::buildErrorHtml(status, server);
	}
	client.setResponseStatus(status);
	return out;
}

void		WebServer::_handleResponse(Client& client, const Server& server) {
	if (client.getResource().done())
	{
		std::cerr << "Resource done reading\n";
		client.setResponse(Http::buildResponse(client.getResource().getContent(), client.getResponseStatus(), server.getName()));
	}
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

/// @brief closes a client that had errors or hangup or finished
/// @param it
/// @param fd 
/// @param i 
void	WebServer::_closeClient(std::map<int, Client>::iterator& it, size_t& i) {
	close(_fds[i].fd);
	_clientSockets.erase(it);
	_clientsServers.erase(_fds[i].fd);
	_fds.erase(_fds.begin() + i);
	i--;
}
