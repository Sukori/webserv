/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMessage.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:26:44 by pberset           #+#    #+#             */
/*   Updated: 2026/01/06 15:52:54 by ylabussi         ###   ########.fr       */
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

		/* destrozs message */
		~HttpMessage(void);

		/* copies data from argument message in *this */
		HttpMessage&	operator=(const HttpMessage&);

		/* getter for the start line */
		const std::string&	getStartLine(void) const;

		/* getter for the header */
		const header&		getHeader(void) const;

		/* getter for the message body */
		const std::string&	getBody(void) const;

		/* gets the message type from start line */
		MessageType			getMessageType(void) const;

		/* checks if message type matches with argument */
		bool				operator==(MessageType) const;

		/* checks if message type does not matche with argument */
		bool				operator!=(MessageType) const;

		/* copies argument to start line field */
		void				setStartLine(const std::string&);

		/* copies argument to body field */
		void				setBody(const std::string&);

		/*
		adds one field to the header from a line
		line MUST follow the format: header = key ":" [ value ]
		key is case insensitive
		*/
		void	add_header_field(const std::string&);

		/*
		adds one field to the header from a (key, value) pair
		key argument is case insensitive
		*/
		void	add_header_field(const std::string&, const std::string&);

		/* outputs a string ready to be sent */
		std::string	toString(void) const;

	protected:
		std::string	_start_line;
		header		_headers;
		std::string	_body;
};

std::ostream&	operator<<(std::ostream&, const HttpMessage&);

#endif
