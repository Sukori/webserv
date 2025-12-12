/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 17:24:08 by pberset           #+#    #+#             */
/*   Updated: 2025/12/12 18:55:17 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include <Configuration.hpp>

int	main(int argc, char* argv[]) {

if (argc != 2) {
		std::cerr << "input: need exactly one config file";
		return (1);
	}
	Configuration	conf;
	conf.openConfiguration(argv[1]);
	return (0);
}

