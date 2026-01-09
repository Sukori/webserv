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

		HttpResponse&	operator=(const HttpMessage&);
		HttpResponse&	operator=(const HttpResponse&);

		int		getStatusCode(void) const;

		class MessageIsNotResponse: public std::exception
		{
			public:
				const char *what(void) const throw();
		};
};


#endif