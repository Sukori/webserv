/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 14:11:31 by pberset           #+#    #+#             */
/*   Updated: 2026/01/19 14:11:37 by pberset          ###   Lausanne.ch       */
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

long unsigned int	Client::getMessageSize(void) {
    return (_messageSize);
}

bool    Client::isMessageComplete(void) {
    return (_messageComplete);
}

int	Client::getSocketStatus(void) {
    return (_socket);
}

int	Client::readMessage(void) {
    char    temp_buffer[4096] = {0};
    ssize_t status;

    status = recv(_socket, temp_buffer, 4096, 0);
    if (status < 0) {
        std::cout << "recv read error" << std::endl;
    } else if (status == 0) {
        std::cout << "client closed the connection" << std::endl;
    } else {
        std::cout << "read " << status << "bytes" << std::endl;
        _requestIn.append(temp_buffer, status);
        _messageSize += status;
        if (_requestIn.find("\r\n\r\n") != std::string::npos) {
           _messageComplete = true;
        }
    }
    return ((int)status);
}
