/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:17:33 by pberset           #+#    #+#             */
/*   Updated: 2026/01/20 17:16:52 by ylabussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"
/*
Cgi::Cgi(void) {
    std::cout << "Default Cgi constructor" << std::endl;
}
Cgi::Cgi(const Cgi &other) {
    std::cout << "Copy Cgi constructor" << std::endl;
    this = &other;
}

Cgi::~Cgi(void) {
    std::cout << "Cgi destructor" << std::endl;
}
Cgi    &Cgi::operator=(const Cgi &other) {
	std::cout << "Cgi assignation operator" << std::endl;
	if (this != &other) {
    // TODO: members
   }
   return (*this);
};*/
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

std::string getQueryString(const std::string& path);
std::string getPathInfo(const std::string& path);

/*
returns -1 if failed fork, child status otherwise
make sure first field of all env is full UPPER_SNAKE_CASE instead of lower-kebab-case
*/
int exec_cgi(const std::string& exe, const std::string& path, const std::map<std::string, std::string>& env, int sockets[2]) {
    typedef std::map<std::string, std::string> env_map;
    int     status;
    pid_t   cpid = fork();
    if (cpid < 0)
        return -1;
    else if (cpid > 0)
    {
        /* parent */
        waitpid(cpid, &status, 0);
        return status;
    }
    else
    {
        /* child */
        char *argv[] = {(char*)exe.c_str(), (char*)path.c_str(), NULL};
        char **envp = new char*[env.size() + 1];
        envp[env.size()] = NULL;
        for (env_map::const_iterator it = env.begin(); it != env.end(); it++)
            envp[std::distance(env.begin(), it)] = strdup((it->first + '=' + it->second).c_str());
        dup2(sockets[0], 0);
        dup2(sockets[1], 1);
        execve(argv[0], argv, envp);
        exit(1);
        (void)sockets;
    }
}



