#include "HttpResponse.hpp"

const char	*HttpResponse::MessageIsNotResponse::what(void) const throw() {return "trying to convert a non-response message into a response";}

HttpResponse::HttpResponse(void) {}
HttpResponse::HttpResponse(const HttpMessage& other): HttpMessage(other) {}
HttpResponse::HttpResponse(const HttpResponse& msg): HttpMessage(msg) {
	if (msg != RESPONSE)
		throw (MessageIsNotResponse());
}

HttpResponse::~HttpResponse(void) {}
