/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validatorConfig.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:06:05 by pberset           #+#    #+#             */
/*   Updated: 2026/03/13 14:06:06 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATORCONFIG_HPP
# define VALIDATORCONFIG_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <sys/stat.h> //stat() to check directory existence

# include "helperConfig.hpp"

//validate location

bool	validLocRoute(std::string& route);
bool	validLocRoot(std::string& root);
void	validLimitExcept(std::vector<std::string>& limitExcept); //default GET is not specified
void	validUploadPath(std::string& uploadPath); //default /www/upload if not specified. Create if not exists
//cgi_params ? subject to deletion
//cgi_pass ? subject to deletion
void	validateLocation(s_location& locStruct);


//validate server

void	validServerName(std::string& serverName); //if not set, use "server<port>"
bool	validServerRoot(std::string& root);
bool	validPort(int port);
void	validIndex(std::vector<std::string>& index); //if not set, use "index.html"
void	validAccessLogs(std::string& accessLogs); //create if not exists, can be null
void	validErrorLogs(std::string& errorLogs); //create if not exists, can be null
bool	validClientMaxBodySize(unsigned int maxBodySize);
void	validErrorPages(std::map<int, std::string>& errorPages);
void	validateServer(s_server& servStruct);

#endif
