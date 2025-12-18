#include "HttpMessage.hpp"
#include "HttpRequest.hpp"

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

	std::set<std::string> allowed_methods;
	allowed_methods.insert("GET");
	allowed_methods.insert("POST");
	allowed_methods.insert("DELETE");

	HttpRequest rq1 (allowed_methods);
	try
	{
		rq1 = m1;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	std::cout << "method:	" << rq1.getMethod() << '\n';
	std::cout << "path:	" << rq1.getPath() << '\n';


	return (0);
}

