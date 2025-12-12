/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:22:45 by pberset           #+#    #+#             */
/*   Updated: 2025/12/12 18:56:56 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include <iostream>
# include <string>
# include <fstream>

class Configuration {
	public:
		Configuration(void);
		//Configuration(const Configuration &other);
		~Configuration(void);
		//Configuration   &operator=(const Configuration &other);
		//Configuration	&operator<<(const Configuration &other);

		int				openConfiguration(const std::string& file);
		int				parseConfiguration(const std::ifstream& data);
		//setters
		//getters

	private:
    // TODO: members elements of config splitted
};

#endif

