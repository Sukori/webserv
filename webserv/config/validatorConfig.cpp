/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validatorConfig.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:05:52 by pberset           #+#    #+#             */
/*   Updated: 2026/04/15 15:03:02 by ylabussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <validatorConfig.hpp>
#include <helperParser.hpp>
#include <stdexcept>

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

		std::cerr << "path: " << uploadPath << '\n';
		std::cerr << "stat: " << strerror(errno) << std::endl;
		std::cerr << "from validLocuploadPath" << std::endl;
		return (false);
	}

	return (S_ISDIR(buf.st_mode));
}

/// @brief checks all fields of a location block
/// @param locStruct 
void	validateLocation(s_location& locStruct) {

	locStruct.valid = true;
	
	/*if (!validLocRoute(locStruct.route)) {
		std::cerr << "Route: not a valid route for location " << locStruct.route << std::endl;
		locStruct.valid = false;
	}*/
	
	{
		std::string path (locStruct.servRoot + locStruct.root_path);
		if (!validLocRoot(path)) {
			std::cerr << "Root: not a valid root for location " <<  locStruct.root_path << std::endl;
			locStruct.valid = false;
		}
	}
	
	validReturns(locStruct.locReturn);
	
	validLimitExcept(locStruct.limit_except);
	
	{
		std::string path (locStruct.servRoot + locStruct.upload_path);
		if (!validUploadPath(path)) {
			std::cerr << "uploadPath: not a valid uploadPath for location " <<  locStruct.upload_path << std::endl;
			locStruct.valid = false;
		}
	}

	std::cout << "Location " << locStruct.route << "/ validated with status " << locStruct.valid << "\n" << std::endl;
}


//validate server

static void	replacePointsbySpaces(std::string& ip) {

	for (size_t i = 0; i < ip.length(); i++){
		if (ip[i] == '.')
			ip[i] = ' ';
	}
}

/// @brief checks ip format
/// @param ip 
/// @return bool
static bool	validIP(std::string ip) {
	
	replacePointsbySpaces(ip);

	std::istringstream	os(ip);
	std::string			byte;
	
		for (int i = 1; i < 5; i++) {
		os >> byte;
		if (byte.empty()) {
			std::cerr << "validIP: not enough parameters in ip address. expected 4, got " << i << std::endl;
			return (false);
		}
		if (!ft_isNUM(byte)) {
			std::cerr << "from validIP" << std::endl;
			return (false);
		}
		byte.clear();
	}
	return (true);
}

/// @brief validates ip format and port value
/// @param listen 
/// @return bool
bool	validListen(s_listen& listen) {

	if (!listen.ip.compare("localhost")) {
		listen.ip = "127.0.0.1";
		std::cout << "validListen: replaced \'localhost\' with \'127.0.0.1\'" << std::endl;
	} else if (!validIP(listen.ip)) {
		std::cerr << "from validListen" << std::endl;
		return (false);
	}

	if (listen.port == (unsigned int) -1) {
		std::cerr << "validListen: invalid port number (c.f. parser error)" << std::endl;
		return (false);
	}
	return (true);
}

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

/// @brief checks for valid binaries paths
/// @param bins 
/// @return bool
bool	validCgiBins(std::map<std::string, std::string>& bins) {
	
	struct stat	buf;
	std::map<std::string, std::string>::iterator	it = bins.begin();
	std::map<std::string, std::string>::iterator	end = bins.end();
	while (it != end){
		int status = stat(it->second.c_str(), &buf);

		if (status == -1) {
	
			std::cerr << "stat: " << strerror(errno) << std::endl;
			std::cerr << "from validCgiBins" << std::endl;
			return (false);
		}
		++it;
	}

	return (S_ISREG(buf.st_mode));
}

/// @brief checks client_max_body_sive value. warns the admin if 0. false if the value is too large
/// @param maxBodySize 
/// @return bool
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

	if (!validListen(servStruct.listen)) {
		std::cerr << "Listen: not a valid <interface:port> got " << servStruct.listen.ip << ":" << servStruct.listen.port << std::endl;
		servStruct.valid = false;
	}

	validServerName(servStruct.serverName, servStruct.listen.port);
	if (!validServerRoot(servStruct.root)) {
		std::cerr << "Root: not a valid root for server " << servStruct.serverName << std::endl;
		servStruct.valid = false;
	}

	validIndex(servStruct.index);

	if (!servStruct.cgi_bins.empty() && !validCgiBins(servStruct.cgi_bins)) {
		std::cerr << "cgi_bin: invalid cgi binaries for server " << servStruct.serverName << std::endl;
	}

	if (!validClientMaxBodySize(servStruct.client_max_body_size)) {
		std::cerr << "client_max_body_size: not a valid max client body size for server " << servStruct.serverName << std::endl;
		servStruct.valid = false;
	}
	validErrorPages(servStruct.error_pages);

	std::cout << "Validated server " << servStruct.serverName << " with status " << servStruct.valid << "\n" << std::endl;
}
