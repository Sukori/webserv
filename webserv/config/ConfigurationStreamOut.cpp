/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config/ConfigurationStreamOut.cpp                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 17:05:39 by pberset           #+#    #+#             */
/*   Updated: 2026/03/18 17:05:47 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "Configuration.hpp"

std::ostream&	operator<<(std::ostream& os, const std::vector<Location>& locations) {

	for (std::vector<Location>::const_iterator lit = locations.begin(); lit != locations.end(); ++lit) {
		const Location&	loc = *lit;

		os << "\t* route: " << loc.getRoute() << std::endl
		   << "\t\t- root: " << loc.getRoot() << std::endl
		   << "\t\t- alias: " << loc.getAlias() << std::endl
		   << "\t\t- autoindex: " << loc.getAutoIndex() << std::endl;

		if (!loc.getLimExcept().empty()) {
			std::vector<std::string>::const_iterator start = loc.getLimExcept().begin();
			std::vector<std::string>::const_iterator end = loc.getLimExcept().end();
			os << "\t\t- limit except: " << std::endl;
			for (std::vector<std::string>::const_iterator lex = start; lex != end; ++lex) {
				os << "\t\t\t> " << *lex << std::endl;
			}
		}

		os << "\t\t- upload_path: " << loc.getUploadPath() << std::endl;

		if (!loc.getCgiParams().empty()) {
			const std::map<std::string, std::string>::const_iterator& start = loc.getCgiParams().begin();
			const std::map<std::string, std::string>::const_iterator& end = loc.getCgiParams().end();
			os << "\t\t- CGI params: " << std::endl;
			for (std::map<std::string, std::string>::const_iterator cgip = start; cgip != end; ++cgip) {
				os << "\t\t\t> " << cgip->first << " = " << cgip->second << std::endl;
			}
		}

		os << "\t\t- CGI pass: " << loc.getCgiPass() << std::endl;
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
            const std::map<int, std::string>::const_iterator& start = srv.getErrPages().begin();
			const std::map<int, std::string>::const_iterator& end = srv.getErrPages().end();
            os << "error pages:" << std::endl;
			for (std::map<int, std::string>::const_iterator errp = start; errp != end; ++errp) {
				os << "\t* " << errp->first << " = " << errp->second << std::endl;
			}
        }

        os << "Locations:\n";
        const std::vector<Location>& locs = srv.getLocations();
        os << locs << std::endl;
    }

    return os;
}
