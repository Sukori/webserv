#include "HttpResponse.hpp"

const char	*HttpResponse::MessageIsNotResponse::what(void) const throw() {return "trying to convert a non-response message into a response";}

HttpResponse::HttpResponse(void) {}
HttpResponse::HttpResponse(const HttpMessage& other): HttpMessage(other) {}
HttpResponse::HttpResponse(const HttpResponse& msg): HttpMessage(msg) {
	if (msg != RESPONSE)
		throw (MessageIsNotResponse());
}

HttpResponse&	HttpResponse::operator=(const HttpMessage& msg) {
	if (this != &msg)
	{
		if (msg != RESPONSE)
			throw (MessageIsNotResponse());
		_start_line = msg.getStartLine();
		_headers = msg.getHeader();
		_body = msg.getBody();
	}
	return *this;
}

HttpResponse&	HttpResponse::operator=(const HttpResponse& other) {
	if (this != &other)
	{
		_start_line = other._start_line;
		_headers = other._headers;
		_body = other._body;
	}
	return *this;
}

int HttpResponse::getStatusCode() const {
	size_t	i = 0;
	i = _start_line.find_first_of("\t \n", i);
	i = _start_line.find_first_not_of("\t \n", i);
	return std::atoi(_start_line.c_str() + i);
}

HttpResponse::~HttpResponse(void) {}
