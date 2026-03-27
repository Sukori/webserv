/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helperWebServer.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 17:36:42 by pberset           #+#    #+#             */
/*   Updated: 2026/03/18 17:36:49 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "helperWebServer.hpp"

/// @brief logs a formatted message
/// @param message 
void	putLog(const std::string& message) {
	std::cout << message << std::endl;
}

/// @brief closes all sockets
/// @param serverSockets 
void	closeAllSockets(std::map<int, const Server*> serverSockets) {
	for (std::map<int, const Server*>::iterator it = serverSockets.begin(); it != serverSockets.end(); ++it) {
		close(it->first);
	}
}

/// @brief closes all pollfds
/// @param fds 
void	closeAllPollFds(std::vector<pollfd> fds) {
	for (size_t i = 0; i < fds.size(); i++) {
		close(fds[i].fd);
	}
}

/// @brief logs a formatted messages and exits the program
/// @param funct 
/// @param message 
void	exitWithError(const std::string& funct, const std::string& message) {
	putLog(funct +": " + message);
	exit(1);
}
