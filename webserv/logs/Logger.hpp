/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 11:23:07 by pberset           #+#    #+#             */
/*   Updated: 2026/04/15 11:23:19 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <iostream>
# include <fstream>
# include <string>

class Logger {
	public:
		Logger(void);
		~Logger(void);
		
		void	init(const std::string& fileName);
		void	log(const std::string& message);

	private:
		Logger(const Logger& other);
		Logger& operator=(const Logger& other);
		
		std::ofstream	_logs;
};

#endif
