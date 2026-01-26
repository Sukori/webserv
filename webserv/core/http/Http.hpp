#ifndef HTTP_HPP
#define HTTP_HPP

#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <map>
#include <algorithm>
#include <iostream>

class Http {
	public:
		typedef std::map<std::string, std::string> Header;

		struct StartLine
		{
			std::string	path;
			std::string	method;
		};

		static StartLine	getStartLine(int fd);
		static Header		getHeaders(int fd);

	private:
		static std::string	getNextLine(int fd);
		Http(void);
		Http(const Http&);
		~Http(void);
};


#endif