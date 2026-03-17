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
#include "errPages.hpp"

//validate location

bool	validLocRoute(std::string& route) {

	if (route.empty()) {
		std::cerr << "validLocRoute: empty route token" << std::endl;
		return (false);
	}

	struct stat	buf;
	int status = stat(route.c_str(), &buf);

	if (status == -1) {

		std::cerr << "stat: " << strerror(errno) << std::endl;
		std::cerr << "from validLocRoute" << std::endl;
		return (false);
	}

	return (S_ISDIR(buf.st_mode) || S_ISREG(buf.st_mode));
}

bool	validLocRoot(std::string& root) {

		if (root.empty()) {
		std::cerr << "validLocroot: empty root token" << std::endl;
		return (false);
	}

	struct stat	buf;
	int status = stat(root.c_str(), &buf);

	if (status == -1) {

		std::cerr << "stat: " << strerror(errno) << std::endl;
		std::cerr << "from validLocroot" << std::endl;
		return (false);
	}

	return (S_ISDIR(buf.st_mode));
}

static bool	isValidMethod(const std::string& method) {
	return (method == "GET" || method == "POST" || method == "DELETE");
}

static void	removeDuplicates(std::vector<std::string>& methods) {

	std::vector<std::string>::iterator	method = methods.begin();
	std::map<std::string, bool>			seen;

	while (method != methods.end()) {
		bool	validMethod = isValidMethod(*method);
		//if the method is not valid, It will be deleted by the validator
		if (validMethod && seen[*method]) {
				std::cerr << "removeDuplicates: duplicate valid method ignored " << *method<< std::endl;
				method = methods.erase(method);
		} else {
			if (validMethod)
				seen[*method] = true;
			method++;
		}
	}
}

void	validLimitExcept(std::vector<std::string>& limitExcept) {

	if (!limitExcept.empty()) {
		removeDuplicates(limitExcept);
	} else {
		std::cerr << "validLimitExcept: empty list of allowed methods, default to " << DEFAULT_METHOD << std::endl;
		limitExcept.push_back(DEFAULT_METHOD);
		return ;
	}
	//loop all methods input
	std::vector<std::string>::iterator method = limitExcept.begin();

	while (method != limitExcept.end()) {
		bool	validMethod = isValidMethod(*method);
		
		if (!validMethod) {
			std::cerr << "validLimitExcept: invalid method ignored " << *method << std::endl;
			method = limitExcept.erase(method);
			continue ;
		}
		method++;
	}

	if (limitExcept.empty()) {
		std::cerr << "validLimitExcept: empty list of allowed methods, default to " << DEFAULT_METHOD << std::endl;
		limitExcept.push_back(DEFAULT_METHOD);
	}
}//default GET if not specified

bool	validUploadPath(std::string& uploadPath) {

	if (uploadPath.empty()) {
		std::cerr << "validUploadPath: empty uploadPath token" << std::endl;
		return (false);
	}

	struct stat	buf;
	int status = stat(uploadPath.c_str(), &buf);

	if (status == -1) {

		std::cerr << "stat: " << strerror(errno) << std::endl;
		std::cerr << "from validLocuploadPath" << std::endl;
		return (false);
	}

	return (S_ISDIR(buf.st_mode));
}

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
	
	if (!validUploadPath(locStruct.upload_path)) {
		std::cerr << "uploadPath: not a valid uploadPath for location " <<  locStruct.upload_path << std::endl;
		locStruct.route = "ERROR";
	}
}


//validate server

void	validServerName(std::string& serverName, int port) {

	if (serverName.empty()) {
		std::cerr << "validServerName: empty name token, defaulting to port number" << std::endl;
		std::stringstream ss;
		ss << "server";
		ss << port;
		ss >> serverName;
	}
}

bool	validServerRoot(std::string& root) {
	if (root.empty()) {
		std::cerr << "validServerRoot: empty root token" << std::endl;
		return (false);
	}

	struct stat	buf;
	int status = stat(root.c_str(), &buf);

	if (status == -1) {

		std::cerr << "stat: " << strerror(errno) << std::endl;
		std::cerr << "from validLocroot" << std::endl;
		return (false);
	}

	return (S_ISDIR(buf.st_mode));
}

void	validIndex(std::vector<std::string>& index) {

	if (index.empty()) {
		std::cerr << "validIndex: empty index token list, defaulting to {index.html, index.php}" << std::endl;
		index.push_back("index.html");
		index.push_back("index.php");
	}
}

void	validAccessLogs(std::string& accessLogs) {

	if (accessLogs.empty())
		return ;
	if (access(accessLogs.c_str(), W_OK) < 0) {
		std::cerr << "validAccessLogs: " << strerror(errno) << std::endl;
		std::cerr << "won't log access!" << std::endl;
		return ;
	}

	struct stat	buf;
	int status = stat(accessLogs.c_str(), &buf);

	if (status == -1) {
		std::cerr << "stat: " << strerror(errno) << std::endl;
		std::cerr << "from validAccessLogs" << std::endl;
		std::cerr << "won't log access!" << std::endl;
	} else if (!S_ISREG(buf.st_mode)) {
		std::cerr << "validAccessLogs: not a text file " << accessLogs << std::endl;
		std::cerr << "won't log access!" << std::endl;
	}
}

void	validErrorLogs(std::string& errorLogs) {

	if (errorLogs.empty())
		return ;
	if (access(errorLogs.c_str(), W_OK) < 0) {
		std::cerr << "validErrorLogs: " << strerror(errno) << std::endl;
		std::cerr << "won't log errors!" << std::endl;
		return ;
	}

	struct stat	buf;
	int status = stat(errorLogs.c_str(), &buf);

	if (status == -1) {
		std::cerr << "stat: " << strerror(errno) << std::endl;
		std::cerr << "from validErrorLogs" << std::endl;
		std::cerr << "won't log errors!" << std::endl;
	} else if (!S_ISREG(buf.st_mode)) {
		std::cerr << "validErrorLogs: not a text file " << errorLogs << std::endl;
		std::cerr << "won't log errors!" << std::endl;
	}

}

bool	validClientMaxBodySize(unsigned int maxBodySize) {
	if (maxBodySize == 0) {
		std::cerr << "Warning! You are running with client_max_body_size = 0 (disabled)" << std::endl;
		std::cerr << "if this is not your intention, modify the config and reboot the server" << std::endl;
	} else if (maxBodySize > CLIENT_MAX_BODY_SIZE) {
		std::cerr << "validClientMaxBodySize: set client_max_body_size is too large" << std::endl;
		std::cerr << "maximum allowed: " << CLIENT_MAX_BODY_SIZE << std::endl;
		return (false);
	}
	return (true);
}

void	validErrorPages(std::map<int, std::string>& errorPages) {
	std::map<int, std::string>	m = defaultErrorPages();
	errorPages.insert(m.begin(), m.end());
}

void	validateServer(s_server& servStruct) {
	validServerName(servStruct.serverName, servStruct.listen.port);
	if (!validServerRoot(servStruct.root)) {
		std::cerr << "Root: not a valid root for server " << servStruct.serverName << std::endl;
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
