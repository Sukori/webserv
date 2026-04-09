/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:17:33 by pberset           #+#    #+#             */
/*   Updated: 2026/04/09 16:46:08 by ylabussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Cgi.hpp>
//#include <Configuration.hpp>

#define SERVER_SOFTWARE "weebserv"
#define GATEWAY_INTERFACE "CGI/1"
#define SERVER_PROTOCOL "HTTP/1.1"

static std::string ft_uint_to_string(unsigned int i) {
	if (i == 0)
		return "0";
	std::string ret;
	char s[] = "0";
	while (i > 0)
	{
		*s = (char) i%10 + '0';
		ret.insert(0, s);
		i /=10;
	}
	return ret;
}

void add_cgi_env(std::map<std::string, std::string>& env, const Server& server, const Http::StartLine& startLine, const std::string& path) {
	env["SERVER_SOFTWARE"] = SERVER_SOFTWARE "/1.0";					/* SERVER_SOFTWARE   */
	env["SERVER_NAME"] = server.getName();								/* SERVER_NAME       */
	env["GATEWAY_INTERFACE"] = GATEWAY_INTERFACE;						/* GATEWAY_INTERFACE */
	env["SERVER_PROTOCOL"] = SERVER_PROTOCOL;							/* SERVER_PROTOCOL   */
	env["SERVER_PORT"] = ft_uint_to_string(server.getListen().port);	/* SERVER_PORT       */
	env["REQUEST_METHOD"] = startLine.method;							/* REQUEST_METHOD    */
	env["PATH_INFO"] = startLine.path;									/* PATH_INFO         */
	env["SCRIPT_NAME"] = path;											/* SCRIPT_NAME       */
	if (startLine.query.length() > 0)
		env["QUERY_STRING"] = startLine.query.substr(1);				/* QUERY_STRING      */
	env["REMOTE_HOST"] = "localhost";									/* REMOTE_HOST       */
	env["REMOTE_ADDR"] = "127.0.0.1";									/* REMOTE_ADDR       */
	env["SCRIPT_FILENAME"] = path;										/* for php-cgi       */
	env["REDIRECT_STATUS"] = "true";									/* for php-cgi       */
	env["PHP_INI_SCAN_DIR"] = server.getRoot();							/* for php-cgi       */
	
	// TODO; add upload path
}

/*
returns pipe fd out
make sure first field of all env is full UPPER_SNAKE_CASE instead of lower-kebab-case
*/
int exec_cgi(const std::string& exe, const std::string& path, std::map<std::string, std::string> env, const ByteString& dataIn) {
	typedef std::map<std::string, std::string> env_map;
	int		inpfds[2];
	int		outpfds[2];

	if (pipe(outpfds))
		return -1;
	if (pipe(inpfds))
	{
		close(outpfds[0]);
		close(outpfds[1]);
		return -1;
	}
	
	pid_t	cpid = fork();
	if (cpid < 0)
		return close(outpfds[0]), close(outpfds[1]), -1;
	else if (cpid > 0) {
		/* parent */
		write(inpfds[1], dataIn.data(), dataIn.length());
		close(inpfds[1]);
		close(outpfds[1]);
		return outpfds[0];
	} else {
		/* child */
		char *argv[] = {(char*)exe.c_str(), (char*)path.c_str(), NULL};
		char **envp = new char*[env.size() + 1];
		envp[env.size()] = NULL;
		for (env_map::iterator it = env.begin(); it != env.end(); it++)
			envp[std::distance(env.begin(), it)] = strdup((it->first + '=' + it->second).c_str());
		std::cout << argv[0] << ' ' << argv[1] << '\n';
		dup2(inpfds[0], 0);
		dup2(outpfds[1], 1);
		close(outpfds[0]);
		execve(argv[0], argv, envp);
		exit(2);
	}
}
