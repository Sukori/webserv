#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

#include <dirent.h>
#include <iostream>
#include <string>
#include <ByteString.hpp>
#include <Configuration.hpp>

ByteString autoindex(const std::string& requestPath, const std::string& filePath, const Server& serv);

#endif
