/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validatorConfig.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:05:52 by pberset           #+#    #+#             */
/*   Updated: 2026/03/13 14:05:59 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "validatorConfig.hpp"

//validate location

bool	validLocRoute(std::string& route) {

}

bool	validLocRoot(std::string& root) {
	
}

void	validLimitExcept(std::vector<std::string>& limitExcept) {

} //default GET is not specified

void	validUploadPath(std::string& uploadPath) {

} //default /www/upload if not specified. Create if not exists
//cgi_params ? subject to deletion
//cgi_pass ? subject to deletion

void	validateLocation(s_location& locStruct) {
	if (!validLocRoute(locStruct.route)) {
		std::cerr << "Route: not a valid route for location " << locStruct.route << std::endl;
		locStruct.route = "ERROR";
	}
	if (!validLocRoot(locStruct.root_path)) {
		std::cerr << "Root: not a valid root for location " <<  locStruct.root_path << std::endl;
		locStruct.route = "ERROR";
	}
	validLimitExcept(locStruct.limit_except);
	validUploadPath(locStruct.upload_path);
}


//validate server

void	validServerName(std::string& serverName) {

} //if not set, use "server<port>"

bool	validServerRoot(std::string& root) {

}

bool	validPort(int port) {

}

void	validIndex(std::vector<std::string>& index) {

} //if not set, use "index.html"

void	validAccessLogs(std::string& accessLogs) {

} //create if not exists, can be null

void	validErrorLogs(std::string& errorLogs) {

} //create if not exists, can be null

bool	validClientMaxBodySize(unsigned int maxBodySize) {

}

void	validErrorPages(std::map<int, std::string>& errorPages) { //if not set use defaults

}

void	validateServer(s_server& servStruct) {
	validServerName(servStruct.serverName);
	if (!validServerRoot(servStruct.root)) {
		std::cerr << "Root: not a valid root for server " << servStruct.serverName << std::endl;
		servStruct.serverName = "ERROR";
	}
	if (!validPort(servStruct.listen.port)) {
		std::cerr << "listen: not a valid port for server " << servStruct.listen.port << std::endl;
		servStruct.serverName = "ERROR";
	}
	validIndex(servStruct.index);
	validAccessLogs(servStruct.access_logs);
	validErrorLogs(servStruct.error_logs);
	if (!validClientMaxBodySize(servStruct.client_max_body_size)) {
		std::cerr << "client_max_body_size: not a valid max client body size for server " << servStruct.client_max_body_size << std::endl;
		servStruct.serverName = "ERROR";
	}
	validErrorPages(servStruct.error_pages);
}
