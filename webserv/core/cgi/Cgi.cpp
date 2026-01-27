/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:17:33 by pberset           #+#    #+#             */
/*   Updated: 2026/01/27 16:22:02 by ylabussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

/*
cgi env variables

SERVER_SOFTWARE     - const
SERVER_NAME         - from server
GATEWAY_INTERFACE   - const
SERVER_PROTOCOL     - const
SERVER_PORT         - from server
REQUEST_METHOD      - from request
PATH_INFO           - from request
PATH_TRANSLATED     - no idea need TODO research on this
SCRIPT_NAME         - from server
QUERY_STRING        - from request
REMOTE_HOST         - not sure where from, server?
REMOTE_ADDR         - same, prob from server
AUTH_TYPE           - ????
REMOTE_USER         - ????
REMOTE_IDENT        - ????
CONTENT_TYPE        - from request?
CONTENT_LENGTH      - from request

+ headers
*/

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
std::string exec_cgi(const std::string& exe, const std::string& path, const std::map<std::string, std::string>& env, int socket) {
    typedef std::map<std::string, std::string> env_map;
    int     pfds[2];
    if (pipe(pfds))
        return "";
    pid_t   cpid = fork();
    if (cpid < 0)
        return close(pfds[0]), close(pfds[1]), "";
    else if (cpid > 0)
    {
        /* parent */
        close(pfds[1]);
        std::string response = read_all(pfds[0]);
        close(pfds[0]);
        return response;
    }
    else
    {
        /* child */
        char *argv[] = {(char*)exe.c_str(), (char*)path.c_str(), NULL};
        char **envp = new char*[env.size() + 1];
        envp[env.size()] = NULL;
        for (env_map::const_iterator it = env.begin(); it != env.end(); it++)
            envp[std::distance(env.begin(), it)] = strdup((it->first + '=' + it->second).c_str());
        dup2(socket, 0);
        dup2(pfds[1], 1);
        close(pfds[0]);
        execve(argv[0], argv, envp);
        exit(2);
    }
}
