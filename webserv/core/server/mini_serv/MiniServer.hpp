/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MiniServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 11:07:46 by pberset           #+#    #+#             */
/*   Updated: 2026/01/13 17:34:30 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISERVER_HPP
# define MINISERVER_HPP

# include <sys/socket.h>
# include <arpa/inet.h>
# include <string>
# include <sstream>
# include <iostream>
# include <stdlib.h>
# include <unistd.h>

# define BUFFER_SIZE 30720

class MiniServer {
public:
    MiniServer(std::string ipAddr, int port);
    ~MiniServer(void);

	void	startListen(void);

private:
	std::string			_ipAddr;
	int					_port;
	int					_socket;
	int					_newSocket;
	long				_messageIn;
	struct sockaddr_in	_socketAddress;
	unsigned int		_socketAddrLen;
	std::string			_messageServer;

	int			startServer(void);
	int			closeServer(void);
	void		acceptConnection(int& new_socket);
	std::string	buildResponse(void);
	void		sendResponse(void);
};

void	putLog(const std::string& message);
void	exitWithError(const std::string& message);

#endif

