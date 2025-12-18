/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMessage.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:26:44 by pberset           #+#    #+#             */
/*   Updated: 2025/12/18 15:35:21 by ylabussi         ###   ########.fr       */
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

/*
Class to store and manipulate an http message

*/
class HttpMessage {
	public:

		typedef std::map<std::string, std::string> header;

		enum MessageType{
			REQUEST,
			RESPONSE,
			UNDEFINED
		};

		/* create an empty message */
		HttpMessage(void);

		/* create and parse message from raw input string */
		HttpMessage(const std::string&);

		/* create a copy of message from argument */
		HttpMessage(const HttpMessage&);
		
		~HttpMessage(void);
		HttpMessage&	operator=(const HttpMessage&);

		const std::string&	getStartLine(void) const;
		const header&		getHeader(void) const;
		const std::string&	getBody(void) const;

		MessageType			getMessageType(void) const;
		bool				operator==(MessageType) const;
		bool				operator!=(MessageType) const;

		void				setStartLine(const std::string&);
		void				setBody(const std::string&);

		/*
		header line MUST follow the format 
			header	=	key ":" [ value ]
		*/
		void	add_header_field(const std::string&);
		void	add_header_field(const std::string&, const std::string&);

	protected:
		std::string							_start_line;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
};

std::ostream&	operator<<(std::ostream&, const HttpMessage&);

#endif
