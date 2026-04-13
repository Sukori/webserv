#include "Resource.hpp"
#include <Client.hpp>

	Resource::Resource(int fd, const ByteString& content): _fd(fd), _done(false), _content(content) {}
	Resource::Resource(const ByteString& content): _fd(-1), _done(true), _content(content) {}
	Resource::Resource(const Resource& o): _fd(o._fd), _done(o._done), _content(o._content) {}
	Resource::~Resource(void) {}

	Resource&			Resource::operator=(const Resource& o) {
		_fd = o._fd;
		_done = o._done;
		_content = o._content;
		return *this;
	}

	bool				Resource::readChunk(void) {
		if (_done)
			return true;
		byte	buffer[BUFFER_SIZE];
		ssize_t	bytesRead = read(_fd, buffer, BUFFER_SIZE);
		if (bytesRead > 0) {
			_content.append(buffer, bytesRead);
			if (bytesRead < BUFFER_SIZE)
			{
				_done = true;
				close(_fd);
				_fd = -1;
			}
		} else {
			std::cout << "reading error\n";
		}
		return _done;
	}

	bool				Resource::done(void) const {return _done;}
	int					Resource::getFd(void) const {return _fd;}
	const	ByteString&	Resource::getContent(void) const {return _content;}
