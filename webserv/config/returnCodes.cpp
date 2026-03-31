/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   returnCodes.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pberset <pberset@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 18:21:50 by pberset           #+#    #+#             */
/*   Updated: 2026/03/24 18:22:04 by pberset          ###   Lausanne.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "returnCodes.hpp"

bool	needUrlCode(int code) {
	return (code == 300 ||
			code == 301 ||
			code == 302 ||
			code == 303 ||
			code == 307 ||
			code == 308);
}

bool	standaloneCode(int code) {
	return (code == 204 ||
			code == 400 ||
			code == 401 ||
			code == 402 ||
			code == 403 ||
			code == 404 ||
			code == 405 ||
			code == 406 ||
			code == 407 ||
			code == 408 ||
			code == 409 ||
			code == 410 ||
			code == 411 ||
			code == 412 ||
			code == 413 ||
			code == 414 ||
			code == 415 ||
			code == 416 ||
			code == 417 ||
			code == 418 ||
			code == 419 ||
			code == 421 ||
			code == 422 ||
			code == 423 ||
			code == 424 ||
			code == 425 ||
			code == 426 ||
			code == 427 ||
			code == 428 ||
			code == 429 ||
			code == 431 ||
			code == 449 ||
			code == 450 ||
			code == 451 ||
			code == 456 ||
			code == 500 ||
			code == 501 ||
			code == 502 ||
			code == 503 ||
			code == 504 ||
			code == 505 ||
			code == 506 ||
			code == 507 ||
			code == 508 ||
			code == 509 ||
			code == 510 ||
			code == 511);
}
