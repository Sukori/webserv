#include "ByteString.hpp"
#include <string>

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
	ByteString s3 ("haiii123f321");
	std::cout << s3 << '\n';
	std::cout << s3.substr(s3.find("23")) << '\n';
	std::cout << s3.substr(s3.find_first_of("f3")) << '\n';
	std::cout << s3.substr(s3.find_last_not_of("123")) << '\n';
	return 0;
}

