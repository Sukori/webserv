#include "Http.hpp"
#include "../cgi/Cgi.hpp"

// test main for http requests
int	main(int argc, char *argv[]) {
	if (argc < 2)
	return std::cout << "please enter a file to be read" << std::endl, 1;
	int fds[] = {0, 1};
	fds[0] = open(argv[1], O_RDONLY);
	
	std::set<std::string> a;
	a.insert("GET");
	a.insert("POST");
	a.insert("DELETE");
	
	try {
		Http req (fds[0], a);
		const Http::StartLine& sl (req.getStartLine());
		Http::Header h (req.getHeader());

		std::cout << "path:\t" + sl.path + '\n';
		std::cout << "extra:\t" + sl.extra + '\n';
		std::cout << "query:\t" + sl.query + '\n';
		std::cout << "method:\t" + sl.method + '\n';

		std::cout << "headers:\n";
		for (Http::Header::const_iterator it = h.begin(); it != h.end();it++)
		std::cout << '\t' + it->first + '=' + it->second + '\n';
		std::cout << '\n';

		std::string root ("/home/ylabussi/Documents/webserv/webserv/www");

		std::map<std::string, std::string> bin;
		bin.insert(std::make_pair("py", "/usr/bin/python3"));
		bin.insert(std::make_pair("php", "/usr/bin/php"));

		std::vector<std::string> idx;
		idx.insert(idx.end(), "index.html");
		idx.insert(idx.end(), "home.html");
		idx.insert(idx.end(), "index.php");
		idx.insert(idx.end(), "home.php");
		idx.insert(idx.end(), "index.py");
		idx.insert(idx.end(), "home.py");

		std::cout << req.getResponseBody(root, bin, idx);
	} catch (int status) {
		std::cerr << status << '\n';
	}

	Http::buildResponse(200, "message body", "weebserv");
	return 0;
}
