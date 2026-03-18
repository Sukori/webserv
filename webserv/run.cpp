/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 17:28:07 by pberset           #+#    #+#             */
/*   Updated: 2026/03/18 17:28:10 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "core/server/WebServer.hpp"

int	main(int argc, char *argv[]) {

	if (argc > 2) {
		std::cerr << "input: too many arguments";
		return (1);
	}

	std::string	configStr;

	if (argc == 2)
		configStr = readFile(argv[1]);
	else
		configStr = readFile("config/config_files/default.conf");

	if (configStr.empty()) {
		std::cerr << "input: file is empty" << std::endl;
		return (2);
	}
	
	std::string	noComments = filterComments(configStr);
	if (noComments.empty()){
		std::cerr << "input: empty file after comments filtering" << std::endl;
		return (3);
	}
	
	std::string	sureLine = insertSpaces(noComments);

	/* console log for cleaning check
	std::cout << "File extract-------------" << std::endl;
	std::cout << configStr << std::endl;
	std::cout << "FilterComments-----------" << std::endl;
	std::cout << noComments << std::endl;
	std::cout << "insertSpaces-----------" << std::endl;
	std::cout << sureLine << std::endl;
	*/

	Parser			parser(sureLine);
	Configuration*	config = new Configuration(parser.initParser());
	std::cout << *config << std::endl;
	WebServer	webserv(*config);
	webserv.run();

	return (0);
}
