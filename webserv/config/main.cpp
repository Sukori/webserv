/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 17:24:08 by pberset           #+#    #+#             */
/*   Updated: 2025/12/18 16:06:28 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <Configuration.hpp>

int	main(int argc, char* argv[]) {

	if (argc > 2) {
		std::cerr << "input: too many arguments";
		return (1);
	}

	std::string	configStr;

	if (argc == 2)
		configStr = readFile(argv[1]);
	else
		configStr = readFile("config_files/Default.conf");
	
	std::cout << configStr << std::endl;
	return (0);
}

std::string	readFile(const char* file) {
	std::ifstream	fs(file);
	if (!fs.is_open()) {
		std::cerr << "file: failed to open" << std::endl;
		return ("");
	}
	
	std::ostringstream	ss;
	std::string			output;

	ss << fs.rdbuf();
	output = ss.str();
	return (output);
}
