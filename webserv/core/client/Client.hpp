/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:20:21 by pberset           #+#    #+#             */
/*   Updated: 2026/01/19 15:08:24 by ylabussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>

class Client {
	public:
		Client(int socket);
		Client(const Client& other);
		~Client(void);
		Client&		operator=(const Client& other);

		std::string			getRequestIn(void);
		std::string			getResponseOut(void);
		long unsigned int	getMessageSize(void);
		bool				isMessageComplete(void);
		int					getSocketStatus(void);

	private:
		Client(void);
		std::string			_requestIn;
		std::string			_responseOut;
		long unsigned int	_messageSize;
		bool				_messageComplete;
		int					_socket;
};

#endif

