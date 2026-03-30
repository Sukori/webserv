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
#include <sys/stat.h>
#include <exception>
#include <ctime>
#include <cctype>
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
		Http(const std::string& message);
		Http(const Http&);
		~Http(void);

		//int					getSocket(void) const;
		const StartLine&	getStartLine(void) const;
		const Header&		getHeader(void) const;
		std::string			getResponseBody(const std::string& root, const std::map<std::string, std::string>& binaries, const std::vector<std::string>& indexes, int socket);
		void				verifyMethod(const std::set<std::string>& allowed_methods) const;

		static std::string	buildResponse(int status, const std::string& server);
	private:
		Http(void);
		
		//static StartLine	_parseStartLine(int fd);
		StartLine			_parseStartLine(const std::string& message);
		//static Header		_parseHeaders(int fd);
		Header				_parseHeaders(const std::string& message);
		//static std::string	_parseNextLine(int fd);
		static void			_splitPath(const std::string& path, StartLine& sl);

		//const int	_socket;
		size_t		_pos;
		StartLine	_startline;
		Header		_header;
};

#endif
