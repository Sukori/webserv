#include "Http.hpp"

// test main for http requests

int	main(int argc, char *argv[]) {
	std::string raw, line;
	if (argc < 2)
		return 1;
	std::ifstream inFile(argv[1]);
	while (getline(inFile, line))
		raw += line + '\n';
	HttpMessage m1(raw);
	std::cout << "raw: \n" << raw << "\n\n";
	std::cout << "parsed: \n" << m1;

	std::cout << "message is ";
	switch (m1.getMessageType())
	{
	case HttpMessage::REQUEST:
		std::cout << "a request\n";
		break;
	case HttpMessage::RESPONSE:
		std::cout << "a response\n";
		break;
	default:
		std::cout << "undefined\n";
		break;
	}
	return (0);
}

