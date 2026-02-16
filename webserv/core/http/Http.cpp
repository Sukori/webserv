#include "Http.hpp"

Http::Http(void): _socket(0), _allowed_methods() {}
Http::Http(int socket, const std::set<std::string>& allowed_methods):
	_socket(socket),
	_startline(_getStartLine(_socket)),
	_header(_getHeaders(_socket)),
	_allowed_methods(allowed_methods)
{/* TODO: check for 400 Bad Request, 405 Method Not Allowed, check for 411 Length Required */}

Http::Http(const Http& o):
	_socket(o._socket),
	_startline(o._startline),
	_header(o._header),
	_allowed_methods(o._allowed_methods)
{}

Http::~Http(void) {}

bool	Http::isValidRequest(std::string root) const {
	int method_allowed = _allowed_methods.find(_startline.method) != _allowed_methods.end();
	struct stat b;
	int file_exists = stat((root + _startline.path).c_str(), &b) == 0;
	return (method_allowed & file_exists);
/*	
startline:
	method in authorized methods
	path exists

headers:
	/shrug
*/

}

int						Http::getSocket(void) const {return _socket;}
const Http::StartLine&	Http::getStartLine(void) const {return _startline;}
const Http::Header&		Http::getHeader(void) const {return _header;}

std::string		Http::_getNextLine(int fd) {
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

Http::StartLine	Http::_getStartLine(int fd) {
	StartLine ret;
	std::string line = _getNextLine(fd);
	std::cout << line;
	size_t	end = line.find(' '), start = 0;
	ret.method = line.substr(start, end - start);
	start = line.find_first_not_of(' ', end);
	end = line.find_first_of(' ', start);
	_splitPath(line.substr(start, end - start), ret);
	return ret;
}

static int toUPPER_SNAKE_CASE(int c) {
	if (c == '-')
		return '_';
	else
		return std::toupper(c);
}

Http::Header	Http::_getHeaders(int fd) {
	Header ret;
	std::string line, key, val;
	size_t sep;
	while ((line = _getNextLine(fd)).length() > 1)
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

void			Http::_splitPath(const std::string& path, StartLine& sl) {
	size_t extra, query;
	extra = path.find_first_of(".?");
	if (extra == -1ul)
		extra = path.size();
	if (path[extra] == '.')
		extra = path.find_first_of("/?", extra);
	if (extra == -1ul)
		extra = path.size();
	query = path.find('?', extra);
	if (query == -1ul)
		query = path.size();
	sl.path = path.substr(0, extra);
	sl.extra = path.substr(extra, query-extra);
	sl.query = path.substr(query);
}
