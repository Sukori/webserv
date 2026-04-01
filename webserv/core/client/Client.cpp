/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 14:11:31 by pberset           #+#    #+#             */
/*   Updated: 2026/04/01 16:48:58 by ylabussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void): _requestComplete(false) {
}

Client::~Client(void) {
}

const ByteString&	Client::getRequestIn(void) const {
    return (_requestIn);
}

const ByteString&	Client::getResponseOut(void) const {
    return (_responseOut);
}

bool    Client::isRequestComplete(void) const {
    return (_requestComplete);
}

void	Client::setResponse(const ByteString& message) {
	_responseOut = message;
}

/// @brief reads a request until RFC end of header flag from a client and stores it 
/// @param  none
/// @return bytes read
std::size_t	Client::readRequest(int socket) {
    //char    temp_buffer[BUFFER_SIZE] = {0};
	byte	temp_buffer[BUFFER_SIZE];
	std::size_t	bytesRead = 0;

	bytesRead = recv(socket, temp_buffer, BUFFER_SIZE, 0);
	if (bytesRead < 0) {
		std::cerr << "recv read error" << std::endl;
	} else if (bytesRead == 0) {
		std::cerr << "client closed the connection" << std::endl;
	} else {
		_requestIn.append(temp_buffer, bytesRead);
	}
	if (bytesRead < BUFFER_SIZE)
		_requestComplete = true;

	return (bytesRead);
}

/// @brief writes a server response to a client
/// @param  none
/// @return bool. True if response was successfully sent
bool	Client::writeResponse(int socket) {
	if (_responseOut.empty()) {
		return (false);
	}

	std::size_t	bytes_sent = send(socket, _responseOut.data(), _responseOut.length(), 0);
	if (bytes_sent < 0 ) {
		std::cout << "send error" << std::endl;
	} else if (bytes_sent == 0) {
		std::cout << "client has closed the connection" << std::endl;
	} else {
		std::cout << "write " << bytes_sent << "bytes" << std::endl; //debug
		_responseOut.clear();
	}
	return (_responseOut.empty());
}
