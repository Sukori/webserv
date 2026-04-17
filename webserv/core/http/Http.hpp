#ifndef HTTP_HPP
#define HTTP_HPP

#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <sys/stat.h>
#include <exception>
#include <ctime>
#include <cctype>
#include <ByteString.hpp>
#include <Configuration.hpp>
#include <autoindex.hpp>
#include <Resource.hpp>
#include <other.hpp>

class Http {
	public:
		typedef std::map<std::string, std::string> Header;
		struct StartLine
		{
			std::string	path;
			std::string	extra;
			std::string	query;
			std::string	method;
		};

		//Http(int socket);
		Http(const ByteString& message);
		Http(const Http&);
		~Http(void);

		//int					getSocket(void) const;
		const StartLine&	getStartLine(void) const;
		const Header&		getHeader(void) const;
		const ByteString&	getRequestBody(void) const;
		Resource			getResponseBody(const Location& route, const Server& server, int& status);
		void				verifyMethod(const std::set<std::string>& allowed_methods) const;
		void				dechunk(void);
		bool				isChunked(void) const;

		static bool			checkRequestComplete(ByteString& request);

		static Resource		buildErrorHtml(int status, const Server &server);
		static ByteString	buildResponse(const ByteString& body, int status, const std::string& server_name);
	private:
		Http(void);
		
		StartLine			_parseStartLine(const ByteString& message);
		Header				_parseHeaders(const ByteString& message);
		static void			_splitPath(const ByteString& path, StartLine& sl);

		ByteString			_parseBody(const ByteString& message);

		size_t		_pos;
		StartLine	_startline;
		Header		_header;
		ByteString	_body;
};

#endif
