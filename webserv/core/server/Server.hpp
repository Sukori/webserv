/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:22:28 by pberset           #+#    #+#             */
/*   Updated: 2025/11/23 16:22:33 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>

class Server {
	public:
		Server(void);
		Server(const Server &other);
		~Server(void);
		Server    &operator=(const Server &other);

	private:
    // TODO: members
};
#endif

