#include "ByteString.hpp"

const size_t ByteString::npos = -1ul;

ByteString::ByteString(void):						_len(0),				_cur(0),				_data(0)						{}
ByteString::ByteString(size_t l):					_len(l),				_cur(0),				_data(new byte[l])				{}
ByteString::ByteString(const byte* s, size_t l):	_len(l),				_cur(l),				_data(new byte[l])				{std::memmove(_data, s, _cur);}
ByteString::ByteString(const char* s):				_len(std::strlen(s)),	_cur(std::strlen(s)),	_data(new byte[std::strlen(s)])	{std::memmove(_data, s, _cur);}
ByteString::ByteString(const ByteString& o):		_len(o._len),			_cur(o._cur),			_data(new byte[o._len])			{std::memmove(_data, o._data, _cur);}
ByteString::~ByteString(void) {delete[] _data;}

std::string	ByteString::to_string(void) const {return std::string((const char*) _data, _cur);}

ByteString&	ByteString::operator=(const ByteString& o) {
	if (&o == this)
		return *this;
	_len = o._len;
	_cur = o._cur;
	/* _data = o._data; */
	delete[] _data;
	_data = new byte[_len];
	std::memmove(_data, o._data, _cur);
	return *this;
}

size_t		ByteString::append(const byte* s, size_t len) {
	if (_cur + len > _len)
		reserve(_cur + len);
	std::memmove(_data + _cur, s, len);
	_cur += len;
	return _len;
}

size_t		ByteString::append(const char* s) {return append((const byte*) s, std::strlen(s));}
size_t		ByteString::append(const byte c) {return append(&c, 1);}

size_t		ByteString::append(const ByteString& s) {
	return append(s._data, s._cur);
}

const byte*	ByteString::data(void) const {
	return _data;
}

const byte&	ByteString::operator[](size_t i) const {
	return _data[i];
}

byte&		ByteString::operator[](size_t i) {
	return _data[i];
}

bool		ByteString::empty(void) const {
	return _cur == 0;
}

size_t		ByteString::length(void) const {
	return _cur;
}

size_t		ByteString::capacity(void) const {
	return _len;
}

size_t		ByteString::reserve(size_t new_cap) {
	byte* tmp = new byte[new_cap];
	std::memmove(tmp, _data, (new_cap > _cur) ? _cur : new_cap);
	delete[] _data;
	_data = tmp;
	_len = new_cap;
	return _len;
}

size_t		ByteString::shrink_to_fit(void) {
	return reserve(_cur);
}

void		ByteString::clear(void) {
	_cur = 0;
}

ByteString	ByteString::substr(size_t pos, size_t len) const {
	len = (_cur - pos > len) ? len : _cur - pos;
	return ByteString(_data + pos, len);
}

size_t		ByteString::find(const byte* s, size_t len, size_t pos) const {
	for (size_t i = pos; i + len <= _cur; i++)
		if (std::memcmp(s, _data + i, len) == 0)
			return i;
	return npos;
}

size_t		ByteString::find_first_not_of(const byte* s, size_t len, size_t pos) const {
	for (size_t i = pos; i <= _cur; i++)
		if (!std::memchr(s, _data[i], len))
			return i;
	return npos;
}

size_t		ByteString::find_last_not_of(const byte* s, size_t len, size_t pos) const {
	size_t	ret = npos;
	for (size_t i = pos; i <= _cur; i++)
		if (!std::memchr(s, _data[i], len))
			ret = i;
	return ret;
}

size_t		ByteString::find_first_of(const byte* s, size_t len, size_t pos) const {
	for (size_t i = pos; i <= _cur; i++)
		if (std::memchr(s, _data[i], len))
			return i;
	return npos;
}

size_t		ByteString::find_last_of(const byte* s, size_t len, size_t pos) const {
	size_t	ret = npos;
	for (size_t i = pos; i <= _cur; i++)
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
