/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:12:29 by pberset           #+#    #+#             */
/*   Updated: 2026/01/24 15:50:38 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>
# include <arpa/inet.h>

# define BUFFER_SIZE 4096

class Client {
	public:
		Client(sockaddr_in socket);
		~Client(void);

		ssize_t				readRequest(void);
		bool				writeResponse(void);

		std::string			getRequestIn(void);
		std::string			getResponseOut(void);
		ssize_t				getRequestSize(void);
		bool				isRequestComplete(void);
		struct sockaddr_in	getSocket(void);

		void				setResponse(const std::string& response);

	private:
		Client(void);
		std::string			_requestIn;
		std::string			_responseOut;
		ssize_t				_requestSize;
		bool				_requestComplete;
		struct sockaddr_in	_socket;
};

#endif

