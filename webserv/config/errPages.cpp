/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errPages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 16:29:47 by pberset           #+#    #+#             */
/*   Updated: 2026/03/17 16:30:01 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <errPages.hpp>

/// @brief generates a map of all the default error pages
/// @param  
/// @return map int - string, error code - path
std::map<int, std::string>	defaultErrorPages(void) {

	std::map<int, std::string> m;
    m[400] = "/www/error_pages/400.html";
    m[401] = "/www/error_pages/401.html";
    m[403] = "/www/error_pages/403.html";
    m[404] = "/www/error_pages/404.html";
    m[500] = "/www/error_pages/500.html";
    m[502] = "/www/error_pages/502.html";
    m[503] = "/www/error_pages/503.html";
    m[504] = "/www/error_pages/504.html";

	return (m);
}
