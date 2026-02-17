/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:22:45 by pberset           #+#    #+#             */
/*   Updated: 2026/01/24 16:39:41 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include "helperConfig.hpp"

class Location {
	public:
		Location(const struct s_location location);
		~Location(void);

	private:
		const std::string							_route;
		const std::string							_root_path;
		const std::string							_alias;
		const std::vector<std::string>				_limit_except;
		const bool									_autoindex;
		const std::string							_upload_path;
		const std::map<std::string, std::string>	_cgi_param;
		const std::string							_cgi_pass;
};

class Server {
	public:
		Server(const struct s_server server, const std::vector<Location> locations);
		~Server(void);

		//getters
		s_listen&	getListens(void);

	private:
		s_listen							_listen;
		const std::string					_serverName;
		const std::string					_root;
		const std::vector<std::string>		_index;
		const std::string					_access_logs;
		const std::string					_error_logs;
		const unsigned int					_client_max_body_size;
		const std::map<int, std::string>	_error_pages;
		const std::vector<Location>			_locations;
};

class Configuration {
	public:
		Configuration(const std::vector<Server> servers);
		~Configuration(void);

		//getters
		std::vector<Server>&	getServers(void);

	private:
		std::vector<Server>	_servers;
};

//std::ostream&	operator<<(std::ostream& os, const Configuration& config);

#endif

