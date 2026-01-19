/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:23:28 by pberset           #+#    #+#             */
/*   Updated: 2025/12/17 18:07:49 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <Configuration.hpp>

Location::Location(const struct s_location location): _route(), _root_path(), _alias(), _limit_except(), _autoindex(), _upload_path(), _cgi_param(), _cgi_pass() {
}

Location::~Location(void) {}

Server::Server(const struct s_server server, const std::vector<Location> locations): _listen(), _root(), _index(), _access_logs(), _error_logs(), _client_max_body_size(), _error_pages(), _locations() {
}

Server::~Server(void) {}

Configuration::Configuration(const std::vector<Server> servers): _servers() {
}

Configuration::~Configuration(void) {}

