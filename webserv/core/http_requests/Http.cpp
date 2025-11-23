/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:27:00 by pberset           #+#    #+#             */
/*   Updated: 2025/11/23 16:27:09 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"

Http::Http(void) {
    std::cout << "Default Http constructor" << std::endl;
}
Http::Http(const Http &other) {
    std::cout << "Copy Http constructor" << std::endl;
    this = &other;
}

Http::~Http(void) {
    std::cout << "Http destructor" << std::endl;
}
Http    &Http::operator=(const Http &other) {
	std::cout << "Http assignation operator" << std::endl;
	if (this != &other) {
    // TODO: members
   }
   return (*this);
};

