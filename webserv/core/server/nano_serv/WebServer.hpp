/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 14:10:51 by pberset           #+#    #+#             */
/*   Updated: 2026/01/19 14:10:59 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <iostream>
# include <sstream>
# include <string>
# include <vector>
# include <map>
# include <poll.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <sys/time.h>
# include "../../../config/Configuration.hpp" 
# include "Client.hpp"

# define BUFFER_SIZE 4096

class WebServer {
	public:
		WebServer(const Configuration& config);
		~WebServer(void);
	
		void	run(void);
	
	private:
		WebServer(void); 
		const Configuration&	_config;
		std::vector<pollfd>		_fds;
		std::map<int, Client>	_clients;
		int						_socket;
		int						_newSocket;
		const Server*			_findBestConfig(std::string host, int port); //rename ServerConfig when merge with config branch

		struct sockaddr_in		_socketAddress;

		int						_initServer(void);
		int						_closeServer(void);
		void					_acceptConnection(int& newSocket);
};

void	putLog(const std::string& message);
void	exitWithError(const std::string& funct,const std::string& message);

#endif

