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

		Http(int socket);
		Http(const Http&);
		~Http(void);

		int					getSocket(void) const;
		const StartLine&	getStartLine(void) const;
		const Header&		getHeader(void) const;
		std::string			getResponseBody(const std::string& root, const std::map<std::string, std::string>& binaries, const std::vector<std::string>& indexes);
		void				verifyMethod(const std::set<std::string>& allowed_methods) const;

		static std::string	buildResponse(int status, const std::string& body, const std::string& server);
	private:
		Http(void);
		
		static StartLine	_parseStartLine(int fd);
		static Header		_parseHeaders(int fd);
		static std::string	_parseNextLine(int fd);
		static void			_splitPath(const std::string& path, StartLine& sl);

		const int	_socket;
		StartLine	_startline;
		Header		_header;
};

#endif