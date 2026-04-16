/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neon-05 <neon-05@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 14:11:31 by pberset           #+#    #+#             */
/*   Updated: 2026/04/14 17:36:18 by neon-05          ###   ########.fr       */
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
std::size_t	Client::readRequest(int socket, const Server* serv) {
	byte	temp_buffer[BUFFER_SIZE];
	ssize_t	bytesRead = 0;

    bytesRead = recv(socket, &temp_buffer, BUFFER_SIZE, 0);
	std::ostringstream os;
	os << "read " << bytesRead << " bytes";
	serv->getAccStream()->log(os.str());
	os.str() = "";
    if (bytesRead < 0) {
        serv->getErrStream()->log("recv: read error");
		_requestFailed = true;
		throw 400; // Invalid Request
    } else if (bytesRead == 0) {
        serv->getErrStream()->log("readRequest: client closed the connection");
		_requestFailed = true;
		throw 410; // Gone
    } else {
        _request.append(temp_buffer, bytesRead);
		_requestComplete = Http::checkRequestComplete(_request);
		_lastActivityTime = time(NULL);
    }

	return (bytesRead);
}

bool	Client::readResource(Logger*& logger) {return _resource.readChunk(logger);}

/// @brief writes a server response to a client
/// @param  none
/// @return bool. True if response was successfully sent
bool	Client::writeResponse(int socket, Logger*& logger) {
	if (_response.empty()) {
		return (false);
	}

	ssize_t	bytes_sent = send(socket, _response.data(), _response.length(), 0);
	if (bytes_sent < 0 ) {
		logger->log("send error");
	} else if (bytes_sent == 0) {
		logger->log("client has closed the connection");
	} else {
		std::ostringstream	os;
		os << "write " << bytes_sent << " bytes";
		logger->log(os.str()); //debug
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
