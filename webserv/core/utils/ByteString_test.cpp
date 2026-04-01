#include "ByteString.hpp"

int main(void)
{
	ByteString s ("hello");
	std::cout << s.find('h');
	return 0;
}

