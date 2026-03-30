#include "ByteString.hpp"

int main(void)
{
	ByteString s (20);
	std::cout << s.capacity() << '\n';
	std::cout << s.length() << '\n';
	s.append((const byte*) "hello world", 12);
	std::cout << s.capacity() << '\n';
	std::cout << s.length() << '\n';
	s.shrink_to_fit();
	std::cout << s.capacity() << '\n';
	std::cout << s.length() << '\n';
	std::cout << s << '\n';
	ByteString s2 (s);
	std::cout << s2 << '\n';
	s2.clear();
	std::cout << s2 << '\n';
	return 0;
}

