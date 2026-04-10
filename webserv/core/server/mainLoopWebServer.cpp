/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainLoopWebServer.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 17:44:30 by pberset           #+#    #+#             */
/*   Updated: 2026/04/09 17:09:53 by ylabussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <WebServer.hpp>

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
		ctrlno = poll(&_fds[0], _fds.size(), CLIENT_TIMEOUT_MS);
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
					const Server*	serv = _serverSockets.find(_fds[i].fd)->second;
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
	
						if (readBytes < 0) {
							_closeClient(it, _fds[i].fd, i);
							continue ;
						}

						if (it->second.isRequestComplete()) {
							_handleRequest(it, _clientsServers.find(_fds[i].fd)->second);
							_fds[i].events = POLLOUT;
						}

					} else if (_fds[i].revents & POLLOUT) {
						if (it->second.writeResponse(_fds[i].fd)) {
							_fds[i].events = POLLIN;
							it->second.reset();
						} 
					}
					if (time(NULL) - it->second.getLastActivityTime() > CLIENT_TIMEOUT_S) {
						_fds[i].revents = POLLHUP;
					}
					if (_fds[i].revents & POLLHUP) {
						_closeClient(it, _fds[i].fd, i);
						putLog("client timeout"); //error 408
					}
				}
			}
		}
	}
}
