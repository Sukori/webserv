#include "Http.hpp"

std::string		Http::getNextLine(int fd) {
	std::string ret;
	char c;
	while (read(fd, &c, 1) > 0)
	{
		ret += c;
		if (c == '\n')
			break;
	}
	return ret;
}

Http::StartLine	Http::getStartLine(int fd) {
	StartLine ret;
	std::string line = getNextLine(fd);
	std::cout << line;
	size_t	end = line.find(' '), start = 0;
	ret.method = line.substr(start, end - start);
	start = line.find_first_not_of(' ', end);
	end = line.find_first_of(' ', start);
	ret.path = line.substr(start, end - start);
	return ret;
}

static int toUPPER_SNAKE_CASE(int c) {
	if (c == '-')
		return '_';
	else
		return std::toupper(c);
}

Http::Header	Http::getHeaders(int fd) {
	Header ret;
	std::string line, key, val;
	size_t sep;
	while ((line = getNextLine(fd)).length() > 1)
	{
		sep = line.find(':');
		key = line.substr(0, sep);
		std::transform(key.begin(), key.end(), key.begin(), toUPPER_SNAKE_CASE);
		sep++;
		while (isspace(line[sep]))
			sep++;
		val = line.substr(sep, line.find('\n') - sep);
		ret.insert(std::make_pair(key, val));
	}
	return ret;
}
