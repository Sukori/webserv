#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

#include <dirent.h>
#include <iostream>
#include <string>
#include <ByteString.hpp>

ByteString autoindex(const std::string& requestPath, const std::string& filePath);

#endif
