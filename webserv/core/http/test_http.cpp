#include "Http.hpp"
#include "../cgi/Cgi.hpp"

// test main for http requests

int	main(int argc, char *argv[]) {
	if (argc < 2)
		return std::cout << "please enter a file to be read" << std::endl, 1;
	int fds[] = {0, 1};
	fds[0] = open(argv[1], O_RDONLY);
	fds[1] = open("response", O_CREAT | O_WRONLY | O_TRUNC, 0777);
	Http::StartLine	sl = Http::getStartLine(fds[0]);
	Http::Header	h = Http::getHeaders(fds[0]);

	std::cout << "path:	" + sl.path + '\n';
	std::cout << "method:	" + sl.method + '\n';

	std::cout << "headers:\n";
	for (Http::Header::const_iterator it = h.begin(); it != h.end();it++)
	std::cout << it->first + '=' + it->second + '\n';
	std::cout << '\n';

	exec_cgi("/opt/pyenv/shims/python", "../../www/python/index.py", h, fds);

	return 0;
}
