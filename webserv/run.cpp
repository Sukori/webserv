/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 17:28:07 by pberset           #+#    #+#             */
/*   Updated: 2026/04/02 17:59:30 by ylabussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <WebServer.hpp>

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

	Parser			parser(sureLine);

	Configuration*	config = new Configuration(parser.initParser());
	std::cout << *config << std::endl;

	if (config->getServers().empty()) {
		std::cerr << "No valid server definition found.\nExit" << std::endl;
		return (4);
	}
	
	WebServer	webserv(*config);
	WebServer::installSignalHandlers();
	webserv.run();
	delete(config);
	return (0);
}
