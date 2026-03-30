#include "ByteString.hpp"

ByteString::ByteString(void): _len(0), _cur(0), _data(0) {}
ByteString::ByteString(size_t l): _len(l), _cur(0), _data(new byte[l]) {}
ByteString::ByteString(const ByteString& o): _len(o._len), _cur(o._cur), _data(new byte[o._len]) {std::memmove(_data, o._data, _cur);}
ByteString::~ByteString(void) {delete[] _data;}

ByteString&		ByteString::operator=(const ByteString& o) {
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

size_t	ByteString::append(const byte* s, size_t len) {
	if (_cur + len > _len)
		reserve(_cur + len);
	std::memmove(_data + _cur, s, len);
	_cur += len;
	return _len;
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

std::ostream& operator<<(std::ostream& os, const ByteString& s) {
	for (size_t i = 0; i < s.length(); i++)
	{
		if (std::isprint(s[i]))
			os << (char) s[i];
	}
	return os;
}
