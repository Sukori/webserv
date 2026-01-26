/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:22:28 by pberset           #+#    #+#             */
/*   Updated: 2026/01/09 16:24:17 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <unistd.h>
# include <sys/socket.h>
# include <cstring>
# include <arpa/inet.h>
# include <fstream>
# include <sstream>

class Server {
	public:
		Server(void);
		Server(const Server &other);
		~Server(void);
		Server    &operator=(const Server &other);

	private:
    // TODO: members
};

int start_server(void);

#endif

