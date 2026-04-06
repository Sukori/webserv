/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 14:10:51 by pberset           #+#    #+#             */
/*   Updated: 2026/01/25 15:09:11 by pberset          ###   Lausanne.ch       */
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
# include <Configuration.hpp>
# include <Parser.hpp>
# include <Client.hpp>
# include <helperWebServer.hpp>
# include <Http.hpp>

# define BACKLOG 1024

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
		std::map<int, Client>			_clients;
		std::map<int, const Server*>	_serverSockets;
		std::map<int, const Server*>	_clientsServers;

		static volatile sig_atomic_t	_stopRequested;
		static void					_handleSignal(int sig);

		int						_initServer(const struct addrinfo* addrinfo, const Server* server);
		void					_closeServer(void);
		int						_acceptConnection(int fd);
		void					_handleRequest(std::map<int, Client>::iterator& client, const Server* server);
};

#endif
