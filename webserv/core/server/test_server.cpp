/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:23:31 by pberset           #+#    #+#             */
/*   Updated: 2025/11/23 16:23:39 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "test_server.hpp"

test_server::test_server(void) {
    std::cout << "Default test_server constructor" << std::endl;
}
test_server::test_server(const test_server &other) {
    std::cout << "Copy test_server constructor" << std::endl;
    this = &other;
}

test_server::~test_server(void) {
    std::cout << "test_server destructor" << std::endl;
}
test_server    &test_server::operator=(const test_server &other) {
	std::cout << "test_server assignation operator" << std::endl;
	if (this != &other) {
    // TODO: members
   }
   return (*this);
};

