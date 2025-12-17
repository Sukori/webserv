/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:22:45 by pberset           #+#    #+#             */
/*   Updated: 2025/12/17 18:09:49 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>


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

	private:
		const std::vector<s_listen>			_listen;
		const std::string					_root;
		const std::vector<std::string>		_index;
		const std::string					_access_logs;
		const std::string					_error_logs;
		const long							_client_max_body_size;
		const std::map<int, std::string>	_error_pages;
		const std::vector<Location>			_locations;
};

class Configuration {
	public:
		Configuration(const std::vector<Server> servers);
		~Configuration(void);

		//getters

	private:
		const std::vector<Server>	_servers;
};

//std::ostream&	operator<<(std::ostream& os, const Configuration& config);

#endif

