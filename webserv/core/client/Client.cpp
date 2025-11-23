/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:21:46 by pberset           #+#    #+#             */
/*   Updated: 2025/11/23 16:21:53 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void) {
    std::cout << "Default Client constructor" << std::endl;
}
Client::Client(const Client &other) {
    std::cout << "Copy Client constructor" << std::endl;
    this = &other;
}

Client::~Client(void) {
    std::cout << "Client destructor" << std::endl;
}
Client    &Client::operator=(const Client &other) {
	std::cout << "Client assignation operator" << std::endl;
	if (this != &other) {
    // TODO: members
   }
   return (*this);
};

