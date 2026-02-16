#ifndef HTTP_HPP
#define HTTP_HPP

#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <map>
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

		Http(int socket, const std::set<std::string>& allowed_methods);
		Http(const Http&);
		~Http(void);

		bool				isValidRequest(std::string root) const;
		int					getSocket(void) const;
		const StartLine&	getStartLine(void) const;
		const Header&		getHeader(void) const;

	private:
		Http(void);
		static StartLine	_getStartLine(int fd);
		static Header		_getHeaders(int fd);
		static std::string	_getNextLine(int fd);
		static void			_splitPath(const std::string& path, StartLine& sl);

		const int					_socket;
		StartLine					_startline;
		Header						_header;
		const std::set<std::string>	_allowed_methods;
};

#endif