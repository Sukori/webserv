#include "HttpRequest.hpp"

const char	*HttpRequest::MessageIsNotRequest::what(void) const throw() {return "trying to convert a non-request message into a request";}

HttpRequest::HttpRequest(const std::set<std::string>& allowed_methods): _allowed_methods(allowed_methods) {}
HttpRequest::HttpRequest(const HttpMessage& msg, const std::set<std::string>& allowed_methods): HttpMessage(msg), _allowed_methods(allowed_methods) {
	if (msg != REQUEST)
		throw (MessageIsNotRequest());
}

HttpRequest::HttpRequest(const HttpRequest& other): HttpMessage(other), _allowed_methods(other._allowed_methods) {}
HttpRequest::~HttpRequest(void) {}
HttpRequest&	HttpRequest::operator=(const HttpMessage& msg) {
	if (this != &msg)
	{
		if (msg != REQUEST)
			throw (MessageIsNotRequest());
		_start_line = msg.getStartLine();
		_headers = msg.getHeader();
		_body = msg.getBody();
	}
	return *this;
}

HttpRequest&	HttpRequest::operator=(const HttpRequest& other) {
	if (this != &other)
	{
		_start_line = other.getStartLine();
		_headers = other.getHeader();
		_body = other.getBody();
	}
	return *this;
}

std::string		HttpRequest::getMethod(void) const {
	if (_start_line == "")
		return "";
	size_t i = _start_line.find_first_of("\t \n");
	std::string	sub = _start_line.substr(0, i);
	if (_allowed_methods.find(sub) != _allowed_methods.end())
		return sub;
	else
		return "";
}

std::string		HttpRequest::getPath(void) const {
	if (_start_line == "")
		return "";
	size_t start = _start_line.find('/');
	size_t end = _start_line.find_first_of("\t \n", start);
	return _start_line.substr(start, end - start);
}
