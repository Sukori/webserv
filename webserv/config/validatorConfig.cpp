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

/// @brief checks if given route is a duplicate
/// @param route 
/// @return bool 
bool	validLocRoute(std::string& route) {

	if (route.empty()) { //should never happen
		std::cerr << "validLocRoute: empty route token" << std::endl;
		return (false);
	}
	return (true);

	/* Actually, need to test root, but not route
	 * Not necessarily an existing folder
	struct stat	buf;
	int status = stat(route.c_str(), &buf);

	if (status == -1) {

		std::cerr << "stat: " << strerror(errno) << std::endl;
		std::cerr << "from validLocRoute" << std::endl;
		return (false);
	}

	return (S_ISDIR(buf.st_mode) || S_ISREG(buf.st_mode));
	*/
}

/// @brief checks if the giver root is valid. Directory only
/// @param root 
/// @return bool
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

/// @brief checks if a return rule meets requirements
/// @param locReturn 
void	validReturns(std::map<int, std::string>& locReturn) {

	//Remove error
	std::map<int, std::string>::iterator toErase = locReturn.find(0);
	if (toErase != locReturn.end()) {
		std::cerr << "validReturns: removed error placeholder" << std::endl;
		locReturn.erase(toErase);
	}

	for (std::map<int, std::string>::iterator pair = locReturn.begin(); pair != locReturn.end(); ) {
		if (needUrlCode(pair->first) && pair->second == ""){
			std::cerr << "validReturns: missing mandatory redirection path for code " << pair->first << std::endl
				<< "skip" << std::endl;
			locReturn.erase(pair++);
		} else if (!standaloneCode(pair->first) && !needUrlCode(pair->first)) {
			std::cerr << "validReturns: Unknown code " << pair->first << std::endl
				<< "skip" << std::endl;
			locReturn.erase(pair++);
		} else {
			++pair;
		}
	}
}

/// @brief checks if the method is accepted by the server
/// @param method 
/// @return bool
static bool	isValidMethod(const std::string& method) {
	return (method == "GET" || method == "POST" || method == "DELETE");
}

/// @brief removes any duplicates of valid method from the vector methods
/// @param methods 
static void	removeDuplicates(std::set<std::string>& methods) {

	std::set<std::string>::iterator	method = methods.begin();
	std::map<std::string, bool>			seen;

	while (method != methods.end()) {
		bool	validMethod = isValidMethod(*method);
		//if the method is not valid, It will be deleted by the validator
		if (validMethod && seen[*method]) {
				std::cerr << "removeDuplicates: duplicate valid method ignored " << *method<< std::endl;
				methods.erase(method++);
		} else {
			if (validMethod)
				seen[*method] = true;
			method++;
		}
	}
}

/// @brief Removes invalid methods found in the limitExcept parameter defaults to "GET" if empty
/// @param limitExcept 
void	validLimitExcept(std::set<std::string>& limitExcept) {

	if (!limitExcept.empty()) {
		removeDuplicates(limitExcept);
	} else {
		std::cerr << "validLimitExcept: empty list of allowed methods, default to " << DFT_METHOD << std::endl;
		limitExcept.insert(DFT_METHOD);
		return ;
	}

	std::set<std::string>::iterator method = limitExcept.begin();

	while (method != limitExcept.end()) {
		bool	validMethod = isValidMethod(*method);
		
		if (!validMethod) {
			std::cerr << "validLimitExcept: invalid method ignored " << *method << std::endl;
			limitExcept.erase(method++);
			continue ;
		}
		method++;
	}

	if (limitExcept.empty()) {
		std::cerr << "validLimitExcept: empty list of allowed methods, default to " << DFT_METHOD << std::endl;
		limitExcept.insert(DFT_METHOD);
	}
}

/// @brief checks if the upload path exists
/// @param uploadPath 
/// @return bool
bool	validUploadPath(std::string& uploadPath) {

	if (uploadPath.empty()) {
		return (true);
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

/// @brief checks all fields of a location block
/// @param locStruct 
void	validateLocation(s_location& locStruct) {

	locStruct.valid = true;
	
	if (!validLocRoute(locStruct.route)) {
		std::cerr << "Route: not a valid route for location " << locStruct.route << std::endl;
		locStruct.valid = false;
	}
	
	if (!validLocRoot(locStruct.root_path)) {
		std::cerr << "Root: not a valid root for location " <<  locStruct.root_path << std::endl;
		locStruct.valid = false;
	}

	validReturns(locStruct.locReturn);
	
	validLimitExcept(locStruct.limit_except);
	
	if (!validUploadPath(locStruct.upload_path)) {
		std::cerr << "uploadPath: not a valid uploadPath for location " <<  locStruct.upload_path << std::endl;
		locStruct.valid = false;
	}
}


//validate server

/// @brief chcecks if the server has a name. Sets "serverPORT" if empty
/// @param serverName 
/// @param port 
void	validServerName(std::string& serverName, int port) {

	if (serverName.empty()) {
		std::cerr << "validServerName: empty name token, defaulting to port number" << std::endl;
		std::stringstream ss;
		ss << "server";
		ss << port;
		ss >> serverName;
	}
}

/// @brief checks if root is valid
/// @param root 
/// @return bool
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

/// @brief sets {index.html, index.php} if empty
/// @param index 
void	validIndex(std::vector<std::string>& index) {

	if (index.empty()) {
		std::cerr << "validIndex: empty index token list, defaulting to {index.html, index.php}" << std::endl;
		index.push_back("index.html");
		index.push_back("index.php");
	}
}

/// @brief warns the admin if the accesLogs file is invalid or non-existant
/// @param accessLogs 
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

/// @brief warns the admin if the accesLogs file is invalid or non-existant
/// @param errorLogs 
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

/// @brief checks client_max_body_sive value. warns the admin if 0. false if the value is too large
/// @param maxBodySize 
/// @return bool
bool	validClientMaxBodySize(unsigned int maxBodySize) {
	if (maxBodySize == 0) {
		std::cerr << "Warning! You are running with client_max_body_size = 0 (disabled)" << std::endl;
		std::cerr << "if this is not your intention, modify the config and reboot the server" << std::endl;
	} /*else if (maxBodySize > CLIENT_MAX_BODY_SIZE) { Should we really do that ?
		std::cerr << "validClientMaxBodySize: set client_max_body_size is too large" << std::endl;
		std::cerr << "maximum allowed: " << CLIENT_MAX_BODY_SIZE << std::endl;
		return (false);
	}*/
	return (true);
}

/// @brief fills a minimal set of default error pages, if not already specified
/// @param errorPages 
void	validErrorPages(std::map<int, std::string>& errorPages) {
	std::map<int, std::string>	m = defaultErrorPages();
	errorPages.insert(m.begin(), m.end());
}

/// @brief checks the server block
/// @param servStruct 
void	validateServer(s_server& servStruct) {

	servStruct.valid = true;

	validServerName(servStruct.serverName, servStruct.listen.port);
	if (!validServerRoot(servStruct.root)) {
		std::cerr << "Root: not a valid root for server " << servStruct.serverName << std::endl;
		servStruct.valid = false;
	}

	validIndex(servStruct.index);
	validAccessLogs(servStruct.access_logs);
	validErrorLogs(servStruct.error_logs);
	if (!validClientMaxBodySize(servStruct.client_max_body_size)) {
		std::cerr << "client_max_body_size: not a valid max client body size for server " << servStruct.client_max_body_size << std::endl;
		servStruct.valid = false;
	}
	validErrorPages(servStruct.error_pages);
}
