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
# include <sstream>
# include <string>
# include <cstring>
# include <unistd.h>
# include <vector>
# include <map>
# include <sys/stat.h>
# include <cerrno>

# include "helperConfig.hpp"
# include "Configuration.hpp"
# include "errPages.hpp"
# include "returnCodes.hpp"

//validate location

bool	validLocRoute(std::string& route);
bool	validLocRoot(std::string& root);
void	validReturns(std::map<int, std::string>& locReturn);
void	validLimitExcept(std::set<std::string>& limitExcept);
bool	validUploadPath(std::string& uploadPath);
//cgi_params ? subject to deletion
//cgi_pass ? subject to deletion
void	validateLocation(s_location& locStruct);


//validate server

void	validServerName(std::string& serverName, int port);
bool	validServerRoot(std::string& root);
void	validIndex(std::vector<std::string>& index);
void	validAccessLogs(std::string& accessLogs);
void	validErrorLogs(std::string& errorLogs);
bool	validClientMaxBodySize(unsigned int maxBodySize);
void	validErrorPages(std::map<int, std::string>& errorPages);
void	validateServer(s_server& servStruct);

#endif
