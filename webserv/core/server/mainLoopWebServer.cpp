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
					_clientSockets[newSocket]; // can't default constructr this. either make a vector in config for example or new Client;
					_clientsServers.insert(std::make_pair(newSocket, serv));
					struct pollfd	npfd;
					npfd.fd = newSocket;
					npfd.events = POLLIN;
					npfd.revents = 0;
					_fds.push_back(npfd);
					{
						std::ostringstream os;
						os << "New client connected on socket " << newSocket;
						serv->getAccStream()->log(os.str());
					}
				} else {
					continue ;
				}
			} else {
				std::map<int, Client>::iterator	it = _clientSockets.find(_fds[i].fd);
				if (it != _clientSockets.end()) {
					if (_fds[i].revents & POLLIN) {
						if (!it->second.isRequestComplete()) {
							
							try {
								it->second.readRequest(_fds[i].fd);
							} catch (int status) {
								std::cerr << "log temporaire: " << status << std::endl;
								_closeClient(it, i);
								continue ;
							}
							if (it->second.isRequestComplete()) {
								Resource res (_handleRequest(it->second, *_clientsServers.find(_fds[i].fd)->second));
								std::cerr << "resource initialized on fd " << res.getFd() << '\n';

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
						if (it->second.writeResponse(_fds[i].fd)) {
							_fds[i].events = POLLIN;
							it->second.reset();
						}
					}
					if (time(NULL) - it->second.getLastActivityTime() > CLIENT_TIMEOUT_S) {
						it->second.setResponse(Http::buildResponse("\r\ntimeout", 408, _clientsServers.find(_fds[i].fd)->second->getName()));
						it->second.writeResponse(_fds[i].fd);
						_closeClient(it, i);
						putLog("client timeout"); //error 408
					}
					if (_fds[i].revents & POLLHUP) {
						_closeClient(it, i);
						putLog("client hangup"); //error 408
					}
				} else {
					std::map<int, Client*>::iterator	itt = _clientRfds.find(_fds[i].fd);
					std::cout << "reading resource\n";
					if (itt->second->readResource()) { //-> lit fd jusqu'a EOF exec boucle
						std::cout << "resource fully read\n";
						_fds.erase(_fds.begin() + i);
						_clientRfds.erase(itt);
					}
				}
			}
		}
	}
}
