/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MiniServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 11:08:49 by pberset           #+#    #+#             */
/*   Updated: 2026/01/13 14:40:34 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "MiniServer.hpp"

void	putLog(const std::string& message) {
	std::cout << message << std::endl;
}

void	exitWithError(const std::string& message) {
	putLog("Error: " + message);
	exit(1);
}

MiniServer::MiniServer(std::string ipAddr, int port) : _ipAddr(ipAddr), _port(port), _socket(), _newSocket(), _messageIn(), _socketAddress(), _socketAddrLen(sizeof(_socketAddress)), _messageServer(buildResponse()) {
    std::cout << "Default MiniServer constructor" << std::endl;

	_socketAddress.sinFamily = AF_INET;
	_socketAddress.sinPort = htons(_port);
	_socketAddress.inAddr.s_addr = inet_addr(_ipAddr.c_str());
	if (startServer() != 0) {
		std::ostringstream ss;
		ss << "failed to start server with PORT: " << ntohs(_socketAddress.sinPort);
		putLog(ss.str());
	}
}

MiniServer::~MiniServer(void) {
    std::cout << "MiniServer destructor" << std::endl;
	closeServer();
}

int	MiniServer::startServer(void) {
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0) {
		exitWithError("create socket failed");
		return (1);
	}
	if (bind(_socket, (sockaddr *)&_socketAddress, _socketAddrLen) < 0) {
		exitWithError("failed to bind socket to address");
		return (2);
	}
	return (0);

}

int	MiniServer::closeServer(void) {
	close(_socket);
	close(_newSocket);
	exit(0);
}

void	MiniServer::startListen(void) {
	if (listen(_socket, 20) < 0) {
		exitWithError("socket listen failed");
	}
	std::ostringstream ss;
	ss << "Listening on ADDRESS: " << inet_ntoa(_socketAddress.inAddr)
	<< " PORT: " << ntohs(_socketAddress.sinport);
	putLog(ss.str());

	int bytesReceived;

	while (true) {
		putLog("============WAITING============");
		acceptConnection(_newSocket);

		char	buffer[BUFFER_SIZE] = {0};
		bytesReceived = read(_newSocket, buffer, BUFFER_SIZE);
		if (bytesReceived < 0) {
			exitWithError("failed to read from client connection");
		}

		std::ostringstream	oss;
		oss << "received from client";
		putLog(oss.str());

		sendResponse();
		close(_newSocket);
	}
}

void	MiniServer::acceptConnection(int& new_socket) {
	new_socket = accept(_socket, (sockaddr *)&_socketAddress, &_socketAddrLen);
	if (new_socket < 0) {
		std::ostringstream ss;
		ss << "server failed to accept incoming connection from ADDRESS: " << inet_ntoa(_socketAddress.inAddr)
		<< " PORT: " << ntohs(_socketAddress.sinport);
		exitWithError(ss.str());
	}
}

std::string	MiniServer::buildResponse(void) {
	std::string	htmlFile = "<h1>HELLO WORLD</h1>"
	std::ostringstream ss;
	ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << \n << htmlFile;

	return (ss.str());
}

void	MiniServer::sendResponse(void) {
	long	bytesSend;

	bytesSend = write(_newSocket, _messageServer.c_str(), _messageServer.size());

	if (bytesSend == _messageServer.size()) {
		putLog("Response sent to client");
	} else {
		putLog("failed to send response to client");
	}
}

