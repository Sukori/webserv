/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errPages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 16:29:47 by pberset           #+#    #+#             */
/*   Updated: 2026/04/15 16:30:06 by ylabussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errPages.hpp>

/// @brief generates a map of all the default error pages
/// @param  
/// @return map int - string, error code - path
std::map<int, std::string>	defaultErrorPages(void) {

	std::map<int, std::string> m;
    m[400] = "./www/default_error_pages/400.html";
    m[401] = "./www/default_error_pages/401.html";
    m[403] = "./www/default_error_pages/403.html";
    m[404] = "./www/default_error_pages/404.html";
    m[405] = "./www/default_error_pages/405.html";
    m[408] = "./www/default_error_pages/408.html";
    m[413] = "./www/default_error_pages/413.html";
    m[500] = "./www/default_error_pages/500.html";
    m[502] = "./www/default_error_pages/502.html";
    m[503] = "./www/default_error_pages/503.html";
    m[504] = "./www/default_error_pages/504.html";

	return (m);
}
