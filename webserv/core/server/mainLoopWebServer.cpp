/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainLoopWebServer.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 17:44:30 by pberset           #+#    #+#             */
/*   Updated: 2026/03/18 17:44:32 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

/// @brief Main loop server runtime
/// @param  none
void	WebServer::run(void) {

    std::cout << "run" << std::endl;
	int	ctrlno;
	_fds.clear();

	//listen to each sockets and populate the pollfd struct foreach sockets
	for (std::map<int, const Server*>::iterator it = _sockets.begin(); it != _sockets.end(); ++it) {
		ctrlno = listen(it->first, BACKLOG);
		if (ctrlno < 0) {
			exitWithError("listen", strerror(errno));
		}
		struct pollfd	pfd;
		pfd.fd = it->first;
		pfd.events = POLLIN;
		pfd.revents = 0;
		_fds.push_back(pfd);
	}

	//Main loop
	while (true) {
    std::cout << "===== Listening =====" << std::endl;
		ctrlno = poll(&_fds[0], _fds.size(), -1);
		if (ctrlno < 0) {
			closeAllSockets(_sockets);
			closeAllPollFds(_fds);
			exitWithError("poll", strerror(errno));
		} else if (ctrlno == 0) {
			continue ;
		}

		for (size_t i = 0; i < _fds.size(); i++) {
			if (_fds[i].revents == 0) {
				continue ;
			}
			if (_sockets.find(_fds[i].fd) != _sockets.end()) {
				int newSocket = _acceptConnection(_fds[i].fd);
				if (newSocket > 0) {
					_clients.insert(std::pair<int, Client>(newSocket, Client()));
					struct pollfd	npfd;
					npfd.fd = newSocket;
					npfd.events = POLLIN;
					npfd.revents = 0;
					_fds.push_back(npfd);

					putLog("New client");
				} else {
					continue ;
				}
			} else {
				std::map<int, Client>::iterator	it = _clients.find(_fds[i].fd);

				if (it != _clients.end()) {
					if (_fds[i].revents & POLLIN) {
						ssize_t read_bytes = it->second.readRequest(_fds[i].fd);
	
							if (read_bytes <= 0) {
								close(_fds[i].fd);
								_clients.erase(it);
								_fds.erase(_fds.begin() + i);
								i--;
								continue ;
							}
	
							if (it->second.isRequestComplete()) {
								_handleRequest(it->second);
								_fds[i].events = POLLOUT;
							}

					} else if (_fds[i].revents & POLLOUT) {
						if (it->second.writeResponse(_fds[i].fd)) { //for now, we close
							close(_fds[i].fd);
							_clients.erase(it);
							_fds.erase(_fds.begin() + i);
							i--;
							/*personal assumption
							 * it->second.events = POLLIN;
							 * clear the read write containers*/
						}
					}
				}
			}
		}
	}
}
