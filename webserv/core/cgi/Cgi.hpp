/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:17:49 by pberset           #+#    #+#             */
/*   Updated: 2025/11/23 16:17:56 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>
# include <string>

class Cgi {
	public:
		Cgi(void);
		Cgi(const Cgi &other);
		~Cgi(void);
		Cgi    &operator=(const Cgi &other);

	private:
    // TODO: members
};
#endif

