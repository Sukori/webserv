/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:22:45 by pberset           #+#    #+#             */
/*   Updated: 2025/12/17 14:32:15 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include <iostream>
# include <string>
# include <fstream>
# include <vector>

struct s_location {
	std::string	route;
	std::string	root_path;
	std::string	limit_except;
	bool		autoindex;
	std::string	upload_path;
	std::string	cgi_pass;
};

struct s_listen {
	std::string	ip; // for this project, always localhost 127.0.0.1
	int			port; // for this project 8080++
}

struct s_server {
	s_listen	listen;
	std::string	root;
	std::string	index;
	std::string	access_logs;
	long		client_max_body_size; // convert "10m" or "10M" in bits (10000000)
	int			*error_pages;
};

class Location {
	public:
		Location(const struct s_location location);
		~Location(void);

	private:
	const std::string	route;
	const std::string	root_path;
	const std::string	limit_except;
	const bool			autoindex;
	const std::string	upload_path;
	const std::string	cgi_pass;
};

class Server {
	public:
		Server(const struct s_location server, const std::vector<Location> locations);
		~Server(void);

	private:
		const s_listen				listen;
		const std::string			root;
		const std::string			index;
		const std::string			access_logs;
		const long					client_max_body_size;
		const int					*error_pages;
		const std::vector<Location>	locations;
};

class Configuration {
	public:
		Configuration(const std::vector<Server> servers);
		~Configuration(void);
		//Configuration	&operator<<(const Configuration &rhs);

		//getters

	private:
		const std::vector<ServerConfig>	servers;
};

#endif

