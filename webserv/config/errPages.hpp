/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errPages.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 14:44:44 by pberset           #+#    #+#             */
/*   Updated: 2026/03/17 14:45:07 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */


#ifndef ERRPAGES_HPP
# define ERRPAGES_HPP

# include <map>
# include <string>

/*
400 Bad Request
401 Unauthorized
403 Forbidden
404 Not Found
500 Internal Server Error
502 Bad Gateway
503 Service Unavailable
504 Gateway Timeout
*/

std::map<int, std::string>	defaultErrorPages(void);

#endif

