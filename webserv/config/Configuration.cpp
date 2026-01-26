/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:23:28 by pberset           #+#    #+#             */
/*   Updated: 2026/01/25 15:10:28 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <Configuration.hpp>

Location::Location(const struct s_location location): _route(location.route), _root_path(location.root_path), _alias(location.alias), _limit_except(location.limit_except), _autoindex(location.autoindex), _upload_path(location.upload_path), _cgi_param(location.cgi_param), _cgi_pass(location.cgi_pass) {
}

Location::~Location(void) {}

Server::Server(const struct s_server server, const std::vector<Location> locations): _listen(server.listen), _root(server.root), _index(server.index), _access_logs(server.access_logs), _error_logs(server.error_logs), _client_max_body_size(server.client_max_body_size), _error_pages(server.error_pages), _locations(locations) {
}

Server::~Server(void) {}

std::vector<s_listen>&	Server::getListens(void) {
	return (_listen);
}

Configuration::Configuration(const std::vector<Server> servers): _servers(servers) {
}

Configuration::~Configuration(void) {}

std::vector<Server>&	Configuration::getServers(void) {
	return (_servers);
}

