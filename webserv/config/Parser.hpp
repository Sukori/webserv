/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 14:36:55 by pberset           #+#    #+#             */
/*   Updated: 2026/01/28 14:36:59 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PARSER_HPP
# define PARSER_HPP

# include <iostream>
# include <sstream>
# include <vector>
# include "Configuration.hpp"

class Parser {
public:
	Parser(void);
	~Parser(void);

	void	initParser(void);
	void	parseServer(void);
	void	parseLocation(void);

private:
	std::stringstream		_ss;
	std::vector<s_location>	_locations;
	std::vector<s_server>	_servers;
};

#endif

