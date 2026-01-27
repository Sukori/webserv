/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:17:49 by pberset           #+#    #+#             */
/*   Updated: 2026/01/27 16:42:23 by ylabussi         ###   ########.fr       */
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

class Cgi {
	public:
		Cgi(void);
		Cgi(const Cgi &other);
		~Cgi(void);
		Cgi    &operator=(const Cgi &other);

	private:
    // TODO: members
};

/* forks, executes the specified binary with the specified env and returns the message-body of the reponse */
std::string exec_cgi(const std::string& exe, const std::string& path, const std::map<std::string, std::string>& env, int socket);

#endif

