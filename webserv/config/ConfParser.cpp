/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 16:58:43 by pberset           #+#    #+#             */
/*   Updated: 2025/12/12 17:41:24 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

void	ConfParser::configureServer(const std::string& file) {
	
	std::ifstream	fs(file.c_str());
	std::string		config;

	if (!fs) {
		fs.clear();
		std::cerr << "file does not exist: " << file << std::endl;
		return ;
	}
	
	if (!(fs >> config)) {
	    std::cerr << "Failed to read from file: " << file << std::endl;
		return ;
	}

	std::cout << config << std::endl;
}

