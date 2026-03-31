#ifndef BYTESTRING_HPP
#define BYTESTRING_HPP
#include <cstddef>
#include <cstring>
#include <iostream>

typedef char byte;

class ByteString
{
	public:
	ByteString(void);
	ByteString(size_t);
	ByteString(const byte* s, size_t len);
	ByteString(const char* s);
	ByteString(const ByteString&);
	~ByteString(void);

	ByteString&	operator=(const ByteString&);

	size_t		append(const byte*, size_t);
	size_t		append(const char*);
	size_t		append(const byte);
	size_t		append(const ByteString&);
	const byte*	data(void) const;

	const byte&	operator[](size_t) const;
	byte&		operator[](size_t);

	bool		empty(void) const;
	size_t		length(void) const;
	size_t		capacity(void) const;
	
	size_t		reserve(size_t new_cap);
	size_t		shrink_to_fit(void);
	void		clear(void);

	ByteString	substr(size_t pos, size_t len = npos);

	size_t		find(const byte*, size_t len, size_t pos = 0) const;
	size_t		find(byte, size_t pos = 0) const;

	size_t		find_first_not_of(const byte*, size_t len, size_t pos = 0) const;
	size_t		find_last_not_of(const byte*, size_t len, size_t pos = 0) const;
	size_t		find_first_of(const byte*, size_t len, size_t pos = 0) const;
	size_t		find_last_of(const byte*, size_t len, size_t pos = 0) const;

	
	size_t		find(const char*, size_t pos = 0) const;
	size_t		find_first_not_of(const char*, size_t pos = 0) const;
	size_t		find_last_not_of(const char*, size_t pos = 0) const;
	size_t		find_first_of(const char*, size_t pos = 0) const;
	size_t		find_last_of(const char*, size_t pos = 0) const;

	static const size_t npos;

	private:
	byte*	_data;
	size_t	_len;
	size_t	_cur;
};

std::ostream&	operator<<(std::ostream& os, const ByteString&);

#endif
