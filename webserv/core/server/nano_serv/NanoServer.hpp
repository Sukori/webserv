/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NanoServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 14:10:51 by pberset           #+#    #+#             */
/*   Updated: 2026/01/19 14:10:59 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NANOSERVER_HPP
# define NANOSERVER_HPP

# include <iostream>
# include <string>

class NanoServer {
public:
    NanoServer(void);
    NanoServer(const NanoServer &other);
    ~NanoServer(void);
    NanoServer &operator=(const NanoServer &other);

private:
    // TODO: members
};
#endif

