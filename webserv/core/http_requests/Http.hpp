/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:26:44 by pberset           #+#    #+#             */
/*   Updated: 2025/11/23 16:26:50 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP

# include <iostream>
# include <string>

class Http {
	public:
		Http(void);
		Http(const Http &other);
		~Http(void);
		Http    &operator=(const Http &other);

	private:
    // TODO: members
};
#endif

