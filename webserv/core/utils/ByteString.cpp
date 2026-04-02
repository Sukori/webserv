#include "ByteString.hpp"

const size_t ByteString::npos = -1ul;

ByteString::ByteString(void):						_cap(0),				_len(0),				_data(0)						{}
ByteString::ByteString(size_t l):					_cap(l),				_len(0),				_data(new byte[l])				{}
ByteString::ByteString(const byte* s, size_t l):	_cap(l),				_len(l),				_data(new byte[l])				{std::memmove(_data, s, _len);}
ByteString::ByteString(const char* s):				_cap(std::strlen(s)),	_len(std::strlen(s)),	_data(new byte[std::strlen(s)])	{std::memmove(_data, s, _len);}
ByteString::ByteString(const ByteString& o):		_cap(o._cap),			_len(o._len),			_data(new byte[o._cap])			{std::memmove(_data, o._data, _len);}
ByteString::~ByteString(void) {delete[] _data;}

std::string	ByteString::to_string(void) const {return std::string((const char*) _data, _len);}

ByteString&	ByteString::operator=(const ByteString& o) {
	if (&o == this)
		return *this;
	_cap = o._cap;
	_len = o._len;
	/* _data = o._data; */
	delete[] _data;
	_data = new byte[_cap];
	std::memmove(_data, o._data, _len);
	return *this;
}

ByteString&	ByteString::append(const byte* s, size_t len) {
	if (_len + len > _cap)
		reserve(_len + len);
	std::memmove(_data + _len, s, len);
	_len += len;
	return *this;
}

ByteString&	ByteString::append(const char* s) {return append((const byte*) s, std::strlen(s));}
ByteString&	ByteString::append(const byte c) {return append(&c, 1);}
ByteString&	ByteString::append(const ByteString& s) {return append(s._data, s._len);}

const byte&	ByteString::operator[](size_t i) const {return _data[i];}
byte&		ByteString::operator[](size_t i) {return _data[i];}

bool		ByteString::empty(void) const {return _len == 0;}
const byte*	ByteString::data(void) const {return _data;}
size_t		ByteString::length(void) const {return _len;}
size_t		ByteString::capacity(void) const {return _cap;}

size_t		ByteString::reserve(size_t new_cap) {
	byte* tmp = new byte[new_cap];
	std::memmove(tmp, _data, (new_cap > _len) ? _len : new_cap);
	delete[] _data;
	_data = tmp;
	_cap = new_cap;
	return _cap;
}

size_t		ByteString::shrink_to_fit(void) {return reserve(_len);}

void		ByteString::clear(void) {_len = 0;}

ByteString	ByteString::substr(size_t pos, size_t len) const {
	len = (_len - pos > len) ? len : _len - pos;
	return ByteString(_data + pos, len);
}

size_t		ByteString::find(const byte* s, size_t len, size_t pos) const {
	for (size_t i = pos; i + len <= _len; i++)
		if (std::memcmp(s, _data + i, len) == 0)
			return i;
	return npos;
}

size_t		ByteString::find_first_not_of(const byte* s, size_t len, size_t pos) const {
	for (size_t i = pos; i < _len; i++)
		if (!std::memchr(s, _data[i], len))
			return i;
	return npos;
}

size_t		ByteString::find_last_not_of(const byte* s, size_t len, size_t pos) const {
	size_t	ret = npos;
	for (size_t i = pos; i < _len; i++)
		if (!std::memchr(s, _data[i], len))
			ret = i;
	return ret;
}

size_t		ByteString::find_first_of(const byte* s, size_t len, size_t pos) const {
	for (size_t i = pos; i < _len; i++)
		if (std::memchr(s, _data[i], len))
			return i;
	return npos;
}

size_t		ByteString::find_last_of(const byte* s, size_t len, size_t pos) const {
	size_t	ret = npos;
	for (size_t i = pos; i < _len; i++)
		if (std::memchr(s, _data[i], len))
			ret = i;
	return ret;
}

size_t		ByteString::find(const char* s, size_t pos) const {return find((const byte*) s, std::strlen(s), pos);}
size_t		ByteString::find_first_not_of(const char* s, size_t pos) const {return find_first_not_of((const byte*) s, std::strlen(s), pos);}
size_t		ByteString::find_last_not_of(const char* s, size_t pos) const {return find_last_not_of((const byte*) s, std::strlen(s), pos);}
size_t		ByteString::find_first_of(const char* s, size_t pos) const {return find_first_of((const byte*) s, std::strlen(s), pos);}
size_t		ByteString::find_last_of(const char* s, size_t pos) const {return find_last_of((const byte*) s, std::strlen(s), pos);}

size_t		ByteString::find(byte c, size_t pos) const {return find(&c, 1, pos);}
size_t		ByteString::find_first_not_of(byte c, size_t pos) const {return find_first_not_of(&c, 1, pos);}
size_t		ByteString::find_last_not_of(byte c, size_t pos) const {return find_last_not_of(&c, 1, pos);}
size_t		ByteString::find_first_of(byte c, size_t pos) const {return find_first_of(&c, 1, pos);}
size_t		ByteString::find_last_of(byte c, size_t pos) const {return find_last_of(&c, 1, pos);}

std::ostream&	operator<<(std::ostream& os, const ByteString& s) {
	for (size_t i = 0; i < s.length(); i++)
	{
		if (std::isprint(s[i]))
			os << (char) s[i];
	}
	return os;
}
