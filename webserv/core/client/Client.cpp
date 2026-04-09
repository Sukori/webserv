/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neon-05 <neon-05@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 14:11:31 by pberset           #+#    #+#             */
/*   Updated: 2026/04/08 20:24:21 by neon-05          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Client.hpp>

Client::Client(void): _requestComplete(false) {
}

Client::~Client(void) {
}

const ByteString&	Client::getRequest(void) const {
    return (_request);
}

const ByteString&	Client::getResponse(void) const {
    return (_response);
}

bool    Client::isRequestComplete(void) const {
    return (_requestComplete);
    return (_requestComplete);
}

void	Client::setResponse(const ByteString& response) {
	_response = response;
}

/// @brief reads a request until recv is empty from a client and stores it 
/// @param  none
/// @return bytes read
std::size_t	Client::readRequest(int socket) {
	byte	temp_buffer[BUFFER_SIZE];
	ssize_t	bytesRead = 0;

    bytesRead = recv(socket, &temp_buffer, BUFFER_SIZE, 0);
    if (bytesRead < 0) {
        std::cerr << "recv read error" << std::endl;
    } else if (bytesRead == 0) {
		_requestComplete = true;
        std::cerr << "client closed the connection" << std::endl;
    } else {
        _request.append(temp_buffer, bytesRead);
		if (bytesRead < BUFFER_SIZE)
			_requestComplete = true;
    }

	return (bytesRead);
}

/// @brief writes a server response to a client
/// @param  none
/// @return bool. True if response was successfully sent
bool	Client::writeResponse(int socket) {
	if (_response.empty()) {
		return (false);
	}

	ssize_t	bytes_sent = send(socket, _response.data(), _response.length(), 0);
	if (bytes_sent < 0 ) {
		std::cout << "send error\n";
	} else if (bytes_sent == 0) {
		std::cout << "client has closed the connection\n";
	} else {
		std::cout << "write " << bytes_sent << " bytes\n"; //debug
		_response = _response.substr(bytes_sent); // response may still contain bytes ot be sent
	}
	return (_response.empty());
}
