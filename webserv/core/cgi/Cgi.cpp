/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:17:33 by pberset           #+#    #+#             */
/*   Updated: 2026/02/05 16:45:14 by ylabussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"
//#include "../../config/Configuration.hpp"

#define SERVER_SOFTWARE "ftws"
#define GATEWAY_INTERFACE "CGI/1"
#define SERVER_PROTOCOL "HTTP/1"

void add_cgi_env(std::map<std::string, std::string>& env, /*const Server& server,*/ const Http::StartLine& startLine) {
    env.insert(std::make_pair("SERVER_SOFTWARE", SERVER_SOFTWARE "/0.1"));  /* SERVER_SOFTWARE   */
    env.insert(std::make_pair("SERVER_NAME", SERVER_SOFTWARE));             /* SERVER_NAME       */
    env.insert(std::make_pair("GATEWAY_INTERFACE", GATEWAY_INTERFACE));     /* GATEWAY_INTERFACE */
    env.insert(std::make_pair("SERVER_PROTOCOL", SERVER_PROTOCOL));         /* SERVER_PROTOCOL   */
    //env.insert(std::make_pair("SERVER_PORT", server.getPort()));            /* SERVER_PORT       */
    env.insert(std::make_pair("REQUEST_METHOD", startLine.method));         /* REQUEST_METHOD    */
    env.insert(std::make_pair("PATH_INFO", startLine.path));                /* PATH_INFO         */
    //env.insert(std::make_pair("PATH_TRANSLATED", ""));                      /* PATH_TRANSLATED   */
    env.insert(std::make_pair("SCRIPT_NAME", startLine.path));              /* SCRIPT_NAME       */
    env.insert(std::make_pair("QUERY_STRING", startLine.path));             /* QUERY_STRING      */
    env.insert(std::make_pair("REMOTE_HOST", ""));                          /* REMOTE_HOST       */
    env.insert(std::make_pair("REMOTE_ADDR", ""));                          /* REMOTE_ADDR       */
    //env.insert(std::make_pair("AUTH_TYPE", "?"));                           /* AUTH_TYPE         */
    //env.insert(std::make_pair("REMOTE_USER", "?"));                         /* REMOTE_USER       */
    //env.insert(std::make_pair("REMOTE_IDENT", "?"));                        /* REMOTE_IDENT      */
    //env.insert(std::make_pair("CONTENT_TYPE", ""));                         /* CONTENT_TYPE      */
    //env.insert(std::make_pair("CONTENT_LENGTH", ""));                       /* CONTENT_LENGTH    */
}

std::string read_all(int fd) {
    char c;
    std::string ret;
    while (read(fd, &c, 1) > 0)
        ret += c;
    return ret;
}

/*
returns -1 if failed fork, child status otherwise
make sure first field of all env is full UPPER_SNAKE_CASE instead of lower-kebab-case
*/
std::string exec_cgi(const std::string& exe, const std::string& path, const std::map<std::string, std::string>& env, int socketIn) {
    typedef std::map<std::string, std::string> env_map;
    int     pfds[2];
    if (pipe(pfds))
        return "";
    pid_t   cpid = fork();
    if (cpid < 0)
        return close(pfds[0]), close(pfds[1]), "";
    else if (cpid > 0) {
        /* parent */
        close(pfds[1]);
        std::string response = read_all(pfds[0]);
        close(pfds[0]);
        return response;
    } else {
        /* child */
        char *argv[] = {(char*)exe.c_str(), (char*)path.c_str(), NULL};
        char **envp = new char*[env.size() + 1];
        envp[env.size()] = NULL;
        for (env_map::const_iterator it = env.begin(); it != env.end(); it++)
            envp[std::distance(env.begin(), it)] = strdup((it->first + '=' + it->second).c_str());
        dup2(socketIn, 0);
        dup2(pfds[1], 1);
        close(pfds[0]);
        execve(argv[0], argv, envp);
        exit(2);
    }
}
