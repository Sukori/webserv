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

		os	<< "\t\t* route: " << loc.getRoute() << std::endl
			<< "\t\t\t- isValid: " << loc.isValid() << std::endl
			<< "\t\t\t- root: " << loc.getRoot() << std::endl
			<< "\t\t\t- alias: " << loc.getAlias() << std::endl
			<< "\t\t\t- autoindex: " << loc.getAutoIndex() << std::endl;

		std::map<int, std::string>	redirections = loc.getReturn();
        if (!redirections.empty()) {
            const std::map<int, std::string>::const_iterator& start = redirections.begin();
			const std::map<int, std::string>::const_iterator& end = redirections.end();
            os << "\t\t\t- return:" << std::endl;
			for (std::map<int, std::string>::const_iterator redir = start; redir != end; ++redir) {
				os << "\t\t\t\t> " << redir->first << " = " << redir->second << std::endl;
			}
        }

		if (!loc.getLimExcept().empty()) {
			std::set<std::string>::const_iterator start = loc.getLimExcept().begin();
			std::set<std::string>::const_iterator end = loc.getLimExcept().end();
			os << "\t\t\t- limit except: " << std::endl;
			for (std::set<std::string>::const_iterator lex = start; lex != end; ++lex) {
				os << "\t\t\t\t> " << *lex << std::endl;
			}
		}

		os << "\t\t\t- upload_path: " << loc.getUploadPath() << std::endl;

		if (!loc.getCgiParams().empty()) {
			const std::map<std::string, std::string>::const_iterator& start = loc.getCgiParams().begin();
			const std::map<std::string, std::string>::const_iterator& end = loc.getCgiParams().end();
			os << "\t\t\t- CGI params: " << std::endl;
			for (std::map<std::string, std::string>::const_iterator cgip = start; cgip != end; ++cgip) {
				os << "\t\t\t\t> " << cgip->first << " = " << cgip->second << std::endl;
			}
		}

		os << "\t\t\t- CGI pass: " << loc.getCgiPass() << std::endl;
	}

    return os;
}

std::ostream&	operator<<(std::ostream& os, const Configuration& config) {
	const std::vector<Server>& servers = config.getServers();

    for (std::vector<Server>::const_iterator it = servers.begin(); it != servers.end(); ++it) {
        const Server& srv = *it;

        os	<< "server: " << srv.getName() << std::endl
			<< "\t- isValid: " << srv.isValid() << std::endl
        	<< "\t- port: "   << srv.getListen().port << std::endl
        	<< "\t- root: "   << srv.getRoot() << std::endl;

        if (!srv.getIndex().empty()) {
            os << "\t- index: " << *srv.getIndex().begin() << std::endl;
        }

        os	<< "\t- access_logs: " << srv.getAccLogs() << std::endl
        	<< "\t- error_logs: "  << srv.getErrLogs() << std::endl
        	<< "\t- client max body size: " << srv.getMaxBodySize() << std::endl;

		std::map<int, std::string>	errPages = srv.getErrPages();
        if (!errPages.empty()) {
            const std::map<int, std::string>::const_iterator& start = errPages.begin();
			const std::map<int, std::string>::const_iterator& end = errPages.end();
            os << "\t- error pages:" << std::endl;
			for (std::map<int, std::string>::const_iterator errp = start; errp != end; ++errp) {
				os << "\t\t* " << errp->first << " = " << errp->second << std::endl;
			}
        }

        os << "\t- Locations:\n";
        const std::vector<Location>& locs = srv.getLocations();
        os << locs << std::endl;
    }

    return os;
}
