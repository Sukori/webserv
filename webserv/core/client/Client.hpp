/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:12:29 by pberset           #+#    #+#             */
/*   Updated: 2026/04/15 16:50:38 by ylabussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <ctime>
# include <Http.hpp>
# include <helperParser.hpp>
# include <ByteString.hpp>
# include <Resource.hpp>

# define BUFFER_SIZE 1048576 //1MB

class Client {
	public:
		Client(void);
		~Client(void);

		std::size_t			readRequest(int socket);
		bool				readResource(void);
		bool				writeResponse(int socket);

		const ByteString&	getRequest(void) const;
		const ByteString&	getResponse(void) const;
		int					getResponseStatus(void) const;
		Resource&			getResource(void);
		
		bool				isRequestComplete(void) const;
		bool				isRequestFailed(void) const;
		time_t				getLastActivityTime(void) const;
		
		void				setResponse(const ByteString& response);
		void				setResponseStatus(int status);
		void				setResource(const Resource& resource);
		void				reset(void);

	private:
		Resource	_resource;
		ByteString	_request;
		ByteString	_response;
		int			_resp_status;
		bool		_requestComplete;
		bool		_requestFailed;
		time_t		_lastActivityTime;
};


#endif

