#include "Http.hpp"

// test main for http requests

int	main(int argc, char *argv[]) {
	if (argc < 2)
		return std::cout << "please enter a file to be parsed" << std::endl, 1;
	int fd = open(argv[1], O_RDONLY);
	Http::StartLine	sl = Http::getStartLine(fd);
	Http::Header	h = Http::getHeaders(fd);

	std::cout << "path:	" + sl.path + '\n';
	std::cout << "method:	" + sl.method + '\n';

	std::cout << "headers:\n";
	for (Http::Header::const_iterator it = h.begin(); it != h.end();it++)
	std::cout << it->first + '=' + it->second + '\n';
	return 0;
}
