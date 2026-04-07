/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:17:49 by pberset           #+#    #+#             */
/*   Updated: 2026/03/26 16:45:10 by ylabussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <fcntl.h>
# include <wait.h>
# include <unistd.h>
# include <string.h>
# include <iostream>
# include <string>
# include <map>
# include <algorithm>

#include <Http.hpp>

class Cgi {
	public:
		Cgi(void);
		Cgi(const Cgi &other);
		~Cgi(void);
		Cgi	&operator=(const Cgi &other);

	private:
	// TODO: members
};

void add_cgi_env(std::map<std::string, std::string>& env, const Server& server, const Http::StartLine& startLine, const std::string& path);
/* forks, executes the specified binary with the specified env and returns the message-body of the reponse */
int exec_cgi(const std::string& exe, const std::string& path, std::map<std::string, std::string> env, int socket);

#endif
