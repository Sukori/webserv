/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:23:28 by pberset           #+#    #+#             */
/*   Updated: 2025/12/12 18:57:32 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "Configuration.hpp"

Configuration::Configuration(void) {
    std::cout << "Default Configuration constructor" << std::endl;
}

//Configuration::Configuration(const Configuration &other) {
//    std::cout << "Copy Configuration constructor" << std::endl;
//    this = &other;
//}

Configuration::~Configuration(void) {
    std::cout << "Configuration destructor" << std::endl;
}

//Configuration	&Configuration::operator=(const Configuration &other) {
//	std::cout << "Configuration assignation operator" << std::endl;
//	if (this != &other) {
//    // TODO: members
//   }
//   return (*this);
//};
//
//Configuration	&Configuration::operator<<(const Configuration &other) {
//	//TODO: print members
//}

int	Configuration::openConfiguration(const std::string& file) {
	std::ifstream	fs(file.c_str());
	int				errParse;

	if (!fs) {
		fs.clear();
		std::cerr << file << ": file does not exist" << std::endl;
		return (1);
	} else if (!fs.good()) {
		fs.clear();
		std::cerr << file << ": input stream failed" << std::endl;
		return (2);
	} else {
		errParse = parseConfiguration(fs);
	}
	
	return (0);
}

int			parseConfiguration(const std::ifstream& data) {

	std::string	buffer;
	
	data >> buffer;
	std::cout << buffer << std::endl;	
	return (0);
}

