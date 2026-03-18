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
# include "../../config/Configuration.hpp" 
# include "../client/Client.hpp"
# include "helperWebServer.hpp"

# define BUFFER_SIZE 4096
# define BACKLOG 1024

class WebServer {
	public:
		WebServer(const Configuration& config);
		~WebServer(void);
	
		void	run(void);
	
	private:
		WebServer(void); 
		Configuration					_config;
		std::vector<pollfd>				_fds;
		std::map<int, Client>			_clients;
		std::map<int, const Server*>	_sockets;

		int						_initServer(const struct addrinfo* addrinfo, const Server* server);
		void					_closeServer(void);
		int						_acceptConnection(int fd);
		void					_handleRequest(Client& client);
		const Server*			_findBestConfig(std::string host, int port); //rename ServerConfig when merge with config branch
};

#endif
