/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:12:29 by pberset           #+#    #+#             */
/*   Updated: 2026/01/14 16:38:14 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>

# define BUFFER_SIZE 4096

class Client {
	public:
		Client(int socket);
		~Client(void);

		ssize_t			readRequest(void);
		bool			writeResponse(void);

		std::string		getRequestIn(void);
		std::string		getResponseOut(void);
		ssize_t			getRequestSize(void);
		bool			isRequestComplete(void);
		int				getSocketStatus(void);

	private:
		Client(void);
		std::string		_requestIn;
		std::string		_responseOut;
		ssize_t			_requestSize;
		bool			_requestComplete;
		int				_socket;
};

#endif
