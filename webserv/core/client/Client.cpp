/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:21:46 by pberset           #+#    #+#             */
/*   Updated: 2026/01/19 15:26:55 by ylabussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int socket): _socket(socket), _messageComplete(false) {}
Client::Client(const Client& other): 
	_requestIn(other._requestIn),
	_responseOut(other._responseOut),
	_messageSize(other._messageSize),
	_messageComplete(other._messageComplete),
	_socket(other._socket) {}

Client::Client(void): _socket(-1), _messageComplete(false), _messageSize(0) {}
Client::~Client(void) {}

Client&		Client::operator=(const Client& other)
{
	if (this != &other)
	{
		_requestIn = other._requestIn;
		_responseOut = other._responseOut;
		_messageSize = other._messageSize;
		_messageComplete = other._messageComplete;
		_socket = other._socket;
	}
	return *this;
}

std::string			Client::getRequestIn(void) {return _requestIn;}
std::string			Client::getResponseOut(void) {return _responseOut;}
long unsigned int	Client::getMessageSize(void) {return _messageSize;}
bool				Client::isMessageComplete(void) {return _messageComplete;}
int					Client::getSocketStatus(void) {return _socket;} // need to make sure this is what you meant TODO
