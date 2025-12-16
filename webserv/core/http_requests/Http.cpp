/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ylabussi <ylabussi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:27:00 by pberset           #+#    #+#             */
/*   Updated: 2025/12/16 17:22:09 by ylabussi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http.hpp"

Http::Http(void) {
    std::cout << "Default Http constructor" << std::endl;
}
Http::Http(const Http &other) {
    std::cout << "Copy Http constructor" << std::endl;
    //this = &other;
	(void)other;
}

Http::~Http(void) {
    std::cout << "Http destructor" << std::endl;
}

Http    &Http::operator=(const Http &other) {
	std::cout << "Http assignation operator" << std::endl;
	if (this != &other) {
		(void)other;
    // TODO: members
   }
   return (*this);
};









const std::string&			HttpMessage::getStartLine(void) const {return _start_line;}
const HttpMessage::header&	HttpMessage::getHeaders(void) const {return _headers;}
const std::string&			HttpMessage::getBody(void) const {return _body;}

HttpMessage::HttpMessage(void) {}
HttpMessage::HttpMessage(const HttpMessage& other): _start_line(other._start_line), _headers(other._headers), _body(other._body) {}
HttpMessage::HttpMessage(const std::string& raw) {
	std::string::const_iterator	it = raw.begin();
	std::string::const_iterator	end = raw.end();
	std::string::const_iterator	tmp;

	tmp = std::find(it, end, '\n');
	_start_line = std::string(it, tmp);
	while (tmp != end)
	{
		it = tmp + 1;
		tmp = std::find(it, end, '\n');
		if (std::distance(it, tmp) == 0)
			break;
		add_header_field(std::string(it, tmp));
	}
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
	std::string::const_iterator sep = std::find(it, end, ':');
	if (sep == end)
		throw (std::invalid_argument("headers and body MUST be separated by an empty line"));
	if (*(sep + 1) != ' ')
		throw (std::invalid_argument("missing space on line: " + line));
	add_header_field(std::string(it, sep), std::string(sep + 2, end));
	/* TODO												   ^^^
	should check in the standard to see if there is
	a forced space after the ':' in "key: value" headers
	*/
}

void	HttpMessage::add_header_field(const std::string& key, const std::string& val) {
	std::string key_copy (key);
	std::transform(key.begin(), key.end(), key_copy.begin(), tolower);
	if (_headers.count(key) > 0)
		throw (std::invalid_argument("duplicate header -> " + key));
	else
		_headers.insert(std::make_pair(key,val));
}

std::ostream&	operator<<(std::ostream& os, const HttpMessage& message) {
	os << message.getStartLine() << '\n';
	const HttpMessage::header& header = message.getHeaders();
	for (HttpMessage::header::const_iterator it = header.begin(); it != header.end();it++)
		os << '\t' << it->first << ": " << it->second << '\n';
	os << message.getBody() << '\n';
	return os;
}
