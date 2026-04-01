/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:12:29 by pberset           #+#    #+#             */
/*   Updated: 2026/04/01 15:52:59 by ylabussi         ###   ########.fr       */
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

		const ByteString&	getRequestIn(void) const;
		const ByteString&	getResponseOut(void) const;
		std::size_t			getHeaderSize(void) const;
		std::size_t			getBodySize(void) const;
		bool				isRequestComplete(void) const;

		void				setResponse(const ByteString& message);

	private:
		//std::string	_requestIn;
		//std::string	_responseOut;
		ByteString	_requestIn;
		ByteString	_responseOut;
		bool		_requestComplete;
};


#endif

