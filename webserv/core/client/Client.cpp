/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 14:11:31 by pberset           #+#    #+#             */
/*   Updated: 2026/01/24 15:51:44 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void) {
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

void	Client::setResponse(const std::string& response) {
	_responseOut = response;
}

/// @brief reads a request until RFC end of header flag from a client and stores it 
/// @param  none
/// @return bytes read
ssize_t	Client::readRequest(int socket) {
    char    temp_buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_read;

    bytes_read = recv(socket, temp_buffer, BUFFER_SIZE, 0);
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

/// @brief writes a server response to a client
/// @param  none
/// @return bool. True if response was successfully sent
bool	Client::writeResponse(int socket) {
	if (_responseOut.empty()) {
		return (false);
	}

	ssize_t	bytes_sent = send(socket, _responseOut.c_str(), _responseOut.size(), 0);
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
