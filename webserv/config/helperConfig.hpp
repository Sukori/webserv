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
# include <cctype>

struct s_location {
	std::string							route; // location "/dir/" {     for instance
	std::string							root_path; // root "/var/www"; } for instance
	std::string							alias;
	std::vector<std::string>			limit_except;
	bool								autoindex;
	std::string							upload_path;
	std::map<std::string, std::string>	cgi_param;
	std::string							cgi_pass;
};


struct s_listen {
	/* IPV4 are stored in IPV4 mapped IPV6
	::ffff:xxxx:xxxx */
	std::string	ip; 
	int			port; // 8080++ for this project
};

struct s_server {
	std::vector<s_listen>		listen;
	std::string					root;
	std::vector<std::string>	index;
	std::string					access_logs;
	std::string					error_logs;
	long						client_max_body_size; // convert "10m" or "10M" in bits (10000000)
	std::map<int, std::string>	error_pages; // {error_code<int>, path_to_page<string>}, {404, "/www/error_pages/404.html"}
};

std::string	readFile(const char* file);
std::string	filterComments(std::string& str);
std::string	insertSpaces(std::string& str);

class HelperConfig {
public:
	HelperConfig(void);
	~HelperConfig(void);

	void	initParser(void);
	void	parseServer(void);
	void	parseLocation(void);

private:
	std::stringstream	_ss;
};

#endif

