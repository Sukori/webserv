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

	int	ctrlno;
	_fds.clear();

	//listen to each sockets and populate the pollfd struct foreach sockets
	for (std::map<int, const Server*>::iterator it = _serverSockets.begin(); it != _serverSockets.end(); ++it) {
		
		ctrlno = listen(it->first, BACKLOG);
		if (ctrlno < 0) {
			std::cerr << "listen: " << strerror(errno) << " " << it->second->getName() << std::endl
			<< "skipped server" << std::endl;
			continue ;
		}
		
		struct pollfd	pfd;
		pfd.fd = it->first;
		pfd.events = POLLIN;
		pfd.revents = 0;
		_fds.push_back(pfd);
	}

	if (_fds.empty()){
		std::cerr << "No valid server to start. program ended." << std::endl;
		return ;
	}

	//Main loop
	while (!_stopRequested) {
    std::cout << "===== Listening =====" << std::endl;
		ctrlno = poll(&_fds[0], _fds.size(), -1);
		if (ctrlno < 0) {
			if (errno == EINTR) {
				continue ;
			}
			closeAllSockets(_serverSockets);
			closeAllPollFds(_fds);
			exitWithError("poll", strerror(errno));
		} else if (ctrlno == 0) {
			continue ;
		}

		for (size_t i = 0; i < _fds.size(); i++) {
			if (_fds[i].revents == 0) {
				continue ;
			}
			if (_serverSockets.find(_fds[i].fd) != _serverSockets.end()) {
				int newSocket = _acceptConnection(_fds[i].fd);
				if (newSocket > 0) {
					const Server*			serv = _serverSockets.find(_fds[i].fd)->second;
					_clients[newSocket];
					_clientsServers.insert(std::make_pair(newSocket, serv));
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
						ssize_t readBytes;
						try {
							readBytes = it->second.readRequest(_fds[i].fd);
						} catch (int status) {
							std::cerr << "log temporaire: " << status << std::endl;
						}
	
						if (readBytes <= 0) {
							close(_fds[i].fd);
							_clients.erase(it);
							_clientsServers.erase(_fds[i].fd);
							_fds.erase(_fds.begin() + i);
							i--;
							continue ;
						}

						if (it->second.isRequestComplete()) {
							_handleRequest(it, _clientsServers.find(_fds[i].fd)->second);
							_fds[i].events = POLLOUT;
						}

					} else if (_fds[i].revents & POLLOUT) {
						if (it->second.writeResponse(_fds[i].fd)) { //for now, we close
							close(_fds[i].fd);
							_clients.erase(it);
							_clientsServers.erase(_fds[i].fd);
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
