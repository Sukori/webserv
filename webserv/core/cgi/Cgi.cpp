/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:17:33 by pberset           #+#    #+#             */
/*   Updated: 2026/03/26 17:05:12 by ylabussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"
//#include "../../config/Configuration.hpp"

#define SERVER_SOFTWARE "weebserv"
#define GATEWAY_INTERFACE "CGI/1.0"
#define SERVER_PROTOCOL "HTTP/1.0"

static std::string ft_uint_to_string(unsigned int n) {
	std::string ret;
	if (n == 0)
		return "0";
	while (n > 0)
	{
		ret.insert(ret.begin(), n%10 + '0');
		n = n/10;
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
	//env["PATH_TRANSLATED"] = "";										/* PATH_TRANSLATED   */
	env["SCRIPT_FILENAME"] = path;										/* SCRIPT_NAME       */
	if (startLine.query.length() > 0)
		env["QUERY_STRING"] = startLine.query.substr(1);				/* QUERY_STRING      */
	env["REMOTE_HOST"] = "localhost";									/* REMOTE_HOST       */
	env["REMOTE_ADDR"] = "127.0.0.1";									/* REMOTE_ADDR       */
	//env["AUTH_TYPE"] = "";											/* AUTH_TYPE         */
	//env["REMOTE_USER"] = "";											/* REMOTE_USER       */
	//env["REMOTE_IDENT"] = "";											/* REMOTE_IDENT      */
	//env["CONTENT_TYPE"] = "";											/* CONTENT_TYPE      */
	//env["CONTENT_LENGTH"] = "";										/* CONTENT_LENGTH    */
	env["REDIRECT_STATUS"] = "true";
}

/*
make sure first field of all env is full UPPER_SNAKE_CASE instead of lower-kebab-case
*/
int exec_cgi(const std::string& exe, const std::string& path, std::map<std::string, std::string> env, int socketIn) {
	typedef std::map<std::string, std::string> env_map;
	int		pfds[2];
	if (pipe(pfds))
		return -1;
	pid_t	cpid = fork();
	if (cpid < 0)
		return close(pfds[0]), close(pfds[1]), -1;
	else if (cpid > 0) {
		/* parent */
		close(pfds[1]);
		return pfds[0];
	} else {
		/* child */
		char *argv[] = {(char*)exe.c_str(), (char*)path.c_str(), NULL};
		char **envp = new char*[env.size() + 1];
		envp[env.size()] = NULL;
		for (env_map::iterator it = env.begin(); it != env.end(); it++)
			envp[std::distance(env.begin(), it)] = strdup((it->first + '=' + it->second).c_str());
		std::cout << argv[0] << ' ' << argv[1] << '\n';
		dup2(socketIn, 0);
		dup2(pfds[1], 1);
		close(pfds[0]);
		execve(argv[0], argv, envp);
		exit(2);
	}
}
