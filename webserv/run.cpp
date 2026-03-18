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

	if (configStr.empty())
		return (1);
	
	std::string	noComments = filterComments(configStr);
	if (noComments.empty())
		return (2);
	
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
