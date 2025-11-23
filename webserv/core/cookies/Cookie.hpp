/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:24:20 by pberset           #+#    #+#             */
/*   Updated: 2025/11/23 16:24:28 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COOKIE_HPP
# define COOKIE_HPP

# include <iostream>
# include <string>

class Cookie {
	public:
		Cookie(void);
		Cookie(const Cookie &other);
		~Cookie(void);
		Cookie    &operator=(const Cookie &other);

	private:
    // TODO: members
};
#endif

