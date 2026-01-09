/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMessage.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:27:00 by pberset           #+#    #+#             */
/*   Updated: 2026/01/06 17:15:40 by ylabussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpMessage.hpp"

const std::string&			HttpMessage::getStartLine(void) const {return _start_line;}
const HttpMessage::header&	HttpMessage::getHeader(void) const {return _headers;}
const std::string&			HttpMessage::getBody(void) const {return _body;}

HttpMessage::MessageType	HttpMessage::getMessageType(void) const {
	if (_start_line.compare(0, 5, "HTTP/") == 0)
		return RESPONSE;
	else if (_start_line.length() > 0)
		return REQUEST;
	else
		return UNDEFINED;
}

bool	HttpMessage::operator==(MessageType type) const {return getMessageType() == type;}
bool	HttpMessage::operator!=(MessageType type) const {return getMessageType() != type;}

void	HttpMessage::setStartLine(const std::string& line) {_start_line = line;}
void	HttpMessage::setBody(const std::string& body) {_body = body;}

HttpMessage::HttpMessage(void) {}
HttpMessage::HttpMessage(const HttpMessage& other): _start_line(other._start_line), _headers(other._headers), _body(other._body) {}
HttpMessage::HttpMessage(const std::string& raw) {
	std::string::const_iterator	it = raw.begin();
	std::string::const_iterator	end = raw.end();
	std::string::const_iterator	tmp;

	/* put first line in _start_line */
	tmp = std::find(it, end, '\n');
	_start_line = std::string(it, tmp);

	/* parse header lines until empty line or end of raw input */
	while (tmp != end)
	{
		it = tmp + 1;
		tmp = std::find(it, end, '\n');
		if (std::distance(it, tmp) == 0)
			break;
		add_header_field(std::string(it, tmp));
	}

	/* put the remaining lines in the message body */
	if (tmp != end)
		_body = std::string(tmp + 1, end);
}

HttpMessage::~HttpMessage(void) {}

HttpMessage&	HttpMessage::operator=(const HttpMessage& other) {
	if (this != &other)
	{
		_start_line = other._start_line;
		_headers = other._headers;
		_body = other._body;
	}
	return *this;
}

void	HttpMessage::add_header_field(const std::string& line) {
	std::string::const_iterator	it = line.begin();
	std::string::const_iterator	end = line.end();
	std::string::const_iterator	sep = std::find(it, end, ':');
	std::string					key (it, sep);
	if (sep == end)
		throw (std::invalid_argument("headers and body MUST be separated by an empty line"));
	/* skip whitespaces per RFC standard definition */
	sep++;
	while (isspace(*sep))
		sep++;
	add_header_field(key, std::string(sep, end));
}

void	HttpMessage::add_header_field(const std::string& key, const std::string& val) {
	std::string key_copy (key);
	/* keys are case insensitive, thus forcing them to lowercase will take care of that */
	std::transform(key.begin(), key.end(), key_copy.begin(), tolower);
	if (_headers.count(key) > 0)
		throw (std::invalid_argument("duplicate header -> " + key));
	else
		_headers.insert(std::make_pair(key,val));
}

std::string	HttpMessage::toString(void) const
{
	std::string s;
	s += _start_line + '\n';
	for (HttpMessage::header::const_iterator it = _headers.begin(); it != _headers.end();it++)
		s += it->first + ": " + it->second + '\n';
	if (_body.length() > 0)
		s += '\n' + _body + '\n';
	return s;
}

std::ostream&	operator<<(std::ostream& os, const HttpMessage& message) {
	return os << message.toString();
}
