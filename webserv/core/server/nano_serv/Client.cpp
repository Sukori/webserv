/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 14:11:31 by pberset           #+#    #+#             */
/*   Updated: 2026/01/24 15:51:44 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int socket): _socket(socket) {
    std::cout << "Default Client constructor" << std::endl;
}

Client::~Client(void) {
    std::cout << "Client destructor" << std::endl;
}

std::string	Client::getRequestIn(void) {
    return (_requestIn);
}

std::string	Client::getResponseOut(void) {
    return (_responseOut);
}

ssize_t	Client::getRequestSize(void) {
    return (_requestSize);
}

bool    Client::isRequestComplete(void) {
    return (_requestComplete);
}

int	Client::getSocketStatus(void) {
    return (_socket);
}

void	Client::setResponse(const std::string& response) {
	_responseOut = response;
}

ssize_t	Client::readRequest(void) {
    char    temp_buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_read;

    bytes_read = recv(_socket, temp_buffer, BUFFER_SIZE, 0);
    if (bytes_read < 0) {
        std::cout << "recv read error" << std::endl;
    } else if (bytes_read == 0) {
        std::cout << "client closed the connection" << std::endl;
    } else {
        std::cout << "read " << bytes_read << "bytes" << std::endl; // debug
        _requestIn.append(temp_buffer, bytes_read);
        _requestSize += bytes_read;
        if (_requestIn.find("\r\n\r\n") != std::string::npos) {
           _requestComplete = true;
        }
    }
    return (bytes_read);
}

bool	Client::writeResponse(void) {
	if (_responseOut.empty()) {
		return (false);
	}

	ssize_t	bytes_sent = send(_socket, _responseOut.c_str(), _responseOut.size(), 0);
	if (bytes_sent < 0 ) {
		std::cout << "send error" << std::endl;
	} else if (bytes_sent == 0) {
		std::cout << "client has closed the connection" << std::endl;
	} else {
		std::cout << "write " << bytes_sent << "bytes" << std::endl; //debug
		_responseOut.erase(0, bytes_sent);
	}
	return (_responseOut.empty());
}
