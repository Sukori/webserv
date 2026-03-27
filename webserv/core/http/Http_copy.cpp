#include "Http_copy.hpp"
#include "../cgi/Cgi.hpp"

Http::Http(const std::string& message):
	_pos(0),
	_startline(_parseStartLine(message)),
	_header(_parseHeaders(message)) {
	/* check for 400 Bad Request, 411 Length Required */
	/*if (_header.count("CONTENT_LENGTH") == 0)
		throw 411; // Length Required*/
	//GET without length is valid. https://developer.mozilla.org/en-US/docs/Web/HTTP/Reference/Methods/GET
	//POST, PUT without length are not valid
}

void	Http::verifyMethod(const std::set<std::string>& allowed_methods) const {
	if (allowed_methods.find(_startline.method) == allowed_methods.end())
		throw 405; // Method Not Allowed
}

Http::Http(const Http& o):
	_pos(o._pos),
	_startline(o._startline),
	_header(o._header)
{}

Http::~Http(void) {}

const Http::StartLine&	Http::getStartLine(void) const {return _startline;}
const Http::Header&		Http::getHeader(void) const {return _header;}

Http::StartLine	Http::_parseStartLine(const std::string& message) {
	StartLine	ret;
	size_t		lineEnd = message.find("\r\n", _pos);
	if (lineEnd == message.npos)
		throw 400;
	
	size_t	methodEnd = message.find(' ', _pos);
	if (methodEnd == message.npos || methodEnd >= lineEnd)
		throw 400;
	ret.method = message.substr(_pos, methodEnd - _pos);

	size_t	targetStart = message.find_first_not_of(' ', methodEnd);
	if (targetStart == message.npos || targetStart >= lineEnd)
		throw 400;
	
	size_t	targetEnd = message.find(' ', targetStart);
	if (targetEnd == message.npos || targetEnd >= lineEnd)
		throw 400;
	
	_splitPath(message.substr(targetStart, targetEnd - targetStart), ret);

	_pos = lineEnd + 2;
	return ret;
}

static int toUPPER_SNAKE_CASE(int c) {
	if (c == '-')
		return '_';
	else
		return std::toupper(static_cast<unsigned char>(c));
}

Http::Header	Http::_parseHeaders(const std::string& message) {
	Header	ret;
	size_t	lineEnd = message.find("\r\n", _pos);

	if (lineEnd == message.npos)
		throw 400; // Bad Request
	
	while (_pos < message.size()) {
		lineEnd = message.find("\r\n", _pos);
		if (lineEnd == message.npos)
			throw 400; // Bad Request
		
		//empty line => end of headers
		if (lineEnd == _pos) {
			_pos = lineEnd + 2;
            break;
		}

		std::string	line = message.substr(_pos, lineEnd - _pos);
		size_t		sep = line.find(':');
		if (sep == line.npos || sep == 0)
			throw 400;

		std::string	key = line.substr(0, sep);
		std::transform(key.begin(), key.end(), key.begin(), toUPPER_SNAKE_CASE);
		
		size_t	valueStart = sep + 1;
		while (valueStart < line.size() && std::isspace(static_cast<unsigned char>(line[valueStart])))
			++valueStart;

		std::string	val = line.substr(valueStart);
		ret.insert(std::make_pair(key, val));

		_pos = lineEnd + 2;
	}
	return ret;
}

void			Http::_splitPath(const std::string& path, StartLine& sl) {
	size_t extra, query;
	extra = path.find_first_of(".?");
	if (extra == path.npos)
		extra = path.size();
	if (extra < path.size() && path[extra] == '.')
		extra = path.find_first_of("/?", extra);
	if (extra == path.npos)
		extra = path.size();
	query = path.find('?', extra);
	if (query == path.npos)
		query = path.size();
	sl.path = path.substr(0, extra);
	sl.extra = path.substr(extra, query-extra);
	sl.query = path.substr(query);
}

static std::string get_ext(const std::string& path) {
	size_t sep = path.find('.');
	if (sep == path.npos)
		return "";
	else
		return path.substr(sep + 1);
}

std::string Http::getResponseBody(const std::string& root, const std::map<std::string, std::string>& binaries, const std::vector<std::string>& indexes, int socket) {
	std::string file_path;
	file_path = root + _startline.path;
	if (_startline.path.find('.') == _startline.path.npos)
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
		return exec_cgi(bin_f, file_path, _header, socket);
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

static std::string	reasonPhrase(int status) {
	switch (status) {
        case 200: return " OK";
        case 201: return " Created";
        case 204: return " No Content";
        case 400: return " Bad Request";
        case 403: return " Forbidden";
        case 404: return " Not Found";
        case 405: return " Method Not Allowed";
        case 411: return " Length Required";
        case 500: return " Internal Server Error";
        default:  return " Unknown";
	}
}

std::string Http::buildResponse(int status, const int fd, const std::string& server) {
	/**
	 * startline: HTTP/1.1 status ~status-desc(optional)~
	 * headers:
	 * 		Date:
	 * 		Server:
	 * 		Content-Length:
	 * 		Content-Type:
	 * 
	 * body: ez, just paste
	 */

	std::string	body;
	char		c;

	while (read(fd, &c, 1) > 0) //Cette boucle est bloquante, mais le serveur doit fonctionner de manière non bloquante
	{
		body += c;
	}

	std::string res;
	res += "HTTP/1.1 "; //https://http.dev/1.1
	res += ft_uint_to_string(status);
	res += reasonPhrase(status);
	res += "\r\nContent-Length: ";
	res += ft_uint_to_string(body.length());
	res += "\r\nContent-Type: text/html; charset=UTF-8";
	res += "\r\nDate: ";
	{
		time_t t = time(NULL);
		std::string	d = ctime(&t); //appends a '\n' we do not want
		if (!d.empty() && d[d.size() - 1] == '\n')
			d.erase(d[d.size() - 1]);
		res += d;
	}
	res += "\r\nServer: " + server;
	res += "\r\n\r\n";
	res += body;

	return res;
}
