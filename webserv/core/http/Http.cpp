#include "Http.hpp"
#include "../cgi/Cgi.hpp"

Http::Http(const ByteString& message):
	_pos(0),
	_startline(_parseStartLine(message)),
	_header(_parseHeaders(message)),
	_body(_parseBody(message)) {
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
	_header(o._header),
	_body(o._body) {}

Http::~Http(void) {}

const Http::StartLine&	Http::getStartLine(void) const {return _startline;}
const Http::Header&		Http::getHeader(void) const {return _header;}

Http::StartLine	Http::_parseStartLine(const ByteString& message) {
	StartLine	ret;
	size_t		lineEnd = message.find("\r\n", _pos);
	if (lineEnd == message.npos)
		throw 400;
	
	size_t	methodEnd = message.find(' ', _pos);
	if (methodEnd == message.npos || methodEnd >= lineEnd)
		throw 400;
	ret.method = message.substr(_pos, methodEnd - _pos).to_string();
	
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

Http::Header	Http::_parseHeaders(const ByteString& message) {
	Header	ret;
	size_t	lineEnd = message.find("\r\n", _pos);

	if (lineEnd == message.npos)
		throw 400; // Bad Request
	
	while (_pos < message.length()) {
		lineEnd = message.find("\r\n", _pos);
		if (lineEnd == message.npos)
			throw 400; // Bad Request
		
		//empty line => end of headers
		if (lineEnd == _pos) {
			_pos = lineEnd + 2;
			break;
		}
		
		std::string	line = message.substr(_pos, lineEnd - _pos).to_string();
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

void			Http::_splitPath(const ByteString& path, StartLine& sl) {
	size_t extra, query;
	extra = path.find_first_of(".?");
	if (extra == path.npos)
		extra = path.length();
	if (extra < path.length() && path[extra] == '.')
		extra = path.find_first_of("/?", extra);
	if (extra == path.npos)
		extra = path.length();
	query = path.find('?', extra);
	if (query == path.npos)
		query = path.length();
	sl.path = path.substr(0, extra).to_string();
	sl.extra = path.substr(extra, query-extra).to_string();
	sl.query = path.substr(query).to_string();
}

ByteString		Http::_parseBody(const ByteString& message) {
	if (_startline.method == "POST" && _header.count("CONTENT_LENGTH") == 0)
		throw 411; // Length Required
	return message.substr(_pos, std::atoi(_header["CONTENT_LENGTH"].c_str()));
}

static std::string get_ext(const std::string& path) {
	size_t sep = path.rfind('.');
	if (sep == path.npos)
		return "";
	else
		return path.substr(sep + 1);
}

#define BUFFER_SIZE 16
/* reads all data on fd and closes it */
static ByteString	read_all(int fd) {
	byte	buf[BUFFER_SIZE];
	size_t				bytes;
	ByteString			ret;
	while ((bytes = read(fd, buf, BUFFER_SIZE)) > 0)
		ret.append(buf, bytes);
	close(fd);
	return ret;
}

ByteString	Http::getResponseBody(const std::string& route, const std::map<std::string, std::string>& binaries, const Server& server) {
	std::string file_path;
	std::string root = server.getRoot() + route;
	file_path = root + _startline.path;
	if (_startline.path.rfind('.') == _startline.path.npos)
	{
		bool found (false);
		const std::vector<std::string> &indexes = server.getIndex();
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
	} catch (const std::out_of_range& e) {
		bin_f = "";
	}

	if (bin_f == "") {
		/* hmtl */
		int	fd = open(file_path.c_str(), O_RDONLY);
		return read_all(fd);
	}
	else {
		/* cgi */
		int pfds[2];
		pipe(pfds);
		write(pfds[1], _body.data(), _body.length());
		close(pfds[1]);
		add_cgi_env(_header, server, _startline, file_path);
		return read_all(exec_cgi(bin_f, file_path, _header, pfds[0]));
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

ByteString	Http::buildResponse(const ByteString& body, int status, const std::string& server) {
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

	//std::string	body = reasonPhrase(status);

	ByteString res;
	res.append("HTTP/1.1 "); //https://http.dev/1.1
	res.append(ft_uint_to_string(status).c_str());
	res.append(reasonPhrase(status).c_str());
	res.append("\r\nContent-Length: ");
	res.append(ft_uint_to_string(body.length()).c_str());
	/* res.append("\r\nContent-Type: text/html; charset=UTF-8"); */ // we don't necessarly pass html
	res.append("\r\nDate: ");
	{
		time_t t = time(NULL);
		std::string	d = ctime(&t); //appends a '\n' we do not want
		if (!d.empty() && d[d.size() - 1] == '\n')
			d.erase(d[d.size() - 1]);
		res.append(d.c_str());
	}
	res.append(("\r\nServer: " + server).c_str());
	res.append("\r\n\r\n");
	res.append(body);

	return res;
}

ByteString Http::buildErrorHtml(int status, const Server &server) {
	std::string path;
	std::cout << server.getRoot();
	path = server.getRoot() + server.getErrPages().at(status);
	std::cout << "error: " << path << '\n';
	int fd = open(path.c_str(), O_RDONLY);
	if (fd < 0)
		return ByteString();
	else
		return read_all(fd);
}
