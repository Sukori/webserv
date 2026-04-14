/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neon-05 <neon-05@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 14:10:51 by pberset           #+#    #+#             */
/*   Updated: 2026/04/14 16:48:21 by neon-05          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <iostream>
# include <sstream>
# include <string>
# include <cstring>
# include <vector>
# include <map>
# include <set>
# include <cerrno>
# include <cstdlib>
# include <fcntl.h>
# include <poll.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/time.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <signal.h>
# include <time.h>
# include <Configuration.hpp>
# include <Parser.hpp>
# include <Client.hpp>
# include <helperWebServer.hpp>
# include <Http.hpp>
# include <Resource.hpp>

# define BACKLOG 1024 //concurrent clients
# define CLIENT_TIMEOUT_MS 15000
# define CLIENT_TIMEOUT_S (CLIENT_TIMEOUT_MS / 1000)

class WebServer {
	public:
		WebServer(const Configuration& config);
		~WebServer(void);

		static void	installSignalHandlers(void);
	
		void	run(void);
	
	private:
		WebServer(void); 
		Configuration					_config;
		std::vector<pollfd>				_fds;
		std::map<int, Client*>			_clientRfds;
		std::map<int, Client>			_clientSockets;
		std::map<int, const Resource*>	_resources;
		std::map<int, const Server*>	_serverSockets; // socket server map Server
		std::map<int, const Server*>	_clientsServers; // socket client map Server
		
		static volatile sig_atomic_t	_stopRequested;
		static void					_handleSignal(int sig);

		int						_initServer(const struct addrinfo* addrinfo, const Server* server);
		void					_closeServer(void);
		int						_acceptConnection(int fd);
		Resource				_handleRequest(Client& client, const Server& server);
		void					_handleResponse(Client& client, const Server& server);
		void					_closeClient(std::map<int, Client>::iterator& it, size_t& i);
};

#endif
