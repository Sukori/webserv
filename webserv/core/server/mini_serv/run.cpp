/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 11:09:48 by pberset           #+#    #+#             */
/*   Updated: 2026/01/13 17:37:28 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "MiniServer.hpp"

int	main() {
	MiniServer	server = MiniServer("0.0.0.0", 8080);
	server.startListen();
	return (0);
}

