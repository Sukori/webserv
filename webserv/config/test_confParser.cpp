/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_confParser.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 17:24:08 by pberset           #+#    #+#             */
/*   Updated: 2025/12/12 17:41:43 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

int	main(int argc, char* argv[]) {
	(void)argc;
	ConfParser::configureServer(argv[1]);
	return (0);
}

