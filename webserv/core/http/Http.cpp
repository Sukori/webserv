#include "Http.hpp"
#include "../cgi/Cgi.hpp"
#include <ctime>

Http::Http(int socket):
	_socket(socket),
	_startline(_parseStartLine(_socket)),
	_header(_parseHeaders(_socket)) {
	/* check for 400 Bad Request */
}

void	Http::verifyMethod(const std::set<std::string>& allowed_methods) const {
	if (allowed_methods.find(_startline.method) == allowed_methods.end())
		throw 405; // Method Not Allowed
}

Http::Http(const Http& o):
	_socket(o._socket),
	_startline(o._startline),
	_header(o._header)
{}

Http::~Http(void) {}

int						Http::getSocket(void) const {return _socket;}
const Http::StartLine&	Http::getStartLine(void) const {return _startline;}
const Http::Header&		Http::getHeader(void) const {return _header;}

std::string		Http::_parseNextLine(int fd) {
	std::string ret;
	char c;
	while (read(fd, &c, 1) > 0)
	{
		if (c == '\n')
			break;
		ret += c;
	}
	return ret;
}

Http::StartLine	Http::_parseStartLine(int fd) {
	StartLine ret;
	std::string line = _parseNextLine(fd);
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

Http::Header	Http::_parseHeaders(int fd) {
	Header ret;
	std::string line, key, val;
	size_t sep;
	while ((line = _parseNextLine(fd)).length() >1)
	{
		std::cout << line << std::endl;
		sep = line.find(':');
		if (sep == line.npos)
			throw 400; // Bad Request
		key = line.substr(0, sep);
		std::transform(key.begin(), key.end(), key.begin(), toUPPER_SNAKE_CASE);
		sep++;
		while (isspace(line[sep]))
			sep++;
		val = line.substr(sep);
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

static std::string get_ext(const std::string& path) {
	size_t sep = path.find('.');
	if (sep == -1ul)
		return "";
	else
		return path.substr(sep + 1);
}

std::string Http::getResponseBody(const std::string& root, const std::map<std::string, std::string>& binaries, const std::vector<std::string>& indexes) {
	std::string file_path;
	file_path = root + _startline.path;
	if (_startline.path.find('.') == -1ul)
	{
		bool found (false);
		for (std::vector<std::string>::const_iterator it = indexes.begin(); it != indexes.end(); it++)
		{
			file_path = root + _startline.path + (*(_startline.path.end() - 1) != '/' ? "/" : "") + *it;
			found = access(file_path.c_str(), R_OK) != -1;
			if (found)
				break;
		}
		if (!found)
			throw 404; // Not Found
		}
	std::cout << "getting file " << file_path << '\n';
	if (access(file_path.c_str(), R_OK) == -1)
		throw 404; // Not Found
	std::string ext (get_ext(file_path));
	std::string bin_f;

	try {
		bin_f = binaries.at(ext);
	}
	catch (const std::out_of_range& e) {
		bin_f = "";
	}

	if (bin_f == "") {
		/* hmtl */
		std::string lines, line;
		std::ifstream file (file_path.c_str());
		while (std::getline(file, line))
			lines += line + '\n';
		return lines;
	}
	else {
		/* cgi */
		add_cgi_env(_header, _startline, file_path);
		return exec_cgi(bin_f, file_path, _header, _socket);
	}
}

static std::string ft_uint_to_string(unsigned int n) {
	std::string ret;
	if (n == 0)
		return "0";
	while (n > 0)
	{
		ret.insert(ret.begin(), n%10 + '0');
		n = n/10;
	}
	return ret;
}

std::string Http::buildResponse(int status, const std::string& body, const std::string& server) {
	std::string res;
	res += "HTTP/1.0 ";
	res += ft_uint_to_string(status);
	res += "\r\ncontent-length: ";
	res += ft_uint_to_string(body.length());
	res += "\r\ndate: ";
	{
		time_t t = time(NULL);
		res += ctime(&t);
	}
	res += "server: " + server + "\r\n";
	if (body.length() > 0)
	{
		res += "\r\n";
		res += body;
	}
	return res;
}

std::string Http::buildErrorHtml(int status, const Server &server) {
	std::string path, ret, line;
	path = server.getRoot() + server.getErrPages().at(status);
	std::cout << "error: " << path << '\n';
	std::ifstream file(path.c_str());
	while(std::getline(file, line))
		ret += line;
	return ret + '\n';
}
