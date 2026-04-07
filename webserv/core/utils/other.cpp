#include "other.hpp"

/* idk where to put this */
std::string&	trimLastSlash(std::string& s) {
	if (!s.empty() && s[s.length() - 1] == '/')
		s.erase(s.length() - 1);
	return s;
}
