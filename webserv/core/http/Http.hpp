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
#include "../utils/ByteString.hpp"
#include "../../config/Configuration.hpp"

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
		ByteString			getResponseBody(const Location& route, const std::map<std::string, std::string>& binaries, const Server& server, int& status);
		void				verifyMethod(const std::set<std::string>& allowed_methods) const;

		static ByteString	buildErrorHtml(int status, const Server &server);
		static ByteString	buildResponse(const ByteString& body, int status, const std::string& server);
	private:
		Http(void);
		
		//static StartLine	_parseStartLine(int fd);
		StartLine			_parseStartLine(const ByteString& message);
		//static Header		_parseHeaders(int fd);
		Header				_parseHeaders(const ByteString& message);
		//static std::string	_parseNextLine(int fd);
		static void			_splitPath(const ByteString& path, StartLine& sl);

		ByteString			_parseBody(const ByteString& message);

		//const int	_socket;
		size_t		_pos;
		StartLine	_startline;
		Header		_header;
		ByteString	_body;
};

#endif
