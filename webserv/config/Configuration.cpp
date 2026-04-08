/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:23:28 by pberset           #+#    #+#             */
/*   Updated: 2026/04/07 18:51:15 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <Configuration.hpp>

Location::Location(void): _valid(false), _autoindex(true) {}

Location::Location(const struct s_location location): _valid(location.valid), 
_route(location.route), 
_root_path(location.root_path), 
_alias(location.alias), 
_return(location.locReturn), 
_limit_except(location.limit_except), 
_autoindex(location.autoindex), 
_upload_path(location.upload_path) {}

Location::Location(const Location& rhs): _valid(rhs._valid), 
_route(rhs._route), 
_root_path(rhs._root_path), 
_alias(rhs._alias), 
_return(rhs._return), 
_limit_except(rhs._limit_except), 
_autoindex(rhs._autoindex), 
_upload_path(rhs._upload_path) {}

Location::~Location(void) {}

const std::string&	Location::getRoute(void) const {
	return(_route);
}

const std::string&	Location::getRoot(void) const {
	return (_root_path);
}

const std::string&	Location::getAlias(void) const {
	return (_alias);
}


const std::map<int, std::string>&	Location::getReturn(void) const {
	return (_return);
}

const std::set<std::string>&	Location::getLimExcept(void) const {
	return (_limit_except);
}

const bool&	Location::getAutoIndex(void) const {
	return (_autoindex);
}

const std::string&	Location::getUploadPath(void) const {
	return (_upload_path);
}

const bool&	Location::isValid(void) const {
	return (_valid);
}

Server::Server(const struct s_server server, const std::vector<Location> locations): _valid(server.valid), 
_listen(server.listen), 
_serverName(server.serverName), 
_root(server.root), 
_index(server.index),
_cgi_bins(server.cgi_bins), 
_access_logs(server.access_logs), 
_error_logs(server.error_logs), 
_client_max_body_size(server.client_max_body_size), 
_error_pages(server.error_pages), 
_locations(locations) {
}

Server::Server(const Server& rhs): _valid(rhs._valid), 
_listen(rhs._listen), 
_serverName(rhs._serverName), 
_root(rhs._root), 
_index(rhs._index),
_cgi_bins(rhs._cgi_bins),
_access_logs(rhs._access_logs), 
_error_logs(rhs._error_logs), 
_client_max_body_size(rhs._client_max_body_size), 
_error_pages(rhs._error_pages), 
_locations(rhs._locations) {}

Server::~Server(void) {}

const s_listen&	Server::getListen(void) const{
	return (_listen);
}

const std::string&	Server::getName(void) const {
	return (_serverName);
}

const std::string&	Server::getRoot(void) const {
	return (_root);
}

const std::vector<std::string>&	Server::getIndex(void) const{
	return (_index);
}

const std::map<std::string, std::string>&	Server::getCgiBins(void) const {
	return (_cgi_bins);
}

const std::string&	Server::getAccLogs(void) const {
	return (_access_logs);
}

const std::string&	Server::getErrLogs(void) const {
	return (_error_logs);
}

const unsigned int&	Server::getMaxBodySize(void) const {
	return (_client_max_body_size);
}

const std::map<int, std::string>&	Server::getErrPages(void) const {
	return (_error_pages);
}

const std::vector<Location>&	Server::getLocations(void) const {
	return (_locations);
}

const Location&	Server::getLocation(const std::string& route) const {

	std::vector<Location>::const_iterator	it(_locations.begin());
	const Location* ret = NULL;
	size_t length = 0;
	while (it != _locations.end()) {
		/*if ((*it).getRoute().compare(route) == 0) {
			return (*it);
		}*/
		/* TODO implement a proper route check ABSOLUTELY */
		if ((route + '/').rfind(it->getRoute() + '/', 0) == 0 && it->getRoute().length() >= length) // check if route starts with the location's route
		{
			ret = &*it;
			length = ret->getRoute().length();
		}
		++it;
	}
	if (ret == NULL)
	{
		std::cerr << "getLocation: no location matches route " << route << '\n';
		throw 404;
	}
	std::cout << "getLocation: route match found: " << ret->getRoute() << '\n';
	return *ret;
}		

const bool&	Server::isValid(void) const {
	return (_valid);
}

void	Server::setNotValid(void) {
	_valid = false;
}

Configuration::Configuration(const std::vector<Server> servers): _servers(servers) {
}

Configuration::~Configuration(void) {}

const std::vector<Server>&	Configuration::getServers(void) const {
	return (_servers);
}
