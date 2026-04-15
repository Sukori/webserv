#include "autoindex.hpp"

static ByteString htmlHeader(const std::string& path) {
	ByteString ret;
	ret.append("<html>");
	ret.append("<head>");
	ret.append("<title>");
	ret.append(path.c_str());
	ret.append("</title>");
	ret.append("</head>");
	ret.append("<body>");
	return ret;
}

static ByteString htmlFooter() {
	ByteString ret;
	ret.append("</body>");
	ret.append("</html>");
	return ret;
}

static ByteString	makeLink(struct dirent *ent, const std::string& path) {
	ByteString ret;
	ret.append("<a href=\"");
	ret.append(path.c_str());
	ret.append(ent->d_name);
	ret.append("\">");
	ret.append(ent->d_name);
	if (ent->d_type == DT_DIR)
		ret.append("/");
	ret.append("</a><br>");
	return ret;
}

ByteString autoindex(const std::string& requestPath, const std::string& filePath)
{
	ByteString ret ("\r\n");
	DIR *dir;
	struct dirent *ent;
	std::cerr << "autoindex: trying to open " << filePath << '\n';
	if ((dir = opendir(filePath.c_str())) != NULL) {
		ret.append(htmlHeader(requestPath));
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			ret.append(makeLink(ent, requestPath + ((requestPath.find('/', requestPath.length() - 1) != requestPath.npos) ? "" : "/")));
		}
		ret.append(htmlFooter());
		closedir(dir);
	} else {
		std::cerr << "could not open file\n";
		return "";
	}
	return ret;
}
