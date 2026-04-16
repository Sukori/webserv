/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errPages.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neon-05 <neon-05@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 14:44:44 by pberset           #+#    #+#             */
/*   Updated: 2026/04/16 03:28:54 by neon-05          ###   ########.fr       */
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
405 method not allowed
408 timeout
500 Internal Server Error
502 Bad Gateway
503 Service Unavailable
504 Gateway Timeout
*/

std::map<int, std::string>	defaultErrorPages(void);

#endif

