#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "HttpMessage.hpp"

class HttpResponse: public HttpMessage
{
	public:
		HttpResponse(void);
		HttpResponse(const HttpMessage&);
		HttpResponse(const HttpResponse&);
		~HttpResponse(void);

		class MessageIsNotResponse: public std::exception
		{
			public:
				const char *what(void) const throw();
		};
};


#endif