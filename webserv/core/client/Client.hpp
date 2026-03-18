/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
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
		Client(void);
		~Client(void);

		ssize_t				readRequest(int socket);
		bool				writeResponse(int socket);

		std::string			getRequestIn(void);
		std::string			getResponseOut(void);
		ssize_t				getRequestSize(void);
		bool				isRequestComplete(void);

		void				setResponse(const std::string& response);

	private:
		std::string			_requestIn;
		std::string			_responseOut;
		ssize_t				_requestSize;
		bool				_requestComplete;
};


#endif

