#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <set>

#include "HttpMessage.hpp"

class HttpRequest: public HttpMessage
{
	public:
		HttpRequest(const std::set<std::string>& allowed_methods);
		HttpRequest(const HttpMessage&, const std::set<std::string>& allowed_methods);
		HttpRequest(const HttpRequest&);
		~HttpRequest(void);
		HttpRequest&	operator=(const HttpMessage&);
		HttpRequest&	operator=(const HttpRequest&);

		std::string	getMethod(void) const;
		std::string	getPath(void) const;

		class MessageIsNotRequest: public std::exception
		{
			public:
				const char *what(void) const throw();
		};

	private:
		const std::set<std::string> _allowed_methods;
};

#endif