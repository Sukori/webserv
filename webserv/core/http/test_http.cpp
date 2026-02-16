#include "Http.hpp"
#include "../cgi/Cgi.hpp"

std::string get_ext(const std::string& path) {
	size_t sep = path.find('.');
	if (sep == -1ul)
		return "";
	else
		return path.substr(sep + 1);
}

std::string getResponseBody(const Http& req, const std::string& root, const std::map<std::string, std::string>& binaries) {
	const Http::StartLine& sl (req.getStartLine());
	std::string ext (get_ext(sl.path));
	std::string bin_f;

	try {
		bin_f = binaries.at(ext);
	}
	catch (const std::out_of_range& e) {
		bin_f = "";
	}

	if (bin_f == "") {
		/* hmtl */
		std::string lines, line;
		std::ifstream file ((root + sl.path + (ext == "" ? "index.html" : "")).c_str());
		if (file.fail())
			std::cout << "cannot open file\n"; /* TODO: check for 403 forbidden, 404 not found,  */
		while (std::getline(file, line))
			lines += line+'\n';
		return lines;
	}
	else {
		/* cgi */
		Http::Header h (req.getHeader());
		add_cgi_env(h, req.getStartLine());
		return exec_cgi(bin_f, root + sl.path, h, req.getSocket());
	}
}

// test main for http requests
int	main(int argc, char *argv[]) {
	if (argc < 2)
		return std::cout << "please enter a file to be read" << std::endl, 1;
	int fds[] = {0, 1};
	fds[0] = open(argv[1], O_RDONLY);

	std::set<std::string> a;
	a.insert("GET");
	a.insert("POST");
	a.insert("DELETE");

	Http req (fds[0], a);
	const Http::StartLine& sl (req.getStartLine());
	Http::Header h (req.getHeader());

	std::cout << "path:\t" + sl.path + '\n';
	std::cout << "extra:\t" + sl.extra + '\n';
	std::cout << "query:\t" + sl.query + '\n';
	std::cout << "method:\t" + sl.method + '\n';

	std::cout << "headers:\n";
	for (Http::Header::const_iterator it = h.begin(); it != h.end();it++)
	std::cout << '\t' + it->first + '=' + it->second + '\n';
	std::cout << '\n';

	std::string root ("/home/ylabussi/Documents/webserv/webserv/www");

	std::map<std::string, std::string> bin;
	bin.insert(std::make_pair("py", "/opt/pyenv/shims/python"));
	bin.insert(std::make_pair("php", "/usr/bin/php"));

	std::cout << getResponseBody(req, root, bin);

	return 0;
}
