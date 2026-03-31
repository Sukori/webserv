/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   returnCodes.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 18:22:06 by pberset           #+#    #+#             */
/*   Updated: 2026/03/24 18:22:19 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RETURNCODES_HPP
# define RETURNCODES_HPP

bool	needUrlCode(int code);
bool	standaloneCode(int code);

#endif

/*
204:	NO CONTENT

300:	Multiple Choices
301:	Moved Permanently
302:	Found
303:	See Other
304:	Not Modified
305:	Use Proxy
306:	(unused but reserved)
307:	Temporary Redirect
308:	Permanent Redirect
310:	Too many Redirects

400:	Bad Request
401:	Unauthorized
402:	Payment Required
403:	Forbidden
404:	Not Found
405:	Method Not Allowed
406:	Not Acceptable
407:	Proxy Authentication Required
408:	Request Time-out
409:	Conflict
410:	Gone
411:	Length Required
412:	Precondition Failed
413:	Request Entity Too Large
414:	Request-URI Too Long 
415:	Unsupported Media Type
416:	Requested range unsatisfiable
417:	Expectation failed
418:	I'm a teapot
419:	Page expired
421:	Bad mapping
422:	Unprocessable entity 
423:	Locked
424:	Failed Dependency
425:	Too Early
426:	Upgrade Required
427:	Invalid digital signature
428:	Precondition Required
429:	Too Many Requests
431:	Request Header Fields Too Large
449:	Retry With 	Microsoft
450:	Blocked by Windows Parental Controls
451:	Unavailable For Legal Reasons
456:	Unrecoverable Error

500:	Internal Server Error
501:	Not Implemented
502:	Bad Gateway ou Proxy Error
503:	Service Unavailable 
504:	Gateway Time-out
505:	HTTP Version not supported
506:	Variant Also Negotiates
507:	Insufficient storage
508:	Loop detected
509:	Bandwidth Limit Exceeded
510:	Not extended
511:	Network authentication required
*/
