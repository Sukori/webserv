#include <Http.hpp>
#include <Cgi.hpp>
#include <Client.hpp>

Http::Http(const ByteString& message):
	_pos(0),
	_startline(_parseStartLine(message)),
	_header(_parseHeaders(message)),
	_body(_parseBody(message)) {
	/* check for 400 Bad Request, 411 Length Required */
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
const ByteString&		Http::getRequestBody(void) const {return _body;}

Http::StartLine	Http::_parseStartLine(const ByteString& message) {
	StartLine	ret;
	size_t		lineEnd = message.find("\r\n", _pos);
	if (lineEnd == message.npos)
		throw 400; // Bad Request
	
	size_t	methodEnd = message.find(' ', _pos);
	if (methodEnd == message.npos || methodEnd >= lineEnd)
		throw 400; // Bad Request
	ret.method = message.substr(_pos, methodEnd - _pos).to_string();
	
	size_t	targetStart = message.find_first_not_of(' ', methodEnd);
	if (targetStart == message.npos || targetStart >= lineEnd)
		throw 400; // Bad Request
	
	size_t	targetEnd = message.find(' ', targetStart);
	if (targetEnd == message.npos || targetEnd >= lineEnd)
		throw 400; // Bad Request
	
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
			throw 400; // Bad Request

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

/* reads all data on fd and closes it *
static ByteString	read_all(int fd) {
	byte	buf[BUFFER_SIZE];
	size_t				bytes;
	ByteString			ret;
	while ((bytes = read(fd, buf, BUFFER_SIZE)) > 0)
		ret.append(buf, bytes);
	close(fd);
	return ret;
}*/

Resource	Http::getResponseBody(const Location& loc, const Server& server, int& status) {
	std::string new_path (_startline.path);
	new_path.replace(0, loc.getRoute().length(), loc.getRoot()); // replace the user route with the actual root
	std::string root (server.getRoot());
	std::string file_path (root + new_path);


	if (_startline.method == "DELETE")
	{
		if (access(file_path.c_str(), W_OK) != -1)
		{
			std::remove(file_path.c_str());
			throw 204;
		}
		else
			throw 404;
	}

	if (loc.getReturn().begin()->first != 0)
	{
		std::cout << loc.getReturn().begin()->second << '\n';
		status = loc.getReturn().begin()->first;
		return Resource(ByteString("location:").append(loc.getReturn().begin()->second.c_str()).append("\r\n"));
	}

	if (new_path.find('.') == new_path.npos)
	{
		bool found (false);
		const std::vector<std::string> &indexes = server.getIndex();
		for (std::vector<std::string>::const_iterator it = indexes.begin(); it != indexes.end(); it++)
		{
			file_path = root + new_path + *it;
			found = access(file_path.c_str(), R_OK) != -1;
			if (found)
				break;
		}
		if (!found)
		{
			if (loc.getAutoIndex())
			{
				status = 200;
				return Resource(autoindex(_startline.path, root + new_path));
			}
			else
				throw 404; // Not Found
		}
	}
	else if (access(file_path.c_str(), R_OK) == -1)
			throw 404; // Not Found

	std::cout << "getting file " << file_path << '\n';

	std::string ext (get_ext(file_path));
	std::string bin_f;

	try {
		bin_f = server.getCgiBins().at(ext);
	} catch (const std::out_of_range& e) {
		bin_f = "";
	}

	std::cout << "status set\n";
	status = 200;
	if (bin_f == "") {
		/* hmtl */
		int	fd = open(file_path.c_str(), O_RDONLY);
		return Resource(fd, "\r\n"); // added CRLF to account for added header fileds in cgi output
	}
	else {
		/* cgi */
		std::cout << "starting cgi\n";
		add_cgi_env(_header, server, _startline, file_path);
		return Resource(exec_cgi(bin_f, file_path, _header, _body));
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
		case 413: return " Request Entity Too Large";
		case 500: return " Internal Server Error";
		default:  return " Unknown";
	}
}

ByteString	Http::buildResponse(const ByteString& body, int status, const std::string& server_name) {
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

	size_t len;
	{
		size_t tmp = body.find("\r\n\r\n");
		len = body.length() - ((tmp != body.npos) ? tmp + 4 : 2);
	}
	ByteString res;
	res.append("HTTP/1.1 "); //https://http.dev/1.1
	res.append(ft_uint_to_string(status).c_str());
	res.append(reasonPhrase(status).c_str());
	res.append("\r\nContent-Length: ");
	res.append(ft_uint_to_string(len).c_str());
	/* res.append("\r\nContent-Type: text/html; charset=UTF-8"); */ // we don't necessarily pass html
	res.append("\r\nDate: ");
	{
		time_t t = time(NULL);
		std::string	d = ctime(&t); //appends a '\n' we do not want
		if (!d.empty() && d[d.size() - 1] == '\n')
			d.erase(d[d.size() - 1]);
		res.append(d.c_str());
	}
	res.append(std::string("\r\nServer: ").append(server_name).c_str());
	res.append("\r\n");
	res.append(body);

	return res;
}

Resource Http::buildErrorHtml(int status, const Server &server) {
	std::string path;
	std::cout << server.getRoot();
	path = server.getRoot() + server.getErrPages().at(status);
	int fd = open(path.c_str(), O_RDONLY);
	if (fd < 0)
		return Resource();
	else
		return Resource(fd);
}

bool	Http::checkRequestComplete(const ByteString& request) {
	if (request.find("POST") == 0)
	{
		size_t cur = request.find("Content-Length");
		if (cur == request.npos)
			return false;
		cur = request.find_first_not_of(' ', cur + 15);
		size_t len = std::atol((const char*) request.data() + cur);
		cur = request.find("\r\n\r\n", cur);
		return cur + len + 4 == request.length();
	}
	else
	{
		return request.find("\r\n", request.length() - 2) != request.npos;
	}
}

static char	hexToInt(byte b) {
	if ('0' <= b && b <= '9')
		return b - '0';
	else if ('A' <= b && b <= 'F')
		return b - 'A' + 10;
	else
		return -1;
}

static ByteString	parseChunk(const ByteString& s, size_t& cur) {
	size_t chunk_len = 0;
	while (cur < s.find("\r\n", cur))
	{
		chunk_len <<= 4;
		chunk_len |= hexToInt(s[cur]);
		cur++;
	}
	cur += 2;
	ByteString ret (s.substr(cur, chunk_len));
	cur += 2 + chunk_len;
	return ret;
}

void	Http::dechunk(void) {
	ByteString newbody, tmp;
	size_t cur = 0;
	do {
		tmp = parseChunk(_body, cur);
		newbody.append(tmp);
	} while (tmp.length() > 0);
	_body = newbody;
	_header["CONTENT_LENGTH"] = ft_uint_to_string(_body.length());
}

bool	Http::isChunked(void) const {
	return _header.count("TRANSFER_ENCODING") > 0 && _header.at("TRANSFER_ENCODING").find("chunked") != std::string::npos;
}
