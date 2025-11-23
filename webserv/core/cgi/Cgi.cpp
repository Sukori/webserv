/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:17:33 by pberset           #+#    #+#             */
/*   Updated: 2025/11/23 16:17:42 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

Cgi::Cgi(void) {
    std::cout << "Default Cgi constructor" << std::endl;
}
Cgi::Cgi(const Cgi &other) {
    std::cout << "Copy Cgi constructor" << std::endl;
    this = &other;
}

Cgi::~Cgi(void) {
    std::cout << "Cgi destructor" << std::endl;
}
Cgi    &Cgi::operator=(const Cgi &other) {
	std::cout << "Cgi assignation operator" << std::endl;
	if (this != &other) {
    // TODO: members
   }
   return (*this);
};

