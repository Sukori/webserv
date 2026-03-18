/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helperParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 17:13:09 by pberset           #+#    #+#             */
/*   Updated: 2026/03/18 17:13:18 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPERPARSER_HPP
# define HELPERPARSER_HPP

# include <iostream>
# include <string>

unsigned int	ft_stoui(std::string& token);
void			ft_toLower(std::string& str);
void			ft_toLowerChr(char* c);
bool			ft_stob(std::string& token);
bool			ft_isNUM(std::string token);

#endif

