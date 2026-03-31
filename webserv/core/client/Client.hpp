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
# include <cstring>
# include <sys/socket.h>
# include <arpa/inet.h>
# include "../http/Http.hpp"
# include "../../config/helperParser.hpp"
# include "../utils/ByteString.hpp"

# define BUFFER_SIZE 4096

class Client {
	public:
		Client(void);
		~Client(void);

		std::size_t			readRequest(int socket);
		bool				writeResponse(int socket);

		const ByteString&	getRequestBodyIn(void) const;
		const ByteString&	getResponseBodyOut(void) const;
		const std::string&	getrawHeaderIn(void) const;
		const std::string&	getRawHeaderOut(void) const;
		std::size_t			getHeaderSize(void) const;
		std::size_t			getBodySize(void) const;
		bool				isRequestComplete(void) const;

		void				setResponse(const std::string& header, const ByteString& body);

	private:
		//std::string	_requestIn;
		//std::string	_responseOut;
		ByteString	_requestBodyIn;
		ByteString	_responseBodyOut;
		std::string	_rawHeaderIn;
		std::string	_rawHeaderOut;
		std::size_t	_headerSize;
		bool		_headerComplete;
		bool		_alreadyChecked;
		bool		_expectsBody;
		std::size_t	_expectedBodySize;
		std::size_t	_bodySize;
		bool		_bodyComplete;
};


#endif

