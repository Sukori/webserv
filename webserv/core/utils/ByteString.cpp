#include "ByteString.hpp"

ByteString::ByteString(void): _len(0), _cur(0), _data(0) {}
ByteString::ByteString(size_t l): _len(l), _cur(0), _data(new byte[l]) {}
ByteString::ByteString(const ByteString& o): _len(o._len), _cur(o._cur), _data(new byte[_len]) {std::memmove(_data, o._data, _cur);}
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
}

size_t	ByteString::append(byte* s, size_t len) {
	if (_cur + len > _len)
	{
		byte* tmp = new byte[_cur + len];
		std::memmove(tmp, _data, _cur);
		delete[] _data;
		_data = tmp;
		_len = _cur + len;
	}
	std::memmove(_data + _cur, s, len);
	return _len;
}

const byte*	ByteString::data(void) const {
	return _data;
}
