/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:23:28 by pberset           #+#    #+#             */
/*   Updated: 2026/01/24 16:27:12 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "Configuration.hpp"

Location::Location(const struct s_location location): _route(location.route), _root_path(location.root_path), _alias(location.alias), _limit_except(location.limit_except), _autoindex(location.autoindex), _upload_path(location.upload_path), _cgi_param(location.cgi_param), _cgi_pass(location.cgi_pass) {
	(void)_autoindex;
}

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

const std::vector<std::string>&	Location::getLimExcept(void) const {
	return (_limit_except);
}

const bool&	Location::getAutoIndex(void) const {
	return (_autoindex);
}

const std::string&	Location::getUploadPath(void) const {
	return (_upload_path);
}

const std::map<std::string, std::string>&	Location::getCgiParams(void) const {
	return (_cgi_param);
}

const std::string&	Location::getCgiPass(void) const {
	return (_cgi_pass);
}

Server::Server(const struct s_server server, const std::vector<Location> locations): _listen(server.listen), _serverName(server.serverName), _root(server.root), _index(server.index), _access_logs(server.access_logs), _error_logs(server.error_logs), _client_max_body_size(server.client_max_body_size), _error_pages(server.error_pages), _locations(locations) {
	(void)_client_max_body_size;
}

Server::~Server(void) {}

const s_listen&	Server::getListen(void) const{
	return (_listen);
}

const std::string&					Server::getName(void) const {
	return (_serverName);
}

const std::string&					Server::getRoot(void) const {
	return (_root);
}

const std::vector<std::string>&		Server::getIndex(void) const{
	return (_index);
}

const std::string&					Server::getAccLogs(void) const {
	return (_access_logs);
}

const std::string&					Server::getErrLogs(void) const {
	return (_error_logs);
}

const unsigned int&					Server::getMaxBodySize(void) const {
	return (_client_max_body_size);
}

const std::map<int, std::string>&	Server::getErrPages(void) const {
	return (_error_pages);
}

const std::vector<Location>&		Server::getLocations(void) const {
	return (_locations);
}

Configuration::Configuration(const std::vector<Server> servers): _servers(servers) {
}

Configuration::~Configuration(void) {}

const std::vector<Server>&	Configuration::getServers(void) const {
	return (_servers);
}

std::ostream&	operator<<(std::ostream& os, const std::vector<Location>& locations) {

	for (std::vector<Location>::const_iterator lit = locations.begin(); lit != locations.end(); ++lit) {
		const Location&	loc = *lit;

		os << "	* route: " << loc.getRoute() << std::endl
		   << "		- root: " << loc.getRoot() << std::endl
		   << "		- alias: " << loc.getAlias() << std::endl
		   << "		- autoindex: " << loc.getAutoIndex() << std::endl;

		if (!loc.getLimExcept().empty()) {
			os << "		- limite except: " << *loc.getLimExcept().begin() << std::endl;
		}

		os << "		- upload_path: " << loc.getUploadPath() << std::endl;

		if (!loc.getCgiParams().empty()) {
			const std::pair<std::string, std::string>& params = *loc.getCgiParams().begin();

			os << "		- CGI params: " << params.first << ", " << params.second << std::endl;
		}

		os << "		- CGI pass: " << loc.getCgiPass() << std::endl;
	}

    return os;
}

std::ostream&	operator<<(std::ostream& os, const Configuration& config) {
	const std::vector<Server>& servers = config.getServers();

    for (std::vector<Server>::const_iterator it = servers.begin(); it != servers.end(); ++it) {
        const Server& srv = *it;

        os << "server: " << srv.getName() << std::endl
           << "port: "   << srv.getListen().port << std::endl
           << "root: "   << srv.getRoot() << std::endl;

        if (!srv.getIndex().empty()) {
            os << "index: " << *srv.getIndex().begin() << std::endl;
        }

        os << "access_logs: " << srv.getAccLogs() << std::endl
           << "error_logs: "  << srv.getErrLogs() << std::endl
           << "client max body size: " << srv.getMaxBodySize() << std::endl;

        if (!srv.getErrPages().empty()) {
            const std::pair<int, std::string>& err = *srv.getErrPages().begin();
            os << "error pages: " << err.first << ", " << err.second << std::endl;
        }

        os << "Locations:\n";
        const std::vector<Location>& locs = srv.getLocations();
        os << locs << std::endl;
    }

    return os;
}
