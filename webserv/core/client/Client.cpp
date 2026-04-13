/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 14:11:31 by pberset           #+#    #+#             */
/*   Updated: 2026/04/13 18:27:27 by ylabussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Client.hpp>

Client::Client(void): _requestComplete(false), _requestFailed(false), _lastActivityTime(time(NULL)) {
}

Client::~Client(void) {
}

const ByteString&	Client::getRequest(void) const {
    return (_request);
}

const ByteString&	Client::getResponse(void) const {
    return (_response);
}

int					Client::getResponseStatus(void) const {
	return (_resp_status);
}

Resource&			Client::getResource(void) {
	return (_resource);
}

bool    Client::isRequestComplete(void) const {
    return (_requestComplete);
}

bool	Client::isRequestFailed(void) const {
	return (_requestFailed);
}

time_t	Client::getLastActivityTime(void) const {
	return (_lastActivityTime);
}

void	Client::setResponse(const ByteString& response) {
	_response = response;
}

void	Client::setResponseStatus(int status) {
	_resp_status = status;
}

void	Client::setResource(const Resource& resource) {
	_resource = resource;
}

/// @brief reads a request until recv is empty from a client and stores it . RFC 7230 § 6.6
/// @param  none
/// @return bytes read
std::size_t	Client::readRequest(int socket) {
	byte	temp_buffer[BUFFER_SIZE];
	ssize_t	bytesRead = 0;

    bytesRead = recv(socket, &temp_buffer, BUFFER_SIZE, 0);
	std::cerr << "read " << bytesRead << " bytes\n";
    if (bytesRead < 0) {
        std::cerr << "recv read error" << std::endl;
		_requestFailed = true;
		throw 400; // Invalid Request
    } else if (bytesRead == 0) {
        std::cerr << "client closed the connection" << std::endl;
		_requestFailed = true;
		throw 410; // Gone
    } else {
        _request.append(temp_buffer, bytesRead);
		_requestComplete = Http::checkRequestComplete(_request);
		_lastActivityTime = time(NULL);
    }

	return (bytesRead);
}

bool			Client::readResource(void) {return _resource.readChunk();}

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
		_response = _response.substr(bytes_sent); // response may still contain bytes to be sent
		_lastActivityTime = time(NULL);
	}
	return (_response.empty());
}

void	Client::reset(void) {
	_requestComplete = false;
	_requestFailed = false;
	_request.clear();
	_response.clear();
}
