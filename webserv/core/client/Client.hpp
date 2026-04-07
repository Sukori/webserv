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
# include <Http.hpp>
# include <helperParser.hpp>
# include <ByteString.hpp>

# define BUFFER_SIZE 1048576 //1MB

class Client {
	public:
		Client(void);
		~Client(void);

		std::size_t			readRequest(int socket);
		bool				writeResponse(int socket);

		const ByteString&	getRequest(void) const;
		const ByteString&	getResponse(void) const;

		bool				isRequestComplete(void) const;

		void				setResponse(const ByteString& response);

	private:
		ByteString	_request;
		ByteString	_response;
		bool		_requestComplete;
};


#endif

