/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:12:29 by pberset           #+#    #+#             */
/*   Updated: 2026/03/30 15:22:55 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>
# include <arpa/inet.h>
# include "../http/Http.hpp"
# include "../../config/helperParser.hpp"

# define BUFFER_SIZE 4096

class Client {
	public:
		Client(void);
		~Client(void);

		ssize_t		readRequest(int socket);
		bool		writeResponse(int socket);

		std::string	getRequestIn(void);
		std::string	getResponseOut(void);
		ssize_t		getHeaderSize(void);
		ssize_t		getBodySize(void);
		bool		isRequestComplete(void);

		void		setResponse(const std::string& response);

	private:
		std::string	_requestIn;
		std::string	_responseOut;
		std::string	_rawHeaderIn;
		ssize_t		_headerSize;
		bool		_headerComplete;
		bool		_alreadyChecked;
		bool		_expectsBody;
		ssize_t		_expectedBodySize;
		ssize_t		_bodySize;
		bool		_bodyComplete;
};


#endif

