#ifndef RESOURCE_HPP
#define RESOURCE_HPP
#include <ByteString.hpp>
#include <Logger.hpp>

class Resource
{
public:
	Resource(int fd, const ByteString& content = "");
	Resource(const ByteString& content = "");
	Resource(const Resource& o);
	~Resource(void);

	Resource& operator=(const Resource& o);


	bool				readChunk(Logger*& logger);

	int					getFd(void) const;
	bool				done(void) const;
	const	ByteString&	getContent(void)const;

private:
	int			_fd;
	bool		_done;
	ByteString	_content;
};

#endif
