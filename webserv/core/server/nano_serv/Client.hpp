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

class Client {
public:
    Client(int socket);
    ~Client(void);

	int	readRequest(void);

private:
	std::string			_requestIn;
	std::string			_responseOut;
	long unsigned int	_messageSize;
	bool				_messageComplete;
	int					_socket;
};

#endif

