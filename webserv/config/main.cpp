/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 17:24:08 by pberset           #+#    #+#             */
/*   Updated: 2026/01/06 18:00:43 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "Configuration.hpp"
#include "Parser.hpp"

int	main(int argc, char* argv[]) {

	if (argc > 2) {
		std::cerr << "input: too many arguments";
		return (1);
	}

	std::string	configStr;

	if (argc == 2)
		configStr = readFile(argv[1]);
	else
		configStr = readFile("config_files/default.conf");

	if (configStr.empty())
		return (1);
	
	std::string	noComments = filterComments(configStr);
	if (noComments.empty())
		return (2);
	
	std::string	safeLine = insertSpaces(noComments);

	Parser			parser(safeLine);
	Configuration*	config = new Configuration(parser.initParser()); // will be used repeatedly, so better fix it into memory, instead of keeping on stack

	//at this point, config is valid, or we handle error

	return (0);
}

