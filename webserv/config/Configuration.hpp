/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:22:45 by pberset           #+#    #+#             */
/*   Updated: 2025/12/17 16:13:58 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include <iostream>
# include <string>
# include <fstream>
# include <vector>
# include <map>

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
	std::string	ip; // "127.0.0.1" for this project
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

class Location {
	public:
		Location(const struct s_location location);
		~Location(void);

	private:
	const std::string							route;
	const std::string							root_path;
	const std::string							alias;
	const std::vector<std::string>				limit_except;
	const bool									autoindex;
	const std::string							upload_path;
	const std::map<std::string, std::string>	cgi_param;
	const std::string							cgi_pass;
};

class Server {
	public:
		Server(const struct s_server server, const std::vector<Location> locations);
		~Server(void);

	private:
		const std::vector<s_listen>			listen;
		const std::string					root;
		const std::vector<std::string>		index;
		const std::string					access_logs;
		const std::string					error_logs;
		const long							client_max_body_size;
		const std::map<int, std::string>	error_pages;
		const std::vector<Location>			locations;
};

class Configuration {
	public:
		Configuration(const std::vector<Server> servers);
		~Configuration(void);

		//getters

	private:
		const std::vector<Server>	servers;
};

//std::ostream&	operator<<(std::ostream& os, const Configuration& config);

#endif

