/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PegException.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 19:06:31 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 11:40:06 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PEGEXCEPTION_HPP
# define PEGEXCEPTION_HPP

# include <stdexcept>
# include <string>

class PegException : public std::runtime_error {

public:

	PegException(const std::string& msg) : std::runtime_error(msg)
	{}
	virtual ~PegException() throw()
	{}
};

#endif

