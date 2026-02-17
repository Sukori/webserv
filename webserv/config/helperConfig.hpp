/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helperConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:55:02 by pberset           #+#    #+#             */
/*   Updated: 2026/01/25 14:58:56 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPERCONFIG_HPP
# define HELPERCONFIG_HPP

# include <iostream>
# include <fstream>
# include <string>
# include <sstream>
# include <vector>
# include <map>
# include <utility>
# include <cctype>

struct s_location {
	std::string							route = "/"; // location "/dir/"     for instance
	std::string							root_path = "/"; // root "/var/www"; for instance
	std::string							alias = "/";
	std::vector<std::string>			limit_except;
	bool								autoindex = true;
	std::string							upload_path = "/uploads/";
	std::map<std::string, std::string>	cgi_param = {std::string(""), std::string("")};
	std::string							cgi_pass = "";
};


struct s_listen {
	std::string	ip; //allways broadcast for this project
	int			port; // 8080++ for this project
	std::string	protocol; //HTTP, TCP, SSL, SSH ...
};

struct s_server {
	std::string					serverName = "webserv";
	s_listen					listen;
	std::string					root = "/";
	std::vector<std::string>	index;
	std::string					access_logs = "/logs/logfiles/access.log";
	std::string					error_logs = "/logs/logfiles/error.log";
	unsigned int				client_max_body_size = 1024; // convert "10m" or "10M" in bits (10000000)
	std::map<int, std::string>	error_pages; // {error_code<int>, path_to_page<string>}, {404, "/www/error_pages/404.html"}
};

std::string	readFile(const char* file);
std::string	filterComments(std::string& str);
std::string	insertSpaces(std::string& str);

#endif

