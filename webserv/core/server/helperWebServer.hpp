/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helperWebServer.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 17:36:21 by pberset           #+#    #+#             */
/*   Updated: 2026/03/18 17:36:35 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HELPERWEBSERVER_HPP
# define HELPERWEBSERVER_HPP

# include <iostream>
# include <string>
# include <map>
# include <poll.h>
# include <cstdlib>
# include <unistd.h>
# include <Configuration.hpp>

void	putLog(const std::string& message);
void	closeAllSockets(std::map<int, const Server*> serverSockets);
void	closeAllPollFds(std::vector<pollfd> fds);
void	exitWithError(const std::string& funct,const std::string& message);

#endif
