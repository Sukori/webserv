#ifndef BYTESTRING_HPP
#define BYTESTRING_HPP
#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>

typedef unsigned char byte;

class ByteString
{
	public:
	ByteString(void);
	ByteString(size_t);
	ByteString(const byte* s, size_t len);
	ByteString(const char* s);
	ByteString(const ByteString&);
	~ByteString(void);

	// @brief makes a std::string out of this with no regard for the bytes copied
	std::string	to_string(void) const;

	// @brief copies contents of ByteString o over to this
	ByteString&	operator=(const ByteString&);

	// @brief appends len bytes from s to this
	ByteString&	append(const byte*, size_t);
	// @brief appends c string parameter to this
	ByteString&	append(const char*);
	// @brief appends char parameter to this
	ByteString&	append(const byte);
	// @brief appends ByteString parameter to this
	ByteString&	append(const ByteString&);

	const byte&	operator[](size_t) const;
	byte&		operator[](size_t);

	// @brief true if the string is empty
	bool		empty(void) const;
	// @brief getter for the pointer to the bytes
	const byte*	data(void) const;
	// @brief getter for the length of the string
	size_t		length(void) const;
	// @brief getter for the capacity of the array buffer
	size_t		capacity(void) const;
	
	size_t		reserve(size_t new_cap);
	size_t		shrink_to_fit(void);
	void		clear(void);

	ByteString	substr(size_t pos = 0, size_t len = npos) const;

	size_t		find(const byte*, size_t len, size_t pos = 0) const;
	size_t		find_first_not_of(const byte*, size_t len, size_t pos = 0) const;
	size_t		find_last_not_of(const byte*, size_t len, size_t pos = 0) const;
	size_t		find_first_of(const byte*, size_t len, size_t pos = 0) const;
	size_t		find_last_of(const byte*, size_t len, size_t pos = 0) const;
	
	size_t		find(const char*, size_t pos = 0) const;
	size_t		find_first_not_of(const char*, size_t pos = 0) const;
	size_t		find_last_not_of(const char*, size_t pos = 0) const;
	size_t		find_first_of(const char*, size_t pos = 0) const;
	size_t		find_last_of(const char*, size_t pos = 0) const;
	
	size_t		find(byte, size_t pos = 0) const;
	size_t		find_first_not_of(byte, size_t pos = 0) const;
	size_t		find_last_not_of(byte, size_t pos = 0) const;
	size_t		find_first_of(byte, size_t pos = 0) const;
	size_t		find_last_of(byte, size_t pos = 0) const;

	static const size_t npos;

	private:
	size_t	_cap;
	size_t	_len;
	byte*	_data;
};

std::ostream&	operator<<(std::ostream& os, const ByteString&);

#endif
