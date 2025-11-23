/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:24:32 by pberset           #+#    #+#             */
/*   Updated: 2025/11/23 16:24:38 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "Cookies.hpp"

Cookies::Cookies(void) {
    std::cout << "Default Cookies constructor" << std::endl;
}
Cookies::Cookies(const Cookies &other) {
    std::cout << "Copy Cookies constructor" << std::endl;
    this = &other;
}

Cookies::~Cookies(void) {
    std::cout << "Cookies destructor" << std::endl;
}
Cookies    &Cookies::operator=(const Cookies &other) {
	std::cout << "Cookies assignation operator" << std::endl;
	if (this != &other) {
    // TODO: members
   }
   return (*this);
};

