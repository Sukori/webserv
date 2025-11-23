/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:23:01 by pberset           #+#    #+#             */
/*   Updated: 2025/11/23 16:23:07 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void) {
    std::cout << "Default Server constructor" << std::endl;
}
Server::Server(const Server &other) {
    std::cout << "Copy Server constructor" << std::endl;
    this = &other;
}

Server::~Server(void) {
    std::cout << "Server destructor" << std::endl;
}
Server    &Server::operator=(const Server &other) {
	std::cout << "Server assignation operator" << std::endl;
	if (this != &other) {
    // TODO: members
   }
   return (*this);
};

