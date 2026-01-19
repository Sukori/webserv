/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NanoServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 14:11:12 by pberset           #+#    #+#             */
/*   Updated: 2026/01/19 14:11:19 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

//_clients.insert(std::make_pair(new_socket_fd, Client(new_socket_fd)));
#include "NanoServer.hpp"

NanoServer::NanoServer(void) {
    std::cout << "Default NanoServer constructor" << std::endl;
}

NanoServer::NanoServer(const NanoServer &rhs) {
    std::cout << "Copy NanoServer constructor" << std::endl;
    this = &rhs;
}

NanoServer::~NanoServer(void) {
    std::cout << "NanoServer destructor" << std::endl;
}

NanoServer    &NanoServer::operator=(const NanoServer &rhs) {
    std::cout << "NanoServer assignation operator" << std::endl;
    if (this != &rhs) {
        // TODO: members
    }
    return (*this);
};

