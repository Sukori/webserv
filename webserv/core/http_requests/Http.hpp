/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:26:44 by pberset           #+#    #+#             */
/*   Updated: 2025/12/16 17:20:36 by ylabussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP

# include <stdexcept>
# include <fstream>
# include <iostream>
# include <string>
# include <map>
# include <algorithm>

class HttpMessage {
	public:
		typedef std::map<std::string, std::string> header;

		HttpMessage(void);
		HttpMessage(const std::string&);
		HttpMessage(const HttpMessage&);
		~HttpMessage(void);
		HttpMessage&	operator=(const HttpMessage&);

		const std::string&	getStartLine(void) const;
		const header&		getHeaders(void) const;
		const std::string&	getBody(void) const;

		void	add_header_field(const std::string&);
		void	add_header_field(const std::string&, const std::string&);

	private:
		std::string							_start_line;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
};

std::ostream&	operator<<(std::ostream&, const HttpMessage&);

class Http {
	public:
		Http(void);
		Http(const Http &other);
		~Http(void);
		Http    &operator=(const Http &other);

	private:
    // TODO: members
};
#endif
