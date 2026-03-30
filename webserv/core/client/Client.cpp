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
	_headerSize = 0;
	_expectedBodySize = 0;
	_bodySize = 0;
	_headerComplete = false;
	_alreadyChecked = false;
	_expectsBody = false;
	_bodyComplete = false;
}

Client::~Client(void) {
}

std::string	Client::getRequestIn(void) {
    return (_requestIn);
}

std::string	Client::getResponseOut(void) {
    return (_responseOut);
}

ssize_t	Client::getHeaderSize(void) {
    return (_headerSize);
}

ssize_t	Client::getBodySize(void) {
    return (_bodySize);
}

bool    Client::isRequestComplete(void) {
    return (_headerComplete && _bodyComplete);
}

void	Client::setResponse(const std::string& response) {
	_responseOut = response;
}

/// @brief reads a request until RFC end of header flag from a client and stores it 
/// @param  none
/// @return bytes read
ssize_t	Client::readRequest(int socket) {
    char    temp_buffer[BUFFER_SIZE] = {0};
	ssize_t	bytesRead = 0;

	if (!_headerComplete) {
    	bytesRead = recv(socket, temp_buffer, BUFFER_SIZE, 0);
    	if (bytesRead < 0) {
    	    std::cerr << "recv read error" << std::endl;
    	} else if (bytesRead == 0) {
    	    std::cerr << "client closed the connection" << std::endl;
    	} else {
    	    _rawHeaderIn.append(temp_buffer, bytesRead);
    	    _headerSize += bytesRead;
			std::size_t	clrf = _rawHeaderIn.find("\r\n\r\n");
    	    if ( clrf != std::string::npos) {
				_headerComplete = true;
				clrf += 4;
				_requestIn.append(_rawHeaderIn);
				std::string	sub = _rawHeaderIn.substr(clrf, std::string::npos);
				_bodySize += sub.length();
				_rawHeaderIn.erase(clrf, std::string::npos);
    	    }
    	}
	}
	if (_headerComplete) {
		if (!_alreadyChecked) {
			try {
				Http	header(_rawHeaderIn);
				std::map<std::string, std::string> headerMap = header.getHeader();
				
				_expectsBody = !header.getStartLine().method.compare("POST");

				if (_expectsBody) {
					std::map<std::string, std::string>::iterator	query = headerMap.find("CONTENT_LENGTH");
					if (query != headerMap.end()) {
						if (ft_isNUM(query->second))
							_expectedBodySize = ft_stoui(query->second);
						else
							throw 411;
					} else {
						throw 411;
					}
					if (_expectedBodySize <= 0) {
						throw 411;
					} else if (_bodySize == _expectedBodySize) {
						_bodyComplete = true;
					} else if (_bodySize > _expectedBodySize) {
						throw 429;
					}
				} else {
					_bodyComplete = true;
				}
			} catch (int status) {
				std::cerr << "Http::Header: " << status << std::endl;
			}
			_alreadyChecked = true;
		}
		std::cout << "ensuite" << std::endl;
		if (!_bodyComplete && _expectsBody) {
			bytesRead = recv(socket, temp_buffer, BUFFER_SIZE, 0);
    		if (bytesRead < 0) {
    		    std::cerr << "recv read error" << std::endl;
    		} else if (bytesRead == 0) {
    		    std::cerr << "client closed the connection" << std::endl;
    		} else {
    		    _requestIn.append(temp_buffer, bytesRead);
    		    _bodySize += bytesRead;
    		    if (_bodySize == _expectedBodySize) {
					_bodyComplete = true;
    		    } else if (_bodySize > _expectedBodySize) {
					throw 429;
				}
    		}
		}
	}

    return (bytesRead);
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
