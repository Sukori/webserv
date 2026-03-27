#ifndef BYTESTRING_HPP
#define BYTESTRING_HPP
#include <cstdint>
#include <cstddef>
#include <cstring>

typedef uint8_t byte;

class ByteString
{
	public:
	ByteString(void);
	ByteString(size_t);
	ByteString(const ByteString&);
	~ByteString(void);

	ByteString&	operator=(const ByteString&);

	size_t		append(byte*, size_t);
	const byte*	data(void) const;

	const byte&	operator[](size_t) const;
	byte&		operator[](size_t);

	bool		empty(void) const;
	size_t		length(void) const;
	size_t		capacity(void) const;

	size_t		reserve(void);
	size_t		shrink_to_fit(void);

	void		clear(void);

	private:
	byte*	_data;
	size_t	_len;
	size_t	_cur;
};

#endif