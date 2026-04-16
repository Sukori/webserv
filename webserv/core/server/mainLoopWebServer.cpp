/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainLoopWebServer.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 17:44:30 by pberset           #+#    #+#             */
/*   Updated: 2026/04/15 15:36:51 by ylabussi         ###   ########.fr       */
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
	std::ostringstream osLog;
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
			const Server*	serv = NULL;
			if (_serverSockets.find(_fds[i].fd) != _serverSockets.end()) {
				serv = _serverSockets.find(_fds[i].fd)->second;
				int newSocket = _acceptConnection(_fds[i].fd);
				if (newSocket > 0) {
					_clientSockets[newSocket]; // can't default constructr this. either make a vector in config for example or new Client;
					_clientsServers.insert(std::make_pair(newSocket, serv));
					struct pollfd	npfd;
					npfd.fd = newSocket;
					npfd.events = POLLIN;
					npfd.revents = 0;
					_fds.push_back(npfd);
					{
						osLog << "New client connected on socket " << newSocket;
						serv->getAccStream()->log(osLog.str());
						osLog.str() = "";
					}
				} else {
					continue ;
				}
			} else {
				std::map<int, Client>::iterator	it = _clientSockets.find(_fds[i].fd);
				if (it != _clientSockets.end()) {
					serv = _clientsServers.find(_fds[i].fd)->second;
					if (_fds[i].revents & POLLIN) {
						if (!it->second.isRequestComplete()) {
							
							try {
								it->second.readRequest(_fds[i].fd, serv);
							} catch (int status) {
								osLog << "closing client with status: " << status;
								serv->getErrStream()->log(osLog.str());
								osLog.str() = "";
								_closeClient(it, i);
								continue ;
							}
							if (it->second.isRequestComplete()) {
								Resource res (_handleRequest(it->second, *_clientsServers.find(_fds[i].fd)->second));
								osLog << "resource initialized on fd " << res.getFd();
								serv->getAccStream()->log(osLog.str());
								osLog.str() = "";

								_fds[i].events = POLLOUT;
								it->second.setResource(res);
								struct pollfd	npfd;
								npfd.fd = res.getFd(); //-> fd ressource exec 1 fois
								npfd.events = POLLIN;
								npfd.revents = 0;
								_fds.push_back(npfd);
								_clientRfds[npfd.fd] = &it->second;
							}
						}
					} else if (_fds[i].revents & POLLOUT) {
						_handleResponse(it->second, *_clientsServers.find(_fds[i].fd)->second); //-> build response et append to client._response exec 1 fois
						if (it->second.writeResponse(_fds[i].fd, serv->getAccStream())) {
							_fds[i].events = POLLIN;
							it->second.reset();
						}
					}
					osLog << "closing socket " << it->first;
					if (time(NULL) - it->second.getLastActivityTime() > CLIENT_TIMEOUT_S) {
						it->second.setResponse(Http::buildResponse("\rtimeout", 408, _clientsServers.find(_fds[i].fd)->second->getName()));
						it->second.writeResponse(_fds[i].fd, serv->getErrStream());
						_closeClient(it, i);
						serv->getErrStream()->log("client timeout" + osLog.str()); //error 408
					}
					if (_fds[i].revents & POLLHUP) {
						_closeClient(it, i);
						serv->getErrStream()->log("client hangup" + osLog.str()); //error 408
					}
					osLog.str() = "";
				} else {
					std::map<int, Client*>::iterator	itt = _clientRfds.find(_fds[i].fd);
					if (itt != _clientRfds.end()) {
						int clientFd = -1;
						for (std::map<int, Client>::iterator sit = _clientSockets.begin(); sit != _clientSockets.end(); ++sit) {
							if (&sit->second == itt->second) {
								clientFd = sit->first;
								break;
							}
						}
						if (clientFd != -1) {
							serv = _clientsServers.find(clientFd)->second;
						}
						if (serv != NULL) {
							serv->getAccStream()->log("reading resource");
							if (itt->second->readResource(serv->getAccStream())) { //-> lit fd jusqu'a EOF exec boucle
								serv->getAccStream()->log("resource fully read");
								_fds.erase(_fds.begin() + i);
								_clientRfds.erase(itt);
							}
						}
					}
				}
			}
		}
	}
}
