//test main only to separate test CGI
#include "Cgi.hpp"


static std::string get_next_line(int fd)
{
	char c;
	std::string ret;
	while (read(fd, &c, 1) > 0)
	{
		if (c == '\n')
			break;
		ret += c;
	}
	return ret;
}

static int to_upper_snake_case(int c)
{
	if (c == '-')
		return '_';
	else
		return toupper(c);
}

static void	add_env_field(const std::string& line, std::map<std::string, std::string>& env) {
	std::string::const_iterator	it = line.begin();
	std::string::const_iterator	end = line.end();
	std::string::const_iterator	sep = std::find(it, end, ':');
	std::string					key (it, sep);
	if (sep == end)
		throw (std::invalid_argument("headers and body MUST be separated by an empty line"));
	/* skip whitespaces per RFC standard definition */
	std::transform(key.begin(), key.end(), key.begin(), to_upper_snake_case);
	sep++;
	while (isspace(*sep))
		sep++;
	env.insert(std::make_pair(key, std::string(sep, end)));
}

int	main() {

	std::map<std::string, std::string> env;
	env.insert(std::make_pair("TEST", "haiii \\^w^/"));

	int sockets[2] = {0,1};
	sockets[0] = open("req", O_RDONLY);
	get_next_line(sockets[0]);
	std::string line;
	while ((line = get_next_line(sockets[0])).length() > 0)
	{
		//std::cout << line;
		add_env_field(line, env);
	}
	
	

	exec_cgi("/opt/pyenv/shims/python", "../../www/python/index.py", env, sockets);

	return (0);
}

