#include "HttpMessage.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

// test main for http requests

int	main(int argc, char *argv[]) {
	std::string raw, line;
	if (argc < 2)
		return std::cout << "please enter a file to be parsed" << std::endl, 1;
	std::ifstream inFile(argv[1]);
	while (getline(inFile, line))
		raw += line + '\n';
	HttpMessage m1(raw);
	std::cout << "raw:\n" << raw << "\n\n";
	std::cout << "parsed:\n" << m1;

	std::set<std::string> allowed_methods;
	allowed_methods.insert("GET");
	allowed_methods.insert("POST");
	allowed_methods.insert("DELETE");

	if (m1 == HttpMessage::REQUEST)
	{
		HttpRequest rq1 (allowed_methods);
		rq1 = m1;

		std::cout << "method:	" << rq1.getMethod() << '\n';
		std::cout << "path:	" << rq1.getPath() << '\n';
	}
	else if (m1 == HttpMessage::RESPONSE)
	{
		HttpResponse rs1 (m1);
		std::cout << "status code:	" << rs1.getStatusCode() << '\n';
	}

	std::cout << "\nbuilding response message:\n";
	HttpResponse	rs2;

	rs2.setStartLine("HTTP/1.1 404");
	rs2.add_header_field("date", "Mon, 05 Jan 2026 15:39:51 GMT");
	rs2.setBody("/* message body here */");
	std::cout << '\n' << rs2 << '\n';

	return (0);
}
